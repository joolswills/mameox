/**
	* \file			xbox_Keyboard.c
	* \brief		Implementation of the "Keyboard" section of
	*           osdepend.h
	*/

//= I N C L U D E S ====================================================
#include "osd_cpu.h"
#include "osdepend.h"
#include <assert.h>

#include "xbox_Keyboard.h"
#include "InputManager.h"
#include "DebugLogger.h"

//= D E F I N E S ======================================================
#define MAX_SUPPORTED_KEYS    128

  // Helper macro, calls Helper_AddEntry
#define BEGINENTRYMAP()                                  UINT32 keycount = 0;
#define ADDENTRY( _name__, _code__, _standardCode__ )    Helper_AddEntry( (_name__), (_code__), (_standardCode__), &keycount )

//= G L O B A L = V A R S ==============================================
extern CInputManager			  g_inputManager; // Defined in MAMEoXUtil.cpp


  // number of OSD input keywords we've added (defined in xbox_Input.c)
extern "C" {
  extern UINT32					  g_numOSDInputKeywords;
}

  // The keyboard info struct for the MAME core
static struct KeyboardInfo		g_keyboardInfo[MAX_SUPPORTED_KEYS] = { 0 };

//= P R O T O T Y P E S ================================================
	//------------------------------------------------------------------
	//	Helper_AddEntry
  //! \brief    Adds an entry into the g_keyboardInfo array
  //!
  //! \param    name - The user friendly name of the entry
  //! \param    code - The OSD-specific identifier of the entry
  //! \param    standardCode - The "standard" input code of the entry (see input.h in the MAME tree)
  //! \param    keycount - [IN,OUT] The index in the KeyboardInfo array to modify. Incremented on success
  //!
  //! \note     standardCode may be set to a value obtained via joyoscode_to_code for any codes 
  //!           w/out a specific entry in the keycode/keyboard enum (defined in input.h of the MAME tree)
	//------------------------------------------------------------------
static void Helper_AddEntry( const char *name, BYTE code, INT32 standardCode, UINT32 *keycount );

//= F U N C T I O N S ==================================================

//---------------------------------------------------------------------
//	osd_get_key_list
//---------------------------------------------------------------------
const struct KeyboardInfo *osd_get_key_list( void )
{
/*
  return a list of all available keys (see input.h)
*/
	return g_keyboardInfo;
}

//---------------------------------------------------------------------
//	osd_is_key_pressed
//---------------------------------------------------------------------
int osd_is_key_pressed( int keycode )
{
/*
  tell whether the specified key is pressed or not. keycode is the OS dependant
  code specified in the list returned by osd_get_key_list().
*/
	return g_inputManager.IsKeyPressed( keycode );
}

//---------------------------------------------------------------------
//	osd_readkey_unicode
//---------------------------------------------------------------------
int osd_readkey_unicode( int flush )
{
/*
  Return the Unicode value of the most recently pressed key. This
  function is used only by text-entry routines in the user interface and should
  not be used by drivers. The value returned is in the range of the first 256
  bytes of Unicode, e.g. ISO-8859-1. A return value of 0 indicates no key down.

  Set flush to 1 to clear the buffer before entering text. This will avoid
  having prior UI and game keys leak into the text entry.
*/
//	PRINTMSG(( T_TRACE, "osd_readkey_unicode" ));
	return 0;
}




#pragma code_seg( "MXCINPUT" )
#pragma data_seg( "MXDINPUT" )
#pragma comment(linker, "/merge:MXCINPUT=MOXINPUT")
#pragma comment(linker, "/merge:MXDINPUT=MOXINPUT")

//---------------------------------------------------------------------
//	InitializeKeyboard
//---------------------------------------------------------------------
void InitializeKeyboard( void )
{
  BEGINENTRYMAP();

    // Add the letters
  char str[2] = { 'A', 0 };
  UINT32 i = 0;
  for( ; i < KEYCODE_Z - KEYCODE_A; ++i )
  {
    str[0] = 'A' + i;
    ADDENTRY( str, str[0], KEYCODE_A + i );
  }

  ADDENTRY( "1", '1', KEYCODE_1 );
  ADDENTRY( "2", '2', KEYCODE_2 );
  ADDENTRY( "3", '3', KEYCODE_3 );
  ADDENTRY( "4", '4', KEYCODE_4 );
  ADDENTRY( "5", '5', KEYCODE_5 );
  ADDENTRY( "6", '6', KEYCODE_6 );
  ADDENTRY( "7", '7', KEYCODE_7 );
  ADDENTRY( "8", '8', KEYCODE_8 );
  ADDENTRY( "9", '9', KEYCODE_9 );
  ADDENTRY( "0", '0', KEYCODE_0 );

  ADDENTRY( "NumPad 1", VK_NUMPAD1, KEYCODE_1_PAD );
  ADDENTRY( "NumPad 2", VK_NUMPAD2, KEYCODE_2_PAD );
  ADDENTRY( "NumPad 3", VK_NUMPAD3, KEYCODE_3_PAD );
  ADDENTRY( "NumPad 4", VK_NUMPAD4, KEYCODE_4_PAD );
  ADDENTRY( "NumPad 5", VK_NUMPAD5, KEYCODE_5_PAD );
  ADDENTRY( "NumPad 6", VK_NUMPAD6, KEYCODE_6_PAD );
  ADDENTRY( "NumPad 7", VK_NUMPAD7, KEYCODE_7_PAD );
  ADDENTRY( "NumPad 8", VK_NUMPAD8, KEYCODE_8_PAD );
  ADDENTRY( "NumPad 9", VK_NUMPAD9, KEYCODE_9_PAD );
  ADDENTRY( "NumPad 0", VK_NUMPAD0, KEYCODE_0_PAD );
  ADDENTRY( "NumPad /", VK_DIVIDE, KEYCODE_SLASH_PAD );
  ADDENTRY( "NumPad -", VK_SUBTRACT, KEYCODE_MINUS_PAD );
  ADDENTRY( "NumPad *", VK_MULTIPLY, KEYCODE_ASTERISK );
  ADDENTRY( "NumPad +", VK_ADD, KEYCODE_PLUS_PAD );
  ADDENTRY( "NumPad Del", VK_DECIMAL, KEYCODE_DEL_PAD );
  ADDENTRY( "NumPad Enter", VK_SEPARATOR, KEYCODE_ENTER_PAD );

  ADDENTRY( "F1", VK_F1, KEYCODE_F1 );
  ADDENTRY( "F2", VK_F2, KEYCODE_F2 );
  ADDENTRY( "F3", VK_F3, KEYCODE_F3 );
  ADDENTRY( "F4", VK_F4, KEYCODE_F4 );
  ADDENTRY( "F5", VK_F5, KEYCODE_F5 );
  ADDENTRY( "F6", VK_F6, KEYCODE_F6 );
  ADDENTRY( "F7", VK_F7, KEYCODE_F7 );
  ADDENTRY( "F8", VK_F8, KEYCODE_F8 );
  ADDENTRY( "F9", VK_F9, KEYCODE_F9 );
  ADDENTRY( "F10", VK_F10, KEYCODE_F10 );
  ADDENTRY( "F11", VK_F11, KEYCODE_F11 );
  ADDENTRY( "F12", VK_F12, KEYCODE_F12 );

  ADDENTRY( "Escape", VK_ESCAPE, KEYCODE_ESC );
  ADDENTRY( "Tilde", VK_OEM_3, KEYCODE_TILDE );
  
  ADDENTRY( "Minus", VK_OEM_MINUS, KEYCODE_MINUS );
  ADDENTRY( "Equals", VK_OEM_PLUS, KEYCODE_EQUALS );
  ADDENTRY( "Backspace", VK_BACK, KEYCODE_BACKSPACE );
  ADDENTRY( "Tab", VK_TAB, KEYCODE_TAB );

  ADDENTRY( "[", VK_OEM_4, KEYCODE_OPENBRACE );
  ADDENTRY( "]", VK_OEM_6, KEYCODE_CLOSEBRACE );

  ADDENTRY( "Enter", VK_RETURN, KEYCODE_ENTER );

  ADDENTRY( ":", VK_OEM_1, KEYCODE_COLON );
  ADDENTRY( "'", VK_OEM_7, KEYCODE_QUOTE );
	

  ADDENTRY( "\\", VK_OEM_5, KEYCODE_BACKSLASH );
  ADDENTRY( "\\", VK_OEM_5, KEYCODE_BACKSLASH2 );

  ADDENTRY( "Comma", VK_OEM_COMMA, KEYCODE_COMMA );
  ADDENTRY( "Period", VK_OEM_PERIOD, KEYCODE_STOP );
  ADDENTRY( "/", VK_OEM_2, KEYCODE_SLASH );
	
  ADDENTRY( "Space", VK_SPACE, KEYCODE_SPACE );
  
  ADDENTRY( "Insert", VK_INSERT, KEYCODE_INSERT );
  ADDENTRY( "Delete", VK_DELETE, KEYCODE_DEL );
  

  ADDENTRY( "Home", VK_HOME, KEYCODE_HOME );
  ADDENTRY( "End", VK_END, KEYCODE_END );
  
  ADDENTRY( "Page up", VK_PRIOR, KEYCODE_PGUP );
  ADDENTRY( "Page down", VK_NEXT, KEYCODE_PGDN );
  

  ADDENTRY( "Left", VK_LEFT, KEYCODE_LEFT );
  ADDENTRY( "Right", VK_RIGHT, KEYCODE_RIGHT );
  ADDENTRY( "Up", VK_UP, KEYCODE_UP );
  ADDENTRY( "Down", VK_DOWN, KEYCODE_DOWN );
  
  ADDENTRY( "Print screen", VK_PRINT, KEYCODE_PRTSCR );
  ADDENTRY( "Pause", VK_PAUSE, KEYCODE_PAUSE );
  
  ADDENTRY( "Left shift", VK_LSHIFT, KEYCODE_LSHIFT );
  ADDENTRY( "Right shift", VK_RSHIFT, KEYCODE_RSHIFT );
  ADDENTRY( "Left ctrl", VK_LCONTROL, KEYCODE_LCONTROL );
  ADDENTRY( "Right ctrl", VK_RCONTROL, KEYCODE_RCONTROL );
  ADDENTRY( "Left alt", VK_LMENU, KEYCODE_LALT );
  ADDENTRY( "Right alt", VK_RMENU, KEYCODE_RALT );
  
  ADDENTRY( "Scroll lock", VK_SCROLL, KEYCODE_SCRLOCK );
  ADDENTRY( "Num lock", VK_NUMLOCK, KEYCODE_NUMLOCK );
  ADDENTRY( "Caps lock", VK_CAPITAL, KEYCODE_CAPSLOCK );

  ADDENTRY( "Left win", VK_LWIN, KEYCODE_LWIN );
  ADDENTRY( "Right win", VK_RWIN, KEYCODE_RWIN );
  ADDENTRY( "Apps", VK_APPS, KEYCODE_MENU );
  
}


//-------------------------------------------------------
//	Helper_AddEntry
//-------------------------------------------------------
static void Helper_AddEntry( const char *name, BYTE code, INT32 standardCode, UINT32 *keycount )
{
  struct KeyboardInfo &ki = g_keyboardInfo[*keycount];

  assert( name && keycount && *keycount < MAX_SUPPORTED_KEYS );

  ki.code = code;
  ki.standardcode = standardCode;
  ki.name = strdup( name );
  if( !ki.name )
  {
    PRINTMSG(( T_ERROR, "Out of memory creating keyboard entry!" ));
    osd_print_error( "Out of memory!" );
    return;
  }

    // Convert spaces in name to underscores
  char *cur = ki.name;
  while( *cur )
  {
    if( *cur == ' ' )
      *cur = '_';
    ++cur;
  }


    // Reallocate the osd_input_keywords array, and add the new entry
  struct ik *inputkeywords = (struct ik*)osd_realloc( osd_input_keywords, (g_numOSDInputKeywords + 2) * sizeof(struct ik) );
  if( inputkeywords )
  {
    osd_input_keywords = inputkeywords;
    osd_input_keywords[g_numOSDInputKeywords].name = ki.name;  // Just share the value
    osd_input_keywords[g_numOSDInputKeywords].type = IKT_OSD_KEY;
    osd_input_keywords[g_numOSDInputKeywords].val = code;
    ++g_numOSDInputKeywords;

      // Tag the end of the list
		osd_input_keywords[g_numOSDInputKeywords].name = NULL;
    ++(*keycount);
  }
}

#pragma code_seg()
#pragma data_seg()


