/*
 * ------------------------------------------------------------------------
 *  Name:   FInputMouseWin32.cpp
 *  Desc:   Create the mouse and touch device for windows platform.
 *  Author: Yish
 *  Date:   2016/3/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputEngine.h"
#include "FInputMouse.h"
#include "FInputTouch.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Callback function to listen the Mouse event.
//  
///////////////////////////////////////////////////////////////////////////

static HWND l_AppHWND = NULL;
static class FInputMouseWin32* l_pMouse;
static class FInputTouchWin32* l_pTouch;
static WNDPROC l_fnSavedProc;
static BOOL CALLBACK FInputMouse_EnumThreadWndProc(HWND hWnd, LPARAM lParam);
static LRESULT CALLBACK FInputMouse_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////
//  
//  Mouse device for windows platform.
//  
///////////////////////////////////////////////////////////////////////////

class FInputMouseWin32 : public FInputMouse
{
	friend LRESULT CALLBACK FInputMouse_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	FInputMouseWin32()
	{
		l_pMouse = this;
	}

	// Initialize the mouse device.
	bool Init()
	{
		if (!FInputMouse::Init())
			return false;

		EnumThreadWindows(GetCurrentThreadId(), FInputMouse_EnumThreadWndProc, NULL);
		m_bValid = true;
		return true;
	}

	// Update the mouse device.
	void Update()
	{
		POINT cursorPt, cursorPtClient;
		::GetCursorPos(&cursorPt);
		cursorPtClient = cursorPt;
		::ScreenToClient(l_AppHWND, &cursorPtClient);
		SetCursorPos(FPointI(cursorPt.x, cursorPt.y), FPointI(cursorPtClient.x, cursorPtClient.y));

		FInputMouse::Update();
	}

	// Change the position of cursor. (relative to screen)
	void SetCursorScreenPos(const FPointI& pos)
	{
		FScopedLock lock(m_pMutex);
		::SetCursorPos(pos.x, pos.y);
		m_cursorPosScreen = pos;
		POINT ptClient = { pos.x, pos.y };
		::ScreenToClient(l_AppHWND, &ptClient);
		m_cursorPosClient.Set(ptClient.x, ptClient.y);
	}

	// Change the position of cursor. (relative to client)
	void SetCursorClientPos(const FPointI& pos)
	{
		FScopedLock lock(m_pMutex);
		POINT ptClient = { pos.x, pos.y };
		POINT ptScreen = ptClient;
		::ClientToScreen(l_AppHWND, &ptScreen);
		::SetCursorPos(ptScreen.x, ptScreen.y);
		m_cursorPosScreen.Set(ptScreen.x, ptScreen.y);
		m_cursorPosClient = pos;
	}

	// Show or hide the cursor.
	void ShowCursor(bool bShow)
	{
		FScopedLock lock(m_pMutex);
		if (m_bShowCursor != bShow)
		{
			::ShowCursor(bShow ? TRUE : FALSE);
			m_bShowCursor = bShow;
		}
	}

protected:

	// Set the current position of the cursor.
	void SetCursorPos(const FPointI& pt, const FPointI& ptClient)
	{
		FScopedLock lock(m_pMutex);
		m_cursorPosScreen = pt;
		m_cursorPosClient = ptClient;
	}
};

///////////////////////////////////////////////////////////////////////////
//  
//  Touch pad device for windows platform.
//  
///////////////////////////////////////////////////////////////////////////

class FInputTouchWin32 : public FInputTouch
{
public:
	FInputTouchWin32()
	{
		l_pTouch = this;
	}

	// Initialize the touch device.
	bool Init()
	{
		if (!FInputTouch::Init())
			return false;

		m_bValid = l_pMouse->IsValid();
		return true;
	}
};

///////////////////////////////////////////////////////////////////////////
//  
//  Callback function to handle the windows message.
//  
///////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK FInputMouse_EnumThreadWndProc(HWND hWnd, LPARAM lParam)
{
	if (GetWindowLongPtr(hWnd, GWLP_HWNDPARENT) == 0)
	{
		l_AppHWND = hWnd;
		l_fnSavedProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)FInputMouse_WndProc);
	}

	return TRUE;
}

static LRESULT CALLBACK FInputMouse_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bButtonEvent = false;

	FPointI cursorPos;
	cursorPos.x = LOWORD(lParam);
	cursorPos.y = HIWORD(lParam);

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_LBUTTON, FInputMouse::EVENT_DOWN, cursorPos, (uint32)wParam);
		break;
	case WM_LBUTTONUP:
		l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_LBUTTON, FInputMouse::EVENT_UP, cursorPos, (uint32)wParam);
		break;
	case WM_RBUTTONDOWN:
		l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_RBUTTON, FInputMouse::EVENT_DOWN, cursorPos, (uint32)wParam);
		break;
	case WM_RBUTTONUP:
		l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_RBUTTON, FInputMouse::EVENT_UP, cursorPos, (uint32)wParam);
		break;
	case WM_MBUTTONDOWN:
		l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_MBUTTON, FInputMouse::EVENT_DOWN, cursorPos, (uint32)wParam);
		break;
	case WM_MBUTTONUP:
		l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_MBUTTON, FInputMouse::EVENT_UP, cursorPos, (uint32)wParam);
		break;
	case WM_XBUTTONDOWN:
		if (HIWORD(wParam) & XBUTTON1)
			l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_X1BUTTON, FInputMouse::EVENT_DOWN, cursorPos, (uint32)wParam);
		else if (HIWORD(wParam) & XBUTTON2)
			l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_X2BUTTON, FInputMouse::EVENT_DOWN, cursorPos, (uint32)wParam);
		else
		{
			FASSERT(0);
		}
		break;
	case WM_XBUTTONUP:
		if (HIWORD(wParam) & XBUTTON1)
			l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_X1BUTTON, FInputMouse::EVENT_UP, cursorPos, (uint32)wParam);
		else if (HIWORD(wParam) & XBUTTON2)
			l_pMouse->DispatchButtonEvent(FInputMouse::MOUSE_X2BUTTON, FInputMouse::EVENT_UP, cursorPos, (uint32)wParam);
		else
		{
			FASSERT(0);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			int iDelta = (int)GET_WHEEL_DELTA_WPARAM(wParam);
			uint32 nMask = (uint32)LOWORD(wParam);
			l_pMouse->DispatchWheelEvent(cursorPos, nMask, iDelta);
			break;
		}
	case WM_MOUSEMOVE:
		l_pMouse->DispatchMoveEvent(cursorPos, (uint32)wParam);
		break;
	}

	if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN)
		::SetCapture(hWnd);
	else if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP || uMsg == WM_XBUTTONUP)
		::ReleaseCapture();

	if (l_fnSavedProc)
		return l_fnSavedProc(hWnd, uMsg, wParam, lParam);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////
//  
//  Function to create the mouse device.
//  
///////////////////////////////////////////////////////////////////////////

template <> FInputDevice* CreateInputDevice<IDT_MOUSE>()
{
	return new FInputMouseWin32();
}

template <> FInputDevice* CreateInputDevice<IDT_TOUCH>()
{
	return new FInputTouchWin32();
}