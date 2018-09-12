//------------------------------------------------------------------------
// Name: FlyD3DAdapter.h
// Desc: This file define a class contain the info for an adapter.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DADAPTER_H__
#define __FLY_D3DADAPTER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "stl_headers.h"

#include "FlyD3DUtil.h"
//////////////////////////////////////////////////////////////////////////

class FlyD3DAdapter
{
    typedef std::vector<D3DDISPLAYMODE> DisplayModeList;
    typedef std::vector<D3DFORMAT> DepthStencilList;

protected:
    UINT m_nAdapter;                // ID of the adapter.
    D3DDISPLAYMODE m_DesktopMode;   // Desktop display mode.

    DisplayModeList m_DisplayModes; // Display mode list.
    DepthStencilList m_DepthStencils; // Depth/Stencil buffer format list.

    // Information of the adapter.
    D3DADAPTER_IDENTIFIER9 m_Identifier;

    bool ConfirmDepthFmt( IDirect3D9* pD3D,D3DFORMAT adapterFmt,D3DFORMAT backBufFmt,
        D3DFORMAT dsFmt );

public:
    // Constructor and destructor.
    FlyD3DAdapter( UINT nAdapterID );
    ~FlyD3DAdapter(void);

    // Enum the video display mode.
    HRESULT EnumVideoMode( IDirect3D9* pD3D,D3DFORMAT adapterFmt );

    // Enum the depth/stencil buffer mode.
    HRESULT EnumDepthStencilMode( IDirect3D9* pD3D,D3DFORMAT adapterFmt,
        D3DFORMAT backBufFmt );

    UINT GetAdapterID(void) const;
    const char* GetDescription(void) const;
    D3DDISPLAYMODE* GetDesktopMode(void);
    UINT GetVideoModeCount(void) const;
    D3DDISPLAYMODE* GetVideoMode( int nIndex );
    UINT GetDepthStencilFmtCount(void) const;
    D3DFORMAT GetDepthStencilFmt( int nIndex );

    void SetDesktopMode( const D3DDISPLAYMODE& mode );
    void SetAdapterIdentifier( const D3DADAPTER_IDENTIFIER9& identifier );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DADAPTER_H__