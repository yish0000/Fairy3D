//------------------------------------------------------------------------
// Name: FlyBspSceneManager.h
// Desc: This file define a specialisation of the SceneManager class.
//       Deal with the indoor scenes based on a bsp tree.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_BSP_SCENEMANAGER_H__
#define __FLY_BSP_SCENEMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlySceneManager.h"

#include "FlyBspLevel.h"
#include "FlyBspRenderable.h"
#include "FlyDynArray.h"
//////////////////////////////////////////////////////////////////////////


/*
* This class is to deal with indoor scenes based on a BSP tree.
*/
class FlyBspSceneManager : public FlySceneManager
{
    typedef FlyDynArray<FlyBspRenderable*> RenderableList;

protected:
    FlyBspLevel* m_pLevel;          // Pointer to the bsp level.
    RenderableList m_Renderables;   // Renderable object list for the level.

    // Build the renderable objects for the level.
    HRESULT BuildRenderables(void);

    // Render the world geometry.
    void RenderWorldLevel(void);

    // Process the node, Search visible geometry.
    void ProcessNode( FlyBspNode* pNode,FlyBspNode* pCamLeaf );

    // Handle a single bsp face, Send the geometry data to buffer.
    void HandleSingleFace( sBspFace* pFace );

    // Test the collision in the 3D world.
    bool TestNode( FlyBspNode* pNode,const FlyRay& ray,float fL,float& fDist );
    bool TestLeaf( FlyBspNode* pNode,const FlyRay& ray,float fL,float& fDist );

public:
    // Constructor and destructor.
    FlyBspSceneManager(void);
    ~FlyBspSceneManager(void);

    // Load the world geometry data.
    HRESULT LoadWorldGeometry( const char* cFilename );

    // Clear the scene data.
    void ClearScene(void);

    // Test the collision in the 3D world.
    bool TestCollision( const FlyRay& ray,float fL,float& fDist );
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyBspSceneFactory : public FlySceneFactory
{
public:
    // Constructor and destructor.
    FlyBspSceneFactory(void) { m_sTypename = "bsp"; }
    ~FlyBspSceneFactory(void) { /* Nothing to do! */ };

    FlySceneManager* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneManager* pManager );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_BSP_SCENEMANAGER_H__