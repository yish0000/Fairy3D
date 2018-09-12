/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRay.h
 *  Desc:   This file implements the ray class for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_RAY_H__
#define __F3D_RAY_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** Ray
*/
class FAIRY_API F3DRay
{
public:
    F3DVector3 m_vOrig;  // Original point of the ray.
    F3DVector3 m_vDir;   // Direction of the ray.

public:

	// Constructors
    F3DRay() {}
    F3DRay(const F3DVector3& orig, const F3DVector3& dir) : m_vOrig(orig), m_vDir(dir) {}

    // Set the values.
    void Set( const F3DVector3& vOrig, const F3DVector3& vDir )
    {
        m_vOrig = vOrig;
        m_vDir = vDir;
    }

	// Transform the ray.
    void DeTransform( const F3DMatrix4& m );

	// Check whether the triangle intersects with me.
    bool Intersect( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2, bool bCull, float* t ) const;
	// Check whether the triangle intersects with me.
    bool Intersect( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2, bool bCull, float fL, float* t ) const;
	// Check whether the plane intersects with me.
    bool Intersect( const F3DPlane& plane, bool bCull, float* t, F3DVector3* pvHit ) const;
	// Check whether the plane intersects with me.
    bool Intersect( const F3DPlane& plane, bool bCull, float fL, float* t, F3DVector3* pvHit ) const;
	// Check whether the AABB intersects with me.
    bool Intersect( const F3DAABB& box, float* t ) const;
	// Check whether the AABB intersects with me.
    bool Intersect( const F3DAABB& box, float fL, float* t ) const;
	// Check whether the OBB intersects with me.
    bool Intersect( const F3DOBB& box, float* t ) const;
	// Check whether the OBB intersects with me.
    bool Intersect( const F3DOBB& box, float fL, float* t ) const;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RAY_H__