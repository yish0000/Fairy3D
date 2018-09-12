//------------------------------------------------------------------------
// Name: FlySphere.cpp
// Desc: This file is to implement the class FlySphere.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyVector.h"
#include "FlyPlane.h"
#include "FlyAabb.h"
#include "FlySphere.h"


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlySphere::FlySphere(void)
{
    m_vCenter.Set( 0.0f,0.0f,0.0f );
    m_fRadius = 0.0f;
}


//------------------------------------------------------------------------
FlySphere::FlySphere( const FlyVector& vCenter,float fRadius )
{
    m_vCenter = vCenter;
    m_fRadius = fRadius;
}


//------------------------------------------------------------------------
void FlySphere::Set( FlyVector vCenter,float fRadius )
{
    m_vCenter = vCenter;
    m_fRadius = fRadius;
}


//------------------------------------------------------------------------
bool FlySphere::Intersects( const FlyVector& vPoint )
{
    FlyVector vDist = m_vCenter - vPoint;
    float fDist = vDist.GetLength();

    return (fDist <= m_fRadius);
}


//------------------------------------------------------------------------
bool FlySphere::Intersects( const FlyPlane& plane )
{
    float fDist = ((FlyPlane)plane).Distance( m_vCenter );

    return (fDist <= m_fRadius);
}


//------------------------------------------------------------------------
bool FlySphere::Intersects( const FlyAabb& aabb )
{
    float s, d = 0.0f;
    FlyVector vMin = aabb.vcMin;
    FlyVector vMax = aabb.vcMax;

    // This algorithm is from OGRE.
    if( m_vCenter.x < vMin.x )
    {
        s = m_vCenter.x - vMin.x;
        d += s * s;
    }
    else if( m_vCenter.x > vMax.x )
    {
        s = m_vCenter.x - vMax.x;
        d += s * s;
    }

    if( m_vCenter.y < vMin.y )
    {
        s = m_vCenter.y - vMin.y;
        d += s * s;
    }
    else if( m_vCenter.y > vMin.y )
    {
        s = m_vCenter.y - vMax.y;
        d += s * s;
    }

    if( m_vCenter.z < vMin.z )
    {
        s = m_vCenter.z - vMin.z;
        d += s * s;
    }
    else if( m_vCenter.z > vMax.z )
    {
        s = m_vCenter.z - vMax.z;
        d += s * s;
    }

    return (d <= m_fRadius*m_fRadius);
}


//------------------------------------------------------------------------
bool FlySphere::Intersects( const FlySphere& sphere )
{
    FlyVector vDist = m_vCenter - sphere.m_vCenter;
    float fDist = vDist.GetLength();

    return (fDist <= m_fRadius);
}