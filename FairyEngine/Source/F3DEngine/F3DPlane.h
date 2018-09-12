/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPlane.h
 *  Desc:   This file define the plane class for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_PLANE_H__
#define __F3D_PLANE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** Plane
A plane is defined in 3D space by the equation
Ax + By + Cz + D = 0,
a constant (D) which is the distance along
the normal you have to go to move the plane back to the origin.
*/
class FAIRY_API F3DPlane
{
public:
    F3DVector3 m_vN;	// Normal of the plane.
    float m_fD;         // Distant to the origin.

public:

	// Constructors
    F3DPlane(void) {}
    F3DPlane( const F3DVector3& vN, float fD ) : m_vN(vN), m_fD(fD) {}
	F3DPlane( const F3DVector3& vN, const F3DVector3& vP ) { Set(vN, vP); }
	F3DPlane( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2 ) { Set(v0, v1, v2); }

	// Build the plane by a normal and a point.
    void Set( const F3DVector3& vN, const F3DVector3& vP )
    {
        m_vN = vN;
        m_vN.Normalize();
        m_fD = -(m_vN * vP);
    }

    // Build the plane.
    void Set( const F3DVector3& vN, float fD )
    {
        m_vN = vN;
        m_vN.Normalize();
        m_fD = fD;
    }

    // Build the plane by three points.
    void Set( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2 )
    {
        F3DVector3 vEdge1 = v1 - v0;
        F3DVector3 vEdge2 = v2 - v0;
        m_vN.Cross( vEdge1,vEdge2 );
        m_vN.Normalize();
        m_fD = -(m_vN * v0);
    }

    // Normalize the plane.
    void Normalize(void);

	// Get the distant to the point. (Absolute value)
    float Distance( const F3DVector3& point ) const;
    // Get the distant to the point.
    float DotCoord( const F3DVector3& point ) const;

	// Classify the relation between point and plane. 
    int Classify( const F3DVector3& point ) const;
	// Classify the relation between specified polygon and plane. 
    int Classify( const F3DPolygon& polygon ) const;
	// Classify the relation between specified AABB and plane. 
    int Classify( const F3DAABB& box ) const;

	// Clip the specified ray.
    bool Clip( const F3DRay& ray, float fL, F3DRay* pF, F3DRay* pB, float* t ) const;

	// Check whether the triangle intersect with me.
    bool Intersect( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2 ) const;
	// Check whether the plane intersect with me.
    bool Intersect( const F3DPlane& plane, F3DRay* pIntersection ) const;
	// Check whether the AABB intersect with me.
    bool Intersect( const F3DAABB& box ) const;
	// Check whether the OBB intersect with me.
    bool Intersect( const F3DOBB& box ) const;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PLANE_H__