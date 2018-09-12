//------------------------------------------------------------------------
// Name: FlyQuat.cpp
// Desc: This file is to implement the class FlyQuat.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyVector.h"
#include "FlyMatrix.h"
#include "FlyQuat.h"


//------------------------------------------------------------------------
// get the mgnitude of a quaternion
//------------------------------------------------------------------------
float FlyQuat::GetMagnitude(void)
{
	return sqrtf(x*x + y*y + z*z + w*w);
}


//------------------------------------------------------------------------
void FlyQuat::Normalize(void)
{
	float m=sqrtf(x*x + y*y + z*z + w*w);
	if(m!=0) 
	{
		x /= m;
		y /= m;
		z /= m;
		w /= m;
	}
}


//------------------------------------------------------------------------
void FlyQuat::Conjugate(FlyQuat q)
{
	x=-q.x;
	y=-q.y;
	z=-q.z;
	w=q.w;
}


//------------------------------------------------------------------------
void FlyQuat::Rotate(const FlyQuat &q1, const FlyQuat &q2)
{
	FlyQuat t = q1*q2*(~q1); 
	x = t.x;
	y = t.y;
	z = t.z;
	w = t.w;
}


//------------------------------------------------------------------------
FlyVector FlyQuat::Rotate(const FlyVector &v)
{
	FlyQuat t(x, y, z, w);
	FlyQuat r = t*v*(~t);

	return FlyVector(r.x, r.y, r.z);
}


//------------------------------------------------------------------------
// Make a quaternion from a rotation matrix. This code is from the 
// OGRE engine's source.
//------------------------------------------------------------------------
void FlyQuat::MakeFromMatrix( FlyMatrix mRot )
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    float fTrace = mRot._11 + mRot._22 + mRot._33;
    float fRoot;

    if ( fTrace > 0.0f )
    {
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = (float)sqrt(fTrace + 1.0f);  // 2w
        w = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;  // 1/(4w)
        x = (mRot._32 - mRot._23)*fRoot;
        y = (mRot._13 - mRot._31)*fRoot;
        z = (mRot._21 - mRot._12)*fRoot;
    }
    else
    {
        // |w| <= 1/2
        int i = 0;
        int iNext[3] = { 1, 2, 0 };

        if ( mRot._22 > mRot._11 )
            i = 1;
        if ( mRot._33 > mRot.GetValue(i,i) )
            i = 2;

        int j = iNext[i];
        int k = iNext[j];

        fRoot = (float)sqrt(mRot.GetValue(i,i)-mRot.GetValue(j,j)-mRot.GetValue(k,k)+1.0f);
        float* apkQuat[3] = { &x, &y, &z };
        *apkQuat[i] = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;
        w = (mRot.GetValue(k,j)-mRot.GetValue(j,k))*fRoot;
        *apkQuat[j] = (mRot.GetValue(j,i)+mRot.GetValue(i,j))*fRoot;
        *apkQuat[k] = (mRot.GetValue(k,i)+mRot.GetValue(i,k))*fRoot;
    }
}


//------------------------------------------------------------------------
// get a rotation matrix out of the quaternion
//------------------------------------------------------------------------
void FlyQuat::GetMatrix(FlyMatrix *pMat)
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    // set matrix to identity
    memset(pMat, 0, sizeof(FlyMatrix));
    pMat->_44 = 1.0f;

    x2 = x * 2.0f; 
    y2 = y * 2.0f; 
    z2 = z * 2.0f;

    xx = x * x2;
    xy = x * y2;
    xz = x * z2;

    yy = y * y2;   
    yz = y * z2;   
    zz = z * z2;

    wx = w * x2;   
    wy = w * y2;   
    wz = w * z2;

    pMat->_11 = 1.0f - (yy + zz);
    pMat->_12 = xy - wz;
    pMat->_13 = xz + wy;

    pMat->_21 = xy + wz;
    pMat->_22 = 1.0f - (xx + zz);
    pMat->_23 = yz - wx;

    pMat->_31 = xz - wy;
    pMat->_32 = yz + wx;
    pMat->_33 = 1.0f - (xx + yy);
}


//------------------------------------------------------------------------
// Construct a quaternion from the axis and a angle.
//------------------------------------------------------------------------
void FlyQuat::FromAngleAxis( FlyVector vAxis,float fAngle )
{
    float fSin = sinf( fAngle / 2.0f );

    x = fSin*vAxis.x;
    y = fSin*vAxis.y;
    z = fSin*vAxis.z;
    w = cosf( fAngle / 2.0f );
}


//------------------------------------------------------------------------
// construct quaternion from Euler angles
//------------------------------------------------------------------------
void FlyQuat::MakeFromEuler(float fPitch, float fYaw, float fRoll)
{
	float cX, cY, cZ, sX, sY, sZ, cYcZ, sYsZ, cYsZ, sYcZ;

	fPitch *= 0.5f;
	fYaw   *= 0.5f;
	fRoll  *= 0.5f;

	cX = cosf(fPitch);
	cY = cosf(fYaw);
	cZ = cosf(fRoll);

	sX = sinf(fPitch);
	sY = sinf(fYaw);
	sZ = sinf(fRoll);

	cYcZ = cY * cZ;
	sYsZ = sY * sZ;
	cYsZ = cY * sZ;
	sYcZ = sY * cZ;

	w = cX * cYcZ + sX * sYsZ;
	x = sX * cYcZ - cX * sYsZ;
	y = cX * sYcZ + sX * cYsZ;
	z = cX * cYsZ - sX * sYcZ;
}


//------------------------------------------------------------------------
// Make a quaternion from a rotation matrix.
// Source: Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course 
// notes, article "Quaternion Calculus and Fast Animation".
//------------------------------------------------------------------------



//------------------------------------------------------------------------
// converts quaternion into euler angles
//------------------------------------------------------------------------
void FlyQuat::GetEulers(float *fPitch, float *fYaw, float *fRoll)
{
	double   r11, r21, r31, r32, r33, r12, r13;
	double   q00, q11, q22, q33;
	double   tmp;

	q00 = w * w;
	q11 = x * x;
	q22 = y * y;
	q33 = z * z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (x*y + w*z);
	r31 = 2 * (x*z - w*y);
	r32 = 2 * (y*z + w*x);
	r33 = q00 - q11 - q22 + q33;

	tmp = fabs(r31);
	if(tmp > 0.999999)
	{
		r12 = 2 * (x*y - w*z);
		r13 = 2 * (x*z + w*y);

		*fPitch = 0.0f;
		*fYaw   = (float)-((FLY_PI/2) * r31/tmp);
		*fRoll  = (float)atan2(-r12, -r31*r13);
	}
	else
	{
		*fPitch = (float)atan2(r32, r33);
		*fYaw   = (float)asin(-r31);
		*fRoll  = (float)atan2(r21, r11);
	}
}


//------------------------------------------------------------------------
// Interpolate a quaternion with line method.
//------------------------------------------------------------------------
FlyQuat FlyQuat::Lerp( float f,FlyQuat p,FlyQuat q )
{
    FlyQuat tmp;

    tmp.x = (1.0f - f) * p.x + q.x;
    tmp.y = (1.0f - f) * p.y + q.y;
    tmp.z = (1.0f - f) * p.z + q.z;
    tmp.w = (1.0f - f) * p.w + q.w;

    return tmp;
}


//------------------------------------------------------------------------
// Interpolate a quaternion with sphereical linear method.
// This code is from Ogre3D Source.
//------------------------------------------------------------------------
FlyQuat FlyQuat::Slerp( float f,FlyQuat p,FlyQuat q )
{
    FlyQuat tmp;
    float fCos = p.x*q.x+p.y*q.y+p.z*q.z+p.w*q.w;

    // Do we need to invert rotation?
    if( fCos < 0.0f )
    {
        fCos = -fCos;
        tmp = FlyQuat( -q.x,-q.y,-q.z,-q.w );
    }
    else
    {
        tmp = q;
    }

    if( fabs(fCos) < 1 - FLY_EPSILON )
    {
        // Standard case (slerp)
        float fSin = (float)sqrt( 1.0f - fCos*fCos );
        float fAngle = (float)atan2( fSin,fCos );
        float fInvSin = 1.0f / fSin;
        float fCoeff0 = sinf((1.0f - f) * fAngle) * fInvSin;
        float fCoeff1 = sinf(f * fAngle) * fInvSin;
        return p * fCoeff0 + tmp * fCoeff1;
    }
    else
    {
        // There are two situations:
        // 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
        //    interpolation safely.
        // 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
        //    are an infinite number of possibilities interpolation. but we haven't
        //    have method to fix this case, so just use linear interpolation here.
        FlyQuat t = p*(1.0f - f) + tmp*f;
        // taking the complement requires renormalisation
        t.Normalize();
        return t;
    }

    return tmp;
}


//------------------------------------------------------------------------
// Interpolate a quaternion with Spherical Cubic method.
//------------------------------------------------------------------------
FlyQuat FlyQuat::Squad( float f,FlyQuat p,FlyQuat q )
{
    return FlyQuat( 0.0f,0.0f,0.0f,0.0f );
}


//------------------------------------------------------------------------
// Interpolate a quaternion with Spline method.
//------------------------------------------------------------------------
FlyQuat FlyQuat::Spline( float f,FlyQuat p,FlyQuat q )
{
    return FlyQuat( 0.0f,0.0f,0.0f,0.0f );
}


//------------------------------------------------------------------------
// dives a quaternion by a scalar value
//------------------------------------------------------------------------
void FlyQuat::operator /= (float f)
{
	w /= f;
	x /= f;
	y /= f;
	z /= f;
}


//------------------------------------------------------------------------
FlyQuat FlyQuat::operator / (float f)
{
	return FlyQuat(x/f, y/f, z/f, w/f);
}


//------------------------------------------------------------------------
// multiplies a quaternion by a scalar value
//------------------------------------------------------------------------
void FlyQuat::operator *= (float f)
{
	w *= f;
	x *= f;
	y *= f;
	z *= f;
}


//------------------------------------------------------------------------
FlyQuat FlyQuat::operator * (float f)
{
	return FlyQuat(x*f, y*f, z*f, w*f);
}


//------------------------------------------------------------------------
// adds two quaternions
//------------------------------------------------------------------------
void FlyQuat::operator += (const FlyQuat &q)
{
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
}


//------------------------------------------------------------------------
FlyQuat FlyQuat::operator + (const FlyQuat &q) const
{
	return FlyQuat(x+q.x, y+q.y, z+q.z, w+q.w);
}


//------------------------------------------------------------------------
void FlyQuat::operator -= (const FlyQuat &q)
{
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;
}


//------------------------------------------------------------------------
FlyQuat FlyQuat::operator - (const FlyQuat &q) const
{
    return FlyQuat(x-q.x, y-q.y, z-q.z, w-q.w);
}


//------------------------------------------------------------------------
// multiplies two quaternions with *= operator
//------------------------------------------------------------------------
void FlyQuat::operator *= (const FlyQuat &q)
{
	float _x, _y, _z, _w;

	_w = w*q.w - x*q.x - y*q.y - z*q.z;
	_x = w*q.x + x*q.w + y*q.z - z*q.y;
	_y = w*q.y + y*q.w + z*q.x - x*q.z;
	_z = w*q.z + z*q.w + x*q.y - y*q.x;

	x = _x;
	y = _y;
	z = _z;
	w = _w;
}


//------------------------------------------------------------------------
// multiplies two quaternions with * operator
//------------------------------------------------------------------------
FlyQuat FlyQuat::operator * (const FlyQuat &q) const
{
	FlyQuat qResult;

	qResult.w = w*q.w - x*q.x - y*q.y - z*q.z;
	qResult.x = w*q.x + x*q.w + y*q.z - z*q.y;
	qResult.y = w*q.y + y*q.w + z*q.x - x*q.z;
	qResult.z = w*q.z + z*q.w + x*q.y - y*q.x;

	return qResult;
}


//------------------------------------------------------------------------
// multiplies a vector with a quaternion
//------------------------------------------------------------------------
FlyQuat FlyQuat::operator * (const FlyVector &v) const
{
   return FlyQuat(w*v.x + y*v.z - z*v.y,
                  w*v.y + z*v.x - x*v.z,
                  w*v.z + x*v.y - y*v.x,
                  -(x*v.x + y*v.y + z*v.z));
}