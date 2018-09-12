/*
 * ------------------------------------------------------------------------
 *  Name:   F3DSphere.cpp
 *  Desc:   This file define a sphere class for engine.
 *  Author: Yish
 *  Date:   2010/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"

#include "F3DVector.h"
#include "F3DPlane.h"
#include "F3DAABB.h"
#include "F3DSphere.h"

/** Check whether the sphere contain the specified AABB.
*/
int F3DSphere::ContainBox( const F3DAABB& box ) const
{
    float sradius = m_fRadius * m_fRadius;

    F3DVector3 vMinDist = box.vMin - m_vCenter;
    F3DVector3 vMaxDist = box.vMax - m_vCenter;

    if( vMinDist.GetSqrLength() < sradius &&
        vMaxDist.GetSqrLength() < sradius )
    {
        return F3D_MATH_INSIDE;
    }

    float s, d = 0;

    if( m_vCenter.x < box.vMin.x )
    {
        s = m_vCenter.x - box.vMin.x;
        d += s * s;
    }
    else if( m_vCenter.x > box.vMax.x )
    {
        s = m_vCenter.x - box.vMax.x;
        d += s * s;
    }

    if( m_vCenter.y < box.vMin.y )
    {
        s = m_vCenter.y - box.vMin.y;
        d += s * s;
    }
    else if( m_vCenter.y > box.vMax.y )
    {
        s = m_vCenter.y - box.vMax.y;
        d += s * s;
    }

    if( m_vCenter.z < box.vMin.z )
    {
        s = m_vCenter.z - box.vMin.z;
        d += s * s;
    }
    else if( m_vCenter.z > box.vMax.z )
    {
        s = m_vCenter.z - box.vMax.z;
        d += s * s;
    }

    return (d > sradius) ? F3D_MATH_OUTSIDE : F3D_MATH_INTERSECT;
}

/** Check whether the point intersect with me.
*/
bool F3DSphere::Intersect( const F3DVector3& point ) const
{
    F3DVector3 vDist = m_vCenter - point;
    return (vDist.GetLength() <= m_fRadius);
}

/** Check whether the specified plane intersect with me.
*/
bool F3DSphere::Intersect( const F3DPlane& plane ) const
{
    return (((F3DPlane)plane).Distance(m_vCenter) <= m_fRadius);
}

/** Check whether the specified AABB intersect with me.
*/
bool F3DSphere::Intersect( const F3DAABB& box ) const
{
    float s, d = 0.0f;
    F3DVector3 vMin = box.vMin;
    F3DVector3 vMax = box.vMax;

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

    return (d <= m_fRadius * m_fRadius);
}

/** Check whether the specified sphere intersect with me.
*/
bool F3DSphere::Intersect( const F3DSphere& sphere ) const
{
    F3DVector3 vDist = m_vCenter - sphere.m_vCenter;
    return (vDist.GetLength() <= m_fRadius);
}