//------------------------------------------------------------------------
// Name: FlyMatrix.cpp
// Desc: This file is to implement the class FlyMatrix.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyVector.h"
#include "FlyMatrix.h"


void FlyMatrix::Identity(void)
{
	float *f = (float*)&this->_11;
	memset(f, 0, sizeof(FlyMatrix));
	_11 = _22 = _33 = _44 = 1.0f;
}


//------------------------------------------------------------------------
void FlyMatrix::RotaX(float a)
{
	float fCos = cosf(a);
	float fSin = sinf(a);

	_22 =  fCos;
	_23 =  fSin;
	_32 = -fSin;
	_33 =  fCos;

	_11 = _44 = 1.0f;
	_12 = _13 = _14 = _21 = _24 = _31 = _34 = _41 = _42 = _43 = 0.0f;
}


//------------------------------------------------------------------------
void FlyMatrix::RotaY(float a)
{
	float fCos = cosf(a);
	float fSin = sinf(a);

	_11 =  fCos;
	_13 = -fSin;
	_31 =  fSin;
	_33 =  fCos;

	_22 = _44 = 1.0f;
	_12 = _23 = _14 = _21 = _24 = _32 = _34 = _41 = _42 = _43 = 0.0f;
}


//------------------------------------------------------------------------
void FlyMatrix::RotaZ(float a)
{
	float fCos = cosf(a);
	float fSin = sinf(a);

	_11  =  fCos;
	_12  =  fSin;
	_21  = -fSin;
	_22  =  fCos;

	_33 = _44 = 1.0f;
	_13 = _14 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f;
}


//------------------------------------------------------------------------
void FlyMatrix::ApplyInverseRota(FlyVector *pvc)
{
	pvc->x = pvc->x * _11 + pvc->y * _12 + pvc->z * _13;
	pvc->y = pvc->x * _21 + pvc->y * _22 + pvc->z * _23;
	pvc->z = pvc->x * _31 + pvc->y * _32 + pvc->z * _33;
	pvc->w = 1.0f;
}


//------------------------------------------------------------------------
void FlyMatrix::Rota(float x, float y, float z)
{
	Rota(FlyVector(x, y, z));
}


//------------------------------------------------------------------------
void FlyMatrix::Rota(const FlyVector &vc)
{
	float sr, sp, sy, cr, cp, cy;

	Identity();

	sy = sinf( vc.z );
	cy = cosf( vc.z );
	sp = sinf( vc.y );
	cp = cosf( vc.y );
	sr = sinf( vc.x );
	cr = cosf( vc.x );

	_11 = cp*cy;
	_12 = cp*sy;
	_13 = -sp;
	_21 = sr*sp*cy+cr*(-sy);
	_22 = sr*sp*sy+cr*cy;
	_23 = sr*cp;
	_31 = cr*sp*cy+(-sr)*(-sy);
	_32 = cr*sp*sy+(-sr)*cy;
	_33 = cr*cp;
}


//------------------------------------------------------------------------
void FlyMatrix::SetTranslation(FlyVector vc, bool b)
{
	if(b) Identity();

	_41 = vc.x;
	_42 = vc.y;
	_43 = vc.z;
}


//------------------------------------------------------------------------
FlyVector FlyMatrix::GetTranslation(void)
{
	return FlyVector(_41, _42, _43);
}


//------------------------------------------------------------------------
void FlyMatrix::RotaArbi(const FlyVector& _vcAxis, float a)
{
    FlyVector vcAxis = _vcAxis;

    float fCos = cosf( a );
    float fSin = sinf( a );
    float fOneMinusCos = 1.0f - fCos;
    float fX2 = vcAxis.x*vcAxis.x;
    float fY2 = vcAxis.y*vcAxis.y;
    float fZ2 = vcAxis.z*vcAxis.z;
    float fXYM = vcAxis.x*vcAxis.y*fOneMinusCos;
    float fXZM = vcAxis.x*vcAxis.z*fOneMinusCos;
    float fYZM = vcAxis.y*vcAxis.z*fOneMinusCos;
    float fXSin = vcAxis.x*fSin;
    float fYSin = vcAxis.y*fSin;
    float fZSin = vcAxis.z*fSin;

    _11 = fX2*fOneMinusCos+fCos;
    _12 = fXYM-fZSin;
    _13 = fXZM+fYSin;
    _21 = fXYM+fZSin;
    _22 = fY2*fOneMinusCos+fCos;
    _23 = fYZM-fXSin;
    _31 = fXZM-fYSin;
    _32 = fYZM+fXSin;
    _33 = fZ2*fOneMinusCos+fCos;
}


//------------------------------------------------------------------------
float FlyMatrix::GetValue( int x,int y )
{
    assert( x >= 0 && x < 4 );
    assert( y >= 0 && y < 4 );

    float* p = &_11;
    p += x * 4 + y;

    return *p;
}


//------------------------------------------------------------------------
void FlyMatrix::Translate(float dx, float dy, float dz)
{
	_41 = dx;
	_42 = dy;
	_43 = dz;
}


//------------------------------------------------------------------------
void FlyMatrix::LookAt(FlyVector vcPos, FlyVector vcLookAt,FlyVector vcWorldUp)
{
	FlyVector vcDir = vcLookAt - vcPos;
	FlyVector vcUp, vcRight; 
	float     fAngle=0.0f;

	vcDir.Normalize();

	fAngle = vcWorldUp * vcDir;

	vcUp = vcWorldUp - (vcDir * fAngle);
	vcUp.Normalize();

	vcRight.Cross(vcUp, vcDir);

	_11 = vcRight.x; _21 = vcUp.x; _31 = vcDir.x;
	_12 = vcRight.y; _22 = vcUp.y; _32 = vcDir.y;
	_13 = vcRight.z; _23 = vcUp.z; _33 = vcDir.z;

	_41 = vcPos.x;
	_42 = vcPos.y;
	_43 = vcPos.z;

	_14=0.0f; _24=0.0f; _34=0.0f; _44=1.0f;
}


//------------------------------------------------------------------------
void FlyMatrix::Billboard(FlyVector vcPos,FlyVector vcDir,FlyVector vcWorldUp)
{
	FlyVector vcUp, vcRight; 
	float     fAngle = 0.0f;

	fAngle = vcWorldUp * vcDir;

	vcUp = vcWorldUp - (vcDir * fAngle);
	vcUp.Normalize();

	vcRight.Cross(vcUp, vcDir);

	_11 = vcRight.x; _21 = vcUp.x; _31 = vcDir.x;
	_12 = vcRight.y; _22 = vcUp.y; _32 = vcDir.y;
	_13 = vcRight.z; _23 = vcUp.z; _33 = vcDir.z;

	_41 = vcPos.x;
	_42 = vcPos.y;
	_43 = vcPos.z;

	_14=0.0f; _24=0.0f; _34=0.0f; _44=1.0f;
}


//------------------------------------------------------------------------
FlyMatrix FlyMatrix::operator * ( const FlyMatrix& m ) const
{
	FlyMatrix mResult;

	float* pA = (float*)this;
	float* pB = (float*)&m;
	float* pM = (float*)&mResult;

	memset( pM, 0, sizeof(FlyMatrix) );

	for( unsigned char i=0; i<4; i++ )
	{
		for( unsigned char j=0; j<4; j++ )
		{
			pM[4*i+j] += pA[4*i]   * pB[j];
			pM[4*i+j] += pA[4*i+1] * pB[4+j];
			pM[4*i+j] += pA[4*i+2] * pB[8+j];
			pM[4*i+j] += pA[4*i+3] * pB[12+j];
		}
	}

	return mResult;
}


//------------------------------------------------------------------------
FlyVector FlyMatrix::operator * (const FlyVector &vc) const
{
    FlyVector vcResult;

	vcResult.x = vc.x*_11 + vc.y*_21 + vc.z*_31 + _41;
	vcResult.y = vc.x*_12 + vc.y*_22 + vc.z*_32 + _42;
	vcResult.z = vc.x*_13 + vc.y*_23 + vc.z*_33 + _43;
	vcResult.w = vc.x*_14 + vc.y*_24 + vc.z*_34 + _44;

	vcResult.x = vcResult.x/vcResult.w;
	vcResult.y = vcResult.y/vcResult.w;
	vcResult.z = vcResult.z/vcResult.w;
	vcResult.w = 1.0f;

	return vcResult;
}


//------------------------------------------------------------------------
void FlyMatrix::TransposeOf(const FlyMatrix &m)
{
	_11 = m._11;
	_21 = m._12;
	_31 = m._13;
	_41 = m._14;

	_12 = m._21;
	_22 = m._22;
	_32 = m._23;
	_42 = m._24;

	_13 = m._31;
	_23 = m._32;
	_33 = m._33;
	_43 = m._34;

	_14 = m._41;
	_24 = m._42;
	_34 = m._43;
	_44 = m._44;
}


//------------------------------------------------------------------------
void FlyMatrix::InverseOf(const FlyMatrix &m)
{
	FlyMatrix mTrans;
	float fTemp[12],fDet;

	// calculate transposed matrix
	mTrans.TransposeOf(m);

	// Paare  die ersten 8 Kofaktoren
	fTemp[ 0]  = mTrans._33 * mTrans._44;
	fTemp[ 1]  = mTrans._34 * mTrans._43;
	fTemp[ 2]  = mTrans._32 * mTrans._44;
	fTemp[ 3]  = mTrans._34 * mTrans._42;
	fTemp[ 4]  = mTrans._32 * mTrans._43;
	fTemp[ 5]  = mTrans._33 * mTrans._42;
	fTemp[ 6]  = mTrans._31 * mTrans._44;
	fTemp[ 7]  = mTrans._34 * mTrans._41;
	fTemp[ 8]  = mTrans._31 * mTrans._43;
	fTemp[ 9]  = mTrans._33 * mTrans._41;
	fTemp[10]  = mTrans._31 * mTrans._42;
	fTemp[11]  = mTrans._32 * mTrans._41;

	// Berechne die ersten 8 Kofaktoren
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


	fTemp[ 0]  = mTrans._13 * mTrans._24;
	fTemp[ 1]  = mTrans._14 * mTrans._23;
	fTemp[ 2]  = mTrans._12 * mTrans._24;
	fTemp[ 3]  = mTrans._14 * mTrans._22;
	fTemp[ 4]  = mTrans._12 * mTrans._23;
	fTemp[ 5]  = mTrans._13 * mTrans._22;
	fTemp[ 6]  = mTrans._11 * mTrans._24;
	fTemp[ 7]  = mTrans._14 * mTrans._21;
	fTemp[ 8]  = mTrans._11 * mTrans._23;
	fTemp[ 9]  = mTrans._13 * mTrans._21;
	fTemp[10]  = mTrans._11 * mTrans._22;
	fTemp[11]  = mTrans._12 * mTrans._21;

	// Berechne die zweiten 8 Kofaktoren
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

	fDet =	mTrans._11*this->_11 + 
			mTrans._12*this->_12 + 
			mTrans._13*this->_13 +
			mTrans._14*this->_14;

	fDet = 1 / fDet;

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