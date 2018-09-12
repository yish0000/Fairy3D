//------------------------------------------------------------------------
// Name: FlyOctree.cpp
// Desc: This file is to implement the class FlyOctree.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyOctree.h"

#include "FlyOctreeNode.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyOctree::FlyOctree( FlyOctree* pParent )
{
    m_pParent = pParent;

    m_nNumSceneNodes = 0;
    memset( &m_BBox,0,sizeof(FlyAabb) );
    memset( &m_vHalfSize,0,sizeof(FlyVector) );

    memset( m_pChildren,0,sizeof(FlyOctree*)*8 );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyOctree::~FlyOctree(void)
{
    for( int x=0;x<2;x++ )
    {
        for( int y=0;y<2;y++ )
        {
            for( int z=0;z<2;z++ )
                SAFE_DELETE( m_pChildren[x][y][z] );
        }
    }

    m_pParent = NULL;
}


//------------------------------------------------------------------------
// Add a scene node to the octree.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyOctreeNode*:     Pointer to the scene node.
//------------------------------------------------------------------------
void FlyOctree::AddSceneNode( FlyOctreeNode* pNode )
{
    pNode->SetOctree( this );
    m_SceneNodes.push_back( pNode );

    AddNodeCount();
}


//------------------------------------------------------------------------
// Remove a scene node from the octree.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyOctreeNode*:     Pointer to the scene node.
//------------------------------------------------------------------------
void FlyOctree::RemoveSceneNode( FlyOctreeNode* pNode )
{
    OctreeNodeList::iterator itr;
    itr = std::find( m_SceneNodes.begin(),m_SceneNodes.end(),pNode );

    if( itr != m_SceneNodes.end() )
    {
        pNode->SetOctree( NULL );
        m_SceneNodes.erase( itr );
        DecNodeCount();
    }
}


//------------------------------------------------------------------------
// Check whether the box size is fit in the octree's child node.
//------------------------------------------------------------------------
bool FlyOctree::IsTwiceSize( const FlyAabb& bbox )
{
    FlyVector vHalfSize = m_BBox.GetHalfSize();
    FlyVector vBoxSize = bbox.vcMax - bbox.vcMin;
    return ((vBoxSize.x <= vHalfSize.x) &&
            (vBoxSize.y <= vHalfSize.y) &&
            (vBoxSize.z <= vHalfSize.z));
}


//------------------------------------------------------------------------
// Which child is the specified box int ?
// Because the octree is loose, So we can only check the center point.
//------------------------------------------------------------------------
void FlyOctree::GetChildIndices( const FlyAabb& bbox,int* x,int* y,int* z )
{
    FlyVector vOctCenter = m_BBox.GetCenter();
    FlyVector vBoxCenter = bbox.GetCenter();

    if( vBoxCenter.x > vOctCenter.x )
        *x = 1;
    else
        *x = 0;

    if( vBoxCenter.y > vOctCenter.y )
        *y = 1;
    else
        *y = 0;

    if( vBoxCenter.z > vOctCenter.z )
        *z = 1;
    else
        *z = 0;
}


//------------------------------------------------------------------------
// Get the boundingBox for culling.
//------------------------------------------------------------------------
void FlyOctree::GetCullBBox( FlyAabb* pBox )
{
    pBox->vcMin = m_BBox.vcMin - m_vHalfSize;
    pBox->vcMax = m_BBox.vcMax + m_vHalfSize;
}


//------------------------------------------------------------------------
// Add the count of the sceneNode.
//------------------------------------------------------------------------
void FlyOctree::AddNodeCount(void)
{
    m_nNumSceneNodes++;

    if( m_pParent )
        m_pParent->AddNodeCount();
}


//------------------------------------------------------------------------
// Dec the count of the sceneNode.
//------------------------------------------------------------------------
void FlyOctree::DecNodeCount(void)
{
    m_nNumSceneNodes--;

    if( m_pParent )
        m_pParent->DecNodeCount();
}