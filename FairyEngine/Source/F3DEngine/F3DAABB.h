/*
 * ------------------------------------------------------------------------
 *  Name:   F3DAABB.h
 *  Desc:   This file define the Axis aligned bounding box for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_AABB_H__
#define __F3D_AABB_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** AABB (Axis aligned bounding box.)
*/
class FAIRY_API F3DAABB
{
public:

	/** The corner of the AABB.
	   5-----6
	  /|    /|
	 / |   / |
	4-----7  |
	|  1--|--2
	| /   | /
	|/    |/
	0-----3
	*/

	enum
	{
		CORNER_LDB,
		CORNER_LUB,
		CORNER_RUB,
		CORNER_RDB,
		CORNER_LDT,
		CORNER_LUT,
		CORNER_RUT,
		CORNER_RDT,
	};

	// Define some constant variables.
	static const F3DAABB BOX_EMPTY;
	static const F3DAABB BOX_INFINITE;

public:
    F3DVector3 vMin;     // Maximum point of the box.
    F3DVector3 vMax;     // Minimum point of the box.

public:

	// Constructors.
    F3DAABB(void) {}
    F3DAABB(const F3DVector3& min, const F3DVector3& max) : vMin(min), vMax(max) {}
	F3DAABB(const F3DOBB& obb) { Construct(obb); }
	F3DAABB(const F3DSphere& sphere) { Construct(sphere); }

    // Set the values.
    void Set( const F3DVector3& min, const F3DVector3& max )
    {
        vMax = max;
        vMin = min;
    }

	// Zero the AABB
	void Zero()
	{
		vMax.Zero();
		vMin.Zero();
	}

    // Construct the AABB from OBB.
    void Construct( const F3DOBB& obb );
    // Construct the AABB from sphere.
    void Construct( const F3DSphere& sphere );
	// Construct the AABB from vertices
	void Construct( const F3DVector3* pVertices, uint32 nNumVerts );

	// Get the center point.
	F3DVector3 GetCenter(void) const;
	// Get the extents of box.
	F3DVector3 GetExtents(void) const;
	// Get the specified corner.
	F3DVector3 GetCorner( int cornerID ) const;

	// Get the six planes of box.
	void GetPlanes(F3DPlane* planes) const;

	// Is the box a empty box ?
	bool IsEmpty(void) const
	{
		return F3D_FloatEqual(vMin.x, vMax.x)
			&& F3D_FloatEqual(vMin.y, vMax.y)
			&& F3D_FloatEqual(vMin.z, vMax.z);
	}

	// Check whether the AABB is culled by the specified clip planes ?
    int Cull( const F3DPlane* planes, int iNumPlanes ) const;

    // Merge another box with me.
    void Merge( const F3DAABB& box );
    // Merge the specified point.
    void Merge( const F3DVector3& point );

	// Transform the AABB.
	void TransformBox( const F3DMatrix4& m );

	// Get the bounding sphere's radius.
	float GetBoundingRadius(void) const;

	// Check whether the box contain another AABB.
	int ContainBox( const F3DAABB& box ) const;
    // Check whether the box contain the specified ray.
    bool Contain( const F3DRay& ray, float fL ) const;
    // Check whether the box contain the specified point.
    bool Contain( const F3DVector3& point ) const;

    // Check whether the AABB intersect with the ray.
    bool Intersect( const F3DRay& ray, float* t ) const;
    // Check whether the AABB intersect with the ray.
    bool Intersect( const F3DRay& ray, float fL, float* t ) const;
    // Check whether the AABB intersect with another AABB.
    bool Intersect( const F3DAABB& box ) const;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_AABB_H__
