//------------------------------------------------------------------------
// Name: AppRenderer.h
// Desc: This file define a class to do the Render things for the 
//       application.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __APP_RENDERER_H__
#define __APP_RENDERER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "stl_user_config.h"
#include "FlyTypes.h"
#include "FlyInputDevice.h"

#include "FlyTimer.h"
#include "FlyParticleSystem.h"
#include "FlyRootManager.h"

#include "AppCamera.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Class to control the ParticleSystem.
class CAppRenderer
{
protected:
    FlyRootManager*     m_pRoot;
    LPFLYRENDERDEVICE   m_pDevice;
    LPFLYSKINMANAGER    m_pSkinMgr;
    LPFLYVCACHEMANAGER  m_pVCMgr;
    LPFLYINPUTDEVICE    m_pFI;

    FlyParticleSystem   m_System;
    HINSTANCE           m_hInst;
    HWND                m_hWnd;
    HWND                m_hRender;
    bool                m_bPreview;
    float               m_fElapsed;
    float               m_fFPS;
    CAppCamera*         m_pCamera;
    FlyTimer*           m_pTimer;
    VERTEX*             m_pVerts;

    bool InitFIDevice(void);
    void RenderAxis(void);
    void PrepareAxis(void);

public:
    // Constructor and destructor.
    CAppRenderer(void);
    ~CAppRenderer(void);

    bool Create( HINSTANCE hInst,HWND hWnd,HWND hRender );
    void Release(void);
    void Update(void);
    void Render(void);
    void Preview( bool bPre );
    void HandleInputEvents(void);

    HINSTANCE GetAppInstance(void) { return m_hInst; }
    HWND GetMainWindow(void) { return m_hWnd; }
    HWND GetRenderWindow(void) { return m_hRender; }
    CAppCamera* GetCameraPtr(void) { return m_pCamera; }
    FlyParticleSystem* GetParticleSystem(void) { return &m_System; }
    float GetFPS(void) { return m_fFPS; }
    UINT GetNumParticles(void) { return m_System.GetNumActiveParticles(); }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __APP_RENDERER_H__