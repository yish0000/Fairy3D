//------------------------------------------------------------------------
// Name: AppUIManager.h
// Desc: This file define a class to control the editor's surface.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __APPUI_MANAGER_H__
#define __APPUI_MANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "FlyTypes.h"

#include "uiStatusBar.h"
#include "uiToolDialog.h"
#include "uiManager.h"
#include "uiMacros.h"

#include "Res/resource.h"
//////////////////////////////////////////////////////////////////////////


class CAppUIManager
{
protected:
    HINSTANCE       m_hInst;        // Handle to the application.
    HWND            m_hWnd;         // Handle to the main window.
    uiManager       m_uiMgr;        // UI Manager.
    uiStatusBar     m_uiStatus;     // Status bar.
    uiToolDialog    m_uiTool;       // Some Tool dialog.

public:
    // Constructor and destructor.
    CAppUIManager(void)  { /* Nothing to do! */ };
    ~CAppUIManager(void) { /* Nothing to do! */ };

    bool Init( HINSTANCE hInst,HWND hWnd );
    bool CreateControls(void);

    void MsgProcGUI( WPARAM wParam,LPARAM lParam );

    bool ShowSelTextureDlg( HWND hWnd,char* cFilename );
    bool ShowSelShapeDlg( HWND hWnd,char* cFilename );
    bool ShowVectorSetting( HWND hWnd,FlyVector* pVec );

    uiManager*      GetUIManager(void) { return &m_uiMgr; }
    uiStatusBar*    GetStatusBar(void) { return &m_uiStatus; }
    uiToolDialog*   GetToolDialog(void) { return &m_uiTool; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __APPUI_MANAGER_H__