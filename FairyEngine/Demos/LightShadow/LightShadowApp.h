//------------------------------------------------------------------------
// Name: LightShadowApp.h
// Desc: This application is to test the light and shadow system.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __LIGHTSHADOWAPP_H__
#define __LIGHTSHADOWAPP_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "../Common/AppFramework.h"

#include "FlyCamManager.h"
#include "FlySceneManager.h"
#include "FlyLightObject.h"

//#include "FlyFont.h"
#include "FlyMeshEntity.h"
//////////////////////////////////////////////////////////////////////////


class LightShadowApp : public AppFramework
{
protected:
    POINT m_ptOldCursor;    // Old cursor position.
    POINT m_ptNewCursor;    // New cursor position.
    FlyVector m_vPos;       // Translate the mesh.
    float m_fRotX,m_fRotY;  // Rotate the mesh.
    float m_fScale;         // Scale of the mesh.

    //FlyFont* m_pFont;       // A font object to print rendering infos.
    bool m_bKeystate[4];    // Up,down,left,right.

public:
    // Constructor and destructor.
    LightShadowApp(void);
    ~LightShadowApp(void);

    // Override the callback function.
    LRESULT MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    void OnEngineRunningA(void);
    void OnEngineRunningB(void);

protected:
    void OnMouseMove( WPARAM wParam,LPARAM lParam );
    void OnMouseWheel( WPARAM wParam,LPARAM lParam );

    // Create the scene Content.
    HRESULT OneTimeSceneInit(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __LIGHTSHADOWAPP_H__