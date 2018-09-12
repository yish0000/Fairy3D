//------------------------------------------------------------------------
// Name: FlyBspNode.h
// Desc: This file define a class which act as the node in the bsp tree.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_BSPNODE_H__
#define __FLY_BSPNODE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyQuake3Types.h"

//////////////////////////////////////////////////////////////////////////


/*
* This class Encapsulates a node in a BSP tree.
* A BSP tree represents space partitioned by planes.
*/
class _FLY_EXPORT FlyBspNode
{
    friend class FlyBspLevel;
    friend class FlyBspSceneManager;

protected:
    FlyBspLevel* m_pOwner;  // Owner create this node.
    FlyBspNode* m_pFront;   // Front child of this node.
    FlyBspNode* m_pBack;    // Back child of this node.

    FlyPlane m_Splitter;    // Splitter plane of this node.
    int m_nFaceStart;       // Start index of the face.
    int m_nNumFaces;        // This node's number of faces.
    FlyAabb m_WorldBBox;    // BoundingBox of this node.

    NodeBrushList m_SolidBrushes;

    // This member is just for the leaf node.
    // It is used for the PVS, visible test.
    int m_nVisCluster;

public:
    // Default Constructor.
    FlyBspNode(void);

    // Constructor and destructor.
    FlyBspNode( FlyBspLevel* pOwner );
    ~FlyBspNode(void);

    // Is the node a leaf ?
    bool IsLeaf(void) const;

    // Get the front child node. If call this on a leaf,
    // return NULL.
    FlyBspNode* GetFront(void) const;

    // Get the back child node. If call this on a leaf.
    // return NULL.
    FlyBspNode* GetBack(void) const;

    // Set the child node.
    void SetFront( FlyBspNode* pNode );
    void SetBack( FlyBspNode* pNode );
    void SetOwner( FlyBspLevel* pLevel ) { m_pOwner = pLevel; }

    // Get the side of a world point to the splitter plane.
    int GetPointSide( const FlyVector& vPos );

    // Get the distance of a point to the splitter plane.
    float GetDistance( const FlyVector& vPos );

    // This function must be called by a splitting node.
    // Get the next node down the tree with the intention of
    // locating the leaf which contain the given point.
    FlyBspNode* GetNextNode( const FlyVector& vPos );

    // Get the splitter plane of this node.
    const FlyPlane& GetSplitter(void) const;

    // Get the boundingBox for this node.
    const FlyAabb& GetBoundingBox(void) const;

    // Get the start face index of this node.
    int GetFaceGroupStart(void) const;

    // Get the number of face groups.
    int GetNumFaceGroup(void) const;

    // Get the list of brushes, must be called by a 
    // leaf node.
    const NodeBrushList& GetSolidBrushes(void) const;

    // Check whether the leaf is visible.
    bool IsLeafVisible( const FlyBspNode* pLeaf ) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_BSPNODE_H__