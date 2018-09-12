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
    FlyVector vcCenter;     // centerpoint

    //---------------------------------------

    FlyAabb(void) { /* nothing to do */ ; }
    FlyAabb(FlyVector vcMin, FlyVector vcMax);

    void Construct(const FlyObb *pObb); // build from obb
    int  Cull(const FlyPlane *pPlanes, int nNumPlanes);

    // Merge with another box.
    void Merge( const FlyAabb& box );

    // normals pointing outwards
    void GetPlanes(FlyPlane *pPlanes);

    bool Contains(const FlyRay &Ray, float fL);
    bool Intersects(const FlyRay &Ray, float *t);
    bool Intersects(const FlyRay &Ray, float fL, float *t);
    bool Intersects(const FlyAabb &aabb);
    bool Intersects(const FlyVector &vc0);
    bool IsInBox( const FlyAabb& box );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_AABB_H__
