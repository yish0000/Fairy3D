// FairyPackerView.cpp : CFairyPackerView ���ʵ��
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "FairyPackerDoc.h"
#include "FairyPackerView.h"
#include "ProgressDlg.h"
#include "SearchFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFairyPackerView

IMPLEMENT_DYNCREATE(CFairyPackerView, CListView)

BEGIN_MESSAGE_MAP(CFairyPackerView, CListView)
    ON_WM_CREATE()
    ON_COMMAND(ID_VIEW_LARGEICON, &CFairyPackerView::OnViewLargeicon)
    ON_COMMAND(ID_VIEW_SMALLICON, &CFairyPackerView::OnViewSmallicon)
    ON_COMMAND(ID_VIEW_LIST, &CFairyPackerView::OnViewList)
    ON_COMMAND(ID_VIEW_DETAILS, &CFairyPackerView::OnViewDetails)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LARGEICON, &CFairyPackerView::OnUpdateViewLargeicon)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SMALLICON, &CFairyPackerView::OnUpdateViewSmallicon)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LIST, &CFairyPackerView::OnUpdateViewList)
    ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILS, &CFairyPackerView::OnUpdateViewDetails)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CFairyPackerView::OnNMDblclk)
    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CFairyPackerView::OnLvnColumnclick)
    ON_UPDATE_COMMAND_UI(ID_EDIT_OPEN, &CFairyPackerView::OnUpdateEditOpen)
    ON_UPDATE_COMMAND_UI(ID_EDIT_ADDFILE, &CFairyPackerView::OnUpdateEditAddfile)
    ON_UPDATE_COMMAND_UI(ID_EDIT_ADDFOLDER, &CFairyPackerView::OnUpdateEditAddfolder)
    ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, &CFairyPackerView::OnUpdateEditDelete)
    ON_UPDATE_COMMAND_UI(ID_EDIT_EXTRACT, &CFairyPackerView::OnUpdateEditExtract)
    ON_UPDATE_COMMAND_UI(ID_EDIT_EXTRACTTO, &CFairyPackerView::OnUpdateEditExtractto)
    ON_COMMAND(ID_EDIT_OPEN, &CFairyPackerView::OnEditOpen)
    ON_COMMAND(ID_EDIT_EXTRACT, &CFairyPackerView::OnEditExtract)
    ON_COMMAND(ID_EDIT_EXTRACTTO, &CFairyPackerView::OnEditExtractto)
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_EDIT_ADDFILE, &CFairyPackerView::OnEditAddfile)
    ON_COMMAND(ID_EDIT_ADDFOLDER, &CFairyPackerView::OnEditAddfolder)
    ON_COMMAND(ID_EDIT_DELETE, &CFairyPackerView::OnEditDelete)
    ON_COMMAND(ID_TOOL_DEDRAG, &CFairyPackerView::OnEditDedrag)
    ON_UPDATE_COMMAND_UI(ID_TOOL_DEDRAG, &CFairyPackerView::OnUpdateEditDedrag)
    ON_COMMAND(ID_EDIT_SELECTALL, &CFairyPackerView::OnEditSelectall)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_SEARCHFILES, &CFairyPackerView::OnEditSearchfiles)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SEARCHFILES, &CFairyPackerView::OnUpdateEditSearchfiles)
END_MESSAGE_MAP()

// CFairyPackerView ����/����

CFairyPackerView::CFairyPackerView()
{
	// TODO: �ڴ˴���ӹ������
    m_nSortMode = 0;
    m_bSortBack = false;
}

CFairyPackerView::~CFairyPackerView()
{
}

BOOL CFairyPackerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CListView::PreCreateWindow(cs);
}

void CFairyPackerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	//  �Ӷ������������ ListView��
}


// CFairyPackerView ���

#ifdef _DEBUG
void CFairyPackerView::AssertValid() const
{
	CListView::AssertValid();
}

void CFairyPackerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CFairyPackerDoc* CFairyPackerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFairyPackerDoc)));
	return (CFairyPackerDoc*)m_pDocument;
}
#endif //_DEBUG


// CFairyPackerView ��Ϣ�������

void CFairyPackerView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
    // TODO: �ڴ����ר�ô����/����û���
    CString str;
    int iIndex = 0;
    int iconIndex = 3;  // Ĭ�����ļ���ͼ��

    CListCtrl& lc = GetListCtrl();
    CFairyExplorer* pExplorer = &GetDocument()->m_Explorer;
    if( !pExplorer->IsOpen() )
    {
        lc.DeleteAllItems();
        return;
    }

    const CFairyExplorer::PKNode* pCurNode = pExplorer->GetCurrentNode();
    lc.DeleteAllItems();

    // �����ǰ���ڸ�Ŀ¼����һ���ǻص���һĿ¼������
    if( pCurNode != pExplorer->GetRootNode() )
        lc.InsertItem( iIndex++,_T(".."),3 );

    std::list<CFairyExplorer::PKNode>::const_iterator it;
    for( it=pCurNode->Children.begin();it!=pCurNode->Children.end();++it )
    {
        WString ext, typeName;
        if( !it->bIsDirectory )
        {
			WString filename = F_UTF8ToUnicode(it->strName);

            // �ļ�����
            WStringUtil::FileExt( filename.c_str() ,ext, true );
            if( ext == L"" ) ext = L"file";

            SHFILEINFO shfi;
            ::SHGetFileInfo( ext.c_str(),FILE_ATTRIBUTE_NORMAL,&shfi,sizeof(SHFILEINFO),
                SHGFI_USEFILEATTRIBUTES|SHGFI_SYSICONINDEX|SHGFI_TYPENAME );
            iconIndex = shfi.iIcon;
            typeName = shfi.szTypeName;

            lc.InsertItem( iIndex, filename.c_str(), iconIndex );
            str.Format( _T("%d"), it->nFileSize );
            lc.SetItemText( iIndex, 1, str );
            str.Format( _T("%d"), it->nCompressedSize );
            lc.SetItemText( iIndex, 2, str );
            lc.SetItemText( iIndex, 3, typeName.c_str() );
        }
        else
        {
            iconIndex = 3;
            lc.InsertItem( iIndex, F_UTF8ToUnicode(it->strName).c_str(), iconIndex );
        }

        lc.SetItemData( iIndex, (DWORD_PTR)(&(*it)) );

		if (&(*it) == (CFairyExplorer::PKNode*)lHint && pHint != NULL)
		{
			lc.SetItemState(iIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			lc.SetSelectionMark(iIndex);
			lc.EnsureVisible(iIndex, FALSE);
		}

		++iIndex;
    }
}

int CFairyPackerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CListView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  �ڴ������ר�õĴ�������
    CListCtrl& lc = GetListCtrl();

    HMODULE hShell32 = LoadLibrary(_T("shell32.dll"));
    if( hShell32 != NULL )
    {
        typedef BOOL (WINAPI* FII_PROC)(BOOL fFullInit);
        LPCSTR str = (LPCSTR)660;
        FII_PROC FileIconInit = (FII_PROC)GetProcAddress( hShell32,(LPCSTR)660 );
        if( FileIconInit ) FileIconInit( TRUE );
    }

    HIMAGELIST hIcons,hSmallIcons;
    ::Shell_GetImageLists( &hIcons,&hSmallIcons );
    lc.SetImageList( CImageList::FromHandle(hIcons),LVSIL_NORMAL );
    lc.SetImageList( CImageList::FromHandle(hSmallIcons),LVSIL_SMALL );
    lc.ModifyStyle( 0,LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SHAREIMAGELISTS );
    lc.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
    m_dwStyle = LVS_REPORT;

    LVCOLUMN col;
    memset( &col,0,sizeof(LVCOLUMN) );

    col.mask = LVCF_WIDTH|LVCF_TEXT|LVCF_FMT|LVCF_SUBITEM;
    col.cx = 400;
    col.fmt = LVCFMT_LEFT|LVCFMT_BITMAP_ON_RIGHT;
    col.pszText = _T("����");
    col.cchTextMax = 256;
    col.iSubItem = 0;

    lc.InsertColumn( 0,&col );

    col.cx = 80;
    col.pszText = _T("��С");
    col.iSubItem = 1;
    lc.InsertColumn( 1,&col );

    col.cx = 80;
    col.pszText = _T("ѹ�����С");
    col.iSubItem = 2;
    lc.InsertColumn( 2,&col );

    col.cx = 100;
    col.pszText = _T("����");
    col.iSubItem = 3;
    lc.InsertColumn( 3,&col );

    return 0;
}

void CFairyPackerView::OnViewLargeicon()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();
    lc.ModifyStyle( m_dwStyle,LVS_ICON );
    m_dwStyle = LVS_ICON;
}

void CFairyPackerView::OnViewSmallicon()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();
    lc.ModifyStyle( m_dwStyle,LVS_SMALLICON );
    m_dwStyle = LVS_SMALLICON;
}

void CFairyPackerView::OnViewList()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();
    lc.ModifyStyle( m_dwStyle,LVS_LIST );
    m_dwStyle = LVS_LIST;
}

void CFairyPackerView::OnViewDetails()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();
    lc.ModifyStyle( m_dwStyle,LVS_REPORT );
    m_dwStyle = LVS_REPORT;
}

void CFairyPackerView::OnUpdateViewLargeicon(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetCheck( m_dwStyle == LVS_ICON );
}

void CFairyPackerView::OnUpdateViewSmallicon(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetCheck( m_dwStyle == LVS_SMALLICON );
}

void CFairyPackerView::OnUpdateViewList(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetCheck( m_dwStyle == LVS_LIST );
}

void CFairyPackerView::OnUpdateViewDetails(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->SetCheck( m_dwStyle == LVS_REPORT );
}

void CFairyPackerView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CListCtrl& lc = GetListCtrl();
    int selected = ((LPNMITEMACTIVATE)pNMHDR)->iItem;
    if( selected >= 0 && selected < lc.GetItemCount() )
    {
        CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData(selected);
        OnOpenNode( node );
    }
}

void CFairyPackerView::OnOpenNode( CFairyExplorer::PKNode* pNode )
{
    CFairyExplorer* exp = &GetDocument()->m_Explorer;
    if( pNode == NULL )
    {
        exp->EnterFolder( ".." );
        m_pDocument->UpdateAllViews( NULL );
    }
    else if( pNode->bIsDirectory )
    {
        exp->SetCurrentNode( pNode );
        m_pDocument->UpdateAllViews( NULL );
    }
    else
    {
        // ��ѹ������ָ�����ļ�
        TCHAR tempPath[MAX_PATH];
        ::GetTempPath( MAX_PATH, tempPath );
        _tcscat_s( tempPath, MAX_PATH, _T("__FPK_TEMP__") );
        FFilePairList files;
        files.push_back( std::make_pair(pNode->strName, F_UnicodeToUTF8(tempPath)) );
        ProcessOperation( FAIRY_OP_EXTRACT, exp, 1, files );
        ::ShellExecute( NULL, _T("open"), F_UTF8ToUnicode(pNode->strName).c_str(), NULL, tempPath, SW_SHOW );
    }
}

// ���ļ�����������
int CALLBACK CFairyPackerView::SortFunc( LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort )
{
    CFairyPackerView* pListView = (CFairyPackerView*)lParamSort;
    CFairyExplorer::PKNode* node1 = (CFairyExplorer::PKNode*)lParam1;
    CFairyExplorer::PKNode* node2 = (CFairyExplorer::PKNode*)lParam2;

    if( !node1 || !node2 )
        return 0;

    if( node1->bIsDirectory && node2->bIsDirectory )
        return strcmp( node1->strName.c_str(),node2->strName.c_str() );
    else if( node1->bIsDirectory )
        return -1;
    else if( node2->bIsDirectory )
        return 1;

    switch( pListView->m_nSortMode )
    {
    case 0:
        if( pListView->m_bSortBack )
            return strcmp( node1->strName.c_str(),node2->strName.c_str() );
        else
            return strcmp( node2->strName.c_str(),node1->strName.c_str() );
    case 1:
        if( node1->nFileSize == node2->nFileSize )
            return 0;
        else
        {
            if( pListView->m_bSortBack )
                return node1->nFileSize < node2->nFileSize ? 1 : -1;
            else
                return node1->nFileSize > node2->nFileSize ? 1 : -1;
        }
        break;
    case 2:
        if( node1->nCompressedSize == node2->nCompressedSize )
            return 0;
        else
        {
            if( pListView->m_bSortBack )
                return node1->nCompressedSize < node2->nCompressedSize ? 1 : -1;
            else
                return node1->nCompressedSize > node2->nCompressedSize ? 1 : -1;
        }
        break;
    case 3:
        {
            AString ext1,ext2;
            AStringUtil::FileExt( node1->strName,ext1 );
            AStringUtil::FileExt( node2->strName,ext2 );
            if( pListView->m_bSortBack )
                return strcmp( ext1.c_str(),ext2.c_str() );
            else
                return strcmp( ext2.c_str(),ext1.c_str() );
        }
        break;
    }

    ASSERT(0);
    return false;
}

void CFairyPackerView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CListCtrl& lc = GetListCtrl();
    if( m_nSortMode != pNMLV->iSubItem )
        m_nSortMode = pNMLV->iSubItem;
    else
        m_bSortBack = !m_bSortBack;
    lc.SortItems( SortFunc,(DWORD_PTR)this );
}

void CFairyPackerView::OnUpdateEditOpen(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    CListCtrl& lc = GetListCtrl();

    if( GetDocument()->m_Explorer.IsOpen() &&
        lc.GetFirstSelectedItemPosition() != NULL )
    {
        pCmdUI->Enable( TRUE );
    }
    else
    {
        pCmdUI->Enable( FALSE );
    }
}

void CFairyPackerView::OnUpdateEditAddfile(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    CListCtrl& lc = GetListCtrl();

    if( GetDocument()->m_Explorer.IsOpen() &&
        !GetDocument()->m_Explorer.IsReadonly() )
    {
        pCmdUI->Enable( TRUE );
    }
    else
    {
        pCmdUI->Enable( FALSE );
    }
}

void CFairyPackerView::OnUpdateEditAddfolder(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    CListCtrl& lc = GetListCtrl();

    if( GetDocument()->m_Explorer.IsOpen() &&
        !GetDocument()->m_Explorer.IsReadonly() )
    {
        pCmdUI->Enable( TRUE );
    }
    else
    {
        pCmdUI->Enable( FALSE );
    }
}

void CFairyPackerView::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    CListCtrl& lc = GetListCtrl();

    if( GetDocument()->m_Explorer.IsOpen() &&
        !GetDocument()->m_Explorer.IsReadonly() &&
        GetSelectedNode() != NULL )
    {
        pCmdUI->Enable( TRUE );
    }
    else
    {
        pCmdUI->Enable( FALSE );
    }
}

void CFairyPackerView::OnUpdateEditExtract(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->Enable( GetDocument()->m_Explorer.IsOpen() );
}

void CFairyPackerView::OnUpdateEditExtractto(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    pCmdUI->Enable( GetDocument()->m_Explorer.IsOpen() );
}

void CFairyPackerView::OnEditOpen()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();

    POSITION pos = lc.GetFirstSelectedItemPosition();
    if( pos )
    {
        int selected = lc.GetNextSelectedItem(pos);
        CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData(selected);
        OnOpenNode( node );
    }
}

void CFairyPackerView::OnEditExtract()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();
    CFairyExplorer& exp = GetDocument()->m_Explorer;

    UINT fileCount = 0;
    FFilePairList fileList;
    POSITION pos = lc.GetFirstSelectedItemPosition();
    if( !pos )
    {
        fileCount = exp.GetNodeFileCount( exp.GetCurrentNode() );
        fileList.push_back( std::make_pair(".", "") );
    }
    else
    {
        while( pos )
        {
            int selected = lc.GetNextSelectedItem( pos );
            CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData( selected );
            if( !node ) continue;
            fileCount += exp.GetNodeFileCount( node );
            fileList.push_back( std::make_pair(node->strName,"") );
        }
    }

    ProcessOperation( FAIRY_OP_EXTRACT,&exp,fileCount,fileList );
}

void CFairyPackerView::OnEditExtractto()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();
    CFairyExplorer& exp = GetDocument()->m_Explorer;

    // ѡ��һ��Ŀ���ļ���
    TCHAR szPath[MAX_PATH];
    ZeroMemory( szPath,sizeof(szPath) );

    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = _T("��ѡ����ҪҪ��ѹ��Ŀ¼��");
    bi.ulFlags = 0;
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;

    //����ѡ��Ŀ¼�Ի���
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if( !(lp && SHGetPathFromIDList(lp,szPath)) )
        return;

    UINT fileCount = 0;
    FFilePairList fileList;
    POSITION pos = lc.GetFirstSelectedItemPosition();
    if( !pos )
    {
        fileCount = exp.GetNodeFileCount( exp.GetCurrentNode() );
        fileList.push_back( std::make_pair(".", F_UnicodeToUTF8(szPath)) );
    }
    else
    {
        while( pos )
        {
            int selected = lc.GetNextSelectedItem( pos );
            CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData( selected );
            if( !node ) continue;
            fileCount += exp.GetNodeFileCount( node );
            fileList.push_back( std::make_pair(node->strName, F_UnicodeToUTF8(szPath)) );
        }
    }

    ProcessOperation( FAIRY_OP_EXTRACT,&exp,fileCount,fileList );
}

void CFairyPackerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    // TODO: �ڴ˴������Ϣ����������
    CMenu menu;
    menu.LoadMenu( IDR_VIEWMENU );
    CMenu* popMenu = menu.GetSubMenu(0);
    popMenu->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, AfxGetMainWnd() );
}

void CFairyPackerView::OnEditAddfile()
{
    // TODO: �ڴ���������������
    static std::vector<TCHAR> tmpBuf;
    tmpBuf.resize( 1000 * (MAX_PATH + 1) + 1 );
    CFileDialog dlg( TRUE,NULL,NULL,OFN_ALLOWMULTISELECT );
    dlg.GetOFN().lpstrFile = &tmpBuf[0];
    dlg.GetOFN().nMaxFile = 1000 * (MAX_PATH + 1) + 1;
    if( dlg.DoModal() != IDOK ) return;

    AString addToDir = "";
    CFairyExplorer* exp = &GetDocument()->m_Explorer;
    CFairyExplorer::PKNode* node = GetSelectedNode();
    if( node != NULL && node->bIsDirectory )
        addToDir = node->strName;

    int fileCount = 0;
    FFilePairList params;
    POSITION pos = dlg.GetStartPosition();
    while( pos )
    {
        CString filename = dlg.GetNextPathName(pos);
        fileCount += exp->GetDiskFileCount( F_UnicodeToUTF8((const TCHAR*)filename) );
        params.push_back( std::make_pair(F_UnicodeToUTF8((const TCHAR*)filename), addToDir) );
    }

    ProcessOperation( FAIRY_OP_ADDFILE,exp,fileCount,params );
    m_pDocument->UpdateAllViews(NULL);
}

// ��ȡ��ǰѡ���Ľڵ�
CFairyExplorer::PKNode* CFairyPackerView::GetSelectedNode(void)
{
    CListCtrl& lc = GetListCtrl();
    POSITION selectedPos = lc.GetFirstSelectedItemPosition();
    if( selectedPos != NULL )
    {
        int selectedIndex = lc.GetNextSelectedItem( selectedPos );
        return (CFairyExplorer::PKNode*)lc.GetItemData( selectedIndex );
    }

    return NULL;
}
void CFairyPackerView::OnEditAddfolder()
{
    // TODO: �ڴ���������������
    // ѡ��һ��Ŀ���ļ���
    TCHAR szPath[MAX_PATH];
    ZeroMemory( szPath,sizeof(szPath) );

    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = _T("��ѡ����ҪҪ��ӵ�Ŀ¼��");
    bi.ulFlags = 0;
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;

    //����ѡ��Ŀ¼�Ի���
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if( !(lp && SHGetPathFromIDList(lp, szPath)) )
        return;

    AString addToDir = "";
    CFairyExplorer* exp = &GetDocument()->m_Explorer;
    CFairyExplorer::PKNode* pNode = GetSelectedNode();
    if( pNode && pNode->bIsDirectory )
        addToDir = pNode->strName;

    UINT fileCount = exp->GetDiskFileCount( F_UnicodeToUTF8(szPath) );
    FFilePairList params;
    params.push_back( std::make_pair(F_UnicodeToUTF8(szPath), addToDir) );
    ProcessOperation( FAIRY_OP_ADDFILE,exp,fileCount,params );
    m_pDocument->UpdateAllViews( NULL );
}

void CFairyPackerView::OnEditDelete()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();
    CFairyExplorer* exp = &GetDocument()->m_Explorer;

    bool bRemove = false;
    POSITION pos = lc.GetFirstSelectedItemPosition();
    while( pos )
    {
        int selectIndex = lc.GetNextSelectedItem( pos );
        CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData( selectIndex );
        if( !node ) continue;
        if( node->strName == "." || node->strName == ".." ) continue;

        if( !bRemove )
        {
            CString info = _T("ȷ��Ҫ�Ƴ�");
			if (!pos)
			{
				info += _T("(");
				info += F_UTF8ToUnicode(node->strName).c_str();
				info += _T(")��");
			}
            else
                info += _T("��ѡ���������");

            if( MessageBox(info, _T("��ʾ"), MB_YESNO|MB_ICONQUESTION)
                != IDYES )
            {
                return;
            }
        }

        exp->Remove( node->strName );
        bRemove = true;
    }

    if( bRemove )
		m_pDocument->UpdateAllViews( NULL );
}

void CFairyPackerView::OnEditDedrag()
{
    // TODO: �ڴ���������������
    CFairyExplorer* exp = &GetDocument()->m_Explorer;
    uint32 fragSize = exp->GetPackage()->GetFragmentSize();
    if( fragSize == 0 )
    {
        MessageBox( _T("û����Ƭ��Ҫ����"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION );
        return;
    }

    UINT fileCount = exp->GetNodeFileCount( exp->GetRootNode() );
    FFilePairList params;
    params.push_back( std::make_pair("","") );
    int ret = ProcessOperation( FAIRY_OP_DEFRAG,exp,fileCount,params );
    if( !ret )
    {
        TCHAR info[120];
        _stprintf_s( info, 120, _T("��Ƭ������ϣ�Ϊ����ʡ��%d�ֽڣ�"), fragSize );
        MessageBox( info, _T("��ʾ"), MB_OK|MB_ICONINFORMATION );
    }
}

void CFairyPackerView::OnUpdateEditDedrag(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if( GetDocument()->m_Explorer.IsOpen() &&
        !GetDocument()->m_Explorer.IsReadonly() )
    {
        pCmdUI->Enable( TRUE );
    }
    else
    {
        pCmdUI->Enable( FALSE );
    }
}

void CFairyPackerView::OnEditSelectall()
{
    // TODO: �ڴ���������������
    CListCtrl& lc = GetListCtrl();
    for( int i=0;i<lc.GetItemCount();++i )
    {
        lc.SetItemState( i,LVIS_SELECTED|LVIS_FOCUSED,
            LVIS_SELECTED|LVIS_FOCUSED );
    }
}

void CFairyPackerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if( nChar == VK_DELETE )
	{
		OnEditDelete();
	}

	CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFairyPackerView::OnEditSearchfiles()
{
	// TODO: Add your command handler code here
	CSearchFileDlg dlg;
	dlg.SetCurPath(CString(_T("/")) + F_UTF8ToUnicode(GetDocument()->m_Explorer.GetCurrentPath()).c_str());
	dlg.DoModal();
}

void CFairyPackerView::OnUpdateEditSearchfiles(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if( GetDocument()->m_Explorer.IsOpen() )
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}
