/*
 * ------------------------------------------------------------------------
 *  Name:   F3DMatrix.h
 *  Desc:   This file define the matrix types for engine.
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_MATRIX_H__
#define __F3D_MATRIX_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** 3x3 Matrix
*/
class FAIRY_API F3DMatrix3
{
public:

	// Identity matrix.
	static const F3DMatrix3 IDENTITY;

public:

	union
	{
		struct
		{
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};

		float m[3][3];
	};

public:

	// Constructors
	F3DMatrix3() {}
	F3DMatrix3(const float arr[3][3]) { memcpy(m, arr, sizeof(float)*9); }
	F3DMatrix3(const F3DMatrix3& mat) { memcpy(m, mat.m, sizeof(float)*9); }
	F3DMatrix3(float f11, float f12, float f13,
			   float f21, float f22, float f23,
			   float f31, float f32, float f33)
	{
		_11 = f11; _12 = f12; _13 = f13;
		_21 = f21; _22 = f22; _23 = f23;
		_31 = f31; _32 = f32; _33 = f33;
	}

	// Rotate the 2d vector with matrix.
	F3DVector2 operator * (const F3DVector2& v) const;
	friend F3DVector2 operator * (const F3DVector2& v, const F3DMatrix3& mat);
	// Rotate the 3d vector with matrix.
	F3DVector3 operator * (const F3DVector3& v) const;
	friend F3DVector3 operator * (const F3DVector3& v, const F3DMatrix3& mat);

	// Multiply the two matrix.
	F3DMatrix3 operator * (const F3DMatrix3& m2) const;
	F3DMatrix3& operator *= (const F3DMatrix3& m2)
	{
		*this = *this * m2;
		return *this;
	}

	// Operator ==
	bool operator == (const F3DMatrix3& mat) const;
	// Operator !=
	bool operator != (const F3DMatrix3& mat) const;

	// Set me to a identity matrix
	void Identity()
	{
		_11 = _22 = _33 = 1.0f;
		_12 = _13 = _21 = _23 = _31 = _32 = 0.0f;
	}

	// Make the translate matrix at 2d space.
	void Translate(float x, float y);
	// Make the rotation matrix at 2d space.
	void Rotate(float fAngle);

	// Transpose the matrix.
	void Transpose();
	// Calculate the inverse matrix
	void Inverse();

	// Get the specified row.
	F3DVector3 GetRow(int row) const
	{
		FASSERT(row >= 0 && row < 3);
		return F3DVector3(m[row][0], m[row][1], m[row][2]);
	}

	// Set the specified row.
	void SetRow(int row, const F3DVector3& data)
	{
		FASSERT(row >= 0 && row < 4);
		m[row][0] = data.x; m[row][1] = data.y; m[row][2] = data.z;
	}

	// Get the specified column.
	F3DVector3 GetCol(int col) const
	{
		FASSERT(col >= 0 && col < 3);
		return F3DVector3(m[0][col], m[1][col], m[2][col]);
	}

	// Set the specified column.
	void SetCol(int col, const F3DVector3& data)
	{
		FASSERT(col >= 0 && col <4);
		m[0][col] = data.x; m[1][col] = data.y; m[2][col] = data.z;
	}
};

/** 4x4 Matrix (left-half system)
*/
class FAIRY_API F3DMatrix4
{
public:

	// Identity matrix.
	static const F3DMatrix4 IDENTITY;

public:

    union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };

        float m[4][4];
    };

public:

	// Constructors
    F3DMatrix4() {}
	F3DMatrix4(const float arr[4][4]) { memcpy(m, arr, sizeof(float)*16); }
	F3DMatrix4(const F3DMatrix4& mat) { memcpy(m, mat.m, sizeof(float)*16); }
    F3DMatrix4(float f11, float f12, float f13, float f14,
               float f21, float f22, float f23, float f24,
               float f31, float f32, float f33, float f34,
               float f41, float f42, float f43, float f44)
    {
		_11 = f11; _12 = f12; _13 = f13; _14 = f14;
		_21 = f21; _22 = f22; _23 = f23; _24 = f24;
		_31 = f31; _32 = f32; _33 = f33; _34 = f34;
		_41 = f41; _42 = f42; _43 = f43; _44 = f44;
    }

	// Rotate the vector with matrix.
	F3DVector3 operator * (const F3DVector3& v) const;
	friend F3DVector3 operator * (const F3DVector3& v, const F3DMatrix3& mat);
	F3DVector4 operator * (const F3DVector4& v) const;
	friend F3DVector4 operator * (const F3DVector4& v, const F3DMatrix4& mat);

	// Multiply the two matrix.
	F3DMatrix4 operator * (const F3DMatrix4& mat) const;
	F3DMatrix4& operator *= (const F3DMatrix4& mat)
	{
		*this = *this * mat;
		return *this;
	}

	// Operator +
	F3DMatrix4 operator + (const F3DMatrix4& mat) const;
	F3DMatrix4& operator += (const F3DMatrix4& mat);

	// Operator - 
	F3DMatrix4 operator - (const F3DMatrix4& mat) const;
	F3DMatrix4& operator -= (const F3DMatrix4& mat);

	// Scale the matrix.
	F3DMatrix4 operator * (float f) const;
	friend F3DMatrix4 operator * (float f, const F3DMatrix4& mat);
	F3DMatrix4 operator / (float f) const { return (*this) * (1.0f / f); }
	F3DMatrix4& operator *= (float f);
	F3DMatrix4& operator /= (float f) { (*this) *= (1.0f / f); return *this; }

	// Operator ==
	bool operator == (const F3DMatrix4& mat) const;
	// Operator !=
	bool operator != (const F3DMatrix4& mat) const;

    // Make me a identity matrix.
    void Identity();

	// Rotate around a specified axis.
    void RotateX( const float fAngle );
    void RotateY( const float fAngle );
    void RotateZ( const float fAngle );

	// Make the rotation matrix by euler angle.
    void RotateEulerXYZ( float pitch, float yaw, float roll );
    void RotateEulerXZY( float pitch, float roll, float yaw );
    void RotateEulerYXZ( float yaw, float pitch, float roll );
    void RotateEulerYZX( float yaw, float roll, float pitch );
    void RotateEulerZXY( float roll, float pitch, float yaw );
    void RotateEulerZYX( float roll, float yaw, float pitch );

	// Make the rotation matrix around a specified axis.
	void RotateAxis( const F3DVector3& vAxis, float fAngle );
	void RotateAxis( const F3DVector3& vPos, const F3DVector3& vAxis, float fAngle );

    // Make a translation matrix.
	void Translate( const F3DVector3& v );
	void Translate( float dx, float dy, float dz ) { Translate(F3DVector3(dx, dy, dz)); }
	// Set the translation data for the specified matrix without changing other data.
    void SetTranslation( const F3DVector3& v );
	void SetTranslation( float dx, float dy, float dz ) { SetTranslation(F3DVector3(dx, dy, dz)); }
	// Get the translation data.
    F3DVector3 GetTranslation() const;

	// Get the rotation part of the matrix.
	F3DMatrix3 GetRotationPart() const;
	// Set the rotation part of the matrix.
	void SetRotationPart(const F3DMatrix3& mat);

    // Make a scale matrix.
    void Scale( float dx, float dy, float dz );

    // Transpose matrix.
    void Transpose();
    void TransposeOf( const F3DMatrix4& mat );

    // Inverse matrix.
	void Inverse();
    void InverseOf( const F3DMatrix4& mat );

    // Make a transform matrix.
    void MakeTransform(const F3DVector3& vPos, const F3DQuat& qOrienuation, const F3DVector3& vScale);
	// Decompose the translation, rotation and scale data from the matrix.
	void DecomposeMatrix(F3DVector3& vScale, F3DQuat& qOrient, F3DVector3& vPos);

    // Make a camera view matrix.
    void MakeView3D(const F3DVector3& vRight, const F3DVector3& vUp, const F3DVector3& vDir, const F3DVector3& vPos);
    void LookAt(const F3DVector3& vPos, const F3DVector3& vLookAt, const F3DVector3& vWorldUp = F3DVector3::UNIT_Y);

    // Make an orthotropic projection matrix.
    void MakeOrthoProj( float l, float r, float b, float t, float n, float f );
    // Make a perspective projection matrix.
    void MakePerspProj( float fov, float aspect, float n, float f );

    // Make a billboard matrix.
    void Billboard(const F3DVector3& vPos, const F3DVector3& vDir, const F3DVector3& vWorldUp = F3DVector3::UNIT_Y);

    // Make a mirror matrix.
    void Mirror( const F3DPlane& plane );

    // Rotate a vertex inversely.
    void ApplyInverseRotate( F3DVector3& v );

	// Get the specified row.
	F3DVector4 GetRow(int row) const
	{
		FASSERT(row >= 0 && row < 4);
		return F3DVector4(m[row][0], m[row][1], m[row][2], m[row][3]);
	}

	// Set the specified row.
	void SetRow(int row, const F3DVector4& data)
	{
		FASSERT(row >= 0 && row < 4);
		m[row][0] = data.x; m[row][1] = data.y; m[row][2] = data.z; m[row][3] = data.w;
	}

	// Get the specified column.
	F3DVector4 GetCol(int col) const
	{
		FASSERT(col >= 0 && col < 4);
		return F3DVector4(m[0][col], m[1][col], m[2][col], m[3][col]);
	}

	// Set the specified column.
	void SetCol(int col, const F3DVector4& data)
	{
		FASSERT(col >= 0 && col <4);
		m[0][col] = data.x; m[1][col] = data.y; m[2][col] = data.z; m[3][col] = data.w;
	}
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_MATRIX_H__