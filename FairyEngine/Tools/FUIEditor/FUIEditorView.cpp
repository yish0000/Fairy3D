// FUIEditorView.cpp : CFUIEditorView ���ʵ��
//

#include "stdafx.h"
#include "FUIEditor.h"

#include "FUIEditorDoc.h"
#include "FUIEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFUIEditorView

IMPLEMENT_DYNCREATE(CFUIEditorView, CView)

BEGIN_MESSAGE_MAP(CFUIEditorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFUIEditorView ����/����

CFUIEditorView::CFUIEditorView()
{
	// TODO: �ڴ˴���ӹ������

}

CFUIEditorView::~CFUIEditorView()
{
}

BOOL CFUIEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CFUIEditorView ����

void CFUIEditorView::OnDraw(CDC* /*pDC*/)
{
	CFUIEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CFUIEditorView ��ӡ

BOOL CFUIEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CFUIEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CFUIEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CFUIEditorView ���

#ifdef _DEBUG
void CFUIEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CFUIEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFUIEditorDoc* CFUIEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFUIEditorDoc)));
	return (CFUIEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CFUIEditorView ��Ϣ�������
