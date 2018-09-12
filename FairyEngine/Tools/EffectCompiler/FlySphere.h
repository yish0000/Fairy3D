//------------------------------------------------------------------------
// Name: FlySphere.h
// Desc: This file define a Bounding Sphere for the Math Library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_SPHERE_H__
#define __FLY_SPHERE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"
//////////////////////////////////////////////////////////////////////////


class _MATH_EXPORT FlySphere
{
public:
    FlyVector   m_vCenter;
    float       m_fRadius;

    // Standard Constructor
    FlySphere(void);
    FlySphere( const FlyVector& vCenter,float fRadius );

    void Set( FlyVector vCenter,float fRadius );

    // Check the intersecting
    bool Intersects( const FlyVector& vPoint );
    bool Intersects( const FlyPlane& plane );
    bool Intersects( const FlyAabb& aabb );
    bool Intersects( const FlySphere& sphere );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SPHERE_H__