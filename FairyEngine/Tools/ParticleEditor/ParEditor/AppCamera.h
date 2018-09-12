//------------------------------------------------------------------------
// Name: AppCamera.h
// Desc: This file define a class the camera to control the 3D views.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __APP_CAMERA_H__
#define __APP_CAMERA_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "FlyRenderDevice.h"
//////////////////////////////////////////////////////////////////////////


class CAppCamera
{
protected:
    FlyVector   m_vPos;
    FlyVector   m_vRight;
    FlyVector   m_vUp;
    FlyVector   m_vDir;
    FlyQuat     m_Quat;

    float       m_fRotX;
    float       m_fRotY;
    float       m_fRotZ;

    // Pointer to the Render device.
    LPFLYRENDERDEVICE m_pDevice;

    void    RecalcAxis(void);

public:
    // Constructor and destrucotr.
    CAppCamera( LPFLYRENDERDEVICE pDevice );
    ~CAppCamera(void);

    void    Update(void);
    void    Reset(void);

    void    MoveAlongX( float fValue );
    void    MoveAlongY( float fValue );
    void    MoveAlongZ( float fValue );
    void    Pitch( float fAngle );
    void    Yaw( float fAngle );
    void    Roll( float fAngle );
    void    SetPosition( const FlyVector& vPos );

    FlyVector GetRight(void) { return m_vRight; }
    FlyVector GetUp(void) { return m_vUp; }
    FlyVector GetDir(void) { return m_vDir; }
    FlyVector GetPosition(void) { return m_vPos; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __APP_CAMERA_H__
