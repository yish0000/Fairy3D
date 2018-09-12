/*
 * ------------------------------------------------------------------------
 *  Name:   F3DLightObject.h
 *  Desc:   Light object in the scene.
 *  Author: Yish
 *  Date:   2011/1/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_LIGHTOBJECT_H__
#define __F3D_LIGHTOBJECT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DSceneObject.h"

///////////////////////////////////////////////////////////////////////////

/** Light object in the scene.
*/
class FAIRY_API F3DLightObject : public F3DSceneObject
{
protected:
    SLightDesc m_Light;         // Light parameter.
    bool m_bCastShadow;         // Does the light cast shadow ?
    bool m_bActive;             // Is the light active ?

public:
    F3DLightObject( const AString& name );
    virtual ~F3DLightObject();

    // Does the light cast shadow ?
    bool IsCastShadow(void) const { return m_bCastShadow; }
    // Set the flag which the light will cast shadow.
    void SetCastShadow( bool bCastShadow ) { m_bCastShadow = bCastShadow; }

    // Is the light active ?
    bool IsActive() const { return m_bActive; }
    // Activate the light object.
    void SetActive( bool bActive ) { m_bActive = bActive; }

    // Get the light parameter.
    ELightType GetLightType(void) const { return m_Light.Type; }
    const F3DColor& GetDiffuseColor(void) const { return m_Light.cDiffuse; }
    const F3DColor& GetAmbientColor(void) const { return m_Light.cAmbient; }
    const F3DColor& GetSpecularColor(void) const { return m_Light.cSpecular; }
    const F3DVector3& GetPosition(void) const { return m_Light.vPosition; }
    const F3DVector3& GetDirection(void) const { return m_Light.vDirection; }
    float GetRange(void) const { return m_Light.fRange; }
    float GetSpotTheta(void) const { return m_Light.fTheta; }
    float GetSpotPhi(void) const { return m_Light.fPhi; }
    float GetAttenuationA(void) const { return m_Light.fAttenuation0; }
    float GetAttenuationB(void) const { return m_Light.fAttenuation1; }

    // Set the light parameter.
    void SetLightType( ELightType Type ) { m_Light.Type = Type; }
    void SetDiffuseColor( const F3DColor& color ) { m_Light.cDiffuse = color; }
    void SetAmbientColor( const F3DColor& color ) { m_Light.cAmbient = color; }
    void SetSpecularColor( const F3DColor& color ) { m_Light.cSpecular = color; }
    void SetPosition( const F3DVector3& vPos ) { m_Light.vPosition = vPos; }
    void SetDirection( const F3DVector3& vDirection ) { m_Light.vDirection = vDirection; }
    void SetRange( float fRange ) { m_Light.fRange = fRange; }
    void SetSpotTheta( float fTheta ) { m_Light.fTheta = fTheta; }
    void SetSpotPhi( float fPhi ) { m_Light.fPhi = fPhi; }
    void SetAttenuationA( float fAttenuation ) { m_Light.fAttenuation0 = fAttenuation; }
    void SetAttenuationB( float fAttenuation ) { m_Light.fAttenuation1 = fAttenuation; }

    // Get the light parameter.
    const SLightDesc* GetLightData(void) const { return &m_Light; }
};

// Light object list.
typedef std::vector<F3DLightObject*> F3DLightList;

/** Factory of the light object.
*/
class FAIRY_API F3DLightObjectFactory : public F3DSceneObjectFactory
{
public:
	F3DLightObjectFactory();

	// Create a scene object.
	virtual F3DSceneObject* CreateInstance(const AString& name);
	// Destroy the specified scene object.
	virtual void DestroyInstance(F3DSceneObject* pObj);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_LIGHTOBJECT_H__