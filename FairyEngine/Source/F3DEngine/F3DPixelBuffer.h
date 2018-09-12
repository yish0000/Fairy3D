/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPixelBuffer.h
 *  Desc:   ���ļ�������һ����������ػ��������ࡣ
 *  Author: Yish
 *  Date:   2012/12/29
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_PIXELBUFFER_H__
#define __F3D_PIXELBUFFER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DGraphicBuffer.h"

///////////////////////////////////////////////////////////////////////////


class F3DRenderTexture;

/** ���ػ�����
*/
class FAIRY_API F3DPixelBuffer : public FGeneralAlloc
{
public:
	F3DPixelBuffer( uint32 nWidth, uint32 nHeight, EPixelFormat Format,
		EBufferUsage Usage, EMemoryMode MemMode );
	virtual ~F3DPixelBuffer();

	// �������仺������С
	virtual bool Resize( uint32 nWidth, uint32 nHeight, EPixelFormat Format ) = 0;

	// �������ػ�����ָ������
	virtual void* Lock( const F2DRECT& rc, ELockOption lockMode, uint32& nSlice ) = 0;

	// ���ػ���������
	virtual void Unlock() = 0;

	// ����һ���������п�������
	virtual bool Blit( F3DPixelBuffer* pBuffer, const F2DRECT& rcSrc, const F2DRECT& rcDst );
	// ���ڴ��п�������
	virtual bool BlitFromMemory( EPixelFormat Format, fbyte* pData, uint32 nSlice, const F2DRECT& rcSrc, const F2DRECT& rcDest ) = 0;
	// �����������ڵ�ָ�������ݵ��ڴ�
	virtual bool BlitToMemory( const F2DRECT& rcSrc, fbyte** pData, uint32* nSlice ) = 0;

	// ��������������������
	bool Blit( F3DPixelBuffer* pBuffer );
	// ��ָ���ڴ��п������ݵ���������ȫ��
	bool BlitFromMemory( EPixelFormat Format, fbyte* pData, uint32 nSlice, const F2DRECT& rcSrc );
	// �����������������ڴ�
	bool BlitToMemory( fbyte** ppData, uint32* nSlice );

	// ��ȡ��Ⱦ����
	virtual F3DRenderTexture* GetRenderTexture() = 0;

	// ��ȡ���
	uint32 GetWidth() const { return m_nWidth; }
	// ��ȡ�߶�
	uint32 GetHeight() const { return m_nHeight; }
	// ��ȡ���ظ�ʽ
	EPixelFormat GetPixelFormat() const { return m_Format; }
	// �������Ƿ�����
	bool IsLocked() const { return m_bLocked; }
	// ��ȡ���ػ�������USAGE
	EBufferUsage GetUsage() const { return m_Usage; }
	// ��ȡ�ڴ�ģʽ
	EMemoryMode GetMemoryMode() const { return m_MemMode; }

protected:
	uint32 m_nWidth;			// ���
	uint32 m_nHeight;			// �߶�
	EPixelFormat m_Format;		// ���ظ�ʽ
	EBufferUsage m_Usage;		// 
	EMemoryMode m_MemMode;		// �ڴ�ģʽ
	bool m_bLocked;				// �Ƿ�����
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_PIXELBUFFER_H__