/*
 * ------------------------------------------------------------------------
 *  Name:   FZipFileStream.cpp
 *  Desc:   本文件用于实现基于ZIP压缩包的文件数据流。
 *  Author: Yish
 *  Date:   2010/10/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FZipFileStream.h"
#include "FLogManager.h"

#include <zzip/zzip.h>

/** 构造函数
@Param ZIP文件指针
@Param 压缩前的大小
*/
FZipFileStream::FZipFileStream( ZZIP_FILE *pZipFile,size_t uncompressSize )
    : m_pZipFile(pZipFile)
{
    m_nStreamSize = uncompressSize;
}

/** 构造函数
@Param ZIP文件的名字
@Param ZIP文件指针
@Param 压缩前的大小
*/
FZipFileStream::FZipFileStream( const AString& name,ZZIP_FILE* pZipFile,size_t uncompressSize )
    : FVFileStream(name),m_pZipFile(pZipFile)
{
    m_nStreamSize = uncompressSize;
}

/** 析构函数
*/
FZipFileStream::~FZipFileStream(void)
{
    Close();
}

/** 移动文件指针
@Param 移动的字节数
@Param 指针位置
*/
void FZipFileStream::Seek( long nPos,int flag )
{
    switch( flag )
    {
    case START:
        zzip_seek( m_pZipFile,static_cast<zzip_off_t>(nPos),SEEK_SET );
        break;
    case CURRENT:
        zzip_seek( m_pZipFile,static_cast<zzip_off_t>(nPos),SEEK_CUR );
        break;
    case END:
        zzip_seek( m_pZipFile,static_cast<zzip_off_t>(nPos),SEEK_END );
        break;
    default: FASSERT(0); break;
    }
}

/** 获取当前ZIP文件指针的位置
*/
size_t FZipFileStream::Tell(void) const
{
    return zzip_tell( m_pZipFile );
}

/** 检查ZIP文件指针是否到达末尾
*/
bool FZipFileStream::Eof(void) const
{
    return zzip_tell(m_pZipFile) >= static_cast<zzip_off_t>(m_nStreamSize);
}

/** 跳过指定的字节数
@Param 要跳过的字节数目
*/
void FZipFileStream::Skip( long nCount )
{
    zzip_seek( m_pZipFile,static_cast<zzip_off_t>(nCount),SEEK_CUR );
}

/** 关闭ZIP文件流
*/
void FZipFileStream::Close(void)
{
    if( m_pZipFile )
    {
        zzip_close( m_pZipFile );
        m_pZipFile = NULL;
    }
}

/** 读取指定数目的数据
@Param 存储数据的缓冲区指针
@Param 要读取数据的大小
*/
size_t FZipFileStream::Read( void* pBuf,size_t nSize )
{
    zzip_ssize_t ret = zzip_file_read( m_pZipFile,pBuf,nSize );

    if( ret < 0 )
    {
        ZZIP_DIR* dir = zzip_dirhandle( m_pZipFile );

        // 报告错误
        AString msg = zzip_strerror_of( dir );
        FLOG_ERRORF("FZipFileStream::Read, An exception is found when reading zip package! (%s)", msg.c_str());
    }

    return ret;
}