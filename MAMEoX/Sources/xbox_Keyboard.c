/**
	* \file			xbox_JoystickMouse.c
	* \brief		Implementation of the "Joystick & Mouse/Trackball" section of
	*           osdepend.h
	*/

//= I N C L U D E S ====================================================
#include "osd_cpu.h"
#include "osdepend.h"

#include "DebugLogger.h"



//= G L O B A L = V A R S ==============================================
static struct KeyboardInfo		g_keyboardInfo = {0};

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	osd_get_key_list
//---------------------------------------------------------------------
const struct KeyboardInfo *osd_get_key_list( void )
{
/*
  return a list of all available keys (see input.h)
*/
//	PRINTMSG( T_TRACE, "KeyboardInfo" );

	return &g_keyboardInfo;
}

//---------------------------------------------------------------------
//	osd_is_key_pressed
//---------------------------------------------------------------------
int osd_is_key_pressed(int keycode)
{
/*
  tell whether the specified key is pressed or not. keycode is the OS dependant
  code specified in the list returned by osd_get_key_list().
*/
//	PRINTMSG( T_TRACE, "osd_is_key_pressed" );
	return 0;
}

//---------------------------------------------------------------------
//	osd_readkey_unicode
//---------------------------------------------------------------------
int osd_readkey_unicode(int flush)
{
/*
  Return the Unicode value of the most recently pressed key. This
  function is used only by text-entry routines in the user interface and should
  not be used by drivers. The value returned is in the range of the first 256
  bytes of Unicode, e.g. ISO-8859-1. A return value of 0 indicates no key down.

  Set flush to 1 to clear the buffer before entering text. This will avoid
  having prior UI and game keys leak into the text entry.
*/
//	PRINTMSG( T_TRACE, "osd_readkey_unicode" );
	return 0;
}