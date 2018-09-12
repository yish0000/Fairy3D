/*
 * ------------------------------------------------------------------------
 *  Name:   F3DQuat.cpp
 *  Desc:   This file define the quaternion type for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DMatrix.h"
#include "F3DQuat.h"

///////////////////////////////////////////////////////////////////////////
//// Define some constant variables.

const F3DQuat F3DQuat::ZERO( 0.0f, 0.0f, 0.0f, 0.0f );
const F3DQuat F3DQuat::IDENTITY( 0.0f, 0.0f, 0.0f, 1.0f );

///////////////////////////////////////////////////////////////////////////

/** Multiply with another quaternion.
*/
F3DQuat& F3DQuat::operator *= ( const F3DQuat& q )
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

	return *this;
}

/** Multiply with another quaternion.
*/
F3DQuat F3DQuat::operator * ( const F3DQuat& q ) const
{
	return F3DQuat(
		w*q.w - x*q.x - y*q.y - z*q.z,
		w*q.x + x*q.w + y*q.z - z*q.y,
		w*q.y + y*q.w + z*q.x - x*q.z,
		w*q.z + z*q.w + x*q.y - y*q.x);
}

/** Rotate a specified vector by me.
*/
F3DVector3 F3DQuat::operator * ( const F3DVector3& v ) const
{
	F3DQuat vq(v, 1.0f);
	F3DQuat qi(-x, -y, -z, w);
	F3DQuat qvq = (*this) * vq * qi;
	return F3DVector3(qvq.x, qvq.y, qvq.z);
}

/** Normalize the quaternion.
*/
void F3DQuat::Normalize()
{
	float m = sqrtf( x*x + y*y + z*z + w*w );
	FASSERT( m != 0.0f);

	if( m != 0.0f )
	{
        float fInvM = 1.0f / m;

		x *= fInvM;
		y *= fInvM;
		z *= fInvM;
		w *= fInvM;
	}
}

/** Calculate the inverse quaternion.
*/
void F3DQuat::InverseOf( const F3DQuat& quat )
{
    float fNormal = quat.x*quat.x + quat.y*quat.y +
        quat.z*quat.z + quat.w*quat.w;
	FASSERT( fNormal != 0.0f );

    if( fNormal > 0.0f )
    {
        float fInvNormal = 1.0f / fNormal;

        x = -x * fInvNormal;
        y = -y * fInvNormal;
        z = -z * fInvNormal;
        w =  w * fInvNormal;
    }
}

// Get the dot product with another quaternion.
float F3DQuat::DotProduct( const F3DQuat& q ) const
{
	return w * q.w + x * q.x + y * q.y + z * q.z; 
}

/** Calculate the quaternion from the rotation around a specified axis. 
Quaternion: { cos(theta/2), x*sin(theta/2), y*sin(theta/2), z*sin(theta/2) }
*/
void F3DQuat::FromAngleAxis( const F3DVector3& vAxis, float fAngle )
{
    float f = fAngle * 0.5f;
    float fSin = sinf(f);

    x = vAxis.x * fSin;
    y = vAxis.y * fSin;
    z = vAxis.z * fSin;
    w = cosf(f);
}

/** Get the angle and axis data from the quaternion.
*/
void F3DQuat::GetAngleAxis( F3DVector3& vAxis, float& fAngle ) const
{
    float fSqrLength = x*x + y*y + z*z;
    if( fSqrLength > 0.0f )
    {
        fAngle = 2.0f * acosf(w);

        float fInvLength = 1.0f / sqrtf( fSqrLength );
        vAxis.x = x * fInvLength;
        vAxis.y = y * fInvLength;
        vAxis.z = z * fInvLength;
    }
    else
    {
        fAngle = 0.0f;
        vAxis.x = 0.0f;
        vAxis.y = 0.0f;
        vAxis.z = 0.0f;
    }
}

/** Calculate the quaternion from a rotation matrix.
*/
void F3DQuat::MakeFromMatrix( const F3DMatrix4& mRot )
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    float tr,s,q[4];
    int i,j,k;

    int nxt[3] = { 1, 2, 0 };

    // 计算矩阵轨迹
    tr = mRot.m[0][0] + mRot.m[1][1] + mRot.m[2][2];

    // 检查矩阵轨迹是正还是负
    if( tr > 0.0f )
    {
        s = (float)sqrt( tr + 1.0f );
        w = s / 2.0f;
        s = 0.5f / s;
        x = (mRot.m[1][2] - mRot.m[2][1]) * s;
        y = (mRot.m[2][0] - mRot.m[0][2]) * s;
        z = (mRot.m[0][1] - mRot.m[1][0]) * s;
    }
    else
    {
        // 轨迹是负
        i = 0;
        if( mRot.m[1][1] > mRot.m[0][0] ) i = 1;
        if( mRot.m[2][2] > mRot.m[i][i] ) i = 2;
        j = nxt[i];
        k = nxt[j];

        s = (float)sqrt((mRot.m[i][i] - (mRot.m[j][j] + mRot.m[k][k])) + 1.0f);
        q[i] = s * 0.5f;
        if( s!= 0.0f ) s = 0.5f / s;
        q[3] = (mRot.m[j][k] - mRot.m[k][j]) * s;
        q[j] = (mRot.m[i][j] - mRot.m[j][i]) * s;
        q[k] = (mRot.m[i][k] - mRot.m[k][i]) * s;
        x = q[0];
        y = q[1];
        z = q[2];
        w = q[3];
    }
}

/** Calculate the quaternion from euler angles.
*/
void F3DQuat::MakeFromEuler( float fPitch, float fYaw, float fRoll )
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
    x = cX * sYcZ + sX * cYsZ;
    y = sX * cYcZ - cX * sYsZ;
    z = cX * cYsZ - sX * sYcZ;
}

/** Calculate the conjugate of the quaternion.
*/
void F3DQuat::Conjugate( const F3DQuat& q )
{
    x = -q.x;
    y = -q.y;
    z = -q.z;
    w =  q.w;
}

/** Calculate the magnitude of the quaternion.
*/
float F3DQuat::GetMagnitude() const
{
    return sqrtf(x*x + y*y + z*z + w*w);
}

/** Calculate the rotation matrix.
*/
void F3DQuat::GetMatrix( F3DMatrix4& mat ) const
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    memset( &mat, 0, sizeof(F3DMatrix4) );
    mat._44 = 1.0f;

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

    mat._11 = 1.0f - (yy + zz);
    mat._12 = xy + wz;
    mat._13 = xz - wy;

    mat._21 = xy - wz;
    mat._22 = 1.0f - (xx + zz);
    mat._23 = yz + wx;

    mat._31 = xz + wy;
    mat._32 = yz - wx;
    mat._33 = 1.0f - (xx + yy);
}

/** Get the euler angle X from the quaternion.
*/
float F3DQuat::GetPitch() const
{
    float ftx = 2.0f * x;
    float fty = 2.0f * y;
    float ftz = 2.0f * z;
    float ftwx = ftx * w;
    float ftxx = ftx * x;
    float ftyz = ftz * y;
    float ftzz = ftz * z;

    return atan2f( ftyz+ftwx, 1.0f-(ftxx+ftzz) );
}

/** Get the euler angle Y from the quaternion.
*/
float F3DQuat::GetYaw() const
{
    float ftx = 2.0f * x;
    float fty = 2.0f * y;
    float ftz = 2.0f * z;
    float ftwy = fty * w;
    float ftxx = ftx * x;
    float ftxz = ftz * x;
    float ftyy = fty * y;

    return atan2f( ftxz+ftwy, 1-(ftxx+ftyy) );
}

/** Get the euler angle Z from the quaternion.
*/
float F3DQuat::GetRoll() const
{
    float ftx = 2.0f * x;
    float fty = 2.0f * y;
    float ftz = 2.0f * z;
    float ftwz = ftz * w;
    float ftxy = fty * x;
    float ftyy = fty * y;
    float ftzz = ftz * z;

    return atan2f( ftxy+ftwz, 1.0f-(ftyy+ftzz) );
}

/** Get the euler angles from the quaternion.
*/
void F3DQuat::GetEulers( float& fPitch, float& fYaw, float& fRoll ) const
{
    double r11,r21,r31,r32,r33,r12,r13;
    double q00,q11,q22,q33;
    double tmp;

    q00 = w * w;
    q11 = x * x;
    q22 = y * y;
    q33 = z * z;

    r11 = q00 + q11 - q22 - q33;
    r21 = 2 * (x*y + w*z);
    r31 = 2 * (x*z - w*y);
    r32 = 2 * (y*z + w*x);
    r33 = q00 - q11 - q22 + q33;

    tmp = fabs( r31 );
    if( tmp > 0.999999 )
    {
        r12 = 2 * (x*y - w*z);
        r13 = 2 * (x*z + w*y);

        fPitch = 0.0f;
        fYaw   = (float)-((F3D_PI*0.5f) * r31/tmp);
        fRoll  = (float)atan2(-r12, -r31*r13);
    }
    else
    {
        fPitch = (float)atan2(r32, r33);
        fYaw   = (float)asin(-r31);
        fRoll  = (float)atan2(r21, r11);
    }
}

/** Get the blend of two quaternions.
*/
void F3DQuat::Rotate( const F3DQuat& q1, const F3DQuat& q2 )
{
	F3DQuat t = q1 * q2 * (~q1);

	x = t.x;
	y = t.y;
	z = t.z;
	w = t.w;
}

/** Rotate the vector by me.
*/
F3DVector3 F3DQuat::Rotate( const F3DVector3& v ) const
{
//     F3DQuat t(x, y, z, w);
//     F3DQuat r = t*v*(~t);
// 
//     return F3DVector(r.x, r.y, r.z);

    // nVidia SDK implementation
    F3DVector3 uv,uuv;
    F3DVector3 qvec( x,y,z );

    uv.Cross( qvec,v );
    uuv.Cross( qvec,uv );
    uv *= 2.0f * w;
    uuv *= 2.0f;

    return v + uv + uuv;
}

/** Normalised linear interpolation - faster but less accurate (non-constant rotation velocity)
*/
F3DQuat F3DQuat::Lerp(const F3DQuat& p, const F3DQuat& q, float f, bool shortestPath /* = false */)
{
    F3DQuat ret;
	float fCos = p.DotProduct(q);
	if( fCos < 0.0f && shortestPath )
		ret = p + f * ((-q) - p);
	else
		ret = p + f * (q - p);
	ret.Normalize();
	return ret;
}

/** Spherical linear interpolation
*/
F3DQuat F3DQuat::Slerp(const F3DQuat& q1, const F3DQuat& q2, float f, bool shortestPath /* = false */)
{
    F3DQuat tmp;
    float fCos = q1.x*q2.x+q1.y*q2.y+q1.z*q2.z+q1.w*q2.w;

    // Do we need to invert rotation?
    if( fCos < 0.0f && shortestPath )
    {
        fCos = -fCos;
        tmp = F3DQuat( -q2.x, -q2.y, -q2.z, -q2.w );
    }
    else
    {
        tmp = q2;
    }

    if( fabs(fCos) < 1.0f - F3D_EPSILON )
    {
        // Standard case (slerp)
        float fSin = (float)sqrt( 1.0f - fCos*fCos );
        float fAngle = (float)atan2( fSin,fCos );
        float fInvSin = 1.0f / fSin;
        float fCoeff0 = sinf((1.0f - f) * fAngle) * fInvSin;
        float fCoeff1 = sinf(f * fAngle) * fInvSin;
        return static_cast<F3DQuat>(q1) * fCoeff0 + tmp * fCoeff1;
    }
    else
    {
        // There are two situations:
        // 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
        //    interpolation safely.
        // 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
        //    are an infinite number of possibilities interpolation. but we haven't
        //    have method to fix this case, so just use linear interpolation here.
        F3DQuat t = static_cast<F3DQuat>(q1)*(1.0f - f) + tmp*f;
        // taking the complement requires renormalisation
        t.Normalize();
        return t;
    }

    return tmp;
}

/** Spherical quadratic interpolation
*/
F3DQuat F3DQuat::Squad(const F3DQuat& p, const F3DQuat& a, const F3DQuat& b, const F3DQuat& q, float f,
					   bool shortestPath /* = false */)
{
	float fSlerpT = 2.0f * f * (1.0f - f);
	F3DQuat kSlerpP = Slerp(p, q, f, shortestPath);
	F3DQuat kSlerpQ = Slerp(a, b, f);
	return Slerp(kSlerpP ,kSlerpQ, fSlerpT);
}