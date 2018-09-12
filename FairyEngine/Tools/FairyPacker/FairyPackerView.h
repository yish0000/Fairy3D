// FairyPackerView.h : CFairyPackerView 类的接口
//


#pragma once


class CFairyPackerView : public CListView
{
protected: // 仅从序列化创建
	CFairyPackerView();
	DECLARE_DYNCREATE(CFairyPackerView)

// 属性
public:
	CFairyPackerDoc* GetDocument() const;

// 操作
public:
    // ListView排序的回调函数
    static int CALLBACK SortFunc( LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort );

    // 获取当前选定的节点
    CFairyExplorer::PKNode* GetSelectedNode(void);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
    void OnOpenNode( CFairyExplorer::PKNode* pNode );

// 实现
public:
	virtual ~CFairyPackerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DWORD m_dwStyle;    // ListView当前的显示风格
    int m_nSortMode;    // 当前的排序模式
    bool m_bSortBack;   // 反向排序？

// 生成的消息映射函数
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

#ifndef _DEBUG  // FairyPackerView.cpp 中的调试版本
inline CFairyPackerDoc* CFairyPackerView::GetDocument() const
   { return reinterpret_cast<CFairyPackerDoc*>(m_pDocument); }
#endif

