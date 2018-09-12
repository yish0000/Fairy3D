#pragma once


// CFairyTreeView 视图

class CFairyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CFairyTreeView)

protected:
	CFairyTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CFairyTreeView();

public:
    CFairyPackerDoc* GetDocument() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
    CImageList m_ImageList;

protected:
	DECLARE_MESSAGE_MAP()
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
    // 更新指定的项
    void UpdateNode( HTREEITEM hItem,CFairyExplorer::PKNode* node );
public:
    afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // FairyTreeView.cpp 中的调试版本
inline CFairyPackerDoc* CFairyTreeView::GetDocument() const
{ return reinterpret_cast<CFairyPackerDoc*>(m_pDocument); }
#endif
