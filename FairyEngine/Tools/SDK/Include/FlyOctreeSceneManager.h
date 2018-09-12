//------------------------------------------------------------------------
// Name: FlyOctreeSceneManager.h
// Desc: This file define a sceneManager use the octree to make the
//       object-culling and collision query more fast.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_OCTREE_SCENEMANAGER_H__
#define __FLY_OCTREE_SCENEMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlySceneManager.h"

//////////////////////////////////////////////////////////////////////////


class FlyOctree;
class FlyOctreeNode;

/*
* The sceneManager use the loose octree to manage the
* scene objects.
*/
class _FLY_EXPORT FlyOctreeSceneManager : public FlySceneManager
{
    typedef std::list<FlyOctreeNode*> OctreeNodeList;

public: enum Visibility { VSB_FULL,VSB_PARTIAL,VSB_NONE };

protected:
    FlyOctree* m_pOctreeRoot;   // Pointer to the root node of the octree.

    int m_nMaxDepth;            // Max depth of the octree.
    FlyAabb m_OctreeBox;        // BoundingBox of the octree.
    bool m_bShowBBox;           // Show boundingBox for debug ?

    void ProcessNode( FlyOctree* octree,bool bFoundVisible );

    // Render the visible objects.
    virtual void RenderVisibleObjects(void);

    // Get the specified boundingBox's visibility.
    Visibility GetVisibility( const FlyAabb& bbox );

    void FindNodes( const FlyAabb& bbox,std::list<FlySceneNode*>& nodeList,
        bool bFull,FlyOctree* pOctree );
    void FindNodes( const FlySphere& sphere,std::list<FlySceneNode*>& nodeList,
        bool bFull,FlyOctree* pOctree );
    void FindNodes( const FlyRay& ray,std::list<FlySceneNode*>& nodeList,
        FlyOctree* pOctree );

public:
    // Constructor and destructor.
    FlyOctreeSceneManager(void);
    ~FlyOctreeSceneManager(void);

    void InitScene( const FlyAabb& bbox,int depth );

    // Override the virtual function.
    virtual void ClearScene(void);
    virtual FlySceneNode* CreateSceneNode( const char* cName=NULL );
    virtual void DestroySceneNode( FlySceneNode* pNode,bool bDestroyChildren );
    virtual void DestroySceneNode( const char* cName,bool bDestroyChildren );

    void UpdateOctreeNode( FlyOctreeNode* pNode );
    void RemoveOctreeNode( FlyOctreeNode* pNode );
    void AddOctreeNode( FlyOctreeNode* pNode,FlyOctree* pOctree,int nDepth=0 );

    void ResizeOctree( const FlyAabb& bbox );

    void FindInsectedNodes( const FlyAabb& bbox,std::list<FlySceneNode*>& nodeList );
    void FindInsectedNodes( const FlySphere& sphere,std::list<FlySceneNode*>& nodeList );
    void FindInsectedNodes( const FlyRay& ray,std::list<FlySceneNode*>& nodeList );

    void SetShowBBox( bool bShow ) { m_bShowBBox = bShow; }
    bool IsShowBBox(void) { return m_bShowBBox; }
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyOctreeSceneFactory : public FlySceneFactory
{
public:
    // Constructor and destructor.
    FlyOctreeSceneFactory(void) { m_sTypename = "octree"; }
    ~FlyOctreeSceneFactory(void) { /* Nothing to do! */ };

    FlySceneManager* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneManager* pManager );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_OCTREE_SCENEMANAGER_H__