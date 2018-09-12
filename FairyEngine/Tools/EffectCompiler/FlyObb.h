//------------------------------------------------------------------------
// Name: FlyObb.h
// Desc: This file define a Obb class for the Math library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_OBB_H__
#define __FLY_OBB_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"
//////////////////////////////////////////////////////////////////////////


// Class for the Obb.
class _MATH_EXPORT FlyObb
{
public:
    float     fA0,fA1,fA2;      // half axis length
    FlyVector vcA0,vcA1,vcA2;   // box axis
    FlyVector vcCenter;         // centerpoint

    //---------------------------------------

    FlyObb(void) { /* nothing to do */ ; }

    void DeTransform(const FlyObb &obb, 
                     const FlyMatrix &m);

    bool Intersects(const FlyRay &Ray, float *t);
    bool Intersects(const FlyRay &Ray, float fL, float *t);
    bool Intersects(const FlyObb &Obb);
    bool Intersects(const FlyVector &v0, 
                    const FlyVector &v1,
                    const FlyVector &v2);

    int  Cull(const FlyPlane *pPlanes, int nNumPlanes);      

private:
    void ObbProj(const FlyObb &Obb, const FlyVector &vcV, 
                 float *pfMin, float *pfMax);
    void TriProj(const FlyVector &v0, const FlyVector &v1, 
                 const FlyVector &v2, const FlyVector &vcV, 
                 float *pfMin, float *pfMax);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_OBB_H__