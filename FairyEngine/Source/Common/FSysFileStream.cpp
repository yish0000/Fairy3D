/*
 * ------------------------------------------------------------------------
 *  Name:   FSysFileStream.cpp
 *  Desc:   本文件用于实现文件的标准数据流。
 *  Author: Yish
 *  Date:   2010/10/2
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FSysFileStream.h"

/** 构造函数
@Param 该数据流所属文件的名字
*/
FSysFileStream::FSysFileStream( const AString& name )
	: FVFileStream(name)
{
    m_pStream = fopen( name.c_str(), "rb" );
    FASSERT( m_pStream && "Open the file failed!" );
    fseek( m_pStream, 0, SEEK_END );
    m_nStreamSize = ftell( m_pStream );
    fseek( m_pStream, 0, SEEK_SET );
}

/** 构造函数
@Param 该数据流所属文件的名字
@Param 标准文件流指针
*/
FSysFileStream::FSysFileStream( const AString& name, FILE* pStream )
    : FVFileStream(name), m_pStream(pStream)
{
    fseek( m_pStream, 0, SEEK_END );
    m_nStreamSize = ftell( m_pStream );
    fseek( m_pStream, 0, SEEK_SET );
}

/** 构造函数
@Param 该数据流所属文件的名字
@Param 标准文件流指针
@Param 该文件的大小
*/
FSysFileStream::FSysFileStream( const AString& name, FILE* pStream, size_t nSize )
    : FVFileStream(name),m_pStream(pStream)
{
    m_nStreamSize = nSize;
}

/** 析构函数
*/
FSysFileStream::~FSysFileStream(void)
{
    Close();
}

/** 移动数据流指针
@Param 移动到数据流的哪一个位置
*/
void FSysFileStream::Seek( long nPos, int flag/* =START */ )
{
    FASSERT( m_pStream );
    fseek( m_pStream, nPos, flag );
}

/** 返回当前指针在数据流中的位置
*/
size_t FSysFileStream::Tell(void) const
{
    FASSERT( m_pStream );
    return ftell( m_pStream );
}

/** 当前数据流是否到头
*/
bool FSysFileStream::Eof(void) const
{
    FASSERT( m_pStream );
    return (feof(m_pStream) != 0);
}

/** 跳过指定的字节
*/
void FSysFileStream::Skip( long nCount )
{
    FASSERT( m_pStream );
    fseek( m_pStream, nCount, SEEK_CUR );
}

/** 关闭该文件流
*/
void FSysFileStream::Close(void)
{
    if( m_pStream )
    {
        fclose( m_pStream );
        m_pStream = NULL;
    }
}

/** 读取指定数量的数据到缓冲区
@Param 缓冲区指针
@Param 要读取数据的量
@Return 读取数据的大小
*/
size_t FSysFileStream::Read( void* pBuf, size_t nSize )
{
    FASSERT( m_pStream );
    return fread( pBuf, 1, nSize, m_pStream );
}