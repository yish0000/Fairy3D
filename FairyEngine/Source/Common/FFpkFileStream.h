/*
 * ------------------------------------------------------------------------
 *  Name:   FFpkFileStream.h
 *  Desc:   ���ļ�������һ��FPK�ļ�������
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
        uint32 offset;          // ���ݿ��ƫ��
        uint32 compress_size;   // ѹ����Ĵ�С
        uint32 orig_size;       // ԭʼ��С
    };

    typedef std::vector<SBlockInfo> BlockList;

protected:
    FILE* m_pFPKStream;     // FPK�ļ�����ָ��
    SFPKFileEntry* m_pInfo; // ���ļ�����Ϣ
    size_t m_nCurOffset;    // ��ǰ�ļ�����ƫ��

    uint32 m_nBlockSize;    // ÿ��Ĵ�С
    uint32 m_nCurBlock;     // ָ�����ڵ����ݿ�
    BlockList m_Blocks;     // �ļ��ֿ���Ϣ

    FBYTE* m_pTempMem;      // ��ʱ�ڴ����ڽ�ѹ����
    FBYTE* m_pFileBuf;      // �����ݴ���ļ��ж��������ݿ�

	FCompressor* m_pCompress;	// ѹ����

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
    // ��ʼ���ļ���
    void InitStream( FILE* pakFile,SFPKFileEntry* pInfo );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_FPKFILESTREAM_H__