//------------------------------------------------------------------------
// Name: FlyRenderTarget.cpp
// Desc: This file is to implementing the class FlyRenderTarget.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "FlyRenderTarget.h"

#include "FlyKernel.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyRenderTarget::FlyRenderTarget(void)
{
    m_pTarget = NULL;
    m_pCamera = NULL;
    m_bActive = false;
    m_bShowSkybox = false;
    m_nWidth = 0;
    m_nHeight = 0;
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyRenderTarget::~FlyRenderTarget(void)
{
    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    if( m_pTarget ) pResMgr->DeleteTexture( m_pTarget );
}


//------------------------------------------------------------------------
// Add a clip plane for the target.
//------------------------------------------------------------------------
void FlyRenderTarget::AddClipPlane( const FlyPlane& plane )
{
    m_ClipPlanes.push_back( plane );
}

//------------------------------------------------------------------------
// Remove the specified clip plane.
//------------------------------------------------------------------------
void FlyRenderTarget::RemoveClipPlane( UINT nIndex )
{
    m_ClipPlanes.erase( m_ClipPlanes.begin() + nIndex );
}

//------------------------------------------------------------------------
// Remove all the clip planes.
//------------------------------------------------------------------------
void FlyRenderTarget::RemoveAllClipPlanes(void)
{
    m_ClipPlanes.clear();
}


//------------------------------------------------------------------------
// Build the render target for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the render target.
//      UINT:           Width of the target.
//      UINT:           Height of the target.
//------------------------------------------------------------------------
HRESULT FlyRenderTarget::BuildRenderTarget( const char* cName,UINT nWidth,UINT nHeight )
{
    HRESULT hr;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Clean the old target.
    if( m_pTarget ) pResMgr->DeleteTexture( m_pTarget );

    m_pTarget = pResMgr->AddNullTexture( cName );
    if( !m_pTarget ) return FLY_RESOURCEEXISTED;

    // Build the texture.
    hr = m_pTarget->Create2DTexture( nWidth,nHeight,TU_RENDERTARGET,PFT_R5G6B5,0 );
    if( FAILED(hr) )
    {
        pResMgr->DeleteTexture( m_pTarget );
        m_pTarget = NULL;
        return hr;
    }

    return FLY_OK;
}