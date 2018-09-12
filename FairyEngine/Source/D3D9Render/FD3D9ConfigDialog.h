/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9ConfigDialog.h
 *  Desc:   本文件为本模块定义了一个配置对话框对象。
 *  Author: Yish
 *  Date:   2011/7/6
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_CONFIGDIALOG_H__
#define __FD3D9_CONFIGDIALOG_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Common.h"

///////////////////////////////////////////////////////////////////////////


/** 渲染设备的配置信息
*/
struct SDeviceSettings
{
    uint32 nAdapterID;              // 所选的适配器ID
    uint32 nWidth,nHeight;          // 主窗口分辨率
    bool bFullscreen;               // 全屏模式?
    bool bColor16Bit;               // 16位颜色?
    bool bVSync;                    // 是否开启垂直同步
    D3DMULTISAMPLE_TYPE msType;     // 抗锯齿类型
    DWORD dwQuality;                // 抗锯齿强度
};

/** D3D9图形模块配置对话框
*/
class FD3D9ConfigDialog
{
private:
    HINSTANCE m_hModule;            // 本模块的实例句柄
    SDeviceSettings m_Settings;     // 渲染设备配置
    FD3D9Enumerator* m_pEnum;       // D3D9设备枚举器

    // 配置对话框的回调函数
    static INT CALLBACK MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

public:
    // 构造函数/析构函数
    FD3D9ConfigDialog( LPDIRECT3D9 pD3D );
    ~FD3D9ConfigDialog(void);

    // 弹出配置对话框
    bool ShowDialog(void);

    // 获取设备的配置信息
    const SDeviceSettings& GetDeviceSettings(void) const { return m_Settings; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_CONFIGDIALOG_H__