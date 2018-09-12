// FairyTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "FairyExplorer.h"
#include "FairyPackerDoc.h"
#include "FairyTreeView.h"


// CFairyTreeView

IMPLEMENT_DYNCREATE(CFairyTreeView, CTreeView)

CFairyTreeView::CFairyTreeView()
{

}

CFairyTreeView::~CFairyTreeView()
{
}

BEGIN_MESSAGE_MAP(CFairyTreeView, CTreeView)
    ON_WM_CREATE()
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CFairyTreeView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CFairyTreeView 诊断

#ifdef _DEBUG
void CFairyTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CFairyTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif

CFairyPackerDoc* CFairyTreeView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFairyPackerDoc)));
    return (CFairyPackerDoc*)m_pDocument;
}
#endif //_DEBUG


// CFairyTreeView 消息处理程序

BOOL CFairyTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CTreeView::PreCreateWindow(cs);
}

int CFairyTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CTreeView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    CBitmap bmp;
    bmp.LoadBitmap( IDB_TREEVIEW );
    m_ImageList.Create( 16,16,ILC_COLOR24|ILC_MASK,0,0 );
    m_ImageList.Add( &bmp,RGB(255,255,255) );

    CTreeCtrl& tc = GetTreeCtrl();
    tc.ModifyStyle( 0,TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_LINESATROOT );
    tc.SetImageList( &m_ImageList,TVSIL_NORMAL );

    return 0;
}

void CFairyTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    // TODO: 在此添加专用代码和/或调用基类
    CTreeCtrl& tc = GetTreeCtrl();
    CFairyExplorer& exp = GetDocument()->m_Explorer;
    if( !exp.IsOpen() )
    {
        tc.DeleteAllItems();
        return;
    }

    if( lHint == 1 && pHint == NULL )
		tc.DeleteAllItems();

    CFairyExplorer::PKNode* node = const_cast<CFairyExplorer::PKNode*>(exp.GetCurrentNode());
    HTREEITEM currentItem = (HTREEITEM)node->pUserData;
    if( currentItem != NULL )
    {
        UpdateNode( currentItem,node );
        tc.SelectItem( currentItem );
        return;
    }

    // 获取资源包的根节点
    CFairyExplorer::PKNode* root = const_cast<CFairyExplorer::PKNode*>(exp.GetRootNode());

    AString strDir,strBase;
    AStringUtil::SplitPath( exp.GetPackageName(),strBase,strDir );
    if( GetDocument()->m_Explorer.IsReadonly() ) strBase += "(Readonly)";

    // 插入根节点
    HTREEITEM rootItem = tc.InsertItem( F_UTF8ToUnicode(strBase).c_str(),0,1 );
    tc.SetItemData( rootItem,(DWORD_PTR)root );
    UpdateNode( rootItem,const_cast<CFairyExplorer::PKNode*>(root) );
    tc.Select( rootItem,TVGN_CARET );
    root->pUserData = rootItem;
}

void CFairyTreeView::UpdateNode( HTREEITEM hItem,CFairyExplorer::PKNode* node )
{
    CTreeCtrl& tc = GetTreeCtrl();

    if( tc.ItemHasChildren(hItem) )
    {
        HTREEITEM childItem = tc.GetChildItem( hItem );
        while( childItem != NULL )
        {
            HTREEITEM nextItem = tc.GetNextItem( childItem,TVGN_NEXT );
            tc.DeleteItem( childItem );
            childItem = nextItem;
        }
    }

    // 添加子节点
    std::list<CFairyExplorer::PKNode>::iterator it;
    for( it=node->Children.begin();it!=node->Children.end();++it )
    {
        CFairyExplorer::PKNode* child = &(*it);
        if( !child->bIsDirectory ) continue;
        HTREEITEM childItem = tc.InsertItem( F_UTF8ToUnicode(child->strName).c_str(), 0, 1, hItem );
        tc.SetItemData( childItem,(DWORD_PTR)child );
        child->pUserData = childItem;
        UpdateNode( childItem,child );
    }
}

void CFairyTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    CTreeCtrl& tc = GetTreeCtrl();
    HTREEITEM hItem = tc.GetSelectedItem();
    if( hItem != NULL )
    {
        CFairyExplorer& exp = GetDocument()->m_Explorer;
        CFairyExplorer::PKNode* node = (CFairyExplorer::PKNode*)tc.GetItemData( hItem );
        exp.SetCurrentNode( node );
        GetDocument()->UpdateAllViews( this );
    }
}
