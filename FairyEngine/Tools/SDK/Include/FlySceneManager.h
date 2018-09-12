//------------------------------------------------------------------------
// Name: FlySceneManager.h
// Desc: This file
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_SCENEMANAGER_H__
#define __FLY_SCENEMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyFrustum.h"
#include "FlyCameraBase.h"
#include "FlySceneObject.h"
#include "FlySceneNode.h"

#include "FlyRenderTarget.h"
#include "FlyRenderAutoParameter.h"
#include "FlyRenderableHelper.h"
//////////////////////////////////////////////////////////////////////////


class FlySceneFactory;

/*
* This class defines the interface and the basic behaviour of a 
* 'Scene Manager'.
* A sceneManager organises the culling and rendering of the scene.
*/
class _FLY_EXPORT FlySceneManager
{
    typedef std::map<std::string,FlySceneNode*> SceneNodeMap;
    typedef std::map<std::string,FlySceneObject*> SceneObjectMap;
    typedef std::map<std::string,FlyLightObject*> LightObjectMap;

    typedef std::vector<FlyPlane> ClipPlaneList;
    typedef std::vector<FlyRenderTarget*> RenderTargetList;

protected:
    FlySceneFactory* m_pCreator;    // Pointer to the creator.

    std::string m_sName;            // Name of the sceneManager.
    LPFLYRENDERDEVICE m_pDevice;    // Pointer to the renderDevice.
    RenderTargetList m_Targets;     // Texture target list.
    ClipPlaneList m_ClipPlanes;     // Clip plane list.

    FlyFrustum m_Frustum;           // Pointer to the frustum.
    FlyCameraBase* m_pCamera;       // Current camera for sceneManager.

    SceneObjectMap m_SceneObjs;     // Scene object list.
    LightObjectMap m_LightObjs;     // Light object list.

    SceneNodeMap m_SceneNodes;      // Scene node list.
    FlySceneNode* m_pSceneRoot;     // Pointer to the root scene node.

    FOGMODE m_FogMode;              // Current fog mode.
    FLYCOLOR m_cFogColor;           // Current fog color.
    float m_fFogDensity;            // Current fog density.
    float m_fFogNear;               // Near plane where the fog start.
    float m_fFogFar;                // Far plane where the fog end.
    FLYCOLOR m_cAmbient;            // Color of the ambient.

    bool m_bShowSkybox;             // Does we show the skybox ?
    FlySkyboxHelper* m_pSkyHelper;  // Pointer to the skybox renderable object.

    bool m_bShadowMode;             // Open the stencil shadow mode.
    FlyShadowHelper* m_pShadowHelper;   // Render the shadow. (ShadowVolume technique)

    // Update the shader parameter automatically.
    FlyRenderAutoParameter* m_pAutoParameters;

    // Update the scene graph.
    virtual void UpdateSceneData(void);

    // Render the visible scene objects.
    virtual void RenderVisibleObjects(void);

    // Render the world geometry.
    virtual void RenderWorldLevel(void) { /* Nothing to do! */ };

    // Render the scenes to the texture target.
    virtual void RenderToTarget(void);

public:
    // Constructor and destructor.
    FlySceneManager(void);
    virtual ~FlySceneManager(void);

    // Render the scene to the texture target.
    void AddRenderTarget( FlyRenderTarget* pTarget );
    void RemoveRenderTarget( UINT nIndex );
    void RemoveAllRenderTargets(void);
    FlyRenderTarget* GetRenderTarget( UINT nIndex );

    void AddClipPlane( const FlyPlane& plane );
    void RemoveClipPlane( UINT nIndex );
    void RemoveAllClipPlanes(void);

    // Load the world data from file.
    virtual HRESULT LoadWorldGeometry( const char* cFilename );

    // Clear the scene data.
    virtual void ClearScene(void);

    // Entry function to render the scene.
    virtual void RenderScene(void);

    // Test the collision in the 3D world.
    virtual bool TestCollision( const FlyRay& ray,float fL,float& fDist );

    // Create a mesh entity.
    FlyMeshEntity* CreateMeshEntity( const char* cName,const char* cMeshName );

    // Create an animation entity.
    FlyAnimEntity* CreateAnimation( const char* cName,const char* cAnimName );

    // Create a particle effect entity.
    FlyParticleEntity* CreateParticleEntity( const char* cName,const char* cParName );

    // Create a manual entity.
    FlyManualEntity* CreateManualEntity( const char* cName );

    // Create a scene object.
    FlySceneObject* CreateSceneObject( const char* cName,const char* cTypename );

    void DestroySceneObject( const char* cName );
    void DestroySceneObject( FlySceneObject* pObject );
    void DestroyAllSceneObjects(void);
    FlySceneObject* GetSceneObject( const char* cName );

    // Create a light object.
    FlyLightObject* CreateDefaultLight( const char* cName );

    // Create a light object.
    FlyLightObject* CreateLightObject( const char* cName,const char* cTypename );

    void DestroyLight( const char* cName );
    void DestroyLight( FlyLightObject* pObject );
    void DestroyAllLights(void);
    FlyLightObject* GetLightObject( const char* cName );

    virtual FlySceneNode* CreateSceneNode( const char* cName=NULL );
    virtual void DestroySceneNode( const char* cName,bool bDestroyChildren );
    virtual void DestroySceneNode( FlySceneNode* pNode,bool bDestroyChildren );

    FlySceneNode* GetSceneNode( const char* cName );
    FlySceneNode* GetSceneRoot(void);

    const char* GetName(void) const;
    void SetName( const char* cName );

    const FLYCOLOR& GetAmbientColor(void) const;
    void SetAmbientColor( const FLYCOLOR& color );

    FOGMODE GetFogMode(void) const { return m_FogMode; }
    const FLYCOLOR& GetFogColor(void) const { return m_cFogColor; }
    float GetFogDensity(void) const { return m_fFogDensity; }
    float GetFogNear(void) const { return m_fFogNear; }
    float GetFogFar(void) const { return m_fFogFar; }

    void SetFogMode( FOGMODE fogMode ) { m_FogMode = fogMode; }
    void SetFogColor( const FLYCOLOR& color ) { m_cFogColor = color; }
    void SetFogDensity( float fDensity ) { m_fFogDensity = fDensity; }
    void SetFogNear( float fNear ) { m_fFogNear = fNear; }
    void SetFogFar( float fFar ) { m_fFogFar = fFar; }

    bool IsShowSkybox(void) const { return m_bShowSkybox; }
    void SetSkybox( bool bEnable,float fBoxSize,const char* cFrontTexture,
        const char* cBackTexture,const char* cLeftTexture,
        const char* cRightTexture,const char* cTopTexture,
        const char* cBottomTexture );

    bool IsShadowModeOpen(void) const;
    void SetShadowMode( bool bShadowMode );

    FlyCameraBase* GetCurrentCamera(void);
    void SetCurrentCamera( FlyCameraBase* pCamera );
    FlyFrustum* GetFrustum(void);

    // Get the typename of the sceneManager.
    const char* GetSceneType(void) const;

    FlySceneFactory* GetCreator(void);
    void SetCreator( FlySceneFactory* pFactory );
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlySceneFactory
{
protected:
    std::string m_sTypename;

public:
    // Constructor and destructor.
    FlySceneFactory(void) { m_sTypename = "default"; }
    virtual ~FlySceneFactory(void) { /* Nothing to do! */ };

    virtual FlySceneManager* CreateInstance( const char* cName );
    virtual void DestroyInstance( FlySceneManager* pManager );

    const char* GetTypename(void);
    void SetTypename( const char* cTypename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SCENEMANAGER_H__