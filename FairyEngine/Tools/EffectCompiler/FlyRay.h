//------------------------------------------------------------------------
// Name: FlyRay.h
// Desc: This file define a Ray class for the Math library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RAY_H__
#define __FLY_RAY_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"
//////////////////////////////////////////////////////////////////////////


// Class for the Ray.
class _MATH_EXPORT FlyRay
{
public:
    FlyVector m_vcOrig;  // ray origin
    FlyVector m_vcDir;   // ray direction

    //---------------------------------------

    FlyRay(void) { /* nothing to do */ ; }

    void Set(FlyVector vcOrig, FlyVector vcDir);
    void DeTransform(const FlyMatrix &_m); // move to matrixspace

    bool Intersects(const FlyVector &vc0, const FlyVector &vc1,
                    const FlyVector &vc2, bool bCull,
                    float *t);
    bool Intersects(const FlyVector &vc0, const FlyVector &vc1,
                    const FlyVector &vc2, bool bCull,
                    float fL, float *t);
    bool Intersects(const FlyPlane &plane, bool bCull,
                    float *t, FlyVector *vcHit);       
    bool Intersects(const FlyPlane &plane, bool bCull,
                    float fL, float *t, FlyVector *vcHit);
    bool Intersects(const FlyAabb &aabb, float *t);
    bool Intersects(const FlyAabb &aabb, float fL, float *t);
    bool Intersects(const FlyObb &obb, float *t);
    bool Intersects(const FlyObb &obb, float fL, float *t);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RAY_H__