//------------------------------------------------------------------------
// Name: FlyMatrix.h
// Desc: This file define a Matrix4 class for the Math library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_MATRIX_H__
#define __FLY_MATRIX_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"

//////////////////////////////////////////////////////////////////////////


// Class for the Matrix4.
class _MATH_EXPORT FlyMatrix
{
public:
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;

    //---------------------------------------

    // Default Constructor of the class
    FlyMatrix(void) { /* Nothing to do! */ };

    // Constructor of the class.
    FlyMatrix( float f11,float f12,float f13,float f14,
               float f21,float f22,float f23,float f24,
               float f31,float f32,float f33,float f34,
               float f41,float f42,float f43,float f44 );

    void Identity(void);                    // identity matrix
    void RotaX(float a);                    // x axis
    void RotaY(float a);                    // y axis
    void RotaZ(float a);                    // z axis
    void RotaEulerXYZ(float pitch, float yaw, float roll);
    void RotaEulerXZY(float pitch, float roll, float yaw);
    void RotaEulerYXZ(float yaw, float pitch, float roll);
    void RotaEulerYZX(float yaw, float roll, float pitch);
    void RotaEulerZXY(float roll, float pitch, float yaw);
    void RotaEulerZYX(float roll, float yaw, float pitch);
    void RotaArbi(const FlyVector &vcAxis, float a);
    void ApplyInverseRota(FlyVector *pvc);
    void Translate(float dx, float dy, float dz);
    void SetTranslation(FlyVector vc, bool EraseContent=false);
    FlyVector GetTranslation(void);

    float GetValue( int x,int y ) const;

    void Billboard(FlyVector vcPos, FlyVector vcDir,
                   FlyVector vcWorldUp = FlyVector(0,1,0));
    void LookAt(FlyVector vcPos, FlyVector vcLookAt, 
                FlyVector vcWorldUp = FlyVector(0,1,0));

    void Reflection( const FlyPlane& plane );

    void TransposeOf(const FlyMatrix &m);   // transpose m, save result in this
    void InverseOf(const FlyMatrix &m);     // invert m, save result in this

    void MakeTransform( const FlyVector& vPos,const FlyQuat& qOrienuation,
                        const FlyVector& vScale );

    FlyMatrix operator * (const FlyMatrix &m) const;    // matrix multiplication
    FlyVector operator * (const FlyVector &vc) const;   // matrix vector multiplication

    static const FlyMatrix IDENTITY;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MATRIX_H__