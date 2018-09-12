/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9ConfigDialog.h
 *  Desc:   ���ļ�Ϊ��ģ�鶨����һ�����öԻ������
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


/** ��Ⱦ�豸��������Ϣ
*/
struct SDeviceSettings
{
    uint32 nAdapterID;              // ��ѡ��������ID
    uint32 nWidth,nHeight;          // �����ڷֱ���
    bool bFullscreen;               // ȫ��ģʽ?
    bool bColor16Bit;               // 16λ��ɫ?
    bool bVSync;                    // �Ƿ�����ֱͬ��
    D3DMULTISAMPLE_TYPE msType;     // ���������
    DWORD dwQuality;                // �����ǿ��
};

/** D3D9ͼ��ģ�����öԻ���
*/
class FD3D9ConfigDialog
{
private:
    HINSTANCE m_hModule;            // ��ģ���ʵ�����
    SDeviceSettings m_Settings;     // ��Ⱦ�豸����
    FD3D9Enumerator* m_pEnum;       // D3D9�豸ö����

    // ���öԻ���Ļص�����
    static INT CALLBACK MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );

public:
    // ���캯��/��������
    FD3D9ConfigDialog( LPDIRECT3D9 pD3D );
    ~FD3D9ConfigDialog(void);

    // �������öԻ���
    bool ShowDialog(void);

    // ��ȡ�豸��������Ϣ
    const SDeviceSettings& GetDeviceSettings(void) const { return m_Settings; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_CONFIGDIALOG_H__