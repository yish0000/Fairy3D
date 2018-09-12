/*
 * -----------------------------------------------------------------------
 *  Name:   FlyD3D9RenderDevice.cpp
 *  Desc:   本文件用于实现引擎的Direct3D9设备接口。
 *  Author: Yish
 *  Date:   2011/7/4
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FlyD3D9RenderDevice.h"
#include "FlyD3D9RenderWindow.h"
#include "FlyD3D9Util.h"


/** 构造函数
@Param D3D9设备驱动模块实例句柄
*/
FlyD3D9RenderDevice::FlyD3D9RenderDevice( HINSTANCE hModule ) : m_hModule(hModule),
    m_pD3D(NULL),m_pd3dDevice(NULL),m_bDeviceLost(false),m_bRendering(false)
{
    FLOG_INFO( "D3D9渲染设备已经创建." );

    // 创建Direct3D对象
    m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if( !m_pD3D )
        FLY_ERRORINFO( "创建Direct3D对象失败！","FlyD3D9RenderDevice::FlyD3D9RenderDevice()" );

    // 初始化设备配置
    m_Settings.nAdapterID = D3DADAPTER_DEFAULT;
    m_Settings.bColor16Bit = false;
    m_Settings.bVSync = false;
    m_Settings.msType = D3DMULTISAMPLE_NONE;
    m_Settings.dwQuality = 0;
}

/** 析构函数
*/
FlyD3D9RenderDevice::~FlyD3D9RenderDevice(void)
{
    SAFE_RELEASE( m_pd3dDevice );
    SAFE_RELEASE( m_pD3D );

    FLOG_INFO( "D3D9渲染设备已经卸载." );
}

/** 初始化渲染设备
@Param 主窗口的
@Param 是否开启配置对话框
@Param 设备选项数据(如果开启对话框则忽略此参数)
*/
void FlyD3D9RenderDevice::Initialize( const SWindowInfo& mainWnd,bool bShowDialog /* = true */,
                                      const FNameValueList* configData /* = NULL */ )
{
    FASSERT( !m_pMainWnd );
    FLOG_INFO( "D3D9渲染设备开始初始化..." );

    SWindowInfo wndInfo;
    memcpy( &wndInfo,&mainWnd,sizeof(SWindowInfo) );

    if( bShowDialog )
    {
        FlyD3D9ConfigDialog dlg(m_pD3D);

        if( !dlg.ShowDialog() )
        {
            FLOG_INFO( "配置对话框关闭，引擎即将关闭..." );
            return;
        }

        m_Settings = dlg.GetDeviceSettings();
        wndInfo.wndWidth = m_Settings.nWidth;
        wndInfo.wndHeight = m_Settings.nHeight;
        wndInfo.bFullScreen = m_Settings.bFullscreen;
    }
    else
    {
        FASSERT( configData );
        AKeyValueList::const_iterator it;

        // 适配器ID
        it = configData->find( _T("adapter") );
        if( it != configData->end() )
            _tscanf_s( it->second.c_str(),_T("%d"),&m_Settings.nAdapterID );

        // 颜色质量
        it = configData->find( _T("colordepth") );
        if( it != configData->end() )
        {
            if( it->second == _T("16") )
                m_Settings.bColor16Bit = true;
            else
                m_Settings.bColor16Bit = false;
        }

        // 垂直同步
        it = configData->find( _T("vsync") );
        if( it != configData->end() )
        {
            if( it->second == _T("true") )
                m_Settings.bVSync = true;
            else
                m_Settings.bVSync = false;
        }

        // 抗锯齿质量
        it = configData->find( _T("msquality") );
        if( it != configData->end() )
        {
            if( it->second == "none" )
            {
                m_Settings.msType = D3DMULTISAMPLE_NONE;
                m_Settings.dwQuality = 0;
            }
            else if( it->second.find(_T("(")) != it->second.npos )
            {
                m_Settings.msType = D3DMULTISAMPLE_NONMASKABLE;
                _tscanf_s( it->second.c_str(),_T("NonMaskable(%d)"),&m_Settings.dwQuality );
            }
            else
            {
                m_Settings.msType = FlyD3D9Util::StrToMSType( it->second.c_str() );
                m_Settings.dwQuality = 0;
            }
        }
    }

    // 创建设备的主窗口
    m_pMainWnd = CreateRenderWindow( _T("MainWnd"),wndInfo );

    FLOG_INFO( "D3D9渲染设备初始化完毕！" );
}

/** 关闭渲染设备
*/
void FlyD3D9RenderDevice::Shutdown(void)
{
    // 释放所有的渲染目标
    TargetQueue::iterator it;
    for( it=m_RenderTargets.begin();it!=m_RenderTargets.end();++it )
    {
        if( (*it) == m_pMainWnd )
            continue;
        else
            SAFE_DELETE( *it );
    }

    // 释放主窗口
    SAFE_DELETE( m_pMainWnd );
    m_RenderTargets.clear();

    FLOG_INFO( "D3D9渲染设备已经关闭！" );
}

/** 刷新渲染设备配置
*/
void FlyD3D9RenderDevice::RefreshDeviceSettings( const FNameValueList& settings )
{
    FNameValueList::const_iterator it;

    // 适配器ID
    it = settings.find( _T("adapter") );
    if( it != settings.end() )
        _tscanf_s( it->second.c_str(),_T("%d"),&m_Settings.nAdapterID );

    // 颜色质量
    it = settings.find( _T("colordepth") );
    if( it != settings.end() )
    {
        if( it->second == _T("16") )
            m_Settings.bColor16Bit = true;
        else
            m_Settings.bColor16Bit = false;
    }

    // 垂直同步
    it = settings.find( _T("vsync") );
    if( it != settings.end() )
    {
        if( it->second == _T("true") )
            m_Settings.bVSync = true;
        else
            m_Settings.bVSync = false;
    }

    // 抗锯齿质量
    it = settings.find( _T("msquality") );
    if( it != settings.end() )
    {
        if( it->second == "none" )
        {
            m_Settings.msType = D3DMULTISAMPLE_NONE;
            m_Settings.dwQuality = 0;
        }
        else if( it->second.find(_T("(")) != it->second.npos )
        {
            m_Settings.msType = D3DMULTISAMPLE_NONMASKABLE;
            _tscanf_s( it->second.c_str(),_T("NonMaskable(%d)"),&m_Settings.dwQuality );
        }
        else
        {
            m_Settings.msType = FlyD3D9Util::StrToMSType( it->second.c_str() );
            m_Settings.dwQuality = 0;
        }
    }

    // 重置渲染设备
}

/** 开始渲染场景
*/
void FlyD3D9RenderDevice::BeginRendering(void)
{
    FASSERT( !m_bRendering );
    FASSERT_( m_pActiveViewport,"No active viewport! Cannot begin rendering!" );
    m_pd3dDevice->BeginScene();
    m_bRendering = true;
}

/** 为活动的视口清除帧缓冲区
@Param 清除颜色缓冲区
@Param 清除深度缓冲区
@Param 清除模板缓冲区
*/
void FlyD3D9RenderDevice::ClearBuffer( EFrameBufferType clearType,const FColor32& color,
                                       float fZValue,ulong nStencilValue )
{
    FASSERT_( m_pActiveViewport,"No active viewport! Cannot clear framebuffer!" );

    DWORD dwClear = 0;
    if( clearType & FBC_COLOR ) dwClear |= D3DCLEAR_TARGET;
    if( clearType & FBC_DEPTH ) dwClear |= D3DCLEAR_ZBUFFER;
    if( (clearType & FBC_STENCIL) && m_DevCaps.bHWStencil ) dwClear |= D3DCLEAR_STENCIL;

    m_pd3dDevice->Clear( 0,NULL,dwClear,color.GetRGBAValue(),fZValue,nStencilValue );
}

/** 结束渲染场景
*/
void FlyD3D9RenderDevice::EndRendering(void)
{
    FASSERT( m_bRendering );
    m_pd3dDevice->EndScene();
    m_bRendering = false;
}

/** 创建一个渲染窗口
@Param 渲染窗口的名字
@Param 要创建窗口的参数
*/
FlyRenderWindow* FlyD3D9RenderDevice::CreateRenderWindow( const FString& name,const SWindowInfo& wndInfo )
{
    if( m_pMainWnd && m_pMainWnd->IsFullScreen() )
    {
        FLY_ERRORINFO( "主窗口是全屏模式，则不能再创建副窗口！",
            "FlyD3D9RenderDevice::CreateRenderWindow()" );
    }

    if( m_pMainWnd && wndInfo.bFullScreen )
    {
        FLY_ERRORINFO( "副窗口不能被创建为全屏模式！","FlyD3D9RenderDevice::CreateRenderWindow()" );
    }

    // 检测是否有相同名字的渲染目标
    TargetQueue::const_iterator it;
    for( it=m_RenderTargets.begin();it!=m_RenderTargets.end();it++ )
    {
        if( (*it)->GetName() == name )
        {
            FLY_ERRORINFO( "已存在名为(" + name + ")的渲染目标！",
                "FlyD3D9RenderDevice::CreateRenderWindow()" );
        }
    }

    // 创建一个新的渲染窗口，添加到目标队列
    F3DRenderWindow* wnd = new FD3D9RenderWindow( name,(name == "MainWnd" ? true : false),this );
    wnd->Create( wndInfo );
    m_RenderTargets.insert( wnd );

    return wnd;
}

/*
//------------------------------------------------------------------------
// Create the direct3D device for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:               The id of the active adapter.
//      DEVICESETTINGS*:    Pointer to the device settings.
//      HWND*:              Sub Render window's handle.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::Go( UINT nAdapterID,DEVICESETTINGS* pSettings,
                                HWND* hWnds )
{
    HRESULT     hr;
    D3DCAPS9    caps;
    RECT        rc;
    UINT        nWidth,nHeight;
    bool        bHW = true;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    logMgr.WriteLine( 0,"Initialising the Render system (Direct3D) ..." );

    // Create the direct3D object.
    m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

    if( !m_pD3D )
    {
        logMgr.WriteError( 0,"Create the Direct3D object failed!" );
        return FLY_CREATEAPI;
    }

    m_pD3D->GetDeviceCaps( nAdapterID,D3DDEVTYPE_HAL,&caps );

    if( (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 )
    {
        logMgr.WriteLine( 0,"Warning: No hardware vertex processing!" );
        bHW = false;
    }

    // Prepare the present parameters.
    memset( &m_d3dpp,0,sizeof(D3DPRESENT_PARAMETERS) );
    m_d3dpp.Windowed                = pSettings->bWindowed;
    m_d3dpp.BackBufferCount         = 1;
    m_d3dpp.BackBufferFormat        = pSettings->backBufferFmt;
    m_d3dpp.EnableAutoDepthStencil  = TRUE;
    m_d3dpp.AutoDepthStencilFormat  = pSettings->depthStencilFmt;
    m_d3dpp.MultiSampleType         = D3DMULTISAMPLE_NONE;
    m_d3dpp.SwapEffect              = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.MultiSampleQuality      = 0;
    m_d3dpp.PresentationInterval    = pSettings->nPresentInterval;

    // Fullscreen mode.
    if( !pSettings->bWindowed )
    {
        m_d3dpp.hDeviceWindow       = m_hWndMain;
        m_d3dpp.BackBufferWidth     = pSettings->nWidth;
        m_d3dpp.BackBufferHeight    = pSettings->nHeight;
    }

    // Window mode.
    else
    {
        GetClientRect( m_hWndMain,&rc );

        m_d3dpp.hDeviceWindow       = m_hWndMain;
        m_d3dpp.BackBufferWidth     = rc.right;
        m_d3dpp.BackBufferHeight    = rc.bottom;
    }

    // Create the Direct3D device.
    if( bHW )
    {
        hr = m_pD3D->CreateDevice( nAdapterID,D3DDEVTYPE_HAL,m_hWndMain,D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                   &m_d3dpp,&m_pd3dDevice );
    }
    else
    {
        hr = m_pD3D->CreateDevice( nAdapterID,D3DDEVTYPE_HAL,m_hWndMain,D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                   &m_d3dpp,&m_pd3dDevice );
    }

    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Create the Direct3D device failed!" );
        return hr;
    }

    // Create the swap chain if there are more than one sub-Windows.
    // And must be the window mode.
    for( int i=0;i<m_nNumWnds;i++ )
    {
        GetClientRect( hWnds[i],&rc );
        nWidth = rc.right;
        nHeight = rc.bottom;

        m_pWindows[i] = new FlyD3DRenderWindow();
        m_pWindows[i]->SetD3DDevicePtr( m_pd3dDevice );

        hr = m_pWindows[i]->Create( hWnds[i],nWidth,nHeight,&m_d3dpp );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Create the RenderWindow failed!" );
            return hr;
        }
    }

    logMgr.WriteLine( 0,"Initialized(online and ready) " );

    // Store the device caps.
    memset( &caps,0,sizeof(D3DCAPS9) );
    m_pd3dDevice->GetDeviceCaps( &caps );
    StoreDeviceCaps( &caps );
    LogDeviceCaps( &caps );

    // Open the stencil buffer ?
    if( pSettings->depthStencilFmt == D3DFMT_D24S8 ||
        pSettings->depthStencilFmt == D3DFMT_D24X4S4 ||
        pSettings->depthStencilFmt == D3DFMT_D15S1 )
        m_bStencilEnable = true;
    else
        m_bStencilEnable = false;

    // Save the current main target.
    m_pd3dDevice->GetRenderTarget( 0,&m_pMainTarget );

    return OneTimeInit();
}


//------------------------------------------------------------------------
// Init the Render device render state, This function can be only run
// once.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::OneTimeInit(void)
{
    float fAspect;
    FLYLIGHT light;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_GOURAUD );
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,D3DCULL_CCW );
    m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_SOLID );

    // Set the default sampler state.
    int i = 0;
    for( ;i<MAX_TEXTURES;i++ )
    {
        m_pd3dDevice->SetSamplerState( i,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR );
        m_pd3dDevice->SetSamplerState( i,D3DSAMP_MINFILTER,D3DTEXF_LINEAR );
        m_pd3dDevice->SetSamplerState( i,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR );
    }

    // Set the default value for texture stages.
    for( i=0;i<MAX_TEXTURES;i++ )
        m_Samplers[i].InitSamplerStates();

    // Set the default render states.
    m_RenderStates.InitRenderStates();

    // Set the world matrix.
    SetWorldMatrix( NULL );

    // Init the view matrix.
    SetView3D( FlyVector(1.0f,0.0f,0.0f),FlyVector(0.0f,1.0f,0.0f),
               FlyVector(0.0f,0.0f,1.0f),FlyVector(0.0f,0.0f,0.0f) );

    // Init the Main projection matrix.
    fAspect = m_d3dpp.BackBufferWidth / (float)m_d3dpp.BackBufferHeight;
    CalcPerspProjMatrix( D3DX_PI/4.0f,fAspect,1.0f,1000.0f,&m_mProjection );

    // Ini t the sub projection matrices.
    for( i=0;i<m_nNumWnds;i++ )
    {
        fAspect = m_pWindows[i]->GetBackWidth() / (float)m_pWindows[i]->GetBackHeight();
        CalcPerspProjMatrix( D3DX_PI/4.0f,fAspect,1.0f,1000.0f,&m_mSubProj[i] );
    }

    // Use the main stage.
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION,(D3DMATRIX*)&m_mProjection );

    return FLY_OK;
}

//------------------------------------------------------------------------
// Swap the buffers, Present the scene on the screen.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SwapBuffers(void)
{
    HRESULT hr;

    if( m_nActiveStage == -1 )
        hr = m_pd3dDevice->Present( NULL,NULL,NULL,NULL );
    else
        hr = m_pWindows[m_nActiveStage]->SwapBuffers();

    if( hr == D3DERR_DEVICELOST )
        m_bDeviceLost = true;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Use a texture as the render target for the device.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyTexture*:  Pointer to the texture, This texture can only
//                          have one level, and the same format with the
//                          Backbuffer surface.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetTextureRenderTarget( FlyTexture* pTexture )
{
    HRESULT hr;
    IDirect3DTexture9* pd3dTex;
    IDirect3DSurface9* pSurface;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( !pTexture )
    {
        m_pd3dDevice->SetRenderTarget( 0,m_pMainTarget );
        return FLY_OK;
    }

    if( pTexture->GetUsage() != TU_RENDERTARGET )
    {
        logMgr.WriteError( 0,"Texture is not a RenderTarget! SetTextureRenderTarget() failed!" );
        return FLY_INVALIDPARAM;
    }

    if( pTexture->GetMipmapCount() > 1 )
    {
        logMgr.WriteError( 0,"Multi-level texture can't be a RenderTarget! SetTextureRenderTarget() failed!" );
        return FLY_INVALIDPARAM;
    }

//     if( FlyD3DUtil::FormatToD3DFormat(pTexture->GetFormat())
//         != m_d3dpp.BackBufferFormat )
//     {
//         logMgr.WriteError( 0,"Texture format is different from backbuffer! SetTextureRenderTarget() failed!" );
//         return FLY_INVALIDPARAM;
//     }

    // Get the texture's surface.
    pd3dTex = (LPDIRECT3DTEXTURE9)((FlyD3DTexture*)pTexture)->GetD3DTexturePtr();
    hr = pd3dTex->GetSurfaceLevel( 0,&pSurface );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Get the texture surface error, SetTextureRenderTarget() failed!" );
        return hr;
    }

    m_pd3dDevice->SetRenderTarget( 0,pSurface );
    pSurface->Release();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the active scene stage for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the scene stage, -1 is the main stage.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::UseSceneStage( int nStage )
{
    HRESULT hr;
    LPDIRECT3DSURFACE9 pBackbuffer;

    if( nStage < -1 || nStage >= m_nNumWnds )
        return FLY_INVALIDPARAM;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( !m_d3dpp.Windowed && nStage != -1 )
    {
        logMgr.WriteError( 0,"Can't change the stage during the fullscreen mode!" );
        return FLY_FAIL;
    }

    if( nStage == -1 )
    {
        hr = m_pd3dDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&pBackbuffer );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Can't get the backbuffer, UseSceneStage() failed!" );
            return hr;
        }

        m_pd3dDevice->SetRenderTarget( 0,pBackbuffer );
        m_pd3dDevice->SetTransform( D3DTS_PROJECTION,(D3DMATRIX*)&m_mProjection );
        m_nActiveStage = -1;
        pBackbuffer->Release();

        // Change the main target.
        SAFE_RELEASE( m_pMainTarget );
        m_pd3dDevice->GetRenderTarget( 0,&m_pMainTarget );
        return FLY_OK;
    }

    hr = m_pWindows[nStage]->GetBackBuffer( &pBackbuffer );
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Can't get the backbuffer, UseSceneStage() failed!" );
        return hr;
    }

    m_pd3dDevice->SetRenderTarget( 0,pBackbuffer );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION,(D3DMATRIX*)&m_mSubProj[nStage] );
    m_nActiveStage = nStage;
    pBackbuffer->Release();

    // Change the main target.
    SAFE_RELEASE( m_pMainTarget );
    m_pd3dDevice->GetRenderTarget( 0,&m_pMainTarget );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the active scene stage index.
//------------------------------------------------------------------------
int FlyD3DRenderDevice::GetActiveStage(void) const
{
    return m_nActiveStage;
}


//------------------------------------------------------------------------
// Set the Direct3D clear color for the RenderDevice.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Red value of the color.
//      float:      Green value of the color.
//      float:      Blue value of the color.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetClearColor( float fR,float fG,float fB )
{
    m_ClearColor = D3DCOLOR_COLORVALUE( fR,fG,fB,1.0f );
}


//------------------------------------------------------------------------
// Set the RenderDevice to the fullscreen mode.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Use the fullscreen mode ?
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetFullscreen( bool bFullscreen )
{
    RECT rc;
    HRESULT hr;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( m_nActiveStage != -1 )
    {
        logMgr.WriteError( 0,"Only the main stage can be set to Fullscreen!" );
        return FLY_FAIL;
    }

    if( GetParent(m_hWndMain) != NULL )
    {
        logMgr.WriteError( 0,"Window is not the top level, SetFullscreen() failed!" );
        return FLY_FAIL;
    }

    if( bFullscreen )
    {
        GetWindowRect( m_hWndMain,&m_rcWindow );

        GetClientRect( m_hWndMain,&rc );
        m_nWndWidth = rc.right;
        m_nWndHeight = rc.bottom;
    }

    // Reset the present parameters.
    if( bFullscreen )
    {
        m_d3dpp.BackBufferWidth = m_nWidth;
        m_d3dpp.BackBufferHeight = m_nHeight;
    }
    else
    {
        m_d3dpp.BackBufferWidth = m_nWndWidth;
        m_d3dpp.BackBufferHeight = m_nWndHeight;
    }

    m_d3dpp.Windowed = !bFullscreen;
 
    hr = ResetDevice();
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Reset the device error, SetFullscreen() failed!" );
        return hr;
    }

    // If we change to window mode.
    // Reset the window pos.
    if( !bFullscreen )
    {
        SetWindowPos( m_hWndMain,HWND_NOTOPMOST,m_rcWindow.left,m_rcWindow.top,
                      m_rcWindow.right-m_rcWindow.left,
                      m_rcWindow.bottom-m_rcWindow.top,SWP_SHOWWINDOW );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the current screen to an image file, This function must be 
// called after rendering and before swap the buffers, Or you we can't
// Catch any pixels.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYIMAGETYPE:   Which type of image to save.
//      const char*:    Filename to store the image.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SaveScreenToFile( FLYIMAGETYPE Type,const char* cFilename )
{
    HRESULT hr;
    D3DLOCKED_RECT rect;
    IDirect3DSurface9* pSurface;
    IDirect3DSurface9* pStretch;
    IDirect3DSurface9* pImageSurface;
    D3DSURFACE_DESC desc;
    FlyImage image;

    // If is rendering, can't save the screen.
    if( m_bRendering ) return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // 1. Get the backbuffer.
    if( m_nActiveStage == -1 )
    {
        hr = m_pd3dDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&pSurface );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Get Main stage back buffer error, SaveScreenToFile() failed!" );
            return hr;
        }

        // Create an image surface.
        hr = m_pd3dDevice->CreateOffscreenPlainSurface( m_d3dpp.BackBufferWidth,
            m_d3dpp.BackBufferHeight,m_d3dpp.BackBufferFormat,D3DPOOL_SYSTEMMEM,
            &pImageSurface,NULL );
        if( FAILED(hr) )
        {
            pSurface->Release();
            logMgr.WriteError( 0,"Create the image surface error, SaveScreenToFile() failed!" );
            return hr;
        }
    }
    else
    {
        hr = m_pWindows[m_nActiveStage]->GetBackBuffer( &pSurface );
        if( FAILED(hr) )
        {
            logMgr.WriteError( 0,"Get sub stage %d back buffer error, SaveScreenToFile() failed!",
                m_nActiveStage );
            return hr;
        }

        // Create an image surface.
        hr = m_pd3dDevice->CreateOffscreenPlainSurface( m_pWindows[m_nActiveStage]->GetBackWidth(),
            m_pWindows[m_nActiveStage]->GetBackHeight(),m_d3dpp.BackBufferFormat,D3DPOOL_SYSTEMMEM,
            &pImageSurface,NULL );
        if( FAILED(hr) )
        {
            pSurface->Release();
            logMgr.WriteError( 0,"Create the image surface error, SaveScreenToFile() failed!" );
            return hr;
        }
    }

    // 2. Copy the pixels to the system memory.
    pSurface->GetDesc( &desc );

    if( desc.MultiSampleType == D3DMULTISAMPLE_NONE )
    {
        hr = m_pd3dDevice->GetRenderTargetData( pSurface,pImageSurface );
        if( FAILED(hr) )
        {
            pSurface->Release();
            pImageSurface->Release();
            logMgr.WriteError( 0,"Read the render target data error, SaveScreenToFile() failed!" );
            return hr;
        }

        pSurface->Release();
    }
    else
    {
        // Create the stretch surface.
        hr = m_pd3dDevice->CreateRenderTarget( desc.Width,desc.Height,desc.Format,
            D3DMULTISAMPLE_NONE,0,FALSE,&pStretch,NULL );
        if( FAILED(hr) )
        {
            pSurface->Release();
            pImageSurface->Release();
            logMgr.WriteError( 0,"Create the stretch surface error, SaveScreenToFile() failed!" );
            return hr;
        }

        // Stretch the surface.
        hr = m_pd3dDevice->StretchRect( pSurface,NULL,pStretch,NULL,D3DTEXF_NONE );
        if( FAILED(hr) )
        {
            pSurface->Release();
            pStretch->Release();
            pImageSurface->Release();
            logMgr.WriteError( 0,"Stretch the surface error, SaveScreenToFile() failed!" );
            return hr;
        }

        // Read the pixels datas.
        hr = m_pd3dDevice->GetRenderTargetData( pStretch,pImageSurface );
        if( FAILED(hr) )
        {
            pSurface->Release();
            pStretch->Release();
            pImageSurface->Release();
            logMgr.WriteError( 0,"Read the render target data error, SaveScreenToFile() failed!" );
            return hr;
        }

        pSurface->Release();
        pStretch->Release();
    }

    if( Type == FIMAGE_DDS )
    {
        hr = D3DXSaveSurfaceToFile( cFilename,D3DXIFF_DDS,pSurface,NULL,NULL );
        if( FAILED(hr) )
        {
            pImageSurface->Release();
            logMgr.WriteError( 0,"Save the surface to DDS error, SaveScreenToFile() failed!" );
            return hr;
        }

        pImageSurface->Release();
        return FLY_OK;
    }
    else
    {
        PIXELFORMAT pfmt;
        pImageSurface->GetDesc( &desc );

        if( desc.Format == D3DFMT_X8R8G8B8 )
            pfmt = PFT_X8R8G8B8;
        else if( desc.Format == D3DFMT_R5G6B5 )
            pfmt = PFT_R5G6B5;
        else
        {
            pImageSurface->Release();
            logMgr.WriteError( 0,"Unknown pixel format, SaveScreenToFile() failed!" );
            return FLY_UNKNOWNFORMAT;
        }

        // Lock the pixels for the surface.
        hr = pImageSurface->LockRect( &rect,NULL,0 );
        if( FAILED(hr) )
        {
            pImageSurface->Release();
            logMgr.WriteError( 0,"Lock the pixel data error, SaveScreenToFile() failed!" );
            return hr;
        }

        // Load the pixel datas.
        image.LoadImageFromMemory( (UCHAR*)rect.pBits,pfmt,desc.Width,desc.Height );

        pImageSurface->UnlockRect();
        pImageSurface->Release();

        switch( Type )
        {
        case FIMAGE_BMP:
            image.SaveImageToBitmap( cFilename,false );
            break;
        case FIMAGE_JPG:
            image.SaveImageToJPEG( cFilename,false );
            break;
        case FIMAGE_PNG:
            image.SaveImageToPNG( cFilename,false );
            break;
        case FIMAGE_TEX:
            image.SaveImageToFile( cFilename,false );
            break;
        case FIMAGE_TGA:
            image.SaveImageToTGA( cFilename,false );
            break;
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Resize the main window for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        New width of the window.
//      int:        New height of the window.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::ResizeMainWindow( int nWidth,int nHeight )
{
    HRESULT hr;

    // Must be the main stage.
    if( m_nActiveStage != -1 )
        return FLY_FAIL;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( !m_d3dpp.Windowed )
    {
        logMgr.WriteError( 0,"Can't resize the window during fullscreen mode!" );
        return FLY_FAIL;
    }

    if( m_d3dpp.BackBufferWidth == nWidth &&
        m_d3dpp.BackBufferHeight == nHeight )
        return FLY_OK;

    m_d3dpp.BackBufferWidth = nWidth;
    m_d3dpp.BackBufferHeight = nHeight;

    hr = ResetDevice();
    if( FAILED(hr) )
    {
        logMgr.WriteError( 0,"Reset the device error, ResizeMainWindow() failed!" );
        return hr;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Resize the sub window for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the render window.
//      int:        New width of the window.
//      int:        New height of the window.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::ResizeSubWindow( int nIndex,int nWidth,int nHeight )
{
    HRESULT hr;

    if( nIndex < 0 || nIndex >= m_nNumWnds )
        return FLY_INVALIDPARAM;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( !m_d3dpp.Windowed )
    {
        logMgr.WriteError( 0,"Can't resize window during fullscreen mode!" );
        return FLY_FAIL;
    }

    hr = m_pWindows[nIndex]->ResizeWindow( nWidth,nHeight );
    if( FAILED(hr) ) return hr;

    if( nIndex == m_nActiveStage )
        UseSceneStage( nIndex );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Reset the device for the engine.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::ResetDevice(void)
{
    HRESULT hr;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // Lost the device objects.
    OnDeviceLost();

    // Reset the device.
    hr = m_pd3dDevice->Reset( &m_d3dpp );
    if( FAILED(hr) ) return hr;

    // Reset the device objects.
    OnDeviceReset();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Execute this function when the device is lost.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::OnDeviceLost(void)
{
    // Release the swap chains.
    for( int i=0;i<m_nNumWnds;i++ )
        m_pWindows[i]->DestroyD3DResources();

    FlyD3DResourceManager* pResMgr = static_cast<FlyD3DResourceManager*>(
        FlyKernel::Instance().GetResourceManager());
    pResMgr->OnDeviceLost();
}


//------------------------------------------------------------------------
// Execute this function after resetting the device.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::OnDeviceReset(void)
{
    if( m_d3dpp.Windowed )
    {
        // Create the swap chains.
        for( int i=0;i<m_nNumWnds;i++ )
                m_pWindows[i]->CreateD3DResources();
    }

    FlyD3DResourceManager* pResMgr = static_cast<FlyD3DResourceManager*>(
        FlyKernel::Instance().GetResourceManager());
    pResMgr->OnDeviceReset();
}


//------------------------------------------------------------------------
// Restore device, Execute when the device is lost.
// This function is just for Direct3D.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::RestoreDevice(void)
{
    HRESULT hr;

    if( m_bDeviceLost )
    {
        // Test the cooperative level
        hr = m_pd3dDevice->TestCooperativeLevel();
        if( FAILED(hr) )
        {
            if( hr == D3DERR_DEVICELOST )
            {
                Sleep( 100 );
                return FLY_OK;
            }

            // We can reset the device.
            if( hr == D3DERR_DEVICENOTRESET )
            {
                hr = ResetDevice();
                if( FAILED(hr) )
                    return hr;

                m_bDeviceLost = false;
            }

            return hr;
        }
    }

    return FLY_OK;
}




//------------------------------------------------------------------------
// Get the instance of the module.
//------------------------------------------------------------------------
HINSTANCE FlyD3DRenderDevice::GetAppInstance(void) const
{
    return m_hDLL;
}


//------------------------------------------------------------------------
// Get the handle to the main window for the engine.
//------------------------------------------------------------------------
HWND FlyD3DRenderDevice::GetMainWindow(void) const
{
    return m_hWndMain;
}


//------------------------------------------------------------------------
// Get the Sub render window's handle.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the render window.
//------------------------------------------------------------------------
HWND FlyD3DRenderDevice::GetRenderWindow( int nIndex ) const
{
    if( nIndex < 0 || nIndex >= m_nNumWnds )
        return NULL;

    return m_pWindows[nIndex]->GetWindowHandle();
}


//------------------------------------------------------------------------
// Is the RenderDevice lost ? This function is Only for Direct3D.
//------------------------------------------------------------------------
bool FlyD3DRenderDevice::IsDeviceLost(void) const
{
    return m_bDeviceLost;
}


//------------------------------------------------------------------------
// Is the device rendering the scene ?
//------------------------------------------------------------------------
bool FlyD3DRenderDevice::IsRendering(void) const
{
    return m_bRendering;
}


//------------------------------------------------------------------------
// Get the active render material for the device.
//------------------------------------------------------------------------
const FlyRenderMaterial* FlyD3DRenderDevice::GetActiveMaterial(void) const
{
    return m_pActiveMaterial;
}


//------------------------------------------------------------------------
// Set the active render material for the device.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderMaterial*:     Pointer to the render material.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetActiveMaterial( FlyRenderMaterial* pMaterial )
{
    m_pActiveMaterial = pMaterial;
    m_pActivePass = NULL;
}


//------------------------------------------------------------------------
// Get the active render pass for the device.
//------------------------------------------------------------------------
const FlyRenderPass* FlyD3DRenderDevice::GetActivePass(void) const
{
    return m_pActivePass;
}


//------------------------------------------------------------------------
// Set the active render pass for the device.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderPass*:     Pointer to the render pass.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetActivePass( FlyRenderPass* pPass )
{
    m_pActivePass = pPass;
    m_pActiveMaterial = NULL;
}


//------------------------------------------------------------------------
// Get the current active fixed pipeline pass.
//------------------------------------------------------------------------
int FlyD3DRenderDevice::GetActiveFixedPass(void) const
{
    return m_nActiveFixedPass;
}


//------------------------------------------------------------------------
// Set the active fixed pipeline pass.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetActiveFixedPass( int nPass )
{
    m_nActiveFixedPass = nPass;
}


//------------------------------------------------------------------------
// Get the active shader object of the device.
//------------------------------------------------------------------------
const FlyShaderObject* FlyD3DRenderDevice::GetShaderObject(void) const
{
    return m_pActiveShader;
}


//------------------------------------------------------------------------
// Is the engine using the fullscreen mode ?
//------------------------------------------------------------------------
bool FlyD3DRenderDevice::IsFullscreen(void) const
{
    return !m_d3dpp.Windowed;
}


//------------------------------------------------------------------------
// Get the main backbuffer's width.
//------------------------------------------------------------------------
UINT FlyD3DRenderDevice::GetBackWidth(void) const
{
    if( m_nActiveStage < -1 || m_nActiveStage >= m_nNumWnds )
        return 0;

    if( m_nActiveStage == -1 )
        return m_d3dpp.BackBufferWidth;
    else
        return m_pWindows[m_nActiveStage]->GetBackWidth();
}


//------------------------------------------------------------------------
// Get the main backbuffer's height.
//------------------------------------------------------------------------
UINT FlyD3DRenderDevice::GetBackHeight(void) const
{
    if( m_nActiveStage < -1 || m_nActiveStage >= m_nNumWnds )
        return 0;

    if( m_nActiveStage == -1 )
        return m_d3dpp.BackBufferHeight;
    else
        return m_pWindows[m_nActiveStage]->GetBackHeight();
}


//------------------------------------------------------------------------
// Get the device caps for the render device.
//------------------------------------------------------------------------
const DEVICECAPS* FlyD3DRenderDevice::GetDeviceCaps(void) const
{
    return &m_DevCaps;
}


//------------------------------------------------------------------------
// Get the world matrix.
//------------------------------------------------------------------------
const FlyMatrix& FlyD3DRenderDevice::GetWorldMatrix(void) const
{
    return m_mWorld;
}


//------------------------------------------------------------------------
// Get the 3D view matrix.
//------------------------------------------------------------------------
const FlyMatrix& FlyD3DRenderDevice::GetViewMatrix(void) const
{
    return m_mView3D;
}


//------------------------------------------------------------------------
// Get the main stage's projection matrix.
//------------------------------------------------------------------------
const FlyMatrix& FlyD3DRenderDevice::GetProjectionMatrix(void) const
{
    if( m_nActiveStage == -1 )
        return m_mProjection;
    else
        return m_mSubProj[m_nActiveStage];
}


//------------------------------------------------------------------------
// Get the stage's projection matrix.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the stage.
//------------------------------------------------------------------------
const FlyMatrix& FlyD3DRenderDevice::GetStageProjMatrix( int nStage ) const
{
    if( nStage < -1 || nStage >= m_nNumWnds )
        return FlyMatrix::IDENTITY;
    else
    {
        if( nStage == -1 )
            return m_mProjection;
        else
            return m_mSubProj[nStage];
    }
}


//------------------------------------------------------------------------
// Get the Current camera position.
//------------------------------------------------------------------------
const FlyVector& FlyD3DRenderDevice::GetCameraPos(void) const
{
    return m_vCameraPos;
}


//------------------------------------------------------------------------
// Set the world matrix for the RenderDevice.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyMatrix*:   Pointer to the world matrix.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetWorldMatrix( const FlyMatrix* mat )
{
    if( !mat )
    {
        m_mWorld.Identity();
    }
    else
    {
        memcpy( &m_mWorld,mat,sizeof(FlyMatrix) );
    }

    m_pd3dDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)&m_mWorld );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the view matrix for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Right direction of the camera.
//      const FlyVector&:   Up direction of the camera.
//      const FlyVector&:   Front direction of the camera.
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetView3D( const FlyVector& vRight,const FlyVector& vUp,
                                       const FlyVector& vDir,const FlyVector& vPos )
{
    m_mView3D._14 = m_mView3D._24 = m_mView3D._34 = 0.0f;
    m_mView3D._44 = 1.0f;

    m_mView3D._11 = vRight.x;
    m_mView3D._21 = vRight.y;
    m_mView3D._31 = vRight.z;
    m_mView3D._41 = -(vRight * vPos);

    m_mView3D._12 = vUp.x;
    m_mView3D._22 = vUp.y;
    m_mView3D._32 = vUp.z;
    m_mView3D._42 = -(vUp * vPos);

    m_mView3D._13 = vDir.x;
    m_mView3D._23 = vDir.y;
    m_mView3D._33 = vDir.z;
    m_mView3D._43 = -(vDir * vPos);

    // Set view matrix
    m_pd3dDevice->SetTransform( D3DTS_VIEW,(D3DMATRIX*)&m_mView3D );

    // Save the camera's position.
    m_vCameraPos = vPos;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the view matrix for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//      const FlyVector&:   Look at point of the camera.
//      const FlyVector&:   Up direction of the world.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetViewLookAt( const FlyVector& vPos,const FlyVector& vAt,
                                           const FlyVector& vWorldUp )
{
    FlyVector xAxis,yAxis,zAxis;
    zAxis = vAt - vPos;
    zAxis.Normalize();
    xAxis.Cross( vWorldUp,zAxis );
    xAxis.Normalize();
    yAxis.Cross( zAxis,xAxis );

    m_mView3D._14 = m_mView3D._24 = m_mView3D._34 = 0.0f;
    m_mView3D._44 = 1.0f;

    m_mView3D._11 = xAxis.x;
    m_mView3D._21 = xAxis.y;
    m_mView3D._31 = xAxis.z;
    m_mView3D._41 = -(xAxis * vPos);

    m_mView3D._12 = yAxis.x;
    m_mView3D._22 = yAxis.y;
    m_mView3D._32 = yAxis.z;
    m_mView3D._42 = -(yAxis * vPos);

    m_mView3D._13 = zAxis.x;
    m_mView3D._23 = zAxis.y;
    m_mView3D._33 = zAxis.z;
    m_mView3D._43 = -(zAxis * vPos);

    // Set the view matrix.
    m_pd3dDevice->SetTransform( D3DTS_VIEW,(D3DMATRIX*)&m_mView3D );

    // Save the camera position.
    m_vCameraPos = vPos;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the clip plane's near and far distance.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the scene stage.
//      float:      Near distance of the plane.
//      float:      Far distance of the plane.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetViewPlanes( int nStage,float fNear,float fFar )
{
    float fN,fF,q;

    if( nStage < -1 || nStage >= m_nNumWnds )
        return FLY_INVALIDPARAM;

    fN = fNear;
    fF = fFar;

    if( fN <= 0.0f ) fN = 0.01f;
    if( fF < 1.0f ) fF = 1.0f;

    if( fN >= fF )
    {
        fN = fF;
        fF = fN + 1.0f;
    }

    q = fF / (fF - fN);

    // Main scene stage
    if( nStage == -1 )
    {
        m_mProjection._33 = q;
        m_mProjection._43 = -fN * q;
        return FLY_OK;
    }

    // Sub scene stage.
    m_mSubProj[nStage]._33 = q;
    m_mSubProj[nStage]._43 = -fN * q;

    // Active the stage.
    UseSceneStage( m_nActiveStage );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the FOV for the projection matrix.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the scene stage.
//      float:      Field of the view, in the y direction.
//      float:      View space width divided by height.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetFOV( int nStage,float fFOV,float fAspect )
{
    float w,h;

    if( nStage < -1 || nStage >= m_nNumWnds )
        return FLY_INVALIDPARAM;

    h = cosf(fFOV/2.0f) / sinf(fFOV/2.0f);
    w = h / fAspect;

    if( nStage == -1 )
    {
        m_mProjection._11 = w;
        m_mProjection._22 = h;
        return FLY_OK;
    }

    m_mSubProj[nStage]._11 = w;
    m_mSubProj[nStage]._22 = h;

    // Active the stage.
    UseSceneStage( m_nActiveStage );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the projection for the device.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetProjectionMatrix( const FlyMatrix& mProjection )
{
    if( m_nActiveStage == -1 )
        m_mProjection = mProjection;
    else
        m_mSubProj[m_nActiveStage] = mProjection;

    m_pd3dDevice->SetTransform( D3DTS_PROJECTION,(D3DMATRIX*)&mProjection );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Calculate the perspective projection matrix.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Field of the view, in the y direction.
//      float:      Aspect ratio, View space width divided by height.
//      float:      Near plane of the view.
//      float:      Far plane of the view.
//      FlyMatrix*: To store the projection matrix.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::CalcPerspProjMatrix( float fFOV,float fAspect,float fN,
                                              float fF,FlyMatrix* pOut )
{
    float w,h,q;

    h = cosf(fFOV/2.0f) / sinf(fFOV/2.0f);
    w = h / fAspect;
    q = fF / (fF - fN);

    memset( pOut,0,sizeof(FlyMatrix) );
    pOut->_11 = w;
    pOut->_22 = h;
    pOut->_33 = q;
    pOut->_34 = 1.0f;
    pOut->_43 = -fN * q;
}


//------------------------------------------------------------------------
// Set the clip planes for the device.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::SetClipPlanes( const std::vector<FlyPlane>& planeList )
{
    DWORD dwMask = 0;

    for( size_t i=0;i<planeList.size();i++ )
    {
        const FlyPlane& plane = planeList[i];

        m_pd3dDevice->SetClipPlane( (DWORD)i,(float*)&plane );
        dwMask |= (1 << i);
    }

    m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,dwMask );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the Ambient light color for the RenderDevice.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Red value of the RenderDevice.
//      float:      Green value of the RenderDevice.
//      float:      Blue value of the RenderDevice.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetAmbientLight( float fR,float fG,float fB )
{
    m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,D3DCOLOR_COLORVALUE(fR,fG,fB,1.0f) );

    // Save the render states.
    m_cAmbient.fR = fR;
    m_cAmbient.fG = fG;
    m_cAmbient.fB = fB;
    m_cAmbient.fA = 1.0f;
}


//------------------------------------------------------------------------
// Set the fog mode for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      FOGMODE:            Fog mode for the render device.
//      const FLYCOLOR&:    Color of the fog.
//      float:              Density of the fog.
//      float:              Near plane of the fog.
//      float:              Far plane of the fog.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetFog( FOGMODE mode,const FLYCOLOR& clr,float fDensity,
                                 float fNear,float fFar )
{
    if( mode == FOG_NONE )
    {
        m_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE,D3DFOG_NONE );
        m_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,D3DFOG_NONE );
        m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,FALSE );
    }
    else
    {
        if( m_DevCaps.bTableFog )
        {
            m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,TRUE );
            m_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE,mode );
            m_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,D3DFOG_NONE );
            m_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR,clr.GetRGBAValue() );
            m_pd3dDevice->SetRenderState( D3DRS_FOGDENSITY,CONVERT2DWORD(fDensity) );
            m_pd3dDevice->SetRenderState( D3DRS_FOGSTART,CONVERT2DWORD(fNear) );
            m_pd3dDevice->SetRenderState( D3DRS_FOGEND,CONVERT2DWORD(fFar) );
        }
        else
        {
            if( m_DevCaps.bVertexFog )
            {
                m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,TRUE );
                m_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE,D3DFOG_NONE );
                m_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,mode );
                m_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR,clr.GetRGBAValue() );
                m_pd3dDevice->SetRenderState( D3DRS_FOGDENSITY,CONVERT2DWORD(fDensity) );
                m_pd3dDevice->SetRenderState( D3DRS_FOGSTART,CONVERT2DWORD(fNear) );
                m_pd3dDevice->SetRenderState( D3DRS_FOGEND,CONVERT2DWORD(fFar) );
            }
            else
            {
                // Get the logManager.
                FlyLogManager& logMgr = FlyLogManager::GetSingleton();
                logMgr.WriteError( 0,"Device can't support the specified fog effect!" );
            }
        }
    }

    // Save the render states.
    m_FogMode = mode;
    m_cFogColor = clr;
    m_fFogDensity = fDensity;
    m_fFogNear = fNear;
    m_fFogFar = fFar;
}


//------------------------------------------------------------------------
// Set the texture for the stage.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Which stage to set the texture.
//      const FlyTexture*:  Pointer to the texture.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetTexture( int nStage,const FlyTexture* pTexture )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES ) return;

    if( pTexture == NULL )
    {
        m_pd3dDevice->SetTexture( nStage,NULL );
        m_Samplers[nStage].pTexture = NULL;
        return;
    }

    // Set the Direct3D texture.
    FlyD3DTexture* pTex = (FlyD3DTexture*)pTexture;
    m_pd3dDevice->SetTexture( nStage,pTex->GetD3DTexturePtr() );

    // Store the texture stage.
    m_Samplers[nStage].pTexture = (FlyTexture*)pTexture;
}


//------------------------------------------------------------------------
// Set the each stage's sampler filter mode.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Which stage the sampler is ?
//      FILTEROPTION:   Magnification filter type.
//      FILTEROPTION:   Minification filter type.
//      FILTEROPTION:   Mipmap filter to use during minification.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetTextureFilterMode( int nStage,FILTEROPTION magFO,FILTEROPTION minFO,
                                               FILTEROPTION mipFO )
{
    D3DTEXTUREFILTERTYPE filter;

    filter = FlyD3DUtil::GetD3DType( magFO );
    m_pd3dDevice->SetSamplerState( nStage,D3DSAMP_MAGFILTER,filter );
    filter = FlyD3DUtil::GetD3DType( minFO );
    m_pd3dDevice->SetSamplerState( nStage,D3DSAMP_MINFILTER,filter );
    filter = FlyD3DUtil::GetD3DType( mipFO );
    m_pd3dDevice->SetSamplerState( nStage,D3DSAMP_MIPFILTER,filter );

    // Save the render states.
    m_Samplers[nStage].MagFilter = magFO;
    m_Samplers[nStage].MinFilter = minFO;
    m_Samplers[nStage].MipFilter = mipFO;
}


//------------------------------------------------------------------------
// Set the matrix for the texture coords.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Which stage to set the matrix.
//      const FlyMatrix*:   Pointer to the matrix.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetTextureMatrix( int nStage,const FlyMatrix* mat )
{
    D3DMATRIX d3dmat;

    if( nStage < 0 || nStage >= MAX_TEXTURES ) return;

    if( !mat || !m_pActiveMaterial->IsFixedPipeline() )
    {
        m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE );
        return;
    }

    switch( m_Samplers[nStage].pTexture->GetTextureType() )
    {
    case TT_2D:
        m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );
        break;
    case TT_3D:
    case TT_CUBEMAP:
        m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT3 );
        break;
    }

    memcpy( &d3dmat,mat,sizeof(D3DMATRIX) );
    m_pd3dDevice->SetTransform( (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0+nStage),&d3dmat );
}


//------------------------------------------------------------------------
// Set addressing mode for each stage of texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Which stage to set the addressing mode.
//      TEXTUREADDRESS:     Texture-Address mode for U coords.
//      TEXTUREADDRESS:     Texture-Address mode for V coords.
//      TEXTUREADDRESS:     Texture-Address mode for W coords.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetTextureAddressingMode( int nStage,TEXTUREADDRESS addrU,TEXTUREADDRESS addrV,
                                                   TEXTUREADDRESS addrW )
{
    D3DTEXTUREADDRESS address;

    address = FlyD3DUtil::GetD3DType( addrU );
    m_pd3dDevice->SetSamplerState( nStage,D3DSAMP_ADDRESSU,address );
    address = FlyD3DUtil::GetD3DType( addrV );
    m_pd3dDevice->SetSamplerState( nStage,D3DSAMP_ADDRESSV,address );
    address = FlyD3DUtil::GetD3DType( addrW );
    m_pd3dDevice->SetSamplerState( nStage,D3DSAMP_ADDRESSW,address );

    // Save the render states.
    m_Samplers[nStage].AddressU = addrU;
    m_Samplers[nStage].AddressV = addrV;
    m_Samplers[nStage].AddressW = addrW;
}


//------------------------------------------------------------------------
// Set the blending mode for each stage of texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Which stage to set the blending mode.
//      TEXTUREBLENDMODE:   Texture blending mode.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetTextureBlendingMode( int nStage,const TEXTUREBLENDMODE& mode )
{
    DWORD dwFactor;
    DWORD dwArg;
    D3DTEXTUREOP top;

    if( mode.Type == BT_COLOR )
    {
        if( mode.Operation == TOP_BLENDMANUAL )
        {
            dwFactor = MAKE3DCOLOR( 0,0,0,int(mode.fFactor*255) );
            m_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,dwFactor );

            dwArg = FlyD3DUtil::GetD3DType( mode.Arg1 );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_COLORARG1,dwArg );
            dwArg = FlyD3DUtil::GetD3DType( mode.Arg2 );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_COLORARG2,dwArg );

            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_COLOROP,D3DTOP_BLENDFACTORALPHA );
        }
        else
        {
            dwArg = FlyD3DUtil::GetD3DType( mode.Arg1 );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_COLORARG1,dwArg );
            dwArg = FlyD3DUtil::GetD3DType( mode.Arg2 );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_COLORARG2,dwArg );

            top = FlyD3DUtil::GetD3DType( mode.Operation );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_COLOROP,top );
        }

        // Save the render states.
        memcpy( &m_RenderStates.ColorBlend[nStage],&mode,sizeof(TEXTUREBLENDMODE) );
    }
    else
    {
        if( mode.Operation == TOP_BLENDMANUAL )
        {
            dwFactor = MAKE3DCOLOR( 0,0,0,int(mode.fFactor*255) );
            m_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,dwFactor );

            dwArg = FlyD3DUtil::GetD3DType( mode.Arg1 );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_ALPHAARG1,dwArg );
            dwArg = FlyD3DUtil::GetD3DType( mode.Arg2 );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_ALPHAARG2,dwArg );

            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_ALPHAOP,D3DTOP_BLENDFACTORALPHA );
        }
        else
        {
            dwArg = FlyD3DUtil::GetD3DType( mode.Arg1 );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_ALPHAARG1,dwArg );
            dwArg = FlyD3DUtil::GetD3DType( mode.Arg2 );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_ALPHAARG2,dwArg );

            top = FlyD3DUtil::GetD3DType( mode.Operation );
            m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_ALPHAOP,top );
        }

        // Save the render states.
        memcpy( &m_RenderStates.AlphaBlend[nStage],&mode,sizeof(TEXTUREBLENDMODE) );
    }
}


//------------------------------------------------------------------------
// Set the texture coords index.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Which stage to set the coord index.
//      int:        Index of the texture coords.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetTextureCoordIndex( int nStage,int nCoordIndex )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES ) return;

    m_pd3dDevice->SetTextureStageState( nStage,D3DTSS_TEXCOORDINDEX,nCoordIndex );
    m_RenderStates.CoordSetIndex[nStage] = nCoordIndex;
}


//------------------------------------------------------------------------
// Set the lighting mode for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Open or close the light effect.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetLightingMode( bool bLighting )
{
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,bLighting ? TRUE : FALSE );

    m_RenderStates.bLighting = bLighting;
}


//------------------------------------------------------------------------
// Open or close the specular light.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetSpecularMode( bool bSpecular )
{
    m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE,bSpecular ? TRUE : FALSE );

    m_RenderStates.bSpecularLight = bSpecular;
}


//------------------------------------------------------------------------
// Set the light effect for the stage.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Which stage to set the light.
//      const FLYLIGHT*:    Pointer to the light.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetLight( int nStage,const FLYLIGHT* pLight )
{
    D3DLIGHT9 light;

    if( nStage < -1 || nStage >= MAX_LIGHTS )
        return;

    // Cancel the light of the stage.
    if( !pLight )
    {
        m_pd3dDevice->LightEnable( nStage,FALSE );

        m_bLightEnable[nStage] = false;
        return;
    }

    // Set the light for the stage.
    memset( &light,0,sizeof(D3DLIGHT9) );

    switch( pLight->Type )
    {
    case LGT_POINT:
        light.Type = D3DLIGHT_POINT;

        light.Range = pLight->fRange;
        light.Attenuation0 = 1.0f;
        light.Position.x = pLight->vPosition.x;
        light.Position.y = pLight->vPosition.y;
        light.Position.z = pLight->vPosition.z;
        break;
    case LGT_DIRECTIONAL:
        light.Type = D3DLIGHT_DIRECTIONAL;

        light.Direction.x = pLight->vDirection.x;
        light.Direction.y = pLight->vDirection.y;
        light.Direction.z = pLight->vDirection.z;
        break;
    case LGT_SPOT:
        light.Type = D3DLIGHT_SPOT;

        light.Range = pLight->fRange;
        light.Falloff = 1.0f;
        light.Phi = pLight->fPhi;
        light.Theta = pLight->fTheta;
        light.Attenuation0 = pLight->fAttenuation0;
        light.Attenuation1 = pLight->fAttenuation1;
        light.Attenuation2 = 1.0f;

        light.Position.x = pLight->vPosition.x;
        light.Position.y = pLight->vPosition.y;
        light.Position.z = pLight->vPosition.z;

        light.Direction.x = pLight->vDirection.x;
        light.Direction.y = pLight->vDirection.y;
        light.Direction.z = pLight->vDirection.z;
        break;
    }

    // Copy the color values.
    memcpy( &light.Ambient,&pLight->cAmbient,sizeof(float)*4 );
    memcpy( &light.Diffuse,&pLight->cDiffuse,sizeof(float)*4 );
    memcpy( &light.Specular,&pLight->cSpecular,sizeof(float)*4 );

    m_pd3dDevice->SetLight( nStage,&light );
    m_pd3dDevice->LightEnable( nStage,TRUE );

    // Save the render states.
    memcpy( &m_Lights[nStage],pLight,sizeof(FLYLIGHT) );
}


//------------------------------------------------------------------------
// Set the standard material for the renderdevice.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FLYMATERIAL*:     Pointer to the standard material.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetStdMaterial( const FLYMATERIAL* pMaterial )
{
    D3DMATERIAL9 mat;

    if( !pMaterial )
    {
        m_pd3dDevice->SetMaterial( NULL );
        return;
    }

    memcpy( &mat.Ambient,&pMaterial->cAmbient,sizeof(FLYCOLOR) );
    memcpy( &mat.Diffuse,&pMaterial->cDiffuse,sizeof(FLYCOLOR) );
    memcpy( &mat.Specular,&pMaterial->cSpecular,sizeof(FLYCOLOR) );
    memcpy( &mat.Emissive,&pMaterial->cEmissive,sizeof(FLYCOLOR) );
    mat.Power = pMaterial->fPower;

    m_pd3dDevice->SetMaterial( &mat );

    // Save the render states.
    memcpy( &m_RenderStates.StdMaterial,pMaterial,sizeof(FLYMATERIAL) );
}


//------------------------------------------------------------------------
// Set the depth buffer mode for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      DEPTHMODE:      Depth buffer mode for the render device.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetDepthBufferMode( DEPTHMODE mode )
{
    switch( mode )
    {
    case DEPTH_NONE:
        m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,FALSE );
        break;
    case DEPTH_READWRITE:
        m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,TRUE );
        break;
    case DEPTH_READONLY:
        m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,FALSE );
        break;
    }

    // Save the render states.
    m_RenderStates.DepthMode = mode;
}


//------------------------------------------------------------------------
// Set the depth compare function and the value.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC:     Use which func to compare.
//      float:          Compare value for the depth buffer.
//      float:          Slope scale value for the depth buffer.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetDepthFunc( FLYCMPFUNC cmpFunc,float fBias,float fSlopeScale )
{
    m_pd3dDevice->SetRenderState( D3DRS_ZFUNC,cmpFunc+1 );
    m_pd3dDevice->SetRenderState( D3DRS_SLOPESCALEDEPTHBIAS,CONVERT2DWORD(fSlopeScale) );
    m_pd3dDevice->SetRenderState( D3DRS_DEPTHBIAS,CONVERT2DWORD(fBias) );

    // Save the render states.
    m_RenderStates.DepthFunc = cmpFunc;
    m_RenderStates.fDepthBias = fBias;
    m_RenderStates.fSlopeScale = fSlopeScale;
}


//------------------------------------------------------------------------
// Set the stencil buffer mode for the device.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Open the front stencil ?
//      bool:       Open the back stencil ?
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetStencilMode( bool bFront,bool bBack )
{
    if( bFront && bBack )
    {
        m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE,TRUE );
    }
    else
    {
        if( bFront )
            m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,TRUE );
        else
        {
            if( bBack )
            {
                m_pd3dDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE,TRUE );
                m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC,D3DCMP_ALWAYS );
            }
            else
            {
                m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,FALSE );
                m_pd3dDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE,FALSE );
            }
        }
    }

    m_RenderStates.bFrontStencilEnable = bFront;
    m_RenderStates.bBackStencilEnable = bBack;
}


//------------------------------------------------------------------------
// Set the stencil func for the device.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC:     Compare function of the front side.
//      FLYCMPFUNC:     Compare function of the back side.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetStencilFunc( FLYCMPFUNC frontFunc,FLYCMPFUNC backFunc )
{
    m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC,frontFunc+1 );
    m_pd3dDevice->SetRenderState( D3DRS_CCW_STENCILFUNC,backFunc+1 );

    m_RenderStates.StencilFunc = frontFunc;
    m_RenderStates.BackStencilFunc = backFunc;
}


//------------------------------------------------------------------------
// Set the stencil operation for the device. (Front side)
// ----------------------------------------------------------------------
// Param -> IN:
//      STENCILOPERATION:   Operation when test failed.
//      STENCILOPERATION:   Operation when test passed and z-Test failed.
//      STENCILOPERATION:   Operation when both test and z-Test passed.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetStencilOperationF( STENCILOPERATION failOp,STENCILOPERATION zFailOp,
                                               STENCILOPERATION passOp )
{
    D3DSTENCILOP d3dOp;

    d3dOp = FlyD3DUtil::GetD3DType( failOp );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILFAIL,d3dOp );
    d3dOp = FlyD3DUtil::GetD3DType( zFailOp );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL,d3dOp );
    d3dOp = FlyD3DUtil::GetD3DType( passOp );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS,d3dOp );

    m_RenderStates.StencilFail = failOp;
    m_RenderStates.StencilDepthFail = zFailOp;
    m_RenderStates.StencilPass = passOp;
}


//------------------------------------------------------------------------
// Set the stencil operation for the device. (Back side)
// ----------------------------------------------------------------------
// Param -> IN:
//      STENCILOPERATION:   Operation when test failed.
//      STENCILOPERATION:   Operation when test passed and z-Test failed.
//      STENCILOPERATION:   Operation when both test and z-Test passed.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetStencilOperationB( STENCILOPERATION failOp,STENCILOPERATION zFailOp,
                                               STENCILOPERATION passOp )
{
    D3DSTENCILOP d3dOp;

    d3dOp = FlyD3DUtil::GetD3DType( failOp );
    m_pd3dDevice->SetRenderState( D3DRS_CCW_STENCILFAIL,d3dOp );
    d3dOp = FlyD3DUtil::GetD3DType( zFailOp );
    m_pd3dDevice->SetRenderState( D3DRS_CCW_STENCILZFAIL,d3dOp );
    d3dOp = FlyD3DUtil::GetD3DType( passOp );
    m_pd3dDevice->SetRenderState( D3DRS_CCW_STENCILPASS,d3dOp );

    m_RenderStates.BackStencilFail = failOp;
    m_RenderStates.BackStencilDepthFail = zFailOp;
    m_RenderStates.BackStencilPass = passOp;
}


//------------------------------------------------------------------------
// Set the stencil reference value.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetStencilRef( int nStencilRef )
{
    m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,CONVERT2DWORD(nStencilRef) );

    m_RenderStates.nStencilRef = nStencilRef;
}


//------------------------------------------------------------------------
// Set the stencil mask value for device.
// ----------------------------------------------------------------------
// Param -> IN:
//      DWORD:      Stencil mask value (Front side).
//      DWORD:      Stencil mask value (Back side).
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetStencilMask( DWORD dwMask,DWORD dwWriteMask )
{
    m_pd3dDevice->SetRenderState( D3DRS_STENCILMASK,dwMask );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK,dwWriteMask );

    m_RenderStates.dwStencilMask = dwMask;
    m_RenderStates.dwStencilWriteMask = dwWriteMask;
}


//------------------------------------------------------------------------
// Set the cull mode for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      CULLMODE:       Culling mode for the render device.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetCullMode( CULLMODE mode )
{
    switch( mode )
    {
    case CULL_NONE:
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,D3DCULL_NONE );
        break;
    case CULL_CW:
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,D3DCULL_CW );
        break;
    case CULL_CCW:
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,D3DCULL_CCW );
        break;
    }

    // Store the render states.
    m_RenderStates.CullMode = mode;
}


//------------------------------------------------------------------------
// Set the shader mode for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      SHADEMODE:      Shader mode for the render device.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetShadeMode( SHADEMODE mode )
{
    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    switch( mode )
    {
    case SM_FLAT:
        m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_FLAT );
        break;
    case SM_GOURAUD:
        m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_GOURAUD );
        break;
    }

    // Store the render states
    m_RenderStates.ShadeMode = mode;
}


//------------------------------------------------------------------------
// Set the fill mode for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      POLYGONMODE:    Polygon mode for the render device.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetPolygonMode( POLYGONMODE mode )
{
    switch( mode )
    {
    case PM_POINT:
        m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_POINT );
        break;
    case PM_WIREFRAME:
        m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_WIREFRAME );
        break;
    case PM_SOLID:
        m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_SOLID );
        break;
    }

    // Save the render states.
    m_RenderStates.PolygonMode = mode;
}


//------------------------------------------------------------------------
// Set the scene blend mode for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Open or close the alpha blend ?
//      bool:       Open or close the alpha test ?
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetSceneAlphaMode( bool bAlphaBlend,bool bAlphaTest )
{
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,static_cast<DWORD>(bAlphaBlend) );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,static_cast<DWORD>(bAlphaTest) );

    // Save the render states.
    m_RenderStates.bAlphaBlend = bAlphaBlend;
    m_RenderStates.bAlphaTest = bAlphaTest;
}


//------------------------------------------------------------------------
// Set the alpha test compare function and compare value.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC:     Use which compare function ?
//      DWORD:          Compare value for alpha test.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetAlphaTestFunc( FLYCMPFUNC cmpFunc,DWORD alphaRef )
{
    m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,cmpFunc+1 );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,alphaRef );

    // Save the render states.
    m_RenderStates.AlphaTestFunc = cmpFunc;
    m_RenderStates.dwAlphaTestRef = alphaRef;
}


//------------------------------------------------------------------------
// Set the alpha blend mode for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      SCENEBLENDMODE:     Source blend mode.
//      SCENEBLENDMODE:     Destination blend mode.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetAlphaBlendMode( SCENEBLENDMODE src,SCENEBLENDMODE dest )
{
    D3DBLEND blend;

    if( src == SBM_ONE && dest == SBM_ZERO )
    {
        m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,D3DBLEND_ONE );
        m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ZERO );

        m_RenderStates.bAlphaBlend = FALSE;
        m_RenderStates.SourceBlend = SBM_ONE;
        m_RenderStates.DestBlend = SBM_ZERO;
    }
    else
    {
        m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE );

        blend = FlyD3DUtil::GetD3DType( src );
        m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,blend );
        blend = FlyD3DUtil::GetD3DType( dest );
        m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,blend );

        // Save the render states.
        m_RenderStates.SourceBlend = src;
        m_RenderStates.DestBlend = dest;
    }
}



//------------------------------------------------------------------------
// Get the specified stage's texture pointer.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Stage index to get the texture pointer.
//------------------------------------------------------------------------
const FlyTexture* FlyD3DRenderDevice::GetTexture( int nStage ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return NULL;

    return m_Samplers[nStage].pTexture;
}


//------------------------------------------------------------------------
// Get the sampler stage states of the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Stage index to get the sampler states.
//      FILTEROPTION*:  To store the mag filter.
//      FILTEROPTION*:  To store the min filter.
//      FILTEROPTION*:  To store the mip filter.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetTextureFilterMode( int nStage,FILTEROPTION* pMagFO,FILTEROPTION* pMinFO,
                                               FILTEROPTION* pMipFO ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    *pMagFO = m_Samplers[nStage].MagFilter;
    *pMinFO = m_Samplers[nStage].MinFilter;
    *pMipFO = m_Samplers[nStage].MipFilter;
}


//------------------------------------------------------------------------
// Get the addressing mode for the sampler stage.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Stage index to get the addressing mode.
//      TEXTUREADDRESS*:    To store the addressing U mode.
//      TEXTUREADDRESS*:    To store the addressing V mode.
//      TEXTUREADDRESS*:    To store the addressing W mode.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetTextureAddressingMode( int nStage,TEXTUREADDRESS* pAddrU,TEXTUREADDRESS* pAddrV,
                                                   TEXTUREADDRESS* pAddrW ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    *pAddrU = m_Samplers[nStage].AddressU;
    *pAddrV = m_Samplers[nStage].AddressV;
    *pAddrW = m_Samplers[nStage].AddressW;
}


//------------------------------------------------------------------------
// Get the texture blending mode for the texture stage.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Stage index to get the blending mode.
//      BLENDTYPE:          Which blending type, Color or Alpha ?
//      TEXTUREBLENDMODE*:  Pointer to the texture blend mode.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetTextureBlendingMode( int nStage,BLENDTYPE Type,TEXTUREBLENDMODE* pMode ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    if( Type == BT_COLOR )
        memcpy( pMode,&m_RenderStates.ColorBlend[nStage],sizeof(TEXTUREBLENDMODE) );
    else
        memcpy( pMode,&m_RenderStates.AlphaBlend[nStage],sizeof(TEXTUREBLENDMODE) );
}


//------------------------------------------------------------------------
// Get the texture stage's coord index.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Stage index to get the coord index.
//------------------------------------------------------------------------
int FlyD3DRenderDevice::GetTextureCoordIndex( int nStage ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return -1;

    return m_RenderStates.CoordSetIndex[nStage];
}


//------------------------------------------------------------------------
// Get the alpha blend mode and alpha test mode.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool*:      To store the alpha blend mode.
//      bool*:      To store the alpha test mode.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetSceneAlphaMode( bool* pbAlphaBlend,bool* pbAlphaTest ) const
{
    *pbAlphaBlend = m_RenderStates.bAlphaBlend;
    *pbAlphaTest = m_RenderStates.bAlphaTest;
}


//------------------------------------------------------------------------
// Get the alpha test compare func and ref value.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC*:    To store the alpha test compare func.
//      DWORD*:         To store the alpha test compare value.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetAlphaTestFunc( FLYCMPFUNC* pCmpFunc,DWORD* pAlphaRef ) const
{
    *pCmpFunc = m_RenderStates.AlphaTestFunc;
    *pAlphaRef = m_RenderStates.dwAlphaTestRef;
}


//------------------------------------------------------------------------
// Get the texture blending mode factors.
// ----------------------------------------------------------------------
// Param -> IN:
//      SCENEBLENDMODE*:    To store the source blend factor.
//      SCENEBLENDMODE*:    To store the dest blend factor.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetAlphaBlendMode( SCENEBLENDMODE* pSrc,SCENEBLENDMODE* pDest ) const
{
    *pSrc = m_RenderStates.SourceBlend;
    *pDest = m_RenderStates.DestBlend;
}


//------------------------------------------------------------------------
// Get the lighting mode of the render device.
//------------------------------------------------------------------------
bool FlyD3DRenderDevice::GetLightingMode(void) const
{
    return m_RenderStates.bLighting;
}

//------------------------------------------------------------------------
// Do we open the specular light ?
//------------------------------------------------------------------------
bool FlyD3DRenderDevice::GetSpecularMode(void) const
{
    return m_RenderStates.bSpecularLight;
}


//------------------------------------------------------------------------
// Get the specified light stuff data.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        The stage index to get the light stuff.
//      FLYLIGHT*:  To store the light stuff data.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetLight( int nStage,FLYLIGHT* pLight ) const
{
    if( nStage < 0 || nStage >= MAX_LIGHTS )
        return;

    memcpy( pLight,&m_Lights[nStage],sizeof(FLYLIGHT) );
}


//------------------------------------------------------------------------
// Get the standard material of the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYMATERIAL*:   To store the standard material.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetStdMaterial( FLYMATERIAL* pMaterial ) const
{
    memcpy( pMaterial,&m_RenderStates.StdMaterial,sizeof(FLYMATERIAL) );
}


//------------------------------------------------------------------------
// Get the depth buffer mode of the render device.
//------------------------------------------------------------------------
DEPTHMODE FlyD3DRenderDevice::GetDepthBufferMode(void) const
{
    return m_RenderStates.DepthMode;
}


//------------------------------------------------------------------------
// Get the depth buffer mode of the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC*:    To store the compare function.
//      float*:         To store the depth bias.
//      float*:         To store the depth slope scale value.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetDepthFunc( FLYCMPFUNC* pCmpFunc,float* pfBias,float* pfSlopeScale ) const
{
    *pCmpFunc = m_RenderStates.DepthFunc;
    *pfBias = m_RenderStates.fDepthBias;
    *pfSlopeScale = m_RenderStates.fSlopeScale;
}


//------------------------------------------------------------------------
// Get the stencil mode. (Front and back side)
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetStencilMode( bool* pbStencilEnableF,bool* pbStencilEnableB ) const
{
    *pbStencilEnableF = m_RenderStates.bFrontStencilEnable;
    *pbStencilEnableB = m_RenderStates.bBackStencilEnable;
}


//------------------------------------------------------------------------
// Get the stencil compare function.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetStencilFunc( FLYCMPFUNC* pFuncF,FLYCMPFUNC* pFuncB ) const
{
    *pFuncF = m_RenderStates.StencilFunc;
    *pFuncB = m_RenderStates.BackStencilFunc;
}

//------------------------------------------------------------------------
// Get the stencil reference value.
//------------------------------------------------------------------------
int FlyD3DRenderDevice::GetStencilRef(void) const
{
    return m_RenderStates.nStencilRef;
}


//------------------------------------------------------------------------
// Get the stencil mask value. (Front and back side)
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetStencilMask( DWORD* pMask,DWORD* pWriteMask ) const
{
    *pMask = m_RenderStates.dwStencilMask;
    *pWriteMask = m_RenderStates.dwStencilWriteMask;
}


//------------------------------------------------------------------------
// Get the stencil operation. (Front side).
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetStencilOperationF( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,
                                               STENCILOPERATION* pPass ) const
{
    *pFail = m_RenderStates.StencilFail;
    *pZFail = m_RenderStates.StencilDepthFail;
    *pPass = m_RenderStates.StencilPass;
}


//------------------------------------------------------------------------
// Get the stencil operation. (Back side).
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetStencilOperationB( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,
                                               STENCILOPERATION* pPass ) const
{
    *pFail = m_RenderStates.BackStencilFail;
    *pZFail = m_RenderStates.BackStencilDepthFail;
    *pPass = m_RenderStates.BackStencilPass;
}


//------------------------------------------------------------------------
// Get the cull mode of the render device.
//------------------------------------------------------------------------
CULLMODE FlyD3DRenderDevice::GetCullMode(void) const
{
    return m_RenderStates.CullMode;
}


//------------------------------------------------------------------------
// Get the polygon mode of the render device.
//------------------------------------------------------------------------
POLYGONMODE FlyD3DRenderDevice::GetPolygonMode(void) const
{
    return m_RenderStates.PolygonMode;
}


//------------------------------------------------------------------------
// Get the shade mode of the render device.
//------------------------------------------------------------------------
SHADEMODE FlyD3DRenderDevice::GetShadeMode(void) const
{
    return m_RenderStates.ShadeMode;
}


//------------------------------------------------------------------------
// Get the ambient color of the render device.
//------------------------------------------------------------------------
const FLYCOLOR& FlyD3DRenderDevice::GetAmbientColor(void) const
{
    return m_cAmbient;
}


//------------------------------------------------------------------------
// Get the fog mode, color, density, near distance, far distance.
// ----------------------------------------------------------------------
// Param -> IN:
//      FOGMODE*:       To store the fog mode.
//      FLYCOLOR*:      To store the fog color.
//      float*:         To store the fog densify.
//      float*:         To store the fog near distance.
//      float*:         To store the fog far distance.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::GetFog( FOGMODE* pMode,FLYCOLOR* pClr,float* pfDensity,
                                 float* pfNear,float* pfFar ) const
{
    *pMode = m_FogMode;
    *pClr = m_cFogColor;
    *pfDensity = m_fFogDensity;
    *pfNear = m_fFogNear;
    *pfFar = m_fFogFar;
}


//------------------------------------------------------------------------
// Set the shader object for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyShaderObject*:   Pointer to the shader object.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::SetShaderObject( FlyShaderObject* pShader )
{
    if( !pShader )
    {
        m_pd3dDevice->SetVertexShader( NULL );
        m_pd3dDevice->SetPixelShader( NULL );
        m_pActiveShader = NULL;
    }
    else
    {
        FlyD3DShaderObject* pD3DShader = static_cast<FlyD3DShaderObject*>(pShader);
        m_pd3dDevice->SetVertexShader( pD3DShader->GetD3DVertexShaderPtr() );
        m_pd3dDevice->SetPixelShader( pD3DShader->GetD3DPixelShaderPtr() );
        m_pActiveShader = pShader;
    }
}


//------------------------------------------------------------------------
// Transfer the 2D position on the screen to the 3D world.
// ----------------------------------------------------------------------
// Param -> IN:
//      const POINT&:   Position on the screen.
//      FlyVector*:     To Store the orig of the ray.
//      FlyVector*:     To store the direction of the ray.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::Transfer2DTo3D( const POINT& pt,FlyVector* pOrig,FlyVector* pDir )
{
    FlyVector vS;
    DWORD dwWidth,dwHeight;
    FlyMatrix mInvView;

    // Scale to viewport and inverse projection
    if( m_nActiveStage == -1 )
    {
        dwWidth = m_d3dpp.BackBufferWidth;
        dwHeight = m_d3dpp.BackBufferHeight;
        vS.x = (((2.0f * pt.x)/dwWidth)-1) / m_mProjection._11;
        vS.y = -(((2.0f * pt.y)/dwHeight)-1) / m_mProjection._22;
        vS.z = 1.0f;
    }
    else
    {
        dwWidth = m_pWindows[m_nActiveStage]->GetBackWidth();
        dwHeight = m_pWindows[m_nActiveStage]->GetBackHeight();
        vS.x = (((2.0f * pt.x)/dwWidth)-1) / m_mSubProj[m_nActiveStage]._11;
        vS.y = -(((2.0f * pt.y)/dwHeight)-1) / m_mSubProj[m_nActiveStage]._22;
        vS.z = 1.0f;
    }

    mInvView.InverseOf( m_mView3D );

    // Transform the screen space pick ray into 3D space
    pDir->x = vS.x*mInvView._11 + vS.y*mInvView._21 + vS.z*mInvView._31;
    pDir->y = vS.x*mInvView._12 + vS.y*mInvView._22 + vS.z*mInvView._32;
    pDir->z = vS.x*mInvView._13 + vS.y*mInvView._23 + vS.z*mInvView._33;
    pOrig->x = mInvView._41;
    pOrig->y = mInvView._42;
    pOrig->z = mInvView._43;
}


//------------------------------------------------------------------------
// Transfer the 3D world position to the screen coords.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position in the 3D world.
//------------------------------------------------------------------------
POINT FlyD3DRenderDevice::Transfer3DTo2D( const FlyVector& vPosition )
{
    POINT pt;
    float fClip_x,fClip_y;
    float fXp,fYp,fWp;
    DWORD dwWidth,dwHeight;
    FlyMatrix mViewProj;

    if( m_nActiveStage == -1 )
    {
        dwWidth = m_d3dpp.BackBufferWidth;
        dwHeight = m_d3dpp.BackBufferHeight;
        mViewProj = m_mView3D * m_mProjection;
    }
    else
    {
        dwWidth = m_pWindows[m_nActiveStage]->GetBackWidth();
        dwHeight = m_pWindows[m_nActiveStage]->GetBackHeight();
        mViewProj = m_mView3D * m_mSubProj[m_nActiveStage];
    }

    fClip_x = (float)(dwWidth >> 1);
    fClip_y = (float)(dwHeight >> 1);

    fXp = (mViewProj._11*vPosition.x) + (mViewProj._21*vPosition.y) +
          (mViewProj._31*vPosition.z) + mViewProj._41;
    fYp = (mViewProj._12*vPosition.x) + (mViewProj._22*vPosition.y) +
          (mViewProj._32*vPosition.z) + mViewProj._42;
    fWp = (mViewProj._14*vPosition.x) + (mViewProj._24*vPosition.y) +
          (mViewProj._34*vPosition.z) + mViewProj._44;

    float fWpInv = 1.0f / fWp;

    // Converting from [-1,1] to viewport size
    pt.x = (LONG)( (1.0f+fXp*fWpInv)*fClip_x );
    pt.y = (LONG)( (1.0f+fYp*fWpInv)*fClip_y );

    return pt;
}


//------------------------------------------------------------------------
// Get the depth of the specified pixel.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:   X- coordinates of the pixel.
//      UINT:   Y- coordinates of the pixel.
//------------------------------------------------------------------------
float FlyD3DRenderDevice::GetPixelDepth( UINT px,UINT py ) const
{
    D3DLOCKED_RECT d3dRect;
    D3DSURFACE_DESC desc;
    IDirect3DSurface9* pDepthStencil;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    if( px >= m_d3dpp.BackBufferWidth ||
        py >= m_d3dpp.BackBufferHeight )
        return 0.0f;

    // Get the surface of the depth/stencil buffer.
    if( FAILED(m_pd3dDevice->GetDepthStencilSurface(&pDepthStencil)) )
    {
        logMgr.WriteError( 0,"Get the depth/stencil buffer failed! GetPixelDepth()" );
        return 0.0f;
    }

    pDepthStencil->GetDesc( &desc );
    if( desc.Format != D3DFMT_D16_LOCKABLE ||
        desc.Format != D3DFMT_D32F_LOCKABLE )
        return 0.0f;

    // Lock the depth/stencil buffer.
    if( FAILED(pDepthStencil->LockRect(&d3dRect,NULL,D3DLOCK_READONLY)) )
    {
        logMgr.WriteError( 0,"Lock the depth/stencil buffer failed! GetPixelDepth()" );
        return 0.0f;
    }

    DWORD dwValue;
    if( desc.Format == D3DFMT_D16_LOCKABLE )
        dwValue = static_cast<WORD*>(d3dRect.pBits)[py*d3dRect.Pitch+px];
    else
        dwValue = static_cast<DWORD*>(d3dRect.pBits)[py*d3dRect.Pitch+px];

    pDepthStencil->UnlockRect();
    pDepthStencil->Release();
    return DWORD2FLOAT( dwValue );
}


//------------------------------------------------------------------------
// To store the device caps for the render device.
// ----------------------------------------------------------------------
// Param -> IN:
//      D3DCAPS9*:      Pointer to the Direct3D device caps.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::StoreDeviceCaps( D3DCAPS9* pCaps )
{
    m_DevCaps.dwVShaderVersion = pCaps->VertexShaderVersion;
    m_DevCaps.dwPShaderVersion = pCaps->PixelShaderVersion;

    m_DevCaps.nMaxTextures = pCaps->MaxTextureBlendStages;
    if( m_DevCaps.nMaxTextures > MAX_TEXTURES ) m_DevCaps.nMaxTextures = MAX_TEXTURES;

    m_DevCaps.nMaxPrimitiveCount = pCaps->MaxPrimitiveCount;
    m_DevCaps.nMaxStreamStride = pCaps->MaxStreamStride;
    m_DevCaps.dwMaxTextureWidth = pCaps->MaxTextureWidth;
    m_DevCaps.dwMaxTextureHeight = pCaps->MaxTextureHeight;
    m_DevCaps.bAlphaTexture = (pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHA) ? true : false;
    m_DevCaps.bCubeMap = (pCaps->TextureCaps & D3DPTEXTURECAPS_CUBEMAP) ? true : false;
    m_DevCaps.bMipCubeMap = (pCaps->TextureCaps & D3DPTEXTURECAPS_MIPCUBEMAP) ? true : false;
    m_DevCaps.bMipMap = (pCaps->TextureCaps & D3DPTEXTURECAPS_MIPMAP) ? true : false;
    m_DevCaps.bMipVolumeMap = (pCaps->TextureCaps & D3DPTEXTURECAPS_MIPVOLUMEMAP) ? true : false;
    m_DevCaps.bVolumeMap = (pCaps->TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP) ? true : false;
    m_DevCaps.bTableFog = (pCaps->RasterCaps & D3DPRASTERCAPS_FOGTABLE) ? true : false;
    m_DevCaps.bVertexFog = (pCaps->RasterCaps & D3DPRASTERCAPS_FOGVERTEX) ? true : false;
    m_DevCaps.bTwoSideStencil = (pCaps->StencilCaps & D3DSTENCILCAPS_TWOSIDED) ? true : false;

    if( FAILED(m_pD3D->CheckDeviceFormat(pCaps->AdapterOrdinal,pCaps->DeviceType,D3DFMT_X8R8G8B8,
                                         D3DUSAGE_QUERY_VERTEXTEXTURE,D3DRTYPE_TEXTURE,
                                         D3DFMT_A32B32G32R32F)) )
    {
        m_DevCaps.nMaxVertexTextures = 4;
    }
    else
    {
        m_DevCaps.nMaxVertexTextures = 0;
    }
}


//------------------------------------------------------------------------
// Log the device caps for the RenderDevice.
// ----------------------------------------------------------------------
// Param -> IN:
//      D3DCAPS9*:      Pointer to the Direct3D device caps.
//------------------------------------------------------------------------
void FlyD3DRenderDevice::LogDeviceCaps( D3DCAPS9* pCaps )
{
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    logMgr.WriteLine( 0,"Adapter Info: {" );

    if( pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
    {
        if( pCaps->DevCaps & D3DDEVCAPS_PUREDEVICE )
            logMgr.WriteLine( 0,"    Adapter support Hardware Tnl (pure device possible)" );
        else
            logMgr.WriteLine( 0,"    Adapter support Hardware Tnl (No pure device possible)" );
    }
    else
    {
        logMgr.WriteLine( 0,"    Adapter only support Software Tnl" );
    }

    // TEXTURE STUFF
    logMgr.WriteLine( 0,"    Max. Texture stages: %d",pCaps->MaxTextureBlendStages );
    logMgr.WriteLine( 0,"    Max. Textures for single pass: %d",pCaps->MaxSimultaneousTextures );
    logMgr.WriteLine( 0,"    Max. Texture width: %d",pCaps->MaxTextureWidth );
    logMgr.WriteLine( 0,"    Max. Texture height: %d",pCaps->MaxTextureHeight );

    // VERTEX SHADER VERSION
    if( pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
        logMgr.WriteLine( 0,"    Vertex Shader Version 1.0" );
    else if( pCaps->VertexShaderVersion < D3DVS_VERSION(2,0) )
        logMgr.WriteLine( 0,"    Vertex Shader Version 1.1" );
    else
        logMgr.WriteLine( 0,"    Vertex Shader Version 2.0 or better" );

    // PIXEL SHADER VERSION
    if( pCaps->PixelShaderVersion < D3DPS_VERSION(1,1) )
        logMgr.WriteLine( 0,"    Pixel Shader Version 1.0" );
    else if( pCaps->PixelShaderVersion < D3DPS_VERSION(1,2) )
        logMgr.WriteLine( 0,"    Pixel Shader Version 1.1" );
    else if( pCaps->PixelShaderVersion < D3DPS_VERSION(1,3) )
        logMgr.WriteLine( 0,"    Pixel Shader Version 1.2" );
    else if( pCaps->PixelShaderVersion < D3DPS_VERSION(1,4) )
        logMgr.WriteLine( 0,"    Pixel Shader Version 1.3" );
    else if( pCaps->PixelShaderVersion < D3DPS_VERSION(2,0) )
        logMgr.WriteLine( 0,"    Pixel Shader Version 1.4" );
    else
        logMgr.WriteLine( 0,"    Pixel Shader Version 2.0 or better" );

    logMgr.WriteLine( 0,"} ENDINFO" );

    // SCREEN RESOLUTION, FORMAT AND STUFF
    logMgr.WriteLine( 0,"Display info: {" );
    LPDIRECT3DSURFACE9 pDepthStencil = NULL;
    D3DSURFACE_DESC desc;
    D3DFORMAT Format = D3DFMT_UNKNOWN;
    D3DDISPLAYMODE mode;

    if( FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&mode)) )
        logMgr.WriteError( 0,"IDirect3D::GetAdapterDisplayMode() failed! " );

    if( FAILED(m_pd3dDevice->GetDepthStencilSurface(&pDepthStencil)) )
        logMgr.WriteError( 0,"IDirect3DDevice::GetDepthStencilSurface() failed! " );
    else
    {
        if( FAILED(pDepthStencil->GetDesc(&desc)) )
        {
            logMgr.WriteError( 0,"IDirect3DSurface::GetDesc() failed! " );
            Format = D3DFMT_UNKNOWN;
        }
    }

    logMgr.WriteLine( 0,"    Resolution: %d x %d",mode.Width,mode.Height );
    logMgr.WriteLine( 0,"    RefreshRate: %d",mode.RefreshRate );

    // Backbuffer format
    switch( mode.Format )
    {
    case D3DFMT_A2R10G10B10:    logMgr.WriteLine( 0,"    PixelFormat: D3DFMT_A2R10G10B10" );  break;
    case D3DFMT_A8R8G8B8:       logMgr.WriteLine( 0,"    PixelFormat: D3DFMT_A8R8G8B8" );     break;
    case D3DFMT_X8R8G8B8:       logMgr.WriteLine( 0,"    PixelFormat: D3DFMT_X8R8G8B8" );     break;
    case D3DFMT_A1R5G5B5:       logMgr.WriteLine( 0,"    PixelFormat: D3DFMT_A1R5G5B5" );     break;
    case D3DFMT_X1R5G5B5:       logMgr.WriteLine( 0,"    PixelFormat: D3DFMT_X1R5G5B5" );     break;
    case D3DFMT_R5G6B5:         logMgr.WriteLine( 0,"    PixelFormat: D3DFMT_R5G6B5" );       break;
    default:                    logMgr.WriteLine( 0,"    Unknown format!" );                  break;
    }

    // Depth buffer format
    switch( desc.Format )
    {
    case D3DFMT_D16_LOCKABLE:   logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D16_LOCKABLE" );   break;
    case D3DFMT_D32F_LOCKABLE:  logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D32F_LOCKABLE" );  break;
    case D3DFMT_D32:            logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D32" );            break;
    case D3DFMT_D15S1:          logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D15S1" );          break;
    case D3DFMT_D24S8:          logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D24S8" );          break;
    case D3DFMT_D24X8:          logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D24X8" );          break;
    case D3DFMT_D24X4S4:        logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D24X4S4" );        break;
    case D3DFMT_D24FS8:         logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D24FS8" );         break;
    case D3DFMT_D16:            logMgr.WriteLine( 0,"    Depth/Stencil: D3DFMT_D16" );            break;
    default:                    logMgr.WriteLine( 0,"    Unknown format!" );                      break;
    }

    SAFE_RELEASE( pDepthStencil );
    logMgr.WriteLine( 0,"} ENDINFO" );
}


//------------------------------------------------------------------------
// Set the Render state for the render state.
// ----------------------------------------------------------------------
// Param -> IN:
//      D3DRENDERSTATETYPE:     Render state type.
//      DWORD:                  Value of the render state.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::_SetRenderState( D3DRENDERSTATETYPE State,DWORD dwValue )
{
    DWORD oldValue;

    m_pd3dDevice->GetRenderState( State,&oldValue );

    if( oldValue == dwValue )
        return FLY_OK;
    else
        return m_pd3dDevice->SetRenderState( State,dwValue );
}


//------------------------------------------------------------------------
// Set the texture sampler filter type.
// ----------------------------------------------------------------------
// Param -> IN:
//      DWORD:                  Index of the sampler stage.
//      D3DSAMPLERSTATETYPE:    Samplter state type.
//      DWORD:                  Value of the render state.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::_SetSamplerState( DWORD nSampler,D3DSAMPLERSTATETYPE Type,DWORD dwValue )
{
    DWORD oldValue;

    m_pd3dDevice->GetSamplerState( nSampler,Type,&oldValue );

    if( oldValue == dwValue )
        return FLY_OK;
    else
        return m_pd3dDevice->SetSamplerState( nSampler,Type,dwValue );
}


//------------------------------------------------------------------------
// Set the texture stage state.
// ----------------------------------------------------------------------
// Param -> IN:
//      DWORD:                      Index of the texture stage.
//      D3DTEXTURESTAGESTATETYPE:   Texture stage state type.
//      DWORD:                      Value of the texture stage state.
//------------------------------------------------------------------------
HRESULT FlyD3DRenderDevice::_SetTextureStageState( DWORD nStage,D3DTEXTURESTAGESTATETYPE Type,
                                                   DWORD dwValue )
{
    DWORD oldValue;

    m_pd3dDevice->GetTextureStageState( nStage,Type,&oldValue );

    if( oldValue == dwValue )
        return FLY_OK;
    else
        return m_pd3dDevice->SetTextureStageState( nStage,Type,dwValue );
}
*/