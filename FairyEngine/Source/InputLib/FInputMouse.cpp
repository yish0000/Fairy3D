/*
 * ------------------------------------------------------------------------
 *  Name:   FInputMouse.cpp
 *  Desc:   This file define a base class for mouse device.
 *  Author: Yish
 *  Date:   2015/2/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputMouse.h"
#include "FInputEventTypes.h"
#include "FThread.h"
#include "FSysAPI.h"

// Some constant variables.
#define SINGLE_CLICK_MAX_TIME	200
#define DOUBLE_CLICK_MAX_TIME	500
#define CLICK_EVENT_POINT_RANGE	5.0f

#define FILL_BUTTON_EVENT(cls) \
	{ \
		cls* pNewEvent = cls::Create(); \
		if (pNewEvent) { \
			pNewEvent->cursor_pos = pos; \
			pNewEvent->ctrl_key_down = (nMask & MOUSE_KEYMASK_CONTROL) ? true : false; \
			pNewEvent->shift_key_down = (nMask & MOUSE_KEYMASK_SHIFT) ? true : false; \
			pEvent = pNewEvent; \
			ev.btn = btn; \
			ev.evt = evt; \
			ev.cursor_pos = pos; \
			ev.ctrl_key_down = pNewEvent->ctrl_key_down; \
			ev.shift_key_down = pNewEvent->shift_key_down; \
			ev.x1_down = false; ev.x2_down = false; \
			ev.nTime = FSysAPI::GetMilliSecond(); \
		} \
	}

#define FILL_XBUTTON_EVENT(cls) \
	{ \
		cls* pNewEvent = cls::Create(); \
		if (pNewEvent) { \
			pNewEvent->cursor_pos = pos; \
			pNewEvent->ctrl_key_down = (nMask & MOUSE_KEYMASK_CONTROL) ? true : false; \
			pNewEvent->shift_key_down = (nMask & MOUSE_KEYMASK_SHIFT) ? true : false; \
			pNewEvent->x1_down = (nMask & MOUSE_KEYMASK_XBUTTON1) ? true : false; \
			pNewEvent->x2_down = (nMask & MOUSE_KEYMASK_XBUTTON2) ? true : false; \
			pEvent = pNewEvent; \
			ev.btn = btn; \
			ev.evt = evt; \
			ev.cursor_pos = pos; \
			ev.ctrl_key_down = pNewEvent->ctrl_key_down; \
			ev.shift_key_down = pNewEvent->shift_key_down; \
			ev.x1_down = pNewEvent->x1_down; \
			ev.x2_down = pNewEvent->x2_down; \
			ev.nTime = FSysAPI::GetMilliSecond(); \
		} \
	}

// Calculate the distance of two point.
static float CalcDist(const FPointI& pt1, const FPointI& pt2)
{
	float fDiffX = pt2.x - pt1.x;
	float fDiffY = pt2.y - pt1.y;
	return sqrtf(fDiffX * fDiffX + fDiffY * fDiffY);
}

// Generate the input event by button event info.
static FEvent* GenButtonInputEvent(const FInputMouse::ButtonEvent& ev)
{
#define FILL_EVENT(cls) \
	{ \
		cls* pNewEvent = cls::Create(); \
		if (pNewEvent) { \
			pNewEvent->cursor_pos = ev.cursor_pos; \
			pNewEvent->ctrl_key_down = ev.ctrl_key_down; \
			pNewEvent->shift_key_down = ev.shift_key_down; \
			pEvent = pNewEvent; \
		} \
	}

#define FILL_XEVENT(cls) \
	{ \
		cls* pNewEvent = cls::Create(); \
		if (pNewEvent) { \
			pNewEvent->cursor_pos = ev.cursor_pos; \
			pNewEvent->ctrl_key_down = ev.ctrl_key_down; \
			pNewEvent->shift_key_down = ev.shift_key_down; \
			pNewEvent->x1_down = ev.x1_down; \
			pNewEvent->x2_down = ev.x2_down; \
			pEvent = pNewEvent; \
		} \
	}

	FEvent* pEvent = NULL;

	switch (ev.btn)
	{
	case FInputMouse::MOUSE_LBUTTON:
		if (ev.evt == FInputMouse::EVENT_DOWN)
			FILL_EVENT(FInputMouseLButtonDownEvent)
		else if (ev.evt == FInputMouse::EVENT_UP)
			FILL_EVENT(FInputMouseLButtonUpEvent)
		else if (ev.evt == FInputMouse::EVENT_CLICK)
			FILL_EVENT(FInputMouseLButtonClickEvent)
		else if (ev.evt == FInputMouse::EVENT_DBLCLICK)
			FILL_EVENT(FInputMouseLButtonDblClkEvent)
		break;
	case FInputMouse::MOUSE_MBUTTON:
		if (ev.evt == FInputMouse::EVENT_DOWN)
			FILL_EVENT(FInputMouseMButtonDownEvent)
		else if (ev.evt == FInputMouse::EVENT_UP)
			FILL_EVENT(FInputMouseMButtonUpEvent)
		else if (ev.evt == FInputMouse::EVENT_CLICK)
			FILL_EVENT(FInputMouseMButtonClickEvent)
		else if (ev.evt == FInputMouse::EVENT_DBLCLICK)
			FILL_EVENT(FInputMouseMButtonDblClkEvent)
		break;
	case FInputMouse::MOUSE_RBUTTON:
		if (ev.evt == FInputMouse::EVENT_DOWN)
			FILL_EVENT(FInputMouseRButtonDownEvent)
		else if (ev.evt == FInputMouse::EVENT_UP)
			FILL_EVENT(FInputMouseRButtonUpEvent)
		else if (ev.evt == FInputMouse::EVENT_CLICK)
			FILL_EVENT(FInputMouseRButtonClickEvent)
		else if (ev.evt == FInputMouse::EVENT_DBLCLICK)
			FILL_EVENT(FInputMouseRButtonDblClkEvent)
		break;
	case FInputMouse::MOUSE_X1BUTTON:
	case FInputMouse::MOUSE_X2BUTTON:
		if (ev.evt == FInputMouse::EVENT_DOWN)
			FILL_EVENT(FInputMouseXButtonDownEvent)
		else if (ev.evt == FInputMouse::EVENT_UP)
			FILL_EVENT(FInputMouseXButtonUpEvent)
		else if (ev.evt == FInputMouse::EVENT_CLICK)
			FILL_EVENT(FInputMouseXButtonClickEvent)
		else if (ev.evt == FInputMouse::EVENT_DBLCLICK)
			FILL_EVENT(FInputMouseXButtonDblClkEvent)
		break;
	}

	return pEvent;
}

/** Constructor.
*/
FInputMouse::FInputMouse() : FInputDevice(IDT_MOUSE)
{
	m_pMutex = FThreadMutex::Create();
	memset(m_bButtonState, 0, sizeof(m_bButtonState));
}

/** Destructor.
*/
FInputMouse::~FInputMouse()
{
	F_SAFE_DELETE(m_pMutex);
}

/** Initialize the Mouse.
*/
bool FInputMouse::Init()
{
	return true; 
}

/** Update the Mouse
*/
void FInputMouse::Update()
{
	// Clear the expired event.
	FScopedLock lock(m_pMutex);
	uint32 nCurTime = FSysAPI::GetMilliSecond();
	ButtonEventQueue::iterator it = m_EventQueue.begin();
	while (it != m_EventQueue.end())
	{
		if (it->evt == EVENT_CLICK)
		{
			if (nCurTime - it->nTime > DOUBLE_CLICK_MAX_TIME)
				it = m_EventQueue.erase(it);
			else
				++it;
		}
		else if (it->evt == EVENT_DOWN)
		{
			if (nCurTime - it->nTime > SINGLE_CLICK_MAX_TIME)
				it = m_EventQueue.erase(it);
			else
				++it;
		}
		else
		{
			it = m_EventQueue.erase(it);
		}
	}
}

/** Is the specified mouse button pressed ?
*/
bool FInputMouse::IsButtonPressed(EMouseButton btn) const
{
	if (btn < MOUSE_LBUTTON || btn >= NUM_MOUSEBUTTON)
	{
		FASSERT(!"btn < MOUSE_LBUTTON || btn >= NUM_MOUSEBUTTON");
		return false;
	}

	FScopedLock lock(m_pMutex);
	return m_bButtonState[btn];
}

/** Get the current position of the cursor. (relative to the screen)
*/
const FPointI& FInputMouse::GetCursorScreenPos() const
{
	FScopedLock lock(m_pMutex);
	return m_cursorPosScreen;
}

/** Get the current position of the cursor. (relative to the client area)
*/
const FPointI& FInputMouse::GetCursorClientPos() const
{
	FScopedLock lock(m_pMutex);
	return m_cursorPosClient;
}

/** Is the cursor icon showing ?
*/
bool FInputMouse::IsShowCursor() const
{
	FScopedLock lock(m_pMutex);
	return m_bShowCursor;
}

/** Dispatch a new button event.
*/
void FInputMouse::DispatchButtonEvent(EMouseButton btn, EButtonEvent evt, const FPointI& pos, uint32 nMask)
{
	ButtonEvent ev;
	FEvent* pEvent = NULL;

	FASSERT(btn >= MOUSE_LBUTTON && btn < NUM_MOUSEBUTTON);
	FASSERT(evt == EVENT_DOWN || evt == EVENT_UP );

	switch (btn)
	{
	case MOUSE_LBUTTON:
		if (evt == EVENT_DOWN)
			FILL_BUTTON_EVENT(FInputMouseLButtonDownEvent)
		else if (evt == EVENT_UP)
			FILL_BUTTON_EVENT(FInputMouseLButtonUpEvent)
		break;
	case MOUSE_RBUTTON:
		if (evt == EVENT_DOWN)
			FILL_BUTTON_EVENT(FInputMouseRButtonDownEvent)
		else if (evt == EVENT_UP)
			FILL_BUTTON_EVENT(FInputMouseRButtonUpEvent)
		break;
	case MOUSE_MBUTTON:
		if (evt == EVENT_DOWN)
			FILL_BUTTON_EVENT(FInputMouseMButtonDownEvent)
		else if (evt == EVENT_UP)
			FILL_BUTTON_EVENT(FInputMouseMButtonUpEvent)
		break;
	case MOUSE_X1BUTTON:
	case MOUSE_X2BUTTON:
		if (evt == EVENT_DOWN)
			FILL_XBUTTON_EVENT(FInputMouseXButtonDownEvent)
		else if (evt == EVENT_UP)
			FILL_XBUTTON_EVENT(FInputMouseXButtonUpEvent)
		break;
    default: break;
	}

	// Change the button state of the mouse.
	{
		FScopedLock lock(m_pMutex);
		m_bButtonState[btn] = (evt == EVENT_DOWN) ? true : false;
	}

	// Handle the click and double click event.
	HandleEventQueue(ev);

	if (pEvent)
		GetDispatcher()->DispatchEvent(pEvent);
}

/** Dispatch a mouse move event.
*/
void FInputMouse::DispatchMoveEvent(const FPointI& pos, uint32 nMask)
{
	FInputMouseMoveEvent* pEvent = FInputMouseMoveEvent::Create();
	if (pEvent)
	{
		pEvent->cursor_pos = pos;
		pEvent->ctrl_key_down = (nMask & MOUSE_KEYMASK_CONTROL) ? true : false;
		pEvent->lbutton_down = (nMask & MOUSE_KEYMASK_LBUTTON) ? true : false;
		pEvent->rbutton_down = (nMask & MOUSE_KEYMASK_RBUTTON) ? true : false;
		pEvent->mbutton_down = (nMask & MOUSE_KEYMASK_MBUTTON) ? true : false;
		pEvent->shift_key_down = (nMask & MOUSE_KEYMASK_SHIFT) ? true : false;
		pEvent->x1_down = (nMask & MOUSE_KEYMASK_XBUTTON1) ? true : false;
		pEvent->x2_down = (nMask & MOUSE_KEYMASK_XBUTTON2) ? true : false;
		GetDispatcher()->DispatchEvent(pEvent);
	}
}

/** Dispatch a mouse wheel event.
*/
void FInputMouse::DispatchWheelEvent(const FPointI& pos, uint32 nMask, int iDelta)
{
	FInputMouseWheelEvent* pEvent = FInputMouseWheelEvent::Create();
	if (pEvent)
	{
		pEvent->cursor_pos = pos;
		pEvent->ctrl_key_down = (nMask & MOUSE_KEYMASK_CONTROL) ? true : false;
		pEvent->lbutton_down = (nMask & MOUSE_KEYMASK_LBUTTON) ? true : false;
		pEvent->rbutton_down = (nMask & MOUSE_KEYMASK_RBUTTON) ? true : false;
		pEvent->mbutton_down = (nMask & MOUSE_KEYMASK_MBUTTON) ? true : false;
		pEvent->shift_key_down = (nMask & MOUSE_KEYMASK_SHIFT) ? true : false;
		pEvent->x1_down = (nMask & MOUSE_KEYMASK_XBUTTON1) ? true : false;
		pEvent->x2_down = (nMask & MOUSE_KEYMASK_XBUTTON2) ? true : false;
		pEvent->delta = iDelta;
		GetDispatcher()->DispatchEvent(pEvent);
	}
}

/** Handle the button event queue.
*/
void FInputMouse::HandleEventQueue(const ButtonEvent& ev)
{
	FScopedLock lock(m_pMutex);

	FASSERT(ev.evt == EVENT_DOWN || ev.evt == EVENT_UP);

	if (ev.evt != EVENT_UP || m_EventQueue.empty())
	{
		m_EventQueue.push_back(ev);
		return;
	}

	// Get the last button event.
	ButtonEvent& lastEvent = m_EventQueue.back();
	if (lastEvent.btn == ev.btn && lastEvent.evt == EVENT_DOWN && ev.nTime - lastEvent.nTime <= SINGLE_CLICK_MAX_TIME &&
		CalcDist(ev.cursor_pos, lastEvent.cursor_pos) <= CLICK_EVENT_POINT_RANGE)
	{
		// Generate the click event.
		lastEvent = ev;
		lastEvent.evt = EVENT_CLICK;

		// Dispatch the click event.
		FEvent* pClickEvent = GenButtonInputEvent(lastEvent);
		if (pClickEvent)
			GetDispatcher()->DispatchEvent(pClickEvent);

		// Check the double click condition.
		if (m_EventQueue.size() > 1)
		{
			ButtonEvent& lastLastEvent = *(m_EventQueue.rbegin() + 1);
			if (lastLastEvent.btn == lastEvent.btn && lastLastEvent.evt == EVENT_CLICK &&
				lastEvent.nTime - lastLastEvent.nTime <= DOUBLE_CLICK_MAX_TIME &&
				CalcDist(lastEvent.cursor_pos, lastLastEvent.cursor_pos) <= CLICK_EVENT_POINT_RANGE)
			{
				// Generate the double click event.
				lastLastEvent = lastEvent;
				lastLastEvent.evt = EVENT_DBLCLICK;
				m_EventQueue.pop_back();

				// Dispatch the double click event.
				FEvent* pDblClkEvent = GenButtonInputEvent(lastLastEvent);
				if (pDblClkEvent)
					GetDispatcher()->DispatchEvent(pDblClkEvent);
			}
		}
	}
	else
	{
		m_EventQueue.push_back(ev);
	}
}