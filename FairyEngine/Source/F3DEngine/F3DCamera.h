/*
 * ------------------------------------------------------------------------
 *  Name:   F3DCamera.h
 *  Desc:   This file define all the camera classes for engine.
 *  Author: Yish
 *  Date:   2011/1/17
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_CAMERA_H__
#define __F3D_CAMERA_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DFrustum.h"

///////////////////////////////////////////////////////////////////////////

/** Projection mode for camera.
*/
enum EProjectionMode
{
    PJM_ORTHOGONAL = 1,		// Orthogonal projection.
    PJM_PERSPECTIVE,		// Perspective projection.
};

/** Base class of camera.
@remarks
Each camera have two frustum, we will use them to clip the invisible objects.
*/
class FAIRY_API F3DCameraBase : public FGeneralAlloc
{
protected:
    AString m_sName;                // Name of the camera.
	EProjectionMode m_ProjMode;     // Projection mode of the camera.

    F3DVector3 m_vPos;              // Position of camera.
    F3DQuat m_qOrientation;         // Orientation of camera.
    F3DVector3 m_vRight;            // X axis of camera.
    F3DVector3 m_vUp;               // Y axis of camera.
    F3DVector3 m_vDir;              // Z axis of camera.

	float m_fFarClip;               // Far clip plane.
	float m_fNearClip;              // Near clip plane.

    bool m_bYawFixed;               // Is the axis fixed ?
    F3DVector3 m_vYawAxis;          // The axis which camera yaw around.

    F3DMatrix4 m_mViewMatrix;       // View matrix for camera.
	F3DMatrix4 m_mProjection;       // Projection Matrix for camera.
	F3DMatrix4 m_mVP;				// The product of view and projection matrix.
	F3DMatrix4 m_mPostProject;		// Matrix used for post project process.
    F3DMatrix4 m_mBillboard;		// Billboard matrix for camera.
	F3DMatrix4 m_mBillboardH;		// Billboard matrix for camera. (Y axis is F3DVector3(0, 1, 0))

	bool m_bUpdateView;             // Need to update the view matrix ?
	bool m_bUpdateProj;             // Need to update the projection matrix ?
	bool m_bUpdateBillboard;        // Need to update billboard camera ?

	F3DFrustum m_WorldFrustum;		// Frustum in the world coordinates.
	F3DFrustum m_ViewFrustum;		// Frustum in the view coordinates.

public:
    F3DCameraBase( const AString& name );
    virtual ~F3DCameraBase(void);

    // Update the camera object.
    virtual void Update(void);

    // Set the position of camera in world coordinates.
    void SetPosition( const F3DVector3& vPos );
    void SetPosition( float fX, float fY, float fZ );

    // Move the camera in world coordinates.
    void MoveWorld( const F3DVector3& vec );
    void MoveWorldX( float fX );
    void MoveWorldY( float fY );
    void MoveWorldZ( float fZ );

    // Move the camera in local coordinates.
    void MoveLocal( const F3DVector3& vec );
    void MoveLocalX( float fX );
    void MoveLocalY( float fY );
    void MoveLocalZ( float fZ );

    // Get the position of camera in world coordinates.
    const F3DVector3& GetPosition(void) const { return m_vPos; }
    // Get the direction of camera.
    const F3DVector3& GetDirection(void) const { return m_vDir; }
    // Get the up axis of camera.
    const F3DVector3& GetUp(void) const { return m_vUp; }
    // Get the right axis of camera.
    const F3DVector3& GetRight(void) const { return m_vRight; }
    // Get the rotation data of camera.
    const F3DQuat& GetRotation(void) const { return m_qOrientation; }

    // Set the lookat point of camera.
    void LookAt( const F3DVector3& vAt );
	void LookAt( float fX, float fY, float fZ ) { LookAt(F3DVector3(fX, fY, fZ)); }

    // Rotate the camera around the X axis.
    void Pitch( float fAngle );
    // Rotate the camera around the Y axis.
    void Yaw( float fAngle );
    // Rotate the camera around the Z axis.
    void Roll( float fAngle );

    // Rotate the camera around the axis.
    void Rotate( const F3DVector3& vAxis, float fAngle );
    // Rotate the camera by quaternion.
    void Rotate( const F3DQuat& quat );

    // Set the rotation data.
    void SetRotation( const F3DVector3& vAxis, float fAngle );
    void SetRotation( const F3DQuat& quat );

    // Set the fixed yaw axis.
    void SetYawFixed( bool bFixed, const F3DVector3& vYawAxis = F3DVector3(0.0f, 1.0f, 0.0f) );

	// Set the view matrix.
	void SetViewMatrix(const F3DMatrix4& mat);
	// Set the post project process matrix.
	void SetPostProjectMatrix(const F3DMatrix4& mat) { m_mPostProject = mat; }

	// Get the view matrix.
	const F3DMatrix4& GetViewMatrix() const { return m_mViewMatrix; }
	// Get the projection matrix.
	const F3DMatrix4& GetProjectionMatrix(void) const { return m_mProjection; }
	// Get the billboard matrix.
	const F3DMatrix4& GetBillboardMatrix( bool bAxisY = false );
	// Get the product of view and project matrix.
	const F3DMatrix4& GetVPMatrix() const { return m_mVP; }
	// Get the post project process matrix.
	const F3DMatrix4& GetPostProjectMatrix() const { return m_mPostProject; }

    // Set the near/far planes for camera.
    void SetViewPlanes( float fNear, float fFar );

    // Get the near clip plane.
    float GetNearClip(void) const { return m_fNearClip; }
    // Get the far clip plane.
    float GetFarClip(void) const { return m_fFarClip; }

	// Transform the point from world coordinates to cuboid coordinates.
	bool Transform(const F3DVector3& vIn, F3DVector3& vOut);
	// Transform the point from cuboid coordinates to world coordinates.
	bool InvTransform(const F3DVector3& vIn, F3DVector3& vOut);

	// Get the frustum in world coordinates.
	const F3DFrustum& GetWorldFrustum() const { return m_WorldFrustum; }
	// Get the frustum in view coordinates.
	const F3DFrustum& GetViewFrustum() const { return m_ViewFrustum; }

	// Is the specified point visible ?
	bool IsPointVisible( const F3DVector3& vPoint ) const;
	// Is the sphere visible ?
	bool IsSphereVisible( const F3DSphere& sphere ) const;
	// Is the specified AABB visible ?
	bool IsAABBVisible( const F3DAABB& box ) const;
	bool IsAABBVisible( const F3DVector3& vMin, const F3DVector3& vMax ) const
	{
		return IsAABBVisible(F3DAABB(vMin, vMax));
	}

	// Get the projection mode.
	EProjectionMode GetProjectionMode(void) const { return m_ProjMode; }

    // Get the name of the camera.
    const AString& GetName(void) const { return m_sName; }

protected:

	// Update the projection matrix.
	virtual void UpdateProjectMatrix() = 0;
	// Update the frustum in world coordinates.
	virtual void UpdateWorldFrustum() = 0;
	// Update the frustum in view coordinates.
	virtual void UpdateViewFrustum() = 0;
};

///////////////////////////////////////////////////////////////////////////

/** Class of perspective camera.
*/
class FAIRY_API F3DCamera : public F3DCameraBase
{
public:
	F3DCamera(const AString& name);
	virtual ~F3DCamera();

	// Set the projection parameters.
	void SetProjectParam( float fFOV, float fAspect );

	// Get the FOV.
	float GetFOV(void) const { return m_fFOV; }
	// Get the aspect.
	float GetAspect(void) const { return m_fAspect; }

protected:
	float m_fFOV;
	float m_fAspect;

protected:

	// Update the projection matrix.
	virtual void UpdateProjectMatrix();
	// Update the frustum in world coordinates.
	virtual void UpdateWorldFrustum();
	// Update the frustum in view coordinates.
	virtual void UpdateViewFrustum();
};

///////////////////////////////////////////////////////////////////////////

/** class of orthogonal camera.
*/
class FAIRY_API F3DCameraOrtho : public F3DCameraBase
{
public:
	F3DCameraOrtho(const AString& name);
	virtual ~F3DCameraOrtho();

	// Set the projection parameters.
	void SetOrthoParam( float fLeft, float fTop, float fRight, float fBottom );

	// Get the projection parameters.
	float GetLeft() const { return m_fLeft; }
	float GetRight() const { return m_fRight; }
	float GetTop() const { return m_fTop; }
	float GetBottom() const { return m_fBottom; }

protected:
	float m_fLeft;			// Left clip plane.
	float m_fRight;			// Right clip plane.
	float m_fTop;			// Top clip plane.
	float m_fBottom;		// Bottom clip plane.

protected:

	// Update the projection matrix.
	virtual void UpdateProjectMatrix();
	// Update the frustum in world coordinates.
	virtual void UpdateWorldFrustum();
	// Update the frustum in view coordinates.
	virtual void UpdateViewFrustum();
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_CAMERA_H__