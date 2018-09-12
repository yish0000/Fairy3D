/*
 * ------------------------------------------------------------------------
 *  Name:   GameDemo.cpp
 *  Desc:   Base class of the application.
 *  Author: Yish
 *  Date:   2015/5/10
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "GameDemo.h"
#include "FInputEngine.h"
#include "FInputEvent.h"
#include "FCommon.h"

CGameDemo theGame;

CGameDemo::CGameDemo() : m_pListener(NULL)
{
	m_cntUpdateFPS.SetPeriod(1000);
}

CGameDemo::~CGameDemo()
{
	F_SAFE_DELETE(m_pListener);
}

void TestInputListener::OnEventKeydown(FInputEvent* pEvent)
{
	FInputKeydownEvent* pKeyDown = dynamic_cast<FInputKeydownEvent*>(pEvent);
	FSysAPI::OutputDebugInfo("Keydown, vk=%d", pKeyDown->vk);
}

void TestInputListener::OnEventLButtonClick(FInputEvent* pEvent)
{
	FInputMouseLButtonClickEvent* pLButtonClick = dynamic_cast<FInputMouseLButtonClickEvent*>(pEvent);
	FSysAPI::OutputDebugInfo("MouseLButtonClick, pos=(%d,%d)", pLButtonClick->cursor_pos.x, pLButtonClick->cursor_pos.y);
}

void TestInputListener::OnEventRButtonDlbClick(FInputEvent* pEvent)
{
	FInputMouseRButtonDblClkEvent* pRButtonDblClick = dynamic_cast<FInputMouseRButtonDblClkEvent*>(pEvent);
	FSysAPI::OutputDebugInfo("MouseRButtonDblClick, pos=(%d,%d)", pRButtonDblClick->cursor_pos.x, pRButtonDblClick->cursor_pos.y);
}

void TestInputListener::OnEventMouseMove(FInputEvent* pEvent)
{
	FInputMouseMoveEvent* pMove = dynamic_cast<FInputMouseMoveEvent*>(pEvent);
	FSysAPI::OutputDebugInfo("MouseMove, pos=(%d,%d)", pMove->cursor_pos.x, pMove->cursor_pos.y);
}

void TestInputListener::OnEventMouseWheel(FInputEvent* pEvent)
{
	FInputMouseWheelEvent* pWheel = dynamic_cast<FInputMouseWheelEvent*>(pEvent);
	FSysAPI::OutputDebugInfo("MouseWheel, pos=(%d,%d), delta=%d", pWheel->cursor_pos.x, pWheel->cursor_pos.y, pWheel->delta);
}

bool CGameDemo::Init()
{
	m_pListener = new TestInputListener();
	FInputEngine::GetInstance().AddEventListener(FINPUT_KEYDOWN_EVENT, m_pListener, FINPUTCALLBACK_MAKE(TestInputListener::OnEventKeydown));
	FInputEngine::GetInstance().AddEventListener(FINPUT_MOUSE_LBUTTON_CLICK, m_pListener, FINPUTCALLBACK_MAKE(TestInputListener::OnEventLButtonClick));
	FInputEngine::GetInstance().AddEventListener(FINPUT_MOUSE_RBUTTON_DBLCLK, m_pListener, FINPUTCALLBACK_MAKE(TestInputListener::OnEventRButtonDlbClick));
	//FInputEngine::GetInstance().AddEventListener(FINPUT_MOUSE_MOVE, m_pListener, FINPUTCALLBACK_MAKE(TestInputListener::OnEventMouseMove));
	FInputEngine::GetInstance().AddEventListener(FINPUT_MOUSE_WHEEL, m_pListener, FINPUTCALLBACK_MAKE(TestInputListener::OnEventMouseWheel));
	return true;
}

void CGameDemo::Shutdown()
{
}

void CGameDemo::Frame()
{
}

void CGameDemo::OnPause()
{
}

void CGameDemo::OnResume()
{
}

void CGameDemo::OnResizeWindow()
{
}