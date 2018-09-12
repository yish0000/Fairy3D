/*
 * ------------------------------------------------------------------------
 *  Name:   F3DCamera.cpp
 *  Desc:   This file define all the camera classes for engine.
 *  Author: Yish
 *  Date:   2011/1/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DCamera.h"


///////////////////////////////////////////////////////////////////////////
//// Implement class F3DCameraBase

/** Constructor
@param Name of the camera.
*/
F3DCameraBase::F3DCameraBase( const AString& name )
    : m_sName(name), m_vPos(0.0f, 0.0f, 0.0f), m_qOrientation(0.0f, 0.0f, 0.0f, 1.0f)
	, m_vRight(1.0f, 0.0f, 0.0f), m_vUp(0.0f, 1.0f, 0.0f), m_vDir(0.0f, 0.0f, 1.0f)
	, m_bYawFixed(false), m_vYawAxis(0.0f, 1.0f, 0.0f), m_bUpdateView(false), m_bUpdateBillboard(false)
	, m_ProjMode(PJM_PERSPECTIVE), m_fNearClip(0.1f), m_fFarClip(1000.0f), m_bUpdateProj(false)
{
    m_mViewMatrix.MakeView3D( m_vRight, m_vUp, m_vDir, m_vPos );
    m_mBillboard.Identity();
    m_mBillboardH.Identity();
	m_mPostProject.Identity();
	m_mProjection.Identity();
	m_mVP.Identity();
}

/** Destructor
*/
F3DCameraBase::~F3DCameraBase(void)
{
}

/** Update the camera.
*/
void F3DCameraBase::Update(void)
{
	bool bUpdateWorldFrustum = false;
	bool bUpdateViewFrustum = false;

	// Update the view matrix.
    if( m_bUpdateView )
    {
        m_mViewMatrix.MakeView3D(m_vRight, m_vUp, m_vDir, m_vPos);
        m_bUpdateView = false;
		bUpdateWorldFrustum = true;
    }

    // Update the projection matrix.
    if( m_bUpdateProj )
    {
		UpdateProjectMatrix();
		m_bUpdateProj = false;
		bUpdateViewFrustum = true;
		bUpdateWorldFrustum = true;
    }

    // Update the frustums.
    if( bUpdateWorldFrustum )
        UpdateWorldFrustum();
	if( bUpdateViewFrustum )
		UpdateViewFrustum();
}

/** Set the camera's position.
*/
void F3DCameraBase::SetPosition( const F3DVector3& vPos )
{
    m_vPos = vPos;
    m_bUpdateView = true;
}

/** Set the camera's position.
*/
void F3DCameraBase::SetPosition( float fX, float fY, float fZ )
{
    m_vPos.x = fX;
    m_vPos.y = fY;
    m_vPos.z = fZ;
    m_bUpdateView = true;
}

/** Move the camera in the world space.
*/
void F3DCameraBase::MoveWorld( const F3DVector3& vec )
{
    m_vPos += vec;
    m_bUpdateView = true;
}

/** Move the camera in the world space. (X axis)
*/
void F3DCameraBase::MoveWorldX( float fX )
{
    m_vPos.x += fX;
    m_bUpdateView = true;
}

/** Move the camera in the world space. (Y axis)
*/
void F3DCameraBase::MoveWorldY( float fY )
{
    m_vPos.y += fY;
    m_bUpdateView = true;
}

/** Move the camera in the world space. (Z axis)
*/
void F3DCameraBase::MoveWorldZ( float fZ )
{
    m_vPos.z += fZ;
    m_bUpdateView = true;
}

/** Move the camera in the local space.
*/
void F3DCameraBase::MoveLocal( const F3DVector3& vec )
{
    m_vPos += m_qOrientation.Rotate( vec );
    m_bUpdateView = true;
}

/** Move the camera in the local space. (X axis)
*/
void F3DCameraBase::MoveLocalX( float fX )
{
    MoveLocal( F3DVector3(fX, 0.0f, 0.0f) );
}

/** Move the camera in the local space. (Y axis)
*/
void F3DCameraBase::MoveLocalY( float fY )
{
    MoveLocal( F3DVector3(0.0f, fY, 0.0f) );
}

/** Move the camera in the local space. (Z axis)
*/
void F3DCameraBase::MoveLocalZ( float fZ )
{
    MoveLocal( F3DVector3(0.0f, 0.0f, fZ) );
}

/** Rotate the camera. (X axis)
*/
void F3DCameraBase::Pitch( float fAngle )
{
    F3DVector3 vAxis;
    vAxis = m_qOrientation.Rotate( F3DVector3::UNIT_X );

    Rotate( vAxis, fAngle );
}

/** Rotate the camera. (Y axis)
*/
void F3DCameraBase::Yaw( float fAngle )
{
    F3DVector3 vAxis;

    if( m_bYawFixed )
        vAxis = m_vYawAxis;
    else
        vAxis = m_qOrientation.Rotate( F3DVector3::UNIT_Y );

    Rotate( vAxis, fAngle );
}

/** Rotate the camera. (Z axis)
*/
void F3DCameraBase::Roll( float fAngle )
{
    F3DVector3 vAxis;
    vAxis = m_qOrientation.Rotate( F3DVector3::UNIT_Z );

    Rotate( vAxis, fAngle );
}

/** Rotate the camera. (Axis, angle)
*/
void F3DCameraBase::Rotate( const F3DVector3& vAxis, float fAngle )
{
    F3DQuat qFrame;
    qFrame.FromAngleAxis( vAxis, fAngle );

    Rotate( qFrame );
}

/** Rotate the camera with a quaternion.
*/
void F3DCameraBase::Rotate( const F3DQuat& quat )
{
    m_qOrientation = quat * m_qOrientation;

    m_vRight = m_qOrientation.Rotate( F3DVector3::UNIT_X );
    m_vUp = m_qOrientation.Rotate( F3DVector3::UNIT_Y );
    m_vDir = m_qOrientation.Rotate( F3DVector3::UNIT_Z );

    m_bUpdateView = true;
    m_bUpdateBillboard = true;
}

/** Set the current orientation.
@param The axis.
@param The angle.
*/
void F3DCameraBase::SetRotation( const F3DVector3& vAxis, float fAngle )
{
    F3DQuat qFrame;
    qFrame.FromAngleAxis( vAxis, fAngle );

    SetRotation( qFrame );
}

/** Set the current orientation.
*/
void F3DCameraBase::SetRotation( const F3DQuat& quat )
{
    m_qOrientation = quat;

    m_vRight = m_qOrientation.Rotate( F3DVector3::UNIT_X );
    m_vUp = m_qOrientation.Rotate( F3DVector3::UNIT_Y );
    m_vDir = m_qOrientation.Rotate( F3DVector3::UNIT_Z );

    m_bUpdateView = true;
    m_bUpdateBillboard = true;
}

/** Set the loot at point.
*/
void F3DCameraBase::LookAt( const F3DVector3& vAt )
{
    F3DVector3 vWorldUp;
    F3DVector3 xAxis, yAxis, zAxis;
    F3DMatrix4 mRotation;

    zAxis = vAt - m_vPos;
    zAxis.Normalize();
    vWorldUp.Cross( F3DVector3::UNIT_Y, zAxis );
    vWorldUp.Normalize();
    xAxis = vWorldUp;
    yAxis.Cross( zAxis, xAxis );

    mRotation.Identity();
    mRotation._11 = xAxis.x;
    mRotation._21 = yAxis.x;
    mRotation._31 = zAxis.x;

    mRotation._12 = xAxis.y;
    mRotation._22 = yAxis.y;
    mRotation._32 = zAxis.y;

    mRotation._13 = xAxis.z;
    mRotation._23 = yAxis.z;
    mRotation._33 = zAxis.z;

    m_vRight = xAxis;
    m_vUp = yAxis;
    m_vDir = zAxis;
    m_qOrientation.MakeFromMatrix( mRotation );

    m_bUpdateView = true;
    m_bUpdateBillboard = true;
}

/** Set the fixed Y axis.
*/
void F3DCameraBase::SetYawFixed(bool bFixed, const F3DVector3& vYawAxis )
{
    if( !bFixed )
    {
        m_bYawFixed = false;
    }
    else
    {
        m_bYawFixed = true;
        m_vYawAxis = vYawAxis;
    }
}

/** Set the view matrix.
*/
void F3DCameraBase::SetViewMatrix(const F3DMatrix4& mat)
{
	F3DMatrix4 matInv;

	m_mViewMatrix = mat;
	m_mVP = m_mViewMatrix * m_mProjection * m_mPostProject;

	F3DVector3 vDir, vUp, vPos, vRight;
	vDir.Set(mat.m[0][2], mat.m[1][2], mat.m[2][2]);
	vUp.Set(mat.m[0][1], mat.m[1][1], mat.m[2][1]);
	vPos.Set(mat.m[3][0], mat.m[3][1], mat.m[3][2]);
	vRight.Cross(vUp, vDir);

	matInv = mat;
	matInv._41 = matInv._42 = matInv._43 = 0.0f;
	matInv.Transpose();
	vPos = matInv * (-1.0f * vPos);

	vDir.Normalize();
	vUp.Normalize();
	vRight.Normalize();

	m_vPos = vPos;
	m_vDir = vDir;
	m_vUp = vUp;
	m_vRight = vRight;
	m_qOrientation.MakeFromMatrix(matInv);
	m_bUpdateBillboard = true;
}

/** Get the billboard matrix.
@param Is the axis Y fixed ?
*/
const F3DMatrix4& F3DCameraBase::GetBillboardMatrix( bool bAxisY/* =false */ )
{
    if( m_bUpdateBillboard )
    {
        m_mBillboardH.RotateY( m_qOrientation.GetYaw() );
        m_qOrientation.GetMatrix( m_mBillboard );
        m_bUpdateBillboard = false;
    }

    return bAxisY ? m_mBillboardH : m_mBillboard;
}

/** Set the near and far clip planes.
*/
void F3DCameraBase::SetViewPlanes( float fNear, float fFar )
{
    m_fNearClip = fNear;
    m_fFarClip = fFar;
    m_bUpdateProj = true;
}

// Transform the point from world coordinates to cuboid coordinates.
bool F3DCameraBase::Transform(const F3DVector3& vIn, F3DVector3& vOut)
{
	vOut = vIn * m_mVP;

	if( vOut.x < -1.0f || vOut.x > 1.0f ||
		vOut.y < -1.0f || vOut.y > 1.0f ||
		vOut.z < 0.0f || vOut.z > 1.0f )
		return false;
	else
		return true;
}

// Transform the point from cuboid coordinates to world coordinates.
bool F3DCameraBase::InvTransform(const F3DVector3& vIn, F3DVector3& vOut)
{
	F3DMatrix4 matInv;
	matInv.InverseOf(m_mVP);
	vOut = vIn * matInv;
	return true;
}

// Is the specified point visible ?
bool F3DCameraBase::IsPointVisible( const F3DVector3& vPoint ) const
{
	return m_WorldFrustum.IsPointVisible(vPoint);
}

// Is the sphere visible ?
bool F3DCameraBase::IsSphereVisible( const F3DSphere& sphere ) const
{
	return m_WorldFrustum.IsSphereVisible(sphere);
}

// Is the specified AABB visible ?
bool F3DCameraBase::IsAABBVisible( const F3DAABB& box ) const
{
	return m_WorldFrustum.IsAABBVisible(box);
}

///////////////////////////////////////////////////////////////////////////
//// Implement class F3DCamera

F3DCamera::F3DCamera(const AString& name)
	: F3DCameraBase(name), m_fFOV(F3D_PI / 4.0f), m_fAspect(1.333333f)
{
}

F3DCamera::~F3DCamera()
{
}

// Set the projection parameters.
void F3DCamera::SetProjectParam( float fFOV, float fAspect )
{
	m_fFOV = fFOV;
	m_fAspect = fAspect;
	m_bUpdateProj = true;
}

// Update the projection matrix.
void F3DCamera::UpdateProjectMatrix()
{
	m_mProjection.MakePerspProj(m_fFOV, m_fAspect, m_fNearClip, m_fFarClip);
	m_mVP = m_mViewMatrix * m_mProjection * m_mPostProject;
}

// Update the world frustum.
void F3DCamera::UpdateWorldFrustum()
{
	F3DPlane plane;

	// Calculate the product of view matrix and projection matrix.
	F3DMatrix4 mViewProj = m_mViewMatrix * m_mProjection;

	// Left clip plane.
	plane.m_vN.x = ( mViewProj._11 + mViewProj._14 );
	plane.m_vN.y = ( mViewProj._21 + mViewProj._24 );
	plane.m_vN.z = ( mViewProj._31 + mViewProj._34 );
	plane.m_fD   = ( mViewProj._41 + mViewProj._44 );
	plane.Normalize();
	m_WorldFrustum.SetPlane(F3DFrustum::VP_LEFT, plane);

	// Right clip plane.
	plane.m_vN.x = ( mViewProj._14 - mViewProj._11 );
	plane.m_vN.y = ( mViewProj._24 - mViewProj._21 );
	plane.m_vN.z = ( mViewProj._34 - mViewProj._31 );
	plane.m_fD   = ( mViewProj._44 - mViewProj._41 );
	plane.Normalize();
	m_WorldFrustum.SetPlane(F3DFrustum::VP_RIGHT, plane);

	// Top clip plane.
	plane.m_vN.x = ( mViewProj._14 - mViewProj._12 );
	plane.m_vN.y = ( mViewProj._24 - mViewProj._22 );
	plane.m_vN.z = ( mViewProj._34 - mViewProj._32 );
	plane.m_fD   = ( mViewProj._44 - mViewProj._42 );
	plane.Normalize();
	m_WorldFrustum.SetPlane(F3DFrustum::VP_TOP, plane);

	// Bottom clip plane.
	plane.m_vN.x = ( mViewProj._12 + mViewProj._14 );
	plane.m_vN.y = ( mViewProj._22 + mViewProj._24 );
	plane.m_vN.z = ( mViewProj._32 + mViewProj._34 );
	plane.m_fD   = ( mViewProj._42 + mViewProj._44 );
	plane.Normalize();
	m_WorldFrustum.SetPlane(F3DFrustum::VP_BOTTOM, plane);

	// Near clip plane.
	plane.m_vN.x = mViewProj._13;
	plane.m_vN.y = mViewProj._23;
	plane.m_vN.z = mViewProj._33;
	plane.m_fD   = mViewProj._43;
	plane.Normalize();
	m_WorldFrustum.SetPlane(F3DFrustum::VP_FRONT, plane);

	// Far clip plane.
	plane.m_vN.x = ( mViewProj._14 - mViewProj._13 );
	plane.m_vN.y = ( mViewProj._24 - mViewProj._23 );
	plane.m_vN.z = ( mViewProj._34 - mViewProj._33 );
	plane.m_fD   = ( mViewProj._44 - mViewProj._43 );
	plane.Normalize();
	m_WorldFrustum.SetPlane(F3DFrustum::VP_BACK, plane);
}

// Update the view frustum.
void F3DCamera::UpdateViewFrustum()
{
	F3DPlane plane;
	plane.m_fD = 0.0f;

	// Top and bottom clip planes.
	float c, s, fAngle = m_fFOV * 0.5f;

	c = cosf(fAngle);
	s = sinf(fAngle);

	plane.m_vN.Set(0.0f, -c, s);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_TOP, plane);
	plane.m_vN.Set(0.0f, c, s);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_BOTTOM, plane);

	// Left and right clip planes.
	fAngle = tanf(fAngle) * m_fAspect;
	c = cosf(atanf(fAngle));
	s = sinf(atanf(fAngle));

	plane.m_vN.Set(c, 0.0f, s);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_LEFT, plane);
	plane.m_vN.Set(-c, 0.0f, s);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_RIGHT, plane);

	// Front and back clip planes.
	plane.m_vN.Set(0.0f, 0.0f, 1.0f);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_FRONT, plane);
	plane.m_vN.Set(0.0f, 0.0f, -1.0f);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_BACK, plane);
}

///////////////////////////////////////////////////////////////////////////
//// Implement class F3DCameraOrtho

F3DCameraOrtho::F3DCameraOrtho(const AString& name)
	: F3DCameraBase(name), m_fLeft(0.0f), m_fRight(0.0f), m_fTop(0.0f), m_fBottom(0.0f)
{
}

F3DCameraOrtho::~F3DCameraOrtho()
{
}

// Set the projection parameters.
void F3DCameraOrtho::SetOrthoParam( float fLeft, float fTop, float fRight, float fBottom )
{
	m_fLeft = fLeft;
	m_fRight = fRight;
	m_fTop = fTop;
	m_fBottom = fBottom;
	m_bUpdateProj = true;
}

// Update the projection matrix
void F3DCameraOrtho::UpdateProjectMatrix()
{
	m_mProjection.MakeOrthoProj(m_fLeft, m_fRight, m_fBottom, m_fTop, m_fNearClip, m_fFarClip);
	m_mVP = m_mViewMatrix * m_mProjection * m_mPostProject;
}

// Update the frustum in world coordinates.
void F3DCameraOrtho::UpdateWorldFrustum()
{
	F3DPlane plane;

	// Top clip plane
	plane.m_vN = -m_vUp;
	plane.m_fD = plane.m_vN * (m_vPos + m_vUp * m_fTop);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_TOP, plane);

	// Bottom clip plane
	plane.m_vN = m_vUp;
	plane.m_fD = plane.m_vN * (m_vPos + m_vUp * m_fBottom);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_BOTTOM, plane);

	// Left clip plane
	plane.m_vN = m_vRight;
	plane.m_fD = plane.m_vN * (m_vPos + m_vRight * m_fLeft);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_LEFT, plane);

	// Right clip plane
	plane.m_vN = -m_vRight;
	plane.m_fD = plane.m_vN * (m_vPos + m_vRight * m_fRight);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_RIGHT, plane);

	// Near clip plane
	plane.m_vN = m_vDir;
	plane.m_fD = plane.m_vN * (m_vPos + m_vDir * m_fNearClip);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_FRONT, plane);

	// Far clip plane
	plane.m_vN = -m_vDir;
	plane.m_fD = plane.m_vN * (m_vPos + m_vDir * m_fFarClip);
	m_ViewFrustum.SetPlane(F3DFrustum::VP_BACK, plane);
}

// Update the frustum in view coordinates.
void F3DCameraOrtho::UpdateViewFrustum()
{
	F3DPlane plane;

	// Top clip plane.
	plane.m_vN.Set(0.0f, -1.0f, 0.0f);
	plane.m_fD = -m_fTop;
	m_ViewFrustum.SetPlane(F3DFrustum::VP_TOP, plane);

	// Bottom clip plane.
	plane.m_vN.Set(0.0f, 1.0f, 0.0f);
	plane.m_fD = m_fBottom;
	m_ViewFrustum.SetPlane(F3DFrustum::VP_BOTTOM, plane);

	// Left clip plane.
	plane.m_vN.Set(1.0f, 0.0f, 0.0f);
	plane.m_fD = m_fLeft;
	m_ViewFrustum.SetPlane(F3DFrustum::VP_LEFT, plane);

	// Right clip plane.
	plane.m_vN.Set(-1.0f, 0.0f, 0.0f);
	plane.m_fD = -m_fRight;
	m_ViewFrustum.SetPlane(F3DFrustum::VP_RIGHT, plane);

	// Near clip plane.
	plane.m_vN.Set(0.0f, 0.0f, 1.0f);
	plane.m_fD = m_fNearClip;
	m_ViewFrustum.SetPlane(F3DFrustum::VP_FRONT, plane);

	// Far clip plane.
	plane.m_vN.Set(0.0f, 0.0f, -1.0f);
	plane.m_fD = m_fFarClip;
	m_ViewFrustum.SetPlane(F3DFrustum::VP_BACK, plane);
}