/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFrustum.h
 *  Desc:   This file define a frustum class for engine.
 *  Author: Yish
 *  Date:   2013/1/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_FRUSTUM_H__
#define __F3D_FRUSTUM_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** Frustum class.
*/
class FAIRY_API F3DFrustum : public FGeneralAlloc
{
public:

	enum
	{
		VP_LEFT,
		VP_RIGHT,
		VP_TOP,
		VP_BOTTOM,
		VP_FRONT,
		VP_BACK,

		VP_NUM,
	};

	struct ClipPlane
	{
		F3DPlane plane;
		bool enabled;
	};

public:
    F3DFrustum();
	F3DFrustum(const F3DFrustum& frustum);
    virtual ~F3DFrustum();

	// Operator =
	F3DFrustum& operator = (const F3DFrustum& frustum);

	// Build the frustum with specified parameters.
	// Left, Right, Top, Bottom: project rectangle of frustum in identity view space.
	// -1.0f, 1.0f, 1.0f, -1.0f, mean the whole view.
	void Build(F3DCameraBase* pCamera, float fLeft, float fRight, float fTop, float fBottom, bool zClip = true);
	// Build the frustum in the specified viewport.
	// Left, Right, Top, Bottom: project rectangle of frustum in identity view space.
	// viewport.left, viewport.right, viewport.top, viewport.bottom, mean the whole view.
	void Build(F3DViewport* pViewport, uint32 nLeft, uint32 nRight, uint32 nTop, uint32 nBottom, bool zClip = true);

	// Check whether the point is visible.
	bool IsPointVisible( const F3DVector3& vPoint ) const;

	// Check whether the sphere is visible.
	bool IsSphereVisible( const F3DSphere& sphere ) const;

	// Check whether the AABB is visible.
	bool IsAABBVisible( const F3DAABB& box ) const;
	bool IsAABBVisible( const F3DVector3& vMin, const F3DVector3& vMax ) const
	{
		return IsAABBVisible(F3DAABB(vMin, vMax));
	}

	// Get the specified clip plane.
	const F3DPlane& GetPlane(int plane) const
	{
		FASSERT(plane >= 0 && plane < VP_NUM);
		return m_Planes[plane].plane;
	}

	// Change the specified clip plane.
	void SetPlane(int idx, const F3DPlane& plane)
	{
		FASSERT(idx >= 0 && idx < VP_NUM);
		m_Planes[idx].plane = plane;
	}

	// Whether the clip plane is enabled ?
	bool IsPlaneEnabled(int idx) const
	{
		FASSERT(idx >= 0 && idx < VP_NUM);
		return m_Planes[idx].enabled;
	}

	// Enable the specified clip plane.
	void EnablePlane(int idx, bool bEnabled)
	{
		FASSERT(idx >= 0 && idx < VP_NUM);
		m_Planes[idx].enabled = bEnabled;
	}

protected:
	ClipPlane m_Planes[VP_NUM];		// Clip planes of frustum.
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_FRUSTUM_H__