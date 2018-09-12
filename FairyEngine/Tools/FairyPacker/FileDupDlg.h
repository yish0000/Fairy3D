#pragma once
#include "afxwin.h"


// CFileDupDlg dialog

class CFileDupDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileDupDlg)

public:

	enum
	{
		OP_YES,
		OP_YESALL,
		OP_NO,
		OP_NOALL,
	};

public:
	CFileDupDlg(const CString& strMsg, CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileDupDlg();

// Dialog Data
	enum { IDD = IDD_DLG_FILEDUP };

protected:
	CString m_strDupMsg;
	CStatic m_ctlDupMsg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedYesall();
	afx_msg void OnBnClickedNo();
	afx_msg void OnBnClickedNoall();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
