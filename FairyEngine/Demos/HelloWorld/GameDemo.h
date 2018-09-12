/*
 * ------------------------------------------------------------------------
 *  Name:   GameDemo.h
 *  Desc:   A example of Fairy3D engine.
 *  Author: Yish
 *  Date:   2015/5/10
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __GAMEDEMO_H__
#define __GAMEDEMO_H__

#include "FGame.h"
#include "FUIInputListener.h"
#include "FCounter.h"
#include "FThread.h"

class TestInputListener : public FInputEventListener
{
public:
	void OnEventKeydown(FInputEvent* pEvent);
	void OnEventLButtonClick(FInputEvent* pEvent);
	void OnEventRButtonDlbClick(FInputEvent* pEvent);
	void OnEventMouseMove(FInputEvent* pEvent);
	void OnEventMouseWheel(FInputEvent* pEvent);
};

class CGameDemo : public FGame
{
public:
	CGameDemo();
	virtual ~CGameDemo();

	virtual bool Init();
	virtual void Shutdown();
	virtual void Frame();
	virtual void OnPause();
	virtual void OnResume();
	virtual void OnResizeWindow();

protected:
	FCounter m_cntUpdateFPS;
	TestInputListener* m_pListener;
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __GAMEDEMO_H__