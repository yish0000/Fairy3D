/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFrustum.cpp
 *  Desc:   This file define a frustum class for engine.
 *  Author: Yish
 *  Date:   2013/1/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DFrustum.h"
#include "F3DCamera.h"
#include "F3DViewport.h"

// Constructor
F3DFrustum::F3DFrustum()
{
	for(int i=0; i<VP_NUM; i++)
		m_Planes[i].enabled = true;
}

// Copy constructor
F3DFrustum::F3DFrustum(const F3DFrustum& frustum)
{
	for(int i=0; i<VP_NUM; i++)
	{
		m_Planes[i].plane = frustum.GetPlane(i);
		m_Planes[i].enabled = frustum.IsPlaneEnabled(i);
	}
}

// Destructor
F3DFrustum::~F3DFrustum()
{
}

// Operator =
F3DFrustum& F3DFrustum::operator = (const F3DFrustum& frustum)
{
	for(int i=0; i<VP_NUM; i++)
	{
		m_Planes[i].plane = frustum.GetPlane(i);
		m_Planes[i].enabled = frustum.IsPlaneEnabled(i);
	}

	return *this;
}

// Build the frustum with specified parameters.
// Left, Right, Top, Bottom: project rectangle of frustum in identity view space.
// -1.0f, 1.0f, 1.0f, -1.0f, mean the whole view.
void F3DFrustum::Build(F3DCameraBase* pCamera, float fLeft, float fRight, float fTop,
					   float fBottom, bool zClip /* = true */)
{
	if( !pCamera )
	{
		FASSERT(pCamera);
		return;
	}

	for(int i=0; i<VP_NUM; i++)
	{
		if( (i == VP_FRONT || i == VP_BACK) && !zClip )
			m_Planes[i].enabled = false;
		else
			m_Planes[i].enabled = true;
	}

	F3DVector3 vLT(fLeft, fTop, 0.5f);
	F3DVector3 vLD(fLeft, fBottom, 0.5f);
	F3DVector3 vRT(fRight, fTop, 0.5f);
	F3DVector3 vRD(fRight, fBottom, 0.5f);

	// Convert these point to world space.
	pCamera->InvTransform(vLT, vLT);
	pCamera->InvTransform(vLD, vLD);
	pCamera->InvTransform(vRT, vRT);
	pCamera->InvTransform(vRD, vRD);

	F3DVector3 vEyePos = pCamera->GetPosition();
	m_Planes[VP_LEFT].plane.Set(vEyePos, vLT, vLD);
	m_Planes[VP_TOP].plane.Set(vEyePos, vLT, vRT);
	m_Planes[VP_RIGHT].plane.Set(vEyePos, vRT, vRD);
	m_Planes[VP_BOTTOM].plane.Set(vEyePos, vLD, vRD);

	if( zClip )
	{
		F3DVector3 vEyeDir = pCamera->GetDirection();

		// Near plane.
		F3DVector3 vPos = vEyePos + vEyeDir * pCamera->GetNearClip();
		F3DPlane& nearPlane = m_Planes[VP_FRONT].plane;
		nearPlane.m_vN = vEyeDir;
		nearPlane.Normalize();
		nearPlane.m_fD = -nearPlane.m_vN * vPos;

		// Far plane.
		vPos = vEyePos + vEyeDir * pCamera->GetFarClip();
		F3DPlane& farPlane = m_Planes[VP_BACK].plane;
		farPlane.m_vN = -vEyeDir;
		farPlane.m_vN.Normalize();
		farPlane.m_fD = -farPlane.m_vN * vPos;
	}
}

// Build the frustum in the specified viewport.
// Left, Right, Top, Bottom: project rectangle of frustum in identity view space.
// viewport.left, viewport.right, viewport.top, viewport.bottom, mean the whole view.
void F3DFrustum::Build(F3DViewport* pViewport, uint32 nLeft, uint32 nRight, uint32 nTop,
					   uint32 nBottom, bool zClip /* = true */)
{
	if( !pViewport )
	{
		FASSERT(pViewport);
		return;
	}

	for(int i=0; i<VP_NUM; i++)
	{
		if( (i == VP_FRONT || i == VP_BACK) && !zClip )
			m_Planes[i].enabled = false;
		else
			m_Planes[i].enabled = true;
	}

	F3DVector3 vLT((float)nLeft, (float)nTop, 0.5f);
	F3DVector3 vLD((float)nLeft, (float)nBottom, 0.5f);
	F3DVector3 vRT((float)nRight, (float)nTop, 0.5f);
	F3DVector3 vRD((float)nRight, (float)nBottom, 0.5f);

	// Convert these point to world space.
	pViewport->InvTransform(vLT, vLT);
	pViewport->InvTransform(vLD, vLD);
	pViewport->InvTransform(vRT, vRT);
	pViewport->InvTransform(vRD, vRD);

	F3DCameraBase* pCamera = pViewport->GetCamera();
	F3DVector3 vEyePos = pCamera->GetPosition();
	m_Planes[VP_LEFT].plane.Set(vEyePos, vLT, vLD);
	m_Planes[VP_TOP].plane.Set(vEyePos, vLT, vRT);
	m_Planes[VP_RIGHT].plane.Set(vEyePos, vRT, vRD);
	m_Planes[VP_BOTTOM].plane.Set(vEyePos, vLD, vRD);

	if( zClip )
	{
		F3DVector3 vEyeDir = pCamera->GetDirection();

		// Near plane.
		F3DVector3 vPos = vEyePos + vEyeDir * pCamera->GetNearClip();
		F3DPlane& nearPlane = m_Planes[VP_FRONT].plane;
		nearPlane.m_vN = vEyeDir;
		nearPlane.Normalize();
		nearPlane.m_fD = -nearPlane.m_vN * vPos;

		// Far plane.
		vPos = vEyePos + vEyeDir * pCamera->GetFarClip();
		F3DPlane& farPlane = m_Planes[VP_BACK].plane;
		farPlane.m_vN = -vEyeDir;
		farPlane.m_vN.Normalize();
		farPlane.m_fD = -farPlane.m_vN * vPos;
	}
}

/** Check whether the point is visible ?
*/
bool F3DFrustum::IsPointVisible( const F3DVector3& vPoint ) const
{
	for( int i=0;i<VP_NUM;i++ )
	{
		if( !m_Planes[i].enabled ) continue;
		if( m_Planes[i].plane.Classify(vPoint) == F3D_MATH_BACK )
			return false;
	}

	return true;
}

/** Check whether the sphere is visible ?
*/
bool F3DFrustum::IsSphereVisible( const F3DSphere& sphere ) const
{
	for( int i=0;i<VP_NUM;i++ )
	{
		if( !m_Planes[i].enabled ) continue;
		if( m_Planes[i].plane.DotCoord(sphere.m_vCenter) < -sphere.m_fRadius )
			return false;
	}

	return true;
}

/** Check whether the AABB is visible ?
*/
bool F3DFrustum::IsAABBVisible( const F3DAABB& box ) const
{
	for( int i=0;i<VP_NUM;i++ )
	{
		if( !m_Planes[i].enabled ) continue;
		if( m_Planes[i].plane.Classify(box) == F3D_MATH_BACK )
			return false;
	}

	return true;
}