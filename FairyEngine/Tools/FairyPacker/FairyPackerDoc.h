// FairyPackerDoc.h : CFairyPackerDoc ��Ľӿ�
//


#pragma once


class CFairyPackerDoc : public CDocument
{
protected: // �������л�����
	CFairyPackerDoc();
	DECLARE_DYNCREATE(CFairyPackerDoc)

// ����
public:
    CFairyExplorer m_Explorer;

// ����
public:
    CFairyExplorer* GetFPKExplorer() { return &m_Explorer; }

// ��д
public:
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CFairyPackerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};


