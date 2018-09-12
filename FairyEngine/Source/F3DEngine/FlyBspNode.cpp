//------------------------------------------------------------------------
// Name: FlyBspNode.cpp
// Desc: This file is to implement the class FlyBspNode.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyBspNode.h"

#include "FlyBspLevel.h"


//------------------------------------------------------------------------
// Default constructor of the class.
//------------------------------------------------------------------------
FlyBspNode::FlyBspNode(void)
{
    m_pOwner = NULL;
    m_pFront = NULL;
    m_pBack = NULL;
    m_nFaceStart = -1;
    m_nNumFaces = 0;
    m_nVisCluster = -1;

    memset( &m_Splitter,0,sizeof(FlyPlane) );
    memset( &m_WorldBBox,0,sizeof(FlyAabb) );
}


//------------------------------------------------------------------------
// Constructor of the class.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyBspLevel*:   Pointer to the level create the node.
//------------------------------------------------------------------------
FlyBspNode::FlyBspNode( FlyBspLevel* pOwner )
{
    m_pOwner = pOwner;
    m_pFront = NULL;
    m_pBack = NULL;
    m_nFaceStart = -1;
    m_nNumFaces = 0;
    m_nVisCluster = -1;

    memset( &m_Splitter,0,sizeof(FlyPlane) );
    memset( &m_WorldBBox,0,sizeof(FlyAabb) );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyBspNode::~FlyBspNode(void)
{
    SAFE_DELETE( m_pFront );
    SAFE_DELETE( m_pBack );
}


//------------------------------------------------------------------------
// Is the node a leaf ?
//------------------------------------------------------------------------
bool FlyBspNode::IsLeaf(void) const
{
    return ((m_pFront==NULL) && (m_pBack==NULL));
}


//------------------------------------------------------------------------
// Get the front child node of this node.
//------------------------------------------------------------------------
FlyBspNode* FlyBspNode::GetFront(void) const
{
    return m_pFront;
}


//------------------------------------------------------------------------
// Get the back child node of this node.
//------------------------------------------------------------------------
FlyBspNode* FlyBspNode::GetBack(void) const
{
    return m_pBack;
}


//------------------------------------------------------------------------
// Get the splitting plane for this node, not for a leaf.
//------------------------------------------------------------------------
const FlyPlane& FlyBspNode::GetSplitter(void) const
{
    return m_Splitter;
}


//------------------------------------------------------------------------
// Get the bounding Box for this bsp node.
//------------------------------------------------------------------------
const FlyAabb& FlyBspNode::GetBoundingBox(void) const
{
    return m_WorldBBox;
}


//------------------------------------------------------------------------
// Get the start index of the faces of the bsp node.
//------------------------------------------------------------------------
int FlyBspNode::GetFaceGroupStart(void) const
{
    return m_nFaceStart;
}


//------------------------------------------------------------------------
// Get the number of faces of this bsp node.
//------------------------------------------------------------------------
int FlyBspNode::GetNumFaceGroup(void) const
{
    return m_nNumFaces;
}


//------------------------------------------------------------------------
// Check whether the given leaf can be seen in this leaf.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyBspNode*:  Pointer to the leaf node.
//------------------------------------------------------------------------
bool FlyBspNode::IsLeafVisible( const FlyBspNode* pLeaf ) const
{
    return m_pOwner->IsLeafVisible( this,pLeaf );
}


//------------------------------------------------------------------------
// Get the side for the given point on the splitter plane.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   The given point.
//------------------------------------------------------------------------
int FlyBspNode::GetPointSide( const FlyVector& vPos )
{
    // If node is a leaf.
    if( m_pFront == NULL && m_pBack == NULL )
        return -1;

    return m_Splitter.Classify( vPos );
}


//------------------------------------------------------------------------
// Get the distance for the point to the splitter plane.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   The given point.
//------------------------------------------------------------------------
float FlyBspNode::GetDistance( const FlyVector& vPos )
{
    // If node is a leaf.
    if( m_pFront == NULL && m_pBack == NULL )
        return -1;

    return m_Splitter.Distance( vPos );
}


//------------------------------------------------------------------------
// Get the child node, which contain the given point.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   The given point.
//------------------------------------------------------------------------
FlyBspNode* FlyBspNode::GetNextNode( const FlyVector& vPos )
{
    int nSide = GetPointSide( vPos );

    if( nSide == FLY_FRONT )
        return m_pFront;
    else
        return m_pBack;
}


//------------------------------------------------------------------------
// Set the front child node.
//------------------------------------------------------------------------
void FlyBspNode::SetFront( FlyBspNode* pNode )
{
    m_pFront = pNode;
}


//------------------------------------------------------------------------
// Set the back child node.
//------------------------------------------------------------------------
void FlyBspNode::SetBack( FlyBspNode* pNode )
{
    m_pBack = pNode;
}


//------------------------------------------------------------------------
// Get the brush list for the node.
//------------------------------------------------------------------------
const NodeBrushList& FlyBspNode::GetSolidBrushes(void) const
{
    return m_SolidBrushes;
}