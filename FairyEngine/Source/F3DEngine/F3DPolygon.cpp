/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPolygon.cpp
 *  Desc:   This file define a polygon class for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DPlane.h"
#include "F3DRay.h"
#include "F3DAABB.h"
#include "F3DPolygon.h"

/** Default constructor
*/
F3DPolygon::F3DPolygon(void) : m_pPoints(NULL), m_pIndis(NULL),
    m_nNumPoints(0), m_nNumIndices(0), m_nFlag(0)
{
    m_Plane.Set( F3DVector3::UNIT_Y, 0.0f );
    m_AABB = F3DAABB::BOX_EMPTY;
}

/** Copy constructor.
*/
F3DPolygon::F3DPolygon( const F3DPolygon& poly ) : m_nNumPoints(poly.m_nNumPoints), m_nNumIndices(poly.m_nNumIndices),
    m_Plane(poly.m_Plane), m_AABB(poly.m_AABB), m_nFlag(poly.m_nFlag)
{
    m_pPoints = new F3DVector3[m_nNumPoints];
    memcpy( m_pPoints, poly.m_pPoints, sizeof(F3DVector3)*m_nNumPoints );
    m_pIndis = new uint32[m_nNumIndices];
    memcpy( m_pIndis, poly.m_pIndis, sizeof(uint32)*m_nNumIndices );
}

/** Constructor with some vertices.
*/
F3DPolygon::F3DPolygon(const F3DVector3* points, uint32 iNumPoints)
{
	m_nFlag = 0;
	Set(points, iNumPoints);
}

/** Constructor with some vertices and indices.
*/
F3DPolygon::F3DPolygon(const F3DVector3* points, uint32 iNumPoints, const uint32* indices, uint32 iNumIndices)
{
	m_nFlag = 0;
	Set(points, iNumPoints, indices, iNumIndices);
}

/** Destructor.
*/
F3DPolygon::~F3DPolygon(void)
{
    F_SAFE_DELETE_ARRAY( m_pPoints );
    F_SAFE_DELETE_ARRAY( m_pIndis );
}

/** Operator =
*/
F3DPolygon& F3DPolygon::operator = ( const F3DPolygon& poly )
{
    // clear the old data
    F_SAFE_DELETE_ARRAY( m_pPoints );
    F_SAFE_DELETE_ARRAY( m_pIndis );

    m_nNumPoints = poly.m_nNumPoints;
    m_pPoints = new F3DVector3[m_nNumPoints];
    memcpy( m_pPoints, poly.m_pPoints, sizeof(F3DVector3)*m_nNumPoints );
    m_nNumIndices = poly.m_nNumIndices;
    m_pIndis = new uint32[m_nNumIndices];
    memcpy( m_pIndis, poly.m_pIndis, sizeof(uint32)*m_nNumIndices );
    m_nFlag = poly.m_nFlag;
    m_Plane = poly.m_Plane;
    m_AABB = poly.m_AABB;
    return *this;
}

/** Set the polygon data.
*/
void F3DPolygon::Set( const F3DVector3* points, uint32 nNumPoints )
{
	uint32* pIdxBuf = (uint32*)F_MALLOC_TEMP(nNumPoints * sizeof(uint32));
	for(uint32 i=0; i<nNumPoints; i++) pIdxBuf[i] = i;
	Set(points, nNumPoints, pIdxBuf, nNumPoints);
	F_FREE_TEMP(pIdxBuf);
}

/** Set the polygon data.
*/
void F3DPolygon::Set( const F3DVector3* points, uint32 nNumPoints, const uint32* indis, uint32 nNumIndices )
{
	F3DVector3 vcEdge0;
	F3DVector3 vcEdge1;

	bool bGotEm = false;

    // clear the old data
    F_SAFE_DELETE_ARRAY( m_pPoints );
    F_SAFE_DELETE_ARRAY( m_pIndis );

    m_nNumPoints = nNumPoints;
    m_pPoints = new F3DVector3[nNumPoints];
    memcpy( m_pPoints, points, sizeof(F3DVector3)*nNumPoints );

    m_nNumIndices = nNumIndices;
	m_pIndis = new uint32[nNumIndices];
	memcpy( m_pIndis, indis, sizeof(uint32)*nNumIndices );

	vcEdge0 = m_pPoints[m_pIndis[1]] - m_pPoints[m_pIndis[0]];

	// calculate its plane
	for( uint32 i=2;bGotEm==false;i++ )
	{
		if( (i+1) > m_nNumIndices ) break;

		vcEdge1 = m_pPoints[m_pIndis[i]] - m_pPoints[m_pIndis[0]];

		vcEdge0.Normalize();
		vcEdge1.Normalize();

		// edges must not be parallel
		if( vcEdge0.AngleWith(vcEdge1) != 0.0 )        
			bGotEm = true;
	}

	m_Plane.m_vN.Cross( vcEdge0,vcEdge1 );
	m_Plane.m_vN.Normalize();
	m_Plane.m_fD = -(m_Plane.m_vN * m_pPoints[0]);

	CalcBoundingBox();
}

/** Calculate the bounding box.
*/
void F3DPolygon::CalcBoundingBox(void)
{
	F3DVector3 vMax,vMin;

	// pick random start value from poly
	vMax = vMin = m_pPoints[0];

	// seek final values for each coordinate
	for( uint32 i=0;i<m_nNumPoints;i++ )
	{
		if( m_pPoints[i].x > vMax.x ) 
			vMax.x = m_pPoints[i].x;
		else if( m_pPoints[i].x < vMin.x ) 
			vMin.x = m_pPoints[i].x;

		if( m_pPoints[i].y > vMax.y ) 
			vMax.y = m_pPoints[i].y;
		else if( m_pPoints[i].y < vMin.y ) 
			vMin.y = m_pPoints[i].y;

		if( m_pPoints[i].z > vMax.z ) 
			vMax.z = m_pPoints[i].z;
		else if( m_pPoints[i].z < vMin.z ) 
			vMin.z = m_pPoints[i].z;
	}

	m_AABB.vMax = vMax;
	m_AABB.vMin = vMin;
}

/** Swap the face of polygon.
*/
void F3DPolygon::SwapFaces(void)
{
	uint32* pIndis = new uint32[m_nNumIndices];

	// Change the order of index.
	for(uint32 i=0; i<m_nNumIndices; i++)
		pIndis[m_nNumIndices-i-1] = m_pIndis[i];

	// Change the normal's direction.
	m_Plane.m_vN *= -1.0f;
	m_Plane.m_fD *= -1.0f;

	delete[] m_pIndis;
	m_pIndis = pIndis;
}

/** Clip me to tow new polygons.
*/
void F3DPolygon::Clip( const F3DPlane& plane, F3DPolygon* pFront, F3DPolygon* pBack ) const
{
    if( !pFront || !pBack ) return;

    F3DVector3 vHit,vA,vB;
    F3DRay ray;

    // cast away const
    F3DPlane *pPlane = ((F3DPlane*)&plane);

    uint32 nNumFront = 0,     // Vertex count of front polygon.
           nNumBack = 0,      // Vertex count of back polygon.
           nLoop = 0,
           nCurrent = 0;

    F3DVector3* pvcFront = (F3DVector3*)F_MALLOC_TEMP(m_nNumPoints*3*sizeof(F3DVector3));
    F3DVector3* pvcBack = (F3DVector3*)F_MALLOC_TEMP(m_nNumPoints*3*sizeof(F3DVector3));

    // Classify the first point.
    switch( plane.Classify(m_pPoints[0]) )
    {
    case F3D_MATH_FRONT:
        pvcFront[nNumFront++] = m_pPoints[0];
        break;
    case F3D_MATH_BACK:
        pvcBack[nNumBack++] = m_pPoints[0];
        break;
    case F3D_MATH_PLANAR:
        pvcBack[nNumBack++] = m_pPoints[0];
        pvcFront[nNumFront++] = m_pPoints[0];
        break;
    default:
        break;
    }

    // Loop through all the left points.
    for( nLoop=1;nLoop<m_nNumPoints+1;nLoop++ )
    {
        if (nLoop == m_nNumPoints) nCurrent = 0;
        else nCurrent = nLoop;

        // take two neighboring points from the poly
        vA = m_pPoints[nLoop-1];
        vB = m_pPoints[nCurrent];

        // classify points with respect to the plane
        int nClass  = pPlane->Classify( vB );
        int nClassA = pPlane->Classify( vA );

        // if planar then put him to both sides
        if( nClass == F3D_MATH_PLANAR )
        {
            pvcBack[nNumBack++]   = m_pPoints[nCurrent];
            pvcFront[nNumFront++] = m_pPoints[nCurrent];
        }

        // else check if this edge intersects the plane
        else
        {
            ray.m_vOrig = vA;
            ray.m_vDir  = vB - vA;

            float fLength = ray.m_vDir.GetLength();

            if( fLength != 0.0f ) ray.m_vDir /= fLength;

            if( ray.Intersect(plane,false,fLength,0,&vHit) && (nClassA != F3D_MATH_PLANAR) )
            {
                // put the intersection point as new point for both
                pvcBack[nNumBack++]   = vHit;			
                pvcFront[nNumFront++] = vHit;

                // then sort the current point
                if( nClass == F3D_MATH_FRONT )
                {
                    if( nCurrent != 0 )
                        pvcFront[nNumFront++] = m_pPoints[nCurrent];
                }
                else if( nClass == F3D_MATH_BACK )
                {
                    if( nCurrent != 0 )
                        pvcBack[nNumBack++] = m_pPoints[nCurrent];
                }
            }
            // if no intersection then just sort this point
            else
            {
                if( nCurrent == 0 ) continue;

                if( nClass == F3D_MATH_FRONT )
                {
                    pvcFront[nNumFront++] = m_pPoints[nCurrent];
                }
                else if( nClass == F3D_MATH_BACK )
                {
                    pvcBack[nNumBack++] = m_pPoints[nCurrent];
                }
                else
                {
                    return; // should not happen
                }
            }
        }
    }

    /* now we have the vertices for both new polys ready.
      lets take care of the index lists now              */
   
    unsigned int I0, I1, I2;

    unsigned int* pnFront = NULL;
    unsigned int* pnBack  = NULL;
   
    if( nNumFront > 2 )
    {
        pnFront = (uint32*)F_MALLOC_TEMP((nNumFront-2)*3*sizeof(uint32));

        for( nLoop=0;nLoop<(nNumFront-2);nLoop++ )
        {
            if( nLoop==0 )
            {
                I0 = 0;
                I1 = 1;
                I2 = 2;
            }
            else
            {
                I1 = I2;
                I2++;
            }

            pnFront[(nLoop*3)   ] = I0;
            pnFront[(nLoop*3) +1] = I1;
            pnFront[(nLoop*3) +2] = I2;
        }
    }

    if( nNumBack > 2 )
    {
        pnBack = (uint32*)F_MALLOC_TEMP((nNumBack-2)*3*sizeof(uint32));

        for( nLoop=0;nLoop<(nNumBack-2);nLoop++ )
        {
            if( nLoop == 0 )
            {
                I0 = 0;
                I1 = 1;
                I2 = 2;
            }
            else
            {
                I1 = I2;
                I2++;
            }

            pnBack[(nLoop*3)   ] = I0;
            pnBack[(nLoop*3) +1] = I1;
            pnBack[(nLoop*3) +2] = I2;
        }
    }

    // now fuel in the information to the new poly objects
    if( pFront && pnFront )
    {
        pFront->Set( pvcFront,nNumFront,pnFront,(nNumFront-2)*3 );

        // make sure new one has same orientation as original
        if( pFront->GetPlane().m_vN * m_Plane.m_vN < 0.0f )
            pFront->SwapFaces();
    }

    if( pBack && pnBack )
    {
        pBack->Set( pvcBack,nNumBack,pnBack,(nNumBack-2)*3 );

        // make sure new one has same orientation as original
        if( pBack->GetPlane().m_vN * m_Plane.m_vN < 0.0f )
            pBack->SwapFaces();
    }

    // free temporary memory
    if( pvcFront ) { F_FREE_TEMP(pvcFront); }
    if( pvcBack )  { F_FREE_TEMP(pvcBack); }
    if( pnFront )  { F_FREE_TEMP(pnFront); }
    if( pnBack )   { F_FREE_TEMP(pnBack); }
}

/** Clip the polygon, make it be in the AABB.
*/
void F3DPolygon::Clip( const F3DAABB& box )
{
    F3DPolygon BackPoly,ClippedPoly;
    F3DPlane Planes[6];
    bool bClipped=false;

    // get the planes of the aabb, note that
    // the normals are pointing outwards
    box.GetPlanes( Planes );

    // make a copy of this polygon
    ClippedPoly = *this;

    // now clip the poly against the planes 
    for( int i=0;i<6;i++ )
    {
        if( Planes[i].Classify(ClippedPoly) == F3D_MATH_CLIPPED )
        {
            ClippedPoly.Clip( Planes[i],NULL,&BackPoly );
            ClippedPoly = BackPoly;
            bClipped = true;
        }
    }

    // set this poly to the final clip output
    if( bClipped )
    {
        *this = ClippedPoly;
    }
}

/** Is me culled by the specified AABB ?
*/
int F3DPolygon::Cull( const F3DAABB& box ) const
{
    F3DPlane Planes[6];
    int      nClass=0;
    uint32   nInside=0, nCurrent=0;
    bool     bFirst=true;
    F3DRay   Ray;

    // get the planes of the aabb, note that
    // the normals are pointing outwards
    box.GetPlanes( Planes );

    // check if polys aabb is roughly intersecting at all
    if( !m_AABB.Intersect(box) )
        return F3D_MATH_CULLED; // no way

    // now polygon is eventually but not
    // necessarily intersecting the aabb
    for( int p=0;p<6;p++ )
    {
        // one time check if all points are inside the aabb
        if( bFirst )
        {
            for( uint32 i=0;i<m_nNumPoints;i++ )
            {
                if( box.Contain(m_pPoints[i]) )
                    nInside++;
            }

            bFirst = false;

            // all points inside => poly totally inside aabb
            if( nInside == m_nNumPoints ) return F3D_MATH_VISIBLE;
        }

        // check for intersection of poly with aabb plane
        for( uint32 nLoop=1;nLoop<(m_nNumPoints+1);nLoop++ )
        {
            if( nLoop == m_nNumPoints )
                nCurrent = 0;
            else
                nCurrent = nLoop;

            // build ray from two neighboring points
            Ray.m_vOrig = m_pPoints[nLoop-1];
            Ray.m_vDir  = m_pPoints[nCurrent] - m_pPoints[nLoop-1];

            float fLength = Ray.m_vDir.GetLength();
            if( fLength != 0.0f ) Ray.m_vDir /= fLength;

            if( Ray.Intersect(Planes[p],false,fLength,0,NULL) )

/* NEED TO CHECK WHETHER INTERSECTION POINT IS INSIDE AABB AT ALL */

                return F3D_MATH_CLIPPED;
        }
    }

    // no intersection
    return F3D_MATH_CULLED;
}

/** Check whether the ray intersects with me.
*/
bool F3DPolygon::Intersect( const F3DRay& ray, bool bCull, float* t ) const
{
    for( uint32 i=0;i<m_nNumIndices;i+=3 )
    {
        if( ray.Intersect(m_pPoints[m_pIndis[i]],m_pPoints[m_pIndis[i+1]],
            m_pPoints[m_pIndis[i+2]],false,t) )
        {
            return true;
        }

        if( !bCull )
        {
            if( ray.Intersect(m_pPoints[m_pIndis[i+2]],m_pPoints[m_pIndis[i+1]],
                m_pPoints[m_pIndis[i]],false,t) )
            {
                return true;
            }
        }
    }

    return false;
}

/** Check whether the ray intersects with me.
*/
bool F3DPolygon::Intersect( const F3DRay& ray, bool bCull, float fL, float* t ) const
{
    for( uint32 i=0;i<m_nNumIndices;i+=3 )
    {
        if( ray.Intersect(m_pPoints[m_pIndis[i]],m_pPoints[m_pIndis[i+1]],
            m_pPoints[m_pIndis[i+2]],false,fL,t) )
        {
            return true;
        }

        if( !bCull )
        {
            if( ray.Intersect(m_pPoints[m_pIndis[i+2]],m_pPoints[m_pIndis[i+1]],
                m_pPoints[m_pIndis[i]],false,fL,t) )
            {
                return true;
            }
        }
    }

    return false;
}