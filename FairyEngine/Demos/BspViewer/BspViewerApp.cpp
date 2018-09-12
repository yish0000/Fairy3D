//------------------------------------------------------------------------
// Name: BspViewerApp.cpp
// Desc: This file is to implement the class BspViewerApp.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "BspViewerApp.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
BspViewerApp::BspViewerApp(void)
{
    m_sWndTitle = "BspViewer - FR Engine 2.0";

    m_bKeystate[0] = false;
    m_bKeystate[1] = false;
    m_bKeystate[2] = false;
    m_bKeystate[3] = false;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
BspViewerApp::~BspViewerApp(void)
{
}


//------------------------------------------------------------------------
// Initialize the scene contents for the application.
//------------------------------------------------------------------------
HRESULT BspViewerApp::OneTimeSceneInit(void)
{
    FlyKernel& engine = FlyKernel::Instance();

    // Create a bsp sceneManager.
    FlySceneManager* pManager = engine.CreateSceneManager( "Castle","bsp" );
    if( !pManager )
    {
        OnErrorExit( "Create the bsp sceneManager failed! The application will exit." );
        return FLY_FAIL;
    }

    // Load the world geometry data.
    pManager->LoadWorldGeometry( "../../Media/Q3Levels/µØÀÎ/testmap.bsp" );

    // Set the active sceneManager.
    engine.SetActiveSceneManager( pManager );

    // Create a new camera.
    FlyCamManager* pCamMgr = engine.GetCamManager();
    FlyCameraBase* pCamera = pCamMgr->CreateCamera( "DefaultCamera" );
    pManager->SetCurrentCamera( pCamera );
    pCamera->SetPosition( 0.0f,5.0f,0.0f );

    // Create a system font.
    //m_pFont = engine.GetResourceManager()->MakeSystemFont();

    return FLY_OK;
}


//------------------------------------------------------------------------
// The first CALLBACK function for the engine.
//------------------------------------------------------------------------
void BspViewerApp::OnEngineRunningA(void)
{
    // Get the camera of the scene.
    FlyCameraBase* pCamera = FlyKernel::Instance().GetActiveSceneManager()->GetCurrentCamera();

    if( m_bKeystate[0] )
        pCamera->Yaw( -0.05f );

    if( m_bKeystate[1] )
        pCamera->Yaw( 0.05f );

    if( m_bKeystate[2] )
        pCamera->MoveRelativeZ( 1.0f );

    if( m_bKeystate[3] )
        pCamera->MoveRelativeZ( -1.0f );
}


//------------------------------------------------------------------------
// The second CALLBACK function for the engine.
//------------------------------------------------------------------------
void BspViewerApp::OnEngineRunningB(void)
{
//     LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();
//     FlyTimer* pTimer = FlyKernel::Instance().GetTimer();
// 
//     m_pFont->RenderText( 10,15,FLYCOLOR::Red,"Scene Type: Bsp/Indoor level" );
//     m_pFont->RenderText( 10,35,FLYCOLOR::Red,"Triangles: %d",pRenderer->GetNumRenderedPrims() );
//     m_pFont->RenderText( 10,55,FLYCOLOR::Green,"Current FPS: %.3f",pTimer->GetFPS() );
//     m_pFont->RenderText( 10,75,FLYCOLOR::Red,"Objects: %d",pRenderer->GetNumRenderedObjects() );
}


//------------------------------------------------------------------------
// Callback function for the window.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
LRESULT BspViewerApp::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    switch( uMsg )
    {
    case WM_KEYDOWN:
        switch( wParam )
        {
        case VK_LEFT:
            m_bKeystate[0] = true;
            break;
        case VK_RIGHT:
            m_bKeystate[1] = true;
            break;
        case VK_UP:
            m_bKeystate[2] = true;
            break;
        case VK_DOWN:
            m_bKeystate[3] = true;
            break;
        }
        break;
    case WM_KEYUP:
        switch( wParam )
        {
        case VK_LEFT:
            m_bKeystate[0] = false;
            break;
        case VK_RIGHT:
            m_bKeystate[1] = false;
            break;
        case VK_UP:
            m_bKeystate[2] = false;
            break;
        case VK_DOWN:
            m_bKeystate[3] = false;
            break;
        }
        break;
    }

    return AppFramework::MsgProc( hWnd,uMsg,wParam,lParam );
}