/*
 * ------------------------------------------------------------------------
 *  Name:   FZipFileStream.h
 *  Desc:   本文件定义了基于ZIP压缩包的文件数据流。
 *  Author: Yish
 *  Date:   10/3/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_ZIPFILESTREAM_H__
#define __FAIRY_ZIPFILESTREAM_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FVFileStream.h"

///////////////////////////////////////////////////////////////////////////

typedef struct zzip_file ZZIP_FILE;

/** ZIP文件数据流
@remarks
@note
*/
class FZipFileStream : public FVFileStream
{
protected:
    ZZIP_FILE* m_pZipFile;

public:
    FZipFileStream( ZZIP_FILE* pZipFile, size_t uncompressSize );
    FZipFileStream( const AString& name, ZZIP_FILE* pZipFile, size_t uncompressSize );
    ~FZipFileStream(void);

    void Seek( long nPos, int flag/* =START */ );
    size_t Tell(void) const;
    bool Eof(void) const;
    void Skip( long nCount );
    void Close(void);

    size_t Read( void* pBuf, size_t nSize );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_ZIPFILESTREAM_H__