// FairyPacker.cpp : ����Ӧ�ó��������Ϊ��
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
    // �����ļ��ı�׼�ĵ�����
    ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CFairyPackerApp ����

CFairyPackerApp::CFairyPackerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFairyPackerApp ����

CFairyPackerApp theApp;


// CFairyPackerApp ��ʼ��

BOOL CFairyPackerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFairyPackerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CFairyPackerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

    // ���á�DDE ִ�С�
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
    m_pMainWnd->DragAcceptFiles();

    // ������ʱ�ļ���
    TCHAR tempDir[MAX_PATH];
    ::GetTempPath( MAX_PATH,tempDir );
    _tcscat_s( tempDir,MAX_PATH,_T("__FPK_TEMP__") );
    CreateDirectory( tempDir,NULL );

    // ���������ļ�
    LoadConfigData( _T("fpkconfig.ini") );

	return TRUE;
}

// ���������ļ�
bool CFairyPackerApp::LoadConfigData( const WString& filename )
{
    FIniFile file;
    if( !file.LoadFile(F_UnicodeToUTF8(filename).c_str()) )
    {
        // ȡȱʡֵ
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

// д�������ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
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

// CFairyPackerApp ��Ϣ�������
int CFairyPackerApp::ExitInstance()
{
    // TODO: �ڴ����ר�ô����/����û���

    // ɾ����ʱ�ļ���
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

// ����һ������
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