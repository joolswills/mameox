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
#include "xbox_Keyboard.h"

#include "DebugLogger.h"

//= D E F I N E S ======================================================
  //! Macros for redefining input sequences
#define REMAP_SEQ_20(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t) { InputSeq newSeq = SEQ_DEF_20((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p),(q),(r),(s),(t)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_19(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)   { InputSeq newSeq = SEQ_DEF_19((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p),(q),(r),(s)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_18(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)     { InputSeq newSeq = SEQ_DEF_18((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p),(q),(r)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_17(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)       { InputSeq newSeq = SEQ_DEF_17((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p),(q)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)         { InputSeq newSeq = SEQ_DEF_16((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o),(p)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)           { InputSeq newSeq = SEQ_DEF_15((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n),(o)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n)             { InputSeq newSeq = SEQ_DEF_14((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m),(n)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_13(a,b,c,d,e,f,g,h,i,j,k,l,m)               { InputSeq newSeq = SEQ_DEF_13((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l),(m)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_12(a,b,c,d,e,f,g,h,i,j,k,l)                 { InputSeq newSeq = SEQ_DEF_12((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_11(a,b,c,d,e,f,g,h,i,j,k)                   { InputSeq newSeq = SEQ_DEF_11((a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_10(a,b,c,d,e,f,g,h,i,j)                     { InputSeq newSeq = SEQ_DEF_10((a),(b),(c),(d),(e),(f),(g),(h),(i),(j)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_9(a,b,c,d,e,f,g,h,i)                        { InputSeq newSeq = SEQ_DEF_9((a),(b),(c),(d),(e),(f),(g),(h),(i)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_8(a,b,c,d,e,f,g,h)                          { InputSeq newSeq = SEQ_DEF_8((a),(b),(c),(d),(e),(f),(g),(h)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_7(a,b,c,d,e,f,g)                            { InputSeq newSeq = SEQ_DEF_7((a),(b),(c),(d),(e),(f),(g)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_6(a,b,c,d,e,f)                              { InputSeq newSeq = SEQ_DEF_6((a),(b),(c),(d),(e),(f)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_5(a,b,c,d,e)                                { InputSeq newSeq = SEQ_DEF_5((a),(b),(c),(d),(e)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_4(a,b,c,d)                                  { InputSeq newSeq = SEQ_DEF_4((a),(b),(c),(d)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_3(a,b,c)                                    { InputSeq newSeq = SEQ_DEF_3((a),(b),(c)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_2(a,b)                                      { InputSeq newSeq = SEQ_DEF_2((a),(b)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_1(a)                                        { InputSeq newSeq = SEQ_DEF_1((a)); memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }
#define REMAP_SEQ_0()				                                  { InputSeq newSeq = SEQ_DEF_0; memcpy( entry->seq, newSeq, sizeof(entry->seq) ); }



//= G L O B A L = V A R S ==============================================
  //! Whether or not the "MOXINPUT" XBE section has been unloaded
static BOOL                     g_MOXINPUTSectionUnloaded = FALSE;
static BOOL                     g_systemInitialized = FALSE;

UINT32										      g_numOSDInputKeywords = 0;


//= P R O T O T Y P E S ================================================
static void Helper_CustomizeInputPortDefaults( struct ipd *defaults );


//= F U N C T I O N S ==================================================

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
  if( g_MOXINPUTSectionUnloaded )
  {
    PRINTMSG( T_INFO, "Loading MOXINPUT section!" );
    XLoadSection( "MOXINPUT" );
    g_MOXINPUTSectionUnloaded = FALSE;
  }

  if( !g_systemInitialized )
  {
    InitializeJoystickMouse();
    InitializeKeyboard();
    g_systemInitialized = TRUE;
  }

  Helper_CustomizeInputPortDefaults( defaults );

  if( !g_MOXINPUTSectionUnloaded )
  {
    PRINTMSG( T_INFO, "Unloading MOXINPUT section!" );
    XFreeSection( "MOXINPUT" );
    g_MOXINPUTSectionUnloaded = TRUE;
  }
}




#pragma code_seg( "MXCINPUT" )
#pragma data_seg( "MXDINPUT" )
#pragma comment(linker, "/merge:MXCINPUT=MOXINPUT")
#pragma comment(linker, "/merge:MXDINPUT=MOXINPUT")

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
      REMAP_SEQ_9(  JOYCODE_1_BUTTON1, CODE_OR,
                    JOYCODE_2_BUTTON1, CODE_OR,
                    JOYCODE_3_BUTTON1, CODE_OR,
                    JOYCODE_4_BUTTON1, CODE_OR,
                    KEYCODE_ENTER );
      break;

			// *** IPT_UI_CANCEL *** //
		case IPT_UI_CANCEL:
				// Remap cancel to gamepad0 back + start
      REMAP_SEQ_13( JOYCODE_1_SELECT, JOYCODE_1_START, CODE_OR, 
                    JOYCODE_2_SELECT, JOYCODE_2_START, CODE_OR,
                    JOYCODE_3_SELECT, JOYCODE_3_START, CODE_OR,
                    JOYCODE_4_SELECT, JOYCODE_4_START, CODE_OR,
                    KEYCODE_ESC );
			break;

			// *** IPT_UI_TOGGLE_CROSSHAIR *** //
    case IPT_UI_TOGGLE_CROSSHAIR:
      REMAP_SEQ_13( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_LA ), CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_LA ), CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_LA ), CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_LA ), CODE_OR,
                    KEYCODE_F1 );
      break;

			// *** IPT_UI_CONFIGURE *** //
    case IPT_UI_CONFIGURE:
      REMAP_SEQ_13( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_WHITE ), CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_WHITE ), CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_WHITE ), CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_WHITE ), CODE_OR,
                    KEYCODE_TAB );
      break;

			// *** IPT_UI_PAUSE *** //
    case IPT_UI_PAUSE:
      REMAP_SEQ_13( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_BLACK ), CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_BLACK ), CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_BLACK ), CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_BLACK ), CODE_OR,
                    KEYCODE_P );
      break;

			// *** IPT_UI_ON_SCREEN_DISPLAY *** //
    case IPT_UI_ON_SCREEN_DISPLAY:
      REMAP_SEQ_13( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_RA ), CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_RA ), CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_RA ), CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_RA ), CODE_OR,
                    KEYCODE_TILDE );
      break;

			// *** IPT_UI_RESET_MACHINE *** //
    case IPT_UI_RESET_MACHINE:
      REMAP_SEQ_17( JOYCODE_1_SELECT, JOYCODE_1_BUTTON5, JOYCODE_1_BUTTON6, CODE_OR, 
                    JOYCODE_2_SELECT, JOYCODE_2_BUTTON5, JOYCODE_2_BUTTON6, CODE_OR,
                    JOYCODE_3_SELECT, JOYCODE_3_BUTTON5, JOYCODE_3_BUTTON6, CODE_OR,
                    JOYCODE_4_SELECT, JOYCODE_4_BUTTON5, JOYCODE_4_BUTTON6, CODE_OR,
                    KEYCODE_F3 );
      break;

			// *** IPT_UI_SAVE_STATE *** //
    case IPT_UI_SAVE_STATE:
      REMAP_SEQ_18( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_RA ), JOYCODE_1_BUTTON6, CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_RA ), JOYCODE_2_BUTTON6, CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_RA ), JOYCODE_3_BUTTON6, CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_RA ), JOYCODE_4_BUTTON6, CODE_OR,
                    KEYCODE_F7, KEYCODE_LSHIFT );
      break;

			// *** IPT_UI_LOAD_STATE *** //
    case IPT_UI_LOAD_STATE:
      REMAP_SEQ_19( JOYCODE_1_SELECT, BUTTONCODE( 0, BUTTON_RA ), JOYCODE_1_BUTTON5, CODE_OR, 
                    JOYCODE_2_SELECT, BUTTONCODE( 1, BUTTON_RA ), JOYCODE_1_BUTTON5, CODE_OR,
                    JOYCODE_3_SELECT, BUTTONCODE( 2, BUTTON_RA ), JOYCODE_1_BUTTON5, CODE_OR,
                    JOYCODE_4_SELECT, BUTTONCODE( 3, BUTTON_RA ), JOYCODE_1_BUTTON5, CODE_OR,
                    KEYCODE_F7, CODE_NOT, KEYCODE_LSHIFT );
      break;

			// *** IPT_UI_UP *** //
    case IPT_UI_UP:
      REMAP_SEQ_17( JOYCODE_1_UP, CODE_OR, AXISCODE( 0, JT_LSTICK_UP ), CODE_OR,
                    JOYCODE_2_UP, CODE_OR, AXISCODE( 1, JT_LSTICK_UP ), CODE_OR,
                    JOYCODE_3_UP, CODE_OR, AXISCODE( 2, JT_LSTICK_UP ), CODE_OR,
                    JOYCODE_4_UP, CODE_OR, AXISCODE( 3, JT_LSTICK_UP ), CODE_OR,
                    KEYCODE_UP );
      break;

			// *** IPT_UI_LEFT *** //
    case IPT_UI_LEFT:
      REMAP_SEQ_17( JOYCODE_1_LEFT, CODE_OR, AXISCODE( 0, JT_LSTICK_LEFT ), CODE_OR,
                    JOYCODE_2_LEFT, CODE_OR, AXISCODE( 1, JT_LSTICK_LEFT ), CODE_OR,
                    JOYCODE_3_LEFT, CODE_OR, AXISCODE( 2, JT_LSTICK_LEFT ), CODE_OR,
                    JOYCODE_4_LEFT, CODE_OR, AXISCODE( 3, JT_LSTICK_LEFT ), CODE_OR,
                    KEYCODE_LEFT );
      break;

			// *** IPT_UI_DOWN *** //
    case IPT_UI_DOWN:
      REMAP_SEQ_17( JOYCODE_1_DOWN, CODE_OR, AXISCODE( 0, JT_LSTICK_DOWN ), CODE_OR,
                    JOYCODE_2_DOWN, CODE_OR, AXISCODE( 1, JT_LSTICK_DOWN ), CODE_OR,
                    JOYCODE_3_DOWN, CODE_OR, AXISCODE( 2, JT_LSTICK_DOWN ), CODE_OR,
                    JOYCODE_4_DOWN, CODE_OR, AXISCODE( 3, JT_LSTICK_DOWN ), CODE_OR,
                    KEYCODE_DOWN );
      break;

			// *** IPT_UI_RIGHT *** //
    case IPT_UI_RIGHT:
      REMAP_SEQ_17( JOYCODE_1_RIGHT, CODE_OR, AXISCODE( 0, JT_LSTICK_RIGHT ), CODE_OR,
                    JOYCODE_2_RIGHT, CODE_OR, AXISCODE( 1, JT_LSTICK_RIGHT ), CODE_OR,
                    JOYCODE_3_RIGHT, CODE_OR, AXISCODE( 2, JT_LSTICK_RIGHT ), CODE_OR,
                    JOYCODE_4_RIGHT, CODE_OR, AXISCODE( 3, JT_LSTICK_RIGHT ), CODE_OR,
                    KEYCODE_RIGHT );
      break;


			// *** IPT_UI_SNAPSHOT *** //
    case IPT_UI_SNAPSHOT:
      REMAP_SEQ_17( JOYCODE_1_SELECT, JOYCODE_1_BUTTON1, JOYCODE_1_BUTTON3, CODE_OR, 
                    JOYCODE_2_SELECT, JOYCODE_2_BUTTON1, JOYCODE_2_BUTTON3, CODE_OR,
                    JOYCODE_3_SELECT, JOYCODE_3_BUTTON1, JOYCODE_3_BUTTON3, CODE_OR,
                    JOYCODE_4_SELECT, JOYCODE_4_BUTTON1, JOYCODE_4_BUTTON3, CODE_OR,
                    KEYCODE_F12 );
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
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER1+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER1)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 0, JT_LSTICK_RIGHT ) );
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
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER2+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER2)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 1, JT_LSTICK_RIGHT ) );
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
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER3+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER3)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 2, JT_LSTICK_RIGHT ) );
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
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_LEFT ) );
      break;

			// *** IPT_LIGHTGUN_X|IPF_PLAYER4+IPT_EXTENSION *** //
    case (IPT_LIGHTGUN_X|IPF_PLAYER4)+IPT_EXTENSION:
      REMAP_SEQ_1( AXISCODE( 3, JT_LSTICK_RIGHT ) );
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
#pragma code_seg()
#pragma data_seg()

