//------------------------------------------------------------------------
// Name: FlyPolygon.h
// Desc: This file define a polygon class for the Math library.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_POLYGON_H__
#define __FLY_POLYGON_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyMathCommon.h"
//////////////////////////////////////////////////////////////////////////


// Class for the polygon.
class _MATH_EXPORT FlyPolygon
{
	friend class FlyPlane;  // access for easier classifying

private:
    FlyPlane     m_Plane;   // plane which poly lies in
    int          m_NumP;    // number of points
    int          m_NumI;    // number of indices
    FlyAabb      m_Aabb;    // bounding box
    unsigned int m_Flag;    // whatever you want it to be

    // Calculate the BoundingBox
    void    CalcBoundingBox(void);

public:
    // Constructor and destructor
    FlyPolygon(void);
    ~FlyPolygon(void);

    FlyVector    *m_pPoints;  // list of points
    unsigned int *m_pIndis;   // index list

    void    Set(const FlyVector *pPoints, int nNumP,
				const unsigned int *pIndis, int nNumI);

    void    Clip(const FlyPlane &Plane, 
				 FlyPolygon *pFront,
				 FlyPolygon *pBack);
    void    Clip(const FlyAabb &aabb);
    int     Cull(const FlyAabb &aabb);

    void    CopyOf( const FlyPolygon& Poly );

    void    SwapFaces(void);

    bool    Intersects(const FlyRay &Ray, bool, float *t);
    bool    Intersects(const FlyRay &Ray, bool, float fL, float *t);

    int     GetNumPoints(void)      { return m_NumP;    }
    int     GetNumIndis(void)       { return m_NumI;    }
    FlyVector* GetPoints(void)      { return m_pPoints; }
    unsigned int* GetIndices(void)  { return m_pIndis;  }
    FlyPlane GetPlane(void)         { return m_Plane;   }
    FlyAabb  GetAabb(void)          { return m_Aabb;    }
    unsigned int  GetFlag(void)     { return m_Flag;    }
    void    SetFlag(unsigned int n) { m_Flag = n;       }

    // DEBUG MODE ONLY
    void	Print( FILE* );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_POLYGON_H__