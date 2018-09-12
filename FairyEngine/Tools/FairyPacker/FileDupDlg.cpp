// FileDupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "FileDupDlg.h"


// CFileDupDlg dialog

IMPLEMENT_DYNAMIC(CFileDupDlg, CDialog)

CFileDupDlg::CFileDupDlg(const CString& strMsg, CWnd* pParent /*=NULL*/)
	: CDialog(CFileDupDlg::IDD, pParent)
{
	m_strDupMsg = strMsg;
}

CFileDupDlg::~CFileDupDlg()
{
}

void CFileDupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DUPINFO, m_ctlDupMsg);
}


BEGIN_MESSAGE_MAP(CFileDupDlg, CDialog)
	ON_BN_CLICKED(IDC_DUP_YES, &CFileDupDlg::OnBnClickedYes)
	ON_BN_CLICKED(IDC_DUP_YESALL, &CFileDupDlg::OnBnClickedYesall)
	ON_BN_CLICKED(IDC_DUP_NO, &CFileDupDlg::OnBnClickedNo)
	ON_BN_CLICKED(IDC_DUP_NOALL, &CFileDupDlg::OnBnClickedNoall)
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CFileDupDlg message handlers

void CFileDupDlg::OnBnClickedYes()
{
	// TODO: Add your control notification handler code here
	EndDialog(OP_YES);
}

void CFileDupDlg::OnBnClickedYesall()
{
	// TODO: Add your control notification handler code here
	EndDialog(OP_YESALL);
}

void CFileDupDlg::OnBnClickedNo()
{
	// TODO: Add your control notification handler code here
	EndDialog(OP_NO);
}

void CFileDupDlg::OnBnClickedNoall()
{
	// TODO: Add your control notification handler code here
	EndDialog(OP_NOALL);
}

void CFileDupDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	EndDialog(OP_NO);
	CDialog::OnClose();
}

int CFileDupDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CString strText;
	strText.Format(_T("此位置已经包含同名文件（%s）！\n\n确定要覆盖吗？"), m_strDupMsg);
	m_ctlDupMsg.SetWindowText(strText);
	return 0;
}
