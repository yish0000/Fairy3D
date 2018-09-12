//------------------------------------------------------------------------
// Name: AnimViewerApp.cpp
// Desc: This file is to implement the class AnimViewerApp.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#define _WIN32_WINNT 0x0400

#include "AnimViewerApp.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
AnimViewerApp::AnimViewerApp(void)
{
    m_sWndTitle = "AnimViewer - FR Engine 2.0";

    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fScale = 1.0f;
    memset( &m_vPos,0,sizeof(FlyVector) );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
AnimViewerApp::~AnimViewerApp(void)
{
}


//------------------------------------------------------------------------
// Initialize the scene content for this application.
//------------------------------------------------------------------------
HRESULT AnimViewerApp::OneTimeSceneInit(void)
{
    FlyKernel& engine = FlyKernel::Instance();

    // Create a new sceneManager.
    FlySceneManager* pManager = engine.CreateSceneManager( "TestAnim","default" );
    if( !pManager )
    {
        OnErrorExit( "Create the sceneManager failed! Application will exit." );
        return FLY_FAIL;
    }

    // Create a new animation entity.
    //FlyAnimEntity* pEntity = pManager->CreateAnimation( "Robot","../../Media/Animations/Youming/Youming.AMD" );
    //FlyAnimEntity* pEntity = pManager->CreateAnimation( "Robot","../../Media/Animations/Cylinder/Cylinder.amd" );
    FlyAnimEntity* pEntity = pManager->CreateAnimation( "Robot","../../Media/Animations/tianhe/tianhe.AMD" );

    // Add the animation node for animation.
    FlyAnimManager* pAnimMgr = engine.GetAnimManager();
    pAnimMgr->GetAnimationByIndex(0)->AddAnimNode( "../../Media/Animations/tianhe/C01.anm" );
    //pAnimMgr->GetAnimationByIndex(0)->AddAnimNode( "../../Media/Animations/Youming/C01.anm" );
    //pAnimMgr->GetAnimationByIndex(0)->AddAnimNode( "../../Media/Animations/Cylinder/C02.anm" );

    // Attach the entity to the node.
    pEntity->SetAnimation( 0 );
    pEntity->SetAnimFPS( 25.0f );
    pEntity->SetAnimLoop( true );
    pEntity->Pause();
    FlySceneNode* pAnim = pManager->GetSceneRoot()->CreateChildNode( "Animation" );
    pAnim->AttachObject( (FlySceneObject*)pEntity );
    pAnim->SetScale( FlyVector(0.5f,0.5f,0.5f) );

    // Build the world axis.
    BuildWorldAxis( pManager );

    // Set the active sceneManager.
    engine.SetActiveSceneManager( pManager );

    // Create a new camera.
    FlyCamManager* pCamMgr = engine.GetCamManager();
    FlyCameraBase* pCamera = pCamMgr->CreateCamera( "DefaultCamera" );
    pManager->SetCurrentCamera( pCamera );
    pCamera->SetPosition( 0.0f,0.0f,-20.0f );

    // Create the font object.
    //m_pFont = engine.GetResourceManager()->MakeSystemFont();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the world axis for the application.
//------------------------------------------------------------------------
HRESULT AnimViewerApp::BuildWorldAxis( FlySceneManager* pManager )
{
    VERTEXL pVerts[94];
    memset( pVerts,0,sizeof(VERTEXL)*94 );

    for( int i=0;i<21;i++ )
    {
        if( i == 10 ) continue;
        int n = i > 10 ? i-1 : i;

        // Fill in the columns.
        pVerts[n*2+0].x = -100.0f + 10.0f * i;
        pVerts[n*2+0].y = 0.0f;
        pVerts[n*2+0].z = -100.0f;
        pVerts[n*2+0].color = 0xffff0000;

        pVerts[n*2+1].x = -100.0f + 10.0f * i;
        pVerts[n*2+1].y = 0.0f;
        pVerts[n*2+1].z = 100.0f;
        pVerts[n*2+1].color = 0xffff0000;

        // Fill in the rows.
        pVerts[40+n*2+0].x = -100.0f;
        pVerts[40+n*2+0].y = 0.0f;
        pVerts[40+n*2+0].z = -100.0f + 10.0f * i;
        pVerts[40+n*2+0].color = 0xffff0000;

        pVerts[40+n*2+1].x = 100.0f;
        pVerts[40+n*2+1].y = 0.0f;
        pVerts[40+n*2+1].z = -100.0f + 10.0f * i;
        pVerts[40+n*2+1].color = 0xffff0000;
    }

    pVerts[80].x = pVerts[82].z = -105.0f;
    pVerts[81].x = pVerts[83].z = 105.0f;
    pVerts[84].x = pVerts[86].x = 110.0f;
    pVerts[85].x = pVerts[87].x = 120.0f;
    pVerts[84].z = pVerts[87].z = 5.0f;
    pVerts[85].z = pVerts[86].z = -5.0f;
    pVerts[88].x = pVerts[91].x = -5.0f;
    pVerts[89].x = pVerts[93].x = 5.0f;
    pVerts[88].z = pVerts[89].z = 120.0f;
    pVerts[91].z = pVerts[93].z = 110.0f;
    pVerts[90] = pVerts[89];
    pVerts[92] = pVerts[91];
    for( int i=80;i<94;i++ ) pVerts[i].color = 0xff00ff00;

    FlyAabb box;
    box.vcMin = FlyVector( -120.0f,-5.0f,-120.0f );
    box.vcMax = FlyVector(  120.0f, 5.0f, 120.0f );

    FlyManualEntity* pEntity = pManager->CreateManualEntity( "WorldAxis" );
    FlyManualSubObj* pSubObj = pEntity->BeginSubObject( VF_DEFAULTNOLIGHT,RT_LINELIST );
    pSubObj->CreateGraphicBuffer( 94,0 );
    pSubObj->AddVertexData( 94,pVerts,box );
    pEntity->EndSubObject( pSubObj );

    pManager->GetSceneRoot()->AttachObject( (FlySceneObject*)pEntity );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Callback function A for the engine.
//------------------------------------------------------------------------
void AnimViewerApp::OnEngineRunningA(void)
{
    FlySceneManager* pManager = FlyKernel::Instance().GetActiveSceneManager();

    FlyQuat qX,qY,quat;
    qX.FromAngleAxis( FlyVector(1.0f,0.0f,0.0f),m_fRotX );
    qY.FromAngleAxis( FlyVector(0.0f,1.0f,0.0f),m_fRotY );
    quat = qX * qY;

    pManager->GetSceneRoot()->SetPosition( m_vPos );
    pManager->GetSceneRoot()->SetOrientation( quat );
    pManager->GetSceneRoot()->SetScale( FlyVector(m_fScale,m_fScale,m_fScale) );
}


//------------------------------------------------------------------------
// Callback function B for the engine.
//------------------------------------------------------------------------
void AnimViewerApp::OnEngineRunningB(void)
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
// Do things when move the mouse.
//------------------------------------------------------------------------
void AnimViewerApp::OnMouseMove( WPARAM wParam,LPARAM lParam )
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
void AnimViewerApp::OnMouseWheel( WPARAM wParam,LPARAM lParam )
{
    short lDelta = (short)HIWORD( wParam );

    if( lDelta > 0 ) m_fScale += 0.05f;
    if( lDelta < 0 ) m_fScale -= 0.05f;

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
LRESULT AnimViewerApp::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
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
        {
            FlyAnimEntity* pAnim = (FlyAnimEntity*)FlyKernel::Instance().GetActiveSceneManager()->GetSceneObject("Robot");
            if( !pAnim ) break;
            float fCurFrame = pAnim->GetCurrentFrame();

            if( wParam == VK_RIGHT )
            {
                fCurFrame += 1.0f;
                if( fCurFrame > (float)(pAnim->GetAnimation()->GetNumAnimFrames(0)-1) )
                    fCurFrame = pAnim->GetAnimation()->GetNumAnimFrames(0) - 1;

                pAnim->SetCurrentFrame( fCurFrame );
            }

            if( wParam == VK_LEFT )
            {
                fCurFrame -= 1.0f;
                if( fCurFrame < 0.0f ) fCurFrame = 0.0f;
                pAnim->SetCurrentFrame( fCurFrame );
            }

            if( wParam == 'P' ) pAnim->Pause();
            if( wParam == 'L' ) pAnim->Play();

            if( wParam == '1' ) pAnim->SetAnimation( 0 );
            if( wParam == '2' ) pAnim->SetAnimation( 1 );

            if( wParam == VK_SPACE ) FlyKernel::Instance().SaveScreenshot();
        }
        break;
    }

    return AppFramework::MsgProc( hWnd,uMsg,wParam,lParam );
}