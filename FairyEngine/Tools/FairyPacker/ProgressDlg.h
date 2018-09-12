#pragma once
#include "afxcmn.h"


// CProgressDlg �Ի���

class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgressDlg();

// �Ի�������
	enum { IDD = IDD_DLG_PROGRESS };

    // �̺߳���
    static DWORD WINAPI ProcessProc( LPVOID lpParameter );

    // �ص�����
    static bool FPKCallback( const AString& filename );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CString m_szCurFile;            // ��ǰ������ļ���
    CProgressCtrl m_ctlProgress;    // �������ؼ�
    int m_iCurProgress;             // ��ǰ�Ľ���
    UINT m_nFileCount;              // ������ļ�����
    UINT m_nFileIndex;              // ��ǰ�����ļ�������
    bool m_bRunning;                // ��ǰ�Ƿ��ڴ���
    CFairyExplorer* m_pExp;         // ѹ���������ָ��
    int m_iOperationType;           // ��ǰ����������
    FFilePairList* m_FileList;      // Ҫ������ļ����б�

    DWORD m_dwThreadRet;            // �̵߳ķ���ֵ
    HANDLE m_hTheadProc;            // �������ݵĺ�̨�߳�
    CRITICAL_SECTION m_csLock;

    static CProgressDlg* s_pThis;

    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
