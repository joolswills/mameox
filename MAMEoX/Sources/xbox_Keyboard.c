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

/*
// master translation table
static int key_trans_table[][4] =
{
	// MAME key				dinput key			virtual key		ascii
	{ KEYCODE_ESC, 			DIK_ESCAPE,			VK_ESCAPE,	 	27 },
	{ KEYCODE_1, 			DIK_1,				'1',			'1' },
	{ KEYCODE_2, 			DIK_2,				'2',			'2' },
	{ KEYCODE_3, 			DIK_3,				'3',			'3' },
	{ KEYCODE_4, 			DIK_4,				'4',			'4' },
	{ KEYCODE_5, 			DIK_5,				'5',			'5' },
	{ KEYCODE_6, 			DIK_6,				'6',			'6' },
	{ KEYCODE_7, 			DIK_7,				'7',			'7' },
	{ KEYCODE_8, 			DIK_8,				'8',			'8' },
	{ KEYCODE_9, 			DIK_9,				'9',			'9' },
	{ KEYCODE_0, 			DIK_0,				'0',			'0' },
	{ KEYCODE_MINUS, 		DIK_MINUS, 			0xbd,			'-' },
	{ KEYCODE_EQUALS, 		DIK_EQUALS,		 	0xbb,			'=' },
	{ KEYCODE_BACKSPACE,	DIK_BACK, 			VK_BACK, 		8 },
	{ KEYCODE_TAB, 			DIK_TAB, 			VK_TAB, 		9 },
	{ KEYCODE_Q, 			DIK_Q,				'Q',			'Q' },
	{ KEYCODE_W, 			DIK_W,				'W',			'W' },
	{ KEYCODE_E, 			DIK_E,				'E',			'E' },
	{ KEYCODE_R, 			DIK_R,				'R',			'R' },
	{ KEYCODE_T, 			DIK_T,				'T',			'T' },
	{ KEYCODE_Y, 			DIK_Y,				'Y',			'Y' },
	{ KEYCODE_U, 			DIK_U,				'U',			'U' },
	{ KEYCODE_I, 			DIK_I,				'I',			'I' },
	{ KEYCODE_O, 			DIK_O,				'O',			'O' },
	{ KEYCODE_P, 			DIK_P,				'P',			'P' },
	{ KEYCODE_OPENBRACE,	DIK_LBRACKET, 		0xdb,			'[' },
	{ KEYCODE_CLOSEBRACE,	DIK_RBRACKET, 		0xdd,			']' },
	{ KEYCODE_ENTER, 		DIK_RETURN, 		VK_RETURN, 		13 },
	{ KEYCODE_LCONTROL, 	DIK_LCONTROL, 		VK_CONTROL, 	0 },
	{ KEYCODE_A, 			DIK_A,				'A',			'A' },
	{ KEYCODE_S, 			DIK_S,				'S',			'S' },
	{ KEYCODE_D, 			DIK_D,				'D',			'D' },
	{ KEYCODE_F, 			DIK_F,				'F',			'F' },
	{ KEYCODE_G, 			DIK_G,				'G',			'G' },
	{ KEYCODE_H, 			DIK_H,				'H',			'H' },
	{ KEYCODE_J, 			DIK_J,				'J',			'J' },
	{ KEYCODE_K, 			DIK_K,				'K',			'K' },
	{ KEYCODE_L, 			DIK_L,				'L',			'L' },
	{ KEYCODE_COLON, 		DIK_SEMICOLON,		0xba,			';' },
	{ KEYCODE_QUOTE, 		DIK_APOSTROPHE,		0xde,			'\'' },
	{ KEYCODE_TILDE, 		DIK_GRAVE, 			0xc0,			'`' },
	{ KEYCODE_LSHIFT, 		DIK_LSHIFT, 		VK_SHIFT, 		0 },
	{ KEYCODE_BACKSLASH,	DIK_BACKSLASH, 		0xdc,			'\\' },
	{ KEYCODE_Z, 			DIK_Z,				'Z',			'Z' },
	{ KEYCODE_X, 			DIK_X,				'X',			'X' },
	{ KEYCODE_C, 			DIK_C,				'C',			'C' },
	{ KEYCODE_V, 			DIK_V,				'V',			'V' },
	{ KEYCODE_B, 			DIK_B,				'B',			'B' },
	{ KEYCODE_N, 			DIK_N,				'N',			'N' },
	{ KEYCODE_M, 			DIK_M,				'M',			'M' },
	{ KEYCODE_COMMA, 		DIK_COMMA,			0xbc,			',' },
	{ KEYCODE_STOP, 		DIK_PERIOD, 		0xbe,			'.' },
	{ KEYCODE_SLASH, 		DIK_SLASH, 			0xbf,			'/' },
	{ KEYCODE_RSHIFT, 		DIK_RSHIFT, 		VK_SHIFT, 		0 },
	{ KEYCODE_ASTERISK, 	DIK_MULTIPLY, 		VK_MULTIPLY,	'*' },
	{ KEYCODE_LALT, 		DIK_LMENU, 			VK_MENU, 		0 },
	{ KEYCODE_SPACE, 		DIK_SPACE, 			VK_SPACE,		' ' },
	{ KEYCODE_CAPSLOCK, 	DIK_CAPITAL, 		VK_CAPITAL, 	0 },
	{ KEYCODE_F1, 			DIK_F1,				VK_F1, 			0 },
	{ KEYCODE_F2, 			DIK_F2,				VK_F2, 			0 },
	{ KEYCODE_F3, 			DIK_F3,				VK_F3, 			0 },
	{ KEYCODE_F4, 			DIK_F4,				VK_F4, 			0 },
	{ KEYCODE_F5, 			DIK_F5,				VK_F5, 			0 },
	{ KEYCODE_F6, 			DIK_F6,				VK_F6, 			0 },
	{ KEYCODE_F7, 			DIK_F7,				VK_F7, 			0 },
	{ KEYCODE_F8, 			DIK_F8,				VK_F8, 			0 },
	{ KEYCODE_F9, 			DIK_F9,				VK_F9, 			0 },
	{ KEYCODE_F10, 			DIK_F10,			VK_F10, 		0 },
	{ KEYCODE_NUMLOCK, 		DIK_NUMLOCK,		VK_NUMLOCK, 	0 },
	{ KEYCODE_SCRLOCK, 		DIK_SCROLL,			VK_SCROLL, 		0 },
	{ KEYCODE_7_PAD, 		DIK_NUMPAD7,		VK_NUMPAD7, 	0 },
	{ KEYCODE_8_PAD, 		DIK_NUMPAD8,		VK_NUMPAD8, 	0 },
	{ KEYCODE_9_PAD, 		DIK_NUMPAD9,		VK_NUMPAD9, 	0 },
	{ KEYCODE_MINUS_PAD,	DIK_SUBTRACT,		VK_SUBTRACT, 	0 },
	{ KEYCODE_4_PAD, 		DIK_NUMPAD4,		VK_NUMPAD4, 	0 },
	{ KEYCODE_5_PAD, 		DIK_NUMPAD5,		VK_NUMPAD5, 	0 },
	{ KEYCODE_6_PAD, 		DIK_NUMPAD6,		VK_NUMPAD6, 	0 },
	{ KEYCODE_PLUS_PAD, 	DIK_ADD,			VK_ADD, 		0 },
	{ KEYCODE_1_PAD, 		DIK_NUMPAD1,		VK_NUMPAD1, 	0 },
	{ KEYCODE_2_PAD, 		DIK_NUMPAD2,		VK_NUMPAD2, 	0 },
	{ KEYCODE_3_PAD, 		DIK_NUMPAD3,		VK_NUMPAD3, 	0 },
	{ KEYCODE_0_PAD, 		DIK_NUMPAD0,		VK_NUMPAD0, 	0 },
	{ KEYCODE_DEL_PAD, 		DIK_DECIMAL,		VK_DECIMAL, 	0 },
	{ KEYCODE_F11, 			DIK_F11,			VK_F11, 		0 },
	{ KEYCODE_F12, 			DIK_F12,			VK_F12, 		0 },
	{ KEYCODE_OTHER, 		DIK_F13,			VK_F13, 		0 },
	{ KEYCODE_OTHER, 		DIK_F14,			VK_F14, 		0 },
	{ KEYCODE_OTHER, 		DIK_F15,			VK_F15, 		0 },
	{ KEYCODE_ENTER_PAD,	DIK_NUMPADENTER,	VK_RETURN, 		0 },
	{ KEYCODE_RCONTROL, 	DIK_RCONTROL,		VK_CONTROL, 	0 },
	{ KEYCODE_SLASH_PAD,	DIK_DIVIDE,			VK_DIVIDE, 		0 },
	{ KEYCODE_PRTSCR, 		DIK_SYSRQ, 			0, 				0 },
	{ KEYCODE_RALT, 		DIK_RMENU,			VK_MENU, 		0 },
	{ KEYCODE_HOME, 		DIK_HOME,			VK_HOME, 		0 },
	{ KEYCODE_UP, 			DIK_UP,				VK_UP, 			0 },
	{ KEYCODE_PGUP, 		DIK_PRIOR,			VK_PRIOR, 		0 },
	{ KEYCODE_LEFT, 		DIK_LEFT,			VK_LEFT, 		0 },
	{ KEYCODE_RIGHT, 		DIK_RIGHT,			VK_RIGHT, 		0 },
	{ KEYCODE_END, 			DIK_END,			VK_END, 		0 },
	{ KEYCODE_DOWN, 		DIK_DOWN,			VK_DOWN, 		0 },
	{ KEYCODE_PGDN, 		DIK_NEXT,			VK_NEXT, 		0 },
	{ KEYCODE_INSERT, 		DIK_INSERT,			VK_INSERT, 		0 },
	{ KEYCODE_DEL, 			DIK_DELETE,			VK_DELETE, 		0 },
	{ KEYCODE_LWIN, 		DIK_LWIN,			VK_LWIN, 		0 },
	{ KEYCODE_RWIN, 		DIK_RWIN,			VK_RWIN, 		0 },
	{ KEYCODE_MENU, 		DIK_APPS,			VK_APPS, 		0 }
};
*/

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