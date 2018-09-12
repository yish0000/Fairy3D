/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTestCase1.cpp
 *  Desc:   本文件用于对本模块进行单元测试。
 *  Author: Yish
 *  Date:   2010/12/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FTestCase.h"
#include <conio.h>
#include <tchar.h>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////
// 测试渲染设备

class F3DTestWinApp
{
public:
	F3DTestWinApp();
	virtual ~F3DTestWinApp();

	bool Init( const char* szWndTitle );
	void Run();
	void Exit();

	// 更新每一帧的逻辑
	void DoFrame();

	// 处理WINDOWS消息
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 获取APP的指针
	static F3DTestWinApp& GetInstance();

protected:
	HINSTANCE m_hInst;
	HWND m_hWnd;

protected:

	// 初始化设备
};

// 窗口回调函数
LRESULT CALLBACK _WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	F3DTestWinApp& app = F3DTestWinApp::GetInstance();
	if( app.MsgProc(hWnd, uMsg, wParam, lParam) )
		return 0;
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////

// 窗口类名
const TCHAR F3D_TESTWND_CLASS[] = _T("F3DTestAppWnd");

/** 构造函数
*/
F3DTestWinApp::F3DTestWinApp()
{
	m_hInst = NULL;
	m_hWnd = NULL;
}

/** 析构函数
*/
F3DTestWinApp::~F3DTestWinApp()
{
}

/** 获取应用程序的实例
*/
F3DTestWinApp& F3DTestWinApp::GetInstance()
{
	static F3DTestWinApp obj;
	return obj;
}

/** 初始化应用程序
*/
bool F3DTestWinApp::Init(const char* szWndTitle)
{
	m_hInst = GetModuleHandle(0);

	return true;
}

/** 程序主循环
*/
void F3DTestWinApp::Run()
{
	MSG msg;
	memset(&msg, 0, sizeof(MSG));

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DoFrame();
		}
	}
}

/** 更新每帧
*/
void F3DTestWinApp::DoFrame()
{
}

/** 退出应用程序
*/
void F3DTestWinApp::Exit()
{
	UnregisterClass(F3D_TESTWND_CLASS, m_hInst);
}

/** 处理WINDOWS消息
*/
bool F3DTestWinApp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return true;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////

F_TESTCASE(TestDevice)
{
	//F3DTestWinApp& app = F3DTestWinApp::GetInstance();
	//if( app.Init("TestDevice") ) app.Run();
	//app.Exit();
	printf("Test the RenderDevice!\n");
}