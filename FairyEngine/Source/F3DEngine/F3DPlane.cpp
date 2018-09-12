/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPlane.cpp
 *  Desc:   This file define the plane class for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DRay.h"
#include "F3DAABB.h"
#include "F3DOBB.h"
#include "F3DPlane.h"
#include "F3DPolygon.h"
#include "F3DPlane.h"

/** Normalize the plane.
*/
void F3DPlane::Normalize(void)
{
    float fL = m_vN.GetLength();

    if( fL > 1e-08f )
    {
        float fInvLen = 1.0f / fL;
        m_vN *= fInvLen;
        m_fD *= fInvLen;
    }
}

/** Get the distant to the point. (Absolute value)
*/
float F3DPlane::Distance( const F3DVector3& point ) const
{
    return fabs( (m_vN*point) + m_fD );
}

/** Get the distant to the point.
*/
float F3DPlane::DotCoord( const F3DVector3& point ) const
{
    return m_vN*point + m_fD;
}

/** Classify the relation between point and plane. 
*/
int F3DPlane::Classify( const F3DVector3& point ) const
{
	float f = (point * m_vN) + m_fD;

	if( f > 0.0f ) return F3D_MATH_FRONT;
	if( f < 0.0f ) return F3D_MATH_BACK;

	return F3D_MATH_PLANAR;
}

/** Classify the relation between specified AABB and plane. 
*/
int F3DPlane::Classify( const F3DAABB& box ) const
{
    F3DVector3 vCenter = (box.vMin + box.vMax) * 0.5f;
    F3DVector3 vHalfSize = (box.vMax - box.vMin) * 0.5f;

    float fDist = DotCoord( vCenter );
    float fMaxAbsDist = fabs(m_vN.x*vHalfSize.x) +
                        fabs(m_vN.y*vHalfSize.y) +
                        fabs(m_vN.z*vHalfSize.z);

    if( fDist < -fMaxAbsDist ) return F3D_MATH_BACK;
    if( fDist >  fMaxAbsDist ) return F3D_MATH_FRONT;
    return F3D_MATH_CLIPPED;
}

/** Classify the relation between specified polygon and plane. 
*/
int F3DPlane::Classify( const F3DPolygon& polygon ) const
{
    int numFront=0,numBack=0,numPlanar=0;
    int nClass;

	int numPoints = polygon.GetNumPoints();
    const F3DVector3* pPoints = polygon.GetPoints();

	// loop through all points
	for( int i=0;i<numPoints;i++ )
	{
		nClass = Classify( pPoints[i] );

		if( nClass == F3D_MATH_FRONT ) numFront++;
		else if( nClass == F3D_MATH_BACK ) numBack++;
		else
		{
			numFront++;
			numBack++;
			numPlanar++;
		}
	}

	// all points are planar
	if( numPlanar == numPoints )
		return F3D_MATH_PLANAR;
	// all points are in front of plane
	else if( numFront == numPoints )
		return F3D_MATH_FRONT;
	// all points are on backside of plane
	else if( numBack == numPoints )
		return F3D_MATH_BACK;
	// polygon is intersecting the plane
	else
		return F3D_MATH_CLIPPED;
}

/** Clip the specified ray.
*/
bool F3DPlane::Clip( const F3DRay& ray, float fL, F3DRay* pF, F3DRay* pB, float* t ) const
{
	F3DVector3 vHit(0.0f, 0.0f, 0.0f);

	// ray intersects plane at all?
	if( !ray.Intersect(*this, false, fL, t, &vHit) )
		return false;

	int n = Classify( ray.m_vOrig );

	// ray comes fron planes backside
	if( n == F3D_MATH_BACK )
	{
		if( pB ) pB->Set( ray.m_vOrig,ray.m_vDir );
		if( pF ) pF->Set( vHit,ray.m_vDir );
	}

	// ray comes from planes front side
	else if( n == F3D_MATH_FRONT )
	{
		if( pF ) pF->Set( ray.m_vOrig,ray.m_vDir );
		if( pB ) pB->Set( vHit,ray.m_vDir );
	}

	return true;
}

/** Check whether the plane intersect with me.
*/
bool F3DPlane::Intersect( const F3DPlane& plane, F3DRay* pIntersection ) const
{
    F3DVector3 vCross;
    float fSqrLength;

    // if crossproduct of normals 0 than planes parallel
    vCross.Cross( this->m_vN,plane.m_vN );
    fSqrLength = vCross.GetSqrLength();

    if( fSqrLength < 1e-08f )
        return false;

    // find line of intersection
    if( pIntersection )
    {
        float fN00 = this->m_vN.GetSqrLength();
        float fN01 = this->m_vN * plane.m_vN;
        float fN11 = plane.m_vN.GetSqrLength();
        float fDet = fN00*fN11 - fN01*fN01;

        if( fabs(fDet) < 1e-08f )
            return false;

        float fInvDet = 1.0f / fDet;
        float fC0 = (fN11*this->m_fD - fN01*plane.m_fD) * fInvDet;
        float fC1 = (fN00*plane.m_fD - fN01*this->m_fD) * fInvDet;

        (*pIntersection).m_vDir = vCross;
        (*pIntersection).m_vOrig = this->m_vN*fC0 + plane.m_vN*fC1;
    }

    return true;
}

/** Check whether the triangle intersect with me.
*/
bool F3DPlane::Intersect( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2 ) const
{
	int n = this->Classify( v0 );

	if( (n == this->Classify(v1)) && (n == this->Classify(v2)) )
		return false;

	return true;
}

/** Check whether the AABB intersect with me.
*/
bool F3DPlane::Intersect( const F3DAABB& box ) const
{
    F3DVector3 vmin,vmax;

    // x component
    if( m_vN.x >= 0.0f )
    {
        vmin.x = box.vMin.x;
        vmax.x = box.vMax.x;
    }
    else
    {
        vmin.x = box.vMax.x;
        vmax.x = box.vMin.x;
    }

    // y component
    if( m_vN.y >= 0.0f )
    {
        vmin.y = box.vMin.y;
        vmax.y = box.vMax.y;
    }
    else
    {
        vmin.y = box.vMax.y;
        vmax.y = box.vMin.y;
    }

    // z component
    if( m_vN.z >= 0.0f )
    {
        vmin.z = box.vMin.z;
        vmax.z = box.vMax.z;
    }
    else
    {
        vmin.z = box.vMax.z;
        vmax.z = box.vMin.z;
    }

    if( ((m_vN * vmin) + m_fD) > 0.0f )
        return false;

    if( ((m_vN * vmax) + m_fD) >= 0.0f )
        return true;

    return false;
}

/** Check whether the OBB intersect with me.
*/
bool F3DPlane::Intersect( const F3DOBB& box ) const
{
    float fRadius = fabs( box.fA0 * (m_vN * box.vA0) ) 
                  + fabs( box.fA1 * (m_vN * box.vA1) ) 
                  + fabs( box.fA2 * (m_vN * box.vA2) );

    return (this->Distance(box.vCenter) <= fRadius);
}