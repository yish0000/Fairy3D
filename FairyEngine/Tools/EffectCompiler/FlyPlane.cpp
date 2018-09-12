//------------------------------------------------------------------------
// Name: FlyPlane.cpp
// Desc: This file is to implement the class FlyPlane.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyVector.h"
#include "FlyRay.h"
#include "FlyObb.h"
#include "FlyAabb.h"
#include "FlyPlane.h"
#include "FlyPolygon.h"


//------------------------------------------------------------------------
void FlyPlane::Set(const FlyVector &vcN, const FlyVector &vcP)
{
    m_vcN = vcN;
    m_vcN.Normalize();
	m_fD = - ( m_vcN * vcP);
}


//------------------------------------------------------------------------
void FlyPlane::Set(const FlyVector &vcN,float fD)
{
	m_vcN = vcN;
    m_vcN.Normalize();
	m_fD = fD;
}


//------------------------------------------------------------------------
void FlyPlane::Set(const FlyVector &v0,const FlyVector &v1,const FlyVector &v2)
{
	FlyVector vcEdge1 = v1 - v0;
	FlyVector vcEdge2 = v2 - v0;

	m_vcN.Cross(vcEdge1, vcEdge2);
	m_vcN.Normalize();
	m_fD = - (m_vcN * v0);
}


//------------------------------------------------------------------------
void FlyPlane::Normalize(void)
{
    float fL = m_vcN.GetLength();

    m_vcN /= fL;
    m_fD  /= fL;
}


//------------------------------------------------------------------------
float FlyPlane::Distance(const FlyVector &vcPoint)
{
	return _fabs((m_vcN*vcPoint) + m_fD);
}


//------------------------------------------------------------------------
float FlyPlane::DotCoord(const FlyVector &vcPoint)
{
    return m_vcN*vcPoint + m_fD;
}


//------------------------------------------------------------------------
int FlyPlane::Classify(const FlyVector &vcPoint)
{
	float f = (vcPoint * m_vcN) + m_fD;

	if (f >  0.00001) return FLY_FRONT;
	if (f < -0.00001) return FLY_BACK;

	return FLY_PLANAR;
}


//------------------------------------------------------------------------
int FlyPlane::Classify(const FlyPolygon &Poly)
{
	int NumFront=0, NumBack=0, NumPlanar=0;
	int nClass;

	// cast away const
	FlyPolygon *pPoly = ((FlyPolygon*)&Poly);

	int NumPoints = pPoly->GetNumPoints();

	// loop through all points
	for(int i=0; i < NumPoints; i++)
	{
		nClass = Classify( pPoly->m_pPoints[i] );

		if(nClass == FLY_FRONT)     NumFront++;
		else if(nClass == FLY_BACK) NumBack++;
		else
		{
			NumFront++;
			NumBack++;
			NumPlanar++;
		}
	}

	// all points are planar
	if (NumPlanar == NumPoints)
		return FLY_PLANAR;
	// all points are in front of plane
	else if (NumFront == NumPoints)
		return FLY_FRONT;
	// all points are on backside of plane
	else if (NumBack == NumPoints)
		return FLY_BACK;
	// poly is intersecting the plane
	else
		return FLY_CLIPPED;
}



//------------------------------------------------------------------------
bool FlyPlane::Clip(const FlyRay *pRay,float fL,FlyRay *pF,FlyRay *pB,float* t)
{
	FlyVector vcHit(0.0f,0.0f,0.0f);

	FlyRay *ray = (FlyRay*)pRay;

	// ray intersects plane at all?
	if ( !ray->Intersects( *this, false, fL, t, &vcHit) ) 
		return false;

	int n = Classify( ray->m_vcOrig );

	// ray comes fron planes backside
	if ( n == FLY_BACK )
	{
		if (pB) pB->Set(ray->m_vcOrig, ray->m_vcDir);
		if (pF) pF->Set(vcHit, ray->m_vcDir);
	}

	// ray comes from planes front side
	else if ( n == FLY_FRONT )
	{
		if (pF) pF->Set(ray->m_vcOrig, ray->m_vcDir);
		if (pB) pB->Set(vcHit, ray->m_vcDir);
	}

	return true;
}


//------------------------------------------------------------------------
bool FlyPlane::Intersects(const FlyPlane &plane, FlyRay *pIntersection)
{
	FlyVector vcCross;
	float     fSqrLength;

	// if crossproduct of normals 0 than planes parallel
	vcCross.Cross(this->m_vcN, plane.m_vcN);
	fSqrLength = vcCross.GetSqrLength();

	if (fSqrLength < 1e-08f) 
		return false;

	// find line of intersection
	if (pIntersection)
	{
		float fN00 = this->m_vcN.GetSqrLength();
		float fN01 = this->m_vcN * plane.m_vcN;
		float fN11 = plane.m_vcN.GetSqrLength();
		float fDet = fN00*fN11 - fN01*fN01;

		if (_fabs(fDet) < 1e-08f) 
			return false;

		float fInvDet = 1.0f/fDet;
		float fC0 = (fN11*this->m_fD - fN01*plane.m_fD) * fInvDet;
		float fC1 = (fN00*plane.m_fD - fN01*this->m_fD) * fInvDet;

		(*pIntersection).m_vcDir  = vcCross;
		(*pIntersection).m_vcOrig = this->m_vcN*fC0 + plane.m_vcN*fC1;
	}

	return true;
}


//------------------------------------------------------------------------
bool FlyPlane::Intersects(const FlyVector &vc0,const FlyVector &vc1,const FlyVector &vc2)
{
	int n = this->Classify(vc0);

	if( (n == this->Classify(vc1))&&(n == this->Classify(vc2)) )
		return false;

	return true;
}


//------------------------------------------------------------------------
bool FlyPlane::Intersects(const FlyAabb &aabb)
{
	FlyVector Vmin, Vmax;

	// x component
	if (m_vcN.x >= 0.0f)
	{
		Vmin.x = aabb.vcMin.x;
		Vmax.x = aabb.vcMax.x;
	}
	else
	{
		Vmin.x = aabb.vcMax.x;
		Vmax.x = aabb.vcMin.x;
	}

	// y component
	if(m_vcN.y >= 0.0f)
	{
		Vmin.y = aabb.vcMin.y;
		Vmax.y = aabb.vcMax.y;
	}
	else
	{
		Vmin.y = aabb.vcMax.y;
		Vmax.y = aabb.vcMin.y;
	}

	// z component
	if(m_vcN.z >= 0.0f)
	{
		Vmin.z = aabb.vcMin.z;
		Vmax.z = aabb.vcMax.z;
	}
	else
	{
		Vmin.z = aabb.vcMax.z;
		Vmax.z = aabb.vcMin.z;
	}

	if(((m_vcN * Vmin) + m_fD) > 0.0f)
		return false;

	if(((m_vcN * Vmax) + m_fD) >= 0.0f)
		return true;

	return false;
}


//------------------------------------------------------------------------
bool FlyPlane::Intersects(const FlyObb &obb)
{
    float fRadius = _fabs( obb.fA0 * (m_vcN * obb.vcA0) ) 
                  + _fabs( obb.fA1 * (m_vcN * obb.vcA1) ) 
                  + _fabs( obb.fA2 * (m_vcN * obb.vcA2) );

    float fDistance = this->Distance(obb.vcCenter);

    return (fDistance <= fRadius);
}