//------------------------------------------------------------------------
// Name: FlyKernel.h
// Desc: This file define a kernel interface to control the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_KERNEL_H__
#define __FLY_KERNEL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyRenderDevice.h"
#include "FlyResourceManager.h"
#include "FlyRenderer.h"

//////////////////////////////////////////////////////////////////////////


/*
* User can use this class to control the whole engine.
*/
class _FLY_EXPORT FlyKernel
{
    typedef std::vector<FlySceneManager*> SceneManagerList;
    typedef std::vector<FlySceneFactory*> SceneFactoryList;

    typedef std::vector<FlyCamFactory*> CamFactoryList;
    typedef std::vector<FlyEntityFactory*> EntityFactoryList;
    typedef std::vector<FlyLightFactory*> LightFactoryList;

public:
    typedef void (CALLBACK *PCALLBACKMAINLOOP) (void);

protected:
    FlyTimer* m_pTimer;                 // Timer object of the engine.

    LPFLYRENDERDEVICE m_pDevice;        // Pointer to the renderDevice.
    LPFLYRESOURCEMANAGER m_pResMgr;     // Pointer to the resourceManager.
    LPFLYRENDERER m_pRenderer;          // Pointer to the renderer.

    FlyMeshManager* m_pMeshMgr;         // Pointer to the mesh Manager.
    FlyAnimManager* m_pAnimMgr;         // Pointer to the animation manager.
    FlyParticleSystem* m_pParSystem;    // Pointer to the particle system.
    FlyCamManager* m_pCamMgr;           // Pointer to the camera Manager.

    SceneManagerList m_SceneMgrs;       // SceneManager list of the engine.
    FlySceneManager* m_pActiveScene;    // Current active sceneManager.

    FlyPluginManager* m_pPluginMgr;     // Pointer to the PluginManager.

    // Factorys for the engine.
    CamFactoryList m_CamFactorys;       // Camera factory list.
    EntityFactoryList m_EntityFactorys; // Entity factory list.
    LightFactoryList m_LightFactorys;   // Light factory list.
    SceneFactoryList m_SceneFactorys;   // SceneManager factory list.

    PCALLBACKMAINLOOP m_pFuncA;         // Pointer to the function which executed before rendering.
    PCALLBACKMAINLOOP m_pFuncB;         // Pointer to the function which executed after rendering.

public:
    // Constructor and destructor.
    FlyKernel(void);
    ~FlyKernel(void);

    // Initialize the engine.
    HRESULT Initialize( const char* cRenderSystem,HWND hWndMain,HWND* hWnds,
                        int nNumWnds,bool bForceShowConfig );

    // Shutdown the engine.
    void Shutdown(void);

    // Main loop function of the engine.
    void EngineMainLoop(void);

    // Save the screen shot for the application.
    void SaveScreenshot(void);

    // Register a new camera type.
    void RegisterCameraType( FlyCamFactory* pFactory );

    // Register a new entity type.
    void RegisterEntityType( FlyEntityFactory* pFactory );

    // Register a new light type.
    void RegisterLightType( FlyLightFactory* pFactory );

    // Register a new scene type.
    void RegisterSceneType( FlySceneFactory* pFactory );

    // Create a new sceneManager for the engine.
    FlySceneManager* CreateSceneManager( const char* cName,const char* cSceneType );

    // Destroy a sceneManager.
    void DestroySceneManager( const char* cName );

    // Destroy all the sceneManager.
    void DestroyAllSceneManager(void);

    // Select the active sceneManager.
    void SetActiveSceneManager( const char* cName );
    void SetActiveSceneManager( FlySceneManager* pManager );

    // Get the specified sceneManager.
    FlySceneManager* GetSceneManager( const char* cName );

    // Get the active sceneManager.
    FlySceneManager* GetActiveSceneManager(void);

    FlyCamFactory* GetCameraFactory( const char* cTypename );
    FlyEntityFactory* GetEntityFactory( const char* cTypename );
    FlyLightFactory* GetLightFactory( const char* cTypename );
    FlySceneFactory* GetSceneFactory( const char* cTypename );

    FlyMeshManager* GetMeshManager(void);
    FlyAnimManager* GetAnimManager(void);
    FlyParticleSystem* GetParticleSystem(void);
    FlyCamManager* GetCamManager(void);
    FlyPluginManager* GetPluginManager(void);

    LPFLYRENDERDEVICE GetRenderDevice(void);
    LPFLYRESOURCEMANAGER GetResourceManager(void);
    LPFLYRENDERER GetRenderer(void);

    // Get the timer of the engine.
    FlyTimer* GetTimer(void);

    // Set the callback function.
    void SetCallbackFuncA( PCALLBACKMAINLOOP pFunc );
    void SetCallbackFuncB( PCALLBACKMAINLOOP pFunc );

    static FlyKernel& Instance(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_KERNEL_H__