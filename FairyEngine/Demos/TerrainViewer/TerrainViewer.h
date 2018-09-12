//------------------------------------------------------------------------
// Name: TerrainViewer.h
// Desc: This project implemented an terrain viewer for engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __TERRAIN_VIEWER_H__
#define __TERRAIN_VIEWER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "../Common/AppFramework.h"

#include "FlyCamManager.h"
#include "FlyTerrainSceneManager.h"
//#include "FlyFont.h"
#include "FlyMeshEntity.h"
//////////////////////////////////////////////////////////////////////////


class TerrainViewerApp : public AppFramework
{
protected:
    bool m_bKeystate[4];
    bool m_bShiftKey;
    //FlyFont* m_pFont;

public:
    // Constructor and destructor.
    TerrainViewerApp(void);
    ~TerrainViewerApp(void);

    // Override the callback function.
    LRESULT MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    void OnEngineRunningA(void);
    void OnEngineRunningB(void);

protected:

    // Create the scene Content.
    HRESULT OneTimeSceneInit(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __TERRAIN_VIEWER_H__