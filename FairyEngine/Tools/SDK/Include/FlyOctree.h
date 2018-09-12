//------------------------------------------------------------------------
// Name: FlyOctree.h
// Desc: This file define an octree structure to manage the scene nodes.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_OCTREE_H__
#define __FLY_OCTREE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


class FlyOctreeNode;

/*
* An octree structure to manage the scene nodes.
*/
class _FLY_EXPORT FlyOctree
{
    friend class FlyOctreeSceneManager;

    typedef std::list<FlyOctreeNode*> OctreeNodeList;

protected:
    FlyOctree* m_pParent;               // Pointer to the parent tree.
    UINT m_nNumSceneNodes;              // Count of the sceneNodes.
    FlyAabb m_BBox;                     // BoundingBox of the tree.
    FlyVector m_vHalfSize;              // Half size of the tree.

    FlyOctree* m_pChildren[2][2][2];    // Child tree array.

    // To store the scene nodes attached on this octree.
    OctreeNodeList m_SceneNodes;

    void AddNodeCount(void);
    void DecNodeCount(void);

public:
    // Constructor and destructor.
    FlyOctree( FlyOctree* pParent );
    ~FlyOctree(void);

    void AddSceneNode( FlyOctreeNode* pNode );
    void RemoveSceneNode( FlyOctreeNode* pNode );
    UINT GetNumSceneNodes(void);

    bool IsTwiceSize( const FlyAabb& bbox );
    void GetChildIndices( const FlyAabb& bbox,int* x,int* y,int* z );
    void GetCullBBox( FlyAabb* pBox );

    FlyOctree* GetParent(void);
    void SetParent( FlyOctree* pOctree );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_OCTREE_H__