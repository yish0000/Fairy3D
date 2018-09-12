//------------------------------------------------------------------------
// Name: FlyD3DRenderWindow.h
// Desc: If you want more than one RenderTarget for the engine, you
//       can add the renderWindow to the RenderDevice.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DRENDERWINDOW_H__
#define __FLY_D3DRENDERWINDOW_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"

//////////////////////////////////////////////////////////////////////////


class FlyD3DRenderWindow
{
protected:
    HWND m_hWnd; // Handle to the window.
    LPDIRECT3DDEVICE9 m_pd3dDevice; // Pointer to the device.
    LPDIRECT3DSWAPCHAIN9 m_pChain;  // Swapchain pointer.
    D3DPRESENT_PARAMETERS m_d3dpp;  // Present parameters.

public:
    // Constructor and destructor.
    FlyD3DRenderWindow(void);
    ~FlyD3DRenderWindow(void);

    HRESULT Create( HWND hWnd,UINT nWidth,UINT nHeight,
                    D3DPRESENT_PARAMETERS* d3dpp );

    HRESULT ResizeWindow( UINT nWidth,UINT nHeight );
    HRESULT SwapBuffers(void);

    HRESULT GetBackBuffer( IDirect3DSurface9** ppSurface );
    HRESULT GetFrontBufferData( IDirect3DSurface9* pSurface );
    HRESULT GetDisplayMode( D3DDISPLAYMODE* pMode );

    HRESULT CreateD3DResources(void);
    void DestroyD3DResources(void);

    void SetD3DDevicePtr( LPDIRECT3DDEVICE9 pd3dDevice );
    HWND GetWindowHandle(void) { return m_hWnd; }

    UINT GetBackWidth(void);
    UINT GetBackHeight(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DRENDERWINDOW_H__