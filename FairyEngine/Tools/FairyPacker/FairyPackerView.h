// FairyPackerView.h : CFairyPackerView ��Ľӿ�
//


#pragma once


class CFairyPackerView : public CListView
{
protected: // �������л�����
	CFairyPackerView();
	DECLARE_DYNCREATE(CFairyPackerView)

// ����
public:
	CFairyPackerDoc* GetDocument() const;

// ����
public:
    // ListView����Ļص�����
    static int CALLBACK SortFunc( LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort );

    // ��ȡ��ǰѡ���Ľڵ�
    CFairyExplorer::PKNode* GetSelectedNode(void);

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
    void OnOpenNode( CFairyExplorer::PKNode* pNode );

// ʵ��
public:
	virtual ~CFairyPackerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DWORD m_dwStyle;    // ListView��ǰ����ʾ���
    int m_nSortMode;    // ��ǰ������ģʽ
    bool m_bSortBack;   // ��������

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnViewLargeicon();
    afx_msg void OnViewSmallicon();
    afx_msg void OnViewList();
    afx_msg void OnViewDetails();
    afx_msg void OnUpdateViewLargeicon(CCmdUI *pCmdUI);
    afx_msg void OnUpdateViewSmallicon(CCmdUI *pCmdUI);
    afx_msg void OnUpdateViewList(CCmdUI *pCmdUI);
    afx_msg void OnUpdateViewDetails(CCmdUI *pCmdUI);
    afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnUpdateEditOpen(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEditAddfile(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEditAddfolder(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEditDelete(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEditExtract(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEditExtractto(CCmdUI *pCmdUI);
    afx_msg void OnEditOpen();
    afx_msg void OnEditExtract();
    afx_msg void OnEditExtractto();
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnEditAddfile();
    afx_msg void OnEditAddfolder();
    afx_msg void OnEditDelete();
    afx_msg void OnEditDedrag();
    afx_msg void OnUpdateEditDedrag(CCmdUI *pCmdUI);
    afx_msg void OnEditSelectall();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditSearchfiles();
	afx_msg void OnUpdateEditSearchfiles(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // FairyPackerView.cpp �еĵ��԰汾
inline CFairyPackerDoc* CFairyPackerView::GetDocument() const
   { return reinterpret_cast<CFairyPackerDoc*>(m_pDocument); }
#endif

