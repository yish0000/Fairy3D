// SearchFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "MainFrm.h"
#include "FairyPackerDoc.h"
#include "SearchFileDlg.h"


// CSearchFileDlg dialog

IMPLEMENT_DYNAMIC(CSearchFileDlg, CDialog)

CSearchFileDlg::CSearchFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchFileDlg::IDD, pParent)
	, m_szKeyword(_T(""))
	, m_szCurPath(_T(""))
{
	m_nSortMode = 0;
	m_bSortBack = false;
}

CSearchFileDlg::~CSearchFileDlg()
{
}

void CSearchFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KEYWORD, m_szKeyword);
	DDX_Text(pDX, IDC_CURRENT_PATH, m_szCurPath);
	DDX_Control(pDX, IDC_LISTFILES, m_ctlFileList);
}


BEGIN_MESSAGE_MAP(CSearchFileDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTFILES, &CSearchFileDlg::OnNMDblclkListfiles)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTFILES, &CSearchFileDlg::OnLvnColumnclickListfiles)
	ON_COMMAND(ID_SEARCH_DELETE, &CSearchFileDlg::OnSearchDelete)
	ON_COMMAND(ID_SEARCH_OPEN, &CSearchFileDlg::OnSearchOpen)
	ON_COMMAND(ID_SEARCH_OPENFOLDER, &CSearchFileDlg::OnSearchOpenfolder)
	ON_COMMAND(ID_SEARCH_EXTRACT, &CSearchFileDlg::OnSearchExtract)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_SEARCH, &CSearchFileDlg::OnBnClickedSearch)
END_MESSAGE_MAP()


BOOL CSearchFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HIMAGELIST hIcons,hSmallIcons;
	::Shell_GetImageLists( &hIcons,&hSmallIcons );
	m_ctlFileList.SetImageList( CImageList::FromHandle(hIcons), LVSIL_NORMAL );
	m_ctlFileList.SetImageList( CImageList::FromHandle(hSmallIcons), LVSIL_SMALL );
	m_ctlFileList.ModifyStyle( 0, LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SHAREIMAGELISTS );
	m_ctlFileList.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

	LVCOLUMN col;
	memset( &col,0,sizeof(LVCOLUMN) );

	col.mask = LVCF_WIDTH|LVCF_TEXT|LVCF_FMT|LVCF_SUBITEM;
	col.cx = 100;
	col.fmt = LVCFMT_LEFT|LVCFMT_BITMAP_ON_RIGHT;
	col.pszText = _T("名字");
	col.cchTextMax = 256;
	col.iSubItem = 0;

	m_ctlFileList.InsertColumn( 0,&col );

	col.cx = 80;
	col.pszText = _T("大小");
	col.iSubItem = 1;
	m_ctlFileList.InsertColumn( 1,&col );

	col.cx = 80;
	col.pszText = _T("压缩后大小");
	col.iSubItem = 2;
	m_ctlFileList.InsertColumn( 2,&col );

	col.cx = 100;
	col.pszText = _T("类型");
	col.iSubItem = 3;
	m_ctlFileList.InsertColumn( 3,&col );

	col.cx = 200;
	col.pszText = _T("文件位置");
	col.iSubItem = 4;
	m_ctlFileList.InsertColumn(4, &col);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

static CFairyPackerDoc* GetFPKDocument()
{
	return dynamic_cast<CFairyPackerDoc*>((dynamic_cast<CMainFrame*>(AfxGetMainWnd()))->GetActiveDocument());
}

static CFairyExplorer* GetFPKExplorer()
{
	return GetFPKDocument()->GetFPKExplorer();
}

// CSearchFileDlg message handlers

void CSearchFileDlg::OnNMDblclkListfiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CListCtrl& lc = m_ctlFileList;
	int selected = ((LPNMITEMACTIVATE)pNMHDR)->iItem;
	if( selected >= 0 && selected < lc.GetItemCount() )
	{
		CFairyExplorer* pFPK = GetFPKExplorer();
		CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData(selected);

		// 解压并运行指定的文件
		if( node && !node->bIsDirectory )
		{
			TCHAR tempPath[MAX_PATH];
			::GetTempPath( MAX_PATH, tempPath );
			_tcscat_s( tempPath, MAX_PATH, _T("__FPK_TEMP__") );
			FFilePairList files;
			files.push_back( std::make_pair("/" + m_sFileList[selected], F_UnicodeToUTF8(tempPath)) );
			ProcessOperation( FAIRY_OP_EXTRACT, pFPK, 1, files );
			::ShellExecute( NULL, _T("open"), F_UTF8ToUnicode(node->strName).c_str(), NULL, tempPath, SW_SHOW );
		}
	}
}

static AString GetNodePath(const CFairyExplorer::PKNode* pNode)
{
	AString ret;

	while( pNode != NULL && pNode != GetFPKExplorer()->GetRootNode() )
	{
		ret = pNode->strName + FPK_DIR_STR + ret;
		pNode = pNode->pParent;
	}

	return ret;
}

// 按文件的类型排序
int CALLBACK CSearchFileDlg::ListSortFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	CSearchFileDlg* pDlg = (CSearchFileDlg*)lParamSort;
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

	switch( pDlg->m_nSortMode )
	{
	case 0:
		if( pDlg->m_bSortBack )
			return strcmp( node1->strName.c_str(),node2->strName.c_str() );
		else
			return strcmp( node2->strName.c_str(),node1->strName.c_str() );
	case 1:
		if( node1->nFileSize == node2->nFileSize )
			return 0;
		else
		{
			if( pDlg->m_bSortBack )
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
			if( pDlg->m_bSortBack )
				return node1->nCompressedSize < node2->nCompressedSize ? 1 : -1;
			else
				return node1->nCompressedSize > node2->nCompressedSize ? 1 : -1;
		}
		break;
	case 3:
		{
			AString ext1, ext2;
			AStringUtil::FileExt( node1->strName, ext1 );
			AStringUtil::FileExt( node2->strName, ext2 );
			if( pDlg->m_bSortBack )
				return strcmp( ext1.c_str(), ext2.c_str() );
			else
				return strcmp( ext2.c_str(), ext1.c_str() );
		}
		break;
	case 4:
		{
			AString path1 = GetNodePath(node1);
			AString path2 = GetNodePath(node2);
			if( pDlg->m_bSortBack )
				return strcmp(path1.c_str(), path2.c_str());
			else
				return strcmp(path2.c_str(), path1.c_str());
		}
		break;
	}

	ASSERT(0);
	return false;
}

void CSearchFileDlg::OnLvnColumnclickListfiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: Add your control notification handler code here
	if( m_nSortMode != pNMLV->iSubItem )
		m_nSortMode = pNMLV->iSubItem;
	else
		m_bSortBack = !m_bSortBack;
	m_ctlFileList.SortItems( ListSortFunc, (DWORD_PTR)this );
}

void CSearchFileDlg::OnSearchDelete()
{
	// TODO: Add your command handler code here
	CListCtrl& lc = m_ctlFileList;
	CFairyExplorer* exp = GetFPKExplorer();

	bool bRemove = false;
	POSITION pos = lc.GetFirstSelectedItemPosition();
	while (pos)
	{
		int selectIndex = lc.GetNextSelectedItem(pos);
		CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData(selectIndex);
		if (!node) continue;
		if (node->strName == "." || node->strName == "..") continue;

		if (!bRemove)
		{
			CString info = _T("确定要移除");
			if (!pos)
			{
				info += _T("(");
				info += F_UTF8ToUnicode(node->strName).c_str();
				info += _T(")吗？");
			}
			else
				info += _T("被选择的内容吗？");

			if (MessageBox(info, _T("提示"), MB_YESNO | MB_ICONQUESTION)
				!= IDYES)
			{
				return;
			}
		}

		exp->Remove("/" + m_sFileList[selectIndex]);
		lc.DeleteItem(selectIndex);
		m_sFileList.erase(m_sFileList.begin() + selectIndex);
		bRemove = true;
		pos = lc.GetFirstSelectedItemPosition();
	}

	if (bRemove) GetFPKDocument()->UpdateAllViews(NULL);
}

void CSearchFileDlg::OnSearchOpen()
{
	// TODO: Add your command handler code here
	CListCtrl& lc = m_ctlFileList;

	if (lc.GetSelectedCount() != 1)
		return;

	POSITION pos = lc.GetFirstSelectedItemPosition();
	while (pos)
	{
		int selected = lc.GetNextSelectedItem(pos);
		CFairyExplorer* pFPK = GetFPKExplorer();
		CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData(selected);
		if (!node) continue;

		if (node && !node->bIsDirectory)
		{
			TCHAR tempPath[MAX_PATH];
			::GetTempPath(MAX_PATH, tempPath);
			_tcscat_s(tempPath, MAX_PATH, _T("__FPK_TEMP__"));
			FFilePairList files;
			files.push_back(std::make_pair("/" + m_sFileList[selected], F_UnicodeToUTF8(tempPath)));
			ProcessOperation(FAIRY_OP_EXTRACT, pFPK, 1, files);
			::ShellExecute(NULL, _T("open"), F_UTF8ToUnicode(node->strName).c_str(), NULL, tempPath, SW_SHOW);
		}
	}
}

void CSearchFileDlg::OnSearchOpenfolder()
{
	// TODO: Add your command handler code here
	CListCtrl& lc = m_ctlFileList;

	if (lc.GetSelectedCount() != 1)
		return;

	POSITION pos = lc.GetFirstSelectedItemPosition();
	while (pos)
	{
		int selected = lc.GetNextSelectedItem(pos);
		CFairyExplorer* pFPK = GetFPKExplorer();
		CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData(selected);
		if (!node) continue;

		if (node && !node->bIsDirectory)
		{
			CFairyExplorer::PKNode* dirNode = node->pParent;
			ASSERT(dirNode && dirNode->bIsDirectory);
			pFPK->SetCurrentNode(dirNode);
			CDocument* pDoc = dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
			ASSERT(pDoc);
			pDoc->UpdateAllViews(NULL, (LPARAM)node, &lc);
		}
	}
}

void CSearchFileDlg::OnSearchExtract()
{
	// TODO: Add your command handler code here
	CListCtrl& lc = m_ctlFileList;
	CFairyExplorer* exp = GetFPKExplorer();

	if (lc.GetSelectedCount() == 0)
		return;

	// 选择一个目标文件夹
	TCHAR szPath[MAX_PATH];
	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("请选择需要要解压的目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (!(lp && SHGetPathFromIDList(lp, szPath)))
		return;

	UINT fileCount = 0;
	FFilePairList fileList;
	POSITION pos = lc.GetFirstSelectedItemPosition();
	while (pos)
	{
		int selected = lc.GetNextSelectedItem(pos);
		CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)lc.GetItemData(selected);
		if (!node) continue;
		fileCount += exp->GetNodeFileCount(node);
		AString filename = "/" + m_sFileList[selected];
		fileList.push_back(std::make_pair(filename, F_UnicodeToUTF8(szPath)));
	}

	ProcessOperation(FAIRY_OP_EXTRACT, exp, fileCount, fileList);
}

void CSearchFileDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	if (pWnd == GetDlgItem(IDC_LISTFILES))
	{
		CMenu menu;
		menu.LoadMenu(IDR_SEARCHMENU);
		CMenu* popMenu = menu.GetSubMenu(0);
		popMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, pWnd->GetParent());
	}
}


void CSearchFileDlg::OnBnClickedSearch()
{
	// TODO: Add your control notification handler code here
	CFairyExplorer* pFPK = GetFPKExplorer();

	UpdateData();
	if (m_szKeyword.IsEmpty())
	{
		AfxMessageBox(_T("请输入关键字！"));
		return;
	}

	CString str;
	int iIndex = 0;
	int iconIndex = 3;  // 默认是文件夹图标

	m_sFileList.clear();
	pFPK->SearchFiles(F_TEXTTOUTF8((const TCHAR*)m_szCurPath), F_TEXTTOUTF8((const TCHAR*)m_szKeyword), m_sFileList);
	if (m_sFileList.size() > 0)
	{
		m_ctlFileList.DeleteAllItems();

		for (size_t i = 0; i < m_sFileList.size(); ++i)
		{
			const CFairyExplorer::PKNode* pNode = pFPK->GetNodeByPath(m_sFileList[i]);
			WString ext, typeName;
			WString filename = F_UTF8ToUnicode(pNode->strName);

			// 文件类型
			WStringUtil::FileExt(filename.c_str(), ext, true);
			if (ext == L"") ext = L"file";

			SHFILEINFO shfi;
			::SHGetFileInfo(ext.c_str(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFO),
				SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_TYPENAME);
			iconIndex = shfi.iIcon;
			typeName = shfi.szTypeName;

			m_ctlFileList.InsertItem(iIndex, filename.c_str(), iconIndex);
			str.Format(_T("%d"), pNode->nFileSize);
			m_ctlFileList.SetItemText(iIndex, 1, str);
			str.Format(_T("%d"), pNode->nCompressedSize);
			m_ctlFileList.SetItemText(iIndex, 2, str);
			m_ctlFileList.SetItemText(iIndex, 3, typeName.c_str());
			m_ctlFileList.SetItemText(iIndex, 4, F_UTF8TOTEXT(m_sFileList[i]).c_str());

			m_ctlFileList.SetItemData(iIndex, (DWORD_PTR)pNode);
			++iIndex;
		}
	}
	else
	{
		AfxMessageBox(_T("找不到指定的文件！"));
	}
}
