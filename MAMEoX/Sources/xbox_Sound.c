/**
	* \file			xbox_Sound.c
	* \brief		Implementation of the "Sound" section of
	*           osdepend.h
  * \note     Ported from windows/sound.c
	*/

//= I N C L U D E S ====================================================
#include "osdepend.h"
#include "osd_cpu.h"
#include "driver.h"

#include <crtdbg.h>
#include "DebugLogger.h"



//= D E F I N E S ======================================================
  // Whether or not to enable sound debug messages
#define LOG_SOUND

  // The number of audio updates to ignore underflow/overflows before
  //  reporting them
#define IGNORE_UNDERFLOW_FRAMES	100

  // the local buffer is what the stream buffer feeds from
  // note that this needs to be large enough to buffer at frameskip 11
  // for 30fps games like Tapper; we will scale the value down based
  // on the actual framerate of the game
#define MAX_BUFFER_SIZE			(128 * 1024)

  // this is the maximum number of extra samples we will ask for
  // per frame (I know this looks like a lot, but most of the
  // time it will generally be nowhere close to this)
#define MAX_SAMPLE_ADJUST		16

  //! The number of frames where we're in the [g_lowerThresh,g_upperThresh]
  //!  range before we reset the g_currentAdjustment value to 0
#define NUM_TARGET_FRAMES_BEFORE_ADJUST_RESET     10

//= G L O B A L = V A R S ==============================================

  // Defined someplace in the DirectSound subsystem, setting this to
  //  TRUE disables some warnings which are meaningless to us
extern BOOL                   g_fDirectSoundDisableBusyWaitWarning;

static INT32									g_attenuation = 0;

  // DirectSound objects
static LPDIRECTSOUND8					g_pDSound = NULL;

  // sound buffers
static LPDIRECTSOUNDBUFFER8		g_pStreamBuffer = NULL;
static UINT32									g_streamBufferSize = 0;
static UINT32									g_streamBufferWriteCursor = 0;
static INT8                   *g_streamBufferData = NULL;

  // descriptors and formats
static DSBUFFERDESC			      g_streamDesc = {0};
static WAVEFORMATEX			      g_streamFormat = {0};


// buffer over/underflow counts
static INT32									g_totalFrames = 0;
static INT32									g_bufferUnderflows = 0;
static INT32									g_bufferOverflows = 0;

	// global sample tracking
static DOUBLE									g_samplesPerFrame = 0.0;
static DOUBLE									g_samplesLeftOver = 0.0;
static UINT32									g_samplesThisFrame = 0;

	// sample rate adjustments
static INT32									g_currentAdjustment = 0;
static INT32									g_lowerThresh = 0;
static INT32									g_upperThresh = 0;

static BOOL                   g_soundPlaying = FALSE;


//= P R O T O T Y P E S ================================================
static BOOL Helper_DirectSoundInitialize( void );
static void Helper_DirectSoundTerminate( void );

static BOOL Helper_DirectSoundCreateBuffers( void );
static void Helper_DirectSoundDestroyBuffers( void );


static __inline UINT32 Helper_BytesInStreamBuffer( void );
static void Helper_UpdateSampleAdjustment( void );
static void Helper_CopySampleData( INT16 *data, UINT32 bytes_to_copy );
static void Helper_ClearSampleData( UINT32 amountToClear );

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	osd_start_audio_stream
//---------------------------------------------------------------------
INT32 osd_start_audio_stream( INT32 stereo )
{
  //osd_start_audio_stream() is called at the start of the emulation to initialize
  //the output stream, then osd_update_audio_stream() is called every frame to
  //feed new data. osd_stop_audio_stream() is called when the emulation is stopped.

  //The sample rate is fixed at Machine->sample_rate. Samples are 16-bit, signed.
  //When the stream is stereo, left and right samples are alternated in the
  //stream.

  //osd_start_audio_stream() and osd_update_audio_stream() must return the number
  //of samples (or couples of samples, when using stereo) required for next frame.
  //This will be around Machine->sample_rate / Machine->drv->frames_per_second,
  //the code may adjust it by SMALL AMOUNTS to keep timing accurate and to
  //maintain audio and video in sync when using vsync. Note that sound emulation,
  //especially when DACs are involved, greatly depends on the number of samples
  //per frame to be roughly constant, so the returned value must always stay close
  //to the reference value of Machine->sample_rate / Machine->drv->frames_per_second.
  //Of course that value is not necessarily an INT32eger so at least a +/- 1
  //adjustment is necessary to avoid drifting over time.

    // Disable the XBOX's WaitForVoiceOff warning
  g_fDirectSoundDisableBusyWaitWarning = TRUE;
  g_totalFrames = 0;
  g_bufferUnderflows = 0;
  g_bufferOverflows = 0;
  g_streamBufferWriteCursor = 0;


	// determine the number of samples per frame
	g_samplesPerFrame = (DOUBLE)Machine->sample_rate / (DOUBLE)Machine->drv->frames_per_second;
  _RPT1( _CRT_WARN, "Samples per frame: %f\n", g_samplesPerFrame );
  _RPT1( _CRT_WARN, "Consumed per frame: %f\n", (DOUBLE)Machine->sample_rate / 60.0 );


g_samplesPerFrame = (DOUBLE)Machine->sample_rate / 60.0;

 
	// compute how many samples to generate the first frame
	g_samplesLeftOver = g_samplesPerFrame;
	g_samplesThisFrame = (UINT32)g_samplesLeftOver;
	g_samplesLeftOver -= (DOUBLE)g_samplesThisFrame;

	  // skip if sound disabled
	if( Machine->sample_rate )
	{
			// Initialize direct sound
		if( !Helper_DirectSoundInitialize() )
			return 0;

		// set the startup volume
		osd_set_mastervolume( g_attenuation );
	}


	// return the samples to play the first frame
	return g_samplesLeftOver;
}

//---------------------------------------------------------------------
//	osd_stop_audio_stream
//---------------------------------------------------------------------
void osd_stop_audio_stream( void )
{

	// if nothing to do, don't do it
	if( !Machine->sample_rate )
		return;

		// kill the buffers and g_pDSound
  //Helper_DirectSoundDestroyBuffers(); // Done by Helper_DirectSoundTerminate
	Helper_DirectSoundTerminate();

  #ifdef LOG_SOUND
    _RPT2( _CRT_WARN, "Sound buffer: overflows=%d underflows=%d\n", g_bufferOverflows, g_bufferUnderflows );
  #endif
}



//---------------------------------------------------------------------
//	osd_update_audio_stream
//---------------------------------------------------------------------
INT32 osd_update_audio_stream( INT16 *buffer )
{

		// if nothing to do, don't do it
	if( Machine->sample_rate && g_pStreamBuffer)
	{
    if( !g_soundPlaying )
    {
        // Reset the play position = 0
      IDirectSoundBuffer_SetCurrentPosition( g_pStreamBuffer, 0 );
      g_soundPlaying = TRUE;
    }
		  // update the sample adjustment
		Helper_UpdateSampleAdjustment();

		  // copy data into the sound buffer
		Helper_CopySampleData( buffer, g_samplesThisFrame * g_streamFormat.nBlockAlign );
    Helper_ClearSampleData( g_samplesThisFrame * g_streamFormat.nBlockAlign );

    #ifdef LOG_SOUND
	      // reset underflow/overflow tracking
	    if( ++g_totalFrames == IGNORE_UNDERFLOW_FRAMES )
		    g_bufferOverflows = g_bufferUnderflows = 0;
      else if( g_totalFrames > IGNORE_UNDERFLOW_FRAMES )
        g_totalFrames = IGNORE_UNDERFLOW_FRAMES + 1;

      {
        static int prev_overflows = 0, prev_underflows = 0;
        if( g_totalFrames > IGNORE_UNDERFLOW_FRAMES && (g_bufferOverflows != prev_overflows || g_bufferUnderflows != prev_underflows) )
        {
	        prev_overflows = g_bufferOverflows;
	        prev_underflows = g_bufferUnderflows;
          _RPTF2( _CRT_WARN, "************************ overflows=%d underflows=%d\n", g_bufferOverflows, g_bufferUnderflows );
        }
      }
    #endif
	}

	  // compute how many samples to generate next frame
	g_samplesLeftOver += g_samplesPerFrame;
	g_samplesThisFrame = (UINT32)g_samplesLeftOver;
	g_samplesLeftOver -= (DOUBLE)g_samplesThisFrame;

	g_samplesThisFrame += g_currentAdjustment;

	  // return the samples to play this next frame
	return g_samplesThisFrame;
}

//---------------------------------------------------------------------
//	osd_set_mastervolume
//---------------------------------------------------------------------
void osd_set_mastervolume( INT32 attenuation )
{
 // control master volume. attenuation is the attenuation in dB (a negative
 // number). To convert from dB to a linear volume scale do the following:
	//volume = MAX_VOLUME;
	//while (attenuation++ < 0)
	//	volume /= 1.122018454;		//	= (10 ^ (1/20)) = 1dB

		// clamp the attenuation to 0-32 range
	g_attenuation = attenuation;
  if( g_attenuation > 0 )
    g_attenuation = 0;
  else if( g_attenuation < -32 )
    g_attenuation = -32;

	// set the master volume
	if( g_pStreamBuffer )
		IDirectSoundBuffer_SetVolume( g_pStreamBuffer, g_attenuation * 100 );
}

//---------------------------------------------------------------------
//	osd_get_mastervolume
//---------------------------------------------------------------------
INT32 osd_get_mastervolume( void )
{
	return g_attenuation;
}

//---------------------------------------------------------------------
//	osd_sound_enable
//---------------------------------------------------------------------
void osd_sound_enable( INT32 enable )
{
	if( g_pStreamBuffer )
	{
		if( enable )
			IDirectSoundBuffer_SetVolume(g_pStreamBuffer, g_attenuation * 100 );
		else
			IDirectSoundBuffer_SetVolume(g_pStreamBuffer, DSBVOLUME_MIN);
	}
}








//---------------------------------------------------------------------
//	Helper_DirectSoundInitialize
//---------------------------------------------------------------------
static BOOL Helper_DirectSoundInitialize( void )
{
	HRESULT hr;

	if( !g_pDSound && (hr = DirectSoundCreate( NULL, &g_pDSound, NULL )) != S_OK )
	{
    _RPT1( _CRT_WARN, "DirectSoundCreate failed: 0x%X!", hr );
		PRINTMSG( T_ERROR, "DirectSoundCreate failed: 0x%X!", hr );
		return FALSE;
	}

		// make a format description for what we want
	g_streamFormat.wFormatTag			= WAVE_FORMAT_PCM;
	g_streamFormat.nChannels			= (Machine->drv->sound_attributes & SOUND_SUPPORTS_STEREO) ? 2 : 1;
	g_streamFormat.nSamplesPerSec	= Machine->sample_rate;
	g_streamFormat.wBitsPerSample	= 16;
	g_streamFormat.nBlockAlign		= (g_streamFormat.wBitsPerSample * g_streamFormat.nChannels) >> 3;
	g_streamFormat.nAvgBytesPerSec	= g_streamFormat.nSamplesPerSec * g_streamFormat.nBlockAlign;

		// compute the buffer sizes
	g_streamBufferSize = ((UINT64)MAX_BUFFER_SIZE * (UINT64)g_streamFormat.nSamplesPerSec) / 44100;
	g_streamBufferSize = (g_streamBufferSize * g_streamFormat.nBlockAlign) / 4;
	g_streamBufferSize = (g_streamBufferSize * 30) / Machine->drv->frames_per_second;
	g_streamBufferSize = (g_streamBufferSize / 1024) * 1024;  // Drop remainder
  g_streamBufferSize *= 2;

		// compute the upper/lower thresholds
	g_lowerThresh = g_streamBufferSize / 5;
	g_upperThresh = (g_streamBufferSize << 1) / 5;
	
  #ifdef LOG_SOUND
	  _RPTF2( _CRT_WARN, "stream_buffer_size = %d (max %d)\n", g_streamBufferSize, MAX_BUFFER_SIZE );
	  _RPTF1( _CRT_WARN, "lower_thresh = %d\n", g_lowerThresh);
	  _RPTF1( _CRT_WARN, "upper_thresh = %d\n", g_upperThresh);
  #endif

		// create the buffers
	if( !Helper_DirectSoundCreateBuffers() )
	{
		IDirectSound_Release( g_pDSound );
		g_pDSound = NULL;
		return FALSE;
	}

	return TRUE;
}



//---------------------------------------------------------------------
//	Helper_DirectSoundTerminate
//---------------------------------------------------------------------
static void Helper_DirectSoundTerminate( void )
{
	Helper_DirectSoundDestroyBuffers();

		// release the object
	if( g_pDSound )
		IDirectSound_Release( g_pDSound );
	g_pDSound = NULL;
}



//---------------------------------------------------------------------
//	Helper_DirectSoundCreateBuffers
//---------------------------------------------------------------------
static BOOL Helper_DirectSoundCreateBuffers( void )
{
	HRESULT result;

    // If size is 0, we don't need a buffer
  if( !g_streamBufferSize )
  {
    _RPT0( _CRT_WARN, "g_streamBufferSize == 0 in Helper_DirectSoundCreateBuffers!\n" );
    return TRUE;
  }

	  // create a buffer desc for the stream buffer
  memset( &g_streamDesc, 0, sizeof(g_streamDesc) );
	g_streamDesc.dwSize         = sizeof(g_streamDesc);
	g_streamDesc.dwFlags        = DSBCAPS_CTRLPOSITIONNOTIFY;
	g_streamDesc.dwBufferBytes  = 0;
	g_streamDesc.lpwfxFormat    = &g_streamFormat;

	  // create the stream buffer
	if( (result = IDirectSound_CreateSoundBuffer( g_pDSound, &g_streamDesc, &g_pStreamBuffer, NULL )) != S_OK )
	{
    osd_print_error( "Error creating DirectSound buffer: 0x%X\n", result );
	  g_pStreamBuffer = NULL;
    return FALSE;
	}


    // Allocate the stream buffer
  if( g_streamBufferData )
    free( g_streamBufferData );
  
  if( !(g_streamBufferData = (INT8*)calloc( 1, g_streamBufferSize )) )
  {
    _RPT0( _CRT_WARN, "Failed to allocate streambuffer!\n" );
    PRINTMSG( T_ERROR, "Failed to allocate streambuffer!" );
	  IDirectSoundBuffer_Release( g_pStreamBuffer );
	  g_pStreamBuffer = NULL;
    return FALSE;
  }

  IDirectSoundBuffer_SetBufferData( g_pStreamBuffer, g_streamBufferData, g_streamBufferSize );
  g_streamBufferWriteCursor = 0;

	if( (result = IDirectSoundBuffer_Play( g_pStreamBuffer, 0, 0, DSBPLAY_LOOPING )) != S_OK )
	{
		PRINTMSG( T_ERROR, "IDirectSoundBuffer_Play failed! 0x%X", result );
    free( g_streamBufferData );
    g_streamBufferData = NULL;
	  IDirectSoundBuffer_Release( g_pStreamBuffer );
	  g_pStreamBuffer = NULL;
		return FALSE;
	}

	return TRUE;
}



//---------------------------------------------------------------------
//	Helper_DirectSoundDestroyBuffers
//---------------------------------------------------------------------
static void Helper_DirectSoundDestroyBuffers( void )
{
	// stop any playback
	if( g_pStreamBuffer )
	{
		IDirectSoundBuffer_Stop( g_pStreamBuffer );
		IDirectSoundBuffer_Release( g_pStreamBuffer );
    if( g_streamBufferData )
    {
      free( g_streamBufferData );
      g_streamBufferData = NULL;
    }
    g_streamBufferWriteCursor = 0;  // Reset the write cursor to the head of the buffer
    g_soundPlaying = FALSE;
	}
	g_pStreamBuffer = NULL;
}


//---------------------------------------------------------------------
//	Helper_BytesInStreamBuffer
//---------------------------------------------------------------------
static __inline UINT32 Helper_BytesInStreamBuffer( void )
{
	DWORD play_position;
	if( IDirectSoundBuffer_GetCurrentPosition(  g_pStreamBuffer, 
																						  &play_position, 
																							NULL ) != S_OK )
  {
    return 0;
  }
  
  //#ifdef LOG_SOUND
  //  _RPT2( _CRT_WARN, "WCursor: %-6.6lu, PlayPos: %-6.6lu\n", g_streamBufferWriteCursor, play_position );
  //#endif

    // Case 1: Write cursor is past the play_position, so the valid area is
    //          simply between the two
	if( g_streamBufferWriteCursor >= play_position )
		return g_streamBufferWriteCursor - play_position;

    // Case 2: Write cursor has wrapped around in the circular buffer. Valid 
    //          area is the distance between the end of the buffer and 
    //          play_position, + the position of the write cursor
	return (g_streamBufferSize - play_position) + g_streamBufferWriteCursor;
}


//---------------------------------------------------------------------
//	Helper_UpdateSampleAdjustment
//---------------------------------------------------------------------
static void Helper_UpdateSampleAdjustment( void )
{
	static UINT32 consecutive_lows = 0;
	static UINT32 consecutive_mids = 0;
	static UINT32 consecutive_highs = 0;
  UINT32        buffered = Helper_BytesInStreamBuffer(); 
buffered = g_lowerThresh;

  #ifdef LOG_SOUND
    _RPT1( _CRT_WARN, "Helper_UpdateSampleAdjustment: %d buffered\n", buffered );
  #endif

	if( buffered < g_lowerThresh )
	{
		  // we have too few samples in the buffer
		if( ++consecutive_lows > MAX_SAMPLE_ADJUST )
      consecutive_lows = MAX_SAMPLE_ADJUST;
		consecutive_mids = 0;
		consecutive_highs = 0;

		  // adjust so that we generate more samples per frame to compensate
		g_currentAdjustment = (consecutive_lows < MAX_SAMPLE_ADJUST) ? consecutive_lows : MAX_SAMPLE_ADJUST;

    #ifdef LOG_SOUND
      _RPTF1( _CRT_WARN, "too low - adjusting to %d\n", g_currentAdjustment );
    #endif
	}
	else if( buffered > g_upperThresh )
	{
			// we have too many samples in the buffer
		consecutive_lows = 0;
		consecutive_mids = 0;
		if( ++consecutive_highs > MAX_SAMPLE_ADJUST )
      consecutive_highs = MAX_SAMPLE_ADJUST;

		  // adjust so that we generate fewer samples per frame to compensate
		g_currentAdjustment = (consecutive_highs < MAX_SAMPLE_ADJUST) ? -consecutive_highs : -MAX_SAMPLE_ADJUST;
		
    #ifdef LOG_SOUND
    _RPTF1( _CRT_WARN, "too high - adjusting to %d\n", g_currentAdjustment );
    #endif
	}
	else
	{
			// We have the right number of samples in the buffer

		consecutive_lows = 0;
		if( ++consecutive_mids > NUM_TARGET_FRAMES_BEFORE_ADJUST_RESET )
      consecutive_mids = NUM_TARGET_FRAMES_BEFORE_ADJUST_RESET + 1;
		consecutive_highs = 0;

		  // after 10 or so of these, revert back to no adjustment
		if( consecutive_mids >= NUM_TARGET_FRAMES_BEFORE_ADJUST_RESET && g_currentAdjustment )
    {
			g_currentAdjustment = 0;

      #ifdef LOG_SOUND
			  _RPTF1( _CRT_WARN, "%d consecutive_mids - resetting adjustment\n", consecutive_mids );
      #endif
    }
	}
}



//---------------------------------------------------------------------
//	Helper_CopySampleData
//---------------------------------------------------------------------
static void Helper_CopySampleData( INT16 *data, UINT32 totalToCopy )
{	
  UINT32 length1 = 0, length2 = 0;
  void *buffer1 = NULL, *buffer2 = NULL;
  UINT32 bytesToCopy = 0;
  DWORD playCursor, writeCursor;
  HRESULT result = IDirectSoundBuffer_GetCurrentPosition( g_pStreamBuffer, &playCursor, &writeCursor );
  _RPT3( _CRT_WARN, "R: %-8.8lu W: %-8.8lu SB: %-8.8lu\n", playCursor, writeCursor, g_streamBufferWriteCursor );

  if( (g_streamBufferWriteCursor > writeCursor && g_streamBufferWriteCursor - writeCursor > 10) || 
      (g_streamBufferWriteCursor < writeCursor && writeCursor - g_streamBufferWriteCursor > 10 ) )
      g_streamBufferWriteCursor = writeCursor;

    // 1) If playCursor < g_streamBufferWriteCursor < writeCursor, an underflow has ocurred
  if( playCursor < g_streamBufferWriteCursor && g_streamBufferWriteCursor < writeCursor )
  {
    ++g_bufferUnderflows;
    g_streamBufferWriteCursor = writeCursor;
  }

    // Buffer1 will always start at the write cursor position
  buffer1 = g_streamBufferData + g_streamBufferWriteCursor; 

    // 2) Figure out where we should start copying data
  if( playCursor <= g_streamBufferWriteCursor )
  {
      // We can write until the end of the buffer, and then from the beginning
      // of the buffer up to the playCursor
    length1 = g_streamBufferSize - g_streamBufferWriteCursor;

    length2 = playCursor ? playCursor - 1 : 0;
    buffer2 = g_streamBufferData;
  }
  else
  {
      // We can write up to the playCursor
    length1 = playCursor - g_streamBufferWriteCursor;
  }

    // Check for an overflow
  if( length1 + length2 < totalToCopy )
  {
    ++g_bufferOverflows; 
    totalToCopy = length1 + length2;
  }

	  // adjust the write cursor, wrapping around the end of the buffer if necessary
	writeCursor = (g_streamBufferWriteCursor + totalToCopy) % g_streamBufferSize;

	  // copy the first chunk
	bytesToCopy = (totalToCopy > length1) ? length1 : totalToCopy;
	memcpy( buffer1, data, bytesToCopy );
	totalToCopy -= bytesToCopy;
	
	  // copy the second chunk, if necessary
	if( totalToCopy )
	{
      // Advance past the data that was already copied
	  data = (INT16*)((UINT8*)data + bytesToCopy);
		memcpy( buffer2, data, totalToCopy );       // Overflow was handled above, so using totalToCopy will be safe
	}
  g_streamBufferWriteCursor = writeCursor; 

}



//---------------------------------------------------------------------
//	Helper_ClearSampleData
//---------------------------------------------------------------------
static void Helper_ClearSampleData( UINT32 amountToClear )
{	
  UINT32 length1 = 0, length2 = 0;
  void *buffer1 = NULL, *buffer2 = NULL;
  UINT32 bytesToCopy = 0;
  DWORD playCursor, writeCursor;
  HRESULT result = IDirectSoundBuffer_GetCurrentPosition( g_pStreamBuffer, &playCursor, &writeCursor );

    // Buffer1 will always start at the write cursor position
  buffer1 = g_streamBufferData + g_streamBufferWriteCursor; 

    // 2) Figure out where we should start copying data
  if( playCursor <= g_streamBufferWriteCursor )
  {
      // We can write until the end of the buffer, and then from the beginning
      // of the buffer up to the playCursor
    length1 = g_streamBufferSize - g_streamBufferWriteCursor;

    length2 = playCursor ? playCursor - 1 : 0;
    buffer2 = g_streamBufferData;
  }
  else
  {
      // We can write up to the playCursor
    length1 = playCursor - g_streamBufferWriteCursor;
  }

    // Check for an overflow
  if( length1 + length2 < amountToClear )
  {
    amountToClear = length1 + length2;
  }

	  // copy the first chunk
	bytesToCopy = (amountToClear > length1) ? length1 : amountToClear;
	memset( buffer1, 0, bytesToCopy );
	amountToClear -= bytesToCopy;
	
	  // copy the second chunk, if necessary
	if( amountToClear )
	{
      // Advance past the data that was already copied
		memset( buffer2, 0, amountToClear );       // Overflow was handled above, so using totalToCopy will be safe
	}
}



