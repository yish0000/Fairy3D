//------------------------------------------------------------------------
// Name: FlySceneNode.h
// Desc: This file define a sceneNode to store the scene objects.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_SCENENODE_H__
#define __FLY_SCENENODE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlySceneObject.h"

#include "FlyLightObject.h"
#include "FlyRenderableHelper.h"
//////////////////////////////////////////////////////////////////////////


/*
* The class FlySceneNode is a type of Node which is 
* used to organise objects in a 3D world.
*/
class _FLY_EXPORT FlySceneNode
{
    typedef std::vector<FlySceneObject*> SceneObjectList;
    typedef std::vector<FlySceneNode*> SceneNodeList;

protected:
    std::string m_sName;            // Name of the scene node.
    FlySceneManager* m_pCreator;    // Pointer to the sceneManager.

    FlySceneNode* m_pParent;        // Pointer to the parent node.
    SceneNodeList m_Children;       // Child node list.
    bool m_bVisible;                // Is the sceneNode visible ?

    SceneObjectList m_SceneObjs;    // Scene object list.
    FlyAabb m_WorldBBox;            // Bounding box of the scene node.
    LightObjectList m_LightObjs;    // Light object list.

    FlyVector m_vPosition;          // Position of the node relative to its parent.
    FlyQuat m_qOrientation;         // Orientation of the node relative to its parent.
    FlyVector m_vScale;             // Scale of the node.

    FlyVector m_vDerivedPos;        // Position derived from the parent.
    FlyQuat m_qDerivedOrientation;  // Orientation derived from the parent.
    FlyVector m_vDerivedScale;      // Scale derived from the parent.

    bool m_bNeedUpdateBBox;         // Need to update the boundingBox.
    bool m_bTMDataChanged;          // The transform data has been changed.
    FlyMatrix m_mWorldTransform;    // To store the world transform matrix.

    bool m_bShowBBox;               // Show the boundingBox ?
    FlyBBoxHelper* m_pBBoxHelper;   // Pointer to the BBox renderable.

    // Update the boundingBox.
    virtual void UpdateBounds(void);
    virtual void UpdateFromParent(void);

    // Set the flag that we must update the BBox.
    void NeedUpdateBBox(void);

public:
    // Constructor and destructor.
    FlySceneNode( FlySceneManager* pCreator );
    virtual ~FlySceneNode(void);

    void AttachObject( FlySceneObject* pObject );
    void DetachObject( FlySceneObject* pObject );
    FlySceneObject* DetachObject( UINT nIndex );
    void DetachAllObjects(void);

    void AttachLightObject( FlyLightObject* pObject );
    void DetachLightObject( FlyLightObject* pObject );
    FlyLightObject* DetachLightObject( UINT nIndex );
    void DetachAllLights(void);
    bool IsLightAttached( FlyLightObject* pObject );

    virtual void AddChildNode( FlySceneNode* pNode );
    virtual FlySceneNode* CreateChildNode( const FlyVector& vTranslate = FlyVector::ZERO,
        const FlyQuat& qRotation = FlyQuat::IDENTITY );
    virtual FlySceneNode* CreateChildNode( const char* cName,
        const FlyVector& vTranslate = FlyVector::ZERO,
        const FlyQuat& qRotation = FlyQuat::IDENTITY );
    virtual FlySceneNode* RemoveChild( UINT nIndex );
    virtual void RemoveChild( FlySceneNode* pChild );
    virtual void RemoveAllChildren(void);

    // Get the attached scene objects.
    UINT GetNumSceneObjects(void) const;
    FlySceneObject* GetSceneObject( UINT nIndex );

    // Get the attached light objects.
    UINT GetNumLightObjects(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );

    // Get the child node.
    UINT GetNumChildren(void) const;
    FlySceneNode* GetChildNode( UINT nIndex );

    // Get the parent node.
    FlySceneNode* GetParentNode(void);
    void SetParentNode( FlySceneNode* pParentNode );

    // Get and set the name.
    const char* GetName(void) const;
    void SetName( const char* cName );

    // Get the pointer to the node's owner.
    FlySceneManager* GetCreator(void);

    // Make some transformation for the sceneNode.
    virtual void Scale( float x,float y,float z );
    virtual void Translate( const FlyVector& vTrans,bool bOwnAxis=true );
    virtual void Pitch( float fAngle );
    virtual void Yaw( float fAngle );
    virtual void Roll( float fAngle );
    virtual void Rotate( const FlyVector& vAxis,float fAngle );

    const FlyVector& GetPosition(void) const;
    const FlyQuat& GetOrientation(void) const;
    const FlyVector& GetScale(void) const;

    const FlyVector& GetDerivedPosition(void) const;
    const FlyQuat& GetDerivedOrientation(void) const;
    const FlyVector& GetDerivedScale(void) const;

    void SetPosition( const FlyVector& vPosition );
    void SetOrientation( const FlyQuat& qOrientation );
    void SetScale( const FlyVector& vScale );

    bool IsShowBBox(void) const;
    void ShowBoundingBox( bool bShowBBox );

    bool IsVisible(void) const { return m_bVisible; }
    virtual void SetVisible( bool bVisible ) { m_bVisible = bVisible; }

    // Get the world matrix for this node.
    const FlyMatrix& GetWorldTransform(void) const;

    // Get the world boundingBox.
    const FlyAabb& GetWorldBoundingBox(void) const;

    // Find the visible objects of this node.
    virtual void RenderVisibleObjects(void);

    // Update the scene node.
    virtual void UpdateNode( bool bParentChanged );

    // Find the light for the scene object.
    virtual void FindLights( LightObjectList& lightList );

    // Need to update the light objects.
    void NeedUpdateLights(void);

    static UINT s_nCurrentNodeCount;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SCENENODE_H__