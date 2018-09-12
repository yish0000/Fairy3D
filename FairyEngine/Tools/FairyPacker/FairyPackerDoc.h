// FairyPackerDoc.h : CFairyPackerDoc 类的接口
//


#pragma once


class CFairyPackerDoc : public CDocument
{
protected: // 仅从序列化创建
	CFairyPackerDoc();
	DECLARE_DYNCREATE(CFairyPackerDoc)

// 属性
public:
    CFairyExplorer m_Explorer;

// 操作
public:
    CFairyExplorer* GetFPKExplorer() { return &m_Explorer; }

// 重写
public:
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CFairyPackerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};


