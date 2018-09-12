/*
 * ------------------------------------------------------------------------
 *  Name:   F3DSphere.h
 *  Desc:   This file define a sphere class for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_SPHERE_H__
#define __F3D_SPHERE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** Sphere
*/
class FAIRY_API F3DSphere
{
public:
    F3DVector3 m_vCenter;   // Center of the sphere.
    float m_fRadius;        // Radius of the sphere.

public:

	// Constructors
    F3DSphere() {}
    F3DSphere( const F3DVector3& vCenter, float fRadius )
        : m_vCenter(vCenter),m_fRadius(fRadius)
    {
    }

    // Set the values.
    void Set( const F3DVector3& vCenter, float fRadius )
    {
        m_vCenter = vCenter;
        m_fRadius = fRadius;
    }

    // Check whether the sphere contain the specified AABB.
    int ContainBox( const F3DAABB& box ) const;

	// Check whether the point intersect with me.
    bool Intersect( const F3DVector3& point ) const;
	// Check whether the specified plane intersect with me.
    bool Intersect( const F3DPlane& plane ) const;
	// Check whether the specified AABB intersect with me.
    bool Intersect( const F3DAABB& box ) const;
	// Check whether the specified sphere intersect with me.
    bool Intersect( const F3DSphere& sphere ) const;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_SPHERE_H__