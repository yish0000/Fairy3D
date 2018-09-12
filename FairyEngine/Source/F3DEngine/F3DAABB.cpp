/*
 * ------------------------------------------------------------------------
 *  Name:   F3DAABB.cpp
 *  Desc:   This file define the Axis aligned bounding box for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DOBB.h"
#include "F3DRay.h"
#include "F3DSphere.h"
#include "F3DPlane.h"
#include "F3DMatrix.h"
#include "F3DAABB.h"

///////////////////////////////////////////////////////////////////////////
//// Define some constant variables.

// Empty bouding box.
const F3DAABB F3DAABB::BOX_EMPTY(F3DVector3::ZERO, F3DVector3::ZERO);

// Infinite bounding box.
const F3DAABB F3DAABB::BOX_INFINITE(F3DVector3(F3D_FLOAT_MIN, F3D_FLOAT_MIN, F3D_FLOAT_MIN),
                                    F3DVector3(F3D_FLOAT_MAX, F3D_FLOAT_MAX, F3D_FLOAT_MAX));

///////////////////////////////////////////////////////////////////////////

/** Construct the AABB from OBB.
*/
void F3DAABB::Construct( const F3DOBB& obb )
{
    F3DVector3 vA0, vA1, vA2;
    F3DVector3 _vMax, _vMin;

    vA0 = obb.vA0 * obb.fA0;
    vA1 = obb.vA1 * obb.fA1;
    vA2 = obb.vA2 * obb.fA2;

	if( vA0.x > vA1.x )
	{
		if( vA0.x > vA2.x )
		{
			vMax.x =  vA0.x;
			vMin.x = -vA0.x;
		}
		else
		{
			vMax.x =  vA2.x;
			vMin.x = -vA2.x;
		}
	}
	else
	{
		if( vA1.x > vA2.x )
		{
			vMax.x =  vA1.x;
			vMin.x = -vA1.x;
		}
		else
		{
			vMax.x =  vA2.x;
			vMin.x = -vA2.x;
		}
	}

	if( vA0.y > vA1.y )
	{
		if( vA0.y > vA2.y )
		{
			vMax.y =  vA0.y;
			vMin.y = -vA0.y;
		}
		else
		{
			vMax.y =  vA2.y;
			vMin.y = -vA2.y;
		}
	}
	else
	{
		if( vA1.y > vA2.y )
		{
			vMax.y =  vA1.y;
			vMin.y = -vA1.y;
		}
		else
		{
			vMax.y =  vA2.y;
			vMin.y = -vA2.y;
		}
	}

	if( vA0.z > vA1.z )
	{
		if( vA0.z > vA2.z )
		{
			vMax.z =  vA0.z;
			vMin.z = -vA0.z;
		}
		else
		{
			vMax.z =  vA2.z;
			vMin.z = -vA2.z;
		}
	}
	else
	{
		if( vA1.z > vA2.z )
		{
			vMax.z =  vA1.z;
			vMin.z = -vA1.z;
		}
		else
		{
			vMax.z =  vA2.z;
			vMin.z = -vA2.z;
		}
	}

	vMax = vMax + obb.vCenter;
	vMin = vMin + obb.vCenter;
}

/** Construct the AABB from sphere.
*/
void F3DAABB::Construct( const F3DSphere& sphere )
{
#pragma F_COMPILE_WARN("Fix me!  F3DAABB::Construct sphere")
    F3DVector3 v(1.44225f*sphere.m_fRadius,
                 1.44225f*sphere.m_fRadius,
                 1.44225f*sphere.m_fRadius);

    vMin = sphere.m_vCenter - v;
    vMax = sphere.m_vCenter + v;
}

/** Construct the AABB from vertices.
*/
void F3DAABB::Construct( const F3DVector3* pVertices, uint32 nNumVerts )
{
	Zero();

	for(uint32 i=0; i<nNumVerts; i++)
	{
		const F3DVector3& vert = pVertices[i];

		vMin.x = F_Min(vMin.x, vert.x);
		vMin.y = F_Min(vMin.y, vert.y);
		vMin.z = F_Min(vMin.z, vert.z);
		vMax.x = F_Max(vMax.x, vert.x);
		vMax.y = F_Max(vMax.y, vert.y);
		vMax.z = F_Max(vMax.z, vert.z);
	}
}

/** Check where the box is culled by the specified clip planes.
@Param Clip planes
@Param Count
*/
int F3DAABB::Cull( const F3DPlane* planes, int numPlanes ) const
{
    F3DVector3 _vMin,_vMax;
    bool bIntersects = false;

    for( int i=0;i<numPlanes;i++ )
    {
        // x coordinate
        if( planes[i].m_vN.x >= 0.0f )
        {
            _vMin.x = this->vMin.x;
            _vMax.x = this->vMax.x;
        }
        else
        {
            _vMin.x = this->vMax.x;
            _vMax.x = this->vMin.x;
        }

        // y coordinate
        if( planes[i].m_vN.y >= 0.0f )
        {
            _vMin.y = this->vMin.y;
            _vMax.y = this->vMax.y;
        }
        else
        {
            _vMin.y = this->vMax.y;
            _vMax.y = this->vMin.y;
        }

        // z coordinate
        if( planes[i].m_vN.z >= 0.0f )
        {
            _vMin.z = this->vMin.z;
            _vMax.z = this->vMax.z;
        }
        else
        {
            _vMin.z = this->vMax.z;
            _vMax.z = this->vMin.z;
        }

        if( planes[i].DotCoord(_vMin) < 0.0f )
            return F3D_MATH_CULLED;

        if( planes[i].DotCoord(_vMax) >= 0.0f )
            bIntersects = true;
    }

    if( bIntersects ) return F3D_MATH_CLIPPED;

    return F3D_MATH_VISIBLE;
}

/** Merge me with another AABB.
*/
void F3DAABB::Merge( const F3DAABB& box )
{
    if( IsEmpty() )
    {
        vMin = box.vMin;
        vMax = box.vMax;
        return;
    }

    vMin.x = F_Min( vMin.x, box.vMin.x );
    vMin.y = F_Min( vMin.y, box.vMin.y );
    vMin.z = F_Min( vMin.z, box.vMin.z );
    vMax.x = F_Max( vMax.x, box.vMax.x );
    vMax.y = F_Max( vMax.y, box.vMax.y );
    vMax.z = F_Max( vMax.z, box.vMax.z );
}

/** Merge a point to me.
*/
void F3DAABB::Merge( const F3DVector3& point )
{
    if( IsEmpty() )
    {
        vMin = point - 0.5f;
        vMax = point + 0.5f;
        return;
    }

    vMin.x = F_Min( vMin.x,point.x );
    vMin.y = F_Min( vMin.y,point.y );
    vMin.z = F_Min( vMin.z,point.z );
    vMax.x = F_Max( vMax.x,point.x );
    vMax.y = F_Max( vMax.y,point.y );
    vMax.z = F_Max( vMax.z,point.z );
}

/** Get the six planes of AABB (Normal outwards)
*/
void F3DAABB::GetPlanes( F3DPlane* planes ) const
{
    F3DVector3 vN;

    if( !planes ) return;

    // Right
    vN.Set( 1.0f, 0.0f, 0.0f );
    planes[0].Set( vN, vMax );

    // Left
    vN.Set( -1.0f, 0.0f, 0.0f );
    planes[1].Set( vN, vMin );

    // Front
    vN.Set( 0.0f, 0.0f, -1.0f );
    planes[2].Set( vN, vMin );

    // Back
    vN.Set( 0.0f, 0.0f, 1.0f );
    planes[3].Set( vN,vMax );

    // Top
    vN.Set( 0.0f, 1.0f, 0.0f );
    planes[4].Set( vN, vMax );

    // Bottom
    vN.Set( 0.0f, -1.0f, 0.0f );
    planes[5].Set( vN, vMin );
}

/** Check whether the AABB contain the specified point.
*/
bool F3DAABB::Contain( const F3DVector3& point ) const
{
    if( point.x > vMax.x ) return false;
    if( point.y > vMax.y ) return false;
    if( point.z > vMax.z ) return false;
    if( point.x < vMin.x ) return false;
    if( point.y < vMin.y ) return false;
    if( point.z < vMin.z ) return false;

    return true;
}

/** Check whether the AABB contain the specified ray.
@Param Ray
@Param Length of segment in the ray.
*/
bool F3DAABB::Contain( const F3DRay& ray, float fL ) const
{
    F3DVector3 vEnd = ray.m_vOrig + (ray.m_vDir*fL);
    return (Contain(ray.m_vOrig) && Contain(vEnd));
}

/** Check whether the AABB intersect with the ray.
*/
bool F3DAABB::Intersect( const F3DRay& ray, float* t ) const
{
    bool bInside = true;
    float t0,t1,tmp;
    float tNear = -999999.9f;
    float tFar = 999999.9f;
    F3DVector3 MaxT;

    // first pair of planes
    if( fabs(ray.m_vDir.x) < F3D_EPSILON )
    {
        if( (ray.m_vOrig.x < vMin.x) || (ray.m_vOrig.x > vMax.x) )
            return false;
    }

    t0 = (vMin.x - ray.m_vOrig.x) / ray.m_vDir.x;
    t1 = (vMax.x - ray.m_vOrig.x) / ray.m_vDir.x;
    if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
    if( t0 > tNear ) tNear = t0;
    if( t1 < tFar ) tFar = t1;
    if( tNear > tFar ) return false;
    if( tFar < 0 ) return false;

    // second pair of planes
    if( fabs(ray.m_vDir.y) < F3D_EPSILON )
    {
        if( (ray.m_vOrig.y < vMin.y) || (ray.m_vOrig.y > vMax.y) )
            return false;
    }

    t0 = (vMin.y - ray.m_vOrig.y) / ray.m_vDir.y;
    t1 = (vMax.y - ray.m_vOrig.y) / ray.m_vDir.y;
    if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
    if( t0 > tNear ) tNear = t0;
    if( t1 < tFar ) tFar = t1;
    if( tNear > tFar ) return false;
    if( tFar < 0 ) return false;

    // third pair of planes
    if( fabs(ray.m_vDir.z) < F3D_EPSILON )
    {
        if( (ray.m_vOrig.z < vMin.z) || (ray.m_vOrig.z > vMax.z) )
            return false;
    }

    t0 = (vMin.z - ray.m_vOrig.z) / ray.m_vDir.z;
    t1 = (vMax.z - ray.m_vOrig.z) / ray.m_vDir.z;
    if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
    if( t0 > tNear ) tNear = t0;
    if( t1 < tFar ) tFar = t1;
    if( tNear > tFar ) return false;
    if( tFar < 0 ) return false;

    if( tNear > 0 )
    {
        if( t ) *t = tNear;
    }
    else
    {
        if( t ) *t = tFar;
    }

    return true;
}

/** heck whether the AABB intersect with the ray.
*/
bool F3DAABB::Intersect( const F3DRay& ray, float fL, float* t ) const
{
    bool bInside = true;
    float t0,t1,tmp,tFinal;
    float tNear = -999999.9f;
    float tFar = 999999.9f;
    F3DVector3 MaxT;

    // first pair of planes
    if( fabs(ray.m_vDir.x) < F3D_EPSILON )
    {
        if( (ray.m_vOrig.x < vMin.x) || (ray.m_vOrig.x > vMax.x) )
            return false;
    }

    t0 = (vMin.x - ray.m_vOrig.x) / ray.m_vDir.x;
    t1 = (vMax.x - ray.m_vOrig.x) / ray.m_vDir.x;
    if( t0 > t1 ) { tmp=t0; t0=t1; t1=tmp; }
    if( t0 > tNear ) tNear = t0;
    if( t1 < tFar ) tFar = t1;
    if( tNear > tFar ) return false;
    if( tFar < 0 ) return false;

    // second pair of planes
    if( fabs(ray.m_vDir.y) < F3D_EPSILON )
    {
        if( (ray.m_vOrig.y < vMin.y) || (ray.m_vOrig.y > vMax.y) )
            return false;
    }

    t0 = (vMin.y - ray.m_vOrig.y) / ray.m_vDir.y;
    t1 = (vMax.y - ray.m_vOrig.y) / ray.m_vDir.y;
    if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
    if( t0 > tNear ) tNear = t0;
    if( t1 < tFar ) tFar = t1;
    if( tNear > tFar ) return false;
    if( tFar < 0 ) return false;

    // third pair of planes
    if( fabs(ray.m_vDir.z) < F3D_EPSILON )
    {
        if( (ray.m_vOrig.z < vMin.z) || (ray.m_vOrig.z > vMax.z) )
            return false;
    }

    t0 = (vMin.z - ray.m_vOrig.z) / ray.m_vDir.z;
    t1 = (vMax.z - ray.m_vOrig.z) / ray.m_vDir.z;
    if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
    if( t0 > tNear ) tNear = t0;
    if( t1 < tFar ) tFar = t1;
    if( tNear > tFar ) return false;
    if( tFar < 0 ) return false;

    if( tNear > 0 )
        tFinal = tNear;
    else
        tFinal = tFar;

    if( tFinal > fL ) return false;
    if( t ) *t = tFinal;

    return true;
}

/** Check whether the AABB intersect with another AABB.
*/
bool F3DAABB::Intersect( const F3DAABB& box ) const
{
	if( (vMin.x > box.vMax.x) || (box.vMin.x > vMax.x) )
		return false;
	if( (vMin.y > box.vMax.y) || (box.vMin.y > vMax.y) )
		return false;
	if( (vMin.z > box.vMax.z) || (box.vMin.z > vMax.z) )
		return false;

	return true;
}

/** Check whether the box contain another AABB.
*/
int F3DAABB::ContainBox( const F3DAABB& box ) const
{
    if( box.vMin.x > vMax.x ||
        box.vMin.y > vMax.y ||
        box.vMin.z > vMax.z ||
        box.vMax.x < vMin.x ||
        box.vMax.y < vMin.y ||
        box.vMax.z < vMin.z )
    {
        return F3D_MATH_OUTSIDE;
    }

    if( box.vMin.x > vMin.x &&
        box.vMin.y > vMin.y &&
        box.vMin.z > vMin.z &&
        box.vMax.x < vMax.x &&
        box.vMax.y < vMax.y &&
        box.vMax.z < vMax.z )
    {
        return F3D_MATH_INSIDE;
    }
    else
        return F3D_MATH_INTERSECT;
}

/** Transform the AABB.
*/
void F3DAABB::TransformBox( const F3DMatrix4& m )
{
    F3DVector3 vCenter = (vMin + vMax) * 0.5f;
    F3DVector3 vHalfSize = (vMax - vMin) * 0.5f;

    vCenter = vCenter * m;
    F3DVector3 vNewHalfSize(
        fabs(m._11)*vHalfSize.x+fabs(m._21)*vHalfSize.y+fabs(m._31)*vHalfSize.z,
        fabs(m._12)*vHalfSize.x+fabs(m._22)*vHalfSize.y+fabs(m._32)*vHalfSize.z,
        fabs(m._13)*vHalfSize.x+fabs(m._23)*vHalfSize.y+fabs(m._33)*vHalfSize.z
        );

    vMin = vCenter - vNewHalfSize;
    vMax = vCenter + vNewHalfSize;
}

/** Get the bounding sphere's radius.
*/
float F3DAABB::GetBoundingRadius(void) const
{
    float fX = fabs( vMax.x - vMin.x ) * 0.5f;
    float fY = fabs( vMax.y - vMin.y ) * 0.5f;
    float fZ = fabs( vMax.z - vMin.z ) * 0.5f;

    return sqrtf(fX * fX + fY * fY + fZ * fZ);
}

/** Get the center point.
*/
F3DVector3 F3DAABB::GetCenter(void) const
{
    return ( vMin + vMax ) * 0.5f;
}

/** Get the extents of box.
*/
F3DVector3 F3DAABB::GetExtents(void) const
{
    F3DVector3 vExtents;
    vExtents.x = (vMax.x - vMin.x) * 0.5f;
    vExtents.y = (vMax.y - vMin.y) * 0.5f;
    vExtents.z = (vMax.z - vMin.z) * 0.5f;
    return vExtents;
}

/** Get the specified corner.
*/
F3DVector3 F3DAABB::GetCorner( int cornerID ) const
{
    F3DVector3 vc;
    float fSizeX = vMax.x - vMin.x;
    float fSizeY = vMax.y - vMin.y;
    float fSizeZ = vMax.z - vMin.z;

    switch( cornerID )
    {
    case CORNER_LDB:
        vc = vMin;
        break;
    case CORNER_LUB:
        vc = vMin;
        vc.z += fSizeZ;
        break;
    case CORNER_RUB:
        vc = vMax;
        vc.y -= fSizeY;
        break;
    case CORNER_RDB:
        vc = vMin;
        vc.x += fSizeX;
        break;
    case CORNER_LDT:
        vc = vMin;
        vc.y += fSizeY;
        break;
    case CORNER_LUT:
        vc = vMax;
        vc.x -= fSizeX;
        break;
    case CORNER_RUT:
        vc = vMax;
        break;
    case CORNER_RDT:
        vc = vMax;
        vc.z -= fSizeZ;
        break;
	default: FASSERT(0); break;
    }

    return vc;
}