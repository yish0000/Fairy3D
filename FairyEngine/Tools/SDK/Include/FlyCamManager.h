//------------------------------------------------------------------------
// Name: FlyCamManager.h
// Desc: This file define a manager class for the cameras.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_CAM_MANAGER_H__
#define __FLY_CAM_MANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyCameraBase.h"
//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyCamManager
{
    typedef std::vector<FlyCameraBase*> CameraList;

protected:
    CameraList m_Cameras;       // A list of camera object.

public:
    // Constructor and destructor
    FlyCamManager(void);
    ~FlyCamManager(void);

    // Create a free camera into the camList
    // and return its pointer.
    FlyCameraBase* CreateCamera( const char* cCameraName );

    // Create a user-defined camera object.
    FlyCameraBase* CreateUserCamera( const char* cName,const char* cTypename );

    // Get the current active camera.
    FlyCameraBase* GetCameraByName( const char* cCameraName );
    FlyCameraBase* GetCamera( UINT nIndex );

    // Destroy the cameras.
    void    DestroyCamera( const char* cCameraName );
    void    DestroyCamera( FlyCameraBase* pCamera );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_CAM_MANAGER_H__