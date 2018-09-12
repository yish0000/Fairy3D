//------------------------------------------------------------------------
// Name: Application.h
// Desc: This file define a class to control the Particle editor.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __APPLICATION_H__
#define __APPLICATION_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Res/resource.h"

#include "Fly.h"
#include "AppUIManager.h"
#include "AppRenderer.h"
#include "AppKFManager.h"
//////////////////////////////////////////////////////////////////////////


const char g_szWndClass[] = "ParticleEditor";
const char g_szWndTitle[] = "Á£×Ó±à¼­Æ÷ v1.1";

// CALLBACK Function of the window.
LRESULT CALLBACK WndProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

// Class to control the editor.
class CApplication
{
    friend class CAppUIManager;

protected:
    HINSTANCE   m_hInst;
    HWND        m_hWnd;
    HWND        m_hRender;
    char        m_cCurFile[256];
    bool        m_bChanged;

    CAppUIManager m_AppUI;
    CAppRenderer  m_Renderer;
    CAppKFManager m_AppKF;

public:
    // Constructor and destructor.
    CApplication(void);
    ~CApplication(void);

    BOOL Create( HINSTANCE hInst );
    INT  Run(void);
    void Release(void);
    bool ProgramTick(void);
    void OnExit(void);

    HRESULT InitApplication(void);
    void OnMouseMove( WPARAM wParam,LPARAM lParam );
    void OnMouseWheel( WPARAM wParam,LPARAM lParam );

    HINSTANCE GetHandle(void) { return m_hInst; }
    HWND GetHWND(void) { return m_hWnd; }
    CAppUIManager* GetUIManager(void) { return &m_AppUI; }
    CAppKFManager* GetKFManager(void) { return &m_AppKF; }
    CAppRenderer* GetAppRenderer(void) { return &m_Renderer; }

    void MsgProcMenu( WPARAM wParam );
    void MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __APPLICATION_H__