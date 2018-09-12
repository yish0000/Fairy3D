// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "ProgressDlg.h"
#include "FileDupDlg.h"


// CProgressDlg 对话框

CProgressDlg* CProgressDlg::s_pThis = NULL;

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
    , m_szCurFile(_T(""))
{
    m_szCurFile = "";
    m_iCurProgress = 0;
    m_nFileCount = 0;
    m_nFileIndex = 0;
    m_bRunning = false;
    m_pExp = NULL;
    m_iOperationType = -1;
    m_FileList = NULL;

    m_dwThreadRet = 0;
    m_hTheadProc = NULL;
    ::InitializeCriticalSection( &m_csLock );
    s_pThis = this;
}

CProgressDlg::~CProgressDlg()
{
    if( m_hTheadProc )
    {
        CloseHandle( m_hTheadProc );
        m_hTheadProc = NULL;
    }

    ::DeleteCriticalSection( &m_csLock );
    s_pThis = NULL;
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CURRENTFILE, m_szCurFile);
    DDX_Control(pDX, IDC_PROGRESS1, m_ctlProgress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
    ON_BN_CLICKED(IDCANCEL, &CProgressDlg::OnBnClickedCancel)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressDlg 消息处理程序

BOOL CProgressDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_bRunning = true;
    m_ctlProgress.SetRange( 0,1000 );
    SetTimer( 0,50,NULL );

    switch( m_iOperationType )
    {
    case FAIRY_OP_ADDFILE:
        SetWindowText( _T("添加文件") );
        break;
    case FAIRY_OP_EXTRACT:
        SetWindowText( _T("解压文件") );
        break;
    case FAIRY_OP_OPEN:
        {
            SetWindowText( _T("FairyPacker") );
            GetDlgItem(IDCANCEL)->EnableWindow( FALSE );
            m_szCurFile.FormatMessage(IDS_OPENWAIT0);
            UpdateData(FALSE);

            // 使关闭按钮不可用
            CMenu* sysMenu = GetSystemMenu(FALSE);
            sysMenu->EnableMenuItem( SC_CLOSE,MF_DISABLED );
            SetTimer(1, 200, NULL);
        }
        break;
    case FAIRY_OP_DEFRAG:
        SetWindowText( _T("整理碎片") );
        break;
    }

    // 创建线程并开始
    m_hTheadProc = ::CreateThread( NULL,0,ProcessProc,this,0,NULL );
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

// 处理文件时的回调函数
bool CProgressDlg::FPKCallback( const AString& filename )
{
    CProgressDlg* dlg = CProgressDlg::s_pThis;
    ::EnterCriticalSection( &dlg->m_csLock );

    if( dlg->m_bRunning && dlg->m_nFileCount != 0 )
    {
        float fProgess = (float)dlg->m_nFileIndex / dlg->m_nFileCount;
        dlg->m_szCurFile = F_UTF8ToANSI(filename).c_str();
        dlg->m_iCurProgress = (int)(1000 * fProgess);
    }

    ++dlg->m_nFileIndex;
    ::LeaveCriticalSection( &dlg->m_csLock );
    return dlg->m_bRunning;
}

class CFileDupHandler : public IFileDupHandler
{
public:

	enum
	{
		ALL_YES = 1,
		ALL_NO,
	};

public:
	CFileDupHandler() : m_iState(0) {}

	void Reset() { m_iState = 0; }
	int GetState() const { return m_iState; }
	void SetState(int iState) { m_iState = iState; }

	// If return true, we will replace the duplicated file.
	bool Handle(const AString& filename)
	{
		if( m_iState == ALL_YES )
			return true;
		else if( m_iState == ALL_NO )
			return false;

		CFileDupDlg dlg(F_UTF8ToUnicode(filename).c_str());
		if( dlg.DoModal() == CFileDupDlg::OP_YES )
			return true;
		else if( dlg.DoModal() == CFileDupDlg::OP_YESALL )
		{
			m_iState = ALL_YES;
			return true;
		}
		else if( dlg.DoModal() == CFileDupDlg::OP_NO )
			return false;
		else if( dlg.DoModal() == CFileDupDlg::OP_NOALL )
		{
			m_iState = ALL_NO;
			return false;
		}

		return false;
	}

protected:
	int m_iState;
};

// 对话框的线程函数
DWORD CProgressDlg::ProcessProc( LPVOID lpParameter )
{
	CFileDupHandler dupHandler;
    CProgressDlg* dlg = (CProgressDlg*)lpParameter;
    dlg->m_pExp->SetCallback( FPKCallback );
	dlg->m_pExp->SetFileDupHandler(&dupHandler);
    dlg->m_dwThreadRet = 0;

    FFilePairList::iterator it;
    for( it=dlg->m_FileList->begin();it!=dlg->m_FileList->end();++it )
    {
        switch( dlg->m_iOperationType )
        {
        case FAIRY_OP_ADDFILE:
            dlg->m_pExp->Add( it->first,it->second );
            break;
        case FAIRY_OP_EXTRACT:
            dlg->m_pExp->Extract( it->first,it->second );
            break;
        case FAIRY_OP_OPEN:
            if( !dlg->m_pExp->Open(it->first) )
            {
                dlg->MessageBox( _T("加载文件包失败！"),_T("错误"),MB_OK|MB_ICONWARNING );
                dlg->m_dwThreadRet = 1;
            }
            break;
        case FAIRY_OP_DEFRAG:
            if( !dlg->m_pExp->Defrag() )
                dlg->m_dwThreadRet = 1;
            break;
        }
    }

    dlg->m_bRunning = false;
    return dlg->m_dwThreadRet;
}

void CProgressDlg::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    ::EnterCriticalSection( &m_csLock );
    m_bRunning = false;
    m_szCurFile = _T("正在取消...");
    UpdateData( FALSE );
    ::LeaveCriticalSection( &m_csLock );

    ::WaitForSingleObject( m_hTheadProc,INFINITE );
    CDialog::OnCancel();
}

void CProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CDialog::OnTimer(nIDEvent);

    if( nIDEvent == 1 )
    {
        static int strID = IDS_OPENWAIT1;
        m_szCurFile.FormatMessage(strID);
        if( (++strID) > IDS_OPENWAIT5 ) strID = IDS_OPENWAIT0;
        UpdateData(FALSE);
        return;
    }

    ::EnterCriticalSection( &m_csLock );
    m_ctlProgress.SetPos( m_iCurProgress );
    UpdateData( FALSE );
    ::LeaveCriticalSection( &m_csLock );
    if( !m_bRunning ) CDialog::OnCancel();
}