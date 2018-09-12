//------------------------------------------------------------------------
// Name: FlyVector.h
// Desc: This file define a class Vector4D for the library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_VECTOR_H__
#define __FLY_VECTOR_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"
//////////////////////////////////////////////////////////////////////////


// Class for the Vector4D.
class _MATH_EXPORT FlyVector
{
public:
    float x,y,z,w;      // Elements of vector

    //================================================
    FlyVector(void)
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;
    }

    FlyVector( float _x,float _y,float _z,float _w=1.0f )
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    // Set the element's value
    void    Set( float _x,float _y,float _z,float _w=1.0f );

    // Get the vector's length
    float   GetLength(void) const;

    // Get square length
    float   GetSqrLength(void) const;

    // Vector multi -1
    void    Negate(void);

    // Normalize the vector
    void    Normalize(void);

    // Get the angle of two vector
    float   AngleWith( FlyVector& v ) const;

    // Rotate the vector
    void    RotateWith( const FlyMatrix& );

    // Apply inverse rotation
    void    InvRotateWith( const FlyMatrix& );

    // From v1 to v2
    void    Difference( const FlyVector& v1,const FlyVector& v2 );

    //================================================

    void operator += (const FlyVector& v);
    void operator -= (const FlyVector& v);

    void operator += (const float);
    void operator -= (const float);
    void operator *= (const float);
    void operator /= (const float);

    bool operator == (const FlyVector& v) const;

    float operator * (const FlyVector& v) const;        // Dot product
    FlyVector operator * (const float) const;           // Scale the vector up
    FlyVector operator / (const float) const;           // Scale the vector down
    FlyVector operator + (const float) const;
    FlyVector operator - (const float) const;

    FlyQuat	operator * (const FlyQuat& q) const;        // Vector multi a quaternion
    FlyVector operator * (const FlyMatrix& m) const;    // Vector multi a matrix product
    FlyVector operator + (const FlyVector& v) const;    // Add a vector
    FlyVector operator - (const FlyVector& v) const;    // Subtract a vector

    // Get the cross product of two vector
    void Cross( const FlyVector& v1,const FlyVector& v2 );

    static const FlyVector ZERO;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_VECTOR_H__