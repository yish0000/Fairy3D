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

    void  InverseOf( const FlyQuat& quat );
    void  FromAngleAxis( const FlyVector& vAxis,float fAngle );
    void  GetAngleAxis( FlyVector* pAxis,float* pAngle );
    void  MakeFromMatrix( const FlyMatrix& mRot );
    void  MakeFromEuler(float fPitch, float fYaw, float fRoll);
    void  Normalize();
    void  Conjugate(const FlyQuat& q);
    void  GetEulers(float *fPitch, float *fYaw, float *fRoll) const;
    void  GetMatrix(FlyMatrix *pMat) const;
    float GetMagnitude(void) const;
    FlyQuat Lerp( float f,const FlyQuat& p,const FlyQuat& q );
    FlyQuat Slerp( float f,const FlyQuat& p,const FlyQuat& q );
    float GetPitch(void) const;
    float GetYaw(void) const;
    float GetRoll(void) const;

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

    static const FlyQuat IDENTITY;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_QUAT_H__