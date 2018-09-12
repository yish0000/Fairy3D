//------------------------------------------------------------------------
// Name: FlyMathCommon.h
// Desc: This file is the common header of the Math library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_MATH_COMMON_H__
#define __FLY_MATH_COMMON_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <assert.h>
//////////////////////////////////////////////////////////////////////////


// Define some constant variables for the libary.
const double FLY_PI      = 3.14159265;
const double FLY_PI2     = 1.5707963;
const double FLY_2PI     = 6.2831853;
const float  FLY_G       = -32.174f;
const float  FLY_EPSILON = 0.00001f;

// Define some macros for library.
#define FLY_FRONT    0
#define	FLY_BACK     1
#define FLY_PLANAR   2
#define FLY_CLIPPED  3
#define FLY_CULLED   4
#define FLY_VISIBLE  5

#ifdef _USRDLL
    #define _MATH_EXPORT __declspec(dllexport)
#else
    #define _MATH_EXPORT 
#endif

#define FLYMATH_MAX( a,b )  ( a > b ? a : b )
#define FLYMATH_MIN( a,b )  ( a < b ? a : b )

//------------------------------------------------------------------------

// Define some functions
inline float _fabs( float f ) { return f > 0.0f ? f : -f; }


// Pre-define the classes
class FlyVector;
class FlyMatrix;
class FlyRay;
class FlyAabb;
class FlyObb;
class FlyPlane;
class FlyQuat;
class FlyPolygon;
class FlySphere;


//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MATH_COMMON_H__