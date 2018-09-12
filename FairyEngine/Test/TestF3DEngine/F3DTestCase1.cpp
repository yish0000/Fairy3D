/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTestCase1.cpp
 *  Desc:   ���ļ����ڶԱ�ģ����е�Ԫ���ԡ�
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
// ������Ⱦ�豸

class F3DTestWinApp
{
public:
	F3DTestWinApp();
	virtual ~F3DTestWinApp();

	bool Init( const char* szWndTitle );
	void Run();
	void Exit();

	// ����ÿһ֡���߼�
	void DoFrame();

	// ����WINDOWS��Ϣ
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// ��ȡAPP��ָ��
	static F3DTestWinApp& GetInstance();

protected:
	HINSTANCE m_hInst;
	HWND m_hWnd;

protected:

	// ��ʼ���豸
};

// ���ڻص�����
LRESULT CALLBACK _WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	F3DTestWinApp& app = F3DTestWinApp::GetInstance();
	if( app.MsgProc(hWnd, uMsg, wParam, lParam) )
		return 0;
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////

// ��������
const TCHAR F3D_TESTWND_CLASS[] = _T("F3DTestAppWnd");

/** ���캯��
*/
F3DTestWinApp::F3DTestWinApp()
{
	m_hInst = NULL;
	m_hWnd = NULL;
}

/** ��������
*/
F3DTestWinApp::~F3DTestWinApp()
{
}

/** ��ȡӦ�ó����ʵ��
*/
F3DTestWinApp& F3DTestWinApp::GetInstance()
{
	static F3DTestWinApp obj;
	return obj;
}

/** ��ʼ��Ӧ�ó���
*/
bool F3DTestWinApp::Init(const char* szWndTitle)
{
	m_hInst = GetModuleHandle(0);

	return true;
}

/** ������ѭ��
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

/** ����ÿ֡
*/
void F3DTestWinApp::DoFrame()
{
}

/** �˳�Ӧ�ó���
*/
void F3DTestWinApp::Exit()
{
	UnregisterClass(F3D_TESTWND_CLASS, m_hInst);
}

/** ����WINDOWS��Ϣ
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