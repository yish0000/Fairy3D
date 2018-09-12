/*
 * ------------------------------------------------------------------------
 *  Name:   F3DLightObject.cpp
 *  Desc:   Light object in the scene.
 *  Author: Yish
 *  Date:   2011/1/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DLightObject.h"

/** Constructor of the class.
*/
F3DLightObject::F3DLightObject( const AString& name ) : F3DSceneObject(name, "light")
{
    memset(&m_Light, 0, sizeof(SLightDesc));
}

/** Destructor of the class.
*/
F3DLightObject::~F3DLightObject(void)
{
}

///////////////////////////////////////////////////////////////////////////
//  
//  class F3DLightObjectFactory
//  
///////////////////////////////////////////////////////////////////////////

F3DLightObjectFactory::F3DLightObjectFactory()
{
	m_sType = "light";
}

F3DSceneObject* F3DLightObjectFactory::CreateInstance(const AString& name)
{
	return new F3DLightObject(name);
}

void F3DLightObjectFactory::DestroyInstance(F3DSceneObject* pObj)
{
	F_SAFE_DELETE(pObj);
}