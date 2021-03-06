//------------------------------------------------------------------------
// Name: FlyLightObject.h
// Desc: This file define an light object in the 3D scene.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_LIGHTOBJECT_H__
#define __FLY_LIGHTOBJECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


class FlyLightFactory;

/*
* This Class stores the dynamic light data.
*/
class _FLY_EXPORT FlyLightObject
{
    typedef std::vector<FlySceneNode*> ParentNodeList;

protected:
    FlyLightFactory* m_pCreator;    // Pointer to the factory who create me.

    std::string m_sName;        // Name of the light object.
    FLYLIGHT m_Light;           // Store the light data.
    ParentNodeList m_Parents;   // Parent nodes of the light object.

    bool m_bCastShadows;        // Does the light cast shadows ?

public:
    // Constructor and destructor.
    FlyLightObject(void);
    ~FlyLightObject(void);

    virtual void AttachToNode( FlySceneNode* pNode );
    virtual void DetachFromAllNodes(void);
    virtual void DetachFromNode( FlySceneNode* pNode );
    virtual void DetachFromNode( int nIndex );

    // Update the light object.
    virtual void UpdateObject(void) { /* Implement by user! */ };

    // Get the scene node which this object is attached.
    FlySceneNode* GetParentNode( int nIndex );
    UINT GetNumParentNodes(void);

    // Does the light cast shadows ?
    bool IsCastShadows(void) const;
    void SetCastShadows( bool bCastShadows );

    // Notify when this object is attached by a scene node.
    // User don't call this function.
    void NotifyAttached( FlySceneNode* pParentNode );
    
    // Notify when this object is detached by a scene node.
    // User don't call this function.
    void NotifyDetached( FlySceneNode* pParentNode );

    // Get and set the name.
    const char* GetName(void) const;
    void SetName( const char* cName );

    // Get the typename of the light object.
    const char* GetObjectType(void) const;

    // Get and set the creator.
    FlyLightFactory* GetCreator(void) const;
    void SetCreator( FlyLightFactory* pCreator );

    // Get the attributes.
    LIGHTTYPE GetLightType(void) const;
    const FLYCOLOR& GetDiffuseColor(void) const;
    const FLYCOLOR& GetAmbientColor(void) const;
    const FLYCOLOR& GetSpecularColor(void) const;
    const FlyVector& GetPosition(void) const;
    const FlyVector& GetDirection(void) const;
    float GetRange(void) const;
    float GetSpotTheta(void) const;
    float GetSpotPhi(void) const;
    float GetAttenuationA(void) const;
    float GetAttenuationB(void) const;

    // Set the attributes.
    void SetLightType( LIGHTTYPE Type );
    void SetDiffuseColor( const FLYCOLOR& color );
    void SetAmbientColor( const FLYCOLOR& color );
    void SetSpecularColor( const FLYCOLOR& color );
    void SetPosition( const FlyVector& vPos );
    void SetDirection( const FlyVector& vDirection );
    void SetRange( float fRange );
    void SetSpotTheta( float fTheta );
    void SetSpotPhi( float fPhi );
    void SetAttenuationA( float fAttenuation );
    void SetAttenuationB( float fAttenuation );

    FLYLIGHT* GetLightData(void);
};

typedef std::vector<FlyLightObject*> LightObjectList;

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyLightFactory
{
protected:
    std::string m_sTypename;

public:
    // Constructor and destructor.
    FlyLightFactory(void) { m_sTypename = "default"; }
    virtual ~FlyLightFactory(void) { /* Nothing to do! */ };

    virtual FlyLightObject* CreateInstance( const char* cName );
    virtual void DestroyInstance( FlyLightObject* pObject );

    const char* GetTypename(void);
    void SetTypename( const char* cTypename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_LIGHTOBJECT_H__