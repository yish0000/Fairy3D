//------------------------------------------------------------------------
// Name: FlyQuat.h
// Desc: This file define a Quat class for the Math library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_QUAT_H__
#define __FLY_QUAT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"
//////////////////////////////////////////////////////////////////////////


// Class for the Quat.
class _MATH_EXPORT FlyQuat
{
public:
    float x, y, z, w;

    //---------------------------------------

    FlyQuat(void) { x=0.0f, y=0.0f, z=0.0f, w=1.0f; }
    FlyQuat(float _x, float _y, float _z, float _w)
    { x=_x; y=_y; z=_z; w=_w; }

    void  FromAngleAxis( FlyVector vAxis,float fAngle );
    void  MakeFromMatrix( FlyMatrix mRot );
    void  MakeFromEuler(float fPitch, float fYaw, float fRoll);
    void  Normalize();
    void  Conjugate(FlyQuat q);
    void  GetEulers(float *fPitch, float *fYaw, float *fRoll);
    void  GetMatrix(FlyMatrix *pMat);
    float GetMagnitude(void);
    FlyQuat Lerp( float f,FlyQuat p,FlyQuat q );
    FlyQuat Slerp( float f,FlyQuat p,FlyQuat q );
    FlyQuat Squad( float f,FlyQuat p,FlyQuat q );
    FlyQuat Spline( float f,FlyQuat p,FlyQuat q );

    void    operator /= (float f);
    FlyQuat operator /  (float f);

    void    operator *= (float f);
    FlyQuat operator *  (float f);

    FlyQuat operator *  (const FlyVector &v) const;

    FlyQuat operator *  (const FlyQuat &q) const;
    void    operator *= (const FlyQuat &q);

    void    operator += (const FlyQuat &q);
    FlyQuat operator +  (const FlyQuat &q) const;
    void    operator -= (const FlyQuat &q);
    FlyQuat operator -  (const FlyQuat &q) const;

    FlyQuat operator~(void) const { return FlyQuat(-x, -y, -z, w); }

    void Rotate(const FlyQuat &q1, const FlyQuat &q2);
    FlyVector Rotate(const FlyVector &v);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_QUAT_H__