/*
 * -----------------------------------------------------------------------
 *  Name:   F3DSceneNode.cpp
 *  Desc:   Scene node, base class of all the scene objects.
 *  Author: Yish
 *  Date:   2011/2/2
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */

#include "F3DPCH.h"
#include "F3DSceneManager.h"
#include "F3DEngine.h"

/** Construction of the class.
@Param Name of the scene node.
@Param Pointer to the scene manager.
*/
F3DSceneNode::F3DSceneNode( const AString& name )
    : m_sName(name), m_iRef(1), m_pParent(NULL), m_bVisible(true)
    , m_bUpdateBounds(false), m_bUpdateMatrix(true), m_bShowBounds(false)
{
	m_pOwner = F3DEngine::GetInstance().GetSceneManager();
    m_Bounds = F3DAABB::BOX_EMPTY;

    m_vPosition = F3DVector3::ZERO;
    m_qOrientation = F3DQuat::IDENTITY;
    m_vScale.Set( 1.0f, 1.0f, 1.0f );

    m_vDerivedPos = F3DVector3::ZERO;
    m_qDerivedOrientation = F3DQuat::IDENTITY;
    m_vDerivedScale.Set( 1.0f, 1.0f, 1.0f );
}

/** Destructor of the class.
*/
F3DSceneNode::~F3DSceneNode(void)
{
    // Remove all the child nodes.
    RemoveAllChildren();

    // Remove me from the parent node.
    if( m_pParent )
        m_pParent->RemoveChild( this );
}

/** Set the parent node.
 */
void F3DSceneNode::SetParentNode(F3DSceneNode *pParentNode)
{
    if( m_pParent && pParentNode )
    {
        FASSERT(0);
        FLOG_ERROR("F3DSceneNode::SetParentNode, the scene node has already parent node!");
        return;
    }
    
    m_pParent = pParentNode;
    m_bUpdateMatrix = true;
    
    if( IsInScene() )
    {
        m_pOwner->RemoveDeadObject(this);
    }
    else
    {
        m_pOwner->AddObjectToDeadPool(this);
    }
}

/** Add the specified scene node as a child node.
 */
void F3DSceneNode::AddChildNode( F3DSceneNode* pNode )
{
    if( !pNode )
        return;
    
    if( pNode->GetParentNode() )
    {
        FLOG_ERRORF("F3DSceneNode::AddChildNode, the specified node(%s) has already parent!", pNode->GetName().c_str());
        return;
    }
    
    pNode->SetParentNode(this);
    m_Children.push_back(pNode);
    NeedUpdateBounds();
}

/** Attach me to the specified scene node.
*/
void F3DSceneNode::AttachToNode(F3DSceneNode* pNode)
{
	if (!pNode)
		return;

	if (m_pParent)
	{
		FLOG_ERRORF("F3DSceneNode::AttachToNode, I(%s) have already a parent node!", m_sName.c_str());
		return;
	}

	pNode->AddChildNode(this);
}

/** Remove the specified scene node by its index.
 */
F3DSceneNode* F3DSceneNode::RemoveChild( uint32 nIndex )
{
    if( nIndex >= m_Children.size() )
    {
        FLOG_ERRORF("F3DSceneNode::RemoveChild, Invalid index!");
        return NULL;
    }

    F3DSceneNode* pChild = m_Children[nIndex];
    pChild->SetParentNode(NULL);
    m_Children.erase(m_Children.begin() + nIndex);
    NeedUpdateBounds();
    return pChild;
}

/** Remove the specified scene node.
 */
F3DSceneNode* F3DSceneNode::RemoveChild( F3DSceneNode* pChild )
{
    ChildNodeList::iterator itr;
    for( itr=m_Children.begin(); itr!=m_Children.end(); itr++ )
    {
        if( (*itr) == pChild )
        {
            pChild->SetParentNode(NULL);
            m_Children.erase(itr);
            NeedUpdateBounds();
            return pChild;
        }
    }
    
    return NULL;
}

/** Remove the specified scene node by its name.
 */
F3DSceneNode* F3DSceneNode::RemoveChildByName(const AString &name)
{
    ChildNodeList::iterator it = m_Children.begin();
    for(; it!=m_Children.end(); ++it)
    {
        if( (*it)->GetName() == name )
        {
            F3DSceneNode* pChild = *it;
            (*it)->SetParentNode(NULL);
            m_Children.erase(it);
            NeedUpdateBounds();
            return pChild;
        }
    }
    
    return NULL;
}

/** Remove all the child nodes.
 */
void F3DSceneNode::RemoveAllChildren(void)
{
    ChildNodeList::iterator itr;
    for( itr=m_Children.begin(); itr!=m_Children.end(); itr++ )
        (*itr)->SetParentNode(NULL);
 
    m_Children.clear();
    NeedUpdateBounds();
}

/** Get the specified child node.
*/
F3DSceneNode* F3DSceneNode::GetChildNode(uint32 nIndex) const
{
	if (nIndex >= m_Children.size())
		return NULL;

	return m_Children[nIndex];
}

/** Get the specified child node by name.
*/
F3DSceneNode* F3DSceneNode::GetChildNodeByName(const AString& name) const
{
	ChildNodeList::const_iterator it = m_Children.begin();
	for (; it != m_Children.end(); ++it)
	{
		if ((*it)->GetName() == name)
			return *it;
	}

	return NULL;
}

/** Get the specified child node by path.
*/
F3DSceneNode* F3DSceneNode::GetChildNodeByPath(const AString& path) const
{
	if (path.empty())
		return NULL;

	AStringList strList;
	AStringUtil::Split(path, "/", strList);
	if (!strList.size())
		return NULL;

	const F3DSceneNode* pCurNode = this;
	for (AStringList::iterator it = strList.begin(); it != strList.end(); ++it)
	{
		F3DSceneNode* pChild = pCurNode->GetChildNodeByName(*it);
		if (!pChild) return NULL;
		pCurNode = pChild;
	}

	return const_cast<F3DSceneNode*>(pCurNode);
}

/** Scale the scene node.
 */
void F3DSceneNode::Scale( float x, float y, float z )
{
    m_vScale.x *= x;
    m_vScale.y *= y;
    m_vScale.z *= z;

    m_bUpdateMatrix = true;
}

/** Translate the scene node.
 @param F3DVector3, vector to translate.
 @param Use the world axis or our own axis ?
 */
void F3DSceneNode::Translate( const F3DVector3& vTrans, bool bOwnAxis/* =true */ )
{
    if( bOwnAxis )
        m_vPosition += m_qOrientation.Rotate( vTrans );
    else
        m_vPosition += vTrans;

    m_bUpdateMatrix = true;
}

/** Rotate the scene node arround with the specified axis.
 */
void F3DSceneNode::Rotate( const F3DVector3& vAxis, float fAngle )
{
    F3DQuat quat;
    quat.FromAngleAxis( vAxis,fAngle );
    m_qOrientation = quat * m_qOrientation;

    m_bUpdateMatrix = true;
}

/** Update the scene node. (Recurse to every child node)
 */
void F3DSceneNode::UpdateNode( bool bParentChanged )
{
     if( bParentChanged || m_bUpdateMatrix )
         UpdateFromParent();

	 // Update the content.
	 Update();
 
     // Update the child nodes.
     bParentChanged = bParentChanged || m_bUpdateMatrix;
 
     ChildNodeList::iterator itr = m_Children.begin();
     for( ; itr!=m_Children.end(); itr++ )
         (*itr)->UpdateNode( bParentChanged );
  
     // Update the bounding box.
     if( m_bUpdateBounds )
         UpdateBounds();
 
     m_bUpdateMatrix = false;
}

/** Update the transform matrix from the parent node.
 */
void F3DSceneNode::UpdateFromParent(void)
{
    if( m_pParent )
    {
        // Update the orientation.
        F3DQuat quat = m_pParent->GetDerivedOrientation();
        m_qDerivedOrientation = quat * m_qOrientation;

        // Update the scale.
        F3DVector3 vScale = m_pParent->GetDerivedScale();
        m_vDerivedScale.x = vScale.x * m_vScale.x;
        m_vDerivedScale.y = vScale.y * m_vScale.y;
        m_vDerivedScale.z = vScale.z * m_vScale.z;

        // Update the position.
        F3DVector3 vPosition;
        vPosition.x = m_vPosition.x * vScale.x;
        vPosition.y = m_vPosition.y * vScale.y;
        vPosition.z = m_vPosition.z * vScale.z;
        vPosition = quat.Rotate( vPosition );
        m_vDerivedPos = vPosition + m_pParent->GetDerivedPosition();
    }
    else
    {
        m_vDerivedPos = m_vPosition;
        m_qDerivedOrientation = m_qOrientation;
        m_vDerivedScale = m_vScale;
    }

    // Update the transform matrix.
    m_mWorldTransform.MakeTransform( m_vDerivedPos, m_qDerivedOrientation, m_vDerivedScale );

    // After updating the transform matrix, we need to update the bounding box.
    NeedUpdateBounds();
}

/** Update the bounding box for the scene node.
 */
void F3DSceneNode::UpdateBounds(void)
{
    // Reset the bounding box.
    m_Bounds.Zero();
	m_Bounds = CalcBoundingBox();

    // Merge all the child nodes' bounding box.
    ChildNodeList::iterator itr;
    for( itr=m_Children.begin(); itr!=m_Children.end(); itr++ )
        m_Bounds.Merge( (*itr)->GetWorldBoundingBox() );

    m_bUpdateBounds = false;
}

/** Set the flag that we must update the bounding box.
 */
void F3DSceneNode::NeedUpdateBounds(void)
{
    m_bUpdateBounds = true;
    
    if( m_pParent )
        m_pParent->NeedUpdateBounds();
}

/** Check whether the node is in the scene.
 */
bool F3DSceneNode::IsInScene() const
{
    if( !m_pParent )
        return false;
    
    if( m_pParent == m_pOwner->GetSceneRoot() )
        return true;
    else
        return m_pParent->IsInScene();
}

//------------------------------------------------------------------------
// Find the visible scene objects of this node.
//------------------------------------------------------------------------
//void F3DSceneNode::RenderVisibleObjects(void)
//{
//     if( !m_pCreator->GetFrustum()->IsBBoxVisible(m_WorldBBox) ||
//         !m_bVisible )
//         return;
// 
//     // Add all the scene object to renderQueue.
//     SceneObjectList::iterator itr;
//     for( itr=m_SceneObjs.begin();itr!=m_SceneObjs.end();itr++ )
//     {
//         // Update the scene object.
//         (*itr)->NotifyCamera( m_pCreator->GetCurrentCamera() );
// 
//         if( (*itr)->IsVisible() )
//             (*itr)->RenderObject();
//     }
// 
//     // Recurse all the child nodes.
//     SceneNodeList::iterator child;
//     for( child=m_Children.begin();child!=m_Children.end();child++ )
//     {
//         (*child)->RenderVisibleObjects();
//     }
// 
//     // Show the boundingBox if needed.
//     if( m_bShowBBox )
//     {
//         if( !m_pBBoxHelper )
//             m_pBBoxHelper = new FlyBBoxHelper( m_WorldBBox,FLYCOLOR::White );
//         else
//             m_pBBoxHelper->UpdateBBox( m_WorldBBox );
// 
//         m_pBBoxHelper->AddToRenderQueue();
//     }
//}
