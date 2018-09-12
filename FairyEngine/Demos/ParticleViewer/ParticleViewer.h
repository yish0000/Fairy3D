//------------------------------------------------------------------------
// Name: ParticleViewer.h
// Desc: This application is to test the particle system of the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __PARTICLEVIEWER_H__
#define __PARTICLEVIEWER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "../Common/AppFramework.h"

#include "FlySceneManager.h"
#include "FlyCamManager.h"
#include "FlyManualEntity.h"
#include "FlyParticleEntity.h"
//////////////////////////////////////////////////////////////////////////

/*
* This class control the application that control the 
* particle system of the engine.
*/
class ParticleViewerApp : public AppFramework
{
protected:
    POINT m_ptOldCursor;        // Old cursor position.
    POINT m_ptNewCursor;        // New cursor position.
    FlyVector m_vPos;           // Translate the mesh.
    float m_fRotX,m_fRotY;      // Rotate the mesh.
    float m_fScale;             // Scale of the mesh.

public:
    // Constructor and destructor.
    ParticleViewerApp(void);
    ~ParticleViewerApp(void);

    // Override the callback function.
    LRESULT MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );
    
    void OnEngineRunningA(void);
    void OnEngineRunningB(void);

protected:
    void OnMouseMove( WPARAM wParam,LPARAM lParam );
    void OnMouseWheel( WPARAM wParam,LPARAM lParam );

    // Create the scene Content.
    HRESULT OneTimeSceneInit(void);

    // Build the world axis for application.
    HRESULT BuildWorldAxis( FlySceneManager* pManager );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __PARTICLEVIEWER_H__