// FairyPacker.h : FairyPacker Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CFairyPackerApp:
// �йش����ʵ�֣������ FairyPacker.cpp
//

class CFairyPackerApp : public CWinApp
{
public:
	CFairyPackerApp();

    // ɾ��ָ����Ŀ¼
    void DeleteDirectory( CString dirName );

    // ����������Ϣ
    bool LoadConfigData( const WString& filename );
    // ����������Ϣ
    void SaveConfigData( const WString& filename );

// ��д
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFairyPackerApp theApp;

enum
{
    FAIRY_OP_OPEN,      // ���ļ���
    FAIRY_OP_ADDFILE,   // ����ļ�
    FAIRY_OP_EXTRACT,   // ��ѹ�ļ�
    FAIRY_OP_DEFRAG,    // �����ļ�����Ƭ
};

// ͨ����̨�߳̿���һ������
// Param [0] ��������
// Param [1] �ļ������ָ��
// Param [2] �����ļ��ĸ���
// Param [3] Ҫ������ļ��б�
// Return ��������1 �ɹ���0 ʧ��
extern int ProcessOperation( int op, CFairyExplorer* exp, int fileCount, const FFilePairList& fileList );