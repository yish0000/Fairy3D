/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderTarget.cpp
 *  Desc:   This file implements the base class of renderTarget.
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DRenderTarget.h"


/** Constructor.
*/
F3DRenderTarget::F3DRenderTarget( const AString& name )
    : m_sName(name), m_nWidth(0), m_nHeight(0)
{
}

/** Destructor.
*/
F3DRenderTarget::~F3DRenderTarget(void)
{
}

/** Add a new viewport for this renderTarget.
@Param Camera of the viewport.
@Param Relative X value. (0.0 ~ 1.0)
@Param Relative Y value. (0.0 ~ 1.0)
@Param Relative Width of viewport. (0.0 ~ 1.0)
@Param Relative Height of viewport. (0.0 ~ 1.0)
@Param Minimum z value.
@Param Maximum z value.
*/
F3DViewport* F3DRenderTarget::AddViewport(F3DCameraBase* camera, float fX, float fY, float fWidth, float fHeight,
										  float fMinZ /* = 0.0f */, float fMaxZ /* = 1.0f */)
{
    // Create a new viewport.
    F3DViewport* pViewport = new F3DViewport(this, fX, fY, fWidth, fHeight, fMinZ, fMaxZ);
    if( !pViewport ) return NULL;
    pViewport->SetCamera(camera);

    m_Viewports.push_back(pViewport);
    return pViewport;
}

/** Add a new viewport for this renderTarget.
*/
F3DViewport* F3DRenderTarget::AddViewport(F3DCameraBase *camera, const SViewportDesc &vp)
{
	F3DViewport* pViewport = new F3DViewport(this, vp);
	if( !pViewport ) return NULL;
	pViewport->SetCamera(camera);

	m_Viewports.push_back(pViewport);
	return pViewport;
}

/** Remove the specified viewport.
*/
void F3DRenderTarget::RemoveViewport( size_t nIndex )
{
    if( nIndex < m_Viewports.size() )
    {
        F_SAFE_DELETE( m_Viewports[nIndex] );
        m_Viewports.erase( m_Viewports.begin() + nIndex );
    }
    else
    {
        FASSERT(0);
        FLOG_WARNING( "F3DRenderTarget::RemoveViewport(), Can't find the specified viewport!" );
    }
}

/** Get the specified viewport.
*/
F3DViewport* F3DRenderTarget::GetViewport( size_t nIndex )
{
    return nIndex < m_Viewports.size() ? m_Viewports[nIndex] : NULL;
}

/** Add a clip plane for renderTarget.
*/
void F3DRenderTarget::AddClipPlane( const F3DPlane& plane )
{
    m_ClipPlanes.push_back( plane );
}

/** Remove the specified clip plane.
*/
void F3DRenderTarget::RemoveClipPlane( size_t index )
{
    if( index < m_ClipPlanes.size() )
        m_ClipPlanes.erase( m_ClipPlanes.begin() + index );
    else
    {
        FASSERT(0);
        FLOG_WARNING( "F3DRenderTarget::RemoveClipPlane(), Can't find the specified clip plane!" );
    }
}

/** Get the specified clip plane.
*/
const F3DPlane& F3DRenderTarget::GetClipPlane( size_t index ) const
{
    FASSERT( index < m_ClipPlanes.size() );
    return m_ClipPlanes[index];
}