/*
 * ------------------------------------------------------------------------
 *  Name:   F3DVector.cpp
 *  Desc:   This file implements the vector types for engine.
 *  Author: Yish
 *  Date:   2010/11/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DMatrix.h"
#include "F3DQuat.h"

///////////////////////////////////////////////////////////////////////////
//// SOME CONSTANT VARIABLES

const F3DVector3 F3DVector3::ZERO( 0.0f,0.0f,0.0f );
const F3DVector3 F3DVector3::UNIT_X( 1.0f,0.0f,0.0f );
const F3DVector3 F3DVector3::UNIT_Y( 0.0f,1.0f,0.0f );
const F3DVector3 F3DVector3::UNIT_Z( 0.0f,0.0f,1.0f );

///////////////////////////////////////////////////////////////////////////

/** Rotate me with a matrix.
*/
void F3DVector3::RotateWith(const F3DMatrix4& m)
{
	float _x = x * m._11 + y * m._21 + z * m._31;
	float _y = x * m._12 + y * m._22 + z * m._32;
	float _z = x * m._13 + y * m._23 + z * m._33;

	x = _x;
	y = _y;
	z = _z;
}

/** Rotate me with a matrix inversely.
*/
void F3DVector3::InvRotateWith(const F3DMatrix4& m)
{
	float _x = x * m._11 + y * m._12 + z * m._13;
	float _y = x * m._21 + y * m._22 + z * m._23;
	float _z = x * m._31 + y * m._32 + z * m._33;

	x = _x;
	y = _y;
	z = _z;
}

/** Transform the vector with a quaternion.
*/
F3DVector3 F3DVector3::operator * ( const F3DQuat& q ) const
{
	F3DVector4 vec(
        q.w*x + q.z*y - q.y*z,
        q.w*y + q.x*z - q.z*x,
        q.w*z + q.y*x - q.x*y,
        -(q.x*x + q.y*y + q.z*z)
        );
	return F3DVector3(vec.x, vec.y, vec.z);
}

/** Transform the vector with a matrix
*/
F3DVector3 F3DVector3::operator * ( const F3DMatrix4& m ) const
{
	F3DVector4 vcResult(*this);

	vcResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	vcResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	vcResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	vcResult.w = x*m._14 + y*m._24 + z*m._34 + m._44;

    float fInv = 1.0f / vcResult.w;
	vcResult.x = vcResult.x * fInv;
	vcResult.y = vcResult.y * fInv;
	vcResult.z = vcResult.z * fInv;
	vcResult.w = 1.0f;

	return F3DVector3(vcResult.x, vcResult.y, vcResult.z);
}