/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9RenderWindow.cpp
 *  Desc:   ���ļ�ΪD3D9��Ⱦģ��ʵ����һ����Ⱦ���ڶ���
 *  Author: Yish
 *  Date:   2011/7/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FD3D9RenderDevice.h"
#include "FD3D9RenderWindow.h"
#include "FD3D9Util.h"


/** ���ָ�������/ģ�建���Ƿ����
*/
static bool ConfirmDepthFmt( LPDIRECT3D9 pD3D,uint32 nAdapter,D3DFORMAT adapterFmt,
                             D3DFORMAT backBufFmt,D3DFORMAT dsFmt )
{
    if( FAILED(pD3D->CheckDeviceFormat(nAdapter,D3DDEVTYPE_HAL,adapterFmt,
        D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_SURFACE,dsFmt)) )
        return false;

    if( FAILED(pD3D->CheckDepthStencilMatch(nAdapter,D3DDEVTYPE_HAL,adapterFmt,
        backBufFmt,dsFmt)) )
        return false;

    return true;
}

/** ���캯��
@Param ��Ⱦ���ڵ�����
@Param ���������Ǹ�����ʱ���贫�˲���������ΪNULL
*/
FD3D9RenderWindow::FD3D9RenderWindow( const AString& name,bool bMainWnd,FD3D9RenderDevice* pDevice )
    : F3DRenderWindow(name,bMainWnd),m_pDevice(pDevice),m_hWnd(NULL),m_pChain(NULL),
    m_pBackBuffer(NULL),m_pZStencil(NULL),m_dwStyle(0)
{
    m_pd3dDevice = m_pDevice->_GetD3DDevicePtr();

    memset( &m_d3dpp,0,sizeof(D3DPRESENT_PARAMETERS) );
}

/** ��������
*/
FD3D9RenderWindow::~FD3D9RenderWindow(void)
{
    SAFE_RELEASE( m_pZStencil );
    SAFE_RELEASE( m_pBackBuffer );
    SAFE_RELEASE( m_pChain );
}

/** ��������Ⱦ����
@Param Ҫ�������ڵĲ���
*/
void FD3D9RenderWindow::Create( const SWindowInfo& info )
{
    HRESULT hr;

    // ������Ⱦ���ڲ���
    m_hWnd = (HWND)info.wndHandle;
    m_nWidth = info.wndWidth;
    m_nHeight = info.wndHeight;
    m_bFullScreen = info.bFullScreen;

    // ���浱ǰ���ڵķ��
    m_dwStyle = GetWindowLong( m_hWnd,GWL_STYLE );

    // ��ȡ�豸����
    const SDeviceSettings& settings = m_pDevice->_GetDeviceSettings();

    // ��ȡDirect3Dָ��
    LPDIRECT3D9 pD3D = m_pDevice->_GetD3DPtr();

    // ����D3D�豸����
    m_d3dpp.BackBufferCount = 1;
    m_d3dpp.BackBufferFormat = settings.bColor16Bit ? D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;
    m_d3dpp.BackBufferWidth = m_nHeight;
    m_d3dpp.BackBufferHeight = m_nWidth;
    m_d3dpp.hDeviceWindow = m_hWnd;
    m_d3dpp.Windowed = !m_bFullScreen;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.MultiSampleType = settings.msType;
    m_d3dpp.MultiSampleQuality = settings.dwQuality;
    m_d3dpp.PresentationInterval = settings.bVSync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_d3dpp.EnableAutoDepthStencil = TRUE;

    if( !settings.bColor16Bit )
    {
        // ���Դ���һ��24λ���8λģ�建��
        if( ConfirmDepthFmt(pD3D,settings.nAdapterID,D3DFMT_X8R8G8B8,
            D3DFMT_X8R8G8B8,D3DFMT_D24S8) )
        {
            m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
        }
        else
        {
            if( ConfirmDepthFmt(pD3D,settings.nAdapterID,D3DFMT_X8R8G8B8,D3DFMT_X8R8G8B8,
                D3DFMT_D24X4S4) )
                m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24X4S4;
            else
            {
                if( ConfirmDepthFmt(pD3D,settings.nAdapterID,D3DFMT_X8R8G8B8,
                    D3DFMT_X8R8G8B8,D3DFMT_D24X8) )
                    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
                else
                {
                    if( ConfirmDepthFmt(pD3D,settings.nAdapterID,D3DFMT_X8R8G8B8,
                        D3DFMT_X8R8G8B8,D3DFMT_D32) )
                        m_d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
                    else
                        m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
                }
            }
        }
    }
    else
    {
        m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    }

    // ����D3D����
    if( m_bMainWnd )
    {
        if( !m_pd3dDevice )
        {
            // ���Դ����豸(Ӳ������任�͹���)
            hr = pD3D->CreateDevice( settings.nAdapterID,D3DDEVTYPE_HAL,m_hWnd,
                D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice );

            if( FAILED(hr) )
            {
                // ���Դ����豸(��Ӳ�����)
                hr = pD3D->CreateDevice( settings.nAdapterID,D3DDEVTYPE_HAL,m_hWnd,
                    D3DCREATE_MIXED_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice );

                // ���Դ����豸(�������任�͹���)
                if( FAILED(hr) )
                {
                    hr = pD3D->CreateDevice( settings.nAdapterID,D3DDEVTYPE_HAL,m_hWnd,
                        D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice );

                    // ���Դ������豸
                    if( FAILED(hr) )
                    {
                        hr = pD3D->CreateDevice( settings.nAdapterID,D3DDEVTYPE_REF,m_hWnd,
                            D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice );
                        if( FAILED(hr) )
                            F_EXCEPTION("FD3D9RenderWindow::Create", "Create the direct3d device failed!");
                        else
                            FLOG_WARNING( "Software Device is create, it will be very slow!" );
                    }
                    else
                        FLOG_WARNING( "Current video card can't do the hardware vertex processing and lighting!" );
                }
            }

            // ��ȡ�豸�ĺ�̨������
            m_pd3dDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer );

            // ��ȡ�豸�����/ģ�建����
            m_pd3dDevice->GetDepthStencilSurface( &m_pZStencil );
        }
    }
    else
    {
        // ���������ڵ����
        hr = m_pd3dDevice->CreateAdditionalSwapChain( &m_d3dpp,&m_pChain );
        if( FAILED(hr) )
        {
            // �ٳ��Դ���һ��
            hr = m_pd3dDevice->CreateAdditionalSwapChain( &m_d3dpp,&m_pChain );
            if( FAILED(hr) )
                F_EXCEPTION("FD3D9RenderWindow::Create", "Create the SwapChain for secondary window failed!");
        }

        // ��ȡ��Ⱦ���ڵĺ�̨������
        m_pChain->GetBackBuffer( 0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer );

        // SwapChain��Ҫ�Լ������/ģ�建��������֧�ָ����ڵ�resize
        hr = m_pd3dDevice->CreateDepthStencilSurface( m_nWidth,m_nHeight,m_d3dpp.AutoDepthStencilFormat,
            settings.msType,settings.dwQuality,TRUE,&m_pZStencil,NULL );
        if( FAILED(hr) )
        {
            F_EXCEPTION("FD3D9RenderWindow::Create", "Create the depth/stencil buffer for secondary window failed!");
        }
    }
}

/** ����������Ⱦ���ڵĴ�С
@Param ���ڵ��¿��
@Param ���ڵ��¸߶�
*/
void FD3D9RenderWindow::ResizeWindow( uint32 width,uint32 height )
{
    HRESULT hr;

    // ��ȡ��Ⱦ�豸������
    const SDeviceSettings& settings = m_pDevice->_GetDeviceSettings();

    m_nWidth = width;
    m_nHeight = height;

    if( !m_bMainWnd )
    {
        SAFE_RELEASE( m_pZStencil );
        SAFE_RELEASE( m_pBackBuffer );
        SAFE_RELEASE( m_pChain );

        m_d3dpp.BackBufferWidth = width;
        m_d3dpp.BackBufferHeight = height;

        // ���´���SwapChain
        hr = m_pd3dDevice->CreateAdditionalSwapChain( &m_d3dpp,&m_pChain );
        if( FAILED(hr) )
            F_EXCEPTION("FD3D9RenderWindow::ResizeWindow", "Recreate the SwapChain failed!");

        // ��ȡSwapChain�ĺ�̨������
        hr = m_pChain->GetBackBuffer( 0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer );
        if( FAILED(hr) )
            F_EXCEPTION("FD3D9RenderWindow::ResizeWindow", "Get the backbuffer of the secondary window failed!");

        // ���´������/ģ�建����
        hr = m_pd3dDevice->CreateDepthStencilSurface( m_nWidth,m_nHeight,m_d3dpp.AutoDepthStencilFormat,
            settings.msType,settings.dwQuality,TRUE,&m_pZStencil,NULL );
        if( FAILED(hr) )
            F_EXCEPTION"FD3D9RenderWindow::ResizeWindow", "Create the secondary window's depth/stencil buffer failed!");
    }
    else
    {
        m_d3dpp.BackBufferWidth = m_nWidth;
        m_d3dpp.BackBufferHeight = m_nHeight;

        m_pDevice->_OnDeviceLost();
    }

    // ���������ڸô��ڵ��ӿڵĴ�С
    ViewportList::iterator it;
    for( it=m_Viewports.begin();it!=m_Viewports.end();it++ )
        (*it)->_UpdateViewport();
}

/** ������Ⱦ���ڵ�ȫ��ģʽ
*/
bool FD3D9RenderWindow::SetFullScreen( bool bFullScreen )
{
    if( !m_bMainWnd )
    {
        FLOG_WARNING( "FD3D9RenderWindow::SetFullScreen(), Only the main window can be in fullscreen!" );
        return false;
    }

    // ��ȡ��Ⱦ�豸������
    const SDeviceSettings& settings = m_pDevice->_GetDeviceSettings();

    if( m_bFullScreen != bFullScreen )
    {
        m_bFullScreen = bFullScreen;

        if( !m_bFullScreen )
        {
            // �л�Ϊȫ��ģʽ
            DWORD dwStyle = WS_VISIBLE|WS_CLIPCHILDREN|WS_POPUP;
            SetWindowPos( m_hWnd,HWND_TOPMOST,0,0,m_nWidth,m_nHeight,SWP_NOACTIVATE );
            SetWindowLong( m_hWnd,GWL_STYLE,dwStyle );

            // �л�Ϊȫ��ģʽ
            m_d3dpp.Windowed = FALSE;

            // ʹ�豸��ʧ
            m_pDevice->_OnDeviceLost();
        }
        else
        {
            // �ָ����ڵķ��
            SetWindowLong( m_hWnd,GWL_STYLE,m_dwStyle );

            // �л�Ϊ����ģʽ
            m_d3dpp.Windowed = TRUE;

            // ʹ�豸��ʧ
            m_pDevice->_OnDeviceLost();

            RECT rcWnd;
            SetRect( &rcWnd,0,0,m_nWidth,m_nHeight );
            AdjustWindowRect( &rcWnd,m_dwStyle,FALSE );

            // ʹ���ھ���
            int w = rcWnd.right - rcWnd.left;
            int h = rcWnd.bottom - rcWnd.top;
            int screenx = GetSystemMetrics( SM_CXSCREEN );
            int screeny = GetSystemMetrics( SM_CYSCREEN );
            if( w > screenx ) w = screenx;
            if( h > screeny ) h = screeny;
            int x = (screenx - w) / 2;
            int y = (screeny - h) / 2;

            // ���ô���ԭ�ȵ�λ��
            SetWindowPos( m_hWnd,HWND_NOTOPMOST,x,y,w,h,SWP_SHOWWINDOW );
        }
    }

    return true;
}

/** ������Ⱦ����
*/
void FD3D9RenderWindow::Update(void)
{
    HRESULT hr;

    // �����Ⱦ�豸��ʧ���������豸
    if( m_pDevice->IsDeviceLost() )
    {
        hr = m_pd3dDevice->TestCooperativeLevel();

        // ��ʱ���ָܻ��豸������ͣ�߳�100����
        if( hr == D3DERR_DEVICELOST )
        {
            Sleep( 100 );
            return;
        }

        // �ָ���Ⱦ�豸
        if( hr == D3DERR_DEVICENOTRESET )
            m_pDevice->_RestoreLostDevice();
    }

    F3DRenderWindow::Update();
}

/** ����ǰ�󻺳���
*/
void FD3D9RenderWindow::SwapBuffers(void)
{
    HRESULT hr;

    if( m_bMainWnd )
        hr = m_pd3dDevice->Present( NULL,NULL,NULL,NULL );
    else
        hr = m_pChain->Present( NULL,NULL,NULL,NULL,0 );

    if( hr == D3DERR_DEVICELOST )
        m_pDevice->_OnDeviceLost();
    else if( FAILED(hr) )
    {
        F_EXCEPTION("FD3D9RenderWindow::SwapBuffers", "Swap the buffer of device failed!");
    }
}

/** ��ȡ��ǰ��Ļ���浽�ļ�
@Param Ҫ������ļ���
@Param �Ƿ��ǰ̨��������ȡ
*/
bool FD3D9RenderWindow::PrintScreenToFile( const FString& filename,bool bFront )
{
    HRESULT hr;

    if( bFront )
    {
        // ��ǰ̨��������ȡ����
        D3DDISPLAYMODE mode;
        IDirect3DSurface9* pSurface;

        // ��ȡ��ǰ����ʾģʽ
        m_pd3dDevice->GetDisplayMode( 0,&mode );

        // ����һ�������������
        hr = m_pd3dDevice->CreateOffscreenPlainSurface( mode.Width,mode.Height,D3DFMT_X8R8G8B8,
            D3DPOOL_SYSTEMMEM,&pSurface,NULL );
        if( FAILED(hr) )
        {
            F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Create the off screen plain surface failed!");
            return false;
        }

        // ��ȡǰ̨������������
        if( m_bMainWnd )
            hr = m_pd3dDevice->GetFrontBufferData( 0,pSurface );
        else
            hr = m_pChain->GetFrontBufferData( pSurface );
        if( FAILED(hr) )
        {
            SAFE_RELEASE( pSurface );
            F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Get the front buffer data failed!");
            return false;
        }

        // �����ȫ��ģʽ������ǰ̨���������棬
        // ����ģʽ������洰�ڿͻ������ڵ����ء�
        if( m_bFullScreen )
        {
            if( FAILED(D3DXSaveSurfaceToFile(filename.c_str(),D3DXIFF_BMP,pSurface,
                NULL,NULL)) )
            {
                SAFE_RELEASE( pSurface );
                F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Save the surface data to file failed!");
                return false;
            }
        }
        else
        {
            RECT rcClient;

            // ��ȡ���ڿͻ���������
            FD3D9Util::GetClientRectEx( m_hWnd,rcClient );

            // ����ͻ������ڵ���������
            if( FAILED(D3DXSaveSurfaceToFile(filename.c_str(),D3DXIFF_BMP,pSurface,
                NULL,&rcClient)) )
            {
                SAFE_RELEASE( pSurface );
                F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Save the surface data to file failed!");
                return false;
            }
        }

        SAFE_RELEASE( pSurface );
    }
    else
    {
        D3DSURFACE_DESC desc;
        IDirect3DSurface9* pSurface;

        // ��ȡ��̨������������
        m_pBackBuffer->GetDesc( &desc );

        // ����һ����������
        hr = m_pd3dDevice->CreateOffscreenPlainSurface( desc.Width,desc.Height,D3DFMT_X8R8G8B8,
            D3DPOOL_SYSTEMMEM,&pSurface,NULL );
        if( FAILED(hr) )
        {
            F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Create the off screen plain surface failed!");
            return false;
        }

        // ��ȡ��̨����������
        hr = m_pd3dDevice->GetRenderTargetData( m_pBackBuffer,pSurface );
        if( FAILED(hr) )
        {
            SAFE_RELEASE( pSurface );
            F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Read the back buffer data failed!");
            return false;
        }

        // ����֡���ݵ��ļ�
        if( FAILED(D3DXSaveSurfaceToFile(filename.c_str(),D3DXIFF_BMP,pSurface,
            NULL,NULL)) )
        {
            SAFE_RELEASE( pSurface );
            F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Save the surface data to file failed!");
            return false;
        }

        SAFE_RELEASE( pSurface );
    }

    return true;
}