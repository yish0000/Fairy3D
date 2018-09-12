/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9RenderWindow.h
 *  Desc:   ���ļ�ΪD3D9��Ⱦģ�鶨����һ����Ⱦ���ڶ���
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


/** D3D9��Ⱦ���ڶ���
*/
class FD3D9RenderWindow : public F3DRenderWindow
{
protected:
    FD3D9RenderDevice* m_pDevice;       // D3D9��Ⱦ�豸ָ��
    HWND m_hWnd;                        // ��Ⱦ���ڵľ��
    DWORD m_dwStyle;                    // ��Ⱦ���ڵķ��
    D3DPRESENT_PARAMETERS m_d3dpp;      // �豸�Ĳ���

    LPDIRECT3DDEVICE9 m_pd3dDevice;     // Direct3D�豸�ӿ�(����������������ָ��)
    LPDIRECT3DSWAPCHAIN9 m_pChain;      // �ô��ڵ�SwapChain
    LPDIRECT3DSURFACE9 m_pBackBuffer;   // ��Ⱦ���ڵĺ�̨������
    LPDIRECT3DSURFACE9 m_pZStencil;     // ���/ģ�建����

public:
    // ���캯������������
    FD3D9RenderWindow( const FString& name,bool bMainWnd,FD3D9RenderDevice* pDevice );
    ~FD3D9RenderWindow(void);

    // ��������Ⱦ����
    void Create( const SWindowInfo& info );

    // ����������Ⱦ���ڵĴ�С
    void ResizeWindow( uint32 width,uint32 height );

    // ����Ⱦ������Ϊȫ��ģʽ
    bool SetFullScreen( bool bFullScreen );

    // ������Ⱦ����
    void Update(void);

    // ����ǰ�󻺳���
    void SwapBuffers(void);

    // ��ȡ��ǰ��Ļ���浽�ļ�
    bool PrintScreenToFile( const FString& filename,bool bFront );

    // ��ȡ��Ⱦ���ڵĺ�̨������
    LPDIRECT3DSURFACE9 _GetBackBuffer(void) const { return m_pBackBuffer; }

    // ��ȡ��Ⱦ���ڵ����/ģ�建����
    LPDIRECT3DSURFACE9 _GetDepthStencilBuffer(void) const { return m_pZStencil; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_RENDERWINDOW_H__