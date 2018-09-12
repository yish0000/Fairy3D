//------------------------------------------------------------------------
// Name: etCoreManager.cpp
// Desc: This file is to implement the class etCoreManager.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#define _WIN32_WINNT 0x0400

#include "etCoreManager.h"

#include "etSceneManager.h"
#include "etEntityManager.h"
#include "etBrush.h"
#include "etSelectionBuffer.h"
#include "etSpriteObject.h"
#include "etVegetationObject.h"
#include "etLiquidObject.h"

#include "FlyCamManager.h"


extern HINSTANCE g_hInst;

etCoreManager* g_pCore;

//------------------------------------------------------------------------
// Callback function called by engine before rendering scene data.
//------------------------------------------------------------------------
void CALLBACK EngineCallbackFunA(void)
{
    g_pCore->OnEngineRunningA();
}

//------------------------------------------------------------------------
// Callback function called by engine after rendering scene data.
//------------------------------------------------------------------------
void CALLBACK EngineCallbackFunB(void)
{
    g_pCore->OnEngineRunningB();
}


//------------------------------------------------------------------------
// Get the singleton instance of the object.
//------------------------------------------------------------------------
etCoreManager& etCoreManager::Instance(void)
{
    static etCoreManager core;
    return core;
}


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etCoreManager::etCoreManager(void)
{
    m_hInst = NULL;
    m_hMainWnd = NULL;
    m_hRenderWnd = NULL;
    m_bActive = false;
    m_dwLastActive = 0;

    m_bHaveProject = false;
    memset( m_bKeystate,0,64 );
    m_bMouseState[0] = false;
    m_bMouseState[1] = false;
    m_bMouseState[2] = false;
    m_CamMode = CM_EDITCAMERA;
    m_WorkMode = WKM_TERRAIN;

    m_pTimer = NULL;
    m_pDevice = NULL;
    m_pResMgr = NULL;
    m_pRenderer = NULL;

    m_pFactory = NULL;
    m_pSceneMgr = NULL;
    m_pEntityMgr = NULL;
    m_pSelBuf = NULL;
    m_pBrush = NULL;
    m_pEditCamera = NULL;
    m_pRambleCamera = NULL;

    m_pTerrainFX = NULL;
    m_pVegetationFX = NULL;
    m_pLiquidFX = NULL;

    g_pCore = this;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etCoreManager::~etCoreManager(void)
{
}


//------------------------------------------------------------------------
// Initialize the core module of the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      HINSTANCE:  Handle to the application's instance.
//      HWND:       Handle to the main window.
//      HWND:       Handle to the render window.
//------------------------------------------------------------------------
HRESULT etCoreManager::Initialize( HINSTANCE hInst,HWND hMainWnd,HWND hRenderWnd )
{
    HRESULT hr;

    // Save the handles.
    m_hInst = hInst;
    m_hMainWnd = hMainWnd;
    m_hRenderWnd = hRenderWnd;

    // Initialize the engine kernel object.
    FlyKernel& engine = FlyKernel::Instance();
    hr = engine.Initialize( "Direct3D",hRenderWnd,NULL,0,false );
    if( FAILED(hr) ) return ET_CREATECORE;

    m_pDevice = engine.GetRenderDevice();
    m_pRenderer = engine.GetRenderer();
    m_pResMgr = engine.GetResourceManager();

    // Get the timer of the engine.
    m_pTimer = engine.GetTimer();

    engine.SetCallbackFuncA( EngineCallbackFunA );
    engine.SetCallbackFuncB( EngineCallbackFunB );

    return OneTimeSceneInit();
}


//------------------------------------------------------------------------
// Initialize the objects used for rendering the scene.
//------------------------------------------------------------------------
HRESULT etCoreManager::OneTimeSceneInit(void)
{
    FlyKernel& engine = FlyKernel::Instance();

    // Load the config data.
    if( FAILED(m_Config.LoadConfig("editor_config.xml")) )
        m_Config.SaveConfig( "editor_config.xml" );

    m_pDevice->SetViewPlanes( 0,m_Config.GetNearDistance(),m_Config.GetFarDistance() );
    FLYCOLOR clr = m_Config.GetBackColor();
    m_pDevice->SetClearColor( clr.fR,clr.fG,clr.fB );

    // Register the etSceneManager.
    m_pFactory = new etSceneFactory();
    engine.RegisterSceneType( m_pFactory );

    // Register the sprite factory.
    engine.RegisterEntityType( new etSpriteFactory() );
    engine.RegisterEntityType( new etVegetationFactory() );
    engine.RegisterEntityType( new etLiquidFactory() );

    // Build the renderEffects.
    BuildRenderEffects();

    // Create an editable terrain sceneManager.
    m_pSceneMgr = (etSceneManager*)engine.CreateSceneManager( "et-SceneManager","et_scene" );
    if( !m_pSceneMgr ) return ET_CREATESCENEMANAGER;

    // Create cameras for editor.
    FlyCamManager* pCamMgr = engine.GetCamManager();
    m_pEditCamera = pCamMgr->CreateCamera( "edit-Camera" );
    m_pEditCamera->SetYawFixed( true );
    m_pRambleCamera = pCamMgr->CreateCamera( "ramble-Camera" );
    m_pRambleCamera->SetYawFixed( true );

    // Set the camera mode.
    m_pSceneMgr->SetCurrentCamera( m_pEditCamera );

    //////////////////////////////////////////////////////////////////////////

    m_pEditCamera->SetPosition( -20.0f,25.0f,-20.0f );
    m_pEditCamera->Yaw( FLY_PI/4.0f );
    m_pEditCamera->Pitch( 0.3f );
    m_pRambleCamera->SetPosition( 0.0f,5.0f,0.0f );
    m_pRambleCamera->Yaw( FLY_PI/4.0f );
    m_pSceneMgr->CreateNewScene( 512,64,256 );
    m_bHaveProject = true;

    //////////////////////////////////////////////////////////////////////////

    // Create the entityManager.
    m_pEntityMgr = new etEntityManager();
    m_pSceneMgr->GetSceneRoot()->CreateChildNode( "et-Entity" );

    // Create the brush for editor.
    m_pBrush = new etBrush();

    // Create the selectionBuffer.
    m_pSelBuf = new etSelectionBuffer();

    FlySceneNode* pUtilNode = m_pSceneMgr->GetSceneRoot()->CreateChildNode( "et-Util" );
    pUtilNode->AttachObject( m_pBrush );
    pUtilNode->AttachObject( m_pSelBuf );

    m_pBrush->AttachTerrain( m_pSceneMgr->GetTerrainLevel() );
    engine.SetActiveSceneManager( m_pSceneMgr );

//     etLiquidObject* pWater = (etLiquidObject*)m_pSceneMgr->CreateSceneObject( "Water01","liquid" );
//     pWater->Initialize( "Texture\\Water\\water.bmp","Texture\\Water\\distort.bmp",
//         FlyVector(0.0f,5.0f,0.0f),256,256 );
//     FlySceneNode* pNode = m_pSceneMgr->GetSceneRoot()->CreateChildNode( "WaterNode" );
//     //pNode->SetPosition( FlyVector(128.0f,0.0f,128.0f) );
//     pNode->AttachObject( pWater );
//     pWater->SetReflection( true );
//     pWater->SetReflectSky( true );

    // Activate the engine.
    m_bActive = true;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the renderEffects for the editor.
//------------------------------------------------------------------------
HRESULT etCoreManager::BuildRenderEffects(void)
{
    // 1. Load the terrain renderEffect.
    m_pTerrainFX = m_pResMgr->GetEffectByName( "et-TerrainFX" );
    if( !m_pTerrainFX )
    {
        HRSRC hResource = FindResource( g_hInst,(LPCTSTR)IDR_ET_TERRAIN,
            "RenderEffectCode" );
        m_pTerrainFX = m_pResMgr->AddNullEffect( "et-RenderEffect" );

        UINT nStrLen = SizeofResource( g_hInst,hResource );
        char* cTextBuffer = new char[nStrLen];
        memcpy( cTextBuffer,LockResource(LoadResource(g_hInst,hResource)),nStrLen );
        m_pTerrainFX->Compile( cTextBuffer );
        delete[] cTextBuffer;
    }

    // 2. Load the vegetation renderEffect.
    m_pVegetationFX = m_pResMgr->GetEffectByName( "et-VegetationFX" );
    if( !m_pVegetationFX )
    {
        HRSRC hResource = FindResource( g_hInst,(LPCTSTR)IDR_ET_VEGETATION,
            "RenderEffectCode" );
        m_pVegetationFX = m_pResMgr->AddNullEffect( "et-VegetationFX" );

        UINT nStrLen = SizeofResource( g_hInst,hResource );
        char* cTextBuffer = new char[nStrLen];
        memcpy( cTextBuffer,LockResource(LoadResource(g_hInst,hResource)),nStrLen );
        m_pVegetationFX->Compile( cTextBuffer );
        delete[] cTextBuffer;
    }

    // 3. Load the liquid renderEffect.
    m_pLiquidFX = m_pResMgr->GetEffectByName( "et-LiquidFX" );
    if( !m_pLiquidFX )
    {
        HRSRC hResource = FindResource( g_hInst,(LPCTSTR)IDR_ET_LIQUID,
            "RenderEffectCode" );
        m_pLiquidFX = m_pResMgr->AddNullEffect( "et-LiquidFX" );

        UINT nStrLen = SizeofResource( g_hInst,hResource );
        char* cTextBuffer = new char[nStrLen];
        memcpy( cTextBuffer,LockResource(LoadResource(g_hInst,hResource)),nStrLen );
        m_pLiquidFX->Compile( cTextBuffer );
        delete[] cTextBuffer;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Activate the application.
//------------------------------------------------------------------------
void etCoreManager::Activate(void)
{
    m_dwLastActive = timeGetTime();
    m_bActive = true;
}


//------------------------------------------------------------------------
// Call this function at each frame.
//------------------------------------------------------------------------
void etCoreManager::EngineMainLoop(void)
{
    FlyKernel::Instance().EngineMainLoop();

    DWORD dwCurTime = timeGetTime();
    if( dwCurTime - m_dwLastActive >= 1000 )
    {
        m_fFPS = 0.0f;
        m_bActive = false;
    }
}


//------------------------------------------------------------------------
// Reset the camera for the editor.
//------------------------------------------------------------------------
void etCoreManager::ResetCamera(void)
{
    if( m_CamMode == CM_EDITCAMERA )
    {
        m_pEditCamera->SetPosition( -20.0f,25.0f,-20.0f );
        m_pEditCamera->SetRotation( FlyQuat::IDENTITY );
        m_pEditCamera->Yaw( FLY_PI/4.0f );
        m_pEditCamera->Pitch( 0.3f );
    }
    else
    {
        m_pRambleCamera->SetPosition( 0.0f,5.0f,0.0f );
        m_pRambleCamera->SetRotation( FlyQuat::IDENTITY );
        m_pRambleCamera->Yaw( FLY_PI/4.0f );
    }
}


//------------------------------------------------------------------------
// Change the camera mode for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      CAMERAMODE:     New camera mode for the editor.
//------------------------------------------------------------------------
void etCoreManager::SetCameraMode( CAMERAMODE mode )
{
    m_CamMode = mode;

    if( m_CamMode == CM_EDITCAMERA )
        m_pSceneMgr->SetCurrentCamera( m_pEditCamera );
    else
        m_pSceneMgr->SetCurrentCamera( m_pRambleCamera );
}


//------------------------------------------------------------------------
// Get the fog states of the editor.
//------------------------------------------------------------------------
void etCoreManager::GetFog( FOGMODE* pMode,FLYCOLOR* pColor,float* fDensity,
                            float* fNear,float* fFar )
{
    *pMode = m_pSceneMgr->GetFogMode();
    *pColor = m_pSceneMgr->GetFogColor();
    *fDensity = m_pSceneMgr->GetFogDensity();
    *fNear = m_pSceneMgr->GetFogNear();
    *fFar = m_pSceneMgr->GetFogFar();
}


//------------------------------------------------------------------------
// Set the fog states for the editor.
//------------------------------------------------------------------------
void etCoreManager::SetFog( FOGMODE mode,const FLYCOLOR& color,float fDensity,
                            float fNear,float fFar )
{
    m_pSceneMgr->SetFogMode( mode );
    m_pSceneMgr->SetFogColor( color );
    m_pSceneMgr->SetFogDensity( fDensity );
    m_pSceneMgr->SetFogNear( fNear );
    m_pSceneMgr->SetFogFar( fFar );
}


//------------------------------------------------------------------------
// Do we show skybox for the editor ?
//------------------------------------------------------------------------
bool etCoreManager::IsShowSkybox(void) const
{
    return m_pSceneMgr->IsShowSkybox();
}

//------------------------------------------------------------------------
// Set the skybox for the editor.
//------------------------------------------------------------------------
void etCoreManager::SetSkyboxByName( bool bEnable,const char* cSkyName )
{
    for( UINT n=0;n<m_Config.GetNumSkybox();n++ )
    {
        if( strcmp(m_Config.GetSkyboxName(n),cSkyName) == 0 )
        {
            m_pSceneMgr->SetSkybox( bEnable,1000.0f,m_Config.GetSkyboxFront(n),
                m_Config.GetSkyboxBack(n),m_Config.GetSkyboxLeft(n),
                m_Config.GetSkyboxRight(n),m_Config.GetSkyboxTop(n),
                m_Config.GetSkyboxBottom(n) );
            m_nCurrentSkybox = n;
            break;
        }
    }
}

//------------------------------------------------------------------------
// Set the skybox by its index.
//------------------------------------------------------------------------
void etCoreManager::SetSkyboxByIndex( bool bEnable,UINT nIndex )
{
    if( nIndex >= m_Config.GetNumSkybox() )
        return;

    m_pSceneMgr->SetSkybox( bEnable,1000.0f,m_Config.GetSkyboxFront(nIndex),
        m_Config.GetSkyboxBack(nIndex),m_Config.GetSkyboxLeft(nIndex),
        m_Config.GetSkyboxRight(nIndex),m_Config.GetSkyboxTop(nIndex),
        m_Config.GetSkyboxBottom(nIndex) );
    m_nCurrentSkybox = nIndex;
}


//------------------------------------------------------------------------
// Set the clip planes for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Distance of the near clip plane.
//      float:      Distance of the far clip plane.
//------------------------------------------------------------------------
void etCoreManager::SetViewPlanes( float fNear,float fFar )
{
    m_pDevice->SetViewPlanes( 0,fNear,fFar );
}

//------------------------------------------------------------------------
// Set the backcolor of the renderWindow.
//------------------------------------------------------------------------
void etCoreManager::SetBackColor( const FLYCOLOR& color )
{
    m_pDevice->SetClearColor( color.fR,color.fG,color.fB );
}


//------------------------------------------------------------------------
// Update the camera' state for the editor.
//------------------------------------------------------------------------
void etCoreManager::UpdateCameraState(void)
{
    float fMoveSpeed = m_Config.GetCamMoveSpeed();
    float fRotateSpeed = m_Config.GetCamRotateSpeed();

    if( m_CamMode == CM_EDITCAMERA )
    {
        if( m_bKeystate[ETKEY_W] || m_bKeystate[ETKEY_UP] )
        {
            if( m_bKeystate[ETKEY_SHIFT] )
                m_pEditCamera->MoveRelativeZ( fMoveSpeed*m_fElapsed*10.0f );
            else
                m_pEditCamera->MoveRelativeZ( fMoveSpeed*m_fElapsed );
        }

        if( m_bKeystate[ETKEY_A] || m_bKeystate[ETKEY_LEFT] )
        {
            if( m_bKeystate[ETKEY_SHIFT] )
                m_pEditCamera->MoveRelativeX( -fMoveSpeed*m_fElapsed*10.0f );
            else
                m_pEditCamera->MoveRelativeX( -fMoveSpeed*m_fElapsed );
        }

        if( m_bKeystate[ETKEY_S] || m_bKeystate[ETKEY_DOWN] )
        {
            if( m_bKeystate[ETKEY_SHIFT] )
                m_pEditCamera->MoveRelativeZ( -fMoveSpeed*m_fElapsed*10.0f );
            else
                m_pEditCamera->MoveRelativeZ( -fMoveSpeed*m_fElapsed );
        }

        if( m_bKeystate[ETKEY_D] || m_bKeystate[ETKEY_RIGHT] )
        {
            if( m_bKeystate[ETKEY_SHIFT] )
                m_pEditCamera->MoveRelativeX( fMoveSpeed*m_fElapsed*10.0f );
            else
                m_pEditCamera->MoveRelativeX( fMoveSpeed*m_fElapsed );
        }
    }
    else
    {
        if( m_bKeystate[ETKEY_W] || m_bKeystate[ETKEY_UP] )
        {
            if( m_bKeystate[ETKEY_SHIFT] )
                m_pRambleCamera->MoveRelativeZ( fMoveSpeed*m_fElapsed*10.0f );
            else
                m_pRambleCamera->MoveRelativeZ( fMoveSpeed*m_fElapsed );
        }

        if( m_bKeystate[ETKEY_S] || m_bKeystate[ETKEY_DOWN] )
        {
            if( m_bKeystate[ETKEY_SHIFT] )
                m_pRambleCamera->MoveRelativeZ( -fMoveSpeed*m_fElapsed*10.0f );
            else
                m_pRambleCamera->MoveRelativeZ( -fMoveSpeed*m_fElapsed );
        }

        if( m_bKeystate[ETKEY_A] || m_bKeystate[ETKEY_LEFT] )
            m_pRambleCamera->Yaw( -fRotateSpeed*m_fElapsed );

        if( m_bKeystate[ETKEY_D] || m_bKeystate[ETKEY_RIGHT] )
            m_pRambleCamera->Yaw( fRotateSpeed*m_fElapsed );
    }
}


//------------------------------------------------------------------------
// Handle the window message, WM_KEYDOWN.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Is the key down or up ?
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etCoreManager::OnKeyPress( bool bDown,WPARAM wParam,LPARAM lParam )
{
    FlyPlane plane;
    plane.m_vcN.Set( 0.0f,1.0f,0.0f );
    plane.m_fD = 0.0f;

    if( bDown )
    {
        switch( wParam )
        {
        case VK_UP:
            m_bKeystate[ETKEY_UP] = true;
            break;
        case VK_RIGHT:
            m_bKeystate[ETKEY_RIGHT] = true;
            break;
        case VK_DOWN:
            m_bKeystate[ETKEY_DOWN] = true;
            break;
        case VK_LEFT:
            m_bKeystate[ETKEY_LEFT] = true;
            break;
        case 'W':
            m_bKeystate[ETKEY_W] = true;
            break;
        case 'S':
            m_bKeystate[ETKEY_S] = true;
            break;
        case 'A':
            m_bKeystate[ETKEY_A] = true;
            break;
        case 'D':
            m_bKeystate[ETKEY_D] = true;
            break;
        case 'Q':
            m_bKeystate[ETKEY_Q] = true;
            break;
        case 'E':
            m_bKeystate[ETKEY_E] = true;
            break;
        case VK_SHIFT:
            m_bKeystate[ETKEY_SHIFT] = true;
            break;
        }
    }
    else
    {
        switch( wParam )
        {
        case VK_UP:
            m_bKeystate[ETKEY_UP] = false;
            break;
        case VK_RIGHT:
            m_bKeystate[ETKEY_RIGHT] = false;
            break;
        case VK_DOWN:
            m_bKeystate[ETKEY_DOWN] = false;
            break;
        case VK_LEFT:
            m_bKeystate[ETKEY_LEFT] = false;
            break;
        case 'W':
            m_bKeystate[ETKEY_W] = false;
            break;
        case 'S':
            m_bKeystate[ETKEY_S] = false;
            break;
        case 'A':
            m_bKeystate[ETKEY_A] = false;
            break;
        case 'D':
            m_bKeystate[ETKEY_D] = false;
            break;
        case 'Q':
            m_bKeystate[ETKEY_Q] = false;
            break;
        case 'E':
            m_bKeystate[ETKEY_E] = false;
            break;
        case VK_SHIFT:
            m_bKeystate[ETKEY_SHIFT] = false;
            break;
        }
    }
}


//------------------------------------------------------------------------
// Handle the window message, Mouse click.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Is the button down or up ?
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etCoreManager::OnMouseClick( bool bDown,WPARAM wParam,LPARAM lParam )
{
    RECT rc;
    POINT curPos;

    GetCursorPos( &curPos );
    ScreenToClient( m_hRenderWnd,&curPos );
    GetClientRect( m_hRenderWnd,&rc );

    // If cursor are not in RenderView exit function.
    if( curPos.x < 0 || curPos.x > rc.right ||
        curPos.y < 0 || curPos.y > rc.bottom )
        return;

    // 1. Texture panel.
    if( m_WorkMode == WKM_TEXTURE && m_CamMode == CM_EDITCAMERA )
    {
        float fX = m_pBrush->GetPositionX();
        float fZ = m_pBrush->GetPositionZ();

        m_pSceneMgr->GetTerrainLevel()->SelectTile( fX,fZ );
    }

    // 2. Entity panel.
    if( m_WorkMode == WKM_ENTITY && m_CamMode == CM_EDITCAMERA )
    {
        FlyRay ray;
        m_pDevice->Transfer2DTo3D( curPos,&ray.m_vcOrig,&ray.m_vcDir );

        if( m_pSelBuf->GetToolType() == ETT_SELECT )
        {
            if( !(wParam & MK_CONTROL) )
                m_pSelBuf->Clear();

            if( bDown )
                m_pSelBuf->BeginDragRect( curPos );
            else
                m_pSelBuf->EndDragRect();

            m_pEntityMgr->SelectPicked( ray,m_pSelBuf );
        }

        m_pEntityMgr->SetCurrentType( ENTITY_UNKNOWN );
        m_pEntityMgr->SetCurrentIndex( -1 );
    }
}


//------------------------------------------------------------------------
// Handle the window message. WM_MOUSEMOVE.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etCoreManager::OnMouseMove( WPARAM wParam,LPARAM lParam )
{
    RECT rc;
    POINT pt,curPos;
    LONG nDiffX,nDiffY;
    FlyRay ray;

    static POINT s_OldPt = { LOWORD(lParam),LOWORD(lParam) };

    // Exit the function if no project is active.
    if( !m_bHaveProject ) return;

    pt.x = LOWORD( lParam );
    pt.y = HIWORD( lParam );

    // Get the cursor delta.
    nDiffX = pt.x - s_OldPt.x;
    nDiffY = pt.y - s_OldPt.y;

    GetCursorPos( &curPos );
    ScreenToClient( m_hRenderWnd,&curPos );
    GetClientRect( m_hRenderWnd,&rc );

    // If cursor are not in RenderView exit function.
    if( curPos.x < 0 || curPos.x > rc.right ||
        curPos.y < 0 || curPos.y > rc.bottom )
        return;

    // Update the brush.
    if( m_CamMode == CM_EDITCAMERA )
    {
        FlyVector vStart,vEnd,vOut;
        m_pDevice->Transfer2DTo3D( curPos,&ray.m_vcOrig,&ray.m_vcDir );
        vStart = ray.m_vcOrig;
        vEnd = vStart + ray.m_vcDir*512.0f*
            m_pSceneMgr->GetTerrainLevel()->GetHeightScale();
        etTerrain* pTerrain = m_pSceneMgr->GetTerrainLevel();
        if( pTerrain->IntersectSegment(vStart,vEnd,&vOut) )
            m_pBrush->SetPosition( vOut.x,vOut.z );
    }

    // Handle the entities for the editor.
    if( m_WorkMode == WKM_ENTITY && m_CamMode == CM_EDITCAMERA &&
        (wParam & MK_LBUTTON) )
    {
        m_pDevice->Transfer2DTo3D( curPos,&ray.m_vcOrig,&ray.m_vcDir );

        if( m_pSelBuf->IsSelecting() )
        {
            float fMoveSpeed = m_Config.GetETMoveSpeed();
            float fRotateSpeed = m_Config.GetETRotateSpeed();
            float fScaleSpeed = m_Config.GetETScaleSpeed();

            switch( m_pSelBuf->GetToolType() )
            {
            case ETT_MOVE_XZ:
                if( nDiffX < 0 )
                    m_pSelBuf->Translate( FlyVector(-fMoveSpeed,0.0f,0.0f) );
                else if( nDiffX > 0 )
                    m_pSelBuf->Translate( FlyVector(fMoveSpeed,0.0f,0.0f) );

                if( nDiffY < 0 )
                    m_pSelBuf->Translate( FlyVector(0.0f,0.0f,fMoveSpeed) );
                else if( nDiffY > 0 )
                    m_pSelBuf->Translate( FlyVector(0.0f,0.0f,-fMoveSpeed) );
                break;
            case ETT_MOVE_Y:
                if( nDiffY < 0 )
                    m_pSelBuf->Translate( FlyVector(0.0f,fMoveSpeed,0.0f) );
                else if( nDiffY > 0 )
                    m_pSelBuf->Translate( FlyVector(0.0f,-fMoveSpeed,0.0f) );
                break;
            case ETT_ROTATION_X:
                if( nDiffX < 0 )
                    m_pSelBuf->RotateX( -fRotateSpeed );
                else if( nDiffX > 0 )
                    m_pSelBuf->RotateX( fRotateSpeed );
                break;
            case ETT_ROTATION_Y:
                if( nDiffX < 0 )
                    m_pSelBuf->RotateY( -fRotateSpeed );
                else if( nDiffX > 0 )
                    m_pSelBuf->RotateY( fRotateSpeed );
                break;
            case ETT_ROTATION_Z:
                if( nDiffX < 0 )
                    m_pSelBuf->RotateZ( -fRotateSpeed );
                else if( nDiffX > 0 )
                    m_pSelBuf->RotateZ( fRotateSpeed );
                break;
            case ETT_SCALE_XYZ:
                if( nDiffY < 0 )
                    m_pSelBuf->Scale( FlyVector(fScaleSpeed,fScaleSpeed,fScaleSpeed) );
                else if( nDiffY > 0 )
                    m_pSelBuf->Scale( FlyVector(-fScaleSpeed,-fScaleSpeed,-fScaleSpeed) );
                break;
            }
        }
        else
        {
            m_pSelBuf->DraggingRect( curPos );
        }
    }

    // Update the camera's state.
    if( m_CamMode == CM_EDITCAMERA && (wParam & MK_RBUTTON) )
    {
        float fRotateSpeed = m_Config.GetCamRotateSpeed();

        if( nDiffX > 0 ) m_pEditCamera->Yaw(  fRotateSpeed*m_fElapsed );
        if( nDiffX < 0 ) m_pEditCamera->Yaw( -fRotateSpeed*m_fElapsed );
        if( nDiffY > 0 ) m_pEditCamera->Pitch(  fRotateSpeed*m_fElapsed );
        if( nDiffY < 0 ) m_pEditCamera->Pitch( -fRotateSpeed*m_fElapsed );
    }

    // Save the old cursor point.
    s_OldPt = pt;

    // Activate the application.
    Activate();
}


//------------------------------------------------------------------------
// Handle the window message. WM_MOUSEWHELL.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etCoreManager::OnMouseWheel( WPARAM wParam,LPARAM lParam )
{
}


//------------------------------------------------------------------------
// Handle the window message, WM_RBUTTONDBLCLK.
// ----------------------------------------------------------------------
// Param -> IN:
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etCoreManager::OnMouseRDblClick( WPARAM wParam,LPARAM lParam )
{
    POINT pt;
    RECT rcRender;

    if( !m_bHaveProject ) return;

    GetCursorPos( &pt );
    ScreenToClient( m_hRenderWnd,&pt );
    GetClientRect( m_hRenderWnd,&rcRender );

    if( pt.x < rcRender.left || pt.x > rcRender.right ||
        pt.y < rcRender.top || pt.y > rcRender.bottom )
        return;

    // Get the pointer to the entityManager.
    if( m_pEntityMgr->GetCurrentType() != ENTITY_UNKNOWN &&
        m_pEntityMgr->GetCurrentIndex() != -1 )
    {
        FlyVector vPosition;
        vPosition.x = m_pBrush->GetPositionX();
        vPosition.z = m_pBrush->GetPositionZ();
        vPosition.y = m_pSceneMgr->GetHeightValue( vPosition );
        m_pEntityMgr->PutEntityTerrain( vPosition );
    }
}


//------------------------------------------------------------------------
// Call this function before rendering at each frame.
//------------------------------------------------------------------------
void etCoreManager::OnEngineRunningA(void)
{
    if( !m_bHaveProject )
        return;

    // Update the elapsed time and FPS.
    m_fElapsed = m_pTimer->GetElapsed();
    m_fFPS = m_pTimer->GetFPS();

    // 1. Handle the keystates for editor.
    if( m_bHaveProject ) UpdateCameraState();

    // 2. Modify the terrain by brush.
    if( m_bMouseState[0] && m_WorkMode == WKM_TERRAIN &&
        m_CamMode == CM_EDITCAMERA )
        m_pBrush->ModifyTerrain();

    // 3. Update the detail layer.
    m_pSceneMgr->GetTerrainLevel()->GetDetailLayer()->UpdateTextureData();

    // Callback function on the application layer for the editor.
    // Need to be set when application startup.
    if( m_pCBFuncA ) m_pCBFuncA();
}


//------------------------------------------------------------------------
// Call this function after rendering at each frame.
//------------------------------------------------------------------------
void etCoreManager::OnEngineRunningB(void)
{
    if( m_pCBFuncB ) m_pCBFuncB();
}


//------------------------------------------------------------------------
// Set the first callback function for the core module.
// ----------------------------------------------------------------------
// Param -> IN:
//      PCALLBACKMAINLOOP:  Pointer to the callback function.
//------------------------------------------------------------------------
void etCoreManager::SetCallbackFuncA( PCALLBACKMAINLOOP pFunc )
{
    m_pCBFuncA = pFunc;
}


//------------------------------------------------------------------------
// Set the second callback function for the core module.
// ----------------------------------------------------------------------
// Param -> IN:
//      PCALLBACKMAINLOOP:  Pointer to the callback function.
//------------------------------------------------------------------------
void etCoreManager::SetCallbackFuncB( PCALLBACKMAINLOOP pFunc )
{
    m_pCBFuncB = pFunc;
}


//------------------------------------------------------------------------
// Get the count of rendered primitives.
//------------------------------------------------------------------------
UINT etCoreManager::GetNumRenderedPrims(void) const
{
    return m_pRenderer->GetNumRenderedPrims();
}


//------------------------------------------------------------------------
// Shutdown the core module of the editor.
//------------------------------------------------------------------------
void etCoreManager::Shutdown(void)
{
    SAFE_DELETE( m_pFactory );
    SAFE_DELETE( m_pEntityMgr );
    SAFE_DELETE( m_pSelBuf );
    SAFE_DELETE( m_pBrush );

    // Shutdown the engine.
    FlyKernel::Instance().Shutdown();
}


//------------------------------------------------------------------------
// CALLBACK function to handle the window messages.
// ----------------------------------------------------------------------
// Param -> IN:
//      HWND:       Handle to the window.
//      UINT:       Window message.
//      WPARAM:     Param of the window message.
//      LPARAM:     Param of the window message.
//------------------------------------------------------------------------
void etCoreManager::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    etCoreManager& core = etCoreManager::Instance();

    switch( uMsg )
    {
    case WM_MOUSEMOVE:
        OnMouseMove( wParam,lParam );
        break;
    case WM_LBUTTONDOWN:
        OnMouseClick( true,wParam,lParam );
        m_bMouseState[0] = true;
        break;
    case WM_LBUTTONUP:
        OnMouseClick( false,wParam,lParam );
        m_bMouseState[0] = false;
        break;
    case WM_KEYDOWN:
        OnKeyPress( true,wParam,lParam );
        Activate();
        break;
    case WM_KEYUP:
        OnKeyPress( false,wParam,lParam );
        break;
    case WM_MOUSEWHEEL:
        OnMouseWheel( wParam,lParam );
        Activate();
        break;
    case WM_RBUTTONDBLCLK:
        OnMouseRDblClick( wParam,lParam );
        break;
    case WM_KILLFOCUS:
        m_bActive = false;
        m_fFPS = 0.0f;
        break;
    case WM_PAINT:
        if( core.IsHaveProject() ) core.EngineMainLoop();
        break;
    }
}