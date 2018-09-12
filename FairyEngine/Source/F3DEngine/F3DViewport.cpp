/*
 * -----------------------------------------------------------------------
 *  Name:   F3DViewport.cpp
 *  Desc:   This file define the viewport type for engine.
 *  Author: Yish
 *  Date:   2010/12/25
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DViewport.h"
#include "F3DRenderTarget.h"


/** Constructor
@param RenderTarget of the viewport.
*/
F3DViewport::F3DViewport(F3DRenderTarget* pTarget)
	: m_pTarget(pTarget), m_pCamera(NULL), m_bFixedParam(false)
{
	m_ViewParam.fMinZ = 0.0f;
	m_ViewParam.fMaxZ = 1.0f;
	SetRelativeParam(0.0f, 0.0f, 1.0f, 1.0f);
}

/** Constructor
@param RenderTarget of the viewport.
@param Relative left.
@param Relative top.
@param Relative width.
@param Relative height.
*/
F3DViewport::F3DViewport(F3DRenderTarget* pTarget, float fLeft, float fTop, float fWidth, float fHeight, float fMinZ, float fMaxZ)
    : m_pTarget(pTarget), m_pCamera(NULL), m_bFixedParam(false)
{
	m_ViewParam.fMinZ = fMinZ;
	m_ViewParam.fMaxZ = fMaxZ;
	SetRelativeParam(fLeft, fTop, fWidth, fHeight);
}

/** Constructor
@param RenderTarget of the viewport.
@param Viewport parameters.
*/
F3DViewport::F3DViewport(F3DRenderTarget* pTarget, const SViewportDesc& vp)
	: m_pTarget(pTarget), m_pCamera(NULL), m_bFixedParam(false)
{
	m_ViewParam.fMinZ = vp.fMinZ;
	m_ViewParam.fMaxZ = vp.fMaxZ;
	SetRealParam(vp.nX, vp.nY, vp.nWidth, vp.nHeight);
}

/** Destructor
*/
F3DViewport::~F3DViewport(void)
{
}

/** Render the scene to me.
*/
void F3DViewport::Render(void)
{
    if( !m_pCamera )
        return;

    // 获取相机所挂接的场景
//    F3DSceneManager* pScene = m_pCamera->GetSceneManager();
}

/** Set the Z value.
*/
void F3DViewport::SetZValue(float fMinZ, float fMaxZ)
{
	m_ViewParam.fMinZ = fMinZ;
	m_ViewParam.fMaxZ = fMaxZ;
}

/** Transform the point in world space to viewport.
@return, false: the point is outside the viewport.
*/
bool F3DViewport::Transform(const F3DVector3& vIn, F3DVector3& vOut)
{
	if( !m_pCamera )
		return false;

	F3DVector3 vPoint;
	bool clip = m_pCamera->Transform(vIn, vPoint);
	if( vPoint.z < m_ViewParam.fMinZ || vPoint.z > m_ViewParam.fMaxZ )
		clip = true;

	vOut = vPoint * m_mViewScale;
	return clip;
}

/** Transform the point in viewport to world space.
*/
bool F3DViewport::InvTransform(const F3DVector3& vIn, F3DVector3& vOut)
{
	if( !m_pCamera )
		return false;

	// Convert to the projection space.
	F3DVector3 vPoint = vIn * m_mInvViewScale;
	m_pCamera->InvTransform(vPoint, vOut);
	return true;
}

/** Set the relative parameter.
*/
void F3DViewport::SetRelativeParam(float fLeft, float fTop, float fWidth, float fHeight)
{
	m_fRelLeft = fLeft;
	m_fRelTop = fTop;
	m_fRelWidth = fWidth;
	m_fRelHeight = fHeight;
	m_ViewParam.nX = (uint32)(m_pTarget->GetWidth() * fLeft);
	m_ViewParam.nY = (uint32)(m_pTarget->GetHeight() * fTop);
	m_ViewParam.nWidth = (uint32)(m_pTarget->GetWidth() * fWidth);
	m_ViewParam.nHeight = (uint32)(m_pTarget->GetHeight() * fHeight);
}

/** Set the real parameter.
*/
void F3DViewport::SetRealParam(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight)
{
	m_ViewParam.nX = nX;
	m_ViewParam.nY = nY;
	m_ViewParam.nWidth = nWidth;
	m_ViewParam.nHeight = nHeight;
	m_fRelLeft = (float)nX / m_pTarget->GetWidth();
	m_fRelTop = (float)nY / m_pTarget->GetHeight();
	m_fRelWidth = (float)nWidth / m_pTarget->GetWidth();
	m_fRelHeight = (float)nHeight / m_pTarget->GetHeight();
}

/** Call this when my renderTarget's size is changed.
*/
void F3DViewport::OnRTResize()
{
	if( m_bFixedParam ) return;

	uint32 width = m_pTarget->GetWidth();
	uint32 height = m_pTarget->GetHeight();

	m_ViewParam.nX = (uint32)(m_fRelLeft * width);
	m_ViewParam.nY = (uint32)(m_fRelTop * height);
	m_ViewParam.nWidth = (uint32)(m_fRelWidth * width);
	m_ViewParam.nHeight = (uint32)(m_fRelHeight * height);

	// Update the projection matrix of camera.
	if( m_pCamera )
	{
		if( m_pCamera->GetProjectionMode() == PJM_PERSPECTIVE )
		{
			float fAspect = (float)m_ViewParam.nWidth / m_ViewParam.nHeight;
			F3DCamera* pCamera = dynamic_cast<F3DCamera*>(m_pCamera);
			pCamera->SetProjectParam(pCamera->GetFOV(), fAspect);
		}
		else if( m_pCamera->GetProjectionMode() == PJM_ORTHOGONAL )
		{
			F3DCameraOrtho* pCamera = dynamic_cast<F3DCameraOrtho*>(m_pCamera);
			pCamera->SetOrthoParam(m_fRelLeft, m_fRelTop, m_fRelLeft+m_fRelWidth, m_fRelTop+m_fRelHeight);
#pragma F_COMPILE_WARN("Not implemented! update projection matrix for orthogonal camera.")
		}
	}

	// Recalculate the scale matrix.
	CalcScaleMatrix();
}

/** Calculate the scale matrix.
*/
void F3DViewport::CalcScaleMatrix()
{
	m_mViewScale.Identity();
	m_mViewScale._11 = 1.0f * m_ViewParam.nWidth / 2;
	m_mViewScale._22 = -1.0f * m_ViewParam.nHeight / 2;
	m_mViewScale._33 = m_ViewParam.fMaxZ - m_ViewParam.fMinZ;
	m_mViewScale._41 = 1.0f * m_ViewParam.nX + m_ViewParam.nWidth / 2;
	m_mViewScale._42 = 1.0f * m_ViewParam.nY + m_ViewParam.nHeight / 2;
	m_mViewScale._43 = m_ViewParam.fMinZ;

	m_mInvViewScale.InverseOf(m_mViewScale);
}