/*
 * ------------------------------------------------------------------------
 *  Name:   FUISystem.h
 *  Desc:   本文件定义了UI子系统的核心控制类，通过该类可以控制整个UI系统。
 *  Author: Yish
 *  Date:   2010/8/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_SYSTEM_H__
#define __FUI_SYSTEM_H__

//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIWindow.h"

//////////////////////////////////////////////////////////////////////////

class FUIInputListener;

/** UI子系统的核心控制类
*/
class FAIRY_API FUISystem : public FGeneralAlloc
{
public:
    typedef std::vector<FUIWindow*> WindowList;
    typedef std::map<AString, FUIWindow*> WindowMap;

public:
    FUISystem(void);
	virtual ~FUISystem(void);

    // Initialize the UI system.
    virtual bool Init();
	// Shutdown the UI system.
	virtual void Shutdown();
	// Update the UI system.
    virtual void Update();

    // Get the specified window.
    FUIWindow* GetUIWindow( const char* name );

	// Get the singleton instance.
	static FUISystem& GetInstance();

private:
	FUIInputListener* m_pInputListener;
	WindowList m_Windows;
    WindowMap m_WindowMap;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_SYSTEM_H__