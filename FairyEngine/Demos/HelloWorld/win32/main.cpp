/*
 * ------------------------------------------------------------------------
 *  Name:   main.cpp
 *  Desc:   Entry function of the win32 application.
 *  Author: Yish
 *  Date:   2015/5/10
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FCommon.h"
#include "FApplication.h"

static bool l_bAppActive = false;
LRESULT CALLBACK FWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	const char szWindowTitle[] = "Fairy Game Window";
	const char szClassName[] = "FairyWindowClass";

	WNDCLASSEX wnd;
	memset(&wnd, 0, sizeof(wnd));
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hInstance = hInstance;
	wnd.lpszClassName = szClassName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = (WNDPROC)FWndProc;
	RegisterClassEx(&wnd);

	// Create the window.
	uint32 nWndStyle = WS_OVERLAPPEDWINDOW;
	RECT rcWnd = { 0, 0, 960, 640 };
	AdjustWindowRect(&rcWnd, nWndStyle, FALSE);
	uint32 nWindowWidth = rcWnd.right - rcWnd.left;
	uint32 nWindowHeight = rcWnd.bottom - rcWnd.top;
	HWND hWnd = CreateWindow(szClassName, szWindowTitle, nWndStyle, CW_USEDEFAULT, CW_USEDEFAULT, nWindowWidth, nWindowHeight, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		return -1;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Initialize the application.
	if (!FApplication::GetInstance()->OnAppInit())
	{
		UnregisterClass(szClassName, hInstance);
		return -1;
	}

	// Main loop of the application.
	MSG msg = {0};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Update each frames.
			FApplication::GetInstance()->OnFrame();
		}

		if (!l_bAppActive)
			Sleep(5);
	}

	FApplication::GetInstance()->OnAppShutdown();

	UnregisterClass(szClassName, hInstance);
	return 0;
}

LRESULT CALLBACK FWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		l_bAppActive = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		{
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			uint32 nWidth = rcClient.right - rcClient.left;
			uint32 nHeight = rcClient.bottom - rcClient.top;
			FApplication::GetInstance()->OnResizeWindow(nWidth, nHeight);
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}