/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9RenderWindow.h
 *  Desc:   本文件为D3D9渲染模块定义了一个渲染窗口对象。
 *  Author: Yish
 *  Date:   2011/7/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_RENDERWINDOW_H__
#define __FD3D9_RENDERWINDOW_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FD3D9Common.h"
#include "F3DRenderWindow.h"

//////////////////////////////////////////////////////////////////////////


/** D3D9渲染窗口对象
*/
class FD3D9RenderWindow : public F3DRenderWindow
{
protected:
    FD3D9RenderDevice* m_pDevice;       // D3D9渲染设备指针
    HWND m_hWnd;                        // 渲染窗口的句柄
    DWORD m_dwStyle;                    // 渲染窗口的风格
    D3DPRESENT_PARAMETERS m_d3dpp;      // 设备的参数

    LPDIRECT3DDEVICE9 m_pd3dDevice;     // Direct3D设备接口(如果是主窗口则需此指针)
    LPDIRECT3DSWAPCHAIN9 m_pChain;      // 该窗口的SwapChain
    LPDIRECT3DSURFACE9 m_pBackBuffer;   // 渲染窗口的后台缓冲区
    LPDIRECT3DSURFACE9 m_pZStencil;     // 深度/模板缓冲区

public:
    // 构造函数和析构函数
    FD3D9RenderWindow( const FString& name,bool bMainWnd,FD3D9RenderDevice* pDevice );
    ~FD3D9RenderWindow(void);

    // 创建该渲染窗口
    void Create( const SWindowInfo& info );

    // 重新设置渲染窗口的大小
    void ResizeWindow( uint32 width,uint32 height );

    // 将渲染窗口设为全屏模式
    bool SetFullScreen( bool bFullScreen );

    // 更新渲染窗口
    void Update(void);

    // 交换前后缓冲区
    void SwapBuffers(void);

    // 截取当前屏幕保存到文件
    bool PrintScreenToFile( const FString& filename,bool bFront );

    // 获取渲染窗口的后台缓冲区
    LPDIRECT3DSURFACE9 _GetBackBuffer(void) const { return m_pBackBuffer; }

    // 获取渲染窗口的深度/模板缓冲区
    LPDIRECT3DSURFACE9 _GetDepthStencilBuffer(void) const { return m_pZStencil; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_RENDERWINDOW_H__