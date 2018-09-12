//------------------------------------------------------------------------
// Name: LightShadowApp.cpp
// Desc: This file is to implement the class LightShadowApp.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#define _WIN32_WINNT 0x0400

#include "LightShadowApp.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
LightShadowApp::LightShadowApp(void)
{
    m_sWndTitle = "Shadow Volume - FR Engine 2.0";

    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fScale = 1.0f;
    memset( &m_vPos,0,sizeof(FlyVector) );
    //m_pFont = NULL;

    m_bKeystate[0] = false;
    m_bKeystate[1] = false;
    m_bKeystate[2] = false;
    m_bKeystate[3] = false;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
LightShadowApp::~LightShadowApp(void)
{
}


//------------------------------------------------------------------------
// Initialize the scene content for the application.
//------------------------------------------------------------------------
HRESULT LightShadowApp::OneTimeSceneInit(void)
{
    FlyKernel& engine = FlyKernel::Instance();

    // Create a new sceneManager.
    FlySceneManager* pManager = engine.CreateSceneManager( "TestShadow","default" );
    if( !pManager )
    {
        OnErrorExit( "Create the sceneManager failed, The application will exit!" );
        return FLY_OK;
    }

    // Set the backcolor.
    engine.GetRenderDevice()->SetClearColor( 0.1f,0.1f,0.1f );

    // Set the ambient color.
    FLYCOLOR color;
    color.fR = 0.1f;
    color.fG = 0.1f;
    color.fB = 0.1f;
    color.fA = 1.0f;
    pManager->SetAmbientColor( color );

    FlyMeshEntity* pObject = pManager->CreateMeshEntity( "Ground","../../Media/Models/Teapot/Ground.smd" );
    pObject->SetLightingMode( true );
    pManager->GetSceneRoot()->AttachObject( pObject );

    FlySceneNode* pChild = pManager->GetSceneRoot()->CreateChildNode();
    pObject = pManager->CreateMeshEntity( "Teapot","../../Media/Models/Teapot/Teapot.smd" );
    pObject->SetLightingMode( true );
    pObject->SetHaveShadow( true );
    pChild->AttachObject( pObject );

    // Create a light object.
    FlyVector vPosition( 150.0f,150.0f,0.0f );
    FlyLightObject* pLight = pManager->CreateLightObject( "Light1","default" );
    pLight->SetLightType( LGT_POINT );
    pLight->SetPosition( vPosition );
    pLight->SetDiffuseColor( FLYCOLOR::White );
    pLight->SetRange( 1000.0f );
    pLight->SetAttenuationA( 0.9f );
    pLight->SetAttenuationB( 0.0f );
    pLight->SetCastShadows( true );
    pManager->GetSceneRoot()->AttachLightObject( pLight );
    pManager->SetShadowMode( true );

    engine.SetActiveSceneManager( pManager );

    // Create a new camera.
    FlyCamManager* pCamMgr = engine.GetCamManager();
    FlyCameraBase* pCamera = pCamMgr->CreateCamera( "DefaultCamera" );
    pManager->SetCurrentCamera( pCamera );
    pCamera->SetPosition( 0.0f,10.0f,-120.0f );
    pCamera->LookAt( 0.0f,0.0f,0.0f );

    // Create a font object.
    //m_pFont = engine.GetResourceManager()->MakeSystemFont();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Callback function called before rendering the scene.
//------------------------------------------------------------------------
void LightShadowApp::OnEngineRunningA(void)
{
    FlySceneManager* pManager = FlyKernel::Instance().GetActiveSceneManager();

    FlyQuat qX,qY,quat;
    qX.FromAngleAxis( FlyVector(1.0f,0.0f,0.0f),m_fRotX );
    qY.FromAngleAxis( FlyVector(0.0f,1.0f,0.0f),m_fRotY );
    quat = qX * qY;

    FlySceneNode* pNode = pManager->GetSceneRoot()->GetChildNode(0);
    pNode->SetPosition( m_vPos );
    pNode->SetOrientation( quat );
    pNode->SetScale( FlyVector(m_fScale,m_fScale,m_fScale) );

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
// Callback function called after rendering the scene.
//------------------------------------------------------------------------
void LightShadowApp::OnEngineRunningB(void)
{
//     LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();
//     FlyTimer* pTimer = FlyKernel::Instance().GetTimer();
// 
//     m_pFont->RenderText( 10,15,FLYCOLOR::Red,"Triangles: %d",pRenderer->GetNumRenderedPrims() );
//     m_pFont->RenderText( 10,35,FLYCOLOR::Green,"Current FPS: %.3f",pTimer->GetFPS() );
//     m_pFont->RenderText( 10,55,FLYCOLOR::Red,"Objects: %d",pRenderer->GetNumRenderedObjects() );
}


//------------------------------------------------------------------------
// Do things when move the mouse.
//------------------------------------------------------------------------
void LightShadowApp::OnMouseMove( WPARAM wParam,LPARAM lParam )
{
    LONG lDiffX,lDiffY;

    m_ptOldCursor = m_ptNewCursor;
    m_ptNewCursor.x = LOWORD( lParam );
    m_ptNewCursor.y = HIWORD( lParam );

    lDiffX = m_ptNewCursor.x - m_ptOldCursor.x;
    lDiffY = m_ptNewCursor.y - m_ptOldCursor.y;

    if( wParam & MK_LBUTTON )
    {
        if( lDiffX > 0 ) m_fRotY -= 0.05f;
        if( lDiffX < 0 ) m_fRotY += 0.05f;
        if( lDiffY > 0 ) m_fRotX -= 0.05f;
        if( lDiffY < 0 ) m_fRotX += 0.05f;
    }

    if( wParam & MK_RBUTTON )
    {
        if( lDiffX > 0 ) m_vPos.x += 1.0f;
        if( lDiffX < 0 ) m_vPos.x -= 1.0f;
        if( lDiffY > 0 ) m_vPos.y -= 1.0f;
        if( lDiffY < 0 ) m_vPos.y += 1.0f;
    }
}


//------------------------------------------------------------------------
// Do things when the mouse wheel is rolled
//------------------------------------------------------------------------
void LightShadowApp::OnMouseWheel( WPARAM wParam,LPARAM lParam )
{
    short lDelta = (short)HIWORD( wParam );

    FlyCameraBase* pCamera = FlyKernel::Instance().GetActiveSceneManager()->GetCurrentCamera();

    if( lDelta > 0 )
    {
        m_fScale += 0.05f;
        pCamera->Pitch( 0.05f );
    }

    if( lDelta < 0 )
    {
        m_fScale -= 0.05f;
        pCamera->Pitch( -0.05f );
    }

    if( m_fScale < 0.1f ) m_fScale = 0.1f;
    if( m_fScale > 10.0f ) m_fScale = 10.0f;
}


//------------------------------------------------------------------------
// CALLBACK Function to handle the application's window events.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the main window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
LRESULT LightShadowApp::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    switch( uMsg )
    {
    case WM_MOUSEMOVE:
        OnMouseMove( wParam,lParam );
        break;
    case WM_MOUSEWHEEL:
        OnMouseWheel( wParam,lParam );
        break;
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
        case 'P':
            FlyKernel::Instance().SaveScreenshot();
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