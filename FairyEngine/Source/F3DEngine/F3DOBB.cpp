/*
 * ------------------------------------------------------------------------
 *  Name:   F3DOBB.cpp
 *  Desc:   This file define an oriented bounding box for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DMatrix.h"
#include "F3DRay.h"
#include "F3DPlane.h"
#include "F3DOBB.h"

///////////////////////////////////////////////////////////////////////////
//// Some Local functions

static void OBBProj( const F3DOBB& box, const F3DVector3& v, float* pfMin, float* pfMax )
{
	float fDP = v * box.vCenter;
	float fR = box.fA0 * fabs(v * box.vA0) +
		box.fA0 * fabs(v * box.vA1) +
		box.fA1 * fabs(v * box.vA2);

	*pfMin = fDP - fR;
	*pfMax = fDP + fR;
}

static void TRIProj( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2, const F3DVector3& v,
					 float* pfMin, float* pfMax )
{
	*pfMin = v * v0;
	*pfMax = *pfMin;

	float fDP = v * v1;
	if( fDP < *pfMin )
		*pfMin = fDP;
	else if( fDP > *pfMax )
		*pfMax = fDP;

	fDP = v * v2;
	if( fDP < *pfMin )
		*pfMin = fDP;
	else if( fDP > *pfMax )
		*pfMax = fDP;
}

///////////////////////////////////////////////////////////////////////////

/** Zero the OBB.
*/
void F3DOBB::Zero()
{
	fA0 = 0.0f; fA1 = 0.0f; fA2 = 0.0f;
	vA0.Zero(); vA1.Zero(); vA2.Zero();
	vCenter.Zero();
}

/** Construct the OBB from vertices
*/
void F3DOBB::Construct(const F3DVector3* pVertices, uint32 nNumVerts)
{
#pragma F_COMPILE_WARN("F3DOBB::Construct from vertices, NOT IMPLEMENTED!!!");

	FLOG_ERROR("F3DOBB::Construct from vertices, NOT IMPLEMENTED!!!");
}

/** Construct the OBB from two other OBBs.
*/
void F3DOBB::Construct(const F3DOBB& obb1, const F3DOBB& obb2)
{
#pragma F_COMPILE_WARN("F3DOBB::Construct from two OBB, NOT IMPLEMENTED!!!");

	FLOG_ERROR("F3DOBB::Construct from two OBB, NOT IMPLEMENTED!!!");
}

/** Construct the OBB from AABB
*/
void F3DOBB::Construct(const F3DAABB& aabb)
{
	vCenter = aabb.GetCenter();
	vA0 = F3DVector3::UNIT_X;
	vA1 = F3DVector3::UNIT_Y;
	vA2 = F3DVector3::UNIT_Z;

	F3DVector3 vExtent = aabb.GetExtents();
	fA0 = vExtent.x;
	fA1 = vExtent.y;
	fA2 = vExtent.z;
}

/** Transform me from another OBB.
*/
void F3DOBB::DeTransform( const F3DOBB& box, const F3DMatrix4& m )
{
	F3DMatrix4 mat = m;
	F3DVector3 vT;

	// Get the translation data.
	vT.Set( mat._41, mat._42, mat._43 );
	mat._41 = mat._42 = mat._43 = 0.0f;

	// Rotate the axes of OBB.
	this->vCenter = mat * box.vCenter;
	this->vA0 = mat * box.vA0;
	this->vA1 = mat * box.vA1;
	this->vA2 = mat * box.vA2;

	// Set the translation data.
	this->vCenter += vT;

	// Copy the length of each edges.
	fA0 = box.fA0;
	fA1 = box.fA1;
	fA2 = box.fA2;
}

/** Transform me with a matrix.
*/
void F3DOBB::DeTransform( const F3DMatrix4& m )
{
	F3DOBB obb = *this;
	DeTransform(obb, m);
}

/** Is the specified point in the box ?
*/
bool F3DOBB::Contain(const F3DVector3& vPoint) const
{
	// Transfrom point to obb space
	F3DVector3 vd = vPoint - vCenter;

	float d = vA0 * vd;
	if( d < -fA0 || d > fA0 )
		return false;

	d = vA1 * vd;
	if( d < -fA1 || d > fA1 )
		return false;

	d = vA2 * vd;
	if( d < -fA2 || d > fA2 )
		return false;

	return true;
}

/** Check whether the triangle intersect with me ?
*/
bool F3DOBB::Intersect( const F3DVector3& v0, const F3DVector3& v1, const F3DVector3& v2 ) const
{
    float fMin0, fMax0, fMin1, fMax1;
    float fD_C;
    F3DVector3 v, vTriEdge[3], vA[3];

    // just for looping
    vA[0] = this->vA0;
    vA[1] = this->vA1;
    vA[2] = this->vA2;

    // direction of tri-normals
    vTriEdge[0] = v1 - v0;
    vTriEdge[1] = v2 - v0;

    v.Cross( vTriEdge[0], vTriEdge[1] );

    fMin0 = v * v0;
    fMax0 = fMin0;

    OBBProj( (*this), v, &fMin1, &fMax1 );
    if( fMax1 < fMin0 || fMax0 < fMin1 )
        return true;

    // direction of obb planes
    // =======================
    // axis 1:
    v = this->vA0;
    TRIProj( v0, v1, v2, v, &fMin0, &fMax0 );
    fD_C = v * this->vCenter;
    fMin1 = fD_C - this->fA0;
    fMax1 = fD_C + this->fA0;
    if( fMax1 < fMin0 || fMax0 < fMin1 )
        return true;

    // axis 2:
    v = this->vA1;
    TRIProj( v0, v1, v2, v, &fMin0, &fMax0 );
    fD_C = v * this->vCenter;
    fMin1 = fD_C - this->fA1;
    fMax1 = fD_C + this->fA1;
    if( fMax1 < fMin0 || fMax0 < fMin1 )
        return true;

    // axis 3:
    v = this->vA2;
    TRIProj( v0, v1, v2, v, &fMin0, &fMax0 );
    fD_C = v * this->vCenter;
    fMin1 = fD_C - this->fA2;
    fMax1 = fD_C + this->fA2;
    if( fMax1 < fMin0 || fMax0 < fMin1 )
        return true;

    // direction of tri-obb edge-crossproducts
    vTriEdge[2] = vTriEdge[1] - vTriEdge[0];
    for( int j=0;j<3;j++ )
    {
        for( int k=0;k<3;k++ )
        {
		    v.Cross( vTriEdge[j],vA[k] );

		    TRIProj( v0, v1, v2, v, &fMin0, &fMax0 );
		    OBBProj( (*this), v, &fMin1, &fMax1 );

            if( (fMax1 < fMin0) || (fMax0 < fMin1) )
                return true;
        }
    }

    return true;
}

/** Check whether the specified ray intersect with me ?
*/
bool F3DOBB::Intersect( const F3DRay& ray, float* t ) const
{
	float e, f, t1, t2, temp;
	float tmin = -99999.9f, tmax = 99999.9f;

	F3DVector3 vP = this->vCenter - ray.m_vOrig;

	// 1st slap
	e = this->vA0 * vP;
	f = this->vA0 * ray.m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + this->fA0) / f;
		t2 = (e - this->fA0) / f;

		if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
		if( t1 > tmin ) tmin = t1;
		if( t2 < tmax ) tmax = t2;
		if( tmin > tmax ) return false;
		if( tmax < 0.0f ) return false;
	}
	else if( ((-e - this->fA0) > 0.0f) || ((-e + this->fA0) < 0.0f) )
    {
		return false;
    }

	// 2nd slap
	e = this->vA1 * vP;
	f = this->vA1 * ray.m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + this->fA1) / f;
		t2 = (e - this->fA1) / f;

		if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
		if( t1 > tmin ) tmin = t1;
		if( t2 < tmax ) tmax = t2;
		if( tmin > tmax ) return false;
		if( tmax < 0.0f ) return false;
	}
	else if( ((-e - this->fA1) > 0.0f) || ((-e + this->fA1) < 0.0f) )
    {
		return false;
    }

	// 3rd slap
	e = this->vA2 * vP;
	f = this->vA2 * ray.m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + this->fA2) / f;
		t2 = (e - this->fA2) / f;

		if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
		if( t1 > tmin ) tmin = t1;
		if( t2 < tmax ) tmax = t2;
		if( tmin > tmax ) return false;
		if( tmax < 0.0f ) return false;
	}
	else if( ((-e - this->fA2) > 0.0f) || ((-e + this->fA2) < 0.0f) )
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

/** Check whether the specified ray intersect with me ?
*/
bool F3DOBB::Intersect( const F3DRay& ray, float fL, float* t ) const
{
	float e, f, t1, t2, temp;
	float tmin = -99999.9f;
	float tmax =  99999.9f;

	F3DVector3 vP = this->vCenter - ray.m_vOrig;

	// 1st slap
	e = this->vA0 * vP;
	f = this->vA0 * ray.m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + this->fA0) / f;
		t2 = (e - this->fA0) / f;

		if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
		if( t1 > tmin ) tmin = t1;
		if( t2 < tmax ) tmax = t2;
		if( tmin > tmax ) return false;
		if( tmax < 0.0f ) return false;
	}
	else if( ((-e - this->fA0) > 0.0f) || ((-e + this->fA0) < 0.0f) )
    {
		return false;
    }

	// 2nd slap
	e = this->vA1 * vP;
	f = this->vA1 * ray.m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + this->fA1) / f;
		t2 = (e - this->fA1) / f;

		if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
		if( t1 > tmin ) tmin = t1;
		if( t2 < tmax ) tmax = t2;
		if( tmin > tmax ) return false;
		if( tmax < 0.0f ) return false;
	}
	else if( ((-e - this->fA1) > 0.0f) || ((-e + this->fA1) < 0.0f) )
    {
		return false;
    }

	// 3rd slap
	e = this->vA2 * vP;
	f = this->vA2 * ray.m_vDir;
	if( fabs(f) > 0.00001f )
	{
		t1 = (e + this->fA2) / f;
		t2 = (e - this->fA2) / f;

		if( t1 > t2 ) { temp = t1; t1 = t2; t2 = temp; }
		if( t1 > tmin ) tmin = t1;
		if( t2 < tmax ) tmax = t2;
		if( tmin > tmax ) return false;
		if( tmax < 0.0f ) return false;
	}
	else if ( ((-e - this->fA2) > 0.0f) || ((-e + this->fA2) < 0.0f) )
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

/** Check whether another OBB intersect with me ?
*/
bool F3DOBB::Intersect( const F3DOBB& box ) const
{
    float T[3];

    // difference vector between both obb
    F3DVector3 vD = box.vCenter - this->vCenter;

    float matM[3][3];   // B's axis in relation to A
    float ra,           // radius A
          rb,           // radius B
          t;            // absolute values from T[]

    // OBB A's axis as separation axis?
    // ================================
    // first axis vcA0
    matM[0][0] = this->vA0 * box.vA0;
    matM[0][1] = this->vA0 * box.vA1;
    matM[0][2] = this->vA0 * box.vA2;
    ra = this->fA0;
    rb = box.fA0 * fabs(matM[0][0]) + 
         box.fA1 * fabs(matM[0][1]) + 
         box.fA2 * fabs(matM[0][2]);

    T[0] = vD * this->vA0;
    t = fabs( T[0] );
    if( t > (ra + rb) ) return false;

    // second axis vcA1
    matM[1][0] = this->vA1 * box.vA0;
    matM[1][1] = this->vA1 * box.vA1;
    matM[1][2] = this->vA1 * box.vA2;
    ra = this->fA1;
    rb = box.fA0 * fabs(matM[1][0]) + 
         box.fA1 * fabs(matM[1][1]) + 
         box.fA2 * fabs(matM[1][2]);
    T[1] = vD * this->vA1;
    t = fabs( T[1] );
    if( t > (ra + rb) ) return false;

    // third axis vcA2
    matM[2][0] = this->vA2 * box.vA0;
    matM[2][1] = this->vA2 * box.vA1;
    matM[2][2] = this->vA2 * box.vA2;
    ra = this->fA2;
    rb = box.fA0 * fabs(matM[2][0]) + 
         box.fA1 * fabs(matM[2][1]) + 
         box.fA2 * fabs(matM[2][2]);
    T[2] = vD * this->vA2;
    t = fabs( T[2] );
    if( t > (ra + rb) ) return false;

    // OBB B's axis as separation axis?
    // ================================
    // first axis vcA0
    ra = this->fA0 * fabs(matM[0][0]) + 
         this->fA1 * fabs(matM[1][0]) + 
         this->fA2 * fabs(matM[2][0]);
    rb = box.fA0;
    t = fabs( T[0]*matM[0][0] + T[1]*matM[1][0] + T[2]*matM[2][0] );
    if( t > (ra + rb) ) return false;

    // second axis vcA1
    ra = this->fA0 * fabs(matM[0][1]) + 
         this->fA1 * fabs(matM[1][1]) + 
         this->fA2 * fabs(matM[2][1]);
    rb = box.fA1;
    t = fabs( T[0]*matM[0][1] + T[1]*matM[1][1] + T[2]*matM[2][1] );
    if( t > (ra + rb) ) return false;

    // third axis vcA2
    ra = this->fA0 * fabs(matM[0][2]) + 
         this->fA1 * fabs(matM[1][2]) + 
         this->fA2 * fabs(matM[2][2]);
    rb = box.fA2;
    t = fabs( T[0]*matM[0][2] + T[1]*matM[1][2] + T[2]*matM[2][2] );
    if( t > (ra + rb) ) return false;

    // other candidates: cross products of axis:
    // =========================================
    // axis A0xB0
    ra = this->fA1*fabs(matM[2][0]) + this->fA2*fabs(matM[1][0]);
    rb = box.fA1*fabs(matM[0][2]) + box.fA2*fabs(matM[0][1]);
    t = fabs( T[2]*matM[1][0] - T[1]*matM[2][0] );
    if( t > ra + rb ) return false;

    // axis A0xB1
    ra = this->fA1*fabs(matM[2][1]) + this->fA2*fabs(matM[1][1]);
    rb = box.fA0*fabs(matM[0][2]) + box.fA2*fabs(matM[0][0]);
    t = fabs( T[2]*matM[1][1] - T[1]*matM[2][1] );
    if( t > ra + rb ) return false;

    // axis A0xB2
    ra = this->fA1*fabs(matM[2][2]) + this->fA2*fabs(matM[1][2]);
    rb = box.fA0*fabs(matM[0][1]) + box.fA1*fabs(matM[0][0]);
    t = fabs( T[2]*matM[1][2] - T[1]*matM[2][2] );
    if( t > ra + rb ) return false;

    // axis A1xB0
    ra = this->fA0*fabs(matM[2][0]) + this->fA2*fabs(matM[0][0]);
    rb = box.fA1*fabs(matM[1][2]) + box.fA2*fabs(matM[1][1]);
    t = fabs( T[0]*matM[2][0] - T[2]*matM[0][0] );
    if( t > ra + rb ) return false;

    // axis A1xB1
    ra = this->fA0*fabs(matM[2][1]) + this->fA2*fabs(matM[0][1]);
    rb = box.fA0*fabs(matM[1][2]) + box.fA2*fabs(matM[1][0]);
    t = fabs( T[0]*matM[2][1] - T[2]*matM[0][1] );
    if( t > ra + rb ) return false;

    // axis A1xB2
    ra = this->fA0*fabs(matM[2][2]) + this->fA2*fabs(matM[0][2]);
    rb = box.fA0*fabs(matM[1][1]) + box.fA1*fabs(matM[1][0]);
    t = fabs( T[0]*matM[2][2] - T[2]*matM[0][2] );
    if( t > ra + rb ) return false;

    // axis A2xB0
    ra = this->fA0*fabs(matM[1][0]) + this->fA1*fabs(matM[0][0]);
    rb = box.fA1*fabs(matM[2][2]) + box.fA2*fabs(matM[2][1]);
    t = fabs( T[1]*matM[0][0] - T[0]*matM[1][0] );
    if( t > ra + rb ) return false;

    // axis A2xB1
    ra = this->fA0*fabs(matM[1][1]) + this->fA1*fabs(matM[0][1]);
    rb = box.fA0 *fabs(matM[2][2]) + box.fA2*fabs(matM[2][0]);
    t = fabs( T[1]*matM[0][1] - T[0]*matM[1][1] );
    if( t > ra + rb ) return false;

    // axis A2xB2
    ra = this->fA0*fabs(matM[1][2]) + this->fA1*fabs(matM[0][2]);
    rb = box.fA0*fabs(matM[2][1]) + box.fA1*fabs(matM[2][0]);
    t = fabs( T[1]*matM[0][2] - T[0]*matM[1][2] );
    if( t > ra + rb ) return false;

    // no separation axis found => intersection
    return true;
}

/** Check whether the OBB is culled by the specified clip planes.
*/
int F3DOBB::Cull( const F3DPlane* planes, int numPlanes ) const
{
    F3DVector3 vN;
    int nResult = F3D_MATH_VISIBLE;
    float fRadius,fTest;

    // for all planes
    for( int i=0;i<numPlanes;i++ )
    {
        // frustrum normals pointing outwards, we need inwards
        vN = planes[i].m_vN * -1.0f;

        // calculate projected box radius
        fRadius = fabs(fA0 * (vN * vA0)) +
                  fabs(fA1 * (vN * vA1)) +
                  fabs(fA2 * (vN * vA2));

        // testvalue: (N*C - d) (#)
        fTest = vN * this->vCenter - planes[i].m_fD;

        // obb totally outside of at least one plane: (#) < -r
        if( fTest < -fRadius )
            return F3D_MATH_CULLED;

        // or maybe intersecting this plane?
        else if( !(fTest > fRadius) )
            nResult = F3D_MATH_CLIPPED;
    }

    // if not culled then clipped or inside
    return nResult;
}