/*
 * ------------------------------------------------------------------------
 *  Name:   F3DCameraManager.h
 *  Desc:   This file define a camera manager for the engine.
 *  Author: Yish
 *  Date:   2011/1/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_CAMERAMANAGER_H__
#define __F3D_CAMERAMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DCamera.h"

///////////////////////////////////////////////////////////////////////////


/** Camera manager.
*/
class FAIRY_API F3DCameraManager : public FGeneralAlloc
{
public:

	typedef std::map<AString, F3DCameraBase*> CameraMap;

protected:
    CameraMap m_Cameras;	// Camera object map.

public:
    ~F3DCameraManager();

	// Update the the cameras in each frame.
	void Update();

    // Create a camera object.
    F3DCameraBase* CreateCamera(const AString& name, EProjectionMode projMode = PJM_PERSPECTIVE);

    // Destroy the specified camera.
    void RemoveCameraByName(const AString& name);
    void RemoveCamera(F3DCameraBase* pCamera);

    // Get the specified camera object.
    F3DCameraBase* GetCameraByName(const AString& name);

    // Get the count of camera objects.
    size_t GetNumCameras() const { return m_Cameras.size(); }

    // Get the singleton object.
    static F3DCameraManager& GetInstance();

private:
	F3DCameraManager() {}
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_CAMERAMANAGER_H__