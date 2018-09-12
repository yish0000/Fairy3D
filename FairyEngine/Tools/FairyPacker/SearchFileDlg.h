#pragma once
#include "afxcmn.h"


// CSearchFileDlg dialog

class CSearchFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CSearchFileDlg)

public:
	CSearchFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchFileDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SEARCHFILES };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	int m_nSortMode;    // 当前的排序模式
	bool m_bSortBack;   // 反向排序？

	CString m_szKeyword;
	CString m_szCurPath;
	CListCtrl m_ctlFileList;
	AStringList m_sFileList;

	// 排序函数
	static int CALLBACK ListSortFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );

public:
	void SetCurPath(const CString& path) { m_szCurPath = path; }

public:
	afx_msg void OnNMDblclkListfiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListfiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSearchDelete();
	afx_msg void OnSearchOpen();
	afx_msg void OnSearchOpenfolder();
	afx_msg void OnSearchExtract();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnBnClickedSearch();
};
