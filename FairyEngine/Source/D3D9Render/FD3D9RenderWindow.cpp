/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9RenderWindow.cpp
 *  Desc:   本文件为D3D9渲染模块实现了一个渲染窗口对象。
 *  Author: Yish
 *  Date:   2011/7/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FD3D9RenderDevice.h"
#include "FD3D9RenderWindow.h"
#include "FD3D9Util.h"


/** 检测指定的深度/模板缓存是否可用
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

/** 构造函数
@Param 渲染窗口的名字
@Param 当创建的是副窗口时，需传此参数，否则为NULL
*/
FD3D9RenderWindow::FD3D9RenderWindow( const AString& name,bool bMainWnd,FD3D9RenderDevice* pDevice )
    : F3DRenderWindow(name,bMainWnd),m_pDevice(pDevice),m_hWnd(NULL),m_pChain(NULL),
    m_pBackBuffer(NULL),m_pZStencil(NULL),m_dwStyle(0)
{
    m_pd3dDevice = m_pDevice->_GetD3DDevicePtr();

    memset( &m_d3dpp,0,sizeof(D3DPRESENT_PARAMETERS) );
}

/** 析构函数
*/
FD3D9RenderWindow::~FD3D9RenderWindow(void)
{
    SAFE_RELEASE( m_pZStencil );
    SAFE_RELEASE( m_pBackBuffer );
    SAFE_RELEASE( m_pChain );
}

/** 创建该渲染窗口
@Param 要创建窗口的参数
*/
void FD3D9RenderWindow::Create( const SWindowInfo& info )
{
    HRESULT hr;

    // 保存渲染窗口参数
    m_hWnd = (HWND)info.wndHandle;
    m_nWidth = info.wndWidth;
    m_nHeight = info.wndHeight;
    m_bFullScreen = info.bFullScreen;

    // 保存当前窗口的风格
    m_dwStyle = GetWindowLong( m_hWnd,GWL_STYLE );

    // 获取设备配置
    const SDeviceSettings& settings = m_pDevice->_GetDeviceSettings();

    // 获取Direct3D指针
    LPDIRECT3D9 pD3D = m_pDevice->_GetD3DPtr();

    // 构造D3D设备参数
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
        // 尝试创建一个24位深度8位模板缓存
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

    // 创建D3D对象
    if( m_bMainWnd )
    {
        if( !m_pd3dDevice )
        {
            // 尝试创建设备(硬件顶点变换和光照)
            hr = pD3D->CreateDevice( settings.nAdapterID,D3DDEVTYPE_HAL,m_hWnd,
                D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice );

            if( FAILED(hr) )
            {
                // 尝试创建设备(软硬件混合)
                hr = pD3D->CreateDevice( settings.nAdapterID,D3DDEVTYPE_HAL,m_hWnd,
                    D3DCREATE_MIXED_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice );

                // 尝试创建设备(软件顶点变换和光照)
                if( FAILED(hr) )
                {
                    hr = pD3D->CreateDevice( settings.nAdapterID,D3DDEVTYPE_HAL,m_hWnd,
                        D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3dDevice );

                    // 尝试创建软设备
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

            // 获取设备的后台缓冲区
            m_pd3dDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer );

            // 获取设备的深度/模板缓冲区
            m_pd3dDevice->GetDepthStencilSurface( &m_pZStencil );
        }
    }
    else
    {
        // 创建副窗口的情况
        hr = m_pd3dDevice->CreateAdditionalSwapChain( &m_d3dpp,&m_pChain );
        if( FAILED(hr) )
        {
            // 再尝试创建一次
            hr = m_pd3dDevice->CreateAdditionalSwapChain( &m_d3dpp,&m_pChain );
            if( FAILED(hr) )
                F_EXCEPTION("FD3D9RenderWindow::Create", "Create the SwapChain for secondary window failed!");
        }

        // 获取渲染窗口的后台缓冲区
        m_pChain->GetBackBuffer( 0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer );

        // SwapChain需要自己的深度/模板缓冲区，以支持副窗口的resize
        hr = m_pd3dDevice->CreateDepthStencilSurface( m_nWidth,m_nHeight,m_d3dpp.AutoDepthStencilFormat,
            settings.msType,settings.dwQuality,TRUE,&m_pZStencil,NULL );
        if( FAILED(hr) )
        {
            F_EXCEPTION("FD3D9RenderWindow::Create", "Create the depth/stencil buffer for secondary window failed!");
        }
    }
}

/** 重新设置渲染窗口的大小
@Param 窗口的新宽度
@Param 窗口的新高度
*/
void FD3D9RenderWindow::ResizeWindow( uint32 width,uint32 height )
{
    HRESULT hr;

    // 获取渲染设备的配置
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

        // 重新创建SwapChain
        hr = m_pd3dDevice->CreateAdditionalSwapChain( &m_d3dpp,&m_pChain );
        if( FAILED(hr) )
            F_EXCEPTION("FD3D9RenderWindow::ResizeWindow", "Recreate the SwapChain failed!");

        // 获取SwapChain的后台缓冲区
        hr = m_pChain->GetBackBuffer( 0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer );
        if( FAILED(hr) )
            F_EXCEPTION("FD3D9RenderWindow::ResizeWindow", "Get the backbuffer of the secondary window failed!");

        // 重新创建深度/模板缓冲区
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

    // 更新隶属于该窗口的视口的大小
    ViewportList::iterator it;
    for( it=m_Viewports.begin();it!=m_Viewports.end();it++ )
        (*it)->_UpdateViewport();
}

/** 更换渲染窗口的全屏模式
*/
bool FD3D9RenderWindow::SetFullScreen( bool bFullScreen )
{
    if( !m_bMainWnd )
    {
        FLOG_WARNING( "FD3D9RenderWindow::SetFullScreen(), Only the main window can be in fullscreen!" );
        return false;
    }

    // 获取渲染设备的配置
    const SDeviceSettings& settings = m_pDevice->_GetDeviceSettings();

    if( m_bFullScreen != bFullScreen )
    {
        m_bFullScreen = bFullScreen;

        if( !m_bFullScreen )
        {
            // 切换为全屏模式
            DWORD dwStyle = WS_VISIBLE|WS_CLIPCHILDREN|WS_POPUP;
            SetWindowPos( m_hWnd,HWND_TOPMOST,0,0,m_nWidth,m_nHeight,SWP_NOACTIVATE );
            SetWindowLong( m_hWnd,GWL_STYLE,dwStyle );

            // 切换为全屏模式
            m_d3dpp.Windowed = FALSE;

            // 使设备丢失
            m_pDevice->_OnDeviceLost();
        }
        else
        {
            // 恢复窗口的风格
            SetWindowLong( m_hWnd,GWL_STYLE,m_dwStyle );

            // 切换为窗口模式
            m_d3dpp.Windowed = TRUE;

            // 使设备丢失
            m_pDevice->_OnDeviceLost();

            RECT rcWnd;
            SetRect( &rcWnd,0,0,m_nWidth,m_nHeight );
            AdjustWindowRect( &rcWnd,m_dwStyle,FALSE );

            // 使窗口居中
            int w = rcWnd.right - rcWnd.left;
            int h = rcWnd.bottom - rcWnd.top;
            int screenx = GetSystemMetrics( SM_CXSCREEN );
            int screeny = GetSystemMetrics( SM_CYSCREEN );
            if( w > screenx ) w = screenx;
            if( h > screeny ) h = screeny;
            int x = (screenx - w) / 2;
            int y = (screeny - h) / 2;

            // 设置窗口原先的位置
            SetWindowPos( m_hWnd,HWND_NOTOPMOST,x,y,w,h,SWP_SHOWWINDOW );
        }
    }

    return true;
}

/** 更新渲染窗口
*/
void FD3D9RenderWindow::Update(void)
{
    HRESULT hr;

    // 如果渲染设备丢失，则重置设备
    if( m_pDevice->IsDeviceLost() )
    {
        hr = m_pd3dDevice->TestCooperativeLevel();

        // 暂时不能恢复设备，则暂停线程100毫秒
        if( hr == D3DERR_DEVICELOST )
        {
            Sleep( 100 );
            return;
        }

        // 恢复渲染设备
        if( hr == D3DERR_DEVICENOTRESET )
            m_pDevice->_RestoreLostDevice();
    }

    F3DRenderWindow::Update();
}

/** 交换前后缓冲区
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

/** 截取当前屏幕保存到文件
@Param 要保存的文件名
@Param 是否从前台缓冲区截取
*/
bool FD3D9RenderWindow::PrintScreenToFile( const FString& filename,bool bFront )
{
    HRESULT hr;

    if( bFront )
    {
        // 从前台缓冲区截取画面
        D3DDISPLAYMODE mode;
        IDirect3DSurface9* pSurface;

        // 获取当前的显示模式
        m_pd3dDevice->GetDisplayMode( 0,&mode );

        // 创建一个离屏缓存表面
        hr = m_pd3dDevice->CreateOffscreenPlainSurface( mode.Width,mode.Height,D3DFMT_X8R8G8B8,
            D3DPOOL_SYSTEMMEM,&pSurface,NULL );
        if( FAILED(hr) )
        {
            F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Create the off screen plain surface failed!");
            return false;
        }

        // 获取前台缓冲区的数据
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

        // 如果是全屏模式则将整个前台缓冲区保存，
        // 窗口模式则仅保存窗口客户区域内的像素。
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

            // 获取窗口客户区的坐标
            FD3D9Util::GetClientRectEx( m_hWnd,rcClient );

            // 保存客户区域内的像素数据
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

        // 获取后台缓冲区的描述
        m_pBackBuffer->GetDesc( &desc );

        // 创建一个离屏缓存
        hr = m_pd3dDevice->CreateOffscreenPlainSurface( desc.Width,desc.Height,D3DFMT_X8R8G8B8,
            D3DPOOL_SYSTEMMEM,&pSurface,NULL );
        if( FAILED(hr) )
        {
            F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Create the off screen plain surface failed!");
            return false;
        }

        // 读取后台缓冲区数据
        hr = m_pd3dDevice->GetRenderTargetData( m_pBackBuffer,pSurface );
        if( FAILED(hr) )
        {
            SAFE_RELEASE( pSurface );
            F_EXCEPTION("FD3D9RenderWindow::PrintScreenToFile", "Read the back buffer data failed!");
            return false;
        }

        // 保存帧数据到文件
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