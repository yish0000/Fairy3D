/*
 * ------------------------------------------------------------------------
 *  Name:   F3DViewport.h
 *  Desc:   This file define the viewport type for engine.
 *  Author: Yish
 *  Date:   2010/12/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_VIEWPORT_H__
#define __F3D_VIEWPORT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DCamera.h"

///////////////////////////////////////////////////////////////////////////

/** Viewport
The camera renders the scene to a viewpoint. A viewport is a subset of a 
rendering target, which may be the whole surface or just a part of the surface.
*/
class FAIRY_API F3DViewport : public FGeneralAlloc
{
public:

	friend class F3DRenderTarget;

protected:
	F3DRenderTarget* m_pTarget; // Pointer to the renderTarget.
	F3DCameraBase* m_pCamera;   // Pointer to the camera.

    float m_fRelLeft;           // Relative left (0.0 ~ 1.0)
    float m_fRelTop;            // Relative up (0.0 ~ 1.0)
    float m_fRelWidth;          // Relative width (0.0 ~ 1.0)
    float m_fRelHeight;         // Relative height (0.0 ~ 1.0)

	SViewportDesc m_ViewParam;	// Parameters of viewport.
	bool m_bFixedParam;			// Will not change the viewport when rendertarget's size is changed.

	F3DMatrix4 m_mViewScale;	// Transform the point in projection space to screen.
	F3DMatrix4 m_mInvViewScale;	// Transform the point on screen to projection space.

public:
	F3DViewport(F3DRenderTarget* pTarget);
    F3DViewport(F3DRenderTarget* pTarget, float fLeft, float fTop, float fWidth, float fHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	F3DViewport(F3DRenderTarget* pTarget, const SViewportDesc& vp);
    virtual ~F3DViewport(void);

    // Render the viewport. (Call this function in every frame to render.)
    virtual void Render(void);

	// Transform the point in world space to viewport.
	bool Transform(const F3DVector3& vIn, F3DVector3& vOut);
	// Transform the point in viewport to world space.
	bool InvTransform(const F3DVector3& vIn, F3DVector3& vOut);

    // Set the camera.
    void SetCamera( F3DCameraBase* pCamera ) { m_pCamera = pCamera; }
    // Set the relative parameter.
    void SetRelativeParam(float fLeft, float fTop, float fWidth, float fHeight);
	// Set the real parameter.
	void SetRealParam(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight);
	// Set the Z value.
	void SetZValue(float fMinZ, float fMaxZ);
	// Set the flag which indicates the parameters won't be changed when renderTarget's size changed.
	void SetParamFixed(bool bFixed) { m_bFixedParam = bFixed; }

	// Get the Z value.
	float GetMinZ() const { return m_ViewParam.fMinZ; }
	float GetMaxZ() const { return m_ViewParam.fMaxZ; }

    // Get the relative parameters of viewport.
    float GetRelativeLeft(void) const { return m_fRelLeft; }
    float GetRelativeTop(void) const { return m_fRelTop; }
    float GetRelativeWidth(void) const { return m_fRelWidth; }
    float GetRelativeHeight(void) const { return m_fRelHeight; }

    // Get the real parameters.
    uint32 GetLeft(void) const { return m_ViewParam.nX; }
    uint32 GetTop(void) const { return m_ViewParam.nY; }
    uint32 GetWidth(void) const { return m_ViewParam.nWidth; }
    uint32 GetHeight(void) const { return m_ViewParam.nHeight; }

    // Get the pointer to the camera.
    F3DCameraBase* GetCamera(void) { return m_pCamera; }

    // Get the renderTarget.
    F3DRenderTarget* GetRenderTarget(void) { return m_pTarget; }

	// Get the parameter of viewport.
	const SViewportDesc& GetViewParam() const { return m_ViewParam; }

protected:

	// Call this when my renderTarget's size is changed.
	void OnRTResize();
	// Calculate the scale matrix.
	void CalcScaleMatrix();
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_VIEWPORT_H__