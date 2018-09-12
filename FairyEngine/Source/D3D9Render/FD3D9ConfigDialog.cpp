/*
 * -----------------------------------------------------------------------
 *  Name:   FD3D9ConfigDialog.cpp
 *  Desc:   本文件为渲染设备模块定义了一个对话框对象。
 *  Author: Yish
 *  Date:   2011/7/9
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "res/resource.h"
#include "FIniFile.h"
#include "FD3D9Enumerator.h"
#include "FD3D9ConfigDialog.h"
#include "FD3D9Util.h"


/** 检测指定的组合框内是否包含指定项
*/
static bool ContainItem( HWND hComboBox,const TCHAR* strItem )
{
    TCHAR text[80];

    // 获取组合框项的个数
    UINT n = (UINT)SendMessage( hComboBox,CB_GETCOUNT,0,0 );

    for( UINT i=0;i<n;i++ )
    {
        SendMessage( hComboBox,CB_GETLBTEXT,i,(LPARAM)text );

        if( F_Strcmp(text,strItem) == 0 )
            return true;
    }

    return false;
}

// 静态实例指针
static FD3D9ConfigDialog* l_pDialog = NULL;

/** 构造函数
*/
FD3D9ConfigDialog::FD3D9ConfigDialog( LPDIRECT3D9 pD3D )
{
    // 获取本模块的实例句柄
#ifdef FAIRY_DEBUG
    m_hModule = GetModuleHandle( _T("D3D9Render_d.dll") );
#else
    m_hModule = GetModuleHandle( _T("D3D9Render.dll") );
#endif

    // 创建枚举器
    m_pEnum = new FD3D9Enumerator( pD3D );
	l_pDialog = this;
}

/** 析构函数
*/
FD3D9ConfigDialog::~FD3D9ConfigDialog(void)
{
    F_SAFE_DELETE( m_pEnum );
	l_pDialog = NULL;
}

/** 弹出配置对话框
*/
bool FD3D9ConfigDialog::ShowDialog(void)
{
    // 枚举当前可用显卡
    m_pEnum->EnumAdapters();

    // 弹出对话框
    return (1 == DialogBox(m_hModule,MAKEINTRESOURCE(IDD_DLG_SETTINGS),NULL,MsgProc));
}

/** 配置对话框的回调函数
@Param 对话框的句柄
@Param 窗口消息
@Param 窗口消息的附加参数1
@Param 窗口消息的附加参数2
*/
INT FD3D9ConfigDialog::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    uint32 n;
    FD3D9Adapter* pAdapter;
    const D3DDISPLAYMODE* pMode;
    char strTemp[80];

    // 获取控件的句柄
    HWND hAdapterList = GetDlgItem( hWnd,IDC_ADAPTERLIST );
    HWND hFullscreen = GetDlgItem( hWnd,IDC_FULLSCREEN );
    HWND hVideoMode = GetDlgItem( hWnd,IDC_VIDEOMODE );
    HWND hMSQuality = GetDlgItem( hWnd,IDC_MSQUALITY );
    HWND hVSync = GetDlgItem( hWnd,IDC_VSYNC );

    // 获取枚举器
	FD3D9Enumerator* pEnum = l_pDialog->m_pEnum;

    switch( uMsg )
    {
    case WM_INITDIALOG:
        // 添加适配器列表
        for( n=0;n<pEnum->GetAdapterCount();n++ )
        {
            pAdapter = pEnum->GetAdapter( n );
            SendMessage( hAdapterList,CB_ADDSTRING,0,(LPARAM)pAdapter->GetDescription().Description );
        }

        // 全屏模式
        SendMessage( hFullscreen,CB_ADDSTRING,0,(LPARAM)("Open") );
        SendMessage( hFullscreen,CB_ADDSTRING,0,(LPARAM)("Close") );

        // 垂直同步
        SendMessage( hVSync,CB_ADDSTRING,0,(LPARAM)("Open") );
        SendMessage( hVSync,CB_ADDSTRING,0,(LPARAM)("Close") );

        // 枚举默认显卡的显示模式(默认为32位颜色)
        pAdapter = pEnum->GetAdapter( 0 );
        pAdapter->EnumVideoMode( pEnum->GetD3DPtr(),D3DFMT_X8R8G8B8 );
        for( n=0;n<pAdapter->GetVideoModeCount();n++ )
        {
            pMode = &pAdapter->GetVideoMode( n );

            if( FD3D9Util::GetFormatBits(pMode->Format) == 32 )
            {
                _snprintf( strTemp,80,"%d x %d",pMode->Width,pMode->Height );
                SendMessage( hVideoMode,CB_ADDSTRING,0,(LPARAM)strTemp );
            }
        }

        // 枚举可用的抗锯齿模式
        pAdapter->EnumAntiAliasing( pEnum->GetD3DPtr(),D3DFMT_X8R8G8B8 );
        SendMessage( hMSQuality,CB_ADDSTRING,0,(LPARAM)("Close") );
        for( n=0;n<pAdapter->GetMultiSampleCount();n++ )
        {
            DWORD dwQuality;
            D3DMULTISAMPLE_TYPE msType;
            pAdapter->GetMultiSample( n,msType,dwQuality );

            if( msType == D3DMULTISAMPLE_NONMASKABLE )
            {
                _snprintf( strTemp,80,"NonMaskable(%d)",dwQuality );
                SendMessage( hMSQuality,CB_ADDSTRING,0,(LPARAM)strTemp );
            }
            else
            {
                SendMessage( hMSQuality,CB_ADDSTRING,0,(LPARAM)FD3D9Util::ToString(msType) );
            }
        }

        // 选择默认颜色模式
        SendDlgItemMessage( hWnd,IDC_COLOR32BIT,BM_SETCHECK,BST_CHECKED,0 );
        SendDlgItemMessage( hWnd,IDC_COLOR16BIT,BM_SETCHECK,BST_UNCHECKED,0 );

        // 渲染默认项
        SendMessage( hAdapterList,CB_SETCURSEL,0,0 );
        SendMessage( hFullscreen,CB_SETCURSEL,0,0 );
        SendMessage( hVSync,CB_SETCURSEL,0,0 );
        SendMessage( hVideoMode,CB_SETCURSEL,0,0 );
        SendMessage( hMSQuality,CB_SETCURSEL,0,0 );
        break;
    case WM_CLOSE:
        EndDialog( hWnd,0 );
        break;
    case WM_COMMAND:
        switch( LOWORD(wParam) )
        {
        case IDC_ADAPTERLIST:
        case IDC_COLOR16BIT:
        case IDC_COLOR32BIT:
            {
                D3DFORMAT fmt;
                if( SendDlgItemMessage(hWnd,IDC_COLOR16BIT,BM_GETCHECK,0,0) ==
                    BST_CHECKED )
                    fmt = D3DFMT_R5G6B5;
                else
                    fmt = D3DFMT_X8R8G8B8;

                SendMessage( hVideoMode,CB_RESETCONTENT,0,0 );
                SendMessage( hMSQuality,CB_RESETCONTENT,0,0 );

                // 获取当前所选适配器
                n = (UINT)SendMessage( hAdapterList,CB_GETCURSEL,0,0 );
                pAdapter = pEnum->GetAdapter( n );

                // 枚举显卡的显示模式(默认为32位颜色)
                pAdapter = pEnum->GetAdapter( 0 );
                pAdapter->EnumVideoMode( pEnum->GetD3DPtr(),fmt );
                for( n=0;n<pAdapter->GetVideoModeCount();n++ )
                {
                    pMode = &pAdapter->GetVideoMode( n );

                    if( FD3D9Util::GetFormatBits(pMode->Format) == 32 )
                    {
                        _snprintf( strTemp,80,"%d x %d",pMode->Width,pMode->Height );
                        SendMessage( hVideoMode,CB_ADDSTRING,0,(LPARAM)strTemp );
                    }
                }

                // 枚举可用的抗锯齿模式
                pAdapter->EnumAntiAliasing( pEnum->GetD3DPtr(),fmt );
                SendMessage( hMSQuality,CB_ADDSTRING,0,(LPARAM)"Close" );
                for( n=0;n<pAdapter->GetMultiSampleCount();n++ )
                {
                    DWORD dwQuality;
                    D3DMULTISAMPLE_TYPE msType;
                    pAdapter->GetMultiSample( n,msType,dwQuality );

                    if( msType == D3DMULTISAMPLE_NONMASKABLE )
                    {
                        _snprintf( strTemp,80,"NonMaskable(%d)",dwQuality );
                        SendMessage( hMSQuality,CB_ADDSTRING,0,(LPARAM)strTemp );
                    }
                    else
                    {
                        SendMessage( hMSQuality,CB_ADDSTRING,0,(LPARAM)FD3D9Util::ToString(msType) );
                    }
                }

                SendMessage( hVideoMode,CB_SETCURSEL,0,0 );
                SendMessage( hMSQuality,CB_SETCURSEL,0,0 );
                break;
            }
        case IDOK:
            {
                SDeviceSettings& settings = FD3D9ConfigDialog::s_pThis->m_Settings;

                // 所选适配器ID
                settings.nAdapterID = (uint32)SendMessage( hAdapterList,CB_GETCURSEL,0,0 );

                // 是否全屏模式
                if( SendMessage(hFullscreen,CB_GETCURSEL,0,0) == 0 )
                    settings.bFullscreen = true;
                else
                    settings.bFullscreen = false;

                // 所选显示模式
                n = (UINT)SendMessage( hVideoMode,CB_GETCURSEL,0,0 );
                SendMessage( hVideoMode,CB_GETLBTEXT,n,(LPARAM)strTemp );
                sscanf( strTemp,"%d x %d",&settings.nWidth,&settings.nHeight );

                // 颜色质量
                if( SendDlgItemMessage(hWnd,IDC_COLOR16BIT,BM_GETCHECK,0,0) ==
                    BST_CHECKED )
                    settings.bColor16Bit = true;
                else
                    settings.bColor16Bit = false;

                // 抗锯齿质量
                n = (UINT)SendMessage( hMSQuality,CB_GETCURSEL,0,0 );
                SendMessage( hMSQuality,CB_GETLBTEXT,n,(LPARAM)strTemp );
                if( strcmp(strTemp,"None") == 0 )
                {
                    settings.msType = D3DMULTISAMPLE_NONE;
                    settings.dwQuality = 0;
                }
                else if( strstr(strTemp,"(") )
                {
                    settings.msType = D3DMULTISAMPLE_NONMASKABLE;
                    sscanf( strTemp,"NonMaskable(%d)",&settings.dwQuality );
                }
                else
                {
                    settings.msType = FD3D9Util::StrToMSType( strTemp );
                    settings.dwQuality = 0;
                }

                // 垂直同步
                if( SendMessage(hVSync,CB_GETCURSEL,0,0) == 0 )
                    settings.bVSync = true;
                else
                    settings.bVSync = false;

                EndDialog( hWnd,1 );
                break;
            }
        case IDCANCEL:
            EndDialog( hWnd,0 );
            break;
        }
        break;
    }

    return 0;
}