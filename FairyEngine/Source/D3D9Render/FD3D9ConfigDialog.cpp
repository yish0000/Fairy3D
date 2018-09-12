/*
 * -----------------------------------------------------------------------
 *  Name:   FD3D9ConfigDialog.cpp
 *  Desc:   ���ļ�Ϊ��Ⱦ�豸ģ�鶨����һ���Ի������
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


/** ���ָ������Ͽ����Ƿ����ָ����
*/
static bool ContainItem( HWND hComboBox,const TCHAR* strItem )
{
    TCHAR text[80];

    // ��ȡ��Ͽ���ĸ���
    UINT n = (UINT)SendMessage( hComboBox,CB_GETCOUNT,0,0 );

    for( UINT i=0;i<n;i++ )
    {
        SendMessage( hComboBox,CB_GETLBTEXT,i,(LPARAM)text );

        if( F_Strcmp(text,strItem) == 0 )
            return true;
    }

    return false;
}

// ��̬ʵ��ָ��
static FD3D9ConfigDialog* l_pDialog = NULL;

/** ���캯��
*/
FD3D9ConfigDialog::FD3D9ConfigDialog( LPDIRECT3D9 pD3D )
{
    // ��ȡ��ģ���ʵ�����
#ifdef FAIRY_DEBUG
    m_hModule = GetModuleHandle( _T("D3D9Render_d.dll") );
#else
    m_hModule = GetModuleHandle( _T("D3D9Render.dll") );
#endif

    // ����ö����
    m_pEnum = new FD3D9Enumerator( pD3D );
	l_pDialog = this;
}

/** ��������
*/
FD3D9ConfigDialog::~FD3D9ConfigDialog(void)
{
    F_SAFE_DELETE( m_pEnum );
	l_pDialog = NULL;
}

/** �������öԻ���
*/
bool FD3D9ConfigDialog::ShowDialog(void)
{
    // ö�ٵ�ǰ�����Կ�
    m_pEnum->EnumAdapters();

    // �����Ի���
    return (1 == DialogBox(m_hModule,MAKEINTRESOURCE(IDD_DLG_SETTINGS),NULL,MsgProc));
}

/** ���öԻ���Ļص�����
@Param �Ի���ľ��
@Param ������Ϣ
@Param ������Ϣ�ĸ��Ӳ���1
@Param ������Ϣ�ĸ��Ӳ���2
*/
INT FD3D9ConfigDialog::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    uint32 n;
    FD3D9Adapter* pAdapter;
    const D3DDISPLAYMODE* pMode;
    char strTemp[80];

    // ��ȡ�ؼ��ľ��
    HWND hAdapterList = GetDlgItem( hWnd,IDC_ADAPTERLIST );
    HWND hFullscreen = GetDlgItem( hWnd,IDC_FULLSCREEN );
    HWND hVideoMode = GetDlgItem( hWnd,IDC_VIDEOMODE );
    HWND hMSQuality = GetDlgItem( hWnd,IDC_MSQUALITY );
    HWND hVSync = GetDlgItem( hWnd,IDC_VSYNC );

    // ��ȡö����
	FD3D9Enumerator* pEnum = l_pDialog->m_pEnum;

    switch( uMsg )
    {
    case WM_INITDIALOG:
        // ����������б�
        for( n=0;n<pEnum->GetAdapterCount();n++ )
        {
            pAdapter = pEnum->GetAdapter( n );
            SendMessage( hAdapterList,CB_ADDSTRING,0,(LPARAM)pAdapter->GetDescription().Description );
        }

        // ȫ��ģʽ
        SendMessage( hFullscreen,CB_ADDSTRING,0,(LPARAM)("Open") );
        SendMessage( hFullscreen,CB_ADDSTRING,0,(LPARAM)("Close") );

        // ��ֱͬ��
        SendMessage( hVSync,CB_ADDSTRING,0,(LPARAM)("Open") );
        SendMessage( hVSync,CB_ADDSTRING,0,(LPARAM)("Close") );

        // ö��Ĭ���Կ�����ʾģʽ(Ĭ��Ϊ32λ��ɫ)
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

        // ö�ٿ��õĿ����ģʽ
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

        // ѡ��Ĭ����ɫģʽ
        SendDlgItemMessage( hWnd,IDC_COLOR32BIT,BM_SETCHECK,BST_CHECKED,0 );
        SendDlgItemMessage( hWnd,IDC_COLOR16BIT,BM_SETCHECK,BST_UNCHECKED,0 );

        // ��ȾĬ����
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

                // ��ȡ��ǰ��ѡ������
                n = (UINT)SendMessage( hAdapterList,CB_GETCURSEL,0,0 );
                pAdapter = pEnum->GetAdapter( n );

                // ö���Կ�����ʾģʽ(Ĭ��Ϊ32λ��ɫ)
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

                // ö�ٿ��õĿ����ģʽ
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

                // ��ѡ������ID
                settings.nAdapterID = (uint32)SendMessage( hAdapterList,CB_GETCURSEL,0,0 );

                // �Ƿ�ȫ��ģʽ
                if( SendMessage(hFullscreen,CB_GETCURSEL,0,0) == 0 )
                    settings.bFullscreen = true;
                else
                    settings.bFullscreen = false;

                // ��ѡ��ʾģʽ
                n = (UINT)SendMessage( hVideoMode,CB_GETCURSEL,0,0 );
                SendMessage( hVideoMode,CB_GETLBTEXT,n,(LPARAM)strTemp );
                sscanf( strTemp,"%d x %d",&settings.nWidth,&settings.nHeight );

                // ��ɫ����
                if( SendDlgItemMessage(hWnd,IDC_COLOR16BIT,BM_GETCHECK,0,0) ==
                    BST_CHECKED )
                    settings.bColor16Bit = true;
                else
                    settings.bColor16Bit = false;

                // ���������
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

                // ��ֱͬ��
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