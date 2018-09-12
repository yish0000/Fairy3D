/*
 * ------------------------------------------------------------------------
 *  Name:   F3DVector.h
 *  Desc:   This file define the vector types for engine.
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_VECTOR_H__
#define __F3D_VECTOR_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** 2D Vector
*/
class FAIRY_API F3DVector2
{
public:

	union
	{
		struct
		{
			float x, y;
		};

		float m[2];
	};

public:

	// Constructors
	F3DVector2() {}
	F3DVector2(float _x, float _y) : x(_x), y(_y) {}
	F3DVector2(const F3DVector2& v) : x(v.x), y(v.y) {}

	// Operator =
	F3DVector2& operator = (const F3DVector2& v) { x = v.x; y = v.y; return *this; }

	// Operator +
	F3DVector2 operator + (const F3DVector2& v) const { return F3DVector2(x + v.x, y + v.y); }
	
	// Operator -
	F3DVector2 operator - (const F3DVector2& v) const { return F3DVector2(x - v.x, y - v.y); }
	
	// Operator * with float
	F3DVector2 operator * (float f) const { return F3DVector2(x * f, y * f); }
	friend F3DVector2 operator * (float f, const F3DVector2& v) { return F3DVector2(f * v.x, f * v.y); }

	// Operator / with float
	F3DVector2 operator / (float f) const { f = 1.0f / f; return F3DVector2(x * f, y * f); }

	// !!!!! Operator * with vector, Dot product!!!!!
	float operator * (const F3DVector2& v) const { return x * v.x + y * v.y; }

	// Operator ==
	bool operator == (const F3DVector2& v) const { return x == v.x && y == v.y; }
	// Operator !=
	bool operator != (const F3DVector2& v) const { return x != v.x || y != v.y; }

	// Operator +=
	F3DVector2& operator += (const F3DVector2& v) { x += v.x; y += v.y; return *this; }
	// Operator -=
	F3DVector2& operator -= (const F3DVector2& v) { x -= v.x; y -= v.y; return *this; }
	// Operator *=
	F3DVector2& operator *= (float f) { x *= f; y *= f; return *this; }
	// Operator /=
	F3DVector2& operator /= (float f) { x /= f; y /= f; return *this; }

	// Operator -
	F3DVector2 operator - () const { return F3DVector2(-x, -y); }

	// Set the value
	void Set(float _x, float _y) { x = _x; y = _y; }
	// Zero the vector
	void Zero() { x = y = 0.0f; }
	// Is the vector zero ?
	bool IsZero() const { return x == 0.0f && y == 0.0f; }
	// Is the vector equal with another ?
	bool IsEqual(const F3DVector2& v) const { return F3D_FloatEqual(x, v.x) && F3D_FloatEqual(y, v.y); }
	// Get the length of vector
	float GetLength() const { return sqrtf(x * x + y * y); }
	// Get the squared length of vector
	float GetSqrLength() const { return x * x + y * y; }
	// Get the minimum number
	float MinMember() const { return x < y ? x : y; }
	// Get the maximum number
	float MaxMember() const { return x > y ? x : y; }

	// Normalize the vector
    F_INLINE float Normalize();
};

/** 3D Vector
*/
class FAIRY_API F3DVector3
{
public:

	// Define some constant variables.
	static const F3DVector3 ZERO;
	static const F3DVector3 UNIT_X;
	static const F3DVector3 UNIT_Y;
	static const F3DVector3 UNIT_Z;

public:

	union
	{
		struct
		{
			float x, y, z;
		};

		float m[3];
	};

public:

	// Constructors
	F3DVector3() {}
	F3DVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	F3DVector3(const F3DVector3& v) : x(v.x), y(v.y), z(v.z) {}

	// Operator =
	F3DVector3& operator = (const F3DVector3& v) { x += v.x; y += v.y; z += v.z; return *this; }

	// Operator +
	F3DVector3 operator + (const float f) const { return F3DVector3(x + f, y + f, z + f); }
	F3DVector3 operator + (const F3DVector3& v) const { return F3DVector3(x + v.x, y + v.y, z + v.z); }

	// Operator -
	F3DVector3 operator - (const float f) const { return F3DVector3(x - f, y - f, z - f); }
	F3DVector3 operator - (const F3DVector3& v) const { return F3DVector3(x - v.x, y - v.y, z - v.z); }

	// Operator *
	F3DVector3 operator * (const float f) const { return F3DVector3(x * f, y * f, z * f); }
	friend F3DVector3 operator * (const float f, const F3DVector3& v) { return F3DVector3(f * v.x, f * v.y, f * v.z); }

	// Operator /
	F3DVector3 operator / (float f) const { f = 1.0f / f; return F3DVector3(x * f, y * f, z * f); }

	// Operator +=
	F3DVector3& operator += (const F3DVector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	F3DVector3& operator += (const float f) { x += f; y += f; z += f; return *this; }

	// Operator -=
	F3DVector3& operator -= (const F3DVector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	F3DVector3& operator -= (const float f) { x -= f; y -= f; z -= f; return *this; }

	// Operator *=
	F3DVector3& operator *= (const float f) { x *= f; y *= f; z *= f; return *this; }
	// Operator /=
	F3DVector3& operator /= (float f) { f = 1.0f / f; x *= f; y *= f; z *= f; return *this; }

	// Operator ==
	bool operator == (const F3DVector3& v) const { return x == v.x && y == v.y && z == v.z; }
	// Operator !=
	bool operator != (const F3DVector3& v) const { return x != v.x || y != v.y || z != v.z; }

	// Operator -
	F3DVector3 operator - () const { return F3DVector3(-x, -y, -z); }

	// !!!!! Operator * with vector, Dot product!!!!!
	float operator * (const F3DVector3& v) const { return x * v.x + y * v.y + z * v.z; }

	// Transform the vector with a quaternion.
	F3DVector3 operator * (const F3DQuat& q) const;
	// Transform the vector with a matrix
	F3DVector3 operator * (const F3DMatrix4& m) const;

	// Set the values
	void Set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	// Zero the vector
	void Zero() { x = y = z = 0.0f; }
	// Is the vector zero ?
	bool IsZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
	// Is the vector equal with another ?
	bool IsEqual(const F3DVector3& v) const { return F3D_FloatEqual(x, v.x) && F3D_FloatEqual(y, v.y) && F3D_FloatEqual(z, v.z); }
	// Get the length of the vector
	float GetLength() const { return sqrtf(x * x + y * y + z * z); }
	// Get the squared length of the vector
	float GetSqrLength(void) const { return x * x + y * y + z * z; }
	// Get the length of the vector's projection onto (x, z)
	float GetLengthXZ() const { return sqrtf(x * x + z * z); }
	// Get the squared length of the vector's projection onto (x, z)
	float GetSqrLengthXZ() const { return x * x + z * z; }
	// Get the nagative vector
	void Negate() { x = -x; y = -y; z = -z; }

	// Normalize the vector.
	F_INLINE float Normalize();
	// Get the angle with another vector.
	F_INLINE float AngleWith(const F3DVector3& v) const;
	// Get the vector from v1 to v2.
	F_INLINE void Difference(const F3DVector3& v1, const F3DVector3& v2);
	// Get the cross product of two vectors.
	F_INLINE void Cross(const F3DVector3& v1, const F3DVector3& v2);

	// Rotate the vector with a matrix
	void RotateWith(const F3DMatrix4& m);
	// Rotate the vector with a matrix inversely
	void InvRotateWith(const F3DMatrix4& m);
};

/** 4D vector
*/
class FAIRY_API F3DVector4
{
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
	F3DVector4() {}
	F3DVector4(float _x, float _y, float _z, float _w = 1.0f) : x(_x), y(_y), z(_z), w(_w) {}
	F3DVector4(const F3DVector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f) {}
	F3DVector4(const F3DVector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	// Operator =
	F3DVector4& operator = (const F3DVector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	F3DVector4& operator = (const F3DVector3& v) { x = v.x; y = v.y; z = v.z; w = 1.0f; return *this; }

	// Set the values
	void Set(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
	// Zero the vector
	void Zero() { x = y = z = w = 0.0f; }

	// Is the vector equal with another ?
	bool IsEqual(const F3DVector4& v) const
	{
		return F3D_FloatEqual(x, v.x) && F3D_FloatEqual(y, v.y) && F3D_FloatEqual(z, v.z) && F3D_FloatEqual(w, v.w);
	}

	// Convert to the vector3
	F_INLINE F3DVector3 ToVector3() const;
};

///////////////////////////////////////////////////////////////////////////
//// INLINE FUNCTIONS

// Normalize the vector
float F3DVector2::Normalize()
{
	float fLength = GetLength();
	if( fLength < 1e-6 && fLength > -1e-6 )
	{
		Zero();
		fLength = 0.0f;
	}
	else
	{
		float f = 1.0f / fLength;
		x *= f;
		y *= f;
	}

	return fLength;
}

///////////////////////////////////////////////////////////////////////////

// Normalize the vector
float F3DVector3::Normalize()
{
	float fLength = GetLength();
	if( fLength < 1e-6 && fLength > -1e-6 )
	{
		Zero();
		fLength = 0.0f;
	}
	else
	{
		float f = 1.0f / fLength;
		x *= f;
		y *= f;
		z *= f;
	}

	return fLength;
}

// Get the difference of two vectors.
void F3DVector3::Difference(const F3DVector3& v1, const F3DVector3& v2)
{
	x = v2.x - v1.x;
	y = v2.y - v1.y;
	z = v2.z - v1.z;
}

// Get the cross product of two vectors.
void F3DVector3::Cross(const F3DVector3& v1, const F3DVector3& v2)
{
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}

// Get the angle with another vector.
float F3DVector3::AngleWith(const F3DVector3& v) const
{
	return acosf(((*this) * v) / (GetLength() * v.GetLength()));
}

///////////////////////////////////////////////////////////////////////////

// Convert me to a 3d vector.
F3DVector3 F3DVector4::ToVector3() const
{
	FASSERT(w != 0.0f);
	float fInv = 1.0f / w;
	return F3DVector3(x * fInv, y * fInv, z * fInv);
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_VECTOR_H__
