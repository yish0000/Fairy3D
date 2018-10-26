/*
 * -----------------------------------------------------------------------
 *  Name:   F3DSceneObject.h
 *  Desc:   Base class of all the scene object.
 *  Author: Yish
 *  Date:   2011/1/20
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */

#ifndef __F3D_SCENEOBJECT_H__
#define __F3D_SCENEOBJECT_H__

//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "F3DSceneNode.h"

//////////////////////////////////////////////////////////////////////////

/** Scene object
@remarks
    Base class of all type of scene objects.
*/
class FAIRY_API F3DSceneObject : public F3DSceneNode
{
	F_DECLARE_CREATE(F3DSceneObject)

public:
	F3DSceneObject();
    F3DSceneObject( const AString& name, const AString& type );
    virtual ~F3DSceneObject(void);

	// Initialize the scene object.
	virtual bool Init(void* pParam);
    // Update the scene object.
	virtual void Update();

	// Calculate the bounding box of the scene object.
	virtual F3DAABB CalcBoundingBox(void);

	// Get the class id of the scene object.
	const AString& GetClassID() const { return m_sClassID; }
	// Is this object initialized already ?
	bool IsInitAlready() const { return m_bInitAlready; }

protected:
	AString m_sClassID;				// ClassID of the scene object.
	volatile bool m_bInitAlready;	// Has already initialized successfully, we can show it in the scene.
};

//////////////////////////////////////////////////////////////////////////

/** Factory of the scene object.
*/
class FAIRY_API F3DSceneObjectFactory
{
protected:
    AString m_sType;    // Type of the scene object factory.

public:
    F3DSceneObjectFactory();
    virtual ~F3DSceneObjectFactory() {};

    // Create a scene object.
	virtual F3DSceneObject* CreateInstance(const AString& name);
    // Destroy the specified scene object.
	virtual void DestroyInstance(F3DSceneObject* pObj);

    // Get the type of the factory.
    const AString& GetType(void) const { return m_sType; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SCENEOBJECT_H__