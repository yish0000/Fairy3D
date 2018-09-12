// FairyPackerDoc.cpp : CFairyPackerDoc 类的实现
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "FairyPackerDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFairyPackerDoc

IMPLEMENT_DYNCREATE(CFairyPackerDoc, CDocument)

BEGIN_MESSAGE_MAP(CFairyPackerDoc, CDocument)
END_MESSAGE_MAP()


// CFairyPackerDoc 构造/析构

CFairyPackerDoc::CFairyPackerDoc()
{
	// TODO: 在此添加一次性构造代码
}

CFairyPackerDoc::~CFairyPackerDoc()
{
}

// CFairyPackerDoc 序列化

void CFairyPackerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CFairyPackerDoc 诊断

#ifdef _DEBUG
void CFairyPackerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFairyPackerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFairyPackerDoc 命令

BOOL CFairyPackerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    CWaitCursor wait;

    // TODO:  在此添加您专用的创建代码

    // 清理旧的数据
    if( m_Explorer.IsOpen() )
        m_Explorer.Close();

    // 打开文件包
    FFilePairList files;
    files.push_back( std::make_pair(F_UnicodeToUTF8(lpszPathName), "") );
    if( ProcessOperation(FAIRY_OP_OPEN,&m_Explorer,1,files) )
    {
        UpdateAllViews( NULL,1 );
        return FALSE;
    }

    // 刷新视图
    UpdateAllViews( NULL,1 );
    return TRUE;
}

BOOL CFairyPackerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // TODO: 在此添加专用代码和/或调用基类
    m_Explorer.SaveAs( F_UnicodeToUTF8(lpszPathName) );
    return OnOpenDocument( lpszPathName );
}
