// FairyPacker.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "FairyExplorer.h"
#include "FairyPacker.h"
#include "MainFrm.h"

#include "FairyPackerDoc.h"
#include "FairyPackerView.h"
#include "ProgressDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFairyPackerApp

BEGIN_MESSAGE_MAP(CFairyPackerApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &CFairyPackerApp::OnAppAbout)
    // 基于文件的标准文档命令
    ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CFairyPackerApp 构造

CFairyPackerApp::CFairyPackerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFairyPackerApp 对象

CFairyPackerApp theApp;


// CFairyPackerApp 初始化

BOOL CFairyPackerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFairyPackerDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CFairyPackerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

    // 启用“DDE 执行”
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生
    m_pMainWnd->DragAcceptFiles();

    // 创建临时文件夹
    TCHAR tempDir[MAX_PATH];
    ::GetTempPath( MAX_PATH,tempDir );
    _tcscat_s( tempDir,MAX_PATH,_T("__FPK_TEMP__") );
    CreateDirectory( tempDir,NULL );

    // 加载配置文件
    LoadConfigData( _T("fpkconfig.ini") );

	return TRUE;
}

// 加载配置文件
bool CFairyPackerApp::LoadConfigData( const WString& filename )
{
    FIniFile file;
    if( !file.LoadFile(F_UnicodeToUTF8(filename).c_str()) )
    {
        // 取缺省值
        g_Config.compress = FPK_COMP_ZLIB;
        g_Config.bufferLevel = FPK_QUALITY_NORMAL;
        SaveConfigData( filename );
        return false;
    }

    AString item;
    item = file.GetSetting("fpk", "compress");
    g_Config.compress = (item == "zlib") ? FPK_COMP_ZLIB : FPK_COMP_MINILZO;
    item = file.GetSetting("fpk", "buffer");
    if( item == "low" )
        g_Config.bufferLevel = FPK_QUALITY_LOW;
    else if( item == "normal" )
        g_Config.bufferLevel = FPK_QUALITY_NORMAL;
    else
        g_Config.bufferLevel = FPK_QUALITY_HIGH;

    int nCount;
    item = file.GetSetting( "file", "no_compress_count" );
    sscanf( item.c_str(), "%d", &nCount );
    g_Config.extList.resize( nCount );
    for( int i=0;i<nCount;++i )
    {
        char sz[40];
        sprintf_s( sz, 40, "ext%d", i );
        item = file.GetSetting( "file", sz );
        g_Config.extList[i] = item;
    }

    return true;
}

// 写入配置文件
void CFairyPackerApp::SaveConfigData( const WString& filename )
{
    TCHAR szExePath[MAX_PATH];
    GetModuleFileName( NULL,szExePath,MAX_PATH );
    WString cfgFile,sExeName;
    WStringUtil::SplitPath( szExePath, sExeName, cfgFile );
    cfgFile += filename;

    FILE* pFile = _tfopen( cfgFile.c_str(), _T("w") );
    if( !pFile ) return;
    fprintf( pFile,"[fpk]\n" );
    fprintf( pFile,"compress = %s\n",g_Config.compress == FPK_COMP_ZLIB ? "zlib" : "minilzo" );
    if( g_Config.bufferLevel == FPK_QUALITY_LOW )
        fprintf( pFile,"buffer = low\n" );
    else if( g_Config.bufferLevel == FPK_QUALITY_NORMAL )
        fprintf( pFile,"buffer = normal\n" );
    else if( g_Config.bufferLevel == FPK_QUALITY_HIGH )
        fprintf( pFile,"buffer = high\n" );
    fprintf( pFile,"\n[file]\n" );
    fprintf( pFile,"no_compress_count = %d\n",g_Config.extList.size() );
    for( size_t i=0;i<g_Config.extList.size();++i )
    {
        fprintf( pFile,"ext%d = %s\n",i,g_Config.extList[i].c_str() );
    }
    fclose( pFile );
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CFairyPackerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CFairyPackerApp::DeleteDirectory( CString dirName )
{
    CFileFind tempFind;
    CString strFind = dirName + _T("\\*.*");

    BOOL found = tempFind.FindFile( strFind );
    if( found )
    {
        while( found )
        {
            found = tempFind.FindNextFile();

            if( !tempFind.IsDots() )
            {
                if( tempFind.IsDirectory() )
                {
                    CString tempDir = dirName + _T("\\") + tempFind.GetFileName();
                    DeleteDirectory( tempDir );
                }
                else
                {
                    CString tempFile = dirName + _T("\\") + tempFind.GetFileName();
                    DeleteFile( tempFile );
                }
            }
        }

        RemoveDirectory( dirName );
    }

    tempFind.Close();
}

// CFairyPackerApp 消息处理程序
int CFairyPackerApp::ExitInstance()
{
    // TODO: 在此添加专用代码和/或调用基类

    // 删除临时文件夹
    TCHAR tempPath[MAX_PATH];
    ::GetTempPath( MAX_PATH, tempPath );
    _tcscat_s( tempPath, _T("__FPK_TEMP__") );
    CFileFind tempFind;
    if( tempFind.FindFile(tempPath) )
    {
        DeleteDirectory( tempPath );
    }
    tempFind.Close();

    return CWinApp::ExitInstance();
}

// 开启一个操作
int ProcessOperation( int op,CFairyExplorer* exp,int fileCount,const FFilePairList& fileList )
{
    CProgressDlg dlg;
    dlg.m_iOperationType = op;
    dlg.m_nFileCount = fileCount;
    dlg.m_pExp = exp;
    dlg.m_FileList = const_cast<FFilePairList*>(&fileList);
    dlg.DoModal();
    return dlg.m_dwThreadRet;
}