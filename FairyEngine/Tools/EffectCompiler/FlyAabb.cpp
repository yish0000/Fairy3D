//------------------------------------------------------------------------
// Name: FlyAabb.cpp
// Desc: This file is to implement the class FlyAabb.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyVector.h"
#include "FlyRay.h"
#include "FlyPlane.h"
#include "FlyObb.h"
#include "FlyAabb.h"


//------------------------------------------------------------------------
FlyAabb::FlyAabb(FlyVector _vcMin, FlyVector _vcMax)
{
	vcMin = _vcMin;
	vcMax = _vcMax;
	vcCenter = (vcMax + vcMin) / 2.0f;
}


//------------------------------------------------------------------------
void FlyAabb::Construct(const FlyObb *pObb)
{
	FlyVector vcA0, vcA1, vcA2;
	FlyVector _vcMax, _vcMin;

	vcA0 = pObb->vcA0 * pObb->fA0;
	vcA1 = pObb->vcA1 * pObb->fA1;
	vcA2 = pObb->vcA2 * pObb->fA2;

	// find x extensions
	if(vcA0.x > vcA1.x)
	{
		if(vcA0.x > vcA2.x)
		{
			vcMax.x =  vcA0.x;
			vcMin.x = -vcA0.x;
		}
		else
		{
			vcMax.x =  vcA2.x;
			vcMin.x = -vcA2.x;
		}
	}
	else
	{
		if(vcA1.x > vcA2.x)
		{
			vcMax.x =  vcA1.x;
			vcMin.x = -vcA1.x;
		}
		else
		{
			vcMax.x =  vcA2.x;
			vcMin.x = -vcA2.x;
		}
	}

	// find y extensions
	if(vcA0.y > vcA1.y)
	{
		if(vcA0.y > vcA2.y)
		{
			vcMax.y =  vcA0.y;
			vcMin.y = -vcA0.y;
		}
		else
		{
			vcMax.y =  vcA2.y;
			vcMin.y = -vcA2.y;
		}
	}
	else
	{
		if(vcA1.y > vcA2.y)
		{
			vcMax.y =  vcA1.y;
			vcMin.y = -vcA1.y;
		}
		else
		{
			vcMax.y =  vcA2.y;
			vcMin.y = -vcA2.y;
		}
	}

	// find z extensions
	if(vcA0.z > vcA1.z)
	{
		if(vcA0.z > vcA2.z)
		{
			vcMax.z =  vcA0.z;
			vcMin.z = -vcA0.z;
		}
		else
		{
			vcMax.z =  vcA2.z;
			vcMin.z = -vcA2.z;
		}
	}
	else
	{
		if(vcA1.z > vcA2.z)
		{
			vcMax.z =  vcA1.z;
			vcMin.z = -vcA1.z;
		}
		else
		{
			vcMax.z =  vcA2.z;
			vcMin.z = -vcA2.z;
		}
	}

	vcMax = vcMax + pObb->vcCenter;
	vcMin = vcMin + pObb->vcCenter;
    vcCenter = (vcMin + vcMax) / 2.0f;
}


//------------------------------------------------------------------------
// Merge the boundingBox with another box.
//------------------------------------------------------------------------
void FlyAabb::Merge( const FlyAabb& box )
{
    FlyVector vMax,vMin;

    FlyAabb aabb = (FlyAabb)box;
    vMax.x = FLYMATH_MAX( vcMax.x,aabb.vcMax.x );
    vMax.y = FLYMATH_MAX( vcMax.y,aabb.vcMax.y );
    vMax.z = FLYMATH_MAX( vcMax.z,aabb.vcMax.z );
    vMin.x = FLYMATH_MIN( vcMin.x,aabb.vcMin.x );
    vMin.y = FLYMATH_MIN( vcMin.y,aabb.vcMin.y );
    vMin.z = FLYMATH_MIN( vcMin.z,aabb.vcMin.z );

    vcMin = vMin;
    vcMax = vMax;
    vcCenter = (vMin + vMax) / 2.0f;
}


//------------------------------------------------------------------------
// Check whether the box is in another box.
//------------------------------------------------------------------------
bool FlyAabb::IsInBox( const FlyAabb& box )
{
    FlyVector vMin = box.vcMin;
    FlyVector vMax = box.vcMax;

    if( vcMin.x >= vMin.x && vcMin.y >= vMin.y && vcMin.z >= vMin.z &&
        vcMax.x <= vMax.x && vcMax.y <= vMax.y && vcMax.z <= vMax.z )
    {
        return true;
    }

    return false;
}


//------------------------------------------------------------------------
// Culls AABB to n sided frustrum. Normals pointing outwards.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyPlane   - array of planes building frustrum
//      int        - number of planes in array
// ----------------------------------------------------------------------
// OUT: FLY_VISIBLE - obb totally inside frustrum
//      FLY_CLIPPED - obb clipped by frustrum
//      FLY_CULLED  - obb totally outside frustrum
//------------------------------------------------------------------------
int FlyAabb::Cull(const FlyPlane *pPlanes, int nNumPlanes)
{
	FlyVector  vcMin, vcMax;
	bool       bIntersects = false;

	// find and test extreme points
	for(int i=0; i<nNumPlanes; i++)
	{
		// x coordinate
		if(pPlanes[i].m_vcN.x >= 0.0f)
		{
			vcMin.x = this->vcMin.x;
			vcMax.x = this->vcMax.x;
		}
		else
		{
			vcMin.x = this->vcMax.x;
			vcMax.x = this->vcMin.x;
		}

		// y coordinate
		if(pPlanes[i].m_vcN.y >= 0.0f)
		{
			vcMin.y = this->vcMin.y;
			vcMax.y = this->vcMax.y;
		}
		else
		{
			vcMin.y = this->vcMax.y;
			vcMax.y = this->vcMin.y;
		}

		// z coordinate
		if(pPlanes[i].m_vcN.z >= 0.0f)
		{
			vcMin.z = this->vcMin.z;
			vcMax.z = this->vcMax.z;
		}
		else
		{
			vcMin.z = this->vcMax.z;
			vcMax.z = this->vcMin.z;
        }

		if( ((FlyPlane)pPlanes[i]).DotCoord(vcMin) < 0.0f)
			return FLY_CULLED;

		if( ((FlyPlane)pPlanes[i]).DotCoord(vcMax) >= 0.0f)
			bIntersects = true;
	}

	if(bIntersects) return FLY_CLIPPED;

	return FLY_VISIBLE;
}


//------------------------------------------------------------------------
bool FlyAabb::Intersects(const FlyRay &Ray, float *t)
{
	bool bInside = true;
	float t0, t1, tmp;
	float tNear = -999999.9f;
	float tFar  =  999999.9f;
	float epsilon = 0.00001f;
	FlyVector MaxT;

	// first pair of planes
	if(_fabs(Ray.m_vcDir.x) < epsilon)
	{
		if( (Ray.m_vcOrig.x < vcMin.x)||(Ray.m_vcOrig.x > vcMax.x) )
			return false;
	}

	t0 = (vcMin.x - Ray.m_vcOrig.x) / Ray.m_vcDir.x;
	t1 = (vcMax.x - Ray.m_vcOrig.x) / Ray.m_vcDir.x;
	if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
	if (t0 > tNear) tNear = t0;
	if (t1 < tFar)  tFar = t1;
	if (tNear > tFar) return false;
	if (tFar < 0) return false;

	// second pair of planes
	if(_fabs(Ray.m_vcDir.y) < epsilon)
	{
		if( (Ray.m_vcOrig.y < vcMin.y)||(Ray.m_vcOrig.y > vcMax.y) )
			return false;
	}

	t0 = (vcMin.y - Ray.m_vcOrig.y) / Ray.m_vcDir.y;
	t1 = (vcMax.y - Ray.m_vcOrig.y) / Ray.m_vcDir.y;
	if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
	if (t0 > tNear) tNear = t0;
	if (t1 < tFar)  tFar = t1;
	if (tNear > tFar) return false;
	if (tFar < 0) return false;

	// third pair of planes
	if(_fabs(Ray.m_vcDir.z) < epsilon)
	{
		if( (Ray.m_vcOrig.z < vcMin.z)||(Ray.m_vcOrig.z > vcMax.z) )
			return false;
	}

	t0 = (vcMin.z - Ray.m_vcOrig.z) / Ray.m_vcDir.z;
	t1 = (vcMax.z - Ray.m_vcOrig.z) / Ray.m_vcDir.z;
	if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
	if (t0 > tNear) tNear = t0;
	if (t1 < tFar)  tFar = t1;
	if (tNear > tFar) return false;
	if (tFar < 0) return false;


	if(tNear > 0)
	{
		if (t) *t = tNear;
	}
	else
	{
		if (t) *t = tFar;
	}

	return true;
}


//------------------------------------------------------------------------
bool FlyAabb::Intersects(const FlyRay &Ray, float fL, float *t)
{
	bool bInside = true;
	float t0, t1, tmp, tFinal;
	float tNear = -999999.9f;
	float tFar  =  999999.9f;
	float epsilon = 0.00001f;
	FlyVector MaxT;

	// first pair of planes
	if(_fabs(Ray.m_vcDir.x) < epsilon)
	{
		if( (Ray.m_vcOrig.x < vcMin.x)||(Ray.m_vcOrig.x > vcMax.x) )
			return false;
	}

	t0 = (vcMin.x - Ray.m_vcOrig.x) / Ray.m_vcDir.x;
	t1 = (vcMax.x - Ray.m_vcOrig.x) / Ray.m_vcDir.x;
	if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
	if (t0 > tNear) tNear = t0;
	if (t1 < tFar)  tFar = t1;
	if (tNear > tFar) return false;
	if (tFar < 0) return false;

	// second pair of planes
	if(_fabs(Ray.m_vcDir.y) < epsilon)
	{
		if( (Ray.m_vcOrig.y < vcMin.y)||(Ray.m_vcOrig.y > vcMax.y) )
			return false;
	}

	t0 = (vcMin.y - Ray.m_vcOrig.y) / Ray.m_vcDir.y;
	t1 = (vcMax.y - Ray.m_vcOrig.y) / Ray.m_vcDir.y;
	if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
	if (t0 > tNear) tNear = t0;
	if (t1 < tFar)  tFar = t1;
	if (tNear > tFar) return false;
	if (tFar < 0) return false;

	// third pair of planes
	if(_fabs(Ray.m_vcDir.z) < epsilon)
	{
		if( (Ray.m_vcOrig.z < vcMin.z)||(Ray.m_vcOrig.z > vcMax.z) )
			return false;
	}

	t0 = (vcMin.z - Ray.m_vcOrig.z) / Ray.m_vcDir.z;
	t1 = (vcMax.z - Ray.m_vcOrig.z) / Ray.m_vcDir.z;
	if (t0 > t1) { tmp=t0; t0=t1; t1=tmp; }
	if (t0 > tNear) tNear = t0;
	if (t1 < tFar)  tFar = t1;
	if (tNear > tFar) return false;
	if (tFar < 0) return false;


	if(tNear > 0)
		tFinal = tNear;
	else
		tFinal = tFar;

	if (tFinal > fL) return false;
	if (t) *t = tFinal;

	return true;
}


//------------------------------------------------------------------------
bool FlyAabb::Intersects(const FlyAabb &aabb)
{
	if((vcMin.x > aabb.vcMax.x) || (aabb.vcMin.x > vcMax.x))
		return false;
	if((vcMin.y > aabb.vcMax.y) || (aabb.vcMin.y > vcMax.y))
		return false;
	if((vcMin.z > aabb.vcMax.z) || (aabb.vcMin.z > vcMax.z))
		return false;

	return true;
}


// does aabb contain the given point
bool FlyAabb::Intersects(const FlyVector &vc)
{
	if ( vc.x > vcMax.x ) return false;
	if ( vc.y > vcMax.y ) return false;
	if ( vc.z > vcMax.z ) return false;
	if ( vc.x < vcMin.x ) return false;
	if ( vc.y < vcMin.y ) return false;
	if ( vc.z < vcMin.z ) return false;

	return true;
}


//------------------------------------------------------------------------
bool FlyAabb::Contains(const FlyRay &Ray, float fL)
{
	FlyVector vcEnd = Ray.m_vcOrig + (Ray.m_vcDir*fL);
	return ( Intersects(Ray.m_vcOrig) && Intersects(vcEnd) );
}


//------------------------------------------------------------------------
void FlyAabb::GetPlanes(FlyPlane *pPlanes)
{
	FlyVector vcN;

	if (!pPlanes) return;

	// right side
	vcN.Set(1.0f, 0.0f, 0.0f);
	pPlanes[0].Set(vcN, vcMax);

	// left side
	vcN.Set(-1.0f, 0.0f, 0.0f);
	pPlanes[1].Set(vcN, vcMin);

	// front side
	vcN.Set(0.0f, 0.0f, -1.0f);
	pPlanes[2].Set(vcN, vcMin);

	// back side
	vcN.Set(0.0f, 0.0f, 1.0f);
	pPlanes[3].Set(vcN, vcMax);

	// top side
	vcN.Set(0.0f, 1.0f, 0.0f);
	pPlanes[4].Set(vcN, vcMax);

	// bottom side
	vcN.Set(0.0f, -1.0f, 0.0f);
	pPlanes[5].Set(vcN, vcMin);
}