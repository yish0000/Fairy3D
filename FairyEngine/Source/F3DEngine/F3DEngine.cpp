/*
 * -----------------------------------------------------------------------
 *  Name:   F3DEngine.cpp
 *  Desc:   本文件实现了引擎的主控接口，可以通过该文件控制整个引擎。
 *  Author: Yish
 *  Date:   2011/6/10
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2011 All right Observed.
 * -----------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DEngine.h"
#include "F3DSceneManager.h"

F3DEngine& F3DEngine::GetInstance()
{
	static F3DEngine obj;
	return obj;
}

F3DEngine::F3DEngine()
{
}

F3DEngine::~F3DEngine()
{
}

bool F3DEngine::Initialize()
{
	return true;
}

void F3DEngine::Shutdown()
{
}

void F3DEngine::Frame()
{
	// Update the scene manager.
	if (m_pSceneMgr)
		m_pSceneMgr->Update();
}

/*
#include "FlyD3DRenderDevice.h"
#include "FlyD3DResourceManager.h"
#include "FlyD3DRenderer.h"

#include "FlyMeshEntity.h"
#include "FlyAnimEntity.h"
#include "FlyParticleEntity.h"
#include "FlyManualEntity.h"

#include "FlyBspSceneManager.h"
#include "FlyOctreeSceneManager.h"
#include "FlyTerrainSceneManager.h"

#include "FlyMeshManager.h"
#include "FlyAnimManager.h"
#include "FlyParticleSystem.h"
#include "FlyCamManager.h"

#include "FlyPluginManager.h"
#include "FlyLogManager.h"


extern HINSTANCE g_hModule;

//------------------------------------------------------------------------
// Get the instance of the kernel object.
//------------------------------------------------------------------------
FlyKernel& FlyKernel::Instance(void)
{
    static FlyKernel obj;
    return obj;
}


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyKernel::FlyKernel(void)
{
    m_bPrintScreen = false;

    m_pDevice = NULL;
    m_pResMgr = NULL;
    m_pRenderer = NULL;

    m_pMeshMgr = NULL;
    m_pAnimMgr = NULL;
    m_pParSystem = NULL;
    m_pCamMgr = NULL;
    m_pPluginMgr = NULL;

    m_pActiveScene = NULL;

    m_pFuncA = NULL;
    m_pFuncB = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyKernel::~FlyKernel(void)
{
}


//------------------------------------------------------------------------
// Shutdown the engine.
//------------------------------------------------------------------------
void FlyKernel::Shutdown(void)
{
    // Shutdown the engine!
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();
    logMgr.WriteLine( 0,"------------------Engine shutdown-------------------" );

    // Destroy all the sceneManager.
    DestroyAllSceneManager();

    SAFE_DELETE( m_pMeshMgr );
    SAFE_DELETE( m_pAnimMgr );
    SAFE_DELETE( m_pParSystem );
    SAFE_DELETE( m_pCamMgr );
    SAFE_DELETE( m_pPluginMgr );

    // Destroy the default camera factory.
    SAFE_DELETE( m_CamFactorys[0] );

    // Destroy the entity factorys.
    SAFE_DELETE( m_EntityFactorys[0] );
    SAFE_DELETE( m_EntityFactorys[1] );
    SAFE_DELETE( m_EntityFactorys[2] );
    SAFE_DELETE( m_EntityFactorys[3] );

    // Destroy the default light factory.
    SAFE_DELETE( m_LightFactorys[0] );

    // Destroy the scene factorys.
    SAFE_DELETE( m_SceneFactorys[0] );
    SAFE_DELETE( m_SceneFactorys[1] );
    SAFE_DELETE( m_SceneFactorys[2] );
    SAFE_DELETE( m_SceneFactorys[3] );

    SAFE_DELETE( m_pResMgr );
    SAFE_DELETE( m_pRenderer );
    SAFE_DELETE( m_pDevice );

    SAFE_DELETE( m_pTimer );
}


//------------------------------------------------------------------------
// Initialize the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the render system.
//      HWND:           Handle of the main renderWindow.
//      HWND*:          Handle of the sub renderWindows.
//      int:            Count of the sub renderWindows.
//      bool:           Force the engine show config dialog ?
//------------------------------------------------------------------------
HRESULT FlyKernel::Initialize( const char* cRenderSystem,HWND hWndMain,HWND* hWnds,
                               int nNumWnds,bool bForceShowConfig )
{
    HRESULT hr;

    // Create a global timer for engine.
    m_pTimer = new FlyTimer( 0,0 );

    // Create a new log file.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();
    logMgr.CreateLog( "EngineLog.log",true );
    FLOG_INFO( "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" );
    logMgr.WriteText( 0,"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" );
    logMgr.WriteText( 0,"+                                                                     +\n" );
    logMgr.WriteText( 0,"+-+-+-+-+-+-+-+-+-+-+-   FlyRender Engine log    -+-+-+-+-+-+-+-+-+-+-+\n" );
    logMgr.WriteText( 0,"+                                                                     +\n" );
    logMgr.WriteText( 0,"+-+-+-+-+-+-+-+-+-+-+-       Author: Yish        -+-+-+-+-+-+-+-+-+-+-+\n" );
    logMgr.WriteText( 0,"+                   (E-mail: dmfs0000@yahoo.com.cn)                   +\n" );
    logMgr.WriteText( 0,"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n\n" );
    logMgr.WriteLine( 0,"--------------------Engine start--------------------" );

    if( strcmp(cRenderSystem,"Direct3D") == 0 )
    {
        logMgr.WriteLine( 0,"Creating the Render system..." );
        m_pDevice = new FlyD3DRenderDevice( g_hModule );
        if( !m_pDevice )
        {
            logMgr.WriteError( 0,"Create the Render System failed!" );
            return FLY_OUTOFMEMORY;
        }

        // Initialize the renderDevice.
        hr = m_pDevice->CreateDevice( hWndMain,hWnds,nNumWnds,bForceShowConfig );
        if( FAILED(hr) )
        {
            SAFE_DELETE( m_pDevice );
            logMgr.WriteError( 0,"Initialize the renderDevice failed!" );
            return hr;
        }

        // Create the resource Manager for engine.
        logMgr.WriteLine( 0,"Creating the resource manager for engine..." );
        m_pResMgr = new FlyD3DResourceManager( (FlyD3DRenderDevice*)m_pDevice );
        if( !m_pResMgr )
        {
            logMgr.WriteError( 0,"Create the resource manager failed!" );
            return NULL;
        }

        // Create the renderer for engine.
        logMgr.WriteLine( 0,"Creating the renderer for engine..." );
        m_pRenderer = new FlyD3DRenderer( (FlyD3DRenderDevice*)m_pDevice );
        if( !m_pRenderer )
        {
            logMgr.WriteError( 0,"Create the Renderer failed!" );
            return NULL;
        }

        // Initialize the renderer.
        if( FAILED(m_pRenderer->InitRenderer()) )
        {
            SAFE_DELETE( m_pRenderer );
            logMgr.WriteError( 0,"Initialize the renderer failed!" );
            return NULL;
        }

        logMgr.WriteLine( 0,"Render system %s initialized!",cRenderSystem );
    }
    else if( strcmp(cRenderSystem,"OpenGL") == 0 )
    {
        logMgr.WriteError( 0,"Render system OpenGL hasn't been implemented!" );
        return FLY_CREATEAPI;
    }
    else
    {
        logMgr.WriteError( 0,"Render system %s hasn't been implemented!",cRenderSystem );
        return FLY_CREATEAPI;
    }

    // Initialize the camera factorys.
    RegisterCameraType( new FlyCamFactory() );

    // Initialize the entity factorys.
    RegisterEntityType( new FlyMeshEntityFactory() );
    RegisterEntityType( new FlyAnimEntityFactory() );
    RegisterEntityType( new FlyParticleEntityFactory() );
    RegisterEntityType( new FlyManualEntityFactory() );

    // Initialize the light factorys.
    RegisterLightType( new FlyLightFactory() );

    // Initialize the scene factorys.
    RegisterSceneType( new FlySceneFactory() );
    RegisterSceneType( new FlyBspSceneFactory() );
    RegisterSceneType( new FlyOctreeSceneFactory() );
    RegisterSceneType( new FlyTerrainSceneFactory() );

    // Create the meshManager.
    logMgr.WriteLine( 0,"Creating the mesh manager for engine..." );
    m_pMeshMgr = new FlyMeshManager();
    if( !m_pMeshMgr )
    {
        logMgr.WriteError( 0,"Creating the meshManager failed!" );
        return FLY_OUTOFMEMORY;
    }

    // Create the animation Manager.
    logMgr.WriteLine( 0,"Creating the animation manager for engine..." );
    m_pAnimMgr = new FlyAnimManager();
    if( !m_pAnimMgr )
    {
        logMgr.WriteError( 0,"Creating the animation manager failed!" );
        return FLY_OUTOFMEMORY;
    }

    // Create the particle system.
    logMgr.WriteLine( 0,"Creating the particle system for engine..." );
    m_pParSystem = new FlyParticleSystem();
    if( !m_pParSystem )
    {
        logMgr.WriteError( 0,"Creating the particle system failed!" );
        return FLY_OUTOFMEMORY;
    }

    // Create the camera manager.
    logMgr.WriteLine( 0,"Creating the camera Manager for engine..." );
    m_pCamMgr = new FlyCamManager();
    if( !m_pCamMgr )
    {
        logMgr.WriteError( 0,"Creating the camera manager failed!" );
        return FLY_OUTOFMEMORY;
    }

    // Install all the plugins.
    m_pPluginMgr->InstallPlugins();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Main loop function of the engine.
//------------------------------------------------------------------------
void FlyKernel::EngineMainLoop(void)
{
    if( m_pDevice->IsDeviceLost() )
    {
        m_pDevice->RestoreDevice();
        return;
    }

    // Update the timer.
    m_pTimer->Update();

    // Reset the count of rendered objects.
    m_pRenderer->ResetRenderedCount();

    // Begin the rendering.
    m_pDevice->BeginRendering();
    m_pDevice->ClearBuffer( true,true,true );

    if( m_pFuncA ) m_pFuncA();

    // Render the current active scene.
    if( m_pActiveScene )
        m_pActiveScene->RenderScene();

    if( m_pFuncB ) m_pFuncB();

    // Flush the surfaceQueue.
    m_pRenderer->FlushLayoutQueue();

    // End the rendering.
    m_pDevice->EndRendering();

    // 保存当前屏幕到图片
    if( m_bPrintScreen )
    {
        // 生成要保存的文件名
        char cTemp[MAX_PATH];
        time_t t = time( NULL );
        tm* pt = localtime( &t );
        sprintf( cTemp,"Screenshot\\%d_%d_%d__%d_%d_%d.bmp",pt->tm_year+1900,pt->tm_mon+1,
            pt->tm_mday,pt->tm_hour,pt->tm_min,pt->tm_sec );

        m_pDevice->SaveScreenToFile( FIMAGE_BMP,cTemp );
        m_bPrintScreen = false;
    }

    // 交换前后缓冲区
    m_pDevice->SwapBuffers();
}


//------------------------------------------------------------------------
// Save the screen shot for the engine.
//------------------------------------------------------------------------
void FlyKernel::SaveScreenshot(void)
{
    // 创建截屏文件目录
    CreateDirectory( "Screenshot",NULL );
    m_bPrintScreen = true;
}


//------------------------------------------------------------------------
// Register a new camera type for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyCamFactory*:     Pointer to the camera factory.
//------------------------------------------------------------------------
void FlyKernel::RegisterCameraType( FlyCamFactory* pFactory )
{
    m_CamFactorys.push_back( pFactory );

    FlyLogManager::GetSingleton().WriteLine( 0,"A new camera type \"%s\" added!",
        pFactory->GetTypename() );
}


//------------------------------------------------------------------------
// Register a new entity type for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyEntityFactory*:  Pointer to the entity factory.
//------------------------------------------------------------------------
void FlyKernel::RegisterEntityType( FlyEntityFactory* pFactory )
{
    m_EntityFactorys.push_back( pFactory );

    FlyLogManager::GetSingleton().WriteLine( 0,"A new entity type \"%s\" added!",
        pFactory->GetTypename() );
}


//------------------------------------------------------------------------
// Register a new light type for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyLightFactory*:   Pointer to the light factory.
//------------------------------------------------------------------------
void FlyKernel::RegisterLightType( FlyLightFactory* pFactory )
{
    m_LightFactorys.push_back( pFactory );

    FlyLogManager::GetSingleton().WriteLine( 0,"A new light type \"%s\" added!",
        pFactory->GetTypename() );
}


//------------------------------------------------------------------------
// Register a new scene type for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneFactory*:   Pointer to the scene factory.
//------------------------------------------------------------------------
void FlyKernel::RegisterSceneType( FlySceneFactory* pFactory )
{
    m_SceneFactorys.push_back( pFactory );

    FlyLogManager::GetSingleton().WriteLine( 0,"A new scene type \"%s\" added!",
        pFactory->GetTypename() );
}


//------------------------------------------------------------------------
// Create a new sceneManager for the scene.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the sceneManager.
//      const char*:    Type name of the instance.
//------------------------------------------------------------------------
FlySceneManager* FlyKernel::CreateSceneManager( const char* cName,const char* cSceneType )
{
    FlySceneManager* pManager;

    // Check whether the sceneManager is existed ?
    SceneManagerList::iterator itr;
    for( itr=m_SceneMgrs.begin();itr!=m_SceneMgrs.end();itr++ )
    {
        if( strcmp((*itr)->GetName(),cName) == 0 )
            break;
    }

    if( itr != m_SceneMgrs.end() )
        return NULL;

    // Get the specified sceneFactory.
    FlySceneFactory* pFactory = GetSceneFactory( cSceneType );
    if( !pFactory ) return NULL;

    pManager = pFactory->CreateInstance( cName );
    if( !pManager ) return NULL;

    m_SceneMgrs.push_back( pManager );
    return pManager;
}


//------------------------------------------------------------------------
// Destroy a specified sceneManager.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the sceneManager.
//------------------------------------------------------------------------
void FlyKernel::DestroySceneManager( const char* cName )
{
    SceneManagerList::iterator itr;
    for( itr=m_SceneMgrs.begin();itr!=m_SceneMgrs.end();itr++ )
    {
        if( strcmp((*itr)->GetName(),cName) == 0 )
            break;
    }

    if( itr != m_SceneMgrs.end() )
    {
        if( *itr == m_pActiveScene )
            m_pActiveScene = NULL;

        SAFE_DELETE( *itr );
        m_SceneMgrs.erase( itr );
    }
}


//------------------------------------------------------------------------
// Destroy all the sceneManager.
//------------------------------------------------------------------------
void FlyKernel::DestroyAllSceneManager(void)
{
    SceneManagerList::iterator itr;
    for( itr=m_SceneMgrs.begin();itr!=m_SceneMgrs.end();itr++ )
        SAFE_DELETE( *itr );

    m_SceneMgrs.clear();
    m_pActiveScene = NULL;
}


//------------------------------------------------------------------------
// Set the current active sceneManager for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the sceneManager.
//------------------------------------------------------------------------
void FlyKernel::SetActiveSceneManager( const char* cName )
{
    // Search the sceneManager.
    SceneManagerList::iterator itr;
    for( itr=m_SceneMgrs.begin();itr!=m_SceneMgrs.end();itr++ )
    {
        if( strcmp((*itr)->GetName(),cName) == 0 )
            break;
    }

    if( itr != m_SceneMgrs.end() )
        m_pActiveScene = *itr;
}


//------------------------------------------------------------------------
// Set the current active sceneManager for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneManager*:   Pointer to the sceneManager.
//------------------------------------------------------------------------
void FlyKernel::SetActiveSceneManager( FlySceneManager* pManager )
{
    m_pActiveScene = pManager;
}


//------------------------------------------------------------------------
// Get the specified sceneManager.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the sceneManager.
//------------------------------------------------------------------------
FlySceneManager* FlyKernel::GetSceneManager( const char* cName )
{
    // Search the sceneManager.
    SceneManagerList::iterator itr;
    for( itr=m_SceneMgrs.begin();itr!=m_SceneMgrs.end();itr++ )
    {
        if( strcmp((*itr)->GetName(),cName) == 0 )
            break;
    }

    if( itr != m_SceneMgrs.end() )
        return *itr;
    else
        return NULL;
}


//------------------------------------------------------------------------
// Get the current active sceneManager.
//------------------------------------------------------------------------
FlySceneManager* FlyKernel::GetActiveSceneManager(void)
{
    return m_pActiveScene;
}


//------------------------------------------------------------------------
// Get a specified camera factory.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the camera factory.
//------------------------------------------------------------------------
FlyCamFactory* FlyKernel::GetCameraFactory( const char* cTypename )
{
    CamFactoryList::iterator itr;
    for( itr=m_CamFactorys.begin();itr!=m_CamFactorys.end();itr++ )
    {
        if( strcmp((*itr)->GetTypename(),cTypename) == 0 )
            break;
    }

    if( itr != m_CamFactorys.end() )
        return *itr;
    else
        return NULL;
}


//------------------------------------------------------------------------
// Get the specified entity factory.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the entity factory.
//------------------------------------------------------------------------
FlyEntityFactory* FlyKernel::GetEntityFactory( const char* cTypename )
{
    EntityFactoryList::iterator itr;
    for( itr=m_EntityFactorys.begin();itr!=m_EntityFactorys.end();itr++ )
    {
        if( strcmp((*itr)->GetTypename(),cTypename) == 0 )
            break;
    }

    if( itr != m_EntityFactorys.end() )
        return *itr;
    else
        return NULL;
}


//------------------------------------------------------------------------
// Get the specified light factory.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the light factory.
//------------------------------------------------------------------------
FlyLightFactory* FlyKernel::GetLightFactory( const char* cTypename )
{
    LightFactoryList::iterator itr;
    for( itr=m_LightFactorys.begin();itr!=m_LightFactorys.end();itr++ )
    {
        if( strcmp((*itr)->GetTypename(),cTypename) == 0 )
            break;
    }

    if( itr != m_LightFactorys.end() )
        return *itr;
    else
        return NULL;
}


//------------------------------------------------------------------------
// Get the specified scene factory.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the scene factory.
//------------------------------------------------------------------------
FlySceneFactory* FlyKernel::GetSceneFactory( const char* cTypename )
{
    SceneFactoryList::iterator itr;
    for( itr=m_SceneFactorys.begin();itr!=m_SceneFactorys.end();itr++ )
    {
        if( strcmp((*itr)->GetTypename(),cTypename) == 0 )
            break;
    }

    if( itr != m_SceneFactorys.end() )
        return *itr;
    else
        return NULL;
}


//------------------------------------------------------------------------
// Get the pointer to the meshManager.
//------------------------------------------------------------------------
FlyMeshManager* FlyKernel::GetMeshManager(void)
{
    return m_pMeshMgr;
}

//------------------------------------------------------------------------
// Get the pointer to the animManager.
//------------------------------------------------------------------------
FlyAnimManager* FlyKernel::GetAnimManager(void)
{
    return m_pAnimMgr;
}

//------------------------------------------------------------------------
// Get the pointer to the particleSystem.
//------------------------------------------------------------------------
FlyParticleSystem* FlyKernel::GetParticleSystem(void)
{
    return m_pParSystem;
}

//------------------------------------------------------------------------
// Get the pointer to the camera manager.
//------------------------------------------------------------------------
FlyCamManager* FlyKernel::GetCamManager(void)
{
    return m_pCamMgr;
}

//------------------------------------------------------------------------
// Get the pointer to the pluginManager.
//------------------------------------------------------------------------
FlyPluginManager* FlyKernel::GetPluginManager(void)
{
    return m_pPluginMgr;
}


//------------------------------------------------------------------------
// Get the pointer to the renderDevice.
//------------------------------------------------------------------------
LPFLYRENDERDEVICE FlyKernel::GetRenderDevice(void)
{
    return m_pDevice;
}

//------------------------------------------------------------------------
// Get the pointer to the resourceManager.
//------------------------------------------------------------------------
LPFLYRESOURCEMANAGER FlyKernel::GetResourceManager(void)
{
    return m_pResMgr;
}

//------------------------------------------------------------------------
// Get the pointer to the renderer.
//------------------------------------------------------------------------
LPFLYRENDERER FlyKernel::GetRenderer(void)
{
    return m_pRenderer;
}

//------------------------------------------------------------------------
// Get the timer of the engine.
//------------------------------------------------------------------------
FlyTimer* FlyKernel::GetTimer(void)
{
    return m_pTimer;
}


//------------------------------------------------------------------------
// Set the callback function which executed before rendering.
// ----------------------------------------------------------------------
// Param -> IN:
//      PCALLBACKMAINLOOP:  Pointer to the callback function.
//------------------------------------------------------------------------
void FlyKernel::SetCallbackFuncA( PCALLBACKMAINLOOP pFunc )
{
    m_pFuncA = pFunc;
}


//------------------------------------------------------------------------
// Set the callback function which executed after rendering.
// ----------------------------------------------------------------------
// Param -> IN:
//      PCALLBACKMAINLOOP:  Pointer to the callback function.
//------------------------------------------------------------------------
void FlyKernel::SetCallbackFuncB( PCALLBACKMAINLOOP pFunc )
{
    m_pFuncB = pFunc;
}

*/

///////////////////////////////////////////////////////////////////////////
//  
//  Global Functions of 3D Engine
//  
///////////////////////////////////////////////////////////////////////////

extern "C" {

	bool F3D_Init( const char* logFile /* = "F3D.log" */ )
	{
		F3DEngine& f3d = F3DEngine::GetInstance();


		return true;
	}

	void F3D_Finalize()
	{
	}

	F3DEngine* F3D_GetEngine()
	{
		return &F3DEngine::GetInstance();
	}
}