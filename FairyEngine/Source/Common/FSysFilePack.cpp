/*
 * ------------------------------------------------------------------------
 *  Name:   FSysFilePack.cpp
 *  Desc:   本文件用于实现基于普通目录的文件包类型。
 *  Author: Yish
 *  Date:   2010/10/2
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FSysFilePack.h"
#include "FSysFileStream.h"
#include "FLogManager.h"
#include "FStrUtil.h"
#include "FSysAPI.h"

#include <sys/stat.h>

// 一些处理文件路径的辅助函数
static bool is_reserved_dir( const char* fn )
{
    return fn[strlen(fn) - 1] == '.';
}

static AString concatenate_path( const AString& base, const AString& name )
{
	if( base.empty() || AStringUtil::IsAbsPath(name) )
        return name;
    else
        return base + '/' + name;
}

/** 遍历符合要求的文件，读取信息。
@Param 搜索文件用的标示
@Param 是否递归搜索
@Param 是否包含目录
@Param 存储文件的详细信息列表
@Param 存储文件的路径列表
*/
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS

#include <io.h>

void FSysFilePack::EnumFiles(const AString& pattern, bool bRecurse, bool bDir, VFileInfoList* fileinfos,
							 AStringList* fileList)
{
    long lHandle, res;
    struct _finddata_t tagFind;

    size_t pos1 = pattern.rfind( '/' );
    size_t pos2 = pattern.rfind( '\\' );
    if( pos1 == pattern.npos || ((pos2 != pattern.npos) && pos1 < pos2) )
        pos1 = pos2;

    AString directory;
    if( pos1 != pattern.npos )
        directory = pattern.substr( 0,pos1+1 );

    AString full_pattern = concatenate_path( m_sName, pattern );

    lHandle = (long)_findfirst( full_pattern.c_str(),&tagFind );
    res = 0;
    while( lHandle != -1 && res != -1 )
    {
        if( ((tagFind.attrib & _A_SUBDIR) == 0) ||
            (bDir && ((tagFind.attrib & _A_SUBDIR) != 0) &&
            !is_reserved_dir(tagFind.name)) )
        {
            if( fileList )
            {
                fileList->push_back( directory + tagFind.name );
            }

			if( fileinfos )
            {
                VFileInfo fi;
                fi.pFilePack = this;
                fi.sFileName = directory + tagFind.name;
                fi.sBaseName = tagFind.name;
                fi.sPath = directory;
                fi.nCompressedSize = tagFind.size;
                fi.nUncompressedSize = tagFind.size;
                fileinfos->push_back( fi );
            }
        }

        res = _findnext( lHandle,&tagFind );
    }

    if( lHandle != -1 )
        _findclose( lHandle );

    // 搜索目录
    if( bRecurse )
    {
        AString base_dir = m_sName;

        if( !directory.empty() )
        {
            base_dir = concatenate_path( m_sName,directory );

            // 删除最后一个'/'
            base_dir.erase( base_dir.length() - 1 );
        }

        base_dir.append( "/*" );

        AString mask("/");
        if( pos1 != pattern.npos )
            mask.append( pattern.substr(pos1 + 1) );
        else
            mask.append( pattern );

        lHandle = (long)_findfirst( base_dir.c_str(),&tagFind );
        res = 0;
        while( lHandle != -1 && res != -1 )
        {
            if( (tagFind.attrib & _A_SUBDIR) &&
                !is_reserved_dir(tagFind.name) )
            {
                base_dir = directory;
                base_dir.append(tagFind.name).append(mask);

                EnumFiles( base_dir,bRecurse,bDir,fileinfos,fileList );
            }

            res = _findnext( lHandle,&tagFind );
        }

        if( lHandle != -1 )
            _findclose( lHandle );
    }
}

#else

#include <glob.h>

void FSysFilePack::EnumFiles(const AString& pattern, bool bRecurse, bool bDir, VFileInfoList* fileinfos,
							 AStringList* fileList)
{
    glob_t globbuf;
    
    size_t pos1 = pattern.rfind( '/' );
    size_t pos2 = pattern.rfind( '\\' );
    if( pos1 == pattern.npos || ((pos2 != pattern.npos) && pos1 < pos2) )
        pos1 = pos2;

    AString directory;
    if( pos1 != pattern.npos )
        directory = pattern.substr( 0, pos1+1 );

    AString full_pattern = concatenate_path( m_sName, pattern );
    int ret = glob(full_pattern.c_str(), GLOB_NOSORT, NULL, &globbuf);
    if( ret != 0 ) return;

    struct stat st;
    AString full_path;

    for( size_t i=0; i<globbuf.gl_pathc; ++i )
    {
        full_path = directory + globbuf.gl_pathv[i];
        lstat(full_path.c_str(), &st);
        
        if( (S_ISREG(st.st_mode) == 0) ||
            (bDir && S_ISDIR(st.st_mode) && !is_reserved_dir(globbuf.gl_pathv[i])) )
        {
            if( fileList )
            {
                fileList->push_back(directory + globbuf.gl_pathv[i]);
            }

			if( fileinfos )
            {
                VFileInfo fi;
                fi.pFilePack = this;
                fi.sFileName = directory + globbuf.gl_pathv[i];
                fi.sBaseName = globbuf.gl_pathv[i];
                fi.sPath = directory;
                fi.nCompressedSize = st.st_size;
                fi.nUncompressedSize = st.st_size;
                fileinfos->push_back(fi);
            }
        }
    }
    
    globfree(&globbuf);

    // 搜索目录
    if( bRecurse )
    {
        AString base_dir = m_sName;

        if( !directory.empty() )
        {
            base_dir = concatenate_path( m_sName, directory );

            // 删除最后一个'/'
            base_dir.erase( base_dir.length() - 1 );
        }

        base_dir.append( "/*" );

        AString mask("/");
        if( pos1 != pattern.npos )
            mask.append( pattern.substr(pos1 + 1) );
        else
            mask.append( pattern );
        
        int ret = glob(base_dir.c_str(), GLOB_NOSORT, NULL, &globbuf);
        if( ret != 0 )
        {
            FLOG_WARNINGF("FSysFilePack::EnumFiles, Open the directory %s failed!", full_pattern.c_str());
            return;
        }
        
        for( size_t i=0; i<globbuf.gl_pathc; ++i )
        {
            full_path = directory + globbuf.gl_pathv[i];
            lstat(full_path.c_str(), &st);

            if( S_ISDIR(st.st_mode) &&
               !is_reserved_dir(globbuf.gl_pathv[i]) )
            {
                base_dir = directory;
                base_dir.append(globbuf.gl_pathv[i]).append(mask);
                
                EnumFiles( base_dir, bRecurse, bDir, fileinfos, fileList );
            }
        }

        globfree(&globbuf);
    }
}

#endif

///////////////////////////////////////////////////////////////////////////

/** 构造函数
@Param 文件包的名字
@Param 文件包的类型
*/
FSysFilePack::FSysFilePack( const AString& name,const AString& packType )
    : FVFilePack(name,packType)
{
}

/** 析构函数
*/
FSysFilePack::~FSysFilePack(void)
{
    Unload();
}

/** 添加一个文件到文件包
*/
int FSysFilePack::AddEntry( const AString& entry, uint32 nSize, void* data, uint32 flags )
{
    if( !nSize || !data )
        return -1;

    // 计算文件的全名
    AString fullpath = concatenate_path( m_sName, entry );

    FILE* pFile = fopen( fullpath.c_str(), "wb" );
    if( !pFile )
    {
        FLOG_WARNING( "FSysFilePack::AddEntry, fail to create the file!!" );
        return -1;
    }

    fwrite( data, nSize, 1, pFile );
    fclose( pFile );
    return nSize;
}

/** 添加一个文件到文件包
*/
int FSysFilePack::AddEntry( const AString& entry, const AString& filename, uint32 flags )
{
	if( !FSysAPI::IsFileExist(filename.c_str()) )
		return -1;

	// Get the full name.
	AString fullpath = concatenate_path(m_sName, entry);
	if( FSysAPI::FileCopy(filename.c_str(), fullpath.c_str(), true) )
	{
		uint32 fileSize = FSysAPI::GetFileSize(filename.c_str());
		return fileSize;
	}

	return -1;
}

/** 删除文件包中的一个文件
*/
void FSysFilePack::Delete( const AString& path )
{
	FSysAPI::RemoveFile(path.c_str());
}

/** 打开一个文件，返回数据流指针。
@Param 文件的相对路径
@Return 该文件数据流的指针
*/
FVFileStream* FSysFilePack::Open( const AString& filename )
{
    // 计算文件的全名
    AString fullpath = concatenate_path( m_sName, filename );

    struct stat st;
    int ret = stat( fullpath.c_str(), &st );
    FASSERT( ret == 0 && "Get the file attributes error!!" );

    // 以二进制方式打开文件
    FILE* pFile = fopen( fullpath.c_str(), "rb" );
    if( !pFile )
    {
        FLOG_WARNING( "FSysFilePack::Open, open the file failed!!" );
        return NULL;
    }

    // 创建一个文件流
    return new FSysFileStream( filename, pFile, st.st_size );
}

/** 列出文件包里的所有文件
@Param 存储文件名字列表
@Param 是否递归
@Param 是否包含文件夹
*/
void FSysFilePack::ListFiles( AStringList& fileList,bool bRecurse/* =true */,
                              bool bDir/* =true */ )
{
    EnumFiles( "*",bRecurse,bDir,NULL,&fileList );
}

/** 列出文件包里的所有文件
@Param 存储文件的详细属性
@Param 是否递归
@Param 是否包含文件夹
*/
void FSysFilePack::ListFiles( VFileInfoList& fileList,bool bRecurse/* =true */,
                              bool bDir/* =true */ )
{
    EnumFiles( "*",bRecurse,bDir,&fileList,NULL );
}

/** 查找指定的文件
@Param 存储文件的名字列表
@Param 查找文件的关键字
@Param 是否递归
@Param 是否包含文件夹
*/
void FSysFilePack::FindFiles( AStringList& fileList,const AString& pattern,
                              bool bRecurse/* =true */,bool bDir/* =true */ )
{
    EnumFiles( pattern,bRecurse,bDir,NULL,&fileList );
}

/** 查找指定的文件
@Param 存储文件的详细信息列表
@Param 查找文件的关键字
@Param 是否递归
@Param 是否包含文件夹
*/
void FSysFilePack::FindFiles( VFileInfoList& fileList,const AString& pattern,
                              bool bRecurse/* =true */,bool bDir/* =true */ )
{
    EnumFiles( pattern,bRecurse,bDir,&fileList,NULL );
}

/** 检测指定文件是否存在
@Param 指定文件名字
*/
bool FSysFilePack::Exists( const AString& filename )
{
    AString full_path = concatenate_path( m_sName,filename );

    struct stat tagStat;
    bool ret = (stat(full_path.c_str(),&tagStat) == 0);

	if( ret && AStringUtil::IsAbsPath(filename) )
    {
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
        ret = AStringUtil::StartWith( full_path, filename, true );
#else
        ret = AStringUtil::StartWith( full_path, filename, false );
#endif
    }

    return ret;
}

// 获取指定文件的信息
void FSysFilePack::GetFileInfo( const AString& filename,VFileInfo& info )
{
    AString full_path = concatenate_path( m_sName,filename );
    struct stat tagStat;
    bool ret = (stat(full_path.c_str(),&tagStat) == 0);
    if( ret )
    {
        info.pFilePack = this;
        info.sPath = filename;
        AStringUtil::SplitPath( filename,info.sBaseName,info.sPath );
        info.nCompressedSize = tagStat.st_size;
        info.nUncompressedSize = tagStat.st_size;
    }
}


///////////////////////////////////////////////////////////////////////////
// 实现目录文件包的类厂

/** 创建一个目录文件包
@Param 该目录文件包的名字
*/
FVFilePack* FSysFilePackFactory::CreateInstance( const AString& name )
{
    return new FSysFilePack( name,m_sType );
}

/** 销毁一个指定的目录文件包
@Param 该目录文件包的指针
*/
void FSysFilePackFactory::DestroyInstance( FVFilePack* pPack )
{
    F_SAFE_DELETE( pPack );
}