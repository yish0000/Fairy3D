// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "FairyPackerDoc.h"
#include "FairyPackerView.h"
#include "FairyTreeView.h"
#include "MainFrm.h"
#include "OptionDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CMainFrame::OnUpdateFileSaveAs)
    ON_UPDATE_COMMAND_UI(ID_TOOL_INFO, &CMainFrame::OnUpdateToolInfo)
    ON_COMMAND(ID_TOOL_OPTION, &CMainFrame::OnToolOption)
    ON_COMMAND(ID_TOOL_INFO, &CMainFrame::OnToolInfo)
    ON_WM_DROPFILES()
    ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    // TODO: �ڴ����ר�ô����/����û���
    if( !m_wndSplitter.CreateStatic(this,1,2) )
        return FALSE;

    // ������ߵ���״��ͼ
    if( !m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CFairyTreeView),
        CSize(300,300),pContext) )
        return FALSE;

    // ����ListView��ͼ
    if( !m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CFairyPackerView),
        CSize(300,300),pContext) )
        return FALSE;

    return TRUE;
}

void CMainFrame::OnFileNew()
{
    // TODO: �ڴ���������������
    CFileDialog dlg( FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
        _T("Fairy package archives (*.fpk)|*.fpk|All Files (*.*)|*.*||") );
    if( dlg.DoModal() != IDOK ) return;

    CString filename = dlg.GetPathName();
    if( dlg.GetFileExt().IsEmpty() )
        filename += _T(".fpk");

    CFairyPackerDoc* pDoc = (CFairyPackerDoc*)GetActiveDocument();
    CFairyExplorer& exp = pDoc->m_Explorer;

    // ����ɵ�����
    if( exp.IsOpen() )
        exp.Close();

    // �����ĵ�
    if( !exp.Create(F_UnicodeToUTF8((const TCHAR*)filename)) )
    {
        MessageBox( _T("�����ĵ�ʧ�ܣ�"),_T("����"),MB_OK|MB_ICONERROR );
        return;
    }

    // �������е���ͼ
    pDoc->UpdateAllViews( NULL,1 );
    CString title = dlg.GetFileName() + _T(" - FairyPacker");
    AfxGetMainWnd()->SetWindowText( title );
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    CFairyPackerDoc* pDoc = dynamic_cast<CFairyPackerDoc*>(GetActiveDocument());
    if( pDoc->m_Explorer.IsOpen() )
        pCmdUI->Enable( TRUE );
    else
        pCmdUI->Enable( FALSE );
}

void CMainFrame::OnUpdateToolInfo(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    CFairyPackerDoc* pDoc = dynamic_cast<CFairyPackerDoc*>(GetActiveDocument());
    if( pDoc->m_Explorer.IsOpen() )
        pCmdUI->Enable( TRUE );
    else
        pCmdUI->Enable( FALSE );
}

void CMainFrame::OnToolOption()
{
    // TODO: �ڴ���������������
    COptionDlg dlg;
    dlg.DoModal();
}

void CMainFrame::OnToolInfo()
{
    // TODO: �ڴ���������������
    SPackageInfo info;
    TCHAR szMessage[1024];
    CFairyExplorer& exp = ((CFairyPackerDoc*)GetActiveDocument())->m_Explorer;
    exp.GetPackageInfo( info );
    _stprintf_s( szMessage,1024,
        _T("Ŀ¼������    %d\n")
        _T("�ļ�������    %d\n")
        _T("�ļ��ܴ�С��  %I64u\n")
        _T("ѹ�����С��  %u\n")
        _T("ѹ���ʣ�      %.2f%%"),
        info.folderCount,info.fileCount,info.origSize,info.compressSize,
        info.compressDegree*100 );
    MessageBox( szMessage,_T("��Ϣ"),MB_OK|MB_ICONINFORMATION );
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
    // TODO: Add your message handler code here and/or call default

    // ����ļ���ѹ����
    CFairyExplorer* pFPK = ((CFairyPackerDoc*)GetActiveDocument())->GetFPKExplorer();
    if( pFPK && pFPK->IsOpen() )
    {
        int fileCount = 0;
        TCHAR szFile[MAX_PATH];
        FFilePairList params;

        UINT iCount = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
        for( UINT i=0;i<iCount;i++ )
        {
            AString strExt;
            DragQueryFile(hDropInfo, i, szFile, MAX_PATH);
            AStringUtil::FileExt(F_UnicodeToUTF8(szFile), strExt);
            if( strExt == "fpk" ) continue;

            if( GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY )
                fileCount += pFPK->GetDiskFileCount(F_UnicodeToUTF8(szFile));
            else
                fileCount++;

            params.push_back(std::make_pair(F_UnicodeToUTF8(szFile), ""));
        }

        // ����ļ���ѹ����
        ProcessOperation(FAIRY_OP_ADDFILE, pFPK, fileCount, params);
        GetActiveDocument()->UpdateAllViews(NULL);
        return;
    }

    CFrameWnd::OnDropFiles(hDropInfo);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    CFrameWnd::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if( GetActiveDocument() )
        GetActiveDocument()->UpdateAllViews(NULL);
}
