#pragma once


// COptionDlg �Ի���

class COptionDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COptionDlg();

    SConfigData* m_pConfigData;

// �Ի�������
	enum { IDD = IDD_DLG_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnEnChangeFileExt();
    CString m_szFileExt;
    afx_msg void OnBnClickedBtnAdd();
    afx_msg void OnBnClickedBtnDelete();
    afx_msg void OnBnClickedOk();
};
