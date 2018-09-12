//------------------------------------------------------------------------
// Name: TerrainViewer.cpp
// Desc: This file is to implement the class TerrainViewerApp.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "TerrainViewer.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
TerrainViewerApp::TerrainViewerApp(void)
{
    m_sWndTitle = "TerrainViewer - FR Engine 2.0";

    m_bKeystate[0] = false;
    m_bKeystate[1] = false;
    m_bKeystate[2] = false;
    m_bKeystate[3] = false;

    m_bShiftKey = false;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
TerrainViewerApp::~TerrainViewerApp(void)
{
}


//------------------------------------------------------------------------
// Initialize the scene contents for the application.
//------------------------------------------------------------------------
HRESULT TerrainViewerApp::OneTimeSceneInit(void)
{
    FlyKernel& engine = FlyKernel::Instance();

    // Create a terrain sceneManager.
    FlySceneManager* pManager = engine.CreateSceneManager( "Test-Terrain","terrain" );
    if( !pManager )
    {
        OnErrorExit( "Create the terrain sceneManager failed! The application will exit." );
        return FLY_FAIL;
    }

    // Load the world geometry data.
    pManager->LoadWorldGeometry( "../../Media/Terrains/testMap.map" );

//     // Create a mesh entity.
//     FlyMeshEntity* pMesh = pManager->CreateMeshEntity( "tianhe","Model\\tianhe.smd" );
//     pManager->GetSceneRoot()->AttachObject( (FlySceneObject*)pMesh );
// 
//     FlySceneNode* pNode = pManager->GetSceneRoot()->CreateChildNode( FlyVector(50.0f,0.0f,0.0f),
//         FlyQuat(0.0f,0.0f,0.0f,1.0f) );
//     pMesh = pManager->CreateMeshEntity( "mengli","Model\\mengli.smd" );
//     pNode->AttachObject( (FlySceneObject*)pMesh );
//     pNode->ShowBoundingBox( true );
//     pManager->GetSceneRoot()->ShowBoundingBox( true );

    // Set the active sceneManager.
    engine.SetActiveSceneManager( pManager );

    // Create a new camera.
    FlyCamManager* pCamMgr = engine.GetCamManager();
    FlyCameraBase* pCamera = pCamMgr->CreateCamera( "DefaultCamera" );
    pManager->SetCurrentCamera( pCamera );
    pCamera->SetPosition( 255.0f,10.0f,255.0f );

    // Create a system font.
    //m_pFont = engine.GetResourceManager()->MakeSystemFont();

    return FLY_OK;
}


//------------------------------------------------------------------------
// The first CALLBACK function for the engine.
//------------------------------------------------------------------------
void TerrainViewerApp::OnEngineRunningA(void)
{
    // Get the sceneManager.
    FlyTerrainSceneManager* pManager = 
        (FlyTerrainSceneManager*)FlyKernel::Instance().GetActiveSceneManager();

    // Get the active camera.
    FlyCameraBase* pCamera = pManager->GetCurrentCamera();

    if( m_bKeystate[0] )
    {
        if( m_bShiftKey )
        {
            pCamera->Yaw( -0.05f );
        }
        else
        {
            pCamera->MoveRelativeX( -1.0f );

            FlyVector vCamPos = pCamera->GetPosition();
            vCamPos.y = pManager->GetHeightValue(vCamPos) + 5.0f;
            pCamera->SetPosition( vCamPos );
        }
    }

    if( m_bKeystate[1] )
    {
        if( m_bShiftKey )
        {
            pCamera->Yaw( 0.05f );
        }
        else
        {
            pCamera->MoveRelativeX( 1.0f );

            FlyVector vCamPos = pCamera->GetPosition();
            vCamPos.y = pManager->GetHeightValue(vCamPos) + 5.0f;
            pCamera->SetPosition( vCamPos );
        }
    }

    if( m_bKeystate[2] )
    {
        pCamera->MoveRelativeZ( 1.0f );

        FlyVector vCamPos = pCamera->GetPosition();
        vCamPos.y = pManager->GetHeightValue(vCamPos) + 5.0f;
        pCamera->SetPosition( vCamPos );
    }

    if( m_bKeystate[3] )
    {
        pCamera->MoveRelativeZ( -1.0f );

        FlyVector vCamPos = pCamera->GetPosition();
        vCamPos.y = pManager->GetHeightValue(vCamPos) + 5.0f;
        pCamera->SetPosition( vCamPos );
    }
}


//------------------------------------------------------------------------
// The second CALLBACK function for the engine.
//------------------------------------------------------------------------
void TerrainViewerApp::OnEngineRunningB(void)
{
//     LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();
//     FlyTimer* pTimer = FlyKernel::Instance().GetTimer();
// 
//     m_pFont->RenderText( 10,15,FLYCOLOR::Red,"Scene Type: Terrain Level" );
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
LRESULT TerrainViewerApp::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
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
        case 'W':
            static_cast<FlyTerrainSceneManager*>(FlyKernel::Instance().GetActiveSceneManager())->SetWireMode( true );
            break;
        case 'S':
            static_cast<FlyTerrainSceneManager*>(FlyKernel::Instance().GetActiveSceneManager())->SetWireMode( false );
            break;
        case VK_SHIFT:
            m_bShiftKey = true;
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
        case VK_SHIFT:
            m_bShiftKey = false;
            break;
        }
        break;
    }

    return AppFramework::MsgProc( hWnd,uMsg,wParam,lParam );
}