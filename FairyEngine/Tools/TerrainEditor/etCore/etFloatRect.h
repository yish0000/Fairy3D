//------------------------------------------------------------------------
// Name: etFloatRect.h
// Desc: This file define a bounding rectangle for the tile.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_FLOATRECT_H__
#define __ET_FLOATRECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etFloatRect
{
protected:
    float m_fMinX;      // X- coordinate of the min point.
    float m_fMinZ;      // Z- coordinate of the min point.
    float m_fMaxX;      // X- coordinate of the max point.
    float m_fMaxZ;      // Z- coordinate of the max point.

public:
    // Constructor and destructor.
    etFloatRect(void);
    ~etFloatRect(void);

    // Constructor to create a specified rect.
    etFloatRect( float fMinX,float fMinZ,float fMaxX,float fMaxZ );

    bool IsIntersected( const etFloatRect& rect ) const;

    float GetMinPointX(void) const { return m_fMinX; }
    float GetMinPointZ(void) const { return m_fMinZ; }
    float GetMaxPointX(void) const { return m_fMaxX; }
    float GetMaxPointZ(void) const { return m_fMaxZ; }

    void SetMinPoint( float fX,float fZ );
    void SetMaxPoint( float fX,float fZ );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_FLOATRECT_H__