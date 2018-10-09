/*
 * ------------------------------------------------------------------------
 *  Name:   FSysFileStream.h
 *  Desc:   本文件定义了磁盘文件的数据流。
 *  Author: Yish
 *  Date:   10/2/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_SYSFILESTREAM_H__
#define __FAIRY_SYSFILESTREAM_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FVFileStream.h"

///////////////////////////////////////////////////////////////////////////

/** File stream on the disk.
*/
class FSysFileStream : public FVFileStream
{
public:
    FSysFileStream( const AString& name );
    FSysFileStream( const AString& name, FILE* pStream );
    FSysFileStream( const AString& name, FILE* pStream, size_t nSize );
    ~FSysFileStream(void);

    void Seek( long nPos, int flag = START );
    size_t Tell(void) const;
    bool Eof(void) const;
    void Skip( long nCount );
    void Close(void);

    // 读取数据
    size_t Read( void* pBuf, size_t nSize );
	size_t Write(void* pBuf, size_t nSize);

protected:
	FILE* m_pStream;	// 标准文件流指针
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_SYSFILESTREAM_H__