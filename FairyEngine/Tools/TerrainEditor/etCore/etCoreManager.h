//------------------------------------------------------------------------
// Name: etCoreManager.h
// Desc: This file define a class to control the editor.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_COREMANAGER_H__
#define __ET_COREMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"
#include "etConfig.h"

#include "FlyKernel.h"
#include "FlyTimer.h"
#include "FlyCameraBase.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etCoreManager
{
    typedef void (CALLBACK *PCALLBACKMAINLOOP) (void);

protected:
    HINSTANCE m_hInst;      // Handle to the application instance.
    HWND m_hMainWnd;        // Handle to the main window.
    HWND m_hRenderWnd;      // Handle to the render window.
    bool m_bActive;         // Is the engine active.
    DWORD m_dwLastActive;   // The last activation time.
    float m_fElapsed;       // Elapsed time of this frame.
    float m_fFPS;           // Current FPS of the editor.

    bool m_bHaveProject;    // Does we have active project now ?
    bool m_bKeystate[64];   // Key state array.
    bool m_bMouseState[3];  // Mouse button state.

    etConfig m_Config;      // Config data of the editor.
    CAMERAMODE m_CamMode;   // Current camera mode.
    WORKMODE m_WorkMode;    // Current work mode.
    UINT m_nCurrentSkybox;  // Current skybox.

    // Member variables for render engine.
    FlyTimer* m_pTimer;             // Pointer to the timer.
    LPFLYRENDERDEVICE m_pDevice;    // Pointer to the render device.
    LPFLYRESOURCEMANAGER m_pResMgr; // Pointer to the resource manager.
    LPFLYRENDERER m_pRenderer;      // Pointer to the renderer.

    etSceneFactory* m_pFactory;     // Pointer to the sceneFactory.
    etSceneManager* m_pSceneMgr;    // Pointer to the sceneManager.
    etEntityManager* m_pEntityMgr;  // Pointer to the entityManager.
    etSelectionBuffer* m_pSelBuf;   // Pointer to the selectionBuffer.
    etBrush* m_pBrush;              // Pointer to the brush.

    FlyRenderEffect* m_pTerrainFX;      // Pointer to the terrain renderEffect.
    FlyRenderEffect* m_pVegetationFX;   // Pointer to the vegetation renderEffect.
    FlyRenderEffect* m_pLiquidFX;       // Pointer to the liquid renderEffect.

    FlyCameraBase* m_pEditCamera;   // Pointer to the camera when editing terrain.
    FlyCameraBase* m_pRambleCamera; // Pointer to the ramble camera.

    PCALLBACKMAINLOOP m_pCBFuncA;   // Pointer to the first callback function.
    PCALLBACKMAINLOOP m_pCBFuncB;   // Pointer to the second callback function.

public:
    // Constructor and destructor.
    etCoreManager(void);
    ~etCoreManager(void);

    // Initialize the core module of the editor.
    HRESULT Initialize( HINSTANCE hInst,HWND hMainWnd,HWND hRenderWnd );

    // Call this function at each frame.
    void EngineMainLoop(void);

    void OnEngineRunningA(void);
    void OnEngineRunningB(void);

    // Shutdown the core Module of the editor.
    void Shutdown(void);

    //////////////////////////////////////////////////////////////////////

    void CreateNewProject(void);
    HRESULT LoadProject( const char* cProject );
    void CloseProject(void);

    // Reset the camera for the editor.
    void ResetCamera(void);

    // Get the current FPS of the editor.
    float GetCurrentFPS(void) const { return m_fFPS; }
    float GetElapsed(void) const { return m_fElapsed; }
    UINT GetNumRenderedPrims(void) const;

    CAMERAMODE GetCameraMode(void) const { return m_CamMode; }
    void SetCameraMode( CAMERAMODE mode );

    WORKMODE GetWorkMode(void) const { return m_WorkMode; }
    void SetWorkMode( WORKMODE wkMode ) { m_WorkMode = wkMode; }

    void GetFog( FOGMODE* pMode,FLYCOLOR* pColor,float* fDensity,float* fNear,float* fFar );
    void SetFog( FOGMODE mode,const FLYCOLOR& color,float fDensity,float fNear,float fFar );

    bool IsShowSkybox(void) const;
    UINT GetCurrentSkybox(void) const { return m_nCurrentSkybox; }
    void SetSkyboxByName( bool bEnable,const char* cSkyName );
    void SetSkyboxByIndex( bool bEnable,UINT nIndex );

    void SetViewPlanes( float fNear,float fFar );
    void SetBackColor( const FLYCOLOR& color );

    // Have an active project ?
    bool IsHaveProject(void) const { return m_bHaveProject; }

    // Activate the application.
    void Activate(void);

    // Is the engine active now ?
    bool IsActive(void) const { return m_bActive; }

    void ExportEntityList( const char* cXmlDoc );
    void ImportEntityList( const char* cXmlDoc );

    HINSTANCE GetInstance(void) const { return m_hInst; }
    HWND GetMainWindow(void) const { return m_hMainWnd; }
    HWND GetRenderWindow(void) const { return m_hRenderWnd; }
    LPFLYRENDERDEVICE GetRenderDevice(void) { return m_pDevice; }
    LPFLYRENDERER GetRenderer(void) { return m_pRenderer; }
    LPFLYRESOURCEMANAGER GetResourceManager(void) { return m_pResMgr; }
    FlyTimer* GetTimer(void) { return m_pTimer; }

    etConfig* GetConfigData(void) { return &m_Config; }
    etSceneManager* GetSceneManager(void) { return m_pSceneMgr; }
    etEntityManager* GetEntityManager(void) { return m_pEntityMgr; }
    etSelectionBuffer* GetSelectionBuffer(void) { return m_pSelBuf; }
    etBrush* GetBrush(void) { return m_pBrush; }

    void SetCallbackFuncA( PCALLBACKMAINLOOP pFunc );
    void SetCallbackFuncB( PCALLBACKMAINLOOP pFunc );

    // CALLBACK function to handle the window messages.
    void MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

    // Get the pointer to the renderEffect.
    FlyRenderEffect* GetTerrainFX(void) const { return m_pTerrainFX; }
    FlyRenderEffect* GetVegetationFX(void) const { return m_pVegetationFX; }
    FlyRenderEffect* GetLiquidFX(void) const { return m_pLiquidFX; }

    static etCoreManager& Instance(void);

protected:
    HRESULT BuildRenderEffects(void);
    HRESULT OneTimeSceneInit(void);

    void OnMouseClick( bool bDown,WPARAM wParam,LPARAM lParam );
    void OnMouseMove( WPARAM wParam,LPARAM lParam );
    void OnMouseWheel( WPARAM wParam,LPARAM lParam );
    void OnKeyPress( bool bDown,WPARAM wParam,LPARAM lParam );
    void OnMouseRDblClick( WPARAM wParam,LPARAM lParam );

    void UpdateCameraState(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_COREMANAGER_H__