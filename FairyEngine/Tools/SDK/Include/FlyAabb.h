//------------------------------------------------------------------------
// Name: FlyAabb.h
// Desc: This file define a Aabb class for the Math library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_AABB_H__
#define __FLY_AABB_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"
//////////////////////////////////////////////////////////////////////////


// Class for the AABB.
class _MATH_EXPORT FlyAabb
{
public:
    FlyVector vcMin,vcMax;  // box extreme points
    bool m_bEmpty;          // Is the box empty ?

    //---------------------------------------

    FlyAabb(void) { m_bEmpty = false; }
    FlyAabb(FlyVector vcMin, FlyVector vcMax);

    void Construct(const FlyObb *pObb); // build from obb
    int  Cull(const FlyPlane *pPlanes, int nNumPlanes);

    // Merge with another box.
    void Merge( const FlyAabb& box );

    // Merge with a point.
    void Merge( const FlyVector& vPoint );

    // normals pointing outwards
    void GetPlanes(FlyPlane *pPlanes);

    void SetEmpty(void);
    bool IsEmpty(void) const;

    bool Contains(const FlyRay &Ray, float fL);
    bool Contains(const FlyVector &vc0);
    bool Intersects(const FlyRay &Ray, float *t);
    bool Intersects(const FlyRay &Ray, float fL, float *t);
    bool Intersects(const FlyAabb &aabb);
    int ContainBox( const FlyAabb& bbox ) const;

    void TransformBox( const FlyMatrix& mTransform );

    float GetBoundingRadius(void) const;

    // Get the center point of the box.
    FlyVector GetCenter(void) const;

    FlyVector GetHalfSize(void) const;

    enum CornerID
    {
        left_down_bottom,
        left_up_bottom,
        right_up_bottom,
        right_down_bottom,
        left_down_top,
        left_up_top,
        right_up_top,
        right_down_top,
    };

    // Get the eight corners of the box.
	/*
	   5-----6
	  /|    /|
	 / |   / |
	4-----7  |
	|  1--|--2
	| /   | /
	|/    |/
	0-----3
	*/
    FlyVector GetCorner( CornerID ID ) const;

    static const FlyAabb EMPTY;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_AABB_H__
