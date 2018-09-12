//------------------------------------------------------------------------
// Name: TestShader.h
// Desc: This file define a class to control the application.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __TESTSHADER_H__
#define __TESTSHADER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "../Common/AppFramework.h"

#include "FlySceneManager.h"
#include "FlyManualEntity.h"
#include "FlyCamManager.h"
//#include "FlyFont.h"
//////////////////////////////////////////////////////////////////////////


class TestShaderApp : public AppFramework
{
protected:
    FlyRenderEffect* m_pEffect; // Render effect used for app.
    FlyRenderMaterial* m_pMaterial;
    //FlyFont* m_pFont;
    FlyTexture* m_pTexture1;
    FlyTexture* m_pTexture2;

    POINT m_ptOldCursor;    // Old cursor position.
    POINT m_ptNewCursor;    // New cursor position.
    FlyVector m_vPos;       // Translate the mesh.
    float m_fRotX,m_fRotY;  // Rotate the mesh.
    float m_fScale;         // Scale of the mesh.

public:
    // Constructor and destructor.
    TestShaderApp(void);
    ~TestShaderApp(void);

    // Override the CALLBACK function for app.
    LRESULT MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    void OnEngineRunningA(void);
    void OnEngineRunningB(void);

protected:

    // Override the virtual functions.
    HRESULT OneTimeSceneInit(void);

    void OnMouseMove( WPARAM wParam,LPARAM lParam );
    void OnMouseWheel( WPARAM wParam,LPARAM lParam );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __TESTSHADER_H__