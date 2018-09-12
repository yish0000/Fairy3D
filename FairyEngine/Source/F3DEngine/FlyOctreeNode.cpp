//------------------------------------------------------------------------
// Name: FlyOctreeNode.cpp
// Desc: This file is to implement the class FlyOctreeNode.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyOctreeNode.h"

#include "FlyOctreeSceneManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyOctreeNode::FlyOctreeNode( FlySceneManager* pCreator ) : FlySceneNode(pCreator)
{
    m_pOctree = NULL;
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyOctreeNode::~FlyOctreeNode(void)
{
}


//------------------------------------------------------------------------
// Remove me and all the children.
//------------------------------------------------------------------------
void FlyOctreeNode::RemoveMeAndChildren(void)
{
    static_cast<FlyOctreeSceneManager*>(m_pCreator)->RemoveOctreeNode( this );

    // Remove all the child node.
    SceneNodeList::iterator itr;
    for( itr=m_Children.begin();itr!=m_Children.end();itr++ )
        static_cast<FlyOctreeNode*>(*itr)->RemoveMeAndChildren();
}


//------------------------------------------------------------------------
// Remove the specified child node.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the specified child node.
//------------------------------------------------------------------------
FlySceneNode* FlyOctreeNode::RemoveChild( int nIndex )
{
    FlyOctreeNode* pNode = static_cast<FlyOctreeNode*>(
        FlySceneNode::RemoveChild(nIndex));

    pNode->RemoveMeAndChildren();
    return pNode;
}


//------------------------------------------------------------------------
// Remove the specified child node.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneNode*:  Pointer to the speicified child node.
//------------------------------------------------------------------------
void FlyOctreeNode::RemoveChild( FlySceneNode* pChild )
{
    FlySceneNode::RemoveChild( pChild );

    static_cast<FlyOctreeNode*>(pChild)->RemoveMeAndChildren();
}


//------------------------------------------------------------------------
// Remove all the child nodes.
//------------------------------------------------------------------------
void FlyOctreeNode::RemoveAllChildren(void)
{
    SceneNodeList::iterator itr,end;
    end = m_Children.end();
    for( itr=m_Children.begin();itr!=end;itr++ )
    {
        FlyOctreeNode* pNode = static_cast<FlyOctreeNode*>(*itr);
        pNode->SetParentNode( NULL );
        pNode->RemoveMeAndChildren();
    }

    m_Children.clear();
    NeedUpdateBBox();
}


//------------------------------------------------------------------------
// Update the boundingBox for the scene node.
//------------------------------------------------------------------------
void FlyOctreeNode::UpdateBounds(void)
{
    m_WorldBBox.SetEmpty();

    SceneObjectList::iterator itr,end;
    end = m_SceneObjs.end();
    for( itr=m_SceneObjs.begin();itr!=end;itr++ )
    {
        m_WorldBBox.Merge( (*itr)->GetWorldBoundingBox() );
    }

    // Update the scene node.
    // If it hasn't been added to the octree, add it, and if has moved
    // enough to leave it's current node, we'll update it.
    if( !m_WorldBBox.IsEmpty() )
        static_cast<FlyOctreeSceneManager*>(m_pCreator)->UpdateOctreeNode( this );
}


//------------------------------------------------------------------------
// Check whether the scene node is in the boundingBox.
//------------------------------------------------------------------------
bool FlyOctreeNode::IsBBoxIn( const FlyAabb& bbox )
{
    FlyVector vCenter = m_WorldBBox.GetCenter();

    if( !(bbox.vcMax.x > vCenter.x &&
        bbox.vcMax.y > vCenter.y &&
        bbox.vcMax.z > vCenter.z &&
        bbox.vcMin.x < vCenter.x &&
        bbox.vcMin.y < vCenter.y &&
        bbox.vcMin.z < vCenter.z) )
        return false;

    FlyVector vOctreeSize = bbox.vcMax - bbox.vcMin;
    FlyVector vNodeSize = m_WorldBBox.vcMax - m_WorldBBox.vcMin;
    return (vNodeSize.x < vOctreeSize.x &&
            vNodeSize.y < vOctreeSize.y &&
            vNodeSize.z < vOctreeSize.z);
}


//------------------------------------------------------------------------
// Make the node and all its children visible.
//------------------------------------------------------------------------
void FlyOctreeNode::SetVisible( bool bVisible )
{
    m_bVisible = bVisible;

    SceneNodeList::iterator child;
    for( child=m_Children.begin();child!=m_Children.end();child++ )
        (*child)->SetVisible( bVisible );
}


//------------------------------------------------------------------------
// Add all the scene objects to renderQueue.
//------------------------------------------------------------------------
void FlyOctreeNode::AddObjectsToRenderQueue(void)
{
    SceneObjectList::iterator itr,end;
    end = m_SceneObjs.end();
    for( itr=m_SceneObjs.begin();itr!=end;itr++ )
    {
        (*itr)->NotifyCamera( m_pCreator->GetCurrentCamera() );

        if( (*itr)->IsVisible() )
            (*itr)->RenderObject();
    }

    // Render the box if needed.
    if( m_bShowBBox )
    {
        if( !m_pBBoxHelper )
            m_pBBoxHelper = new FlyBBoxHelper( m_WorldBBox,FLYCOLOR::White );
        else
            m_pBBoxHelper->UpdateBBox( m_WorldBBox );

        m_pBBoxHelper->AddToRenderQueue();
    }
}


//------------------------------------------------------------------------
// Which octree is the node attached on ?
//------------------------------------------------------------------------
FlyOctree* FlyOctreeNode::GetOctree(void)
{
    return m_pOctree;
}

//------------------------------------------------------------------------
// Set the octree which the node is attached on.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyOctree*:     Pointer to the octree.
//------------------------------------------------------------------------
void FlyOctreeNode::SetOctree( FlyOctree* pOctree )
{
    m_pOctree = pOctree;
}