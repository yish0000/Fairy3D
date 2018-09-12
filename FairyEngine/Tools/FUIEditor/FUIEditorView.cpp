// FUIEditorView.cpp : CFUIEditorView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFUIEditorView 构造/析构

CFUIEditorView::CFUIEditorView()
{
	// TODO: 在此处添加构造代码

}

CFUIEditorView::~CFUIEditorView()
{
}

BOOL CFUIEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFUIEditorView 绘制

void CFUIEditorView::OnDraw(CDC* /*pDC*/)
{
	CFUIEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CFUIEditorView 打印

BOOL CFUIEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFUIEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFUIEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CFUIEditorView 诊断

#ifdef _DEBUG
void CFUIEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CFUIEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFUIEditorDoc* CFUIEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFUIEditorDoc)));
	return (CFUIEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CFUIEditorView 消息处理程序
