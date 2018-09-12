//------------------------------------------------------------------------
// Name: AppCamera.cpp
// Desc: This file is to implement the class CAppCamera.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "AppCamera.h"


//------------------------------------------------------------------------
// Constructor of the class
// ----------------------------------------------------------------------
// Param -> IN:
//      LPFLYRENDERDEVICE:  Pointer to the Render device.
//------------------------------------------------------------------------
CAppCamera::CAppCamera( LPFLYRENDERDEVICE pDevice )
{
    m_vPos.Set( 0.0f,0.0f,-20.0f );
    m_vRight.Set( 1.0f,0.0f,0.0f );
    m_vUp.Set( 0.0f,1.0f,0.0f );
    m_vDir.Set( 0.0f,0.0f,1.0f );

    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fRotZ = 0.0f;

    m_pDevice = pDevice;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
CAppCamera::~CAppCamera(void)
{
}


//------------------------------------------------------------------------
// Reset the camera for the editor.
//------------------------------------------------------------------------
void CAppCamera::Reset(void)
{
    m_vPos.Set( 0.0f,0.0f,-20.0f );
    m_vRight.Set( 1.0f,0.0f,0.0f );
    m_vUp.Set( 0.0f,1.0f,0.0f );
    m_vDir.Set( 0.0f,0.0f,1.0f );

    m_Quat.x = m_Quat.y = m_Quat.z = 0.0f;
    m_Quat.w = 1.0f;

    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fRotZ = 0.0f;
}


//------------------------------------------------------------------------
// Update the camera during every frame.
//------------------------------------------------------------------------
void CAppCamera::Update(void)
{
    if( m_fRotX != 0.0f ||
        m_fRotY != 0.0f ||
        m_fRotZ != 0.0f )
    {
        RecalcAxis();
        m_fRotX = m_fRotY = m_fRotZ = 0.0f;
    }

    m_pDevice->SetView3D( m_vRight,m_vUp,m_vDir,m_vPos );
}


//------------------------------------------------------------------------
// Recalculate the axis for the camera.
//------------------------------------------------------------------------
void CAppCamera::RecalcAxis(void)
{
    static float f2PI = 6.283185f;

    FlyQuat   qFrame;
    FlyMatrix mat;

    // Keep in range of 360 degrees
    if( m_fRotX > f2PI )
        m_fRotX -= f2PI;
    else if( m_fRotX < -f2PI )
        m_fRotX += f2PI;

    if( m_fRotY > f2PI )
        m_fRotY -= f2PI;
    else if( m_fRotY < -f2PI )
        m_fRotY += f2PI;

    if( m_fRotZ > f2PI )
        m_fRotZ -= f2PI;
    else if( m_fRotZ < -f2PI )
        m_fRotZ += f2PI;

    // Build new quaternion for roation
    qFrame.MakeFromEuler( m_fRotX,m_fRotY,m_fRotZ );

    // Add to current rotation
    m_Quat *= qFrame;

    // Extract the local axis
    m_Quat.GetMatrix( &mat );

    // Out put the result
    m_vRight.x  = mat._11;
    m_vRight.y  = mat._21;
    m_vRight.z  = mat._31;

    m_vUp.x     = mat._12;
    m_vUp.y     = mat._22;
    m_vUp.z     = mat._32;

    m_vDir.x    = mat._13;
    m_vDir.y    = mat._23;
    m_vDir.z    = mat._33;
}


//------------------------------------------------------------------------
// Move the camera along the X axis.
//------------------------------------------------------------------------
void CAppCamera::MoveAlongX( float fValue )
{
    m_vPos += m_vRight * fValue;
}


//------------------------------------------------------------------------
// Move the camera along the Y axis.
//------------------------------------------------------------------------
void CAppCamera::MoveAlongY( float fValue )
{
    m_vPos += m_vUp * fValue;
}


//------------------------------------------------------------------------
// Move the camera along the Z axis.
//------------------------------------------------------------------------
void CAppCamera::MoveAlongZ( float fValue )
{
    m_vPos += m_vDir * fValue;
}


//------------------------------------------------------------------------
// Rotate the camera along the Right axis.
//------------------------------------------------------------------------
void CAppCamera::Pitch( float fAngle )
{
    m_fRotX = fAngle;
}


//------------------------------------------------------------------------
// Rotate the camera along the Up axis.
//------------------------------------------------------------------------
void CAppCamera::Yaw( float fAngle )
{
    m_fRotY = fAngle;
}


//------------------------------------------------------------------------
// Rotate the camera along the Direction axis.
//------------------------------------------------------------------------
void CAppCamera::Roll( float fAngle )
{
    m_fRotZ = fAngle;
}


//------------------------------------------------------------------------
// Set the position for the camera.
//------------------------------------------------------------------------
void CAppCamera::SetPosition( const FlyVector& vPos )
{
    m_vPos = vPos;
}