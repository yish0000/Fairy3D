/*
 * ------------------------------------------------------------------------
 *  Name:   F3DMatrix.cpp
 *  Desc:   This file define the matrix types for engine.
 *  Author: Yish
 *  Date:   11/12/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DPlane.h"
#include "F3DQuat.h"
#include "F3DMatrix.h"

///////////////////////////////////////////////////////////////////////////
//// Define the constant variables

// Identity 3x3 matrix.
const F3DMatrix3 F3DMatrix3::IDENTITY( 1.0f, 0.0f, 0.0f,
									   0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 1.0f);

// Identity 4x4 matrix.
const F3DMatrix4 F3DMatrix4::IDENTITY( 1.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 1.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 1.0f );

///////////////////////////////////////////////////////////////////////////
//// Implement class F3DMatrix3

// Make the translate matrix at 2d space.
void F3DMatrix3::Translate(float x, float y)
{
	Identity();
	_31 = x;
	_32 = y;
}

// Make the rotation matrix at 2d space.
void F3DMatrix3::Rotate(float fAngle)
{
	Identity();
	_22 = _11 = cosf(fAngle);
	_12 = sinf(fAngle);
	_21 = -_12;
}

// Transpose the matrix.
void F3DMatrix3::Transpose()
{
	F_Swap(_12, _21);
	F_Swap(_13, _31);
	F_Swap(_23, _32);
}

// Calculate the inverse matrix
void F3DMatrix3::Inverse()
{
	float matInverse[3][3];
	matInverse[0][0] = m[1][1]*m[2][2] - m[1][2]*m[2][1];
	matInverse[0][1] = m[0][2]*m[2][1] - m[0][1]*m[2][2];
	matInverse[0][2] = m[0][1]*m[1][2] - m[0][2]*m[1][1];
	matInverse[1][0] = m[1][2]*m[2][0] - m[1][0]*m[2][2];
	matInverse[1][1] = m[0][0]*m[2][2] - m[0][2]*m[2][0];
	matInverse[1][2] = m[0][2]*m[1][0] - m[0][0]*m[1][2];
	matInverse[2][0] = m[1][0]*m[2][1] - m[1][1]*m[2][0];
	matInverse[2][1] = m[0][1]*m[2][0] - m[0][0]*m[2][1];
	matInverse[2][2] = m[0][0]*m[1][1] - m[0][1]*m[1][0];

	float fDet = m[0][0] * matInverse[0][0] + m[0][1] * matInverse[1][0] + m[0][2] * matInverse[2][0];
	float fInvDet = 1.0f / fDet;

	m[0][0] = matInverse[0][0] * fInvDet;
	m[0][1] = matInverse[0][1] * fInvDet;
	m[0][2] = matInverse[0][2] * fInvDet;
	m[1][0] = matInverse[1][0] * fInvDet;
	m[1][1] = matInverse[1][1] * fInvDet;
	m[1][2] = matInverse[1][2] * fInvDet;
	m[2][0] = matInverse[2][0] * fInvDet;
	m[2][1] = matInverse[2][1] * fInvDet;
	m[2][2] = matInverse[2][2] * fInvDet;
}

// Multiply the two matrix.
F3DMatrix3 F3DMatrix3::operator * (const F3DMatrix3& mat) const
{
	F3DMatrix3 mRet;

	mRet._11 = _11*mat._11 + _12*mat._21 + _13*mat._31;
	mRet._12 = _11*mat._12 + _12*mat._22 + _13*mat._32;
	mRet._13 = _11*mat._13 + _12*mat._23 + _13*mat._33;

	mRet._21 = _21*mat._11 + _22*mat._21 + _23*mat._31;
	mRet._22 = _21*mat._12 + _22*mat._22 + _23*mat._32;
	mRet._23 = _21*mat._13 + _22*mat._23 + _23*mat._33;

	mRet._31 = _31*mat._11 + _32*mat._21 + _33*mat._31;
	mRet._32 = _31*mat._12 + _32*mat._22 + _33*mat._32;
	mRet._33 = _31*mat._13 + _32*mat._23 + _33*mat._33;

	return mRet;
}

// Transform the vector with me
F3DVector3 F3DMatrix3::operator * (const F3DVector3& v) const
{
	return F3DVector3(
		v.x * _11 + v.y * _21 + v.z * _31,
		v.x * _12 + v.y * _22 + v.z * _32,
		v.x * _13 + v.y * _23 + v.z * _33);
}

// Transform the vector with a matrix
F3DVector3 operator * (const F3DVector3& v, const F3DMatrix3& mat)
{
	return F3DVector3(
		v.x * mat._11 + v.y * mat._21 + v.z * mat._31,
		v.x * mat._12 + v.y * mat._22 + v.z * mat._32,
		v.x * mat._13 + v.y * mat._23 + v.z * mat._33);
}

// Transform the 2d vector with me.
F3DVector2 F3DMatrix3::operator * (const F3DVector2& v) const
{
	F3DVector2 ret(
		v.x * _11 + v.y * _21 + _31,
		v.x * _12 + v.y * _22 + _32);
	float z = v.x * _13 + v.y * _23 + _33;
	float fInv = 1.0f / z;
	ret *= fInv;
	return ret;
}

// Transform the 2d vector with me.
F3DVector2 operator * (const F3DVector2& v, const F3DMatrix3& mat)
{
	F3DVector2 ret(
		v.x * mat._11 + v.y * mat._21 + mat._31,
		v.x * mat._12 + v.y * mat._22 + mat._32);
	float z = v.x * mat._13 + v.y * mat._23 + mat._33;
	float fInv = 1.0f / z;
	ret *= fInv;
	return ret;

}

// Operator ==
bool F3DMatrix3::operator == (const F3DMatrix3& mat) const
{
	for(int i=0; i<3; ++i)
	{
		for(int j=0; j<3; ++j)
		{
			if( m[i][j] != mat.m[i][j] )
				return false;
		}
	}

	return true;
}

// Operator !=
bool F3DMatrix3::operator != (const F3DMatrix3& mat) const
{
	for(int i=0; i<3; ++i)
	{
		for(int j=0; j<3; ++j)
		{
			if( m[i][j] != mat.m[i][j] )
				return true;
		}
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////
//// Implement class F3DMatrix4

// Rotate the vector3 with matrix.
F3DVector3 F3DMatrix4::operator * (const F3DVector3& v) const
{
	F3DVector3 ret(
		v.x * _11 + v.y * _21 + v.z * _31 + _41,
		v.x * _12 + v.y * _22 + v.z * _32 + _42,
		v.x * _13 + v.y * _23 + v.z * _33 + _43);
	float w = 1.0f / (v.x * _14 + v.y * _24 + v.z * _34 + _44);
	ret *= w;
	return ret;
}

// Rotate the vector3 with matrix.
F3DVector3 operator * (const F3DVector3& v, const F3DMatrix4& mat)
{
	F3DVector3 ret(
		v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + mat._41,
		v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + mat._42,
		v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + mat._43);
	float w = 1.0f / (v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + mat._44);
	ret *= w;
	return ret;
}

// Rotate the vector4 with matrix.
F3DVector4 F3DMatrix4::operator * (const F3DVector4& v) const
{
	return F3DVector4(
		v.x * _11 + v.y * _21 + v.z * _31 + _41,
		v.x * _12 + v.y * _22 + v.z * _32 + _42,
		v.x * _13 + v.y * _23 + v.z * _33 + _43,
		v.x * _14 + v.y * _24 + v.z * _34 + _44);
}

// Rotate the vector4 with matrix.
F3DVector4 operator * (const F3DVector4& v, const F3DMatrix4& mat)
{
	return F3DVector4(
		v.x * mat._11 + v.y * mat._21 + v.z * mat._31 + mat._41,
		v.x * mat._12 + v.y * mat._22 + v.z * mat._32 + mat._42,
		v.x * mat._13 + v.y * mat._23 + v.z * mat._33 + mat._43,
		v.x * mat._14 + v.y * mat._24 + v.z * mat._34 + mat._44);
}

/** Multiply the two matrix.
*/
F3DMatrix4 F3DMatrix4::operator * ( const F3DMatrix4& mat ) const
{
	F3DMatrix4 mRet;

	mRet._11 = _11*mat._11 + _12*mat._21 + _13*mat._31 + _14*mat._41;
	mRet._12 = _11*mat._12 + _12*mat._22 + _13*mat._32 + _14*mat._42;
	mRet._13 = _11*mat._13 + _12*mat._23 + _13*mat._33 + _14*mat._43;
	mRet._14 = _11*mat._14 + _12*mat._24 + _13*mat._34 + _14*mat._44;

	mRet._21 = _21*mat._11 + _22*mat._21 + _23*mat._31 + _24*mat._41;
	mRet._22 = _21*mat._12 + _22*mat._22 + _23*mat._32 + _24*mat._42;
	mRet._23 = _21*mat._13 + _22*mat._23 + _23*mat._33 + _24*mat._43;
	mRet._24 = _21*mat._14 + _22*mat._24 + _23*mat._34 + _24*mat._44;

	mRet._31 = _31*mat._11 + _32*mat._21 + _33*mat._31 + _34*mat._41;
	mRet._32 = _31*mat._12 + _32*mat._22 + _33*mat._32 + _34*mat._42;
	mRet._33 = _31*mat._13 + _32*mat._23 + _33*mat._33 + _34*mat._43;
	mRet._34 = _31*mat._14 + _32*mat._24 + _33*mat._34 + _34*mat._44;

	mRet._41 = _41*mat._11 + _42*mat._21 + _43*mat._31 + _44*mat._41;
	mRet._42 = _41*mat._12 + _42*mat._22 + _43*mat._32 + _44*mat._42;
	mRet._43 = _41*mat._13 + _42*mat._23 + _43*mat._33 + _44*mat._43;
	mRet._44 = _41*mat._14 + _42*mat._24 + _43*mat._34 + _44*mat._44;

	return mRet;
}

// Operator +
F3DMatrix4 F3DMatrix4::operator + (const F3DMatrix4& mat) const
{
	return F3DMatrix4(
		_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
		_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
		_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
		_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

// Operator +
F3DMatrix4& F3DMatrix4::operator += (const F3DMatrix4& mat)
{
	*this = *this + mat;
	return *this;
}

// Operator - 
F3DMatrix4 F3DMatrix4::operator - (const F3DMatrix4& mat) const
{
	return F3DMatrix4(
		_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
		_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
		_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
		_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

// Operator - 
F3DMatrix4& F3DMatrix4::operator -= (const F3DMatrix4& mat)
{
	*this = *this - mat;
	return *this;
}

// Scale the matrix.
F3DMatrix4 F3DMatrix4::operator * (float f) const
{
	return F3DMatrix4(
		_11 * f, _12 * f, _13 * f, _14 * f,
		_21 * f, _22 * f, _23 * f, _24 * f,
		_31 * f, _32 * f, _33 * f, _34 * f,
		_41 * f, _42 * f, _43 * f, _44 * f);
}

// Scale the matrix.
F3DMatrix4 operator * (float f, const F3DMatrix4& mat)
{
	return F3DMatrix4(
		mat._11 * f, mat._12 * f, mat._13 * f, mat._14 * f,
		mat._21 * f, mat._22 * f, mat._23 * f, mat._24 * f,
		mat._31 * f, mat._32 * f, mat._33 * f, mat._34 * f,
		mat._41 * f, mat._42 * f, mat._43 * f, mat._44 * f);
}

// Scale the matrix.
F3DMatrix4& F3DMatrix4::operator *= (float f)
{
	_11 *= f; _12 *= f; _13 *= f; _14 *= f;
	_21 *= f; _22 *= f; _23 *= f; _24 *= f;
	_31 *= f; _32 *= f; _33 *= f; _34 *= f;
	_41 *= f; _42 *= f; _43 *= f; _44 *= f;
	return *this;
}

// Operator ==
bool F3DMatrix4::operator == (const F3DMatrix4& mat) const
{
	for(int i=0; i<4; ++i)
	{
		for(int j=0; j<4; ++j)
		{
			if( m[i][j] != mat.m[i][j] )
				return false;
		}
	}

	return true;
}

// Operator !=
bool F3DMatrix4::operator != (const F3DMatrix4& mat) const
{
	for(int i=0; i<4; ++i)
	{
		for(int j=0; j<4; ++j)
		{
			if( m[i][j] != mat.m[i][j] )
				return true;
		}
	}

	return false;
}

// Make me a identity matrix.
void F3DMatrix4::Identity(void)
{
    memset( &_11, 0, sizeof(F3DMatrix4) );
    _11 = _22 = _33 = _44 = 1.0f;
}

/** Make a rotation matrix around X axis.
*/
void F3DMatrix4::RotateX( const float f )
{
	float fCos = cosf(f);
	float fSin = sinf(f);

	_22 =  fCos;
	_23 =  fSin;
	_32 = -fSin;
	_33 =  fCos;

	_11 = _44 = 1.0f;
	_12 = _13 = _14 = _21 = _24 = _31 = _34 = _41 = _42 = _43 = 0.0f;
}

/** Make a rotation matrix around Y axis.
*/
void F3DMatrix4::RotateY( const float f )
{
	float fCos = cosf(f);
	float fSin = sinf(f);

	_11 =  fCos;
	_13 = -fSin;
	_31 =  fSin;
	_33 =  fCos;

	_22 = _44 = 1.0f;
	_12 = _23 = _14 = _21 = _24 = _32 = _34 = _41 = _42 = _43 = 0.0f;
}

/** Make a rotation matrix around Z axis.
*/
void F3DMatrix4::RotateZ( const float f )
{
	float fCos = cosf(f);
	float fSin = sinf(f);

	_11  =  fCos;
	_12  =  fSin;
	_21  = -fSin;
	_22  =  fCos;

	_33 = _44 = 1.0f;
	_13 = _14 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f;
}

/** Make the rotation matrix by euler angle.
*/
void F3DMatrix4::RotateEulerXYZ( float pitch, float yaw, float roll )
{
	F3DMatrix4 mX, mY, mZ;
    mX.RotateX( pitch );
    mY.RotateY( yaw );
    mZ.RotateZ( roll );
    *this = mX * mY * mZ;
}

/** Make the rotation matrix by euler angle.
*/
void F3DMatrix4::RotateEulerXZY( float pitch, float roll, float yaw )
{
	F3DMatrix4 mX, mY, mZ;
    mX.RotateX( pitch );
    mY.RotateY( yaw );
    mZ.RotateZ( roll );
    *this = mX * mZ * mY;
}

/** Make the rotation matrix by euler angle.
*/
void F3DMatrix4::RotateEulerYXZ( float yaw, float pitch, float roll )
{
	F3DMatrix4 mX, mY, mZ;
    mX.RotateX( pitch );
    mY.RotateY( yaw );
    mZ.RotateZ( roll );
    *this = mY * mX * mZ;
}

/** Make the rotation matrix by euler angle.
*/
void F3DMatrix4::RotateEulerYZX( float yaw, float roll, float pitch )
{
	F3DMatrix4 mX, mY, mZ;
    mX.RotateX( pitch );
    mY.RotateY( yaw );
    mZ.RotateZ( roll );
    *this = mY * mZ * mX;
}

/** Make the rotation matrix by euler angle.
*/
void F3DMatrix4::RotateEulerZXY( float roll, float pitch, float yaw )
{
	F3DMatrix4 mX, mY, mZ;
    mX.RotateX( pitch );
    mY.RotateY( yaw );
    mZ.RotateZ( roll );
    *this = mZ * mX * mY;
}

/** Make the rotation matrix by euler angle.
*/
void F3DMatrix4::RotateEulerZYX( float roll, float yaw, float pitch )
{
	F3DMatrix4 mX, mY, mZ;
    mX.RotateX( pitch );
    mY.RotateY( yaw );
    mZ.RotateZ( roll );
    *this = mZ * mY * mX;
}

/** Make the rotation matrix around a specified axis.
*/
void F3DMatrix4::RotateAxis( const F3DVector3& vAxis, float f )
{
    float fCos = cosf(f);
    float fSin = sinf(f);
    float fSum = 1.0f - fCos;

    _11 = (vAxis.x * vAxis.x) * fSum + fCos;
    _12 = (vAxis.y * vAxis.x) * fSum + (vAxis.z * fSin);
    _13 = (vAxis.z * vAxis.x) * fSum - (vAxis.y * fSin);

    _21 = (vAxis.x * vAxis.y) * fSum - (vAxis.z * fSin);
    _22 = (vAxis.y * vAxis.y) * fSum + fCos;
    _23 = (vAxis.z * vAxis.y) * fSum + (vAxis.x * fSin);

    _31 = (vAxis.x * vAxis.z) * fSum + (vAxis.y * fSin);
    _32 = (vAxis.y * vAxis.z) * fSum - (vAxis.x * fSin);
    _33 = (vAxis.z * vAxis.z) * fSum + fCos;

    _14 = _24 = _34 = _41 = _42 = _43 = 0.0f;
    _44 = 1.0f;
}

// Make the rotation matrix around a specified axis.
void F3DMatrix4::RotateAxis( const F3DVector3& vPos, const F3DVector3& vAxis, float fAngle )
{
	Translate(-vPos.x, -vPos.y, -vPos.z);

	F3DMatrix4 mat;
	mat.RotateAxis(vAxis, fAngle);
	*this *= mat;
	mat.Translate(vPos.x, vPos.y, vPos.z);
	*this *= mat;
}

/** Make a translation matrix.
*/
void F3DMatrix4::Translate( const F3DVector3& v )
{
	Identity();
    _41 = v.x;
    _42 = v.y;
    _43 = v.z;
}

/** Set the translation data for the specified matrix.
*/
void F3DMatrix4::SetTranslation( const F3DVector3& v )
{
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
}

/** Get the translation data from matrix.
*/
F3DVector3 F3DMatrix4::GetTranslation() const
{
	return F3DVector3(_41, _42, _43);
}

// Get the rotation part of the matrix.
F3DMatrix3 F3DMatrix4::GetRotationPart() const
{
	F3DMatrix3 ret;
	ret._11 = _11; ret._12 = _12; ret._13 = _13;
	ret._21 = _21; ret._22 = _22; ret._23 = _23;
	ret._31 = _31; ret._32 = _32; ret._33 = _33;
	return ret;
}

// Set the rotation part of the matrix.
void F3DMatrix4::SetRotationPart(const F3DMatrix3& mat)
{
	_11 = mat._11; _12 = mat._12; _13 = mat._13;
	_21 = mat._21; _22 = mat._22; _23 = mat._23;
	_31 = mat._31; _32 = mat._32; _33 = mat._33;
}

/** Make a scale matrix.
*/
void F3DMatrix4::Scale( float dx, float dy, float dz )
{
    Identity();
    _11 = dx;
    _22 = dy;
    _33 = dz;
}

/** Make a billboard matrix.
*/
void F3DMatrix4::Billboard(const F3DVector3& vPos, const F3DVector3& vDir,
						   const F3DVector3& vWorldUp/* =F3DVector::UNIT_Y */)
{
    F3DVector3 vUp, vRight; 
    float fAngle = 0.0f;

    fAngle = vWorldUp * vDir;

    vUp = vWorldUp - (vDir * fAngle);
    vUp.Normalize();
    vRight.Cross( vUp,vDir );

    _11 = vRight.x; _21 = vUp.x; _31 = vDir.x;
    _12 = vRight.y; _22 = vUp.y; _32 = vDir.y;
    _13 = vRight.z; _23 = vUp.z; _33 = vDir.z;

    _41 = vPos.x;
    _42 = vPos.y;
    _43 = vPos.z;

    _14 = 0.0f; _24 = 0.0f; _34 = 0.0f; _44 = 1.0f;
}

/** Make a camera view matrix.
*/
void F3DMatrix4::LookAt(const F3DVector3& vPos, const F3DVector3& vLookAt,
						const F3DVector3& vWorldUp/* =F3DVector::UNIT_Y */)
{
    F3DVector3 xAxis, yAxis, zAxis;
    zAxis = vLookAt - vPos;
    zAxis.Normalize();
    xAxis.Cross( vWorldUp, zAxis );
    xAxis.Normalize();
    yAxis.Cross( zAxis, xAxis );

    _14 = _24 = _34 = 0.0f;
    _44 = 1.0f;

    _11 = xAxis.x;
    _21 = xAxis.y;
    _31 = xAxis.z;
    _41 = -(xAxis * vPos);

    _12 = yAxis.x;
    _22 = yAxis.y;
    _32 = yAxis.z;
    _42 = -(yAxis * vPos);

    _13 = zAxis.x;
    _23 = zAxis.y;
    _33 = zAxis.z;
    _43 = -(zAxis * vPos);
}

/** Make a camera view matrix.
*/
void F3DMatrix4::MakeView3D(const F3DVector3& vRight, const F3DVector3& vUp, const F3DVector3& vDir,
							const F3DVector3& vPos)
{
    this->_14 = this->_24 = this->_34 = 0.0f;
    this->_44 = 1.0f;

    this->_11 = vRight.x;
    this->_21 = vRight.y;
    this->_31 = vRight.z;
    this->_41 = -(vRight * vPos);

    this->_12 = vUp.x;
    this->_22 = vUp.y;
    this->_32 = vUp.z;
    this->_42 = -(vUp * vPos);

    this->_13 = vDir.x;
    this->_23 = vDir.y;
    this->_33 = vDir.z;
    this->_43 = -(vDir * vPos);
}

/** Make a mirror matrix.
*/
void F3DMatrix4::Mirror( const F3DPlane& plane )
{
    _11 = -2.0f * plane.m_vN.x*plane.m_vN.x + 1.0f;
    _12 = -2.0f * plane.m_vN.y*plane.m_vN.x;
    _13 = -2.0f * plane.m_vN.z*plane.m_vN.x;
    _14 =  0.0f;
    _21 = -2.0f * plane.m_vN.x*plane.m_vN.y;
    _22 = -2.0f * plane.m_vN.y*plane.m_vN.y + 1.0f;
    _23 = -2.0f * plane.m_vN.z*plane.m_vN.y;
    _24 =  0.0f;
    _31 = -2.0f * plane.m_vN.x*plane.m_vN.z;
    _32 = -2.0f * plane.m_vN.y*plane.m_vN.z;
    _33 = -2.0f * plane.m_vN.z*plane.m_vN.z + 1.0f;
    _34 =  0.0f;
    _41 = -2.0f * plane.m_vN.x*plane.m_fD;
    _42 = -2.0f * plane.m_vN.y*plane.m_fD;
    _43 = -2.0f * plane.m_vN.z*plane.m_fD;
    _44 =  1.0f;
}

/** Transpose myself.
*/
void F3DMatrix4::Transpose()
{
    F_Swap(_12, _21);
    F_Swap(_13, _32);
    F_Swap(_14, _41);
    F_Swap(_23, _32);
    F_Swap(_24, _42);
    F_Swap(_34, _43);
}

/** Get the transpose matrix of the specified one.
*/
void F3DMatrix4::TransposeOf( const F3DMatrix4& mat )
{
	_11 = mat._11; _12 = mat._21; _13 = mat._31; _14 = mat._41;
	_21 = mat._12; _22 = mat._22; _23 = mat._32; _24 = mat._42;
	_31 = mat._13; _32 = mat._23; _33 = mat._33; _34 = mat._43;
	_41 = mat._14; _42 = mat._24; _43 = mat._34; _44 = mat._44;
}

// Calculate the inverse matrix of me.
void F3DMatrix4::Inverse()
{
	InverseOf(*this);
}

/** Get the inverse matrix of the specified one.
*/
void F3DMatrix4::InverseOf( const F3DMatrix4& mat )
{
    F3DMatrix4 mTrans;
    float fTemp[12], fDet;

    mTrans.TransposeOf(mat);

    fTemp[0]  = mTrans._33 * mTrans._44;
    fTemp[1]  = mTrans._34 * mTrans._43;
    fTemp[2]  = mTrans._32 * mTrans._44;
    fTemp[3]  = mTrans._34 * mTrans._42;
    fTemp[4]  = mTrans._32 * mTrans._43;
    fTemp[5]  = mTrans._33 * mTrans._42;
    fTemp[6]  = mTrans._31 * mTrans._44;
    fTemp[7]  = mTrans._34 * mTrans._41;
    fTemp[8]  = mTrans._31 * mTrans._43;
    fTemp[9]  = mTrans._33 * mTrans._41;
    fTemp[10] = mTrans._31 * mTrans._42;
    fTemp[11] = mTrans._32 * mTrans._41;

    this->_11  = fTemp[0]*mTrans._22 + fTemp[3]*mTrans._23 + fTemp[4] *mTrans._24;
    this->_11 -= fTemp[1]*mTrans._22 + fTemp[2]*mTrans._23 + fTemp[5] *mTrans._24;
    this->_12  = fTemp[1]*mTrans._21 + fTemp[6]*mTrans._23 + fTemp[9] *mTrans._24;
    this->_12 -= fTemp[0]*mTrans._21 + fTemp[7]*mTrans._23 + fTemp[8] *mTrans._24;
    this->_13  = fTemp[2]*mTrans._21 + fTemp[7]*mTrans._22 + fTemp[10]*mTrans._24;
    this->_13 -= fTemp[3]*mTrans._21 + fTemp[6]*mTrans._22 + fTemp[11]*mTrans._24;
    this->_14  = fTemp[5]*mTrans._21 + fTemp[8]*mTrans._22 + fTemp[11]*mTrans._23;
    this->_14 -= fTemp[4]*mTrans._21 + fTemp[9]*mTrans._22 + fTemp[10]*mTrans._23;
    this->_21  = fTemp[1]*mTrans._12 + fTemp[2]*mTrans._13 + fTemp[5] *mTrans._14;
    this->_21 -= fTemp[0]*mTrans._12 + fTemp[3]*mTrans._13 + fTemp[4] *mTrans._14;
    this->_22  = fTemp[0]*mTrans._11 + fTemp[7]*mTrans._13 + fTemp[8] *mTrans._14;
    this->_22 -= fTemp[1]*mTrans._11 + fTemp[6]*mTrans._13 + fTemp[9] *mTrans._14;
    this->_23  = fTemp[3]*mTrans._11 + fTemp[6]*mTrans._12 + fTemp[11]*mTrans._14;
    this->_23 -= fTemp[2]*mTrans._11 + fTemp[7]*mTrans._12 + fTemp[10]*mTrans._14;
    this->_24  = fTemp[4]*mTrans._11 + fTemp[9]*mTrans._12 + fTemp[10]*mTrans._13;
    this->_24 -= fTemp[5]*mTrans._11 + fTemp[8]*mTrans._12 + fTemp[11]*mTrans._13;

    fTemp[0]  = mTrans._13 * mTrans._24;
    fTemp[1]  = mTrans._14 * mTrans._23;
    fTemp[2]  = mTrans._12 * mTrans._24;
    fTemp[3]  = mTrans._14 * mTrans._22;
    fTemp[4]  = mTrans._12 * mTrans._23;
    fTemp[5]  = mTrans._13 * mTrans._22;
    fTemp[6]  = mTrans._11 * mTrans._24;
    fTemp[7]  = mTrans._14 * mTrans._21;
    fTemp[8]  = mTrans._11 * mTrans._23;
    fTemp[9]  = mTrans._13 * mTrans._21;
    fTemp[10] = mTrans._11 * mTrans._22;
    fTemp[11] = mTrans._12 * mTrans._21;

    this->_31  = fTemp[0] *mTrans._42 + fTemp[3] *mTrans._43 + fTemp[4] *mTrans._44;
    this->_31 -= fTemp[1] *mTrans._42 + fTemp[2] *mTrans._43 + fTemp[5] *mTrans._44;
    this->_32  = fTemp[1] *mTrans._41 + fTemp[6] *mTrans._43 + fTemp[9] *mTrans._44;
    this->_32 -= fTemp[0] *mTrans._41 + fTemp[7] *mTrans._43 + fTemp[8] *mTrans._44;
    this->_33  = fTemp[2] *mTrans._41 + fTemp[7] *mTrans._42 + fTemp[10]*mTrans._44;
    this->_33 -= fTemp[3] *mTrans._41 + fTemp[6] *mTrans._42 + fTemp[11]*mTrans._44;
    this->_34  = fTemp[5] *mTrans._41 + fTemp[8] *mTrans._42 + fTemp[11]*mTrans._43;
    this->_34 -= fTemp[4] *mTrans._41 + fTemp[9] *mTrans._42 + fTemp[10]*mTrans._43;
    this->_41  = fTemp[2] *mTrans._33 + fTemp[5] *mTrans._34 + fTemp[1] *mTrans._32;
    this->_41 -= fTemp[4] *mTrans._34 + fTemp[0] *mTrans._32 + fTemp[3] *mTrans._33;
    this->_42  = fTemp[8] *mTrans._34 + fTemp[0] *mTrans._31 + fTemp[7] *mTrans._33;
    this->_42 -= fTemp[6] *mTrans._33 + fTemp[9] *mTrans._34 + fTemp[1] *mTrans._31;
    this->_43  = fTemp[6] *mTrans._32 + fTemp[11]*mTrans._34 + fTemp[3] *mTrans._31;
    this->_43 -= fTemp[10]*mTrans._34 + fTemp[2] *mTrans._31 + fTemp[7] *mTrans._32;
    this->_44  = fTemp[10]*mTrans._33 + fTemp[4] *mTrans._31 + fTemp[9] *mTrans._32;
    this->_44 -= fTemp[8] *mTrans._32 + fTemp[11]*mTrans._33 + fTemp[5] *mTrans._31;

    fDet = mTrans._11*this->_11 + 
		mTrans._12*this->_12 + 
        mTrans._13*this->_13 +
        mTrans._14*this->_14;

    fDet = 1.0f / fDet;

    this->_11 *= fDet;
    this->_12 *= fDet;
    this->_13 *= fDet;
    this->_14 *= fDet;

    this->_21 *= fDet;
    this->_22 *= fDet;
    this->_23 *= fDet;
    this->_24 *= fDet;

    this->_31 *= fDet;
    this->_32 *= fDet;
    this->_33 *= fDet;
    this->_34 *= fDet;

    this->_41 *= fDet;
    this->_42 *= fDet;
    this->_43 *= fDet;
    this->_44 *= fDet;
}

/** Make a transform matrix.
*/
void F3DMatrix4::MakeTransform( const F3DVector3& vPos, const F3DQuat& qOrienuation,
                                const F3DVector3& vScale )
{
    F3DMatrix4 mScale;
    F3DMatrix4 mRotation;

    qOrienuation.GetMatrix( mRotation );
    mRotation._41 = vPos.x;
    mRotation._42 = vPos.y;
    mRotation._43 = vPos.z;

    mScale.Identity();
    mScale._11 = vScale.x;
    mScale._22 = vScale.y;
    mScale._33 = vScale.z;

    *this = mScale * mRotation;
}

/** Decompose the translation, rotation and scale data from the matrix.
*/
void F3DMatrix4::DecomposeMatrix(F3DVector3& vScale, F3DQuat& qOrient, F3DVector3& vPos)
{
	F3DVector3 vecX, vecY, vecZ;

	vecX.Set(_11, _12, _13);
	vecY.Set(_21, _22, _23);
	vecZ.Set(_31, _32, _33);

	vScale.x = vecX.GetLength();
	vScale.y = vecY.GetLength();
	vScale.z = vecZ.GetLength();

	vPos.Set(_41, _42, _43);

	F3DMatrix4 matOrient;
	matOrient.Identity();
	matOrient.SetRow(0, F3DVector4(vecX / vScale.x));
	matOrient.SetRow(1, F3DVector4(vecY / vScale.y));
	matOrient.SetRow(2, F3DVector4(vecZ / vScale.z));
	qOrient.GetMatrix(matOrient);
}

/** Make an orthotropic projection matrix.
matrix =
{
2/(r-l)			0				0				0
0				2/(t-b)			0				0
0				0				2/(f-n)			0
-(r+l)/(r-l)	-(r+b)/(t-b)	-(f+n)/(f-n)	1
}
*/
void F3DMatrix4::MakeOrthoProj( float l, float r, float b, float t, float n, float f )
{
	FASSERT( l < r && b < t && n < f );

    float x	= 2.0f / (r - l);
    float y = 2.0f / (t - b);
    float z = 2.0f / (f - n);
    float tx = -(r+l) / (r-l);
    float ty = -(t+b) / (t-b);
    float tz = -(f+n) / (f-n);

    memset( &_11, 0, sizeof(F3DMatrix4) );
    this->_11 = x;
    this->_22 = y;
    this->_33 = z;
    this->_44 = 1.0f;
    this->_41 = tx;
    this->_42 = ty;
    this->_43 = tz;
}

/** Make a perspetive projection matrix.
matrix =
{
	w		0		0		0
	0		h		0		0
	0		0		q		1
	0		0		-q*n	0
}
*/
void F3DMatrix4::MakePerspProj( float fov, float aspect, float n, float f )
{
    FASSERT( f > n );

    float h = cosf(fov/2.0f) / sinf(fov/2.0f);
    float w = h / aspect;
    float q = f / (f - n);

    memset( &_11, 0, sizeof(F3DMatrix4) );
    this->_11 = w;
    this->_22 = h;
    this->_33 = q;
    this->_34 = 1.0f;
    this->_43 = -q * n;
}

/** Rotate a vertex inversely.
*/
void F3DMatrix4::ApplyInverseRotate( F3DVector3& v )
{
    v.x = v.x * _11 + v.y * _12 + v.z * _13;
    v.y = v.x * _21 + v.y * _22 + v.z * _23;
    v.z = v.x * _31 + v.y * _32 + v.z * _33;
}