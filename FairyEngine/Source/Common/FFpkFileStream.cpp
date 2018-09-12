/*
 * ------------------------------------------------------------------------
 *  Name:   FFpkFileStream.cpp
 *  Desc:   ���ļ�ʵ����FPK�ļ�������
 *  Author: Yish
 *  Date:   2011/10/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FFpkFileStream.h"
#include "FFpkFilePack.h"
#include "FCompress.h"
#include "FLogManager.h"

/** ���캯��
*/
FFpkFileStream::FFpkFileStream( FILE* pakFile,SFPKFileEntry* pInfo )
{
    InitStream( pakFile,pInfo );
}

/** ���캯��
*/
FFpkFileStream::FFpkFileStream( const AString& name,FILE* pakFile,SFPKFileEntry* pInfo )
    : FVFileStream(name)
{
    InitStream( pakFile,pInfo );
}

/** ��������
*/
FFpkFileStream::~FFpkFileStream(void)
{
    Close();
}

/** ��ʼ���ļ���
*/
void FFpkFileStream::InitStream( FILE* pakFile,SFPKFileEntry* pInfo )
{
    m_pFPKStream = pakFile;
    m_pInfo = pInfo;
    m_nStreamSize = pInfo->nOriginalSize;
    m_nCurOffset = 0;
    m_nCurBlock = -1;
    m_nBlockSize = 0;
    m_pTempMem = NULL;
    m_pFileBuf = NULL;
	m_pCompress = NULL;

    if( m_nStreamSize == 0 )
        return;

	// ����ѹ����
	m_pCompress = FCompressor::Create((FCompressor::ECompressType)(pInfo->nFlag & 0xff));
	if( !m_pCompress )
	{
		FLOG_WARNINGF("FFpkFileStream::InitStream, Create compressor failed! (%s)", m_sName.c_str());
		return;
	}

    // ����ѹ�������Ĳ�ͬ���������
    uint32 quality = pInfo->nFlag & 0xff00;
    uint32 numBlocks,lastSize,curDecompSize;
    if( quality == FPK_QUALITY_NONE )
        return;
    else if( quality == FPK_QUALITY_ENTIRE )
    {
        // ��ȡ��������
        m_pFileBuf = (fbyte*)F_MALLOC( pInfo->nCompressedSize );
        fseek_64( m_pFPKStream,pInfo->nOffset,SEEK_SET );
        fread( m_pFileBuf,1,pInfo->nCompressedSize,m_pFPKStream );

        m_pTempMem = (fbyte*)F_MALLOC( m_nStreamSize );
        curDecompSize = (uint32)m_nStreamSize;
        m_pCompress->Decompress(m_pFileBuf, pInfo->nCompressedSize, m_pTempMem, curDecompSize);
        FASSERT( curDecompSize == (uint32)m_nStreamSize );
    }
    else
    {
        switch( quality )
        {
        case FPK_QUALITY_LOW:
            m_nBlockSize = BLOCK_LOW;
            break;
        case FPK_QUALITY_NORMAL:
            m_nBlockSize = BLOCK_NORMAL;
            break;
        case FPK_QUALITY_HIGH:
            m_nBlockSize = BLOCK_HIGH;
            break;
        default: FASSERT(0); break;
        }

        numBlocks = (uint32)m_nStreamSize / m_nBlockSize;
        lastSize = (uint32)m_nStreamSize % m_nBlockSize;
        if( lastSize ) ++numBlocks;
        m_pTempMem = (fbyte*)F_MALLOC( m_nBlockSize );

        // �ݴ���ļ��ж�ȡ�����ݿ�
        uint32 destSize = m_pCompress->CompressBound( m_nBlockSize );
        m_pFileBuf = (fbyte*)F_MALLOC( destSize );

        uint32 curOffset = pInfo->nOffset;
        while( numBlocks > 0 )
        {
            SBlockInfo blk;
            blk.offset = curOffset + sizeof(uint32);
            fseek_64( m_pFPKStream,curOffset,SEEK_SET );
            fread( &blk.compress_size,sizeof(uint32),1,m_pFPKStream );
            blk.orig_size = ((numBlocks != 1) || !lastSize) ? m_nBlockSize : lastSize;
            m_Blocks.push_back( blk );
            curOffset += sizeof(uint32) + blk.compress_size;
            --numBlocks;
        }
    }
}

/** �ر��ļ���
*/
void FFpkFileStream::Close(void)
{
    F_SAFE_FREE( m_pTempMem );
    F_SAFE_FREE( m_pFileBuf );

	F_SAFE_DELETE(m_pCompress);
}

/** ���ļ�ָ���ƶ���ָ��λ��
@Param ���λ��
@Param START, END, CURRENT
*/
void FFpkFileStream::Seek( long nPos, int flag )
{
	long nCurOffset = 0;

    switch( flag )
    {
    case START:
        nCurOffset = nPos;
        break;
    case CURRENT:
        nCurOffset += nPos;
        break;
    case END:
        nCurOffset = (long)m_nStreamSize - 1 + nPos;
        break;
    }

    if( nCurOffset < 0 ) nCurOffset = 0;
    if( nCurOffset > (long)m_nStreamSize ) nCurOffset = (long)m_nStreamSize;
	m_nCurOffset = (size_t)nCurOffset;
}

/** ��ȡָ������������
@Param �洢��ȡ������
@Param Ҫ��ȡ�Ĵ�С
@Return ʵ�ʶ�ȡ���ݵĴ�С
*/
size_t FFpkFileStream::Read( void* pBuf, size_t nSize )
{
    size_t readSize = 0;
    size_t realOffset = 0;

    uint32 quality = m_pInfo->nFlag & 0xff00;
    if( quality == FPK_QUALITY_NONE )
    {
        realOffset = m_pInfo->nOffset + m_nCurOffset;
        m_nCurOffset += nSize;
        readSize = nSize;
        if( m_nCurOffset > m_nStreamSize )
        {
            readSize = nSize - (m_nCurOffset - m_nStreamSize);
            m_nCurOffset = m_nStreamSize;
        }

        fseek_64( m_pFPKStream,realOffset,SEEK_SET );
        fread( pBuf,1,readSize,m_pFPKStream );
    }
    else if( quality == FPK_QUALITY_ENTIRE )
    {
        realOffset = (uint32)m_nCurOffset;
        m_nCurOffset += nSize;
        readSize = nSize;
        if( m_nCurOffset > m_nStreamSize )
        {
            readSize = nSize - (m_nCurOffset - m_nStreamSize);
            m_nCurOffset = m_nStreamSize;
        }

        memcpy( pBuf,&m_pTempMem[realOffset],readSize );
    }
    else
    {
        // �����ݴ���ļ��ж���������
        uint32 decompSize;
        uint32 outOffset = 0;
        fbyte* outBuf = static_cast<fbyte*>(pBuf);

        readSize = nSize;
        while( nSize > 0 )
        {
            uint32 curBlock = (uint32)m_nCurOffset / m_nBlockSize;
            uint32 offsetInBlk = (uint32)m_nCurOffset % m_nBlockSize;

            if( m_nCurBlock != curBlock )
            {
                // ��ѹ��ǰ��
                fseek_64( m_pFPKStream,m_Blocks[curBlock].offset,SEEK_SET );
                fread( m_pFileBuf,1,m_Blocks[curBlock].compress_size,m_pFPKStream );
                decompSize = m_nBlockSize;
                m_pCompress->Decompress( m_pFileBuf,m_Blocks[curBlock].compress_size,m_pTempMem,decompSize );
                m_nCurBlock = curBlock;
            }

            // ��ȡ����
            uint32 thisRead;
            uint32 curBlkSize = m_Blocks[curBlock].orig_size;
            if( nSize > curBlkSize - offsetInBlk )
            {
                thisRead = curBlkSize - offsetInBlk;
                memcpy( &outBuf[outOffset],&m_pTempMem[offsetInBlk],thisRead );
                outOffset += thisRead;
                m_nCurOffset += thisRead;
                nSize -= thisRead;

                // ����ļ��Ѿ����������˳�
                if( m_nCurOffset >= m_nStreamSize )
                    break;
            }
            else
            {
                thisRead = (uint32)nSize;
                memcpy( &outBuf[outOffset],&m_pTempMem[offsetInBlk],thisRead );
                outOffset += thisRead;
                m_nCurOffset += thisRead;
                nSize = 0;
            }
        }

        readSize -= nSize;
    }

    return readSize;
}