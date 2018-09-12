//------------------------------------------------------------------------
// Name: TestShader.cpp
// Desc: This file is to implement the class TestShaderApp.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#define _WIN32_WINNT 0x0400

#include "TestShader.h"


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
TestShaderApp::TestShaderApp(void)
{
    m_sWndTitle = "Test Shader effect - FR Engine 2.0";

    m_pEffect = NULL;
    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fScale = 1.0f;
    memset( &m_vPos,0,sizeof(FlyVector) );
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
TestShaderApp::~TestShaderApp(void)
{
}


//------------------------------------------------------------------------
// Initialize the scene content for application.
//------------------------------------------------------------------------
HRESULT TestShaderApp::OneTimeSceneInit(void)
{
    FlyKernel& engine = FlyKernel::Instance();

    // Create a sceneManager.
    FlySceneManager* pManager = engine.CreateSceneManager( "TestShader","default" );
    if( !pManager )
    {
        OnErrorExit( "Create the sceneManager failed! The application will exit!" );
        return FLY_FAIL;
    }

    // Add a user-defined vertex type.
    FlyVertexType vertexType;
    vertexType.AddElement( ET_FLOAT2,EU_POSITION );
    m_pRenderer->RegisterVertexType( vertexType );

    // Create a manual entity.
    FlyManualEntity* pEntity = pManager->CreateManualEntity( "Wave" );
    FlyManualSubObj* pSubObj = pEntity->BeginSubObject( VF_TWOTEXTURE,RT_TRIANGLELIST );

    // Create a static buffer to render.
    int i,j;

    UINT nNumIndis = 63 * 63 * 6;
    UINT nNumVerts = 64 * 64;

//     float2* pVerts = new float2[nNumVerts];
//     for( j=0;j<64;j++ )
//     {
//         for( i=0;i<64;i++ )
//         {
//             pVerts[j*64+i].x = ((float)i / (float)(64-1) - 0.5f) * FLY_PI;
//             pVerts[j*64+i].y = ((float)j / (float)(64-1) - 0.5f) * FLY_PI;
//         }
//     }
    VERTEX2T* pVerts = new VERTEX2T[nNumVerts];
    for( j=0;j<64;j++ )
    {
        for( i=0;i<64;i++ )
        {
            pVerts[j*64+i].x = ((float)i / (float)(64-1) - 0.5f) * FLY_PI;
            pVerts[j*64+i].y = 0.0f;
            pVerts[j*64+i].z = ((float)j / (float)(64-1) - 0.5f) * FLY_PI;
            pVerts[j*64+i].vN[0] = 0.0f;
            pVerts[j*64+i].vN[1] = 0.0f;
            pVerts[j*64+i].vN[2] = 0.0f;
            pVerts[j*64+i].tu0 = (float)i / 63.0f;
            pVerts[j*64+i].tv0 = (float)j / 63.0f;
            pVerts[j*64+i].tu1 = (float)i / 21.0f;
            pVerts[j*64+i].tv1 = (float)j / 21.0f;
        }
    }

    WORD* pIndices = new WORD[nNumIndis];
    WORD* p = pIndices;
    for( j=1;j<64;j++ )
    {
        for( i=1;i<64;i++ )
        {
            *p++ = (WORD)( (j-1)*64 + (i-1) );
            *p++ = (WORD)( (j-0)*64 + (i-1) );
            *p++ = (WORD)( (j-1)*64 + (i-0) );
            *p++ = (WORD)( (j-1)*64 + (i-0) );
            *p++ = (WORD)( (j-0)*64 + (i-1) );
            *p++ = (WORD)( (j-0)*64 + (i-0) );
        }
    }

    pSubObj->CreateGraphicBuffer( 64*64,63*63*6 );
    pSubObj->AddVertexData( 64*64,pVerts,FlyAabb(FlyVector(-1.57f,-1.0f,-1.57f),
        FlyVector(1.57f,1.0f,1.57f)) );
    pSubObj->AddIndexData( 63*63*6,pIndices );

    // Load the render effect.
    m_pEffect = m_pResMgr->LoadEffectSource( "../../Media/Shaders/asdf.xml" );
    if( !m_pEffect )
    {
        OnErrorExit( "Can't load effect \"../../Media/Shaders/asdf.fx\"!" );
        return FLY_FAIL;
    }

    // Construct the material.
    m_pMaterial = pEntity->AddMaterial( "TestEffect" );
    m_pMaterial->AddNewTechnique( m_pEffect->GetTechniqueByName("OneTexture") );
    m_pMaterial->SetActiveTechnique( 0 );

    pSubObj->SetRenderMaterial( m_pMaterial );
    pEntity->EndSubObject( pSubObj );

    pManager->GetSceneRoot()->AttachObject( (FlySceneObject*)pEntity );

    // Setup the camera.
    FlyCamManager* pCamMgr = engine.GetCamManager();
    FlyCameraBase* pCamera = pCamMgr->CreateCamera( "DefaultCamera" );
    pManager->SetCurrentCamera( pCamera );
    pCamera->SetPosition( 0.0f,2.0f,-3.0f );
    pCamera->LookAt( 0.0f,0.0f,0.0f );

    engine.SetActiveSceneManager( pManager );

    //m_pFont = engine.GetResourceManager()->MakeSystemFont();

    m_pTexture1 = engine.GetResourceManager()->Load2DTexture( "../../Media/Shaders/m02_1_58.bmp",0 );
    m_pTexture2 = engine.GetResourceManager()->Load2DTexture( "../../Media/Shaders/ml2-04.bmp",0 );

    *m_pMaterial->GetRenderEffectInstance(0)->GetParameterByIndex(2) = m_pTexture1;
    *m_pMaterial->GetRenderEffectInstance(0)->GetParameterByIndex(3) = m_pTexture2;

    // Clear the temp buffer.
    delete[] pVerts;
    delete[] pIndices;

    //////////////////////////////////////////////////////////////////////

    VERTEXL pVerts2[94];
    memset( pVerts2,0,sizeof(VERTEXL)*94 );

    for( i=0;i<21;i++ )
    {
        if( i == 10 ) continue;
        int n = i > 10 ? i-1 : i;

        // Fill in the columns.
        pVerts2[n*2+0].x = -100.0f + 10.0f * i;
        pVerts2[n*2+0].y = 0.0f;
        pVerts2[n*2+0].z = -100.0f;
        pVerts2[n*2+0].color = 0xffff0000;

        pVerts2[n*2+1].x = -100.0f + 10.0f * i;
        pVerts2[n*2+1].y = 0.0f;
        pVerts2[n*2+1].z = 100.0f;
        pVerts2[n*2+1].color = 0xffff0000;

        // Fill in the rows.
        pVerts2[40+n*2+0].x = -100.0f;
        pVerts2[40+n*2+0].y = 0.0f;
        pVerts2[40+n*2+0].z = -100.0f + 10.0f * i;
        pVerts2[40+n*2+0].color = 0xffff0000;

        pVerts2[40+n*2+1].x = 100.0f;
        pVerts2[40+n*2+1].y = 0.0f;
        pVerts2[40+n*2+1].z = -100.0f + 10.0f * i;
        pVerts2[40+n*2+1].color = 0xffff0000;
    }

    pVerts2[80].x = pVerts2[82].z = -105.0f;
    pVerts2[81].x = pVerts2[83].z = 105.0f;
    pVerts2[84].x = pVerts2[86].x = 110.0f;
    pVerts2[85].x = pVerts2[87].x = 120.0f;
    pVerts2[84].z = pVerts2[87].z = 5.0f;
    pVerts2[85].z = pVerts2[86].z = -5.0f;
    pVerts2[88].x = pVerts2[91].x = -5.0f;
    pVerts2[89].x = pVerts2[93].x = 5.0f;
    pVerts2[88].z = pVerts2[89].z = 120.0f;
    pVerts2[91].z = pVerts2[93].z = 110.0f;
    pVerts2[90] = pVerts2[89];
    pVerts2[92] = pVerts2[91];
    for( i=80;i<94;i++ ) pVerts2[i].color = 0xff00ff00;

    FlyAabb box;
    box.vcMin = FlyVector( -120.0f,-5.0f,-120.0f );
    box.vcMax = FlyVector(  120.0f, 5.0f, 120.0f );

    pEntity = pManager->CreateManualEntity( "WorldAxis" );
    pSubObj = pEntity->BeginSubObject( VF_DEFAULTNOLIGHT,RT_LINELIST );
    pSubObj->CreateGraphicBuffer( 94,0 );
    pSubObj->AddVertexData( 94,pVerts2,box );
    pEntity->EndSubObject( pSubObj );

    pManager->GetSceneRoot()->AttachObject( (FlySceneObject*)pEntity );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Update the frame's content.
//------------------------------------------------------------------------
void TestShaderApp::OnEngineRunningA(void)
{
    FlySceneManager* pManager = FlyKernel::Instance().GetActiveSceneManager();

    FlyQuat quatX,quatY;
    quatX.FromAngleAxis( FlyVector(1.0f,0.0f,0.0f),m_fRotX );
    quatY.FromAngleAxis( FlyVector(0.0f,1.0f,0.0f),m_fRotY );

    pManager->GetSceneRoot()->SetPosition( m_vPos );
    pManager->GetSceneRoot()->SetOrientation( quatX*quatY );
    pManager->GetSceneRoot()->SetScale( FlyVector(m_fScale,m_fScale,m_fScale) );

    *m_pMaterial->GetRenderEffectInstance(0)->GetParameterByIndex(1) = sinf((float)timeGetTime()/1000.0f);
}


//------------------------------------------------------------------------
// CALLBACK Function called after rendering.
//------------------------------------------------------------------------
void TestShaderApp::OnEngineRunningB(void)
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
void TestShaderApp::OnMouseMove( WPARAM wParam,LPARAM lParam )
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
        if( lDiffX > 0 ) m_vPos.x += 0.1f;
        if( lDiffX < 0 ) m_vPos.x -= 0.1f;
        if( lDiffY > 0 ) m_vPos.y -= 0.1f;
        if( lDiffY < 0 ) m_vPos.y += 0.1f;
    }
}


//------------------------------------------------------------------------
// Do things when the mouse wheel is rolled
//------------------------------------------------------------------------
void TestShaderApp::OnMouseWheel( WPARAM wParam,LPARAM lParam )
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
LRESULT TestShaderApp::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
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
        if( wParam == VK_SPACE ) FlyKernel::Instance().SaveScreenshot();
        break;
    }

    return AppFramework::MsgProc( hWnd,uMsg,wParam,lParam );
}