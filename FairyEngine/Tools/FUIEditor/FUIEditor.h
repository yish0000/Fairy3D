// FUIEditor.h : FUIEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CFUIEditorApp:
// �йش����ʵ�֣������ FUIEditor.cpp
//

class CFUIEditorApp : public CWinApp
{
public:
	CFUIEditorApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFUIEditorApp theApp;