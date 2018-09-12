//------------------------------------------------------------------------
// Name: etFloatRect.cpp
// Desc: This file is to implement the class etFloatRect.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etFloatRect.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etFloatRect::etFloatRect(void)
{
    m_fMinX = 0.0f;
    m_fMinZ = 0.0f;
    m_fMaxX = 0.0f;
    m_fMaxZ = 0.0f;
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etFloatRect::~etFloatRect(void)
{
}

//------------------------------------------------------------------------
// Constructor to create a specified rect.
//------------------------------------------------------------------------
etFloatRect::etFloatRect( float fMinX,float fMinZ,float fMaxX,float fMaxZ )
{
    m_fMinX = fMinX;
    m_fMinZ = fMinZ;
    m_fMaxX = fMaxX;
    m_fMaxZ = fMaxZ;
}


//------------------------------------------------------------------------
// Is the rect intersected by another rect ?
//------------------------------------------------------------------------
bool etFloatRect::IsIntersected( const etFloatRect& rect ) const
{
    float fW1 = _fabs( m_fMaxX-m_fMinX ) / 2.0f;
    float fH1 = _fabs( m_fMaxZ-m_fMinZ ) / 2.0f;
    float fW2 = _fabs( rect.m_fMaxX-rect.m_fMinX ) / 2.0f;
    float fH2 = _fabs( rect.m_fMaxZ-rect.m_fMinZ ) / 2.0f;

    float fCen1X = (m_fMinX+m_fMaxX) / 2.0f;
    float fCen1Z = (m_fMinZ+m_fMaxZ) / 2.0f;
    float fCen2X = (rect.m_fMinX+rect.m_fMaxX) / 2.0f;
    float fCen2Z = (rect.m_fMinZ+rect.m_fMaxZ) / 2.0f;

    if( _fabs(fCen1X-fCen2X) > fW1+fW2 ) return false;
    if( _fabs(fCen1Z-fCen2Z) > fH1+fH2 ) return false;

    return true;
}


//------------------------------------------------------------------------
// Set the minimum point of the rect.
//------------------------------------------------------------------------
void etFloatRect::SetMinPoint( float fX,float fZ )
{
    m_fMinX = fX;
    m_fMinZ = fZ;
}


//------------------------------------------------------------------------
// Set the maximum point of the rect.
//------------------------------------------------------------------------
void etFloatRect::SetMaxPoint( float fX,float fZ )
{
    m_fMaxX = fX;
    m_fMaxZ = fZ;
}