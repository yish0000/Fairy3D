//------------------------------------------------------------------------
// Name: FlySceneObject.h
// Desc: This file define a base class which act as a scene object int
//       the 3D world.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_SCENEOBJECT_H__
#define __FLY_SCENEOBJECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyLightObject.h"
//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlySceneObject
{
protected:
    FlyEntityFactory* m_pCreator;   // Pointer to the factory who create me.

    std::string m_sName;        // Name of the object.
    FlySceneNode* m_pParent;    // Pointer to the parent node.

    FlyAabb m_WorldBBox;        // World bounding box of the object.

    bool m_bShelter;            // Is the scene object a shelter ?
    bool m_bVisible;            // Is the scene object visible ?
    bool m_bHaveShadows;        // Does the scene object have shadows ?

    float m_fSqrDistToCamera;   // Square Distance to the camera.
    float m_fBeyondDist;        // Distance at which the object is no longer rendered.
    bool m_bBeyondDist;         // Is the scene object beyond the 

    bool m_bOpenLight;          // Open the light for the object.
    LightObjectList m_Lights;   // Light list for this scene object.
    bool m_bManualLight;        // Set the light list manually.
    bool m_bNeedUpdateLights;   // Need update the light list.

    // Update the light for the scene object.
    virtual void UpdateLights(void);

public:
    // Constructor and destructor.
    FlySceneObject(void);
    virtual ~FlySceneObject(void);

    virtual void AttachToNode( FlySceneNode* pNode );
    virtual void DetachFromNode(void);

    virtual void RenderObject(void) = 0;
    virtual void UpdateObject(void) = 0;
    virtual void NotifyCamera( FlyCameraBase* pCamera );

    // Get the world transform matrix.
    virtual const FlyMatrix& GetWorldTransform(void) const;

    // Get the bounding box in the local axis.
    virtual const FlyAabb& GetLocalBoundingBox(void) const;

    // Get the bounding box in the 3D world.
    virtual const FlyAabb& GetWorldBoundingBox(void) const;

    // Get the bounding sphere's radius.
    virtual float GetWorldBoundingRadius(void) const;

    FlySceneNode* GetParentNode(void) const;
    void SetParentNode( FlySceneNode* pParentNode );

    const char* GetName(void) const;
    void SetName( const char* cName );

    // Get the object type.
    const char* GetObjectType(void) const;

    float GetBeyondDist(void) const;
    float GetSqrBeyondDist(void) const;
    void SetBeyondDist( float fDistance );

    float GetCameraDist(void) const;
    float GetSqrCameraDist(void) const;

    void SetShelter( bool bShelter );
    void SetVisible( bool bVisible );
    void SetHaveShadow( bool bShadow );

    bool IsAttached(void) const;
    bool IsVisible(void) const;
    bool IsShelterObject(void) const;
    bool IsHaveShadows(void) const;

    // Get the light objects.
    UINT GetNumLights(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );

    // Need update the light list.
    void NeedUpdateLightList(void);

    virtual void SetLightingMode( bool bLighting );
    bool IsLighting(void) const;

    // Set the light list manually.
    virtual void SetLightList( const LightObjectList& lightList );
    void SetManualLight( bool bManual );

    FlyEntityFactory* GetCreator(void) const;
    void SetCreator( FlyEntityFactory* pFactory );
};

//////////////////////////////////////////////////////////////////////////

/*
* Base class to create and destroy the entity.
*/
class _FLY_EXPORT FlyEntityFactory
{
protected:
    std::string m_sTypename;

public:
    // Constructor and destructor.
    FlyEntityFactory(void) { /* Nothing to do! */ };
    virtual ~FlyEntityFactory(void) { /* Nothing to do! */ };

    virtual FlySceneObject* CreateInstance( const char* cName ) { return NULL; }
    virtual void DestroyInstance( FlySceneObject* pObject ) { /* Nothing to do! */ };

    const char* GetTypename(void) const;
    void SetTypename( const char* cTypename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SCENEOBJECT_H__