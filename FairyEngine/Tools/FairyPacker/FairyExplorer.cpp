/*
 * ------------------------------------------------------------------------
 *  Name:   FairyExplorer.cpp
 *  Desc:   本文件定义了一个资源包中文件的树状结构。
 *  Author: Yish
 *  Date:   2011/9/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Fairy & Pear Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FairyExplorer.h"
#include "FStrUtil.h"
#include "FLogManager.h"

#include <sys/stat.h>

#if FAIRY_PLATFORM != FAIRY_PLATFORM_WINDOWS
	#include <glob.h>
#endif

///////////////////////////////////////////////////////////////////////////
//// Local types and functions

SConfigData g_Config;

typedef bool (*EnumCallback) ( const AString& filename, void* param );

// 枚举指定的目录
static bool EnumFile( const AString& path, EnumCallback cb, void* param )
{
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW( F_UTF8ToUnicode((path + "*")).c_str(), &fd );
    
    if( hFind == INVALID_HANDLE_VALUE )
    {
        FindClose( hFind );
        return true;
    }
    
    while( true )
    {
        if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
        {
            if( !cb(path + F_UnicodeToUTF8(fd.cFileName), param) )
                return false;
        }
        else if( wcscmp(fd.cFileName, L".") != 0 && wcscmp(fd.cFileName, L"..") != 0 )
        {
            if( !EnumFile(path + F_UnicodeToUTF8(fd.cFileName) + SYS_DIR_CHAR, cb, param ) )
                return false;
        }
        
        if( !FindNextFileW(hFind, &fd) )
        {
            FindClose( hFind );
            return true;
        }
    }
    
    FindClose( hFind );
    return true;
#else
    glob_t globbuf;
    struct stat fileinfo;
    AString token = path;
    
    if( !cb ) return false;
    
    if( token[token.length() - 1] == '/' )
    {
        token += "*";
    }
    else
    {
        token += "/*";
    }
    
    int ret = glob(token.c_str(), GLOB_NOSORT, NULL, &globbuf);
    if( ret != 0 )
    {
        if( ret == GLOB_NOMATCH )
        {
            globfree(&globbuf);
            return true;
        }
        
        globfree(&globbuf);
        return false;
    }
    
    bool bFail = false;
    for(size_t i=0; i < globbuf.gl_pathc; ++i)
    {
        ret = lstat(globbuf.gl_pathv[i],&fileinfo);
        if( 1 == S_ISDIR(fileinfo.st_mode)  )
        {
            if( !EnumFile(globbuf.gl_pathv[i], cb, param) )
            {
                bFail = true;
                break;
            }
        }
        else
        {
            if( !cb(globbuf.gl_pathv[i], param) )
            {
                bFail = true;
                break;
            }
        }
    }
    
    globfree(&globbuf);
    return !bFail;
#endif
}

// 添加文件到压缩包
bool AddPackFile( const AString& path, void* param )
{
    CFairyExplorer* exp = (CFairyExplorer*)param;
    AString relativePath = path.substr(exp->m_sBasePath.length(), path.length()-exp->m_sBasePath.length());
    std::replace( relativePath.begin(), relativePath.end(), '\\', '/' );
    relativePath = exp->m_sWorkingPath + relativePath;
    return exp->AddFile( path, relativePath );
}

// 统计文件数目
static bool CountFile( const AString& path, void* param )
{
    uint32* nFileCount = (uint32*)param;
    ++(*nFileCount);
    return true;
}

// 处理掉路径中的.. .等特殊符号
static bool HandleWorkPath(AString& path)
{
    bool bIsAbs = AStringUtil::IsAbsPath(path);
    AStringList pathList, realPathList;
    std::replace(path.begin(), path.end(), '\\', '/');
    AStringUtil::Split(path, "/", pathList);
    for(size_t i=0; i<pathList.size(); ++i)
    {
        if( pathList[i] == "." )
            continue;
        else if( pathList[i] == ".." )
        {
            if( realPathList.size() > 0 )
            {
                realPathList.pop_back();
            }
            else
            {
                path = "";
                return false;
            }
        }
        else
        {
            realPathList.push_back(pathList[i]);
        }
    }
    
    path = bIsAbs ? "/" : "";
    for(AStringList::iterator it=realPathList.begin(); it!=realPathList.end(); ++it)
        path += *it + "/";
    
	// 去掉最后一个'/'
    if( path.length() > 0 )
	{
		path.resize(path.length() - 1);
	}

    return true;
}

///////////////////////////////////////////////////////////////////////////
/// Implements class CFairyExplorer

// 构造函数
CFairyExplorer::CFairyExplorer(void)
    : m_pPackage(NULL), m_sCurPath(""), m_sWorkingPath("")
    , m_sBasePath(""), m_pCallback(NULL)
{
    m_Root.bIsDirectory = true;
    m_Root.nFileSize = 0;
    m_Root.nCompressedSize = 0;
    m_Root.pParent = NULL;
    m_pCurNode = &m_Root;
	m_pDupHandler = NULL;
}

// 析构函数
CFairyExplorer::~CFairyExplorer(void)
{
}

// 打开FPK文件包
bool CFairyExplorer::Open( const AString& filename )
{
    // 关闭老的文件包
    Close();

    // 加载FPK
    FVFileSystem& vfs = FVFileSystem::GetInstance();
    m_pPackage = vfs.LoadPack( filename, "packer" );
    if( !m_pPackage ) return false;

    // 构建文件目录树
    Build();
    return true;
}

// 创建一个新文件包
bool CFairyExplorer::Create( const AString& filename )
{
    SFPKHeader header;
    header.nFourcc = F_MAKEFOURCC('F','P','A','K');
    header.nVersion = FPK_CURRENT_VERSION;
    header.nEntryOffset = sizeof(SFPKHeader);
    header.nNumEntries = 0;
    header.nFilenameOffset = sizeof(SFPKHeader);
    header.nFilenameSize = 0;

    FILE* pFile = fopen( filename.c_str(), "wb" );
    if( !pFile )
    {
        FLOG_ERRORF("CFairyExplorer::Create, Failed to create the package(%s)!", filename.c_str());
        return false;
    }
    
    fwrite( &header,sizeof(SFPKHeader),1,pFile );
    fclose( pFile );

    // 打开已创建的文件
    return Open( filename );
}

// 另存为到另一个文件
void CFairyExplorer::SaveAs( const AString& filename )
{
    if( m_pPackage )
    {
        m_pPackage->Flush();
        FSysAPI::FileCopy(m_pPackage->GetName().c_str(), filename.c_str(), true);
    }
}

// 关闭文件包
void CFairyExplorer::Close(void)
{
    m_Root.Children.clear();
    m_Root.pUserData = NULL;
    m_pCurNode = &m_Root;
    GetNodePath( m_pCurNode, m_sCurPath );
    m_sWorkingPath = m_sCurPath;
    if( m_pPackage )
    {
        FVFileSystem::GetInstance().UnloadPack( m_pPackage );
        m_pPackage = NULL;
    }
}

// 解压指定文件
bool CFairyExplorer::Extract( const AString& from, const AString& to )
{
    if( !m_pPackage )
        return false;

	if( from.empty() )
		return false;

    AString strOutput = to;
    AStringUtil::Trim( strOutput );
    if( strOutput.empty() )
    {
        strOutput = "./";
    }
    else if( strOutput[strOutput.length()-1] != '\\' &&
        strOutput[strOutput.length()-1] != '/' )
    {
        strOutput += '/';
    }

	PKNode* pNode = NULL;
	if( from[0] == '/' )	// 绝对路径
	{
		AString fromName = from;
		fromName.erase(fromName.begin());
		pNode = FindChildNodeR(FAIRY_FIND_ANY, &m_Root, fromName);
	}
	else
		pNode = FindChildNodeR(FAIRY_FIND_ANY, m_pCurNode, from);
    if( !pNode ) return false;

    AString path;
    GetNodePath( pNode,path );
    if( !path.empty() ) path.resize( path.size()-1 );
    return ExtractFileR( pNode,path,strOutput );
}

// 进入指定的目录
bool CFairyExplorer::EnterFolder( const AString& path )
{
    FASSERT( m_pCurNode );

	if( path.empty() )
		return false;

    AString strPath = path;
    PKNode* child = NULL;
	if( path[0] == '/' )	// 绝对路径
	{
		strPath.erase(strPath.begin());
		child = FindChildNodeR(FAIRY_FIND_DIR, &m_Root, strPath);
	}
	else
		child = FindChildNodeR(FAIRY_FIND_DIR, m_pCurNode, strPath);

	if( child )
    {
        m_pCurNode = const_cast<PKNode*>(child);
        GetNodePath( m_pCurNode, m_sCurPath );
		return true;
    }

	return false;
}

// 设置当前节点
void CFairyExplorer::SetCurrentNode( const PKNode* pNode )
{
    FASSERT( pNode );
    m_pCurNode = const_cast<PKNode*>(pNode);
    GetNodePath( m_pCurNode, m_sCurPath );
}

// 获取指定路径的节点
const CFairyExplorer::PKNode* CFairyExplorer::GetNodeByPath(const AString& path)
{
	return const_cast<PKNode*>(FindChildNodeR(FAIRY_FIND_ANY, &m_Root, path));
}

// 构建文件包的目录树
void CFairyExplorer::Build(void)
{
    VFileInfoList fileList;
    VFileInfoList::iterator it;
    m_pPackage->ListFiles( fileList,true,true );
    for( it=fileList.begin();it!=fileList.end();++it )
    {
        InsertFileToTree( it->sFileName,(uint32)it->nUncompressedSize,
            (uint32)it->nCompressedSize,false );
    }
}

// 解压指定文件
bool CFairyExplorer::ExtractFile( const AString& filename,const AString& output )
{
    FVFileStream* pFile = m_pPackage->Open( filename );
    if( !pFile ) return false;

    FILE* outFile = _wfopen( F_UTF8ToUnicode(output).c_str(), L"wb" );
    if( !outFile )
    {
        delete pFile;
        return false;
    }

    char* pBuf = new char[pFile->GetStreamSize()];
    pFile->Read( pBuf,pFile->GetStreamSize() );
    fwrite( pBuf,pFile->GetStreamSize(),1,outFile );
    fclose( outFile );
    delete pFile;
    delete[] pBuf;
    return true;
}

// 递归解压指定目录
bool CFairyExplorer::ExtractFileR( PKNode* pNode, const AString& path, const AString& output )
{
    assert( pNode );

    AString strPath = path;
    AString strOutput = output;
    strOutput += pNode->strName;

    if( !pNode->bIsDirectory )
    {
        if( m_pCallback && !m_pCallback(path) )
            return false;

		// 创建目录
		if( !FSysAPI::IsFileExist(output.c_str()) )
		{
			FSysAPI::CreateFolder(output.c_str());
		}

        // 直接解压文件
        if( !ExtractFile(strPath,strOutput) )
            return false;

        return true;
    }

    if( !strPath.empty() )
    {
        strPath += FPK_DIR_STR;
        strOutput += SYS_DIR_STR;

        if( !FSysAPI::IsFileExist(strOutput.c_str()) )
        {
            FSysAPI::CreateFolder(strOutput.c_str());
        }
        else
        {
			if( !FSysAPI::IsDirectory(strOutput.c_str()) )
            {
                FLOG_ERRORF("CFairyExplorer::ExtractFileR, Create the folder failed, file existed(%s)", strOutput.c_str());
                return false;
            }
        }
    }

    // 递归解压子目录
    std::list<PKNode>::iterator it;
    for( it=pNode->Children.begin();it!=pNode->Children.end();++it )
    {
        if( !ExtractFileR(&(*it),strPath+it->strName,strOutput) )
            return false;
    }

    return true;
}

// 添加文件或目录到树中
void CFairyExplorer::InsertFileToTree( const AString& filename,uint32 nSize,uint32 nCompressedSize,
                                       bool bCheckExisted )
{
    PKNode* node = &m_Root;
    AString strName = filename;
    while( strName.length() > 0 )
    {
        size_t pos = strName.find_first_of( "/\\" );
        if( pos == AString::npos )
        {
            PKNode* child = bCheckExisted ? FindChildNode( FAIRY_FIND_FILE, node, strName ) : NULL;

            if( child )
            {
                child->nFileSize = nSize;
                child->nCompressedSize = nCompressedSize;
            }
            else
            {
                // 添加文件信息
                PKNode newNode;
                newNode.bIsDirectory = (nCompressedSize == -1);
                newNode.nCompressedSize = nCompressedSize;
                newNode.nFileSize = nSize;
                newNode.pParent = node;
                newNode.strName = strName;

                if( !newNode.bIsDirectory )
                    node->Children.push_back( newNode );
                else
                {
                    std::list<PKNode>::iterator cp,inserted;
                    for( cp=node->Children.begin();cp!=node->Children.end();++cp )
                    {
                        if( !cp->bIsDirectory )
                            break;
                    }

                    node->Children.insert( cp,newNode );
                }
            }

            // 该路径已经结束
            break;
        }
        else
        {
            AString dirName = strName.substr( 0,pos );
            strName = strName.substr( pos+1,strName.length()-pos-1 );

            PKNode* child = FindChildNode( FAIRY_FIND_DIR,node,dirName );
            if( child )
            {
                node = child;
            }
            else
            {
                // 将目录添加到该节点
                PKNode newNode;
                newNode.bIsDirectory = true;
                newNode.nCompressedSize = 0;
                newNode.nFileSize = 0;
                newNode.pParent = node;
                newNode.strName = dirName;

                std::list<PKNode>::iterator cp,inserted;
                for( cp=node->Children.begin();cp!=node->Children.end();++cp )
                {
                    if( !cp->bIsDirectory )
                        break;
                }

                inserted = node->Children.insert( cp,newNode );
                node = &(*inserted);
            }
        }
    }
}

// 查找指定节点的子节点
CFairyExplorer::PKNode* CFairyExplorer::FindChildNode( int iFind,PKNode* node,const AString& name )
{
    FASSERT( node );

    if( iFind & FAIRY_FIND_DIR )
    {
        if( name == "" || name == "/" || name == "\\" )
            return &m_Root;
        else if( name == "." )
            return node;
        else if( name == ".." )
            return node->pParent;
    }

    AString strName1 = name;
    AStringUtil::ToLowerCase( strName1 );
    std::list<PKNode>::iterator it;
    for( it=node->Children.begin();it!=node->Children.end();++it )
    {
        AString strName2 = it->strName;
        AStringUtil::ToLowerCase( strName2 );
        if( strName1 == strName2 &&
            ((it->bIsDirectory && (iFind & FAIRY_FIND_DIR)) ||
            (!it->bIsDirectory && (iFind & FAIRY_FIND_FILE))) )
            return &(*it);
    }

    return NULL;
}

// 查找指定的节点(递归)
CFairyExplorer::PKNode* CFairyExplorer::FindChildNodeR( int iFind,PKNode* node,const AString& name )
{
    FASSERT( node );

    size_t pos = name.find_first_of( "/\\" );
    if( pos == AString::npos )
        return FindChildNode( iFind,node,name );

    PKNode* nextNode;
    AString dir = name.substr( 0,pos );
    nextNode = FindChildNode( FAIRY_FIND_DIR,node,dir );
    if( !nextNode ) return NULL;
    AString strName = name.substr( pos+1,name.length()-pos-1 );
    return FindChildNodeR( iFind, nextNode, strName );
}

// 获取指定节点的路径
void CFairyExplorer::GetNodePath( const PKNode* pNode, AString& path ) const
{
    path.clear();

    while( pNode != NULL && pNode != &m_Root )
    {
        path = pNode->strName + FPK_DIR_STR + path;
        pNode = pNode->pParent;
    }
}

// 获取指定节点的文件个数
uint32 CFairyExplorer::GetNodeFileCount( const PKNode* node )
{
    uint32 fileCount = 0;
    GetNodeCount( node,fileCount );
    return fileCount;
}

// 获取指定节点的文件个数
void CFairyExplorer::GetNodeCount( const PKNode* pNode, uint32& fileCount )
{
    if( !pNode->bIsDirectory )
        ++fileCount;

    std::list<PKNode>::const_iterator it;
    for( it=pNode->Children.begin();it!=pNode->Children.end();++it )
        GetNodeCount( &(*it),fileCount );
}

// 获取指定目录的文件个数
uint32 CFairyExplorer::GetDiskFileCount( const AString& dir )
{
    AString sBasePath = dir;

    if( sBasePath[sBasePath.length()-1] != SYS_DIR_CHAR )
        sBasePath += SYS_DIR_CHAR;

    uint32 nFileCount = 0;
    EnumFile( sBasePath, CountFile, &nFileCount );
    if( nFileCount == 0 )
        return 1;

    return nFileCount;
}

// 添加指定的文件到文件包
// Param [1] 要添加文件或目录的路径
// Param [2] 添加到选定的目录下，如果选定节点为空或是个文件则添加到当前目录
bool CFairyExplorer::Add( const AString& from, const AString& to )
{
    if( m_pPackage == NULL || m_pPackage->IsReadonly()
        || from.empty() )
        return false;
    
    if( to.empty() )
    {
        m_sWorkingPath = m_sCurPath;
    }
    else
    {
        m_sWorkingPath = m_sCurPath + to;
    }

    // 处理工作路径，将.. .等特殊路径符号处理掉
    HandleWorkPath(m_sWorkingPath);
    
    if( !m_sWorkingPath.empty() &&
        m_sWorkingPath[m_sWorkingPath.length()-1] != FPK_DIR_CHAR )
    {
        m_sWorkingPath += FPK_DIR_CHAR;
    }
    
    if( !FSysAPI::IsFileExist(from.c_str()) )
        return false;

    // 处理源文件路径
    AString srcPath = from;
    
#if FAIRY_PLATFORM != FAIRY_PLATFORM_WINDOWS
    if( !AStringUtil::IsAbsPath(from) )
        srcPath = FSysAPI::GetCurrentDir() + "/" + from;
    HandleWorkPath(srcPath);
#endif

    m_sBasePath.clear();
    size_t pos = srcPath.rfind( SYS_DIR_CHAR );
    
	if( FSysAPI::IsNormalFile(srcPath.c_str()) )
    {
        AString toFilename = srcPath.substr(pos+1, srcPath.length()-pos-1);
        toFilename = m_sWorkingPath + toFilename;
        bool ret = AddFile(srcPath, toFilename);
        m_pPackage->Flush();
        return ret;
    }

    if( pos != AString::npos )
    {
        m_sBasePath = srcPath.substr( 0,pos+1 );
    }

    AString searchDirectory = srcPath;
    if( searchDirectory[searchDirectory.length()-1] != SYS_DIR_CHAR )
    {
        searchDirectory += SYS_DIR_CHAR;
    }
    
    bool ret = EnumFile( searchDirectory, AddPackFile, this );
    m_pPackage->Flush();
    return ret;
}

// 添加一个文件到文件包
// Param [0] 要添加的文件路径
// Param [1] 包内路径
bool CFairyExplorer::AddFile( const AString& sFile, const AString& to )
{
    if( m_pCallback && !m_pCallback(to) )
        return false;
    
    // 读取文件数据
    uint32 fileSize = 0;
    FILE* pFile;
    pFile = fopen( sFile.c_str(),"rb" );
    if( !pFile ) return false;
    fseek( pFile,0,SEEK_END );
    fileSize = static_cast<uint32>( ftell(pFile) );
    char* buffer = new char[fileSize];
    fseek( pFile,0,SEEK_SET );
    fread( buffer,fileSize,1,pFile );
    fclose( pFile );

    uint32 flags = 0;
    flags |= g_Config.compress;
    flags |= FPK_FILE_REPLACE;

    AString fileExt;
    bool bNoCompress = false;
    AStringUtil::FileExt( to,fileExt );
    for( size_t i=0;i<g_Config.extList.size();++i )
    {
        if( g_Config.extList[i] == fileExt )
        {
            bNoCompress = true;
            break;
        }
    }

    if( bNoCompress )
        flags |= FPK_QUALITY_NONE;
    else
    {
        uint32 bufferSize = 0;
        switch( g_Config.bufferLevel )
        {
        case FPK_QUALITY_LOW:
            bufferSize = BLOCK_LOW;
            break;
        case FPK_QUALITY_NORMAL:
            bufferSize = BLOCK_NORMAL;
            break;
        case FPK_QUALITY_HIGH:
            bufferSize = BLOCK_HIGH;
            break;
        }

        if( fileSize <= bufferSize )
            flags |= FPK_QUALITY_ENTIRE;
        else
            flags |= g_Config.bufferLevel;
    }

    uint32 compressSize;
    bool haveFile = m_pPackage->Exists( to );
    if( haveFile )
    {
        if( m_pDupHandler->Handle(to) )
        {
            compressSize = m_pPackage->AddEntry( to,fileSize,buffer,flags );
        }
    }
    else
    {
        compressSize = m_pPackage->AddEntry( to,fileSize,buffer,flags );
        if( compressSize != -1 ) InsertFileToTree( to,fileSize,compressSize );
    }

    delete[] buffer;
    return true;
}

// 枚举当前节点的信息
void CFairyExplorer::EnumNodeInfo( const PKNode* pNode,SPackageInfo& info )
{
    if( pNode->bIsDirectory )
    {
        ++info.folderCount;
    }
    else
    {
        ++info.fileCount;
        info.origSize += pNode->nFileSize;
        info.compressSize += pNode->nCompressedSize;
    }

    std::list<PKNode>::const_iterator it;
    for( it=pNode->Children.begin();it!=pNode->Children.end();++it )
        EnumNodeInfo( &(*it),info );
}

// 获取当前文件包的信息
void CFairyExplorer::GetPackageInfo( SPackageInfo& info )
{
    info.fileCount = 0;
    info.folderCount = 0;
    info.origSize = 0;
    info.compressSize = 0;
    EnumNodeInfo( &m_Root,info );
    info.folderCount--;     // 多计算了一个目录，Package Root
    info.compressDegree = info.origSize ? (float)info.compressSize / (float)info.origSize : 0.0f;
}

// 移除指定的节点及其子节点
void CFairyExplorer::RemoveNodeR( PKNode* pNode, AString& path )
{
    if( !pNode->bIsDirectory )
    {
        m_pPackage->Delete( path );
    }
    else
    {
        if( !path.empty() )
            path += FPK_DIR_CHAR;

        std::list<PKNode>::iterator child;
        for( child=pNode->Children.begin();child!=pNode->Children.end(); )
        {
            PKNode* pChild = &(*child);
            AString childPath = path + pChild->strName;
            RemoveNodeR( pChild, childPath );
            child = pNode->Children.erase( child );
        }
    }
}

// 移除指定的文件或目录，从当前目录的子目录中搜索
bool CFairyExplorer::Remove( const AString& path )
{
    if( path.empty() )
		return false;

	// 获取指定的节点
	PKNode* pNode = NULL;
	if( path[0] == '/' )
	{
		AString strPath = path;
		strPath.erase(strPath.begin());
		pNode = FindChildNodeR(FAIRY_FIND_ANY, &m_Root, strPath);
	}
	else
		pNode = FindChildNodeR( FAIRY_FIND_ANY, m_pCurNode, path );

    if( !pNode ) return false;
    AString internalPath;
    GetNodePath( pNode,internalPath );
    if( !internalPath.empty() )
        internalPath.resize( internalPath.size()-1 );

    // 移除该节点的子节点
    RemoveNodeR( pNode, internalPath );

    // 移除本节点
    std::list<PKNode>::iterator it;
    PKNode* parent = pNode->pParent;
    for( it=parent->Children.begin();it!=parent->Children.end();++it )
    {
        if( &(*it) == pNode )
        {
            parent->Children.erase( it );
            break;
        }
    }

    m_pPackage->Flush();
	return true;
}

// 在指定的目录中搜索文件
bool CFairyExplorer::SearchFiles(const AString& folder, const AString& strFile, AStringList& fileList)
{
	PKNode* pNode = FindChildNodeR(FAIRY_FIND_DIR, &m_Root, folder);
	return pNode ? SearchFileR(pNode, strFile, fileList) : false;
}

// 在当前目录中搜索文件
bool CFairyExplorer::SearchFiles(const AString& strFile, std::vector<const PKNode*>& fileList)
{
	AString strPath = m_sCurPath;
	if( strPath.length() > 0 ) strPath.erase(strPath.length() - 1);
	return SearchFiles(strPath, strFile, fileList);
}

// 在指定的目录中搜索文件
bool CFairyExplorer::SearchFiles(const AString& folder, const AString& strFile, std::vector<const PKNode*>& fileList)
{
	PKNode* pNode = FindChildNodeR(FAIRY_FIND_DIR, &m_Root, folder);
	return pNode ? SearchFileR(pNode, strFile, fileList) : false;
}

// 在当前目录中搜索文件
bool CFairyExplorer::SearchFiles(const AString& strFile, AStringList& fileList)
{
	AString strPath = m_sCurPath;
	if( strPath.length() > 0 ) strPath.erase(strPath.length() - 1);
	return SearchFiles(strPath, strFile, fileList);
}

// 递归搜索文件
bool CFairyExplorer::SearchFileR(PKNode* node, const AString& strFile, AStringList& fileList)
{
	if( !node->bIsDirectory )
	{
		AString strKey;
		if (strFile.find('*') == AString::npos)
			strKey = "*" + strFile + "*";
		else
			strKey = strFile;
		if( AStringUtil::Match(node->strName, strKey) )
		{
			AString filePath;
			GetNodePath(node, filePath);
			if( filePath[filePath.length()-1] == '/' )
				filePath.resize(filePath.length()-1);
			fileList.push_back(filePath);
		}
	}
	else
	{
		std::list<PKNode>::iterator it;
		for(it=node->Children.begin(); it!=node->Children.end(); ++it)
		{
			if( !SearchFileR(&(*it), strFile, fileList) )
				break;
		}
	}

	return true;
}

// 递归搜索文件
bool CFairyExplorer::SearchFileR(PKNode* node, const AString& strFile, std::vector<const PKNode*>& fileList)
{
	if( !node->bIsDirectory )
	{
		if( AStringUtil::Match(node->strName, strFile) )
			fileList.push_back(const_cast<PKNode*>(node));
	}
	else
	{
		std::list<PKNode>::iterator it;
		for(it=node->Children.begin(); it!=node->Children.end(); ++it)
		{
			if( !SearchFileR(&(*it), strFile, fileList) )
				break;
		}
	}

	return true;
}

// 整理文件包碎片
bool CFairyExplorer::Defrag(void)
{
    FASSERT( m_pPackage );
    return m_pPackage->DeFragment( (CB_PACKAGEOP)m_pCallback );
}

// 获取当前压缩包的名字
const AString& CFairyExplorer::GetPackageName(void) const
{
	return m_pPackage ? m_pPackage->GetName() : AStringUtil::BLANK;
}