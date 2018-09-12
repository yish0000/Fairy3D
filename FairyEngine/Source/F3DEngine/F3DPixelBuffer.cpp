/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPixelBuffer.cpp
 *  Desc:   ���ļ�������һ����������ػ�������
 *  Author: Yish
 *  Date:   2012/12/29
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DPixelBuffer.h"


/** ���캯��
*/
F3DPixelBuffer::F3DPixelBuffer( uint32 nWidth, uint32 nHeight, EPixelFormat Format,
	EBufferUsage Usage, EMemoryMode MemMode ) : m_nWidth(nWidth), m_nHeight(nHeight), 
	m_Format(Format), m_Usage(Usage), m_MemMode(MemMode), m_bLocked(false)
{
}

/** ��������
*/
F3DPixelBuffer::~F3DPixelBuffer()
{
}

/** ������һ�������������ݵ�����
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

/** ��������������������
*/
bool F3DPixelBuffer::Blit( F3DPixelBuffer* pBuffer )
{
	return Blit(pBuffer, F2DRECT(0, 0, pBuffer->GetWidth(), pBuffer->GetHeight()),
		F2DRECT(0, 0, m_nWidth, m_nHeight));
}

/** ��ָ���ڴ��п������ݵ���������ȫ��
*/
bool F3DPixelBuffer::BlitFromMemory( EPixelFormat Format, fbyte* pData, uint32 nSlice, const F2DRECT& rcSrc )
{
	return BlitFromMemory(Format, pData, nSlice, rcSrc, F2DRECT(0, 0, m_nWidth, m_nHeight));
}

/** �����������������ڴ�
*/
bool F3DPixelBuffer::BlitToMemory( fbyte** ppData, uint32* nSlice )
{
	return BlitToMemory(F2DRECT(0, 0, m_nWidth, m_nHeight), ppData, nSlice);
}