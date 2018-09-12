/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRay.cpp
 *  Desc:   This file implements the ray class for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DMatrix.h"
#include "F3DPlane.h"
#include "F3DOBB.h"
#include "F3DAABB.h"
#include "F3DRay.h"

/** Transform the ray.
*/
void F3DRay::DeTransform( const F3DMatrix4& m )
{
	F3DMatrix4 mInv;
    F3DMatrix4 _m = (F3DMatrix4)m;

	m_vOrig.x -= _m._41;   
	m_vOrig.y -= _m._42;
	m_vOrig.z -= _m._43;

	// Delete the translation data.
	_m._41 = _m._42 = _m._43 = 0.0f;

	mInv.InverseOf( _m );
	m_vOrig = m_vOrig * mInv;
	m_vDir = m_vDir * mInv;
}

/** Check whether the specified triangle intersect with me ?
*/
bool F3DRay::Intersect( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2,
                        bool bCull, float* t ) const
{
    F3DVector3 pvec,tvec,qvec;
    F3DVector3 edge1 = v1 - v0;
    F3DVector3 edge2 = v2 - v0;

    pvec.Cross( m_vDir,edge2 );

    // if close to 0 ray is parallel
    float det = edge1 * pvec;
    if( bCull && (det < 0.0001f) )
        return false;
    else if( (det < 0.0001f) && (det > -0.0001f) )
        return false;

    // distance to plane, < 0 means beyond plane
    tvec = m_vOrig - v0;
    float u = tvec * pvec;
    if( u < 0.0f || u > det )
        return false;

    qvec.Cross( tvec,edge1 );
    float v = m_vDir * qvec;
    if( v < 0.0f || u+v > det )
        return false;

    if( t )
    {
        *t = edge2 * qvec;
        float fInvDet = 1.0f / det;
        *t *= fInvDet;
    }

    return true;
}

/** Check whether the specified triangle intersect with me ?
*/
bool F3DRay::Intersect( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2,
                        bool bCull, float fL, float* t ) const
{
    F3DVector3 pvec,tvec,qvec;
    F3DVector3 edge1 = v1 - v0;
    F3DVector3 edge2 = v2 - v0;

    pvec.Cross( m_vDir,edge2 );

    // if close to 0 ray is parallel
    float det = edge1 * pvec;
    if( bCull && (det < 0.0001f) )
        return false;
    else if( (det < 0.0001f) && (det > -0.0001f) )
        return false;

    // distance to plane, < 0 means beyond plane
    tvec = m_vOrig - v0;
    float u = tvec * pvec;
    if( u < 0.0f || u > det )
        return false;

    qvec.Cross( tvec,edge1 );
    float v = m_vDir * qvec;
    if( v < 0.0f || u+v > det )
        return false;

    if( t )
    {
        *t = edge2 * qvec;
        float fInvDet = 1.0f / det;
        *t *= fInvDet;

        // collision but not on segment?
        if( *t > fL ) return false;
    }
    else
    {
        // collision but not on segment?
        float f = (edge2*qvec) * (1.0f / det);
        if( f > fL ) return false;
    }

    return true;
}

/** Check whether the AABB intersect with me ?
*/
bool F3DRay::Intersect( const F3DAABB& box, float* t ) const
{
	bool bInside = true;
	float t0,t1,tmp;
	float tNear = -999999.9f;
	float tFar = 999999.9f;
	F3DVector3 MaxT;

	// first pair of planes
	if( fabs(m_vDir.x) < F3D_EPSILON )
	{
		if( (m_vOrig.x < box.vMin.x) || (m_vOrig.x > box.vMax.x) )
			return false;
	}

	t0 = (box.vMin.x - m_vOrig.x) / m_vDir.x;
	t1 = (box.vMax.x - m_vOrig.x) / m_vDir.x;
	if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
	if( t0 > tNear ) tNear = t0;
	if( t1 < tFar ) tFar = t1;
	if( tNear > tFar ) return false;
	if( tFar < 0 ) return false;

	// second pair of planes
	if( fabs(m_vDir.y) < F3D_EPSILON )
	{
		if( (m_vOrig.y < box.vMin.y) || (m_vOrig.y > box.vMax.y) )
			return false;
	}

	t0 = (box.vMin.y - m_vOrig.y) / m_vDir.y;
	t1 = (box.vMax.y - m_vOrig.y) / m_vDir.y;
	if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
	if( t0 > tNear ) tNear = t0;
	if( t1 < tFar ) tFar = t1;
	if( tNear > tFar ) return false;
	if( tFar < 0 ) return false;

	// third pair of planes
	if( fabs(m_vDir.z) < F3D_EPSILON )
	{
		if( (m_vOrig.z < box.vMin.z) || (m_vOrig.z > box.vMax.z) )
			return false;
	}

	t0 = (box.vMin.z - m_vOrig.z) / m_vDir.z;
	t1 = (box.vMax.z - m_vOrig.z) / m_vDir.z;
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

/** Check whether the AABB intersect with me ?
*/
bool F3DRay::Intersect( const F3DAABB& box, float fL, float* t ) const
{
    bool bInside = true;
    float t0,t1,tmp,tFinal;
    float tNear = -999999.9f;
    float tFar = 999999.9f;
    F3DVector3 MaxT;

    // first pair of planes
    if( fabs(m_vDir.x) < F3D_EPSILON )
    {
        if( (m_vOrig.x < box.vMin.x) || (m_vOrig.x > box.vMax.x) )
            return false;
    }

    t0 = (box.vMin.x - m_vOrig.x) / m_vDir.x;
    t1 = (box.vMax.x - m_vOrig.x) / m_vDir.x;
    if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
    if( t0 > tNear ) tNear = t0;
    if( t1 < tFar ) tFar = t1;
    if( tNear > tFar ) return false;
    if( tFar < 0 ) return false;

    // second pair of planes
    if( fabs(m_vDir.y) < F3D_EPSILON )
    {
        if( (m_vOrig.y < box.vMin.y) || (m_vOrig.y > box.vMax.y) )
            return false;
    }

    t0 = (box.vMin.y - m_vOrig.y) / m_vDir.y;
    t1 = (box.vMax.y - m_vOrig.y) / m_vDir.y;
    if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
    if( t0 > tNear ) tNear = t0;
    if( t1 < tFar ) tFar = t1;
    if( tNear > tFar ) return false;
    if( tFar < 0 ) return false;

    // third pair of planes
    if( fabs(m_vDir.z) < F3D_EPSILON )
    {
        if( (m_vOrig.z < box.vMin.z) || (m_vOrig.z > box.vMax.z) )
            return false;
    }

    t0 = (box.vMin.z - m_vOrig.z) / m_vDir.z;
    t1 = (box.vMax.z - m_vOrig.z) / m_vDir.z;
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

/** Check whether the OBB intersect with me ?
*/
bool F3DRay::Intersect( const F3DOBB& box, float* t ) const
{
	float e,f,t1,t2,temp;
	float tmin = -99999.9f;
	float tmax =  99999.9f;

	F3DVector3 vP = box.vCenter - m_vOrig;

	// 1st slap
	e = box.vA0 * vP;
	f = box.vA0 * m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + box.fA0) / f;
		t2 = (e - box.fA0) / f;

		if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
		if( t1 > tmin ) tmin = t1;
		if( t2 < tmax ) tmax = t2;
		if( tmin > tmax ) return false;
		if( tmax < 0.0f ) return false;
	}
	else if( ((-e - box.fA0) > 0.0f) || ((-e + box.fA0) < 0.0f) )
    {
		return false;
    }

	// 2nd slap
	e = box.vA1 * vP;
	f = box.vA1 * m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + box.fA1) / f;
		t2 = (e - box.fA1) / f;

		if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
		if( t1 > tmin ) tmin = t1;
		if( t2 < tmax ) tmax = t2;
		if( tmin > tmax ) return false;
		if( tmax < 0.0f ) return false;
	}
	else if( ((-e - box.fA1) > 0.0f) || ((-e + box.fA1) < 0.0f) )
    {
		return false;
    }

	// 3rd slap
	e = box.vA2 * vP;
	f = box.vA2 * m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + box.fA2) / f;
		t2 = (e - box.fA2) / f;

		if(t1 > t2) { temp=t1; t1=t2; t2=temp; }
		if(t1 > tmin) tmin = t1;
		if(t2 < tmax) tmax = t2;
		if(tmin > tmax) return false;
		if(tmax < 0.0f) return false;
	}
	else if( ((-e - box.fA2) > 0.0f) || ((-e + box.fA2) < 0.0f) )
    {
		return false;
    }

	if( tmin > 0.0f )
	{
		if( t ) *t = tmin;

		return true;
	}

	if( t ) *t = tmax;

	return true;
}

/** Check whether the OBB intersect with me ?
*/
bool F3DRay::Intersect( const F3DOBB& box, float fL, float* t ) const
{
    float e,f,t1,t2,temp;
    float tmin = -99999.9f;
    float tmax =  99999.9f;

    F3DVector3 vP = box.vCenter - m_vOrig;

    // 1st slap
    e = box.vA0 * vP;
    f = box.vA0 * m_vDir;
    if( fabs(f) > 0.00001f )
    {
        t1 = (e + box.fA0) / f;
        t2 = (e - box.fA0) / f;

        if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
        if( t1 > tmin ) tmin = t1;
        if( t2 < tmax ) tmax = t2;
        if( tmin > tmax ) return false;
        if( tmax < 0.0f ) return false;
    }
    else if( ((-e - box.fA0) > 0.0f) || ((-e + box.fA0) < 0.0f) )
    {
        return false;
    }

    // 2nd slap
    e = box.vA1 * vP;
    f = box.vA1 * m_vDir;
    if( fabs(f) > 0.00001f )
    {
        t1 = (e + box.fA1) / f;
        t2 = (e - box.fA1) / f;

        if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
        if( t1 > tmin ) tmin = t1;
        if( t2 < tmax ) tmax = t2;
        if( tmin > tmax ) return false;
        if( tmax < 0.0f ) return false;
    }
    else if( ((-e - box.fA1) > 0.0f) || ((-e + box.fA1) < 0.0f) )
    {
        return false;
    }

    // 3rd slap
    e = box.vA2 * vP;
    f = box.vA2 * m_vDir;
    if( fabs(f) > 0.00001f )
    {
        t1 = (e + box.fA2) / f;
        t2 = (e - box.fA2) / f;

        if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
        if( t1 > tmin ) tmin = t1;
        if( t2 < tmax ) tmax = t2;
        if( tmin > tmax ) return false;
        if( tmax < 0.0f ) return false;
    }
    else if( ((-e - box.fA2) > 0.0f) || ((-e + box.fA2) < 0.0f) )
    {
        return false;
    }

    if( (tmin > 0.0f) && (tmin <= fL) )
    {
        if( t ) *t = tmin;

        return true;
    }

    // intersection on line but not on segment
    if( tmax > fL ) return false;

    if( t ) *t = tmax;

    return true;
}

/** Check whether the plane intersect with me ?
*/
bool F3DRay::Intersect( const F3DPlane& plane, bool bCull, float* t, F3DVector3* vcHit ) const
{
    float Vd = plane.m_vN * m_vDir;

    // ray parallel to plane
    if( fabs(Vd) < 0.00001f )
        return false;

    // normal pointing away from ray dir
    // => intersection backface if any
    if( bCull && (Vd > 0.0f) )
        return false;

    float Vo = -((plane.m_vN * m_vOrig) + plane.m_fD);

    float _t = Vo / Vd;

    // intersection behind ray origin
    if( _t < 0.0f ) return false;

    if( vcHit )
    {
        *vcHit = m_vOrig + (m_vDir * _t);
    }

    if( t ) *t = _t;

    return true;
}

/** Check whether the plane intersect with me ?
*/
bool F3DRay::Intersect( const F3DPlane& plane, bool bCull, float fL, float *t,
                        F3DVector3* vcHit ) const
{
    float Vd = plane.m_vN * m_vDir;

    // ray parallel to plane
    if( fabs(Vd) < 0.00001f )
        return false;

    // normal pointing away from ray dir
    // => intersection backface if any
    if( bCull && (Vd > 0.0f) )
        return false;

    float d = plane.m_vN * m_vOrig;
    float Vo = -( d + plane.m_fD );

	float _t = Vo / Vd;

    // intersection behind ray origin or beyond valid range
    if( (_t < 0.0f) || (_t > fL) )
        return false;

    if( vcHit )
    {
        *vcHit = m_vOrig + (m_vDir * _t);
    }

    if( t ) *t = _t;

    return true;
}