/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPixelBuffer.cpp
 *  Desc:   本文件定义了一个引擎的像素缓冲区。
 *  Author: Yish
 *  Date:   2012/12/29
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DPixelBuffer.h"


/** 构造函数
*/
F3DPixelBuffer::F3DPixelBuffer( uint32 nWidth, uint32 nHeight, EPixelFormat Format,
	EBufferUsage Usage, EMemoryMode MemMode ) : m_nWidth(nWidth), m_nHeight(nHeight), 
	m_Format(Format), m_Usage(Usage), m_MemMode(MemMode), m_bLocked(false)
{
}

/** 析构函数
*/
F3DPixelBuffer::~F3DPixelBuffer()
{
}

/** 拷贝另一个缓冲区的数据到这里
*/
bool F3DPixelBuffer::Blit( F3DPixelBuffer* pBuffer, const F2DRECT& rcSrc, const F2DRECT& rcDst )
{
	if( !pBuffer || pBuffer == this )
		return true;

	if( IsLocked() || pBuffer->IsLocked() )
	{
		FLOG_ERROR("F3DPixelBuffer::Blit, Source buffer or me is locked!");
		return false;
	}

	return true;
}

/** 拷贝整个缓冲区到这里
*/
bool F3DPixelBuffer::Blit( F3DPixelBuffer* pBuffer )
{
	return Blit(pBuffer, F2DRECT(0, 0, pBuffer->GetWidth(), pBuffer->GetHeight()),
		F2DRECT(0, 0, m_nWidth, m_nHeight));
}

/** 从指定内存中拷贝数据到缓冲区的全部
*/
bool F3DPixelBuffer::BlitFromMemory( EPixelFormat Format, fbyte* pData, uint32 nSlice, const F2DRECT& rcSrc )
{
	return BlitFromMemory(Format, pData, nSlice, rcSrc, F2DRECT(0, 0, m_nWidth, m_nHeight));
}

/** 拷贝整个缓冲区到内存
*/
bool F3DPixelBuffer::BlitToMemory( fbyte** ppData, uint32* nSlice )
{
	return BlitToMemory(F2DRECT(0, 0, m_nWidth, m_nHeight), ppData, nSlice);
}