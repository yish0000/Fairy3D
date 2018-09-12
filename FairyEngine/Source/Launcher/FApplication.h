/*
 * ------------------------------------------------------------------------
 *  Name:   FApplication.h
 *  Desc:   Base class of the application.
 *  Author: Yish
 *  Date:   2015/5/10
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_APPLICATION_H__
#define __FAIRY_APPLICATION_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"
#include "FGame.h"

///////////////////////////////////////////////////////////////////////////

class FApplication : public FGeneralAlloc
{
public:
	virtual ~FApplication();

	// Initialize the application.
	virtual bool OnAppInit();
	// Shutdown the application.
	virtual void OnAppShutdown();
	// When the application enter background.
	virtual void OnAppEnterBackground();
	// When the application enter foreground.
	virtual void OnAppEnterForeground();
	// Run the application.
	virtual void OnFrame();
	// When the window is resized.
	virtual void OnResizeWindow(uint32 nWidth, uint32 nHeight);

	static FApplication* GetInstance();

protected:
	bool m_bInited;		// Is the application already initialized successfully ?

protected:
	FApplication();
	F_DISABLE_COPY_ASSIGNMENT(FApplication)
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_APPLICATION_H__