/*
 * ------------------------------------------------------------------------
 *  Name:   FInputEventTypes.h
 *  Desc:   This file define all the type of input events.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_EVENTTYPES_H__
#define __FAIRY_INPUT_EVENTTYPES_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputEvent.h"

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

#define IMPLEMENT_FEVENT_FREE_FUNC(cls) \
	public: \
	void Free() { \
			FInputEventPool<cls>::GetInstance().FreeEvent(this); \
		}

///////////////////////////////////////////////////////////////////////////

class FInputKeydownEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputKeydownEvent)

public:
    FInputKeydownEvent() : FInputEvent(FINPUT_KEYDOWN_EVENT) {}
    
public:
    int vk;
};

class FInputKeyupEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputKeyupEvent)

public:
	FInputKeyupEvent() : FInputEvent(FINPUT_KEYUP_EVENT) {}

public:
    int vk;
};

class FInputLogicKeydownEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputLogicKeydownEvent)

public:
	FInputLogicKeydownEvent() : FInputEvent(FINPUT_LOGICKEYDOWN_EVENT) {}

public:
	int logic_key;
};

class FInputLogicKeyupEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputLogicKeyupEvent)

public:
	FInputLogicKeyupEvent() : FInputEvent(FINPUT_LOGICKEYUP_EVENT) {}

public:
	int logic_key;
};

class FInputMouseLButtonDownEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseLButtonDownEvent)

public:
    FInputMouseLButtonDownEvent() : FInputEvent(FINPUT_MOUSE_LBUTTON_DOWN) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseLButtonUpEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseLButtonUpEvent)

public:
    FInputMouseLButtonUpEvent() : FInputEvent(FINPUT_MOUSE_LBUTTON_UP) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseLButtonClickEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseLButtonClickEvent)

public:
    FInputMouseLButtonClickEvent() : FInputEvent(FINPUT_MOUSE_LBUTTON_CLICK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseLButtonDblClkEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseLButtonDblClkEvent)

public:
    FInputMouseLButtonDblClkEvent() : FInputEvent(FINPUT_MOUSE_LBUTTON_DBLCLK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseRButtonDownEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseRButtonDownEvent)

public:
    FInputMouseRButtonDownEvent() : FInputEvent(FINPUT_MOUSE_RBUTTON_DOWN) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseRButtonUpEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseRButtonUpEvent)

public:
    FInputMouseRButtonUpEvent() : FInputEvent(FINPUT_MOUSE_RBUTTON_UP) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseRButtonClickEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseRButtonClickEvent)

public:
    FInputMouseRButtonClickEvent() : FInputEvent(FINPUT_MOUSE_RBUTTON_CLICK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseRButtonDblClkEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseRButtonDblClkEvent)

public:
    FInputMouseRButtonDblClkEvent() : FInputEvent(FINPUT_MOUSE_RBUTTON_DBLCLK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseMButtonDownEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseMButtonDownEvent)

public:
    FInputMouseMButtonDownEvent() : FInputEvent(FINPUT_MOUSE_MBUTTON_DOWN) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseMButtonUpEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseMButtonUpEvent)

public:
    FInputMouseMButtonUpEvent() : FInputEvent(FINPUT_MOUSE_MBUTTON_UP) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseMButtonClickEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseMButtonClickEvent)

public:
    FInputMouseMButtonClickEvent() : FInputEvent(FINPUT_MOUSE_MBUTTON_CLICK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseMButtonDblClkEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseMButtonDblClkEvent)

public:
    FInputMouseMButtonDblClkEvent() : FInputEvent(FINPUT_MOUSE_MBUTTON_DBLCLK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
};

class FInputMouseXButtonDownEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseXButtonDownEvent)

public:
    FInputMouseXButtonDownEvent() : FInputEvent(FINPUT_MOUSE_XBUTTON_DOWN) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseXButtonUpEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseXButtonUpEvent)

public:
    FInputMouseXButtonUpEvent() : FInputEvent(FINPUT_MOUSE_XBUTTON_UP) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseXButtonClickEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseXButtonClickEvent)

public:
    FInputMouseXButtonClickEvent() : FInputEvent(FINPUT_MOUSE_XBUTTON_CLICK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseXButtonDblClkEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseXButtonDblClkEvent)

public:
    FInputMouseXButtonDblClkEvent() : FInputEvent(FINPUT_MOUSE_XBUTTON_DBLCLK) {}
    
public:
    FPointI cursor_pos;
    bool ctrl_key_down;
    bool shift_key_down;
    bool x1_down;
    bool x2_down;
};

class FInputMouseMoveEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseMoveEvent)

public:
    FInputMouseMoveEvent() : FInputEvent(FINPUT_MOUSE_MOVE) {}
    
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

class FInputMouseWheelEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputMouseWheelEvent)

public:
    FInputMouseWheelEvent() : FInputEvent(FINPUT_MOUSE_WHEEL) {}
    
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

class FInputTouchTapEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchTapEvent)

public:
    FInputTouchTapEvent() : FInputEvent(FINPUT_TOUCH_TAP) {}
    
public:
    ETouchState state;
    FPointI tap_pos;
    int tap_count;
};

class FInputTouchTap2Event : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchTap2Event)

public:
    FInputTouchTap2Event() : FInputEvent(FINPUT_TOUCH_TAP_2) {}
    
public:
    ETouchState state;
    FPointI tap_pos;
};

class FInputTouchPanEvent : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchPanEvent)

public:
	FInputTouchPanEvent() : FInputEvent(FINPUT_TOUCH_PAN) {}

public:
	ETouchState state;
	float velocityX, velocityY;

};

class FInputTouchPan2Event : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchPan2Event)

public:
	FInputTouchPan2Event() : FInputEvent(FINPUT_TOUCH_PAN_2) {}

public:
	ETouchState state;
};

class FInputTouchPinch : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchPinch)

public:
	FInputTouchPinch() : FInputEvent(FINPUT_TOUCH_PINCH) {}

public:
	ETouchState state;
	float scale;
	float velocity;
};

class FInputTouchPress : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchPress)

public:
	FInputTouchPress() : FInputEvent(FINPUT_TOUCH_PRESS) {}

public:
	ETouchState state;
	FPointI touch_pos;
	uint32 press_time;
};

class FInputTouchLongPress : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchLongPress)

public:
	FInputTouchLongPress() : FInputEvent(FINPUT_TOUCH_LONG_PRESS) {}

public:
	ETouchState state;
	FPointI touch_pos;
};

class FInputTouchSwipe : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchSwipe)

public:
	FInputTouchSwipe() : FInputEvent(FINPUT_TOUCH_SWIPE) {}

public:
	ETouchState state;
	ESwipeDirection swipe_dir;
};

class FInputTouchCircle : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputTouchCircle)

public:
	FInputTouchCircle() : FInputEvent(FINPUT_TOUCH_CIRCLE) {}

public:
	ETouchState state;
	ETouchCircle circle;
};

class FInputJoystickStick : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputJoystickStick)

public:
	FInputJoystickStick() : FInputEvent(FINPUT_JOYSTICK_STICK) {}

public:
	float x, y;
};

class FInputJoystickKeydown : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputJoystickKeydown)

public:
	FInputJoystickKeydown() : FInputEvent(FINPUT_JOYSTICK_KEY_DOWN) {}

public:
	int vk;
};

class FInputJoystickKeyup : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputJoystickKeyup)

public:
	FInputJoystickKeyup() : FInputEvent(FINPUT_JOYSTICK_KEY_UP) {}

public:
	int vk;
};

class FInputSensorDeviceShake : public FInputEvent
{
	IMPLEMENT_FEVENT_FREE_FUNC(FInputSensorDeviceShake)

public:
	FInputSensorDeviceShake() : FInputEvent(FINPUT_SENSOR_DEVICESHAKE) {}

public:
	uint32 keep_time;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_EVENT_H__