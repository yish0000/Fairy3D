// FairyPackerDoc.cpp : CFairyPackerDoc ���ʵ��
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "FairyPackerDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFairyPackerDoc

IMPLEMENT_DYNCREATE(CFairyPackerDoc, CDocument)

BEGIN_MESSAGE_MAP(CFairyPackerDoc, CDocument)
END_MESSAGE_MAP()


// CFairyPackerDoc ����/����

CFairyPackerDoc::CFairyPackerDoc()
{
	// TODO: �ڴ����һ���Թ������
}

CFairyPackerDoc::~CFairyPackerDoc()
{
}

// CFairyPackerDoc ���л�

void CFairyPackerDoc::Serialize(CArchive& ar)
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


// CFairyPackerDoc ���

#ifdef _DEBUG
void CFairyPackerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFairyPackerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFairyPackerDoc ����

BOOL CFairyPackerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    CWaitCursor wait;

    // TODO:  �ڴ������ר�õĴ�������

    // ����ɵ�����
    if( m_Explorer.IsOpen() )
        m_Explorer.Close();

    // ���ļ���
    FFilePairList files;
    files.push_back( std::make_pair(F_UnicodeToUTF8(lpszPathName), "") );
    if( ProcessOperation(FAIRY_OP_OPEN,&m_Explorer,1,files) )
    {
        UpdateAllViews( NULL,1 );
        return FALSE;
    }

    // ˢ����ͼ
    UpdateAllViews( NULL,1 );
    return TRUE;
}

BOOL CFairyPackerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // TODO: �ڴ����ר�ô����/����û���
    m_Explorer.SaveAs( F_UnicodeToUTF8(lpszPathName) );
    return OnOpenDocument( lpszPathName );
}
