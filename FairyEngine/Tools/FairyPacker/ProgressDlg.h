#pragma once
#include "afxcmn.h"


// CProgressDlg 对话框

class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgressDlg();

// 对话框数据
	enum { IDD = IDD_DLG_PROGRESS };

    // 线程函数
    static DWORD WINAPI ProcessProc( LPVOID lpParameter );

    // 回调函数
    static bool FPKCallback( const AString& filename );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString m_szCurFile;            // 当前处理的文件名
    CProgressCtrl m_ctlProgress;    // 进度条控件
    int m_iCurProgress;             // 当前的进度
    UINT m_nFileCount;              // 处理的文件个数
    UINT m_nFileIndex;              // 当前处理文件的索引
    bool m_bRunning;                // 当前是否在处理
    CFairyExplorer* m_pExp;         // 压缩包浏览器指针
    int m_iOperationType;           // 当前操作的类型
    FFilePairList* m_FileList;      // 要处理的文件的列表

    DWORD m_dwThreadRet;            // 线程的返回值
    HANDLE m_hTheadProc;            // 处理数据的后台线程
    CRITICAL_SECTION m_csLock;

    static CProgressDlg* s_pThis;

    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
