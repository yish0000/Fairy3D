//------------------------------------------------------------------------
// Name: FlyPlane.h
// Desc: This file define a Plane class for the Math library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_PLANE_H__
#define __FLY_PLANE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"
//////////////////////////////////////////////////////////////////////////


// Class for the Plane.
class _MATH_EXPORT FlyPlane
{
public:
    FlyVector m_vcN;       // plane normal vector
    float     m_fD;        // distance to origin (negative value)

    //---------------------------------------

    FlyPlane(void) { /* nothing to do */ ; }

    void  Set(const FlyVector &vcN, const FlyVector &vcP);
    void  Set(const FlyVector &vcN, float fD);
    void  Set(const FlyVector &v0,  const FlyVector &v1, const FlyVector &v2);
    float Distance(const FlyVector &vcPoint) const;
    float DotCoord(const FlyVector &vcPoint) const;
    int   Classify(const FlyVector &vcPoint) const;
    int   Classify(const FlyPolygon &Polygon) const;
    int   Classify( const FlyAabb& bbox ) const;
    void  Normalize(void);

    bool Clip(const FlyRay* pRay,float fL,FlyRay* pF,FlyRay* pB,float* t);

    bool Intersects(const FlyVector &vc0, const FlyVector &vc1, 
                    const FlyVector &vc2);
    bool Intersects(const FlyPlane &plane, FlyRay *pIntersection);
    bool Intersects(const FlyAabb &aabb);
    bool Intersects(const FlyObb &obb);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_PLANE_H__