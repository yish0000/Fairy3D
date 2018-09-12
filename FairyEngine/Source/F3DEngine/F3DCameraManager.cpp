/*
 * -----------------------------------------------------------------------
 *  Name:   F3DCameraManager.cpp
 *  Desc:   This file define a camera manager for the engine.
 *  Author: Yish
 *  Date:   2011/1/18
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DCameraManager.h"


/** Get the instance of camera manager.
*/
F3DCameraManager& F3DCameraManager::GetInstance()
{
	static F3DCameraManager obj;
	return obj;
}

/** Destructor
*/
F3DCameraManager::~F3DCameraManager(void)
{
    CameraMap::iterator it;
    for( it=m_Cameras.begin(); it!=m_Cameras.end(); it++ )
    {
        F_SAFE_DELETE( it->second );
    }
}

/** Create a new camera.
@param: name of the camera.
@param: is a perspective camera ?
*/
F3DCameraBase* F3DCameraManager::CreateCamera(const AString& name, EProjectionMode projMode /* = PJM_PERSPECTIVE */)
{
    CameraMap::iterator it = m_Cameras.find( name );

    if( it != m_Cameras.end() )
    {
        // The camera existed!
        return it->second;
    }
    else
    {
		F3DCameraBase* pCamera = NULL;

		// Create a new camera.
		if( projMode == PJM_PERSPECTIVE )
			pCamera = new F3DCamera(name);
		else if( projMode == PJM_ORTHOGONAL )
			pCamera = new F3DCameraOrtho(name);
		else
		{
			FASSERT(!"Unknown projection mode!");
			return NULL;
		}

		m_Cameras.insert( CameraMap::value_type(name, pCamera) );
		return pCamera;
    }
}

/** Update the cameras.
*/
void F3DCameraManager::Update()
{
	CameraMap::iterator it = m_Cameras.begin();
	for(; it != m_Cameras.end(); ++it)
		it->second->Update();
}

/** Destroy the specified camera.
@param Name of the camera.
*/
void F3DCameraManager::RemoveCameraByName(const AString& name)
{
    CameraMap::iterator it = m_Cameras.find(name);

    if( it != m_Cameras.end() )
    {
        F_SAFE_DELETE( it->second );
        m_Cameras.erase( it );
    }
}

/** Destroy the specified camera.
@param Pointer to the camera.
*/
void F3DCameraManager::RemoveCamera(F3DCameraBase* pCamera)
{
    CameraMap::iterator it;
    for( it=m_Cameras.begin(); it!=m_Cameras.end(); ++it )
    {
        if( it->second == pCamera )
        {
            F_SAFE_DELETE( pCamera );
            m_Cameras.erase( it );
            break;
        }
    }
}

/** Get the specified camera by name.
*/
F3DCameraBase* F3DCameraManager::GetCameraByName(const AString& name)
{
    CameraMap::iterator it = m_Cameras.find(name);
    return (it != m_Cameras.end()) ? it->second : NULL;
}