/*
 * ------------------------------------------------------------------------
 *  Name:   FMemFileStream.cpp
 *  Desc:   This file define a stream to handle a block of memory.
 *  Author: Yish
 *  Date:   2012/1/31
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FMemFileStream.h"
#include "FTemplate.h"

/** Constructor.
*/
FMemFileStream::FMemFileStream()
{
	m_pData = NULL;
	m_pOffset = NULL;
	m_pEnd = NULL;
	m_bFreeOnClose = false;
	m_nStreamSize = 0;
}

/** Destructor.
*/
FMemFileStream::FMemFileStream( fbyte* pBuf, uint32 nBufLen )
{
	m_pData = m_pOffset = pBuf;
	m_pEnd = pBuf + nBufLen;
	m_bFreeOnClose = false;
	m_nStreamSize = nBufLen;
}

/** Create a specified size of the block of memory stream.
*/
FMemFileStream::FMemFileStream( uint32 nBufLen )
{
	m_nStreamSize = nBufLen;
	m_pData = (fbyte*)F_MALLOC( nBufLen );
	m_pOffset = m_pData;
	m_pEnd = m_pData + nBufLen;
	m_bFreeOnClose = true;
}

/** Copy another file stream.
*/
FMemFileStream::FMemFileStream( FVFileStream* pStream )
{
	m_nStreamSize = pStream->GetStreamSize();
	m_pData = (fbyte*)F_MALLOC( m_nStreamSize );
	m_pOffset = m_pData;
	m_pEnd = m_pData + m_nStreamSize;
	size_t offset = pStream->Tell();
	pStream->Seek( 0, FVFileStream::START );
	pStream->Read( m_pData, m_nStreamSize );
	pStream->Seek( (long)offset, FVFileStream::START );
	m_bFreeOnClose = true;
}

/** Destructor.
*/
FMemFileStream::~FMemFileStream()
{
	Close();
}

/** Close the stream.
*/
void FMemFileStream::Close()
{
	if( m_bFreeOnClose )
		F_FREE( m_pData );
}

/** Seek to the specified position.
*/
void FMemFileStream::Seek( long nPos, int flag/* =START */ )
{
	switch( flag )
	{
	case START:
		m_pOffset = m_pData + nPos;
		break;
	case CURRENT:
		m_pOffset += nPos;
		break;
	case END:
		m_pOffset = m_pEnd - nPos;
		break;
	default: FASSERT(0);
	}

	F_Clamp( m_pOffset, m_pData, m_pEnd );
}

/** Get the current position.
*/
size_t FMemFileStream::Tell() const
{
	return (size_t)(m_pOffset - m_pData);
}

/** 检测是否到达文件流末尾
*/
bool FMemFileStream::Eof() const
{
	return (m_pOffset >= m_pEnd);
}

/** 跳过指定数目的数据
*/
void FMemFileStream::Skip( long nCount )
{
	m_pOffset += nCount;
	F_Clamp( m_pOffset, m_pData, m_pEnd );
}

/** 读取指定数目的数据
*/
size_t FMemFileStream::Read( void* pBuf, size_t nSize )
{
	if( m_pOffset + nSize > m_pEnd )
		nSize = (size_t)(m_pEnd - m_pOffset);
	if( nSize ) memcpy( pBuf, m_pOffset, nSize );
	m_pOffset += nSize;
	return nSize;
}

/** 写入指定大小的数据
*/
size_t FMemFileStream::Write(void* pBuf, size_t nSize)
{
	if (m_pOffset + nSize > m_pEnd)
		nSize = (size_t)(m_pEnd - m_pOffset);
	if (nSize)
		memcpy(m_pOffset, pBuf, nSize);
	m_pOffset += nSize;
	return nSize;
}