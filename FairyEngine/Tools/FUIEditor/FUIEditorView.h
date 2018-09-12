// FUIEditorView.h : CFUIEditorView ��Ľӿ�
//


#pragma once


class CFUIEditorView : public CView
{
protected: // �������л�����
	CFUIEditorView();
	DECLARE_DYNCREATE(CFUIEditorView)

// ����
public:
	CFUIEditorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CFUIEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // FUIEditorView.cpp �еĵ��԰汾
inline CFUIEditorDoc* CFUIEditorView::GetDocument() const
   { return reinterpret_cast<CFUIEditorDoc*>(m_pDocument); }
#endif

