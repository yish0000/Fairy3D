/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPolygon.h
 *  Desc:   This file define a polygon class for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_POLYGON_H__
#define __F3D_POLYGON_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DMathCommon.h"

///////////////////////////////////////////////////////////////////////////

/** Polygon
*/
class FAIRY_API F3DPolygon
{
public:

    friend class F3DPlane;

protected:
    F3DPlane m_Plane;		// On the plane
    uint32 m_nNumPoints;	// Count of vertices.
    uint32 m_nNumIndices;	// Count of indices.
    F3DAABB m_AABB;			// Bounding box of the polygon.
    uint32 m_nFlag;			// Flag.

    F3DVector3* m_pPoints;	// Vertex list.
    uint32* m_pIndis;		// Index list.

public:

	// Constructors
    F3DPolygon();
    F3DPolygon(const F3DPolygon& poly);
	F3DPolygon(const F3DVector3* points, uint32 iNumPoints, const uint32* indices, uint32 iNumIndices);
	F3DPolygon(const F3DVector3* points, uint32 iNumPoints);
	// Destructors.
    ~F3DPolygon();

    // Operator =
    F3DPolygon& operator = ( const F3DPolygon& poly );

    // Set the values.
    void Set( const F3DVector3* points, uint32 nNumPoints, const uint32* indis, uint32 nNumIndices );
	void Set( const F3DVector3* points, uint32 nNumPoints );

	// Clip me to tow new polygons.
    void Clip( const F3DPlane& plane, F3DPolygon* pFront, F3DPolygon* pBack ) const;
	// Clip the polygon, make it be in the AABB.
    void Clip( const F3DAABB& box );

	// Is me culled by the specified AABB ?
    int Cull( const F3DAABB& box ) const;

	// Swap the face of polygon.
    void SwapFaces(void);

    // Check whether the ray intersects with me.
    bool Intersect( const F3DRay& ray, bool bCull, float* t ) const;
    bool Intersect( const F3DRay& ray, bool bCull, float fL, float* t ) const;

	// Get the count of points.
    uint32 GetNumPoints(void) const { return m_nNumPoints; }
	// Get the count of indices.
    uint32 GetNumIndis(void) const { return m_nNumIndices; }
	// Get the points.
    const F3DVector3* GetPoints(void) const { return m_pPoints; }
	// Get the indices.
    unsigned int* GetIndices(void) const { return m_pIndis;  }
	// Get the plane.
    const F3DPlane& GetPlane(void) const { return m_Plane; }
	// Get the bounding box.
    const F3DAABB& GetAABB(void) const { return m_AABB; }
	// Get the flag.
    uint32 GetFlag(void) const { return m_nFlag; }
	// Set the flag.
    void SetFlag( uint32 flag ) { m_nFlag = flag; }

protected:

	// Calculate the bounding box
	void CalcBoundingBox();
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_POLYGON_H__