//------------------------------------------------------------------------
// Name: AppRenderer.cpp
// Desc: This file is to implement the class CAppRenderer.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "AppRenderer.h"


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
CAppRenderer::CAppRenderer(void)
{
    m_hInst     = NULL;
    m_hRender   = NULL;

    m_pDevice   = NULL;
    m_pSkinMgr  = NULL;
    m_pVCMgr    = NULL;
    m_pFI       = NULL;

    m_pTimer    = NULL;
    m_pCamera   = NULL;
    m_pVerts    = NULL;

    m_bPreview  = false;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
CAppRenderer::~CAppRenderer(void)
{
    Release();
}


//------------------------------------------------------------------------
// Release the class, clean all the resources.
//------------------------------------------------------------------------
void CAppRenderer::Release(void)
{
    SAFE_DELETE_ARRAY( m_pVerts );
    SAFE_DELETE( m_pCamera );

    SAFE_DELETE( m_pFI );

    // Shutdown the engine.
    m_pRoot->ShutdownEngine();
}


//------------------------------------------------------------------------
// Create the instance for the renderer.
// ----------------------------------------------------------------------
// Param -> IN:
//      HINSTANCE:  Handle to the application.
//      HWND:       Handle to the main window.
//      HWND:       Handle to the RenderWindow.
//------------------------------------------------------------------------
bool CAppRenderer::Create( HINSTANCE hInst,HWND hWnd,HWND hRender )
{
    m_hInst = hInst;
    m_hWnd  = hWnd;
    m_hRender = hRender;

    // Create a Instance for engine.
    m_pRoot = &(FlyRootManager::GetSingleton());

    // Create the FlyRender engine.
    FLYENGINE_OPTION option;
    option.hDestWindow = hRender;
    option.hWnd3D = NULL;
    option.nMinDepth = 0;
    option.nMinStencil = 0;
    option.nNumWindows = 0;
    m_pRoot->InitEngine( "Direct3D",BSP_SCENE_MANAGER,&option );
    m_pDevice = m_pRoot->GetRenderDevice();
    m_pSkinMgr = m_pDevice->GetSkinManager();
    m_pVCMgr = m_pDevice->GetVCacheManager();
    m_pDevice->SetClearColor( 0.1f,0.1f,0.1f );
    m_pTimer = m_pRoot->GetTimer();

    // Create the camera.
    m_pCamera = new CAppCamera( m_pDevice );

    // Init the FI system.
    if( !InitFIDevice() ) return false;

    // Prepare the axis data.
    PrepareAxis();

    // Initialize the particle system.
    m_System.Init( m_pDevice );
    m_System.AddSceneNode();

    return true;
}


//------------------------------------------------------------------------
// Initialize the FI devices.
//------------------------------------------------------------------------
bool CAppRenderer::InitFIDevice(void)
{
    RECT rcCage = { 0,0,800,600 };
    HINSTANCE hModule = LoadLibrary( "FILib.dll" );
    if( !hModule )
    {
        MessageBox( NULL,"Load \"FILib.dll\" error! FI System Init failed!",
            "Error",MB_OK|MB_ICONERROR );
        return false;
    }
    CREATEINPUTDEVICE pFunc = (CREATEINPUTDEVICE)GetProcAddress( hModule,"CreateInputDevice" );
    if( !pFunc ) return false;
    if( FAILED(pFunc(hModule,&m_pFI)) )
    {
        MessageBox( NULL,"Create the FI device failed!","Error",MB_OK|MB_ICONERROR );
        return false;
    }
    m_pFI->Init( m_hWnd,&rcCage,true );

    return true;
}


//------------------------------------------------------------------------
// Prepare for the axis data.
//------------------------------------------------------------------------
void CAppRenderer::PrepareAxis(void)
{
    m_pVerts = new VERTEX[80];
    if( !m_pVerts ) return;

    for( int i=0;i<10;i++ )
    {
        m_pVerts[i*2+0].x = -100.0f + 10.0f * i;
        m_pVerts[i*2+0].y = 0.0f;
        m_pVerts[i*2+0].z = -100.0f;
        
        m_pVerts[i*2+1].x = -100.0f + 10.0f * i;
        m_pVerts[i*2+1].y = 0.0f;
        m_pVerts[i*2+1].z = 100.0f;
    }
    
    for( i=0;i<10;i++ )
    {
        m_pVerts[20+i*2+0].x = 10.0f + 10.0f * i;
        m_pVerts[20+i*2+0].y = 0.0f;
        m_pVerts[20+i*2+0].z = -100.0f;
        
        m_pVerts[20+i*2+1].x = 10.0f + 10.0f * i;
        m_pVerts[20+i*2+1].y = 0.0f;
        m_pVerts[20+i*2+1].z = 100.0f;
    }

    for( i=0;i<10;i++ )
    {
        m_pVerts[40+i*2+0].x = -100.0f;
        m_pVerts[40+i*2+0].y = 0.0f;
        m_pVerts[40+i*2+0].z = -100.0f + 10.0f * i;
        
        m_pVerts[40+i*2+1].x = 100.0f;
        m_pVerts[40+i*2+1].y = 0.0f;
        m_pVerts[40+i*2+1].z = -100.0f + 10.0f * i;
    }

    for( i=0;i<10;i++ )
    {
        m_pVerts[60+i*2+0].x = -100.0f;
        m_pVerts[60+i*2+0].y = 0.0f;
        m_pVerts[60+i*2+0].z = 10.0f + 10.0f * i;

        m_pVerts[60+i*2+1].x = 100.0f;
        m_pVerts[60+i*2+1].y = 0.0f;
        m_pVerts[60+i*2+1].z = 10.0f + 10.0f * i;
    }
}


//------------------------------------------------------------------------
// Update the application's Renderable objects.
//------------------------------------------------------------------------
void CAppRenderer::Update(void)
{
    m_pTimer->Update();
    m_fElapsed = m_pTimer->GetElapsed();
    m_fFPS = m_pTimer->GetFPS();

    m_pCamera->Update();

    // Handle the input events.
    HandleInputEvents();

    if( m_bPreview ) m_System.Update( m_fElapsed );
}


//------------------------------------------------------------------------
// Render all the Renderable objects for the application.
//------------------------------------------------------------------------
void CAppRenderer::Render(void)
{
    m_pDevice->BeginRendering( true,true,true );

    // Render the axis.
    RenderAxis();

    if( m_bPreview ) m_System.Render();

    m_pDevice->EndRendering();
}


//------------------------------------------------------------------------
// Render the axis for the particle editor.
//------------------------------------------------------------------------
void CAppRenderer::RenderAxis(void)
{
    VERTEX pVertsAxis[4];
    FLYCOLOR clr = { 1.0f,0.0f,0.0f,1.0f };
    memset( pVertsAxis,0,sizeof(VERTEX)*4 );
    pVertsAxis[0].x = -105.0f;
    pVertsAxis[1].x = 105.0f;
    pVertsAxis[2].z = -105.0f;
    pVertsAxis[3].z = 105.0f;

    m_pVCMgr->RenderLines( VID_UU,80,m_pVerts,&clr,false );
    memset( &clr,0,sizeof(FLYCOLOR) ); clr.fG = clr.fA = 1.0f;
    m_pVCMgr->RenderLines( VID_UU,4,pVertsAxis,&clr,false );

    // Draw the 'X' and 'Z'.
    memset( pVertsAxis,0,sizeof(VERTEX)*4 );
    pVertsAxis[0].x = 110.0f; pVertsAxis[0].z = 5.0f;
    pVertsAxis[1].x = 120.0f; pVertsAxis[1].z = -5.0f;
    pVertsAxis[2].x = 110.0f; pVertsAxis[2].z = -5.0f;
    pVertsAxis[3].x = 120.0f; pVertsAxis[3].z = 5.0f;
    m_pVCMgr->RenderLines( VID_UU,4,pVertsAxis,&clr,false );
    pVertsAxis[0].x = -5.0f; pVertsAxis[0].z = 120.0f;
    pVertsAxis[1].x = 5.0f; pVertsAxis[1].z = 120.0f;
    pVertsAxis[2].x = -5.0f; pVertsAxis[2].z = 110.0f;
    pVertsAxis[3].x = 5.0f; pVertsAxis[3].z = 110.0f;
    m_pVCMgr->RenderLines( VID_UU,4,pVertsAxis,&clr,true );
}


//------------------------------------------------------------------------
// Handle the input events for the editor.
//------------------------------------------------------------------------
void CAppRenderer::HandleInputEvents(void)
{
    m_pFI->Update();

    if( m_pFI->IsPressed(IDV_KEYBOARD,FIK_W) )
        m_pCamera->MoveAlongZ( 0.5f );
    if( m_pFI->IsPressed(IDV_KEYBOARD,FIK_S) )
        m_pCamera->MoveAlongZ( -0.5f );
    if( m_pFI->IsPressed(IDV_KEYBOARD,FIK_A) )
        m_pCamera->MoveAlongX( -0.5f );
    if( m_pFI->IsPressed(IDV_KEYBOARD,FIK_D) )
        m_pCamera->MoveAlongX( 0.5f );
}


//------------------------------------------------------------------------
// Preview the particle scene.
//------------------------------------------------------------------------
void CAppRenderer::Preview( bool bPre )
{
    if( !bPre ) m_System.GetSceneNode(0)->Reset();

    m_System.GetSceneNode(0)->Restart();
    m_bPreview = bPre;
}