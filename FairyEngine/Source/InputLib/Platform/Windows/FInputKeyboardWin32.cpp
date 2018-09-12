/*
 * ------------------------------------------------------------------------
 *  Name:   FInputKeyboardWin32.cpp
 *  Desc:   Create the keyboard device for windows platform.
 *  Author: Yish
 *  Date:   2016/3/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FInputEngine.h"
#include "FInputKeyboard.h"

///////////////////////////////////////////////////////////////////////////
//  
//  Callback function to listen the keyboard event.
//  
///////////////////////////////////////////////////////////////////////////

static class FInputKeyboardWin32* l_pKeyboard;
static WNDPROC l_fnSavedProc;
static BOOL CALLBACK FInputKeyboard_EnumThreadWndProc(HWND hWnd, LPARAM lParam);
static LRESULT CALLBACK FInputKeyboard_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class FInputKeyboardWin32 : public FInputKeyboard
{
	friend LRESULT CALLBACK FInputKeyboard_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	FInputKeyboardWin32()
	{
		l_pKeyboard = this;
	}

	// Initialize the keyboard device.
	bool Init()
	{
		if (!FInputKeyboard::Init())
			return false;

		EnumThreadWindows(GetCurrentThreadId(), FInputKeyboard_EnumThreadWndProc, NULL);
		m_bValid = true;
		return true;
	}

	// Update the keyboard device.
	void Update()
	{
		for (int i = 0; i < NUM_VIRTUALKEY; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
				SetKeyState(i, true);
			else
				SetKeyState(i, false);
		}

		FInputKeyboard::Update();
	}
};

static BOOL CALLBACK FInputKeyboard_EnumThreadWndProc(HWND hWnd, LPARAM lParam)
{
	if (GetWindowLongPtr(hWnd, GWLP_HWNDPARENT) == 0)
	{
		l_fnSavedProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)FInputKeyboard_WndProc);
	}

	return TRUE;
}

static LRESULT CALLBACK FInputKeyboard_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		{
			bool bDown = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
			l_pKeyboard->DispatchKeyEvent((int)wParam, bDown);
		}
		break;
	}

	if (l_fnSavedProc)
		return l_fnSavedProc(hWnd, uMsg, wParam, lParam);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////
//  
//  Function to create the keyboard device.
//  
///////////////////////////////////////////////////////////////////////////

template <> FInputDevice* CreateInputDevice<IDT_KEYBOARD>()
{
	return new FInputKeyboardWin32();
}