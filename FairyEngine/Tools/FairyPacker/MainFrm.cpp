// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
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
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    // TODO: 在此添加专用代码和/或调用基类
    if( !m_wndSplitter.CreateStatic(this,1,2) )
        return FALSE;

    // 创建左边的树状视图
    if( !m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CFairyTreeView),
        CSize(300,300),pContext) )
        return FALSE;

    // 创建ListView视图
    if( !m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CFairyPackerView),
        CSize(300,300),pContext) )
        return FALSE;

    return TRUE;
}

void CMainFrame::OnFileNew()
{
    // TODO: 在此添加命令处理程序代码
    CFileDialog dlg( FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
        _T("Fairy package archives (*.fpk)|*.fpk|All Files (*.*)|*.*||") );
    if( dlg.DoModal() != IDOK ) return;

    CString filename = dlg.GetPathName();
    if( dlg.GetFileExt().IsEmpty() )
        filename += _T(".fpk");

    CFairyPackerDoc* pDoc = (CFairyPackerDoc*)GetActiveDocument();
    CFairyExplorer& exp = pDoc->m_Explorer;

    // 清理旧的数据
    if( exp.IsOpen() )
        exp.Close();

    // 加载文档
    if( !exp.Create(F_UnicodeToUTF8((const TCHAR*)filename)) )
    {
        MessageBox( _T("创建文档失败！"),_T("错误"),MB_OK|MB_ICONERROR );
        return;
    }

    // 更新所有的视图
    pDoc->UpdateAllViews( NULL,1 );
    CString title = dlg.GetFileName() + _T(" - FairyPacker");
    AfxGetMainWnd()->SetWindowText( title );
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    CFairyPackerDoc* pDoc = dynamic_cast<CFairyPackerDoc*>(GetActiveDocument());
    if( pDoc->m_Explorer.IsOpen() )
        pCmdUI->Enable( TRUE );
    else
        pCmdUI->Enable( FALSE );
}

void CMainFrame::OnUpdateToolInfo(CCmdUI *pCmdUI)
{
    // TODO: 在此添加命令更新用户界面处理程序代码
    CFairyPackerDoc* pDoc = dynamic_cast<CFairyPackerDoc*>(GetActiveDocument());
    if( pDoc->m_Explorer.IsOpen() )
        pCmdUI->Enable( TRUE );
    else
        pCmdUI->Enable( FALSE );
}

void CMainFrame::OnToolOption()
{
    // TODO: 在此添加命令处理程序代码
    COptionDlg dlg;
    dlg.DoModal();
}

void CMainFrame::OnToolInfo()
{
    // TODO: 在此添加命令处理程序代码
    SPackageInfo info;
    TCHAR szMessage[1024];
    CFairyExplorer& exp = ((CFairyPackerDoc*)GetActiveDocument())->m_Explorer;
    exp.GetPackageInfo( info );
    _stprintf_s( szMessage,1024,
        _T("目录个数：    %d\n")
        _T("文件个数：    %d\n")
        _T("文件总大小：  %I64u\n")
        _T("压缩后大小：  %u\n")
        _T("压缩率：      %.2f%%"),
        info.folderCount,info.fileCount,info.origSize,info.compressSize,
        info.compressDegree*100 );
    MessageBox( szMessage,_T("信息"),MB_OK|MB_ICONINFORMATION );
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
    // TODO: Add your message handler code here and/or call default

    // 添加文件到压缩包
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

        // 添加文件到压缩包
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
