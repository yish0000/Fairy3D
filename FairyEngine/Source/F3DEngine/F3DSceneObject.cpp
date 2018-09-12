/*
 * -----------------------------------------------------------------------
 *  Name:   F3DSceneObject.cpp
 *  Desc:   Base class of all types of scene objects.
 *  Author: Yish
 *  Date:   2011/1/28
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DSceneNode.h"
#include "F3DSceneObject.h"
#include "F3DSceneManager.h"
#include "F3DCamera.h"
#include "F3DEngine.h"

/** Constructor.
@Param Name of the scene object.
*/
F3DSceneObject::F3DSceneObject( const AString& name, const AString& type )
	: F3DSceneNode(name), m_sClassID(type)
{
}

/** Destructor.
*/
F3DSceneObject::~F3DSceneObject(void)
{
}

/** Initialize the scene object.
*/
bool F3DSceneObject::Init(void* pParam)
{
	return true;
}

/** Update the scene object.
*/
void F3DSceneObject::Update()
{
}

/** Calculate the bounding box of this object.
*/
F3DAABB F3DSceneObject::CalcBoundingBox()
{
	return F3DAABB::BOX_INFINITE;
}

///////////////////////////////////////////////////////////////////////////
//  
//  class F3DSceneObjectFactory
//  
///////////////////////////////////////////////////////////////////////////

F3DSceneObjectFactory::F3DSceneObjectFactory()
{
    m_sType = "object";
}

F3DSceneObject* F3DSceneObjectFactory::CreateInstance(const AString& name)
{
	return new F3DSceneObject(name, m_sType);
}

void F3DSceneObjectFactory::DestroyInstance(F3DSceneObject* pObj)
{
	F_SAFE_DELETE(pObj);
}