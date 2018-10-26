/*
 * -----------------------------------------------------------------------
 *  Name:   F3DSceneNode.h
 *  Desc:   Scene node, base class of all the scene objects.
 *  Author: Yish
 *  Date:   2011/2/1
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */

#ifndef __F3D_SCENENODE_H__
#define __F3D_SCENENODE_H__

//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FObject.h"
#include "F3DTypes.h"

//////////////////////////////////////////////////////////////////////////

class F3DSceneManager;

/** Scene Node.
@remarks
	Base class of all type of scene objects.
@note
*/
class FAIRY_API F3DSceneNode : public FObject
{
	F_DECLARE_RTTI(F3DSceneNode)

    typedef std::vector<F3DSceneNode*> ChildNodeList;
    
protected:
    AString m_sName;                // Name of the scene node.
    F3DSceneManager* m_pOwner;      // Pointer to the scene manager.
	int m_iRef;						// Reference count of the node.

    F3DSceneNode* m_pParent;        // Parent node.
    ChildNodeList m_Children;       // Child node list.
    bool m_bVisible;                // Is this node visible ?

    F3DAABB m_Bounds;               // Bounding box of the node.
    bool m_bShowBounds;             // Do we show the bounding box ?
    
    F3DVector3 m_vPosition;         // Position relative to the parent node.
    F3DQuat m_qOrientation;         // Orientation relative to the parent node.
    F3DVector3 m_vScale;            // Scale relative to the parent node.

    F3DVector3 m_vDerivedPos;       // Position derived from the parent node.
    F3DQuat m_qDerivedOrientation;  // Orientation derived from the parent node.
    F3DVector3 m_vDerivedScale;     // Scane derived from the parent.

    bool m_bUpdateBounds;           // Do we need to update the bounding box ?
    bool m_bUpdateMatrix;           // Do we need to update the matrix.
    F3DMatrix4 m_mWorldTransform;   // Transform matrix in the world.

public:
    F3DSceneNode( const AString& name );
    virtual ~F3DSceneNode(void);

	// Reference count of the node.
	virtual void AddRef() { m_iRef++; }
	virtual void Release() { m_iRef--; }
	virtual int GetRefCount() const { return m_iRef; }
    
	// Update the content.
	virtual void Update() = 0;
    // Update the node.
    virtual void UpdateNode( bool bParentChanged );

    // Add the specified node to me.
    virtual void AddChildNode( F3DSceneNode* pNode );
	// Attach me to a specified node.
	virtual void AttachToNode(F3DSceneNode* pNode);

    // Remove the specified child node.
    virtual F3DSceneNode* RemoveChild( uint32 nIndex );
    virtual F3DSceneNode* RemoveChild( F3DSceneNode* pChild );
    virtual F3DSceneNode* RemoveChildByName(const AString& name);

    // Remove all the child nodes.
    virtual void RemoveAllChildren(void);

    // Get the count of child node.
    size_t GetNumChildren(void) const { return m_Children.size(); }

    // Get the specified child node.
    F3DSceneNode* GetChildNode( uint32 nIndex ) const;
    F3DSceneNode* GetChildNodeByName(const AString& name) const;
    F3DSceneNode* GetChildNodeByPath(const AString& path) const;
    
    // Get the parent node.
    F3DSceneNode* GetParentNode(void) const { return m_pParent; }
    // Set the parent node.
    virtual void SetParentNode( F3DSceneNode* pParentNode );

    // Translate the scene node.
    virtual void Translate( const F3DVector3& vTrans, bool bOwnAxis = true );

    // Rotate the scene node arround x axis.
    virtual void Pitch( float fAngle ) { Rotate(F3DVector3(1.0f, 0.0f, 0.0f), fAngle); }
    // Rotate the scene node arround y axis.
    virtual void Yaw( float fAngle ) { Rotate(F3DVector3(0.0f, 1.0f, 0.0f), fAngle); }
    // Rotate the scene node arround z axis.
    virtual void Roll( float fAngle ) { Rotate(F3DVector3(0.0f, 0.0f, 1.0f), fAngle); }

    // Rotate the scene arround specified axis.
    virtual void Rotate( const F3DVector3& vAxis, float fAngle );

    // Scale of the scene node.
    virtual void Scale( float x, float y, float z );

    // Get the position relative to the parent node.
    const F3DVector3& GetPosition(void) const { return m_vPosition; }
    // Get the orientation relative to the parent node.
    const F3DQuat& GetOrientation(void) const { return m_qOrientation; }
    // Get the scale relative to the parent node.
    const F3DVector3& GetScale(void) const { return m_vScale;  }

    // Get the Position derived from the parent node.
    const F3DVector3& GetDerivedPosition(void) const { return m_vDerivedPos; }
    // Get the orientation derived from the parent node.
    const F3DQuat& GetDerivedOrientation(void) const { return m_qDerivedOrientation; }
    // Get the scale derived from the parent node.
    const F3DVector3& GetDerivedScale(void) const { return m_vDerivedScale; }

    // Set the position relative to the parent node.
    void SetPosition( const F3DVector3& vPosition ) { m_vPosition = vPosition; }
    // Set the orientation relative to the parent node.
    void SetOrientation( const F3DQuat& qOrientation ) { m_qOrientation = qOrientation; }
    // Set the scale relative to the parent node.
    void SetScale( const F3DVector3& vScale ) { m_vScale = vScale; }

    // Do we show the bounding box ?
    bool IsShowBoundingBox(void) const { return m_bShowBounds; }
    // Show or hide the bounding box.
    void ShowBoundingBox( bool bShowBox ) { m_bShowBounds = bShowBox; }

    // Is this scene node visible ?
    virtual bool IsVisible(void) const { return m_bVisible; }
    // Set the visible flag of the scene node.
    virtual void SetVisible( bool bVisible ) { m_bVisible = bVisible; }

    // Get the world transform matrix.
	const F3DMatrix4& GetWorldTransform(void) const { return m_mWorldTransform; }

    // Get the bounding box in the world transform.
	const F3DAABB& GetWorldBoundingBox(void) const { return m_Bounds; }

    // Get the name of the scene node.
    const AString& GetName(void) const { return m_sName; }
    
    // Is the scene node in the scene ?
    bool IsInScene() const;

    // Get the pointer to the scene manager.
    F3DSceneManager* GetSceneManager(void) const { return m_pOwner; }

protected:
	// Update the bounding box.
	virtual void UpdateBounds(void);
	// Calculate the bounding box of the node (Without the child node).
	virtual F3DAABB CalcBoundingBox(void) = 0;
	// Update the transform matrix from the parent.
	virtual void UpdateFromParent(void);
	// We need to update the bounding box.
    virtual void NeedUpdateBounds(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SCENENODE_H__