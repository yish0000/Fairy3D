/*
 * ------------------------------------------------------------------------
 *  Name:   FFpkFileStream.h
 *  Desc:   本文件定义了一个FPK文件流对象。
 *  Author: Yish
 *  Date:   2011/10/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_FPKFILESTREAM_H__
#define __FAIRY_FPKFILESTREAM_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FVFileStream.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//// Define and macros

#if FAIRY_COMPILER == FAIRY_COMPILER_MSVC
	#define fseek_64(file, offset, pos) _fseeki64(file, offset, pos)
	#define ftell_64(file) _ftelli64(file)
#else
	#define fseek_64(file, offset, pos) fseeko(file, offset, pos)
	#define ftell_64(file) ftello(file)
#endif

///////////////////////////////////////////////////////////////////////////

struct SFPKFileEntry;
class FCompressor;

///////////////////////////////////////////////////////////////////////////
//  
//  Class FFpkFileStream
//  
///////////////////////////////////////////////////////////////////////////

class FFpkFileStream : public FVFileStream
{
    struct SBlockInfo
    {
        uint32 offset;          // 数据块的偏移
        uint32 compress_size;   // 压缩后的大小
        uint32 orig_size;       // 原始大小
    };

    typedef std::vector<SBlockInfo> BlockList;

protected:
    FILE* m_pFPKStream;     // FPK文件包的指针
    SFPKFileEntry* m_pInfo; // 该文件的信息
    size_t m_nCurOffset;    // 当前文件流的偏移

    uint32 m_nBlockSize;    // 每块的大小
    uint32 m_nCurBlock;     // 指针所在的数据块
    BlockList m_Blocks;     // 文件分块信息

    FBYTE* m_pTempMem;      // 临时内存用于解压数据
    FBYTE* m_pFileBuf;      // 用于暂存从文件中读出的数据块

	FCompressor* m_pCompress;	// 压缩器

public:
    FFpkFileStream( FILE* pakFile,SFPKFileEntry* pInfo );
    FFpkFileStream( const AString& name,FILE* pakFile,SFPKFileEntry* pInfo );
    ~FFpkFileStream(void);

    void Seek( long nPos,int flag/* =START */ );
    size_t Tell(void) const { return m_nCurOffset; }
    bool Eof(void) const { return (m_nCurOffset == m_nStreamSize); }
    void Skip( long nCount ) { Seek(nCount,CURRENT); }
    void Close(void);

    size_t Read( void* pBuf,size_t nSize );
	size_t Write(void* pBuf, size_t nSize) { return 0; }

private:
    // 初始化文件流
    void InitStream( FILE* pakFile,SFPKFileEntry* pInfo );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_FPKFILESTREAM_H__