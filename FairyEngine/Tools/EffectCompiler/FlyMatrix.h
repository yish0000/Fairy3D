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

    // Constructor of the class
    FlyMatrix(void) { /* nothing to do */ ; }

    void Identity(void);                    // identity matrix
    void RotaX(float a);                    // x axis
    void RotaY(float a);                    // y axis
    void RotaZ(float a);                    // z axis
    void Rota(const FlyVector &vc);         // x, y and z
    void Rota(float x, float y, float z);   // x, y and z
    void RotaArbi(const FlyVector &vcAxis, float a);
    void ApplyInverseRota(FlyVector *pvc);
    void Translate(float dx, float dy, float dz);
    void SetTranslation(FlyVector vc, bool EraseContent=false);
    FlyVector GetTranslation(void);

    float GetValue( int x,int y );

    void Billboard(FlyVector vcPos, FlyVector vcDir,
                   FlyVector vcWorldUp = FlyVector(0,1,0));
    void LookAt(FlyVector vcPos, FlyVector vcLookAt, 
                FlyVector vcWorldUp = FlyVector(0,1,0));

    void TransposeOf(const FlyMatrix &m);   // transpose m, save result in this
    void InverseOf(const FlyMatrix &m);     // invert m, save result in this

    FlyMatrix operator * (const FlyMatrix &m) const;    // matrix multiplication
    FlyVector operator * (const FlyVector &vc) const;   // matrix vector multiplication
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MATRIX_H__