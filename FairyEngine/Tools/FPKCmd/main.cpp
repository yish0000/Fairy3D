/*
 * ------------------------------------------------------------------------
 *  Name:   main.cpp
 *  Desc:   命令行方式处理FPK文件包。
 *  Author: Yish
 *  Date:   2011/11/13
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include <stdio.h>
#include "FairyExplorer.h"
#include <FCommon.h>

///////////////////////////////////////////////////////////////////////////
//
//  Define and Macros
//
///////////////////////////////////////////////////////////////////////////

#ifndef MAX_PATH
    #define MAX_PATH 260
#endif

#define CMD_PROC(cmd) void cmd##_proc( const AStringList& params )
#define REGISTER_CMD(cmd) g_CmdMap[#cmd] = &cmd##_proc;
#define REGISTER_CMD_ALIAS(alias, cmd) g_CmdMap[#alias] = &cmd##_proc;
#define PRINT_HELP(cmd, param, desc, detail) \
    if( pa == "" ) printf("%-15s%s\n", #cmd, desc); \
    else if( strstr(#cmd, pa.c_str()) ) printf("Usage: %s %s\n%s\n%s\n", #cmd, param, desc, detail)

#define CHECK_ARGNUM(cmd, num) \
    if( params.size() < num ) { \
        printf("Insufficient num of parameters, Enter 'help %s' for more information.\n", #cmd); \
        return; \
    }

typedef void (*FN_CMD_PROC) ( const AStringList& params );

///////////////////////////////////////////////////////////////////////////
//
//  Predeclare the functions
//
///////////////////////////////////////////////////////////////////////////

void PrintLogo();
void PrintHelpInfo();

///////////////////////////////////////////////////////////////////////////
//
//  class CFileDupHandler
//
///////////////////////////////////////////////////////////////////////////

class CFileDupHandler : public IFileDupHandler
{
public:
    
    enum
    {
        ALL_YES = 1,
        ALL_NO,
        ALL_CMDLINE,    // For Command line.
    };
    
public:
    CFileDupHandler() : m_iState(0) {}
    
    void Reset() { m_iState = 0; }
    int GetState() const { return m_iState; }
    void SetState(int iState) { m_iState = iState; }
    
    // If return true, we will replace the duplicated file.
    bool Handle(const AString& filename)
    {
        if( m_iState == ALL_YES || m_iState == ALL_CMDLINE )
            return true;
        else if( m_iState == ALL_NO )
            return false;
        
        char reply[20];
        printf( "Package already has the file(%s)! To cover it? (Y or N for this time, YA or NA for all)", F_UTF8ToANSI(filename).c_str() );
        fgets(reply, sizeof(reply), stdin);
        if( reply[0] == 'y' || reply[0] == 'Y' )
        {
            if( reply[1] == 'a' || reply[1] == 'A' )
                m_iState = ALL_YES;
            return true;
        }
        else
        {
            if( reply[1] == 'a' || reply[1] == 'A' )
                m_iState = ALL_NO;
            return false;
        }
    }
    
protected:
    int m_iState;
};

///////////////////////////////////////////////////////////////////////////
//
//  Local Type and Global variables
//
///////////////////////////////////////////////////////////////////////////

std::map<AString, FN_CMD_PROC> g_CmdMap;
CFairyExplorer g_FPK;
bool g_bAppRun = true;
int g_nCurFile;
int g_nCount;
CFileDupHandler g_DupHandler;

///////////////////////////////////////////////////////////////////////////
//
//  Callback function when handle the package
//
///////////////////////////////////////////////////////////////////////////

bool CB_Adding( const AString& filename )
{
    printf( "Adding file %s \n", F_UTF8ToANSI(filename).c_str() );
    return true;
}

bool CB_Extracting( const AString& filename )
{
    printf( "Extracting file %s \n", F_UTF8ToANSI(filename).c_str() );
    return true;
}

bool CB_Defrag( const AString& filename )
{
    float fProgess = (float)g_nCurFile / g_nCount * 100;
    printf( "Package is defraging ... %.3f%%\r", fProgess );
    ++g_nCurFile;
    return true;
}

///////////////////////////////////////////////////////////////////////////
//
//  Handle functions of the commands
//
///////////////////////////////////////////////////////////////////////////

CMD_PROC( open )
{
    CHECK_ARGNUM(open, 1);
    
    if( !g_FPK.Open(params[0]) )
        printf( "Open package %s failed!!\n", params[0].c_str() );
    else
    {
        if( g_FPK.IsReadonly() )
            printf( "Package %s(readonly) is opened successfully!!\n", g_FPK.GetPackageName().c_str() );
        else
            printf( "Package %s is opened successfully!!\n", g_FPK.GetPackageName().c_str() );
    }
}

CMD_PROC( create )
{
    CHECK_ARGNUM(create, 1);
    
    if( !g_FPK.Create(params[0]) )
        printf( "Create package %s failed!\n", params[0].c_str() );
    else
        printf( "Package %s is created successfully!!\n", params[0].c_str() );
}

CMD_PROC( close )
{
    if( g_FPK.IsOpen() )
    {
        AString name = g_FPK.GetPackageName();
        g_FPK.Close();
        printf( "Package %s has been closed!!\n", name.c_str() );
    }
    else
    {
        printf( "No package is opened!!\n" );
    }
}

CMD_PROC( cd )
{
    CHECK_ARGNUM(cd, 1);
    
    if( !g_FPK.IsOpen() )
        printf( "No package is opened!!\n" );
    else
    {
        if( !g_FPK.EnterFolder(F_ANSIToUTF8(params[0])) )
            printf( "Invalid folder!!\n" );
    }
}

CMD_PROC( dir )
{
    if( !g_FPK.IsOpen() )
        printf( "No package is opened!!\n" );
    else
    {
        std::list<CFairyExplorer::PKNode>::const_iterator it;
        const CFairyExplorer::PKNode* pNode = g_FPK.GetCurrentNode();
        FASSERT( pNode );
        for( it=pNode->Children.begin();it!=pNode->Children.end();++it )
        {
            if( it->bIsDirectory )
                printf( "    <dir>  %s\n", F_UTF8ToANSI(it->strName).c_str() );
            else
                printf( "    <file> %s\n", F_UTF8ToANSI(it->strName).c_str() );
        }
    }
}

CMD_PROC( add )
{
    CHECK_ARGNUM(add, 1);
    
    if( !g_FPK.IsOpen() )
        printf( "No package is opened!!\n" );
    else
    {
        AString toDir;
        g_FPK.SetFileDupHandler(&g_DupHandler);
        g_FPK.SetCallback( CB_Adding );
        toDir = (params.size() >= 2) ? params[1] : "";
        if( !g_FPK.Add(params[0], F_ANSIToUTF8(toDir)) )
            printf( "Adding \"%s\" to package failed!!\n", params[0].c_str() );

        if( g_DupHandler.GetState() != CFileDupHandler::ALL_CMDLINE )
            g_DupHandler.Reset();
    }
}

CMD_PROC( extract )
{
    CHECK_ARGNUM(extract, 1);
    
    if( !g_FPK.IsOpen() )
        printf( "No package is opened!!\n" );
    else
    {
        AString toDir;
        g_FPK.SetCallback( CB_Extracting );
        toDir = (params.size() >= 2) ? params[1] : "";
        if( !g_FPK.Extract(F_ANSIToUTF8(params[0]), toDir) )
            printf( "Extracting file from package failed!!\n" );
    }
}

CMD_PROC( del )
{
    CHECK_ARGNUM(del, 1);
    
    if( !g_FPK.IsOpen() || g_FPK.IsReadonly() )
        printf( "No package is opened or the package is readonly!!\n" );
    else
    {
        if( !g_FPK.Remove(F_ANSIToUTF8(params[0])) )
            printf( "Can't find the specified target!!\n" );
    }
}

CMD_PROC( find )
{
	CHECK_ARGNUM(find, 1);

	if( !g_FPK.IsOpen() )
		printf("No package is opened!!\n");
	else
	{
		size_t i;
		bool bExtract = false;
		AStringList fileList;
		AString extractTo, path, fileKey;

		for(i=0; i<params.size(); ++i)
		{
			if( params[i] == ">>" )
			{
				bExtract = true;
				break;
			}
		}

		if( i == 2 )
		{
			path = params[0];
			fileKey = params[1];
		}
		else if( i == 1 )
		{
			fileKey = params[0];
		}
		else
		{
			printf("Insufficient num of parameters, Enter 'help help' for more information.\n");
			return;
		}

		if( bExtract )
		{
			if( i < params.size() - 1 )
				extractTo = params[params.size() - 1];
			else
			{
				printf("Insufficient num of parameters, Enter 'help help' for more information.\n");
				return;
			}
		}

		if( !path.empty() )
			g_FPK.SearchFiles(F_ANSIToUTF8(path), F_ANSIToUTF8(fileKey), fileList);
		else
			g_FPK.SearchFiles(F_ANSIToUTF8(fileKey), fileList);

		if( bExtract )
			g_FPK.SetCallback(CB_Extracting);
		for(AStringList::iterator it=fileList.begin(); it!=fileList.end(); ++it)
		{
			if( bExtract )
			{
				g_FPK.Extract("/" + (*it), extractTo);
			}
			else
				printf("%s\n", F_UTF8ToANSI(*it).c_str());
		}
	}
}

CMD_PROC( frag )
{
    if( !g_FPK.IsOpen() )
        printf( "No package is opened!!\n" );
    else
    {
        uint32 fagSize = g_FPK.GetPackage()->GetFragmentSize();
        if( fagSize == 0 )
            printf( "Congratulations, There is no fragments!!\n" );
        else
            printf( "Fragment size: %u, you'd better defrag the package!!\n", fagSize );
    }
}

CMD_PROC( defrag )
{
    if( !g_FPK.IsOpen() )
        printf( "No package is opened!!\n" );
    else
    {
        g_nCurFile = 0;
        g_nCount = g_FPK.GetNodeFileCount( g_FPK.GetRootNode() );
        uint32 fagSize = g_FPK.GetPackage()->GetFragmentSize();
        if( fagSize == 0 )
            printf( "There is no fragments!!\n" );
        else
        {
            g_FPK.SetCallback( CB_Defrag );
            g_FPK.Defrag();
            printf( "Fragments have been deleted, saved %u bytes for you!!\n", fagSize );
        }
    }
}

CMD_PROC( info )
{
    if( !g_FPK.IsOpen() )
        printf( ("No package is opened!!\n") );
    else
    {
        SPackageInfo fpkInfo;
        g_FPK.GetPackageInfo( fpkInfo );
        printf( ("Folder count:      %d\n"), fpkInfo.folderCount );
        printf( ("File count:        %d\n"), fpkInfo.fileCount );
        printf( ("All file size:     %llu\n"), fpkInfo.origSize );
        printf( ("Compressed size:   %u\n"), fpkInfo.compressSize );
        printf( ("Compression Ratio: %.3f%%\n\n"), fpkInfo.compressDegree*100.0f );
    }
}

CMD_PROC( help )
{
    AString pa = params.size() ? params[0] : AString("");
    PRINT_HELP(open, ("[path]"), ("Open a specified package."), (""));
    PRINT_HELP(create, ("[path] [folder]"), ("Create a specified package."),
               ("The folder which we can make to a package is optional."));
    PRINT_HELP(close, (""), ("Close the current package."), (""));
    PRINT_HELP(cd,  ("[folder]"), ("Enter the specified folder."), (""));
    PRINT_HELP(dir/ls, (""), ("List the current folder's elements."), (""));
    PRINT_HELP(add, ("[source] [dest]"), ("Add the specified source to the package."), (""));
    PRINT_HELP(extract, ("[source] [dest]"), ("Extract the specified file to the target folder."), (""));
    PRINT_HELP(del/rm, ("[path]"), ("Delete the specified target in the package."), (""));
	PRINT_HELP(find, ("[path] [file] [>> dest dir]"), ("Find the specified file."), (""));
    PRINT_HELP(frag, (""), ("Look up the fragment size of the package."), (""));
    PRINT_HELP(defrag, (""), ("Delete the fragments of the package to save the space."), (""));
    PRINT_HELP(info, (""), ("Look up the information of the package."), (""));
    PRINT_HELP(help, (""), ("Look up the help infomation of the application."), (""));
    PRINT_HELP(getconfig, (""), ("Get the application's config data."), (""));
    PRINT_HELP(setconfig, "[operation] [value]", "Set the application's config data.",
               "Operation: \n"
               "    compression, to set the compression algorithm(zlib,minilzo);\n"
               "    buffer, to set the compression buffer level(low,normal,high);\n"
               "    add_ext, to add the file extension which is not compressed;\n"
               "    del_ext, to delete the file extension which is not compressed.");
    PRINT_HELP(quit/exit, (""), ("Exit the application."), (""));
}

CMD_PROC( setconfig )
{
    CHECK_ARGNUM(setconfig, 2);
    
    if( params[0] == ("compression") )
    {
        if( params[1] == ("zlib") )
            g_Config.compress = FPK_COMP_ZLIB;
        else if( params[1] == ("minilzo") )
            g_Config.compress = FPK_COMP_MINILZO;
        else
            printf( ("Unknown compression algorithm!!\n") );
    }
    else if( params[0] == ("buffer") )
    {
        if( params[1] == ("low") )
            g_Config.bufferLevel = FPK_QUALITY_LOW;
        else if( params[1] == ("normal") )
            g_Config.bufferLevel = FPK_QUALITY_NORMAL;
        else if( params[1] == ("high") )
            g_Config.bufferLevel = FPK_QUALITY_HIGH;
        else if( params[1] == ("entire") )
            g_Config.bufferLevel = FPK_QUALITY_ENTIRE;
        else
            printf( ("Unknown buffer level!!\n") );
    }
    else if( params[0] == ("add_ext") )
    {
        AStringList::iterator it;
        it = std::find( g_Config.extList.begin(),g_Config.extList.end(),params[1] );
        if( it != g_Config.extList.end() )
            printf( ("The ext has existed!!\n") );
        else
            g_Config.extList.push_back( params[1] );
    }
    else if( params[0] == ("del_ext") )
    {
        AStringList::iterator it;
        it = std::find( g_Config.extList.begin(),g_Config.extList.end(),params[1] );
        if( it == g_Config.extList.end() )
            printf( ("Can't find the ext!!\n") );
        else
            g_Config.extList.erase( it );
    }
    else
        printf( ("Invalid operation! Enter 'help setconfig' for more information.\n") );
}

CMD_PROC( getconfig )
{
    if( g_Config.compress == FPK_COMP_ZLIB )
        printf( ("Compression:  zlib\n") );
    else
        printf( ("Compression:  minilzo\n") );
    if( g_Config.bufferLevel == FPK_QUALITY_ENTIRE )
        printf( ("Buffer Level: entire\n") );
    else if( g_Config.bufferLevel == FPK_QUALITY_LOW )
        printf( ("Buffer Level: low\n") );
    else if( g_Config.bufferLevel == FPK_QUALITY_NORMAL )
        printf( ("Buffer Level: normal\n") );
    else
        printf( ("Buffer Level: high\n") );
    printf( ("File extensions which are not compressed:\n") );
    for( size_t i=0;i<g_Config.extList.size();++i )
        printf( ("*.%s\n"),g_Config.extList[i].c_str() );
}

CMD_PROC( quit )
{
    g_bAppRun = false;
}

///////////////////////////////////////////////////////////////////////////
//
//  Handlers for command line.
//
///////////////////////////////////////////////////////////////////////////

void PrintUsage()
{
    printf("fpack: illegal option.\n");
    
    printf("usage: fpack [-c] [Package Name] [Input targets...]\n");
    printf("       fpack [-a] [Package Name] [-l] [Location to add] [Input targets...]\n");
    printf("       fpack [-e] [Package Name] [-l] [Location to extract] [Which targets to extract...]\n");
	printf("       fpack [-f] [Package Name] [The directory to search] [Search patterns] [>>] [Location to extract the files] \n");
	printf("       fpack [-r] [Package Name] [Targets to delete...]\n");
    printf("       fpack [-df] [Package Name]\n\n");
}

void ProcessCommandLine(int argc, char* argv[])
{
    char* param1 = argv[1];
    
    if( !strcmp(param1, "-c") )
    {
        if( argc >= 4 )
        {
            // Create the fpk.
            AStringList params;
            params.push_back(argv[2]);
            create_proc(params);
            
            g_DupHandler.SetState(CFileDupHandler::ALL_CMDLINE);
            
            // Add files to package.
            int targetCount = argc - 3;
            for(int i=0; i<targetCount; i++)
            {
                params.clear();
                params.push_back(argv[3+i]);
                add_proc(params);
            }
            
            g_DupHandler.SetState(0);
            
            params.clear();
            close_proc(params);
        }
        else
        {
            printf("Illegal parameters. [Package Name] [Input targets...]\n");
        }
    }
    else if( !strcmp(param1, "-a") )
    {
        if( argc >= 4 )
        {
            // Open the fpk.
            if( !g_FPK.Open(argv[2]) )
            {
                printf( ("Open package %s failed!!\n"), argv[2] );
                return;
            }
            else
            {
                if( g_FPK.IsReadonly() )
                {
                    printf( ("Package %s(readonly) is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
                    return;
                }
                else
                    printf( ("Package %s is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
            }
            
            g_DupHandler.SetState(CFileDupHandler::ALL_CMDLINE);
            
            // Add files to package.
            AStringList params;
            int targetCount = argc - 3;
            for(int i=0; i<targetCount; i++)
            {
                params.clear();
                params.push_back(argv[3+i]);
                add_proc(params);
            }
            
            g_DupHandler.SetState(0);
            
            params.clear();
            close_proc(params);
        }
        else
        {
            printf("Illegal parameters. [Package Name] [Input targets...]\n");
        }
    }
    else if( !strcmp(param1, "-e") )
    {
        if( argc >= 5 )
        {
            // Open the fpk.
            if( !g_FPK.Open(argv[2]) )
            {
                printf( ("Open package %s failed!!\n"), argv[2] );
                return;
            }
            else
            {
                if( g_FPK.IsReadonly() )
                    printf( ("Package %s(readonly) is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
                else
                    printf( ("Package %s is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
            }
            
            // Extract the files.
            AStringList params;
            int targetCount = argc - 4;
            for(int i=0; i<targetCount; i++)
            {
                params.clear();
                params.push_back(argv[4+i]);
                params.push_back(argv[3]);
                extract_proc(params);
            }
            
            params.clear();
            close_proc(params);
        }
        else
        {
            printf("Illegal parameters. [Package Name] [Location to extract] [Which targets to extract...]\n");
        }
    }
    else if( !strcmp(param1, "-r") )
    {
        if( argc >= 4 )
        {
            // Open the fpk.
            if( !g_FPK.Open(argv[2]) )
            {
                printf( ("Open package %s failed!!\n"), argv[2] );
                return;
            }
            else
            {
                if( g_FPK.IsReadonly() )
                {
                    printf( ("Package %s(readonly) is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
                    return;
                }
                else
                    printf( ("Package %s is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
            }
            
            // Delete the files.
            AStringList params;
            int targetCount = argc - 3;
            for(int i=0; i<targetCount; i++)
            {
                params.clear();
                params.push_back(argv[3+i]);
                del_proc(params);
            }
            
            params.clear();
            close_proc(params);
        }
        else
        {
            printf("Illegal parameters. [Package Name] [Targets to delete...]");
        }
    }
	else if( !strcmp(param1, "-f") )
	{
		printf("       fpack [-f] [Package Name] [The directory to search] [Search patterns] [>>] [Location to extract the files] \n");
		
		if( argc >= 4 )
		{
			// Open the fpk.
			if( !g_FPK.Open(argv[2]) )
			{
				printf( ("Open package %s failed!!\n"), argv[2] );
				return;
			}
			else
			{
				printf( ("Package %s is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
			}

			bool bExport = false;
			AString extractTo;
			AStringList fileList;
			if( argc == 4 )
				g_FPK.SearchFiles(argv[3], fileList);
			else if( argc == 5 )
				g_FPK.SearchFiles(argv[3], argv[4], fileList);
			else if( argc == 6 )
			{
				if( !strcmp(argv[4], ">>") )
				{
					g_FPK.SearchFiles(argv[3], fileList);
					bExport = true;
					extractTo = argv[5];
				}
				else
				{
					g_FPK.SearchFiles(argv[3], argv[4], fileList);
				}
			}
			else if( argc == 7 )
			{
				if( !strcmp(argv[5], ">>") )
				{
					g_FPK.SearchFiles(argv[3], argv[4], fileList);
					bExport = true;
					extractTo = argv[6];
				}
				else
				{
					g_FPK.SearchFiles(argv[3], argv[4], fileList);
				}
			}

			for(AStringList::iterator it=fileList.begin(); it!=fileList.end(); ++it)
			{
				if( bExport )
					g_FPK.Extract((*it), extractTo);
				else
					printf("%s\n", F_UTF8ToANSI(*it).c_str());
			}
		}
		else
		{
			printf("Illegal parameters. [-f] [Package Name] [The directory to search] [Search patterns] [>>] [Location to extract the files]\n");
		}
	}
    else if( !strcmp(param1, "-df") )
    {
        if( argc >= 3 )
        {
            // Open the fpk.
            if( !g_FPK.Open(argv[2]) )
            {
                printf( ("Open package %s failed!!\n"), argv[2] );
                return;
            }
            else
            {
                if( g_FPK.IsReadonly() )
                {
                    printf( ("Package %s(readonly) is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
                    return;
                }
                else
                    printf( ("Package %s is opened successfully!!\n"), g_FPK.GetPackageName().c_str() );
            }
            
            g_FPK.Defrag();
            
            AStringList params;
            params.clear();
            close_proc(params);
        }
        else
        {
            printf("Illegal parameters. [Package Name]");
        }
    }
    else
    {
        PrintUsage();
    }
}

///////////////////////////////////////////////////////////////////////////
//
//  Main function
//
///////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    PrintLogo();

    // 默认压缩配置
    g_Config.compress = FPK_COMP_ZLIB;
    g_Config.bufferLevel = FPK_QUALITY_NORMAL;
    
    // 处理命令行参数
    if( argc > 1 )
    {
        ProcessCommandLine(argc, argv);
        return 0;
    }
    
    // 注册所有命令
    REGISTER_CMD( open );
    REGISTER_CMD( create );
    REGISTER_CMD( close );
    REGISTER_CMD( cd );
    REGISTER_CMD( dir );
    REGISTER_CMD( add );
    REGISTER_CMD( extract );
    REGISTER_CMD( del );
	REGISTER_CMD( find );
    REGISTER_CMD( frag );
    REGISTER_CMD( defrag );
    REGISTER_CMD( info );
    REGISTER_CMD( help );
    REGISTER_CMD( setconfig );
    REGISTER_CMD( getconfig );
    REGISTER_CMD( quit );

	// 命令别名
	REGISTER_CMD_ALIAS( ls, dir );
	REGISTER_CMD_ALIAS( rm, del );
	REGISTER_CMD_ALIAS( exit, quit );
    
    while( g_bAppRun )
    {
        printf("/%s>", F_UTF8ToANSI(g_FPK.GetCurrentPath()).c_str());
        
        char szLine[1024];
        AStringList strCmdLine;
        fgets(szLine, sizeof(szLine), stdin);
        if( szLine[strlen(szLine)-1] == '\n' ) szLine[strlen(szLine)-1] = '\0';
        AStringUtil::Split(szLine, " ", strCmdLine);
        if( !strCmdLine.size() ) continue;
        std::map<AString, FN_CMD_PROC>::const_iterator it = g_CmdMap.find(strCmdLine[0]);
        if( it != g_CmdMap.end() )
        {
            strCmdLine.erase(strCmdLine.begin());
            it->second(strCmdLine);
        }
        else
        {
            printf("Invalid command!!\n Enter the 'help' for more information.\n");
        }
    }
    
    return 0;
}

void PrintLogo()
{
    printf( "Fairy3D Engine Package Packer\n" );
    printf( "CopyRight (C) Fairy & Pear Studio. All right observed.\n\n" );
}
