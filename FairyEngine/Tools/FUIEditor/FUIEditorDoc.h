// FUIEditorDoc.h : CFUIEditorDoc ��Ľӿ�
//


#pragma once


class CFUIEditorDoc : public CDocument
{
protected: // �������л�����
	CFUIEditorDoc();
	DECLARE_DYNCREATE(CFUIEditorDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CFUIEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


