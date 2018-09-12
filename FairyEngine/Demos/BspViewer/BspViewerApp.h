//------------------------------------------------------------------------
// Name: BspViewerApp.h
// Desc: This project implemented a bsp map viewer for engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __BSPVIEWER_APP_H__
#define __BSPVIEWER_APP_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "../Common/AppFramework.h"

#include "FlyCamManager.h"
#include "FlySceneManager.h"
//#include "FlyFont.h"
//////////////////////////////////////////////////////////////////////////


class BspViewerApp : public AppFramework
{
protected:
    bool m_bKeystate[4];
    //FlyFont* m_pFont;

public:
    // Constructor and destructor.
    BspViewerApp(void);
    ~BspViewerApp(void);

    // Override the callback function.
    LRESULT MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    void OnEngineRunningA(void);
    void OnEngineRunningB(void);

protected:

    // Create the scene Content.
    HRESULT OneTimeSceneInit(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __BSPVIEWER_APP_H__