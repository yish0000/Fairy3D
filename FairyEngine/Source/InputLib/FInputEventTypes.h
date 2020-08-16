/*
 * ------------------------------------------------------------------------
 *  Name:   FEventTypes.h
 *  Desc:   This file define all the type of input events.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_EVENTTYPES_H__
#define __FAIRY_INPUT_EVENTTYPES_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FEvent.h"
#include "FInputTypes.h"

///////////////////////////////////////////////////////////////////////////

#define FINPUT_KEYDOWN_EVENT            0x0001
#define FINPUT_KEYUP_EVENT              0x0002
#define FINPUT_LOGICKEYDOWN_EVENT		0x0003
#define FINPUT_LOGICKEYUP_EVENT			0x0004

#define FINPUT_MOUSE_LBUTTON_DOWN       0x0101
#define FINPUT_MOUSE_LBUTTON_UP         0x0102
#define FINPUT_MOUSE_LBUTTON_CLICK      0x0103
#define FINPUT_MOUSE_LBUTTON_DBLCLK     0x0104
#define FINPUT_MOUSE_RBUTTON_DOWN       0x0105
#define FINPUT_MOUSE_RBUTTON_UP         0x0106
#define FINPUT_MOUSE_RBUTTON_CLICK      0x0107
#define FINPUT_MOUSE_RBUTTON_DBLCLK     0x0108
#define FINPUT_MOUSE_MBUTTON_DOWN       0x0109
#define FINPUT_MOUSE_MBUTTON_UP         0x0110
#define FINPUT_MOUSE_MBUTTON_CLICK      0x0111
#define FINPUT_MOUSE_MBUTTON_DBLCLK     0x0112
#define FINPUT_MOUSE_MOVE               0x0113
#define FINPUT_MOUSE_WHEEL              0x0114
#define FINPUT_MOUSE_XBUTTON_DOWN       0x0115
#define FINPUT_MOUSE_XBUTTON_UP         0x0116
#define FINPUT_MOUSE_XBUTTON_CLICK      0x0117
#define FINPUT_MOUSE_XBUTTON_DBLCLK     0x0118

#define FINPUT_TOUCH_TAP                0x0201
#define FINPUT_TOUCH_PAN                0x0202
#define FINPUT_TOUCH_TAP_2              0x0203
#define FINPUT_TOUCH_PAN_2              0x0204
#define FINPUT_TOUCH_PINCH              0x0205
#define FINPUT_TOUCH_PRESS              0x0206
#define FINPUT_TOUCH_LONG_PRESS         0x0207
#define FINPUT_TOUCH_SWIPE              0x0208
#define FINPUT_TOUCH_CIRCLE             0x0209

#define FINPUT_JOYSTICK_STICK			0x0301
#define FINPUT_JOYSTICK_KEY_DOWN		0x0302
#define FINPUT_JOYSTICK_KEY_UP			0x0303

#define FINPUT_SENSOR_DEVICESHAKE		0x0401

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////

class FInputKeydownEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputKeydownEvent)

public:
    FInputKeydownEvent() : FEvent(FINPUT_KEYDOWN_EVENT) {}
    
public:
    int vk;
};

class FInputKeyupEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputKeyupEvent)

public:
	FInputKeyupEvent() : FEvent(FINPUT_KEYUP_EVENT) {}

public:
    int vk;
};

class FInputLogicKeydownEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputLogicKeydownEvent)

public:
	FInputLogicKeydownEvent() : FEvent(FINPUT_LOGICKEYDOWN_EVENT) {}

public:
	int logic_key;
};

class FInputLogicKeyupEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputLogicKeyupEvent)

public:
	FInputLogicKeyupEvent() : FEvent(FINPUT_LOGICKEYUP_EVENT) {}

public:
	int logic_key;
};

class FInputMouseLButtonDownEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseLButtonDownEvent)

public:
    FInputMouseLButtonDownEvent() : FEvent(FINPUT_MOUSE_LBUTTON_DOWN) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseLButtonUpEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseLButtonUpEvent)

public:
    FInputMouseLButtonUpEvent() : FEvent(FINPUT_MOUSE_LBUTTON_UP) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseLButtonClickEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseLButtonClickEvent)

public:
    FInputMouseLButtonClickEvent() : FEvent(FINPUT_MOUSE_LBUTTON_CLICK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseLButtonDblClkEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseLButtonDblClkEvent)

public:
    FInputMouseLButtonDblClkEvent() : FEvent(FINPUT_MOUSE_LBUTTON_DBLCLK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseRButtonDownEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseRButtonDownEvent)

public:
    FInputMouseRButtonDownEvent() : FEvent(FINPUT_MOUSE_RBUTTON_DOWN) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseRButtonUpEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseRButtonUpEvent)

public:
    FInputMouseRButtonUpEvent() : FEvent(FINPUT_MOUSE_RBUTTON_UP) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseRButtonClickEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseRButtonClickEvent)

public:
    FInputMouseRButtonClickEvent() : FEvent(FINPUT_MOUSE_RBUTTON_CLICK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseRButtonDblClkEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseRButtonDblClkEvent)

public:
    FInputMouseRButtonDblClkEvent() : FEvent(FINPUT_MOUSE_RBUTTON_DBLCLK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseMButtonDownEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseMButtonDownEvent)

public:
    FInputMouseMButtonDownEvent() : FEvent(FINPUT_MOUSE_MBUTTON_DOWN) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseMButtonUpEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseMButtonUpEvent)

public:
    FInputMouseMButtonUpEvent() : FEvent(FINPUT_MOUSE_MBUTTON_UP) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseMButtonClickEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseMButtonClickEvent)

public:
    FInputMouseMButtonClickEvent() : FEvent(FINPUT_MOUSE_MBUTTON_CLICK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseMButtonDblClkEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseMButtonDblClkEvent)

public:
    FInputMouseMButtonDblClkEvent() : FEvent(FINPUT_MOUSE_MBUTTON_DBLCLK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseXButtonDownEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseXButtonDownEvent)

public:
    FInputMouseXButtonDownEvent() : FEvent(FINPUT_MOUSE_XBUTTON_DOWN) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseXButtonUpEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseXButtonUpEvent)

public:
    FInputMouseXButtonUpEvent() : FEvent(FINPUT_MOUSE_XBUTTON_UP) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseXButtonClickEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseXButtonClickEvent)

public:
    FInputMouseXButtonClickEvent() : FEvent(FINPUT_MOUSE_XBUTTON_CLICK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseXButtonDblClkEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseXButtonDblClkEvent)

public:
    FInputMouseXButtonDblClkEvent() : FEvent(FINPUT_MOUSE_XBUTTON_DBLCLK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseMoveEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseMoveEvent)

public:
    FInputMouseMoveEvent() : FEvent(FINPUT_MOUSE_MOVE) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool lbutton_down;
    bool mbutton_down;
    bool rbutton_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseWheelEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputMouseWheelEvent)

public:
    FInputMouseWheelEvent() : FEvent(FINPUT_MOUSE_WHEEL) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool lbutton_down;
    bool mbutton_down;
    bool rbutton_down;
    bool x1_down;
    bool x2_down;
    int delta;
};

class FInputTouchTapEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchTapEvent)

public:
    FInputTouchTapEvent() : FEvent(FINPUT_TOUCH_TAP) {}
    
public:
    ETouchState state;
    FPointI tap_pos;
    int tap_count;
};

class FInputTouchTap2Event : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchTap2Event)

public:
    FInputTouchTap2Event() : FEvent(FINPUT_TOUCH_TAP_2) {}
    
public:
    ETouchState state;
    FPointI tap_pos;
};

class FInputTouchPanEvent : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchPanEvent)

public:
	FInputTouchPanEvent() : FEvent(FINPUT_TOUCH_PAN) {}

public:
	ETouchState state;
	float velocityX, velocityY;

};

class FInputTouchPan2Event : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchPan2Event)

public:
	FInputTouchPan2Event() : FEvent(FINPUT_TOUCH_PAN_2) {}

public:
	ETouchState state;
};

class FInputTouchPinch : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchPinch)

public:
	FInputTouchPinch() : FEvent(FINPUT_TOUCH_PINCH) {}

public:
	ETouchState state;
	float scale;
	float velocity;
};

class FInputTouchPress : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchPress)

public:
	FInputTouchPress() : FEvent(FINPUT_TOUCH_PRESS) {}

public:
	ETouchState state;
	FPointI touch_pos;
	uint32 press_time;
};

class FInputTouchLongPress : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchLongPress)

public:
	FInputTouchLongPress() : FEvent(FINPUT_TOUCH_LONG_PRESS) {}

public:
	ETouchState state;
	FPointI touch_pos;
};

class FInputTouchSwipe : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchSwipe)

public:
	FInputTouchSwipe() : FEvent(FINPUT_TOUCH_SWIPE) {}

public:
	ETouchState state;
	ESwipeDirection swipe_dir;
};

class FInputTouchCircle : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputTouchCircle)

public:
	FInputTouchCircle() : FEvent(FINPUT_TOUCH_CIRCLE) {}

public:
	ETouchState state;
	ETouchCircle circle;
};

class FInputJoystickStick : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputJoystickStick)

public:
	FInputJoystickStick() : FEvent(FINPUT_JOYSTICK_STICK) {}

public:
	float x, y;
};

class FInputJoystickKeydown : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputJoystickKeydown)

public:
	FInputJoystickKeydown() : FEvent(FINPUT_JOYSTICK_KEY_DOWN) {}

public:
	int vk;
};

class FInputJoystickKeyup : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputJoystickKeyup)

public:
	FInputJoystickKeyup() : FEvent(FINPUT_JOYSTICK_KEY_UP) {}

public:
	int vk;
};

class FInputSensorDeviceShake : public FEvent
{
	F_IMPLEMENT_EVENT_CLASS_USEPOOL(FInputSensorDeviceShake)

public:
	FInputSensorDeviceShake() : FEvent(FINPUT_SENSOR_DEVICESHAKE) {}

public:
	uint32 keep_time;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_EVENT_H__