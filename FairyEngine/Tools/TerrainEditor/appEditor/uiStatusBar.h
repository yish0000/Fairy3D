//------------------------------------------------------------------------
// Name: uiStatusBar.h
// Desc: This file define a class manage the StatusBar
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#ifndef __UI_STATUS_BAR_H__
#define __UI_STATUS_BAR_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>

//////////////////////////////////////////////////////////////////////////

class uiStatusBar
{
protected:
    HWND    m_hWnd;     // Parent window
    HWND    m_hBar;     // Status bar's handle
    int     m_nNumBars; // Bar's number

public:
    /* Constructor and destructor */
    uiStatusBar(void) { /* Nothing to do! */ };
    ~uiStatusBar(void) { /* Nothing to do! */ };

    HWND    GetSafeHandle(void)	{ return m_hBar; }
    BOOL    CreateStatusBar( HWND hWnd,int nNumBars );
    void    SetStatusText( int nID,const char* chText );

    void    OnSize( UINT uMsg,WPARAM wParam,LPARAM lParam );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __STATUS_BAR_H__