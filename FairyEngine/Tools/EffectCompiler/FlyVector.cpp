//------------------------------------------------------------------------
// Name: FlyVector.cpp
// Desc: This file is to implement the class FlyVector.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyMath.h"


//------------------------------------------------------------------------
void FlyVector::Set(float _x, float _y, float _z, float _w)
{
	x=_x; y=_y; z=_z; w=_w;
} 


//------------------------------------------------------------------------
float FlyVector::GetLength(void)
{
	float f= (float)sqrt(x*x + y*y + z*z); 

	return f;
}


//------------------------------------------------------------------------
float FlyVector::GetSqrLength(void) const
{
	return (x*x + y*y + z*z); 
}


//------------------------------------------------------------------------
void FlyVector::Negate(void)
{
	x = -x;  y = -y;  z = -z;
}


//------------------------------------------------------------------------
float FlyVector::AngleWith(FlyVector &v)
{
	return (float)acos( ((*this) * v) / (this->GetLength()*v.GetLength()) );
}


//------------------------------------------------------------------------
void FlyVector::Normalize(void)
{
	float f = (float)sqrt(x*x + y*y + z*z);

	if(f != 0.0f)
	{
		x/=f;
		y/=f;
		z/=f;
	}
}


//------------------------------------------------------------------------
void FlyVector::Difference(const FlyVector &v1, const FlyVector &v2)
{
	x = v2.x - v1.x;
	y = v2.y - v1.y;
	z = v2.z - v1.z;
	w = 1.0f;
}

//------------------------------------------------------------------------
void FlyVector::Cross(const FlyVector &v1, const FlyVector &v2)
{
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
	w = 1.0f;
}

//------------------------------------------------------------------------
void FlyVector::RotateWith(const FlyMatrix &m)
{
	// applying rotational part of matrix only
	float _x = x * m._11 + y * m._21 + z * m._31;
	float _y = x * m._12 + y * m._22 + z * m._32;
	float _z = x * m._13 + y * m._23 + z * m._33;

	x = _x;
	y = _y;
	z = _z;
}


//------------------------------------------------------------------------
void FlyVector::InvRotateWith(const FlyMatrix &m)
{
	// using transposed matrix
	float _x = x * m._11 + y * m._12 + z * m._13;
	float _y = x * m._21 + y * m._22 + z * m._23;
	float _z = x * m._31 + y * m._32 + z * m._33;

	x = _x;
	y = _y;
	z = _z;
}


//------------------------------------------------------------------------
void FlyVector::operator += (const FlyVector &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}


//------------------------------------------------------------------------
FlyVector FlyVector::operator + (const FlyVector &v) const
{
	return FlyVector(x+v.x, y+v.y, z+v.z);
}


//------------------------------------------------------------------------
void FlyVector::operator -= (const FlyVector &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}


//------------------------------------------------------------------------
FlyVector FlyVector::operator - (const FlyVector &v) const
{
	return FlyVector(x-v.x, y-v.y, z-v.z);
}


//------------------------------------------------------------------------
void FlyVector::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
}


//------------------------------------------------------------------------
void FlyVector::operator /= (float f)
{
	x /= f;
	y /= f;
	z /= f;
}


//------------------------------------------------------------------------
FlyVector FlyVector::operator * (float f) const
{
	return FlyVector(x*f, y*f, z*f);
}


//------------------------------------------------------------------------
FlyVector FlyVector::operator / (float f) const
{
	return FlyVector(x/f, y/f, z/f);
}


//------------------------------------------------------------------------
void FlyVector::operator += (float f)
{
	x += f;
	y += f;
	z += f;
}


//------------------------------------------------------------------------
void FlyVector::operator -= (float f)
{
	x -= f;
	y -= f;
	z -= f;
}


//------------------------------------------------------------------------
FlyVector FlyVector::operator + (float f) const
{
	return FlyVector(x+f, y+f, z+f);
}


//------------------------------------------------------------------------
FlyVector FlyVector::operator - (float f) const
{
	return FlyVector(x-f, y-f, z-f);
}


//------------------------------------------------------------------------
float FlyVector::operator * (const FlyVector &v) const
{
	return (v.x*x + v.y*y + v.z*z);
}


//------------------------------------------------------------------------
FlyQuat FlyVector::operator * (const FlyQuat &q) const
{
	return FlyQuat( q.w*x + q.z*y - q.y*z,
                    q.w*y + q.x*z - q.z*x,
                    q.w*z + q.y*x - q.x*y,
                  -(q.x*x + q.y*y + q.z*z) );
}


//------------------------------------------------------------------------
FlyVector FlyVector::operator * (const FlyMatrix &m) const
{
	FlyVector vcResult;

	vcResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	vcResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	vcResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	vcResult.w = x*m._14 + y*m._24 + z*m._34 + m._44;

	vcResult.x = vcResult.x/vcResult.w;
	vcResult.y = vcResult.y/vcResult.w;
	vcResult.z = vcResult.z/vcResult.w;
	vcResult.w = 1.0f;

	return vcResult;
}