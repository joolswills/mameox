/**
	* \file			xbox_JoystickMouse.c
	* \brief		Implementation of the "Joystick & Mouse/Trackball" section of
	*           osdepend.h
  *
  * \note     Based off of XMAME source
	*/

//= I N C L U D E S ====================================================
#include <Xtl.h>
#include <assert.h>
#include <stdio.h>

#include "osd_cpu.h"
#include "osdepend.h"
#include "inptport.h"

#include "MAMEoX.h"
#include "DebugLogger.h"

//= D E F I N E S ======================================================

  //! The value of the analog axis must exceed this deadzone
  //!  when being treated as a digital control (valid values are from [0,32767])
#define ANALOG_AS_DIGITAL_DEADZONE          (32768 * 0.33f)
#define ANALOG_BUTTON_AS_DIGITAL_DEADZONE   16

	// Analog stick deadzone
#define ANALOG_DEADZONE_PERCENTAGE      0.15f
#define ANALOG_DEADZONE				          (INT32)(32768.0f * ANALOG_DEADZONE_PERCENTAGE)
#define DEADZONE_RECTIFIER			        1.0f / (1.0f - ANALOG_DEADZONE_PERCENTAGE)

	// macros for building/mapping keycodes
#define JOYCODE(joy, type, index)	      ((index) | ((type) << 8) | ((joy) << 12))
#define JOYINDEX(joycode)			          ((joycode) & 0xff)
#define JT(joycode)			                (((joycode) >> 8) & 0xf)
#define JOYNUM(joycode)				          (((joycode) >> 12) & 0xf)

  // Used to set the temporary name variable in InitializeJoystickMouse
#define JOYNAME( _string__ )  sprintf( name, "J%d %s", stickIndex + 1, _string__ )

  // Helper macro, calls Helper_AddEntry in the InitializeJoystickMouse function
#define ADDENTRY( _name__, _code__, _standardCode__ )    JOYNAME( _name__ ); Helper_AddEntry( name, (_code__), (_standardCode__), &joycount )

#define STDCODE( cde )    (stickIndex == 0 ? JOYCODE_1_##cde : (stickIndex == 1 ? JOYCODE_2_##cde : (stickIndex == 2 ? JOYCODE_3_##cde : JOYCODE_4_##cde )))

  //! The total number of JoystickInfo structures (= the total number of mapped buttons/axes on all joysticks)
#define MAX_JOYSTICKINFO_ENTRIES      128

  //! Macros for redefining input sequences
#define REMAP_SEQ_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) { InputSeq newSeq = SEQ_DEF_16((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)   { InputSeq newSeq = SEQ_DEF_15((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n)     { InputSeq newSeq = SEQ_DEF_14((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_13(a,b,c,d,e,f,g,h,i,j,k,l,m)       { InputSeq newSeq = SEQ_DEF_13((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_12(a,b,c,d,e,f,g,h,i,j,k,l)         { InputSeq newSeq = SEQ_DEF_12((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_11(a,b,c,d,e,f,g,h,i,j,k)           { InputSeq newSeq = SEQ_DEF_11((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_10(a,b,c,d,e,f,g,h,i,j)             { InputSeq newSeq = SEQ_DEF_10((a),(b),(c),(d),(e),(f),(g),(h),(i),(j)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_9(a,b,c,d,e,f,g,h,i)                { InputSeq newSeq = SEQ_DEF_9((a),(b),(c),(d),(e),(f),(g),(h),(i)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_8(a,b,c,d,e,f,g,h)                  { InputSeq newSeq = SEQ_DEF_8((a),(b),(c),(d),(e),(f),(g),(h)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_7(a,b,c,d,e,f,g)                    { InputSeq newSeq = SEQ_DEF_7((a),(b),(c),(d),(e),(f),(g)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_6(a,b,c,d,e,f)                      { InputSeq newSeq = SEQ_DEF_6((a),(b),(c),(d),(e),(f)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_5(a,b,c,d,e)                        { InputSeq newSeq = SEQ_DEF_5((a),(b),(c),(d),(e)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_4(a,b,c,d)                          { InputSeq newSeq = SEQ_DEF_4((a),(b),(c),(d)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_3(a,b,c)                            { InputSeq newSeq = SEQ_DEF_3((a),(b),(c)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_2(a,b)                              { InputSeq newSeq = SEQ_DEF_2((a),(b)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_1(a)                                { InputSeq newSeq = SEQ_DEF_1((a)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_0()				                          { InputSeq newSeq = SEQ_DEF_0; memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }

  //! Macros for converting non-standard internal axis/button codes to MAME ones 
  //!  (only use them for things that are not mapped to an entry in input.h)
#define AXISCODE( _stick__, _axis__ )   joyoscode_to_code( JOYCODE( _stick__, _axis__, 0 ) )
#define BUTTONCODE( _stick__, _ID__ )   joyoscode_to_code( JOYCODE( _stick__, JT_BUTTON, _ID__ ) )


  //! \enum       XBOXJoyType
	//! \brief      Defines a logical type for every available button/axis
typedef enum XBOXJoyType {
	JT_LSTICK_UP = 0,
	JT_LSTICK_DOWN,
	JT_LSTICK_LEFT,
	JT_LSTICK_RIGHT,
	JT_RSTICK_UP,
	JT_RSTICK_DOWN,        // 5
	JT_RSTICK_LEFT,
	JT_RSTICK_RIGHT,
	JT_DPAD_UP,
	JT_DPAD_DOWN,
	JT_DPAD_LEFT,          // 10
	JT_DPAD_RIGHT,
	JT_BUTTON
} XBOXJoyType;

typedef enum ButtonID {
  BUTTON_A = 0,
  BUTTON_X,
  BUTTON_B,
  BUTTON_Y,
  BUTTON_LEFT_TRIGGER,
  BUTTON_RIGHT_TRIGGER,
  BUTTON_START,
  BUTTON_BACK,
  BUTTON_WHITE,
  BUTTON_BLACK,           
  BUTTON_LA,              // Left analog pressed
  BUTTON_RA               // Right analog pressed
};

//= G L O B A L = V A R S ==============================================
static struct JoystickInfo			g_joystickInfo[MAX_JOYSTICKINFO_ENTRIES] = {0};
static UINT32										g_numOSDInputKeywords = 0;
static BOOL                     g_systemInitialized = FALSE;

static UINT32                   g_calibrationStep = 0;
static UINT32                   g_calibrationJoynum = 0;

  //! Whether or not the "STARTUP" XBE section has been unloaded
static BOOL                     g_STARTUPSectionUnloaded = FALSE;

//= P R O T O T Y P E S ================================================
	//------------------------------------------------------------------
	//	Helper_AddEntry
  //! \brief    Adds an entry into the g_joystickInfo array
  //!
  //! \param    name - The user friendly name of the entry
  //! \param    code - The OSD-specific identifier of the entry
  //! \param    standardCode - The "standard" input code of the entry (see input.h in the MAME tree)
  //! \param    joycount - [IN,OUT] The index in the joystickInfo array to modify. Incremented on success
  //!
  //! \note     standardCode may be set to a value obtained via joyoscode_to_code for any codes 
  //!           w/out a specific entry in the keycode/joystick enum (defined in input.h of the MAME tree)
	//------------------------------------------------------------------
static void Helper_AddEntry( const char *name, INT32 code, INT32 standardCode, INT32 *joycount );

static void Helper_CustomizeInputPortDefaults( struct ipd *defaults );
void InitializeJoystickMouse( void );


//= F U N C T I O N S ==================================================


/*
//---------------------------------------------------------------------
//	TerminateJoystickMouse
//---------------------------------------------------------------------
void TerminateJoystickMouse( void )
{
  UINT32 i = 0;
  while( g_joystickInfo[i].name )
  {
    free( g_joystickInfo[i].name );
    g_joystickInfo[i].name = NULL;
    ++i;
  }

  g_systemInitialized = FALSE;
}
*/

//---------------------------------------------------------------------
//	osd_get_joy_list
//---------------------------------------------------------------------
const struct JoystickInfo *osd_get_joy_list( void )
{
  //	return a list of all available joystick inputs (see input.h)
	return g_joystickInfo;
}

//---------------------------------------------------------------------
//	osd_is_joy_pressed
//---------------------------------------------------------------------
int osd_is_joy_pressed( int joycode )
{
	/*
		tell whether the specified joystick direction/button is pressed or not.
		joycode is the OS dependant code specified in the list returned by
		osd_get_joy_list().
	*/

	INT joyindex = JOYINDEX(joycode);
	INT joytype = JT(joycode);
	INT joynum = JOYNUM(joycode);

	const XINPUT_GAMEPAD *gamepad;
	
//	PRINTMSG( T_TRACE, "osd_is_joy_pressed" );
	
	gamepad = GetGamepadState( joynum );
	if( !gamepad )
	{
//		PRINTMSG( T_ERROR, "Invalid joynum %lu!", joynum );
		return 0;
	}


	switch( joytype )
	{
		case JT_BUTTON:
			switch( joyindex )
			{
				case BUTTON_A:
					return (gamepad->bAnalogButtons[XINPUT_GAMEPAD_A] > ANALOG_BUTTON_AS_DIGITAL_DEADZONE);
				case BUTTON_X:
					return (gamepad->bAnalogButtons[XINPUT_GAMEPAD_X] > ANALOG_BUTTON_AS_DIGITAL_DEADZONE);
				case BUTTON_B:
					return (gamepad->bAnalogButtons[XINPUT_GAMEPAD_B] > ANALOG_BUTTON_AS_DIGITAL_DEADZONE);
				case BUTTON_Y:
					return (gamepad->bAnalogButtons[XINPUT_GAMEPAD_Y] > ANALOG_BUTTON_AS_DIGITAL_DEADZONE);
				case BUTTON_LEFT_TRIGGER:
					return (gamepad->bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > ANALOG_BUTTON_AS_DIGITAL_DEADZONE);
				case BUTTON_RIGHT_TRIGGER:
					return (gamepad->bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > ANALOG_BUTTON_AS_DIGITAL_DEADZONE);
				case BUTTON_START:
					return (gamepad->wButtons & XINPUT_GAMEPAD_START);
				case BUTTON_BACK:
					return (gamepad->wButtons & XINPUT_GAMEPAD_BACK);
				case BUTTON_WHITE:
					return (gamepad->bAnalogButtons[XINPUT_GAMEPAD_WHITE] > ANALOG_BUTTON_AS_DIGITAL_DEADZONE);
				case BUTTON_BLACK:
					return (gamepad->bAnalogButtons[XINPUT_GAMEPAD_BLACK] > ANALOG_BUTTON_AS_DIGITAL_DEADZONE);
				case BUTTON_LA:
					return (gamepad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
				case BUTTON_RA:
					return (gamepad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
			}
			break;

		case JT_DPAD_UP:
			return (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
		case JT_DPAD_DOWN:
			return (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		case JT_DPAD_LEFT:
			return (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		case JT_DPAD_RIGHT:
			return (gamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

		case JT_LSTICK_UP:
			return (gamepad->sThumbLY > ANALOG_AS_DIGITAL_DEADZONE );
		case JT_LSTICK_DOWN:
			return (gamepad->sThumbLY < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_LSTICK_LEFT:
			return (gamepad->sThumbLX < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_LSTICK_RIGHT:
			return (gamepad->sThumbLX > ANALOG_AS_DIGITAL_DEADZONE );

		case JT_RSTICK_UP:
			return (gamepad->sThumbRY > ANALOG_AS_DIGITAL_DEADZONE );
		case JT_RSTICK_DOWN:
			return (gamepad->sThumbRY < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_RSTICK_LEFT:
			return (gamepad->sThumbRX < -ANALOG_AS_DIGITAL_DEADZONE );
		case JT_RSTICK_RIGHT:
			return (gamepad->sThumbRX > ANALOG_AS_DIGITAL_DEADZONE );

	}
	
	return 0;
}


//---------------------------------------------------------------------
//	osd_is_joystick_axis_code
//---------------------------------------------------------------------
int osd_is_joystick_axis_code( int joycode )
{
/* added for building joystick seq for analog inputs */
//	PRINTMSG( T_TRACE, "osd_is_joystick_axis_code" );

	switch( JT( joycode ) )
	{
	case JT_LSTICK_UP:
	case JT_LSTICK_DOWN:
	case JT_LSTICK_LEFT:
	case JT_LSTICK_RIGHT:
	case JT_RSTICK_UP:
	case JT_RSTICK_DOWN:
	case JT_RSTICK_LEFT:
	case JT_RSTICK_RIGHT:
			return 1;

  case JT_BUTTON:
    {
        // Most of the XBOX buttons are actually
        // analog controls
      switch( JOYINDEX( joycode ) )
      {
      case BUTTON_A:
      case BUTTON_X:
      case BUTTON_B:
      case BUTTON_Y:
      case BUTTON_LEFT_TRIGGER:
      case BUTTON_RIGHT_TRIGGER:
      case BUTTON_WHITE:
      case BUTTON_BLACK:
        return 1;
      }
    }
    break;

  }
	return 0;
}

//---------------------------------------------------------------------
//	osd_joystick_needs_calibration
//---------------------------------------------------------------------
int osd_joystick_needs_calibration( void )
{
/* Joystick calibration routines BW 19981216 */
/* Do we need to calibrate the joystick at all? */

  // Disabled for now, calibration should be done
  // via the vastly superior LightgunCalibrator in
  // the ROM listing.
/*
  const XINPUT_CAPABILITIES *gp;
  UINT32 i = 0;

    // Search for subtype 'P' devices (guns)
  for( ; i < 4; ++i )
  {
    gp = GetGamepadCaps( 0 );  
    if( gp && gp->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
      return 1;
  }
*/
  return 0;
}


//---------------------------------------------------------------------
//	osd_joystick_start_calibration
//---------------------------------------------------------------------
void osd_joystick_start_calibration( void )
{
/* Preprocessing for joystick calibration. Returns 0 on success */
  const XINPUT_CAPABILITIES *gp;
  UINT32 i = 0;

  g_calibrationStep = 0;

    // Search for the first connected gun
  for( ; i < 4; ++i )
  {
    gp = GetGamepadCaps( 0 );  
    if( gp && gp->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
    {
      g_calibrationJoynum = i;
      return;
    }
  }
}

//---------------------------------------------------------------------
//	osd_joystick_calibrate_next
//---------------------------------------------------------------------
const char *osd_joystick_calibrate_next( void )
{
/* Prepare the next calibration step. Return a description of this step. */
/* (e.g. "move to upper left") */
  static char retString[128];

    // When we hit 2, switch over to the next gun to be calibrated,
    //  or return NULL to exit the process
  if( g_calibrationStep == 2 )
  {
    const XINPUT_CAPABILITIES *gp;
    ++g_calibrationJoynum;
    for( ; g_calibrationJoynum < 4; ++g_calibrationJoynum )
    {
      gp = GetGamepadCaps( g_calibrationJoynum );
      if( gp && gp->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
      {
          // Found another gun
        g_calibrationStep = 0;
        break;
      }
    }

    if( g_calibrationJoynum == 4 )
      return NULL;
  }

  sprintf( retString, "Gun %d: ", g_calibrationJoynum + 1 );
  switch( g_calibrationStep++ )
  {
  case 0:
    strcat( retString, "Upper left" );
    break;

  case 1:
    strcat( retString, "Center" );
    break;
  }

	return retString;
}

//---------------------------------------------------------------------
//	osd_joystick_calibrate
//---------------------------------------------------------------------
void osd_joystick_calibrate( void )
{
/* Get the actual joystick calibration data for the current position */

  if( g_calibrationStep && g_calibrationStep < 3 )
  {
	  const XINPUT_GAMEPAD *gp;
    if( (gp = GetGamepadState( g_calibrationJoynum )) )
    {
      g_calibrationData[g_calibrationJoynum].m_xData[g_calibrationStep-1] = gp->sThumbLX;
      g_calibrationData[g_calibrationJoynum].m_yData[g_calibrationStep-1] = gp->sThumbLY;
    }
    PRINTMSG( T_INFO, "CALIB: STEP %d: %d, %d\n", g_calibrationStep - 1, gp->sThumbLX, gp->sThumbLY );
  }
}

//---------------------------------------------------------------------
//	osd_joystick_end_calibration
//---------------------------------------------------------------------
void osd_joystick_end_calibration( void )
{
/* Postprocessing (e.g. saving joystick data to config) */
}

//---------------------------------------------------------------------
//	osd_lightgun_read
//---------------------------------------------------------------------
void osd_lightgun_read(int player, int *deltax, int *deltay)
{
	const XINPUT_GAMEPAD *gp;
  assert( deltax && deltay );

	if( (gp = GetGamepadState( player )) )
  {
    lightgunCalibration_t *calibData = &g_calibrationData[player];
    FLOAT calibratedX, calibratedY, xMap, yMap;

    *deltax = gp->sThumbLX;
    *deltay = gp->sThumbLY;

      // Don't bother if we're not pointing at the screen
    if( !(*deltax) && !(*deltay) )
      return;

    calibratedX = (*deltax);// + calibData->m_xData[1];
    calibratedY = (*deltay);// + calibData->m_yData[1]);

      // Map from -128 to 128
    xMap = calibData->m_xData[0] + calibData->m_xData[1]; // left X is negative, so add center to bring it closer to the middle
    yMap = calibData->m_yData[0] - calibData->m_yData[1]; // top Y is positive, so sub center to bring it closer to the middle

    if( xMap )
      calibratedX = (int)((FLOAT)calibratedX * 128.0f / -xMap );
    else
      calibratedX = 0;

    if( yMap )
      calibratedY = (int)((FLOAT)calibratedY * 128.0f / yMap );
    else
      calibratedY = 0;

      // Lock to the expected range
    if( calibratedX > 128 )
      calibratedX = 128;
    else if( calibratedX < -128 )
      calibratedX = -128;

    if( calibratedY > 128 )
      calibratedY = 128;
    else if( calibratedY < -128 )
      calibratedY = -128;

    *deltax = calibratedX;
    *deltay = calibratedY;
  }
  else  
	  *deltax = *deltay = 0;
}

//---------------------------------------------------------------------
//	osd_trak_read
//---------------------------------------------------------------------
void osd_trak_read(int player, int *deltax, int *deltay)
{
  /*
	const XINPUT_GAMEPAD *gp;

	if( (gp = GetGamepadState( player )) )
  {
      if( gp->sThumbLX > ANALOG_DEADZONE )
        *deltax = (int)((FLOAT)(gp->sThumbLX - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else if( gp->sThumbLX < -ANALOG_DEADZONE )
        *deltax = (int)((FLOAT)(gp->sThumbLX + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else
        *deltax = 0;

      if( gp->sThumbLY > ANALOG_DEADZONE )
        *deltay = (int)((FLOAT)(gp->sThumbLY - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else if( gp->sThumbLY < -ANALOG_DEADZONE )
        *deltay = (int)((FLOAT)(gp->sThumbLY + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else
        *deltay = 0;

      // Set to [-128,128]
    *deltax = ((FLOAT)*deltax * 128.0f / 32767.0f);
    *deltay = ((FLOAT)*deltay * 128.0f / 32767.0f);

      // Lock to the expected range
    if( *deltax > 128 )
      *deltax = 128;
    else if( *deltax < -128 )
      *deltax = -128;

    if( *deltay > 128 )
      *deltay = 128;
    else if( *deltay < -128 )
      *deltay = -128;
  }
  else  
  */
	  *deltax = *deltay = 0;
}


//---------------------------------------------------------------------
//	osd_analogjoy_read
//---------------------------------------------------------------------
void osd_analogjoy_read(	int player,
													int analog_axis[MAX_ANALOG_AXES], 
													InputCode analogjoy_input[MAX_ANALOG_AXES] )
{
	/* return values in the range -128 .. 128 (yes, 128, not 127) */
	UINT32 i = 0;
  FLOAT analogDivisor = 32767.0f;
	const XINPUT_GAMEPAD *gamepad;
	
//	PRINTMSG( T_TRACE, "osd_analogjoy_read" );

	
	gamepad = GetGamepadState( player );
	if( !gamepad )
	{
//		PRINTMSG( T_ERROR, "Invalid joynum %lu!", player );
		return;
	}

	for( ; i < MAX_ANALOG_AXES; ++i )
	{
		int joyindex, joytype, joynum;

		analog_axis[i] = 0;

		if (analogjoy_input[i] == CODE_NONE )
			continue;

		joyindex = JOYINDEX( analogjoy_input[i] );
		joytype = JT( analogjoy_input[i] );
		joynum = JOYNUM( analogjoy_input[i] );

			// Don't handle invalid controller IDs
		if( joynum >= 4 )
			continue;

    switch( joytype )
    {
	  case JT_LSTICK_UP:
      if( gamepad->sThumbLY > ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLY - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else if( gamepad->sThumbLY < -ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLY + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else
        analog_axis[i] = 0;

       // MAME seems to expect - values for Y axes to be "Up", so we need to
       // negate
      analog_axis[i] *= -1;
      break;

	  case JT_LSTICK_DOWN:
CHECKRAM();
      if( gamepad->sThumbLY > ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLY - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else if( gamepad->sThumbLY < -ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLY + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else
        analog_axis[i] = 0;

       // MAME seems to expect - values for Y axes to be "Up", so we need to
       // negate
      analog_axis[i] *= -1;
			break;

    case JT_LSTICK_LEFT:
      if( gamepad->sThumbLX < -ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLX + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else if( gamepad->sThumbLX > ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLX - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else
        analog_axis[i] = 0;
      break;

	  case JT_LSTICK_RIGHT:
CHECKRAM();
      if( gamepad->sThumbLX < -ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLX + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else if( gamepad->sThumbLX > ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLX - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else
        analog_axis[i] = 0;
      break;

	  case JT_RSTICK_UP:
    case JT_RSTICK_DOWN:
      if( gamepad->sThumbRY > ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbRY - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else if( gamepad->sThumbRY < -ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbRY + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else
        analog_axis[i] = 0;

       // MAME seems to expect - values for Y axes to be "Up", so we need to
       // negate
      analog_axis[i] *= -1;
      break;

    case JT_RSTICK_LEFT:
	  case JT_RSTICK_RIGHT:
      if( gamepad->sThumbRX < -ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbRX + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else if( gamepad->sThumbRX > ANALOG_DEADZONE )
        analog_axis[i] = (int)((FLOAT)(gamepad->sThumbRX - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
      else
        analog_axis[i] = 0;
      break;

    case JT_BUTTON:
      {
        analogDivisor = 255.0f;
          // Most of the XBOX buttons are actually
          // analog controls, we map them from -128 to 128
        switch( joyindex )
        {
        case BUTTON_A:
          analog_axis[i] = (int)gamepad->bAnalogButtons[XINPUT_GAMEPAD_A] - 128;
          break;

        case BUTTON_X:
          analog_axis[i] = (int)gamepad->bAnalogButtons[XINPUT_GAMEPAD_X] - 128;
          break;

        case BUTTON_B:
          analog_axis[i] = (int)gamepad->bAnalogButtons[XINPUT_GAMEPAD_B] - 128;
          break;

        case BUTTON_Y:
          analog_axis[i] = (int)gamepad->bAnalogButtons[XINPUT_GAMEPAD_Y] - 128;
          break;

        case BUTTON_LEFT_TRIGGER:
          analog_axis[i] = (int)gamepad->bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] - 128;
          break;

        case BUTTON_RIGHT_TRIGGER:
          analog_axis[i] = (int)gamepad->bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] - 128;
          break;

        case BUTTON_WHITE:
          analog_axis[i] = (int)gamepad->bAnalogButtons[XINPUT_GAMEPAD_WHITE] - 128;
          break;

        case BUTTON_BLACK:
          analog_axis[i] = (int)gamepad->bAnalogButtons[XINPUT_GAMEPAD_BLACK] - 128;
          break;

        default:
          continue;
        }
      }

        // MAME seems to expect pedals to go from 
        // 128 (released) to -128 (pressed)
      analog_axis[i] *= -1;
      break;

		default:
			continue;
		}

		analog_axis[i] = ((FLOAT)analog_axis[i] * 128.0f / analogDivisor);
		if (analog_axis[i] < -128) 
			analog_axis[i] = -128;
		if (analog_axis[i] >  128) 
			analog_axis[i] =  128;
	}
}



//---------------------------------------------------------------------
//	osd_customize_inputport_defaults
//---------------------------------------------------------------------
void osd_customize_inputport_defaults( struct ipd *defaults )
{
/*
  inptport.c defines some general purpose defaults for key and joystick bindings.
  They may be further adjusted by the OS dependant code to better match the
  available keyboard, e.g. one could map pause to the Pause key instead of P, or
  snapshot to PrtScr instead of F12. Of course the user can further change the
  settings to anything he/she likes.
  This function is called on startup, before reading the configuration from disk.
  Scan the list, and change the keys/joysticks you want.
*/
  if( g_STARTUPSectionUnloaded )
  {
    PRINTMSG( T_INFO, "Loading startup section!" );
    XLoadSection( "STARTUP" );
    g_STARTUPSectionUnloaded = FALSE;
  }

  if( !g_systemInitialized )
    InitializeJoystickMouse();

  Helper_CustomizeInputPortDefaults( defaults );

  if( !g_STARTUPSectionUnloaded )
  {
    PRINTMSG( T_INFO, "Unloading startup section!" );
    XFreeSection( "STARTUP" );
    g_STARTUPSectionUnloaded = TRUE;
  }
}




#pragma code_seg("STARTUP")
//---------------------------------------------------------------------
//	InitializeJoystickMouse
//---------------------------------------------------------------------
void InitializeJoystickMouse( void )
{
	INT32 joycount = 0;
	INT32 stickIndex = 0;

	for( ; stickIndex < 4; ++stickIndex )
	{
    char name[32];
    

      // Note on "CODE_OTHER" entries:
      // The way that internal_code_find_joystick() (input.c) works is to check the standard
      // code against CODE_OTHER, and then the "oscode" against whatever we set in the second parameter
      // to the ADDENTRY macro. However, other places in input.c require us to have a valid 
      // joyoscode_to_code() mapping for each "CODE_OTHER" entry. Therefore, we do an AXISCODE or
      // BUTTONCODE just before the ADDENTRY call. This allows us to use the joycode_to_oscode() mapping
      // to assign buttons to functions in osd_customize_inputport_defaults() below, while retaining
      // the naming functionality in the UI "Input" menu. If we don't set the syscode (param 3 of AXISCODE)
      // to CODE_OTHER, any extended entries will be named "n/a", which isn't such a great user experience

      // DPad
    ADDENTRY( "DPAD UP",      JOYCODE( stickIndex, JT_DPAD_UP, 0 ),        STDCODE( UP ) );
    ADDENTRY( "DPAD RIGHT",   JOYCODE( stickIndex, JT_DPAD_RIGHT , 0 ),    STDCODE( RIGHT ) );
    ADDENTRY( "DPAD DOWN",    JOYCODE( stickIndex, JT_DPAD_DOWN, 0 ),      STDCODE( DOWN ) );
    ADDENTRY( "DPAD LEFT",    JOYCODE( stickIndex, JT_DPAD_LEFT , 0 ),     STDCODE( LEFT ) );

      // Left analog
    AXISCODE( stickIndex, JT_LSTICK_UP );
    AXISCODE( stickIndex, JT_LSTICK_RIGHT );
    AXISCODE( stickIndex, JT_LSTICK_DOWN );
    AXISCODE( stickIndex, JT_LSTICK_LEFT );
    ADDENTRY( "LA UP",        JOYCODE( stickIndex, JT_LSTICK_UP, 0 ),     CODE_OTHER );
    ADDENTRY( "LA RIGHT",     JOYCODE( stickIndex, JT_LSTICK_RIGHT , 0 ), CODE_OTHER );
    ADDENTRY( "LA DOWN",      JOYCODE( stickIndex, JT_LSTICK_DOWN, 0 ),   CODE_OTHER );
    ADDENTRY( "LA LEFT",      JOYCODE( stickIndex, JT_LSTICK_LEFT , 0 ),  CODE_OTHER );

      // Right analog
    AXISCODE( stickIndex, JT_RSTICK_UP );
    AXISCODE( stickIndex, JT_RSTICK_RIGHT );
    AXISCODE( stickIndex, JT_RSTICK_DOWN );
    AXISCODE( stickIndex, JT_RSTICK_LEFT );
    ADDENTRY( "RA UP",        JOYCODE( stickIndex, JT_RSTICK_UP, 0 ),     CODE_OTHER );
    ADDENTRY( "RA RIGHT",     JOYCODE( stickIndex, JT_RSTICK_RIGHT , 0 ), CODE_OTHER );
    ADDENTRY( "RA DOWN",      JOYCODE( stickIndex, JT_RSTICK_DOWN, 0 ),   CODE_OTHER );
    ADDENTRY( "RA LEFT",      JOYCODE( stickIndex, JT_RSTICK_LEFT , 0 ),  CODE_OTHER );

      // Buttons
    BUTTONCODE( stickIndex, BUTTON_WHITE );
    BUTTONCODE( stickIndex, BUTTON_BLACK );
    BUTTONCODE( stickIndex, BUTTON_LA );
    BUTTONCODE( stickIndex, BUTTON_RA );
    ADDENTRY( "A",            JOYCODE( stickIndex, JT_BUTTON, BUTTON_A ),              STDCODE( BUTTON1 ) );
    ADDENTRY( "X",            JOYCODE( stickIndex, JT_BUTTON, BUTTON_X ),              STDCODE( BUTTON2 ) );
    ADDENTRY( "B",            JOYCODE( stickIndex, JT_BUTTON, BUTTON_B ),              STDCODE( BUTTON3 ) );
    ADDENTRY( "Y",            JOYCODE( stickIndex, JT_BUTTON, BUTTON_Y ),              STDCODE( BUTTON4 ) );
    ADDENTRY( "LTrig",        JOYCODE( stickIndex, JT_BUTTON, BUTTON_LEFT_TRIGGER ),   STDCODE( BUTTON5 ) );
    ADDENTRY( "RTrig",        JOYCODE( stickIndex, JT_BUTTON, BUTTON_RIGHT_TRIGGER ),  STDCODE( BUTTON6 ) );
    ADDENTRY( "Start",        JOYCODE( stickIndex, JT_BUTTON, BUTTON_START ),          STDCODE( START ) );
    ADDENTRY( "Back",         JOYCODE( stickIndex, JT_BUTTON, BUTTON_BACK ),           STDCODE( SELECT ) );
    ADDENTRY( "White",        JOYCODE( stickIndex, JT_BUTTON, BUTTON_WHITE ),          CODE_OTHER );
    ADDENTRY( "Black",        JOYCODE( stickIndex, JT_BUTTON, BUTTON_BLACK ),          CODE_OTHER );
    ADDENTRY( "LA",           JOYCODE( stickIndex, JT_BUTTON, BUTTON_LA ),             CODE_OTHER );
    ADDENTRY( "RA",           JOYCODE( stickIndex, JT_BUTTON, BUTTON_RA ),             CODE_OTHER );
  }

  g_systemInitialized = TRUE;
}

//---------------------------------------------------------------------
//	Helper_CustomizeInputPortDefaults
//---------------------------------------------------------------------
static void Helper_CustomizeInputPortDefaults( struct ipd *defaults )
{
  UINT32 i = 0;

	for( ; defaults[i].type != IPT_END; ++i )
	{
		struct ipd *entry = &defaults[i];

		switch( entry->type )
		{
      //-- UI CONTROLS ----------------------------------------------------------------------------

			// *** IPT_UI_SELECT *** //
    case IPT_UI_SELECT:
      REMAP_SEQ_7(  JOYCODE_1_BUTTON1, CODE_OR,
                    JOYCODE_2_BUTTON1, CODE_OR,
                    JOYCODE_3_BUTTON1, CODE_OR,
                    JOYCODE_4_BUTTON1 );
      break;

			// *** IPT_UI_CANCEL *** //
		case IPT_UI_CANCEL:
				// Remap cancel to gamepad0 back + start
      REMAP_SEQ_12( JOYCODE_1_SELECT, JOYCODE_1_START, CODE_OR, 
                    JOYCODE_2_SELECT, JOYCODE_2_START, CODE_OR,
                    JOYCODE_3_SELECT, JOYCODE_3_START, CODE_OR,
                    JOYCODE_4_SELECT, JOYCODE_4_START, CODE_OR );
			break;

			// *** IPT_UI_TOGGLE_CROSSHAIR *** //
    case IPT_UI_TOGGLE_CROSSHAIR:
      REMAP_SEQ_12( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_LA ), CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_LA ), CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_LA ), CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_LA ), CODE_OR );
      break;

			// *** IPT_UI_CONFIGURE *** //
    case IPT_UI_CONFIGURE:
      REMAP_SEQ_12( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_WHITE ), CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_WHITE ), CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_WHITE ), CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_WHITE ), CODE_OR );
      break;

			// *** IPT_UI_PAUSE *** //
    case IPT_UI_PAUSE:
      REMAP_SEQ_12( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_BLACK ), CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_BLACK ), CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_BLACK ), CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_BLACK ), CODE_OR );
      break;

			// *** IPT_UI_ON_SCREEN_DISPLAY *** //
    case IPT_UI_ON_SCREEN_DISPLAY:
      REMAP_SEQ_12( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_RA ), CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_RA ), CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_RA ), CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_RA ), CODE_OR );
      break;

			// *** IPT_UI_RESET_MACHINE *** //
    case IPT_UI_RESET_MACHINE:
      REMAP_SEQ_15( JOYCODE_1_SELECT, JOYCODE_1_BUTTON5, JOYCODE_1_BUTTON6, CODE_OR, 
                    JOYCODE_2_SELECT, JOYCODE_2_BUTTON5, JOYCODE_2_BUTTON6, CODE_OR,
                    JOYCODE_3_SELECT, JOYCODE_3_BUTTON5, JOYCODE_3_BUTTON6, CODE_OR,
                    JOYCODE_4_SELECT, JOYCODE_4_BUTTON5, JOYCODE_4_BUTTON6 );
      break;

			// *** IPT_UI_SAVE_STATE *** //
    case IPT_UI_SAVE_STATE:
      REMAP_SEQ_15( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_RA ), JOYCODE_1_BUTTON6, CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_RA ), JOYCODE_2_BUTTON6, CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_RA ), JOYCODE_3_BUTTON6, CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_RA ), JOYCODE_4_BUTTON6 );
      break;

			// *** IPT_UI_LOAD_STATE *** //
    case IPT_UI_LOAD_STATE:
      REMAP_SEQ_15( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_RA ), JOYCODE_1_BUTTON5, CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_RA ), JOYCODE_1_BUTTON5, CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_RA ), JOYCODE_1_BUTTON5, CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_RA ), JOYCODE_1_BUTTON5 );
      break;

			// *** IPT_UI_UP *** //
    case IPT_UI_UP:
      REMAP_SEQ_15( JOYCODE_1_UP, CODE_OR, AXISCODE( 0, JT_LSTICK_UP ), CODE_OR,
                    JOYCODE_2_UP, CODE_OR, AXISCODE( 1, JT_LSTICK_UP ), CODE_OR,
                    JOYCODE_3_UP, CODE_OR, AXISCODE( 2, JT_LSTICK_UP ), CODE_OR,
                    JOYCODE_4_UP, CODE_OR, AXISCODE( 3, JT_LSTICK_UP ) );
      break;

			// *** IPT_UI_LEFT *** //
    case IPT_UI_LEFT:
      REMAP_SEQ_15( JOYCODE_1_LEFT, CODE_OR, AXISCODE( 0, JT_LSTICK_LEFT ), CODE_OR,
                    JOYCODE_2_LEFT, CODE_OR, AXISCODE( 1, JT_LSTICK_LEFT ), CODE_OR,
                    JOYCODE_3_LEFT, CODE_OR, AXISCODE( 2, JT_LSTICK_LEFT ), CODE_OR,
                    JOYCODE_4_LEFT, CODE_OR, AXISCODE( 3, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_UI_DOWN *** //
    case IPT_UI_DOWN:
      REMAP_SEQ_15( JOYCODE_1_DOWN, CODE_OR, AXISCODE( 0, JT_LSTICK_DOWN ), CODE_OR,
                    JOYCODE_2_DOWN, CODE_OR, AXISCODE( 1, JT_LSTICK_DOWN ), CODE_OR,
                    JOYCODE_3_DOWN, CODE_OR, AXISCODE( 2, JT_LSTICK_DOWN ), CODE_OR,
                    JOYCODE_4_DOWN, CODE_OR, AXISCODE( 3, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_UI_RIGHT *** //
    case IPT_UI_RIGHT:
      REMAP_SEQ_15( JOYCODE_1_RIGHT, CODE_OR, AXISCODE( 0, JT_LSTICK_RIGHT ), CODE_OR,
                    JOYCODE_2_RIGHT, CODE_OR, AXISCODE( 1, JT_LSTICK_RIGHT ), CODE_OR,
                    JOYCODE_3_RIGHT, CODE_OR, AXISCODE( 2, JT_LSTICK_RIGHT ), CODE_OR,
                    JOYCODE_4_RIGHT, CODE_OR, AXISCODE( 3, JT_LSTICK_RIGHT ) );
      break;


			// *** IPT_UI_SNAPSHOT *** //
    case IPT_UI_SNAPSHOT:
      REMAP_SEQ_15( JOYCODE_1_SELECT, JOYCODE_1_BUTTON1, JOYCODE_1_BUTTON3, CODE_OR, 
                    JOYCODE_2_SELECT, JOYCODE_2_BUTTON1, JOYCODE_2_BUTTON3, CODE_OR,
                    JOYCODE_3_SELECT, JOYCODE_3_BUTTON1, JOYCODE_3_BUTTON3, CODE_OR,
                    JOYCODE_4_SELECT, JOYCODE_4_BUTTON1, JOYCODE_4_BUTTON3 );
      break;

      //-- PLAYER 1 CONTROLS ----------------------------------------------------------------------------

			// *** IPT_START1 *** //
    case IPT_START1:
      REMAP_SEQ_1( JOYCODE_1_START );
      break;

			// *** IPT_COIN1 *** //
    case IPT_COIN1:
      REMAP_SEQ_1( JOYCODE_1_SELECT);
      break;

			// *** IPT_JOYSTICK_UP|IPF_PLAYER1 *** //
    case (IPT_JOYSTICK_UP|IPF_PLAYER1):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_1_UP, CODE_OR, AXISCODE( 0, JT_LSTICK_UP ) );
      break;

			// *** IPT_JOYSTICK_RIGHT|IPF_PLAYER1 *** //
    case (IPT_JOYSTICK_RIGHT|IPF_PLAYER1):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_1_RIGHT, CODE_OR, AXISCODE( 0, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_JOYSTICK_DOWN|IPF_PLAYER1 *** //
    case (IPT_JOYSTICK_DOWN|IPF_PLAYER1):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_1_DOWN, CODE_OR, AXISCODE( 0, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICK_LEFT|IPF_PLAYER1 *** //
    case (IPT_JOYSTICK_LEFT|IPF_PLAYER1):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_1_LEFT, CODE_OR, AXISCODE( 0, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_BUTTON1|IPF_PLAYER1 *** //
    case (IPT_BUTTON1|IPF_PLAYER1):
      REMAP_SEQ_1( JOYCODE_1_BUTTON1 );
      break;

			// *** IPT_BUTTON2|IPF_PLAYER1 *** //
    case (IPT_BUTTON2|IPF_PLAYER1):
      REMAP_SEQ_1( JOYCODE_1_BUTTON2 );
      break;

			// *** IPT_BUTTON3|IPF_PLAYER1 *** //
    case (IPT_BUTTON3|IPF_PLAYER1):
      REMAP_SEQ_1( JOYCODE_1_BUTTON3 );
      break;

			// *** IPT_BUTTON4|IPF_PLAYER1 *** //
    case (IPT_BUTTON4|IPF_PLAYER1):
      REMAP_SEQ_1( JOYCODE_1_BUTTON4 );
      break;

			// *** IPT_BUTTON5|IPF_PLAYER1 *** //
    case (IPT_BUTTON5|IPF_PLAYER1):
      REMAP_SEQ_1( JOYCODE_1_BUTTON5 );
      break;

			// *** IPT_BUTTON6|IPF_PLAYER1 *** //
    case (IPT_BUTTON6|IPF_PLAYER1):
      REMAP_SEQ_1( JOYCODE_1_BUTTON6 );
      break;

			// *** IPT_BUTTON7|IPF_PLAYER1 *** //
    case (IPT_BUTTON7|IPF_PLAYER1):
      REMAP_SEQ_1( BUTTONCODE( 0, BUTTON_LA ) );
      break;

			// *** IPT_BUTTON8|IPF_PLAYER1 *** //
    case (IPT_BUTTON8|IPF_PLAYER1):
      REMAP_SEQ_1( BUTTONCODE( 0, BUTTON_RA ) );
      break;

			// *** IPT_BUTTON9|IPF_PLAYER1 *** //
    case (IPT_BUTTON9|IPF_PLAYER1):
      REMAP_SEQ_1( BUTTONCODE( 0, BUTTON_WHITE ) );
      break;

			// *** IPT_BUTTON10|IPF_PLAYER1 *** //
    case (IPT_BUTTON10|IPF_PLAYER1):
      REMAP_SEQ_1( BUTTONCODE( 0, BUTTON_BLACK ) );
      break;

			// *** IPT_JOYSTICKRIGHT_UP|IPF_PLAYER1 *** //
    case (IPT_JOYSTICKRIGHT_UP|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_RSTICK_UP ) );
      break;

			// *** IPT_JOYSTICKRIGHT_LEFT|IPF_PLAYER1 *** //
    case (IPT_JOYSTICKRIGHT_LEFT|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_JOYSTICKRIGHT_DOWN|IPF_PLAYER1 *** //
    case (IPT_JOYSTICKRIGHT_DOWN|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_RSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICKRIGHT_RIGHT|IPF_PLAYER1 *** //
    case (IPT_JOYSTICKRIGHT_RIGHT|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_JOYSTICKLEFT_UP|IPF_PLAYER1 *** //
    case (IPT_JOYSTICKLEFT_UP|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_UP ) );
      break;

			// *** IPT_JOYSTICKLEFT_LEFT|IPF_PLAYER1 *** //
    case (IPT_JOYSTICKLEFT_LEFT|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_JOYSTICKLEFT_DOWN|IPF_PLAYER1 *** //
    case (IPT_JOYSTICKLEFT_DOWN|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICKLEFT_RIGHT|IPF_PLAYER1 *** //
    case (IPT_JOYSTICKLEFT_RIGHT|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_PEDAL|IPF_PLAYER1 *** //
    case (IPT_PEDAL|IPF_PLAYER1):
      REMAP_SEQ_1( JOYCODE_1_BUTTON5 );
      break;

			// *** IPT_PEDAL|IPF_PLAYER1 *** //
    case (IPT_PEDAL2|IPF_PLAYER1):
      REMAP_SEQ_1( JOYCODE_1_BUTTON6 );
      break;

			// *** IPT_PADDLE|IPF_PLAYER1 *** //
    case (IPT_PADDLE|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_PADDLE|IPF_PLAYER1+IPT_EXTENSION *** //
    case ((IPT_PADDLE|IPF_PLAYER1)+IPT_EXTENSION):
      REMAP_SEQ_1( JOYCODE_1_RIGHT );
      break;

			// *** IPT_PADDLE_V|IPF_PLAYER1 *** //
    case (IPT_PADDLE_V|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_UP ) );
      break;

			// *** IPT_PADDLE_V|IPF_PLAYER1+IPT_EXTENSION *** //
    case ((IPT_PADDLE_V|IPF_PLAYER1)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_DIAL|IPF_PLAYER1 *** //
    case (IPT_DIAL|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_DIAL|IPF_PLAYER1+IPT_EXTENSION *** //
    case ((IPT_DIAL|IPF_PLAYER1)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_TRACKBALL_X|IPF_PLAYER1 *** //
    case (IPT_TRACKBALL_X|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_TRACKBALL_X|IPF_PLAYER1+IPT_EXTENSION *** //
    case ((IPT_TRACKBALL_X|IPF_PLAYER1)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_TRACKBALL_Y|IPF_PLAYER1 *** //
    case (IPT_TRACKBALL_Y|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_UP ) );
      break;

			// *** IPT_TRACKBALL_Y|IPF_PLAYER1+IPT_EXTENSION *** //
    case ((IPT_TRACKBALL_Y|IPF_PLAYER1)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_DOWN ) );
      break;


			// *** IPT_AD_STICK_X|IPF_PLAYER1 *** //
    case (IPT_AD_STICK_X|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_AD_STICK_X|IPF_PLAYER1+IPT_EXTENSION *** //
    case (IPT_AD_STICK_X|IPF_PLAYER1)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_AD_STICK_Y|IPF_PLAYER1 *** //
    case (IPT_AD_STICK_Y|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_UP ) );
      break;

			// *** IPT_AD_STICK_Y|IPF_PLAYER1+IPT_EXTENSION *** //
    case (IPT_AD_STICK_Y|IPF_PLAYER1)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_AD_STICK_Z|IPF_PLAYER1 *** //
    case (IPT_AD_STICK_Z|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_AD_STICK_Z|IPF_PLAYER1+IPT_EXTENSION *** //
    case (IPT_AD_STICK_Z|IPF_PLAYER1)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 0, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER1 *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER1+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER1)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 0, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_LIGHTGUN_Y|IPF_PLAYER1 *** //
    case (IPT_LIGHTGUN_Y|IPF_PLAYER1):
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_UP ) );
      break;

			// *** IPT_LIGHTGUN_Y|IPF_PLAYER1+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_Y|IPF_PLAYER1)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_DOWN ) );
      break;



      //-- PLAYER 2 CONTROLS ----------------------------------------------------------------------------

			// *** IPT_START2 *** //
    case IPT_START2:
      REMAP_SEQ_1( JOYCODE_2_START );
      break;

			// *** IPT_COIN2 *** //
    case IPT_COIN2:
      REMAP_SEQ_1( JOYCODE_2_SELECT );
      break;

			// *** IPT_JOYSTICK_UP|IPF_PLAYER2 *** //
    case (IPT_JOYSTICK_UP|IPF_PLAYER2):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_2_UP, CODE_OR, AXISCODE( 1, JT_LSTICK_UP ) );
      break;

			// *** IPT_JOYSTICK_RIGHT|IPF_PLAYER2 *** //
    case (IPT_JOYSTICK_RIGHT|IPF_PLAYER2):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_2_RIGHT, CODE_OR, AXISCODE( 1, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_JOYSTICK_DOWN|IPF_PLAYER2 *** //
    case (IPT_JOYSTICK_DOWN|IPF_PLAYER2):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_2_DOWN, CODE_OR, AXISCODE( 1, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICK_LEFT|IPF_PLAYER2 *** //
    case (IPT_JOYSTICK_LEFT|IPF_PLAYER2):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_2_LEFT, CODE_OR, AXISCODE( 1, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_BUTTON1|IPF_PLAYER2 *** //
    case (IPT_BUTTON1|IPF_PLAYER2):
      REMAP_SEQ_1( JOYCODE_2_BUTTON1 );
      break;

			// *** IPT_BUTTON2|IPF_PLAYER2 *** //
    case (IPT_BUTTON2|IPF_PLAYER2):
      REMAP_SEQ_1( JOYCODE_2_BUTTON2 );
      break;

			// *** IPT_BUTTON3|IPF_PLAYER2 *** //
    case (IPT_BUTTON3|IPF_PLAYER2):
      REMAP_SEQ_1( JOYCODE_2_BUTTON3 );
      break;

			// *** IPT_BUTTON4|IPF_PLAYER2 *** //
    case (IPT_BUTTON4|IPF_PLAYER2):
      REMAP_SEQ_1( JOYCODE_2_BUTTON4 );
      break;

			// *** IPT_BUTTON5|IPF_PLAYER2 *** //
    case (IPT_BUTTON5|IPF_PLAYER2):
      REMAP_SEQ_1( JOYCODE_2_BUTTON5 );
      break;

			// *** IPT_BUTTON6|IPF_PLAYER2 *** //
    case (IPT_BUTTON6|IPF_PLAYER2):
      REMAP_SEQ_1( JOYCODE_2_BUTTON6 );
      break;

			// *** IPT_BUTTON7|IPF_PLAYER2 *** //
    case (IPT_BUTTON7|IPF_PLAYER2):
      REMAP_SEQ_1( BUTTONCODE( 1, BUTTON_LA ) );
      break;

			// *** IPT_BUTTON8|IPF_PLAYER2 *** //
    case (IPT_BUTTON8|IPF_PLAYER2):
      REMAP_SEQ_1( BUTTONCODE( 1, BUTTON_RA ) );
      break;

			// *** IPT_BUTTON9|IPF_PLAYER2 *** //
    case (IPT_BUTTON9|IPF_PLAYER2):
      REMAP_SEQ_1( BUTTONCODE( 1, BUTTON_WHITE ) );
      break;

			// *** IPT_BUTTON10|IPF_PLAYER2 *** //
    case (IPT_BUTTON10|IPF_PLAYER2):
      REMAP_SEQ_1( BUTTONCODE( 1, BUTTON_BLACK ) );
      break;

			// *** IPT_JOYSTICKRIGHT_UP|IPF_PLAYER2 *** //
    case (IPT_JOYSTICKRIGHT_UP|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_RSTICK_UP ) );
      break;

			// *** IPT_JOYSTICKRIGHT_LEFT|IPF_PLAYER2 *** //
    case (IPT_JOYSTICKRIGHT_LEFT|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_JOYSTICKRIGHT_DOWN|IPF_PLAYER2 *** //
    case (IPT_JOYSTICKRIGHT_DOWN|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_RSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICKRIGHT_RIGHT|IPF_PLAYER2 *** //
    case (IPT_JOYSTICKRIGHT_RIGHT|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_JOYSTICKLEFT_UP|IPF_PLAYER2 *** //
    case (IPT_JOYSTICKLEFT_UP|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_UP ) );
      break;

			// *** IPT_JOYSTICKLEFT_LEFT|IPF_PLAYER2 *** //
    case (IPT_JOYSTICKLEFT_LEFT|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_JOYSTICKLEFT_DOWN|IPF_PLAYER2 *** //
    case (IPT_JOYSTICKLEFT_DOWN|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICKLEFT_RIGHT|IPF_PLAYER2 *** //
    case (IPT_JOYSTICKLEFT_RIGHT|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_PEDAL|IPF_PLAYER2 *** //
    case (IPT_PEDAL|IPF_PLAYER2):
      REMAP_SEQ_1( JOYCODE_2_BUTTON5 );
      break;

			// *** IPT_PEDAL2|IPF_PLAYER2 *** //
    case (IPT_PEDAL2|IPF_PLAYER2):
      REMAP_SEQ_1( JOYCODE_2_BUTTON6 );
      break;

			// *** IPT_PADDLE|IPF_PLAYER2 *** //
    case (IPT_PADDLE|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_PADDLE|IPF_PLAYER2+IPT_EXTENSION *** //
    case ((IPT_PADDLE|IPF_PLAYER2)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_PADDLE_V|IPF_PLAYER2 *** //
    case (IPT_PADDLE_V|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_UP ) );
      break;

			// *** IPT_PADDLE_V|IPF_PLAYER2+IPT_EXTENSION *** //
    case ((IPT_PADDLE_V|IPF_PLAYER2)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_DIAL|IPF_PLAYER2 *** //
    case (IPT_DIAL|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_DIAL|IPF_PLAYER2+IPT_EXTENSION *** //
    case ((IPT_DIAL|IPF_PLAYER2)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_TRACKBALL_X|IPF_PLAYER2 *** //
    case (IPT_TRACKBALL_X|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_TRACKBALL_X|IPF_PLAYER2+IPT_EXTENSION *** //
    case ((IPT_TRACKBALL_X|IPF_PLAYER2)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_TRACKBALL_Y|IPF_PLAYER2 *** //
    case (IPT_TRACKBALL_Y|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_UP ) );
      break;

			// *** IPT_TRACKBALL_Y|IPF_PLAYER2+IPT_EXTENSION *** //
    case ((IPT_TRACKBALL_Y|IPF_PLAYER2)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_AD_STICK_X|IPF_PLAYER2 *** //
    case (IPT_AD_STICK_X|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_AD_STICK_X|IPF_PLAYER2+IPT_EXTENSION *** //
    case (IPT_AD_STICK_X|IPF_PLAYER2)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_AD_STICK_Y|IPF_PLAYER2 *** //
    case (IPT_AD_STICK_Y|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_UP ) );
      break;

			// *** IPT_AD_STICK_Y|IPF_PLAYER2+IPT_EXTENSION *** //
    case (IPT_AD_STICK_Y|IPF_PLAYER2)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_AD_STICK_Z|IPF_PLAYER2 *** //
    case (IPT_AD_STICK_Z|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_AD_STICK_Z|IPF_PLAYER2+IPT_EXTENSION *** //
    case (IPT_AD_STICK_Z|IPF_PLAYER2)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 1, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER2 *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER2+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER2)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 1, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_LIGHTGUN_Y|IPF_PLAYER2 *** //
    case (IPT_LIGHTGUN_Y|IPF_PLAYER2):
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_UP ) );
      break;

			// *** IPT_LIGHTGUN_Y|IPF_PLAYER2+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_Y|IPF_PLAYER2)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_DOWN ) );
      break;



      //-- PLAYER 3 CONTROLS ----------------------------------------------------------------------------

			// *** IPT_START3 *** //
    case IPT_START3:
      REMAP_SEQ_1( JOYCODE_3_START );
      break;

			// *** IPT_COIN3 *** //
    case IPT_COIN3:
      REMAP_SEQ_1( JOYCODE_3_SELECT );
      break;

			// *** IPT_JOYSTICK_UP|IPF_PLAYER3 *** //
    case (IPT_JOYSTICK_UP|IPF_PLAYER3):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_3_UP, CODE_OR, AXISCODE( 2, JT_LSTICK_UP ) );
      break;

			// *** IPT_JOYSTICK_RIGHT|IPF_PLAYER3 *** //
    case (IPT_JOYSTICK_RIGHT|IPF_PLAYER3):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_3_RIGHT, CODE_OR, AXISCODE( 2, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_JOYSTICK_DOWN|IPF_PLAYER3 *** //
    case (IPT_JOYSTICK_DOWN|IPF_PLAYER3):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_3_DOWN, CODE_OR, AXISCODE( 2, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICK_LEFT|IPF_PLAYER3 *** //
    case (IPT_JOYSTICK_LEFT|IPF_PLAYER3):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_3_LEFT, CODE_OR, AXISCODE( 2, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_BUTTON1|IPF_PLAYER3 *** //
    case (IPT_BUTTON1|IPF_PLAYER3):
      REMAP_SEQ_1( JOYCODE_3_BUTTON1 );
      break;

			// *** IPT_BUTTON2|IPF_PLAYER3 *** //
    case (IPT_BUTTON2|IPF_PLAYER3):
      REMAP_SEQ_1( JOYCODE_3_BUTTON2 );
      break;

			// *** IPT_BUTTON3|IPF_PLAYER3 *** //
    case (IPT_BUTTON3|IPF_PLAYER3):
      REMAP_SEQ_1( JOYCODE_3_BUTTON3 );
      break;

			// *** IPT_BUTTON4|IPF_PLAYER3 *** //
    case (IPT_BUTTON4|IPF_PLAYER3):
      REMAP_SEQ_1( JOYCODE_3_BUTTON4 );
      break;

			// *** IPT_BUTTON5|IPF_PLAYER3 *** //
    case (IPT_BUTTON5|IPF_PLAYER3):
      REMAP_SEQ_1( JOYCODE_3_BUTTON5 );
      break;

			// *** IPT_BUTTON6|IPF_PLAYER3 *** //
    case (IPT_BUTTON6|IPF_PLAYER3):
      REMAP_SEQ_1( JOYCODE_3_BUTTON6 );
      break;

			// *** IPT_BUTTON7|IPF_PLAYER3 *** //
    case (IPT_BUTTON7|IPF_PLAYER3):
      REMAP_SEQ_1( BUTTONCODE( 2, BUTTON_LA ) );
      break;

			// *** IPT_BUTTON8|IPF_PLAYER3 *** //
    case (IPT_BUTTON8|IPF_PLAYER3):
      REMAP_SEQ_1( BUTTONCODE( 2, BUTTON_RA ) );
      break;

			// *** IPT_BUTTON9|IPF_PLAYER3 *** //
    case (IPT_BUTTON9|IPF_PLAYER3):
      REMAP_SEQ_1( BUTTONCODE( 2, BUTTON_WHITE ) );
      break;

			// *** IPT_BUTTON10|IPF_PLAYER3 *** //
    case (IPT_BUTTON10|IPF_PLAYER3):
      REMAP_SEQ_1( BUTTONCODE( 2, BUTTON_BLACK ) );
      break;

			// *** IPT_JOYSTICKRIGHT_UP|IPF_PLAYER3 *** //
    case (IPT_JOYSTICKRIGHT_UP|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_RSTICK_UP ) );
      break;

			// *** IPT_JOYSTICKRIGHT_LEFT|IPF_PLAYER3 *** //
    case (IPT_JOYSTICKRIGHT_LEFT|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_JOYSTICKRIGHT_DOWN|IPF_PLAYER3 *** //
    case (IPT_JOYSTICKRIGHT_DOWN|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_RSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICKRIGHT_RIGHT|IPF_PLAYER3 *** //
    case (IPT_JOYSTICKRIGHT_RIGHT|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_JOYSTICKLEFT_UP|IPF_PLAYER3 *** //
    case (IPT_JOYSTICKLEFT_UP|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_UP ) );
      break;

			// *** IPT_JOYSTICKLEFT_LEFT|IPF_PLAYER3 *** //
    case (IPT_JOYSTICKLEFT_LEFT|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_JOYSTICKLEFT_DOWN|IPF_PLAYER3 *** //
    case (IPT_JOYSTICKLEFT_DOWN|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICKLEFT_RIGHT|IPF_PLAYER3 *** //
    case (IPT_JOYSTICKLEFT_RIGHT|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_PEDAL|IPF_PLAYER3 *** //
    case (IPT_PEDAL|IPF_PLAYER3):
      REMAP_SEQ_1( JOYCODE_3_BUTTON5 );
      break;

			// *** IPT_PEDAL2|IPF_PLAYER3 *** //
    case (IPT_PEDAL2|IPF_PLAYER3):
      REMAP_SEQ_1( JOYCODE_3_BUTTON6 );
      break;

			// *** IPT_PADDLE|IPF_PLAYER3 *** //
    case (IPT_PADDLE|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_PADDLE|IPF_PLAYER3+IPT_EXTENSION *** //
    case ((IPT_PADDLE|IPF_PLAYER3)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_PADDLE_V|IPF_PLAYER3 *** //
    case (IPT_PADDLE_V|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_UP ) );
      break;

			// *** IPT_PADDLE_V|IPF_PLAYER3+IPT_EXTENSION *** //
    case ((IPT_PADDLE_V|IPF_PLAYER3)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_DIAL|IPF_PLAYER3 *** //
    case (IPT_DIAL|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_DIAL|IPF_PLAYER3+IPT_EXTENSION *** //
    case ((IPT_DIAL|IPF_PLAYER3)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_TRACKBALL_X|IPF_PLAYER3 *** //
    case (IPT_TRACKBALL_X|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_TRACKBALL_X|IPF_PLAYER3+IPT_EXTENSION *** //
    case ((IPT_TRACKBALL_X|IPF_PLAYER3)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_TRACKBALL_Y|IPF_PLAYER3 *** //
    case (IPT_TRACKBALL_Y|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_UP ) );
      break;

			// *** IPT_TRACKBALL_Y|IPF_PLAYER3+IPT_EXTENSION *** //
    case ((IPT_TRACKBALL_Y|IPF_PLAYER3)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_AD_STICK_X|IPF_PLAYER3 *** //
    case (IPT_AD_STICK_X|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_AD_STICK_X|IPF_PLAYER3+IPT_EXTENSION *** //
    case (IPT_AD_STICK_X|IPF_PLAYER3)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_AD_STICK_Y|IPF_PLAYER3 *** //
    case (IPT_AD_STICK_Y|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_UP ) );
      break;

			// *** IPT_AD_STICK_Y|IPF_PLAYER3+IPT_EXTENSION *** //
    case (IPT_AD_STICK_Y|IPF_PLAYER3)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_AD_STICK_Z|IPF_PLAYER3 *** //
    case (IPT_AD_STICK_Z|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_AD_STICK_Z|IPF_PLAYER3+IPT_EXTENSION *** //
    case (IPT_AD_STICK_Z|IPF_PLAYER3)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 2, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER3 *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER3+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER3)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 2, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_LIGHTGUN_Y|IPF_PLAYER3 *** //
    case (IPT_LIGHTGUN_Y|IPF_PLAYER3):
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_UP ) );
      break;

			// *** IPT_LIGHTGUN_Y|IPF_PLAYER3+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_Y|IPF_PLAYER3)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_DOWN ) );
      break;



      //-- PLAYER 4 CONTROLS ----------------------------------------------------------------------------

			// *** IPT_START4 *** //
    case IPT_START4:
      REMAP_SEQ_1( JOYCODE_4_START );
      break;

			// *** IPT_COIN4 *** //
    case IPT_COIN4:
      REMAP_SEQ_1( JOYCODE_4_SELECT );
      break;

			// *** IPT_JOYSTICK_UP|IPF_PLAYER4 *** //
    case (IPT_JOYSTICK_UP|IPF_PLAYER4):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_4_UP, CODE_OR, AXISCODE( 3, JT_LSTICK_UP ) );
      break;

			// *** IPT_JOYSTICK_RIGHT|IPF_PLAYER4 *** //
    case (IPT_JOYSTICK_RIGHT|IPF_PLAYER4):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_4_RIGHT, CODE_OR, AXISCODE( 3, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_JOYSTICK_DOWN|IPF_PLAYER4 *** //
    case (IPT_JOYSTICK_DOWN|IPF_PLAYER4):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_4_DOWN, CODE_OR, AXISCODE( 3, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICK_LEFT|IPF_PLAYER4 *** //
    case (IPT_JOYSTICK_LEFT|IPF_PLAYER4):
        // Don't remap if the game supports analog natively
      REMAP_SEQ_3( JOYCODE_4_LEFT, CODE_OR, AXISCODE( 3, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_BUTTON1|IPF_PLAYER4 *** //
    case (IPT_BUTTON1|IPF_PLAYER4):
      REMAP_SEQ_1( JOYCODE_4_BUTTON1 );
      break;

			// *** IPT_BUTTON2|IPF_PLAYER4 *** //
    case (IPT_BUTTON2|IPF_PLAYER4):
      REMAP_SEQ_1( JOYCODE_4_BUTTON2 );
      break;

			// *** IPT_BUTTON3|IPF_PLAYER4 *** //
    case (IPT_BUTTON3|IPF_PLAYER4):
      REMAP_SEQ_1( JOYCODE_4_BUTTON3 );
      break;

			// *** IPT_BUTTON4|IPF_PLAYER4 *** //
    case (IPT_BUTTON4|IPF_PLAYER4):
      REMAP_SEQ_1( JOYCODE_4_BUTTON4 );
      break;

			// *** IPT_BUTTON5|IPF_PLAYER4 *** //
    case (IPT_BUTTON5|IPF_PLAYER4):
      REMAP_SEQ_1( JOYCODE_4_BUTTON5 );
      break;

			// *** IPT_BUTTON6|IPF_PLAYER4 *** //
    case (IPT_BUTTON6|IPF_PLAYER4):
      REMAP_SEQ_1( JOYCODE_4_BUTTON6 );
      break;

			// *** IPT_BUTTON7|IPF_PLAYER4 *** //
    case (IPT_BUTTON7|IPF_PLAYER4):
      REMAP_SEQ_1( BUTTONCODE( 3, BUTTON_LA ) );
      break;

			// *** IPT_BUTTON8|IPF_PLAYER4 *** //
    case (IPT_BUTTON8|IPF_PLAYER4):
      REMAP_SEQ_1( BUTTONCODE( 3, BUTTON_RA ) );
      break;

			// *** IPT_BUTTON9|IPF_PLAYER4 *** //
    case (IPT_BUTTON9|IPF_PLAYER4):
      REMAP_SEQ_1( BUTTONCODE( 3, BUTTON_WHITE ) );
      break;

			// *** IPT_BUTTON10|IPF_PLAYER4 *** //
    case (IPT_BUTTON10|IPF_PLAYER4):
      REMAP_SEQ_1( BUTTONCODE( 3, BUTTON_BLACK ) );
      break;

			// *** IPT_JOYSTICKRIGHT_UP|IPF_PLAYER4 *** //
    case (IPT_JOYSTICKRIGHT_UP|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_RSTICK_UP ) );
      break;

			// *** IPT_JOYSTICKRIGHT_LEFT|IPF_PLAYER4 *** //
    case (IPT_JOYSTICKRIGHT_LEFT|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_JOYSTICKRIGHT_DOWN|IPF_PLAYER4 *** //
    case (IPT_JOYSTICKRIGHT_DOWN|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_RSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICKRIGHT_RIGHT|IPF_PLAYER4 *** //
    case (IPT_JOYSTICKRIGHT_RIGHT|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_JOYSTICKLEFT_UP|IPF_PLAYER4 *** //
    case (IPT_JOYSTICKLEFT_UP|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_UP ) );
      break;

			// *** IPT_JOYSTICKLEFT_LEFT|IPF_PLAYER4 *** //
    case (IPT_JOYSTICKLEFT_LEFT|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_JOYSTICKLEFT_DOWN|IPF_PLAYER4 *** //
    case (IPT_JOYSTICKLEFT_DOWN|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_JOYSTICKLEFT_RIGHT|IPF_PLAYER4 *** //
    case (IPT_JOYSTICKLEFT_RIGHT|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_PEDAL|IPF_PLAYER4 *** //
    case (IPT_PEDAL|IPF_PLAYER4):
      REMAP_SEQ_1( JOYCODE_4_BUTTON5 );
      break;

			// *** IPT_PEDAL2|IPF_PLAYER4 *** //
    case (IPT_PEDAL2|IPF_PLAYER4):
      REMAP_SEQ_1( JOYCODE_4_BUTTON6 );
      break;

			// *** IPT_PADDLE|IPF_PLAYER4 *** //
    case (IPT_PADDLE|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_PADDLE|IPF_PLAYER4+IPT_EXTENSION *** //
    case ((IPT_PADDLE|IPF_PLAYER4)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_PADDLE_V|IPF_PLAYER4 *** //
    case (IPT_PADDLE_V|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_UP ) );
      break;

			// *** IPT_PADDLE_V|IPF_PLAYER4+IPT_EXTENSION *** //
    case ((IPT_PADDLE_V|IPF_PLAYER4)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_DIAL|IPF_PLAYER4 *** //
    case (IPT_DIAL|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_DIAL|IPF_PLAYER4+IPT_EXTENSION *** //
    case ((IPT_DIAL|IPF_PLAYER4)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_TRACKBALL_X|IPF_PLAYER4 *** //
    case (IPT_TRACKBALL_X|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_TRACKBALL_X|IPF_PLAYER4+IPT_EXTENSION *** //
    case ((IPT_TRACKBALL_X|IPF_PLAYER4)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_TRACKBALL_Y|IPF_PLAYER4 *** //
    case (IPT_TRACKBALL_Y|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_UP ) );
      break;

			// *** IPT_TRACKBALL_Y|IPF_PLAYER4+IPT_EXTENSION *** //
    case ((IPT_TRACKBALL_Y|IPF_PLAYER4)+IPT_EXTENSION):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_AD_STICK_X|IPF_PLAYER4 *** //
    case (IPT_AD_STICK_X|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_AD_STICK_X|IPF_PLAYER4+IPT_EXTENSION *** //
    case (IPT_AD_STICK_X|IPF_PLAYER4)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_RIGHT ) );
      break;

			// *** IPT_AD_STICK_Y|IPF_PLAYER4 *** //
    case (IPT_AD_STICK_Y|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_UP ) );
      break;

			// *** IPT_AD_STICK_Y|IPF_PLAYER4+IPT_EXTENSION *** //
    case (IPT_AD_STICK_Y|IPF_PLAYER4)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_DOWN ) );
      break;

			// *** IPT_AD_STICK_Z|IPF_PLAYER4 *** //
    case (IPT_AD_STICK_Z|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_AD_STICK_Z|IPF_PLAYER4+IPT_EXTENSION *** //
    case (IPT_AD_STICK_Z|IPF_PLAYER4)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 3, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER4 *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_RSTICK_LEFT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER4+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER4)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 3, JT_RSTICK_RIGHT ) );
      break;

			// *** IPT_LIGHTGUN_Y|IPF_PLAYER4 *** //
    case (IPT_LIGHTGUN_Y|IPF_PLAYER4):
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_UP ) );
      break;

			// *** IPT_LIGHTGUN_Y|IPF_PLAYER4+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_Y|IPF_PLAYER4)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_DOWN ) );
      break;

		}
	}
}

//-------------------------------------------------------
//	Helper_AddEntry
//-------------------------------------------------------
static void Helper_AddEntry( const char *name, INT32 code, INT32 standardCode, INT32 *joycount )
{
	int standardcode = JOYCODE_OTHER;
	struct ik *inputkeywords;
  struct JoystickInfo *ji = NULL;
	
  assert( joycount && *joycount < MAX_JOYSTICKINFO_ENTRIES );

  ji = &g_joystickInfo[*joycount];

  ji->name = strdup( name );
  if( !ji->name )
  {
    PRINTMSG( T_ERROR, "Out of memory creating joystick entry!" );
    osd_print_error( "Out of memory!" );
    return;
  }

    // Convert spaces in ji->name to '_'
  {
    char *cur = ji->name;
    while( *cur )
    {
      if( *cur == ' ' )
        *cur = '_';
      ++cur;
    }
  }

  ji->code = code;
  ji->standardcode = standardCode;

    // Reallocate the osd_input_keywords array, and add the new entry
  inputkeywords = (struct ik*)realloc( osd_input_keywords, (g_numOSDInputKeywords + 2) * sizeof(struct ik) );
  if( inputkeywords )
  {
    osd_input_keywords = inputkeywords;
    osd_input_keywords[g_numOSDInputKeywords].name = ji->name;  // Just share the value
    osd_input_keywords[g_numOSDInputKeywords].type = IKT_OSD_JOY;
    osd_input_keywords[g_numOSDInputKeywords].val = code;
    ++g_numOSDInputKeywords;

      // Tag the end of the list
		osd_input_keywords[g_numOSDInputKeywords].name = NULL;
    ++(*joycount);
  }
}
#pragma code_seg()


