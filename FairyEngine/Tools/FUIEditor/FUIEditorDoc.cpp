// FUIEditorDoc.cpp : CFUIEditorDoc 类的实现
//

#include "stdafx.h"
#include "FUIEditor.h"

#include "FUIEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFUIEditorDoc

IMPLEMENT_DYNCREATE(CFUIEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CFUIEditorDoc, CDocument)
END_MESSAGE_MAP()


// CFUIEditorDoc 构造/析构

CFUIEditorDoc::CFUIEditorDoc()
{
	// TODO: 在此添加一次性构造代码

}

CFUIEditorDoc::~CFUIEditorDoc()
{
}

BOOL CFUIEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CFUIEditorDoc 序列化

void CFUIEditorDoc::Serialize(CArchive& ar)
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


// CFUIEditorDoc 诊断

#ifdef _DEBUG
void CFUIEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFUIEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFUIEditorDoc 命令
