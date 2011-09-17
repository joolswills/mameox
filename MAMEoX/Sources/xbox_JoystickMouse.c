/**
	* \file			xbox_JoystickMouse.c
	* \brief		Implementation of the "Joystick & Mouse/Trackball" section of
	*           osdepend.h
  *
  * \note     Based off of XMAME source
	*/

//= I N C L U D E S ====================================================
#include "MAMEoX.h"
#include <assert.h>
#include <stdio.h>

#include "osd_cpu.h"
#include "osdepend.h"
#include "inptport.h"

#include "xbox_JoystickMouse.h"
#include "DebugLogger.h"

//= D E F I N E S ======================================================

//#define USE_HACKEDLIGHTGUN

  //! The value of the analog axis must exceed this deadzone
  //!  when being treated as a digital control (valid values are from [0,32767])
#define ANALOG_AS_DIGITAL_DEADZONE          (32768 * 0.33f)
#define ANALOG_BUTTON_AS_DIGITAL_DEADZONE   16

	// Analog stick deadzone
#define ANALOG_DEADZONE_PERCENTAGE      0.15f
#define ANALOG_DEADZONE				          (INT32)(32768.0f * ANALOG_DEADZONE_PERCENTAGE)
#define DEADZONE_RECTIFIER			        1.0f / (1.0f - ANALOG_DEADZONE_PERCENTAGE)

  // Used to set the temporary name variable in InitializeJoystickMouse
#define JOYNAME( _string__ )  sprintf( name, "J%d %s", stickIndex + 1, _string__ )

  // Helper macro, calls Helper_AddEntry in the InitializeJoystickMouse function
#define BEGINENTRYMAP()                                  UINT32 joycount = 0
#define ADDENTRY( _name__, _code__, _standardCode__ )    JOYNAME( _name__ ); Helper_AddEntry( name, (_code__), (_standardCode__), &joycount )

  //! The total number of JoystickInfo structures (= the total number of mapped buttons/axes on all joysticks)
#define MAX_JOYSTICKINFO_ENTRIES      128

//= G L O B A L = V A R S ==============================================
static struct JoystickInfo			g_joystickInfo[MAX_JOYSTICKINFO_ENTRIES] = {0};
static UINT32                   g_calibrationStep = 0;
static UINT32                   g_calibrationJoynum = 0;

  // number of OSD input keywords we've added (defined in xbox_Input.c)
extern UINT32										g_numOSDInputKeywords;


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
static void Helper_AddEntry( const char *name, INT32 code, INT32 standardCode, UINT32 *joycount );

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
	
//	PRINTMSG(( T_TRACE, "osd_is_joy_pressed" ));
	
	gamepad = GetGamepadState( joynum );
	if( !gamepad )
	{
//		PRINTMSG(( T_ERROR, "Invalid joynum %lu!", joynum ));
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
//	PRINTMSG(( T_TRACE, "osd_is_joystick_axis_code" ));

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
    PRINTMSG(( T_INFO, "CALIB: STEP %d: %d, %d\n", g_calibrationStep - 1, gp->sThumbLX, gp->sThumbLY ));
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
  assert( deltax && deltay );
#ifndef USE_HACKEDLIGHTGUN
  GetLightgunCalibratedPosition( player, deltax, deltay );
#else
  *deltax = *deltay = 0;
#endif
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
  const XINPUT_CAPABILITIES *gpCaps;
	
//	PRINTMSG(( T_TRACE, "osd_analogjoy_read" ));

	gpCaps = GetGamepadCaps( player );
	gamepad = GetGamepadState( player );
	if( !gamepad )
	{
//		PRINTMSG(( T_ERROR, "Invalid joynum %lu!", player ));
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
      // *** JT_LSTICK_UP *** //
	  case JT_LSTICK_UP:
#ifdef USE_HACKEDLIGHTGUN
      if( gpCaps->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
      {
        INT32 junk;
        GetLightgunCalibratedPosition( joynum, &junk, &analog_axis[i] );
        analogDivisor = 128.0f;
      }
      else
#endif
      {
        if( gamepad->sThumbLY > ANALOG_DEADZONE )
          analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLY - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
        else if( gamepad->sThumbLY < -ANALOG_DEADZONE )
          analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLY + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
        else
          analog_axis[i] = 0;

        // MAME seems to expect - values for Y axes to be "Up", so we need to
        // negate
        analog_axis[i] *= -1;
      }
      break;

      // *** JT_LSTICK_DOWN *** //
	  case JT_LSTICK_DOWN:
#ifdef USE_HACKEDLIGHTGUN
      if( gpCaps->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
      {
        INT32 junk;
        GetLightgunCalibratedPosition( joynum, &junk, &analog_axis[i] );
        analogDivisor = 128.0f;
      }
      else
#endif
      {
        if( gamepad->sThumbLY > ANALOG_DEADZONE )
          analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLY - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
        else if( gamepad->sThumbLY < -ANALOG_DEADZONE )
          analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLY + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
        else
          analog_axis[i] = 0;

        // MAME seems to expect - values for Y axes to be "Up", so we need to
        // negate
        analog_axis[i] *= -1;
      }
			break;

      // *** JT_LSTICK_LEFT *** //
    case JT_LSTICK_LEFT:
#ifdef USE_HACKEDLIGHTGUN
      if( gpCaps->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
      {
        INT32 junk;
        GetLightgunCalibratedPosition( joynum, &analog_axis[i], &junk );
        analogDivisor = 128.0f;
      }
      else
#endif
      {
        if( gamepad->sThumbLX < -ANALOG_DEADZONE )
          analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLX + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
        else if( gamepad->sThumbLX > ANALOG_DEADZONE )
          analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLX - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
        else
          analog_axis[i] = 0;
      }
      break;

      // *** JT_LSTICK_RIGHT *** //
	  case JT_LSTICK_RIGHT:
#ifdef USE_HACKEDLIGHTGUN
      if( gpCaps->SubType == XINPUT_DEVSUBTYPE_GC_LIGHTGUN )
      {
        INT32 junk;
        GetLightgunCalibratedPosition( joynum, &analog_axis[i], &junk );
        analogDivisor = 128.0f;
      }
      else
#endif
      {
        if( gamepad->sThumbLX < -ANALOG_DEADZONE )
          analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLX + ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
        else if( gamepad->sThumbLX > ANALOG_DEADZONE )
          analog_axis[i] = (int)((FLOAT)(gamepad->sThumbLX - ANALOG_DEADZONE) * DEADZONE_RECTIFIER);
        else
          analog_axis[i] = 0;
      }
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

#pragma code_seg( "MXCINPUT" )
#pragma data_seg( "MXDINPUT" )
#pragma comment(linker, "/merge:MXCINPUT=MOXINPUT")
#pragma comment(linker, "/merge:MXDINPUT=MOXINPUT")

//---------------------------------------------------------------------
//	InitializeJoystickMouse
//---------------------------------------------------------------------
void InitializeJoystickMouse( void )
{
  BEGINENTRYMAP();
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
}

//-------------------------------------------------------
//	Helper_AddEntry
//-------------------------------------------------------
static void Helper_AddEntry( const char *name, INT32 code, INT32 standardCode, UINT32 *joycount )
{
  struct JoystickInfo *ji = NULL;
	struct ik *inputkeywords;

  assert( joycount && *joycount < MAX_JOYSTICKINFO_ENTRIES );

  ji = &g_joystickInfo[*joycount];

  ji->name = strdup( name );
  if( !ji->name )
  {
    PRINTMSG(( T_ERROR, "Out of memory creating joystick entry!" ));
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
	inputkeywords = (struct ik*)osd_realloc( osd_input_keywords, (g_numOSDInputKeywords + 2) * sizeof(struct ik) );
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
#pragma data_seg()



