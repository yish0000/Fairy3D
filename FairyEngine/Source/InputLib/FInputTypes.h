/*
 * ------------------------------------------------------------------------
 *  Name:   FInputTypes.h
 *  Desc:   Define some structures for input engine.
 *  Author: Yish
 *  Date:   2015/2/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_TYPES_H__
#define __FAIRY_INPUT_TYPES_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"
#include "FTemplate.h"

///////////////////////////////////////////////////////////////////////////

enum EInputDeviceType
{
	IDT_KEYBOARD,	// Keyboard
	IDT_MOUSE,		// Mouse
	IDT_JOYSTICK,	// Joystick
	IDT_TOUCH,		// Touch screen
    IDT_SENSOR,		// All kinds of sensor devices.
    
    IDT_COUNT,
};

///////////////////////////////////////////////////////////////////////////
//// Key code

// Keyboard keys
#define FI_KEY_BACK			0x08
#define FI_KEY_TAB			0x09
#define FI_KEY_CLEAR		0x0c
#define FI_KEY_ENTER		0x0d
#define FI_KEY_SHIFT		0x10
#define FI_KEY_CONTROL		0x11
#define FI_KEY_ALT			0x12
#define FI_KEY_PAUSE		0x13
#define FI_KEY_CAPSLOCK		0x14
#define FI_KEY_ESCAPE		0x1b
#define FI_KEY_SPACE		0x20
#define FI_KEY_PAGEUP		0x21
#define FI_KEY_PAGEDOWN		0x22
#define FI_KEY_END			0x23
#define FI_KEY_HOME			0x24
#define FI_KEY_LEFT			0x25
#define FI_KEY_UP			0x26
#define FI_KEY_RIGHT		0x27
#define FI_KEY_DOWN			0x28
#define FI_KEY_PRINT		0x2a
#define FI_KEY_EXECUTE		0x2b
#define FI_KEY_SNAPSHOT		0x2c
#define FI_KEY_INSERT		0x2d
#define FI_KEY_DELETE		0x2e
#define FI_KEY_HELP			0x2f
#define FI_KEY_0			'0'
#define FI_KEY_1			'1'
#define FI_KEY_2			'2'
#define FI_KEY_3			'3'
#define FI_KEY_4			'4'
#define FI_KEY_5			'5'
#define FI_KEY_6			'6'
#define FI_KEY_7			'7'
#define FI_KEY_8			'8'
#define FI_KEY_9			'9'
#define FI_KEY_A			'A'
#define FI_KEY_B			'B'
#define FI_KEY_C			'C'
#define FI_KEY_D			'D'
#define FI_KEY_E			'E'
#define FI_KEY_F			'F'
#define FI_KEY_G			'G'
#define FI_KEY_H			'H'
#define FI_KEY_I			'I'
#define FI_KEY_J			'J'
#define FI_KEY_K			'K'
#define FI_KEY_L			'L'
#define FI_KEY_M			'M'
#define FI_KEY_N			'N'
#define FI_KEY_O			'O'
#define FI_KEY_P			'P'
#define FI_KEY_Q			'Q'
#define FI_KEY_R			'R'
#define FI_KEY_S			'S'
#define FI_KEY_T			'T'
#define FI_KEY_U			'U'
#define FI_KEY_V			'V'
#define FI_KEY_W			'W'
#define FI_KEY_X			'X'
#define FI_KEY_Y			'Y'
#define FI_KEY_Z			'Z'
#define FI_KEY_LWIN			0x5b
#define FI_KEY_RWIN			0x5c
#define FI_KEY_APPS			0x5d
#define FI_KEY_NUMPAD0		0x60
#define FI_KEY_NUMPAD1		0x61
#define FI_KEY_NUMPAD2		0x62
#define FI_KEY_NUMPAD3		0x63
#define FI_KEY_NUMPAD4		0x64
#define FI_KEY_NUMPAD5		0x65
#define FI_KEY_NUMPAD6		0x66
#define FI_KEY_NUMPAD7		0x67
#define FI_KEY_NUMPAD8		0x68
#define FI_KEY_NUMPAD9		0x69
#define FI_KEY_MULTIPLY		0x6A
#define FI_KEY_ADD			0x6B
#define FI_KEY_SEPARATOR	0x6C
#define FI_KEY_SUBTRACT		0x6D
#define FI_KEY_DECIMAL		0x6E
#define FI_KEY_DIVIDE		0x6F
#define FI_KEY_F1			0x70
#define FI_KEY_F2			0x71
#define FI_KEY_F3			0x72
#define FI_KEY_F4			0x73
#define FI_KEY_F5			0x74
#define FI_KEY_F6			0x75
#define FI_KEY_F7			0x76
#define FI_KEY_F8			0x77
#define FI_KEY_F9			0x78
#define FI_KEY_F10			0x79
#define FI_KEY_F11			0x7A
#define FI_KEY_F12			0x7B
#define FI_KEY_NUMLOCK		0x90
#define FI_KEY_SCROLLLOCK	0x91
#define FI_KEY_LSHIFT		0xa0	// Only for IsKeyPressed()
#define FI_KEY_RSHIFT		0xa1	// Only for IsKeyPressed()
#define FI_KEY_LCONTROL		0xa2	// Only for IsKeyPressed()
#define FI_KEY_RCONTROL		0xa3	// Only for IsKeyPressed()
#define FI_KEY_LALT			0xa4	// Only for IsKeyPressed()
#define FI_KEY_RALT			0xa5	// Only for IsKeyPressed()
#define FI_KEY_SEMICOLON	0xba	// ;:
#define FI_KEY_EQUAL		0xbb	// =+
#define FI_KEY_COMMA		0xbc	// ,<
#define FI_KEY_MINUS		0xbd	// -_
#define FI_KEY_PERIOD		0xbe	// .>
#define FI_KEY_SLANT		0xbf	// /?
#define FI_KEY_WAVE			0xc0	// `~
#define FI_KEY_LBRACKET		0xdb	// [{
#define FI_KEY_BACKSLANT	0xdc	// \|
#define FI_KEY_RBRACKET		0xdd	// ]}
#define FI_KEY_QUOTATION	0xde	// '"

// Joystick keys
#define FI_JOYSTICK_UP				19
#define FI_JOYSTICK_DOWN			20
#define FI_JOYSTICK_LEFT			21
#define FI_JOYSTICK_RIGHT			22
#define FI_JOYSTICK_UP_LEFT			125
#define FI_JOYSTICK_UP_RIGHT		126
#define FI_JOYSTICK_DOWN_LEFT		127
#define FI_JOYSTICK_DOWN_RIGHT		128
#define FI_JOYSTICK_L1				102			
#define FI_JOYSTICK_L2				104
#define FI_JOYSTICK_R1				103
#define FI_JOYSTICK_R2				105
#define FI_JOYSTICK_LS				106
#define FI_JOYSTICK_RS				107
#define FI_JOYSTICK_A				99
#define FI_JOYSTICK_B				97
#define FI_JOYSTICK_C			
#define FI_JOYSTICK_X				98
#define FI_JOYSTICK_Y				96
#define FI_JOYSTICK_Z
#define FI_JOYSTICK_START			108
#define FI_JOYSTICK_SELECT			109
#define FI_JOYSTICK_MODE			
#define FI_JOYSTICK_HOME			3
#define FI_JOYSTICK_VOL_DOWN		25
#define FI_JOYSTICK_VOL_UP			24
#define FI_JOYSTICK_ENTER			23
#define FI_JOYSTICK_BACK			4
#define FI_JOYSTICK_MEDIA_PAY_PAUSE	85
#define FI_JOYSTICK_MEDIA_NEXT		87
#define FI_JOYSTICK_MEDIA_PREV		88

/** State of the touch event.
*/
enum ETouchState
{
    TOUCH_STATE_INVALID,
    TOUCH_STATE_BEGIN,
    TOUCH_STATE_CHANGE,
    TOUCH_STATE_END,
    
    TOUCH_STATE_COUNT,
};

/** Guesture type for the touch device.
 */
enum EGuestureType
{
    GUESTURE_TYPE_INVALID,
    
    GUESTURE_TYPE_TAP,
    GUESTURE_TYPE_TAP_2,
    GUESTURE_TYPE_PINCH,
    GUESTURE_TYPE_PAN,
    GUESTURE_TYPE_PAN_2,
    GUESTURE_TYPE_PRESS,
    GUESTURE_TYPE_LONG_PRESS,
    GUESTURE_TYPE_SWIPE,
    GUESTURE_TYPE_CIRCLE,
    
    GUESTURE_TYPE_NUM,
};

/** Swipe direction on the touch pad.
*/
enum ESwipeDirection
{
	SWIPE_DIRECTION_INVALID,

	SWIPE_DIRECTION_LEFT,
	SWIPE_DIRECTION_RIGHT,
	SWIPE_DIRECTION_UP,
	SWIPE_DIRECTION_DOWN,
};

/** Circle direction on the touch pad.
*/
enum ETouchCircle
{
	TOUCHCIRCLE_INVALID,

	TOUCHCIRCLE_CLOCKWISE,
	TOUCHCIRCLE_COUNTERCLOCKWISE,
};

/** Flag of control keys.
*/
enum EControlKeyFlag
{
	CONTROL_KEY_SCA,
	CONTROL_KEY_CA,
	CONTROL_KEY_SA,
	CONTROL_KEY_SC,
	CONTROL_KEY_S,
	CONTROL_KEY_C,
	CONTROL_KEY_A,
	CONTROL_KEY_NONE,

	CONTROL_KEY_NUM,
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_TYPES_H__