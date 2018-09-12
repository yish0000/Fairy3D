//------------------------------------------------------------------------
// Name: FlyOctreeNode.h
// Desc: This file define a structure used in the octreeSceneManager.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_OCTREE_NODE_H__
#define __FLY_OCTREE_NODE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlySceneNode.h"

//////////////////////////////////////////////////////////////////////////


class FlyOctree;

class _FLY_EXPORT FlyOctreeNode : public FlySceneNode
{
protected:
    FlyOctree* m_pOctree;   // Owner of the octree node.

    // Update the boundingBox for the sceneNode.
    void UpdateBounds(void);

    void RemoveMeAndChildren(void);

public:
    // Constructor and destructor.
    FlyOctreeNode( FlySceneManager* pCreator );
    ~FlyOctreeNode(void);

    // Add all the scene objects to renderQueue.
    void AddObjectsToRenderQueue(void);

    FlySceneNode* RemoveChild( int nIndex );
    void RemoveChild( FlySceneNode* pChild );
    void RemoveAllChildren(void);

    FlyOctree* GetOctree(void);
    void SetOctree( FlyOctree* pOctree );

    bool IsBBoxIn( const FlyAabb& bbox );
    void SetVisible( bool bVisible );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_OCTREE_NODE_H__