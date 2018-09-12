/*
 * ------------------------------------------------------------------------
 *  Name:   F3DQuat.h
 *  Desc:   This file define the quaternion type for the engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_QUAT_H__
#define __F3D_QUAT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** Quaternion
Quaternion: { cos(theta/2), x*sin(theta/2), y*sin(theta/2), z*sin(theta/2) }
*/
class FAIRY_API F3DQuat
{
public:

	// Define some constant variables.
	static const F3DQuat ZERO;
	static const F3DQuat IDENTITY;

public:

	// Elements
	union
	{
		struct 
		{
			float x, y, z, w;
		};

		float m[4];
	};

public:

	// Constructors
    F3DQuat() {}
	F3DQuat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	F3DQuat(float _w, const F3DVector3& _v) : x(_v.x), y(_v.y), z(_v.z), w(_w) {}
	F3DQuat(float eulerX, float eulerY, float eulerZ) { MakeFromEuler(eulerX, eulerY, eulerZ); }
	F3DQuat(const F3DVector3& vAxis, float fAngle) { FromAngleAxis(vAxis, fAngle); }
	F3DQuat(const F3DMatrix4& mRot) { MakeFromMatrix(mRot); }

	// Operator +
	F3DQuat operator + (const F3DQuat& q) const
	{
		return F3DQuat(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	// Operator +=
	F3DQuat& operator += (const F3DQuat& q)
	{
		x += q.x; y += q.y; z += q.z; w += q.w;
		return *this;
	}

	// Operator -
	F3DQuat operator - (const F3DQuat& q) const
	{
		return F3DQuat(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	// Operator -=
	F3DQuat& operator -= (const F3DQuat& q)
	{
		x -= q.x; y -= q.y; z -= q.z; w -= q.w;
		return *this;
	}

	// Operator *
	F3DQuat operator * (float f) const
	{
		return F3DQuat(x * f, y * f, z * f, w * f);
	}

	// Operator *=
	F3DQuat& operator *= (float f)
	{
		x *= f; y *= f; z *= f; w *= f;
		return *this;
	}

	// Operator f * q
	friend F3DQuat operator * (float f, const F3DQuat& q)
	{
		return F3DQuat(q.x * f, q.y * f, q.z * f, q.w * f);
	}

	// Operator /
	F3DQuat operator / (float f) const
	{
		FASSERT(f != 0.0f);
		float fInv = 1.0f / f;
		return F3DQuat(x * fInv, y * fInv, z * fInv, w * fInv);
	}

	// Operator /=
	F3DQuat& operator /= (float f)
	{
		FASSERT(f != 0.0f);
		float fInv = 1.0f / f;
		x *= fInv; y *= fInv; z *= fInv; w *= fInv;
		return *this;
	}

	// Operator ==
	bool operator == (const F3DQuat& q) const
	{
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}

	// Operator !=
	bool operator != (const F3DQuat& q) const
	{
		return x != q.x || y != q.y || z != q.z || w != q.w;
	}

	// Multiply tow quaternions.
	F3DQuat operator * (const F3DQuat& q) const;
	F3DQuat& operator *= (const F3DQuat& q);

	// Rotate the vector by me.
	F3DVector3 operator * (const F3DVector3& v) const;

	// Get the specified quaternion's conjugate quaternion.
	F3DQuat operator ~ () const { return F3DQuat(-x, -y, -z, w); }
	// Get the negative quaternion
	F3DQuat operator - () const { return F3DQuat(-x, -y, -z, -w); }

    // Set the values.
    void Set( float _x, float _y, float _z, float _w )
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

	// Clear the values.
	void Zero() { x = y = z = w = 0.0f; }

	// Get the magnitude of the quaternion.
	float GetMagnitude() const;
    // Normalize the quaternion.
    void Normalize();
	// Convert me to the specified quaternion's conjugate quaternion.
	void Conjugate( const F3DQuat& q );
    // Calculate the inverse quaternion.
    void InverseOf( const F3DQuat& q );
	// Get the dot product with another quaternion.
	float DotProduct( const F3DQuat& q ) const;

    // Calculate the quaternion from the rotation around a specified axis. 
    void FromAngleAxis( const F3DVector3& vAxis, float fAngle );
    // Calculate the quaternion from the specified rotation matrix.
    void MakeFromMatrix( const F3DMatrix4& m );
    // Calculate the quaternion from the euler angles.
    void MakeFromEuler( float fPitch, float fYaw, float fRoll );

	// Get the angle and axis data from the quaternion.
	void GetAngleAxis( F3DVector3& vAxis, float& fAngle ) const;
    // Get the rotation matrix from the quaternion.
    void GetMatrix( F3DMatrix4& mat ) const;
    // Get the euler angles from the quaternion.
    void GetEulers( float& fPitch, float& fYaw, float& fRoll ) const;

	// Get the euler angle from the quaternion.
	float GetPitch() const;
	float GetYaw() const;
	float GetRoll() const;

	// Rotate the specified vector.
	F3DVector3 Rotate( const F3DVector3& v ) const;

    // 获取两个四元数的混合
    void Rotate( const F3DQuat& q1, const F3DQuat& q2 );

	// Normalised linear interpolation - faster but less accurate (non-constant rotation velocity)
	static F3DQuat Lerp(const F3DQuat& q1, const F3DQuat& q2, float f, bool shortestPath = false);
	// Spherical linear interpolation
	static F3DQuat Slerp(const F3DQuat& q1, const F3DQuat& q2, float f, bool shortestPath = false);
	// Spherical quadratic interpolation
	static F3DQuat Squad(const F3DQuat& p, const F3DQuat& a, const F3DQuat& b, const F3DQuat& q, float f, bool shortestPath = false);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_QUAT_H__