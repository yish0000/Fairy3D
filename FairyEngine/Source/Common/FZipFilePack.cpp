/*
 * ------------------------------------------------------------------------
 *  Name:   FZipFilePack.cpp
 *  Desc:   本文件用于实现基于ZIP压缩包的文件包格式。
 *  Author: Yish
 *  Date:   2010/10/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FZipFilePack.h"
#include "FZipFileStream.h"
#include "FLogManager.h"
#include "FStrUtil.h"

#include "zzip/zzip.h"

/** 构造函数
@Param 文件包的名字
@Param 文件包的类型
*/
FZipFilePack::FZipFilePack( const AString& name,const AString& packType )
    : FVFilePack(name,packType),m_pZipDir(NULL)
{
    m_bReadonly = true;
}

/** 析构函数
*/
FZipFilePack::~FZipFilePack(void)
{
    Unload();
}

/** 加载ZIP文件包
*/
bool FZipFilePack::Load(void)
{
    if( !m_pZipDir )
    {
        zzip_error_t zipError;
        m_pZipDir = zzip_dir_open( m_sName.c_str(),&zipError );
        if( !m_pZipDir ) return false;

        if( zipError != ZZIP_NO_ERROR )
        {
            AString errInfo;
            GetZipErrorDesc( zipError,errInfo );

            // 抛出异常
            FLOG_WARNING( "FZipFilePack::Load(), An exception is found when loading zip archive(" + errInfo + ")!" );
            return false;
        }

        ZZIP_DIRENT dirEntry;
        while( zzip_dir_read(m_pZipDir,&dirEntry) )
        {
            VFileInfo info;
            info.pFilePack = this;
            AStringUtil::SplitPath( dirEntry.d_name,info.sBaseName,info.sPath );
            info.nCompressedSize = static_cast<size_t>(dirEntry.d_csize);
            info.nUncompressedSize = static_cast<size_t>(dirEntry.st_size);
            info.sFileName = dirEntry.d_name;

            // 如果是目录
            if( info.sBaseName == "" )
            {
                info.sFileName = info.sFileName.substr( 0,info.sFileName.length()-1 );
                AStringUtil::SplitPath( info.sFileName,info.sBaseName,info.sPath );
                info.nCompressedSize = (size_t)-1;
            }

            m_FileInfos.push_back( info );
        }
    }

    return true;
}

/** 卸载ZIP文件包
*/
void FZipFilePack::Unload(void)
{
    if( m_pZipDir )
    {
        zzip_dir_close( m_pZipDir );
        m_pZipDir = NULL;
        m_FileInfos.clear();
    }
}

/** 打开一个ZIP文件
@Param 文件的名字
*/
FVFileStream* FZipFilePack::Open( const AString& filename )
{
    ZZIP_FILE* pFile = zzip_file_open( m_pZipDir,filename.c_str(),
        ZZIP_ONLYZIP|ZZIP_CASELESS );

    if( !pFile )
    {
        AString errDesc;
        int zipError = zzip_error( m_pZipDir );
        if( zipError != ZZIP_NO_ERROR )
        {
            GetZipErrorDesc( zipError,errDesc );
            FLOG_WARNINGF( "FZipFilePack::Open(), Open file (%s) from ZIP package(%s) failed!",filename.c_str(),m_sName.c_str() );
        }

        return NULL;
    }

    ZZIP_STAT zipStat;
    zzip_dir_stat( m_pZipDir,filename.c_str(),&zipStat,ZZIP_CASEINSENSITIVE );

    // 创建一个ZIP文件流
    return new FZipFileStream( filename,pFile,zipStat.st_size );
}

/** 列出该文件包内的所有文件
@Param 文件名字列表
@Param 遍历时是否递归
@Param 显示目录而非目录下文件
*/
void FZipFilePack::ListFiles( AStringList& fileList,bool bRecurse,bool bDir )
{
    VFileInfoList::iterator itr;
    VFileInfoList::iterator end = m_FileInfos.end();
    for( itr=m_FileInfos.begin();itr!=end;itr++ )
    {
        if( ((itr->nCompressedSize != (size_t)-1) ||
            (itr->nCompressedSize == (size_t)-1 && bDir)) &&
            (bRecurse || itr->sPath.empty()) )
        {
            fileList.push_back( itr->sFileName );
        }
    }
}

/** 列出该文件包内的所有文件包信息
@Param 文件信息列表
@Param 遍历时是否递归
@Param 是否遍历目录
*/
void FZipFilePack::ListFiles( VFileInfoList& fileList,bool bRecurse,bool bDir )
{
    VFileInfoList::iterator itr;
    VFileInfoList::iterator end = m_FileInfos.end();
    for( itr=m_FileInfos.begin();itr!=end;itr++ )
    {
        if( ((itr->nCompressedSize != (size_t)-1) ||
            (itr->nCompressedSize == (size_t)-1 && bDir)) &&
            (bRecurse || itr->sPath.empty()) )
        {
            fileList.push_back( *itr );
        }
    }
}

/** 查找指定的文件
@Param 存储文件名字的列表
@Param 用于匹配文件名的字符
@Param 遍历时是否递归
@Param 是否遍历目录
*/
void FZipFilePack::FindFiles( AStringList& fileList,const AString& pattern,
                                bool bRecurse,bool bDir )
{
    VFileInfoList::iterator itr;
    VFileInfoList::iterator end = m_FileInfos.end();

    // 如果要查找的字符包含目录，则做一个全匹配
    bool full_match = (pattern.find('/') != AString::npos ||
        pattern.find('\\') != AString::npos);

    // 获取关键字的目录
    AString directory;
    if( full_match )
    {
        size_t pos1 = pattern.rfind( '/' );
        size_t pos2 = pattern.rfind( '\\' );

        if( pos1 == AString::npos || ((pos2 != AString::npos) && pos1 < pos2) )
            pos1 = pos2;

        if( pos1 != AString::npos )
            directory = pattern.substr( 0,pos1+1 );
    }

    // 遍历文件信息
    for( itr=m_FileInfos.begin();itr!=end;itr++ )
    {
        if( ((itr->nCompressedSize != (size_t)-1) ||
            (itr->nCompressedSize == (size_t)-1 && bDir)) &&
            (bRecurse || itr->sPath == directory) )
        {
            if( AStringUtil::Match(full_match ? itr->sFileName : itr->sBaseName,pattern) )
                fileList.push_back( itr->sFileName );
        }
    }
}

/** 查找指定的文件
@Param 存储文件信息的列表
@Param 用于匹配文件名的字符
@Param 遍历时是否递归
@Param 是否遍历目录
*/
void FZipFilePack::FindFiles( VFileInfoList& fileList,const AString& pattern,
                                bool bRecurse,bool bDir )
{
    VFileInfoList::iterator itr;
    VFileInfoList::iterator end = m_FileInfos.end();

    // 如果要查找的字符包含目录，则做一个全匹配
    bool full_match = (pattern.find('/') != AString::npos ||
        pattern.find('\\') != AString::npos );

    // 获取关键字的目录
    AString directory;
    if( full_match )
    {
        size_t pos1 = pattern.rfind( '/' );
        size_t pos2 = pattern.rfind( '\\' );

        if( pos1 == AString::npos || ((pos2 != AString::npos) && pos1 < pos2) )
            pos1 = pos2;

        if( pos1 != AString::npos )
            directory = pattern.substr( 0,pos1+1 );
    }

    // 遍历文件信息
    for( itr=m_FileInfos.begin();itr!=end;itr++ )
    {
        if( ((itr->nCompressedSize != (size_t)-1) ||
            (itr->nCompressedSize == (size_t)-1 && bDir)) &&
            (bRecurse || itr->sPath == directory) )
        {
            if( AStringUtil::Match(full_match ? itr->sFileName : itr->sBaseName,pattern) )
                fileList.push_back( *itr );
        }
    }
}

/** 检测是否存在指定文件
@Param 指定的文件名
*/
bool FZipFilePack::Exists( const AString& filename )
{
    ZZIP_STAT zipStat;
    int ret = zzip_dir_stat( m_pZipDir,filename.c_str(),&zipStat,ZZIP_CASEINSENSITIVE );
    return (ret == ZZIP_NO_ERROR);
}

/** 获取指定文件的信息
*/
void FZipFilePack::GetFileInfo( const AString& filename,VFileInfo& info )
{
    VFileInfoList::const_iterator it;
    for( it=m_FileInfos.begin();it!=m_FileInfos.end();++it )
    {
        if( it->sFileName == filename )
            info = *it;
    }
}

/** 获取指定ZIP错误编号对应的错误信息
@Param ZIP错误编号
@Param 存储错误信息
*/
void FZipFilePack::GetZipErrorDesc( int zipError,AString& desc )
{
    switch( zipError )
    {
    case ZZIP_NO_ERROR:
        break;
    case ZZIP_OUTOFMEM:
        desc = "Out of memory";
        break;
    case ZZIP_DIR_OPEN:
    case ZZIP_DIR_STAT:
    case ZZIP_DIR_SEEK:
    case ZZIP_DIR_READ:
        desc = "Unable to read the zip file";
        break;
    case ZZIP_UNSUPP_COMPR:
        desc = "Unsupport compression format";
        break;
    case ZZIP_CORRUPTED:
        desc = "Corrupted archive";
        break;
    default:
        desc = "Unknown error";
        break;
    }
}

/** 创建一个ZIP文件包
@Param ZIP文件包的名字
*/
FVFilePack* FZipFilePackFactory::CreateInstance( const AString& name )
{
    return new FZipFilePack( name,"zip" );
}

/** 销毁一个ZIP文件包
@Param ZIP文件包的指针
*/
void FZipFilePackFactory::DestroyInstance( FVFilePack* pPack )
{
    F_SAFE_DELETE( pPack );
}