/*
 * ------------------------------------------------------------------------
 *  Name:   FInputMouse.h
 *  Desc:   This file define a base class for mouse device.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_INPUT_MOUSE_H__
#define __FAIRY_INPUT_MOUSE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FInputDevice.h"

///////////////////////////////////////////////////////////////////////////

// Key State Masks for Mouse events.
#define MOUSE_KEYMASK_LBUTTON	0x0001
#define MOUSE_KEYMASK_RBUTTON	0x0002
#define MOUSE_KEYMASK_CONTROL	0x0004
#define MOUSE_KEYMASK_SHIFT		0x0008
#define MOUSE_KEYMASK_MBUTTON	0x0010
#define MOUSE_KEYMASK_XBUTTON1	0x0020
#define MOUSE_KEYMASK_XBUTTON2	0x0040

/** Mouse device.
*/
class FInputMouse : public FInputDevice
{
public:

	enum EMouseButton
	{
		MOUSE_LBUTTON,
		MOUSE_MBUTTON,
		MOUSE_RBUTTON,
		MOUSE_X1BUTTON,
		MOUSE_X2BUTTON,

		NUM_MOUSEBUTTON,
	};

	enum EButtonEvent
	{
		EVENT_DOWN,
		EVENT_UP,
		EVENT_CLICK,
		EVENT_DBLCLICK,

		NUM_EVENT,
	};

	struct ButtonEvent
	{
		EMouseButton btn;
		EButtonEvent evt;
		FPointI cursor_pos;
		bool ctrl_key_down;
		bool shift_key_down;
		bool x1_down;
		bool x2_down;
		uint32 nTime;
	};

	typedef std::vector<ButtonEvent> ButtonEventQueue;

public:
	FInputMouse();
	virtual ~FInputMouse();

	// Initialize the mouse device.
	virtual bool Init();
	// Update the mouse device.
	virtual void Update();

	// Set the cursor's position(relative to screen)
	virtual void SetCursorScreenPos(const FPointI& pos) = 0;
	// Set the cursor's position(relative to client)
	virtual void SetCursorClientPos(const FPointI& pos) = 0;
	// Show or hide the cursor.
	virtual void ShowCursor(bool bShow) = 0;

	// Is the cursor showing ?
	bool IsShowCursor() const;
	// Get the position of the cursor relative to the screen.
	const FPointI& GetCursorScreenPos() const;
	// Get the position of the cursor relative to the client area of the application.
	const FPointI& GetCursorClientPos() const;
	// Is the specified mouse button pressed ?
	bool IsButtonPressed(EMouseButton btn) const;

protected:
	FThreadMutex* m_pMutex;
	FPointI m_cursorPosScreen;
	FPointI m_cursorPosClient;
	bool m_bButtonState[NUM_MOUSEBUTTON];
	bool m_bShowCursor;
	ButtonEventQueue m_EventQueue;

	// Dispatch a mouse button event.
	void DispatchButtonEvent(EMouseButton btn, EButtonEvent evt, const FPointI& pos, uint32 nMask);
	// Dispatch a mouse move event.
	void DispatchMoveEvent(const FPointI& pos, uint32 nMask);
	// Dispatch a mouse wheel event.
	void DispatchWheelEvent(const FPointI& pos, uint32 nMask, int iDelta);
	// Handle event queue.
	void HandleEventQueue(const ButtonEvent& ev);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_INPUT_MOUSE_H__