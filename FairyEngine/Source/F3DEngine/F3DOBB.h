/*
 * ------------------------------------------------------------------------
 *  Name:   F3DOBB.h
 *  Desc:   This file define an oriented bounding box for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_OBB_H__
#define __F3D_OBB_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** OBB (Oriented Bounding Box)
*/
class FAIRY_API F3DOBB
{
public:
    float fA0, fA1, fA2;        // Half of the length of each edges
    F3DVector3 vA0, vA1, vA2;   // Three axes.
    F3DVector3 vCenter;			// Center of the box.

public:
    F3DOBB(void) {};
    F3DOBB( const F3DVector3& center, const F3DVector3& v0, const F3DVector3& v1,
        const F3DVector3& v2, float a0, float a1, float a2 )
        : vCenter(center), vA0(v0), vA1(v1), vA2(v2), fA0(a0), fA1(a1), fA2(a2)
    {
    }

    // Set the values
    void Set( const F3DVector3& center, const F3DVector3& v0, const F3DVector3& v1,
        const F3DVector3& v2, float a0, float a1, float a2 )
    {
		vCenter = center;
		vA0 = v0; vA1 = v1; vA2 = v2;
		fA0 = a0; fA1 = a1; fA2 = a2;
    }

	// Zero the OBB.
	void Zero();

	// Construct the OBB from vertices
	void Construct(const F3DVector3* pVertices, uint32 nNumVerts);
	// Construct the OBB from AABB
	void Construct(const F3DAABB& aabb);
	// Construct the OBB from two other OBBs.
	void Construct(const F3DOBB& obb1, const F3DOBB& obb2);

    // Transform me from another OBB.
    void DeTransform( const F3DOBB& box, const F3DMatrix4& m );
	// Transform me with a matrix.
	void DeTransform( const F3DMatrix4& m );

	// Is the specified point in the box ?
	bool Contain(const F3DVector3& vPoint) const;

    // Check whether the OBB is culled by the specified clip planes.
    int Cull( const F3DPlane* planes, int numPlanes ) const;

	// Check whether the specified ray intersect with me ?
    bool Intersect( const F3DRay& ray, float* t ) const;
    // Check whether the specified ray intersect with me ?
    bool Intersect( const F3DRay& ray, float fL, float* t ) const;
    // Check whether another OBB intersect with me ?
    bool Intersect( const F3DOBB& box ) const;
    // Check whether the triangle intersect with me ?
    bool Intersect( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2 ) const;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_OBB_H__