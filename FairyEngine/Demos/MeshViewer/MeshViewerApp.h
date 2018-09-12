//------------------------------------------------------------------------
// Name: MeshViewerApp.h
// Desc: This project implemented an static mesh viewer for engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __MESHVIEWER_APP_H__
#define __MESHVIEWER_APP_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "../Common/AppFramework.h"

#include "FlyCamManager.h"
#include "FlySceneManager.h"

#include "FlyManualEntity.h"
#include "FlyFont.h"
#include "FlyMeshEntity.h"
//////////////////////////////////////////////////////////////////////////


class MeshViewerApp : public AppFramework
{
protected:
    bool m_bKeystate[4];
    POINT m_ptOldCursor;    // Old cursor position.
    POINT m_ptNewCursor;    // New cursor position.
    FlyVector m_vPos;       // Translate the mesh.
    float m_fRotX,m_fRotY;  // Rotate the mesh.
    float m_fScale;         // Scale of the mesh.
    //FlyFont* m_pFont;       // Pointer to the font object.

public:
    // Constructor and destructor.
    MeshViewerApp(void);
    ~MeshViewerApp(void);

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

#endif  //#ifndef __MESHVIEWER_APP_H__