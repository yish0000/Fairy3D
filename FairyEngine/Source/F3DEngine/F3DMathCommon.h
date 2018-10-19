/*
 * ------------------------------------------------------------------------
 *  Name:   F3DMathCommon.h
 *  Desc:   本文件包含一些数学库必要的头文件，定义了一些公共的宏和常量。
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_MATHCOMMON_H__
#define __F3D_MATHCOMMON_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"
#include "FTemplate.h"

#include <math.h>
#include <float.h>

///////////////////////////////////////////////////////////////////////////

//
// Define some constant variables for math library
//

const float F3D_PI = 3.14159265f;
const float F3D_PI2 = 1.570796325f;
const float F3D_2PI = 6.2831853f;
const float F3D_G = -32.174f;
const float F3D_EPSILON = 0.00001f;
const float F3D_DEG2RAD = 0.0174532924f;
const float F3D_RAD2REG = 57.29578f;
const float F3D_FLOAT_MAX = FLT_MAX;
const float F3D_FLOAT_MIN = FLT_MIN;

//
// Define some macros for math library
//

#define F3D_MATH_FRONT        0
#define	F3D_MATH_BACK         1
#define F3D_MATH_PLANAR       2

#define F3D_MATH_CLIPPED      3
#define F3D_MATH_CULLED       4
#define F3D_MATH_VISIBLE      5

#define F3D_MATH_INSIDE       6
#define F3D_MATH_OUTSIDE      7
#define F3D_MATH_INTERSECT    8

///////////////////////////////////////////////////////////////////////////

// Pre-define all the classes
class F3DVector2;
class F3DVector3;
class F3DVector4;
class F3DMatrix3;
class F3DMatrix4;
class F3DRay;
class F3DAABB;
class F3DOBB;
class F3DPlane;
class F3DQuat;
class F3DPolygon;
class F3DCapsule;
class F3DCylinder;
class F3DSphere;

///////////////////////////////////////////////////////////////////////////
//// Define some functions for math library

class FAIRY_API F3DMath
{
public:
    static bool Approximately(float f1, float f2) { return F_Abs(f1 - f2) < F3D_EPSILON; }
    static bool ApproximateZero(float f) { return f > -1e-7f && f < 1e-7f; }
    static float Sign(float f) { return f < 0.0f ? -1.0f : 1.0f; }
    static float Deg2Rad(float fDeg) { return fDeg * F3D_DEG2RAD; }
    static float Rad2Deg(float fRad) { return fRad * F3D_RAD2REG; }
    static float Repeat(float f, float len) { return f - floorf(f / len) * len; }
    static float Clamp01(float f)
    {
        if( f > 1.0f ) return 1.0f;
        if( f < 0.0f ) return 0.0f;
        return f;
    }
    
    static float Lerp(float a, float b, float t)
    {
        return a + (b - a) * Clamp01(t);
    }
    
    static float LerpAngle(float a, float b, float t)
    {
        float num = Repeat(b - a, 360.0f);
        if( num > 180.0f )
            num -= 360.0f;
        return a + num * Clamp01(t);
    }
    
    static float PingPong(float t, float len)
    {
        t = Repeat(t, len);
        return len - F_Abs(t - len);
    }
    
    // Calculate the delta of two angle.
    static float DeltaAngle(float current, float target)
    {
        float num = Repeat(target - current, 360.0f);
        if( num > 180.0f )
            num -= 360.0f;
        return num;
    }
    
    // Calculate the 1 / f, for f * f = r
    static float InvSqrt(float r)
    {
#if FAIRY_COMPILER == FAIRY_COMPILER_MSVC && defined(FAIRY_PLATFORM_32)
        __asm {
            fld1 // r0 = 1.f
            fld r // r1 = r0, r0 = r
            fsqrt // r0 = sqrtf( r0 )
            fdiv // r0 = r1 / r0
        } // returns r0
#else
        return 1.0f / sqrtf(r);
#endif
    }
    
    // Fast inverse square root calculation.
    static float InvSqrtFast(float v);
    
    
};

F_INLINE bool F3D_FloatEqual(float f1, float f2) { return F3DMath::Approximately(f1, f2); }

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_MATHCOMMON_H__
