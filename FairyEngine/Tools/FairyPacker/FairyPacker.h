// FairyPacker.h : FairyPacker 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CFairyPackerApp:
// 有关此类的实现，请参阅 FairyPacker.cpp
//

class CFairyPackerApp : public CWinApp
{
public:
	CFairyPackerApp();

    // 删除指定的目录
    void DeleteDirectory( CString dirName );

    // 加载配置信息
    bool LoadConfigData( const WString& filename );
    // 保存配置信息
    void SaveConfigData( const WString& filename );

// 重写
public:
	virtual BOOL InitInstance();
    virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFairyPackerApp theApp;

enum
{
    FAIRY_OP_OPEN,      // 打开文件包
    FAIRY_OP_ADDFILE,   // 添加文件
    FAIRY_OP_EXTRACT,   // 解压文件
    FAIRY_OP_DEFRAG,    // 整理文件包碎片
};

// 通过后台线程开启一个操作
// Param [0] 操作类型
// Param [1] 文件浏览器指针
// Param [2] 处理文件的个数
// Param [3] 要处理的文件列表
// Return 处理结果，1 成功，0 失败
extern int ProcessOperation( int op, CFairyExplorer* exp, int fileCount, const FFilePairList& fileList );