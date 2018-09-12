// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "OptionDlg.h"


// COptionDlg �Ի���

IMPLEMENT_DYNAMIC(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
    , m_szFileExt(_T(""))
    , m_pConfigData(NULL)
{
}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FILE_EXT, m_szFileExt);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
    ON_EN_CHANGE(IDC_FILE_EXT, &COptionDlg::OnEnChangeFileExt)
    ON_BN_CLICKED(IDC_BTN_ADD, &COptionDlg::OnBnClickedBtnAdd)
    ON_BN_CLICKED(IDC_BTN_DELETE, &COptionDlg::OnBnClickedBtnDelete)
    ON_BN_CLICKED(IDOK, &COptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COptionDlg ��Ϣ�������

BOOL COptionDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    ((CButton*)GetDlgItem(IDC_BTN_ADD))->EnableWindow(FALSE);

    m_pConfigData = &g_Config;
    ((CComboBox*)GetDlgItem(IDC_COMPRESSTYPE))->AddString(_T("ZLib"));
    ((CComboBox*)GetDlgItem(IDC_COMPRESSTYPE))->AddString(_T("MiniLZO"));
    ((CComboBox*)GetDlgItem(IDC_COMPRESSTYPE))->SetCurSel(m_pConfigData->compress==FPK_COMP_ZLIB?0:1);

    if( m_pConfigData->bufferLevel == FPK_QUALITY_ENTIRE )
        ((CButton*)GetDlgItem(IDC_BUFFER_ENTIRE))->SetCheck( BST_CHECKED );
    if( m_pConfigData->bufferLevel == FPK_QUALITY_LOW )
        ((CButton*)GetDlgItem(IDC_BUFFER_LOW))->SetCheck( BST_CHECKED );
    else if( m_pConfigData->bufferLevel == FPK_QUALITY_NORMAL )
        ((CButton*)GetDlgItem(IDC_BUFFER_NORMAL))->SetCheck( BST_CHECKED );
    else
        ((CButton*)GetDlgItem(IDC_BUFFER_HIGH))->SetCheck( BST_CHECKED );

    for( size_t i=0;i<m_pConfigData->extList.size();++i )
    {
        CListBox* pList = (CListBox*)GetDlgItem( IDC_NOCOMP_LIST );
        pList->AddString( F_UTF8ToUnicode(m_pConfigData->extList[i]).c_str() );
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void COptionDlg::OnEnChangeFileExt()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
    // ���͸�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData();
    if( m_szFileExt.IsEmpty() )
        GetDlgItem(IDC_BTN_ADD)->EnableWindow(FALSE);
    else
        GetDlgItem(IDC_BTN_ADD)->EnableWindow(TRUE);
}

void COptionDlg::OnBnClickedBtnAdd()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData();
    CString itemStr;
    CListBox* pList = (CListBox*)GetDlgItem( IDC_NOCOMP_LIST );
    for( int i=0;i<pList->GetCount();++i )
    {
        pList->GetText( i,itemStr );
        if( m_szFileExt == itemStr )
        {
            AfxMessageBox( _T("���ļ������Ѿ����ڣ�") );
            goto lExit;
        }
    }

    pList->AddString( m_szFileExt );

lExit:
    m_szFileExt = _T("");
    UpdateData( FALSE );
    GetDlgItem(IDC_BTN_ADD)->EnableWindow( FALSE );
}

void COptionDlg::OnBnClickedBtnDelete()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CListBox* pList = (CListBox*)GetDlgItem( IDC_NOCOMP_LIST );
    int curSel = pList->GetCurSel();
    if( curSel != LB_ERR )
    {
        if( MessageBox(_T("Ҫɾ����������"), _T("��ʾ"), MB_YESNO|MB_ICONQUESTION)
            == IDYES )
        {
            pList->DeleteString( curSel );
        }
    }
    else
    {
        AfxMessageBox( _T("��ѡ��һ��Ҫɾ�������ͣ�") );
    }
}

void COptionDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if( ((CComboBox*)GetDlgItem(IDC_COMPRESSTYPE))->GetCurSel() == 0 )
        m_pConfigData->compress = FPK_COMP_ZLIB;
    else
        m_pConfigData->compress = FPK_COMP_MINILZO;

    if( ((CButton*)GetDlgItem(IDC_BUFFER_ENTIRE))->GetCheck() == BST_CHECKED )
        m_pConfigData->bufferLevel = FPK_QUALITY_ENTIRE;
    else if( ((CButton*)GetDlgItem(IDC_BUFFER_LOW))->GetCheck() == BST_CHECKED )
        m_pConfigData->bufferLevel = FPK_QUALITY_LOW;
    else if( ((CButton*)GetDlgItem(IDC_BUFFER_NORMAL))->GetCheck() == BST_CHECKED )
        m_pConfigData->bufferLevel = FPK_QUALITY_NORMAL;
    else
        m_pConfigData->bufferLevel = FPK_QUALITY_HIGH;

    m_pConfigData->extList.clear();
    CListBox* pList = (CListBox*)GetDlgItem( IDC_NOCOMP_LIST );
    for( int i=0;i<pList->GetCount();++i )
    {
        CString strItem;
        pList->GetText( i,strItem );
        m_pConfigData->extList.push_back( F_UnicodeToUTF8((LPCTSTR)strItem) );
    }

    ((CFairyPackerApp*)AfxGetApp())->SaveConfigData( _T("fpkconfig.ini") );
    OnOK();
}