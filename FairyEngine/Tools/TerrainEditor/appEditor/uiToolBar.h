//------------------------------------------------------------------------
// Name: uiToolBar.h
// Desc: This file is to control the toolBar for the application.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __UI_TOOLBAR_H__
#define __UI_TOOLBAR_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "res/resource.h"

#include <windows.h>
#include <commctrl.h>
//////////////////////////////////////////////////////////////////////////

class uiToolBar
{
protected:
    HINSTANCE m_hInst;
    HWND m_hWnd;
    HWND m_hToolBar;

public:
    // Constructor and destructor.
    uiToolBar(void) { /* Nothing to do! */ };
    ~uiToolBar(void) { /* Nothing to do! */ };

    BOOL CreateToolBar( HINSTANCE hInst,HWND hWnd );
    BOOL CreateButtons(void);
    void EnableButton( UINT nID,bool bEnable );
    HWND GetSafeHandle(void) { return m_hToolBar; }
    void CheckButton( UINT nID,bool bChecked );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __UI_TOOLBAR_H__