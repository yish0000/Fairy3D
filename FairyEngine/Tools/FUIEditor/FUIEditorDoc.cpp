// FUIEditorDoc.cpp : CFUIEditorDoc ���ʵ��
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


// CFUIEditorDoc ����/����

CFUIEditorDoc::CFUIEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CFUIEditorDoc::~CFUIEditorDoc()
{
}

BOOL CFUIEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CFUIEditorDoc ���л�

void CFUIEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CFUIEditorDoc ���

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


// CFUIEditorDoc ����
