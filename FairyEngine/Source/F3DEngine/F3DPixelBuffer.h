/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPixelBuffer.h
 *  Desc:   本文件定义了一个引擎的像素缓冲区基类。
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

/** 像素缓冲区
*/
class FAIRY_API F3DPixelBuffer : public FGeneralAlloc
{
public:
	F3DPixelBuffer( uint32 nWidth, uint32 nHeight, EPixelFormat Format,
		EBufferUsage Usage, EMemoryMode MemMode );
	virtual ~F3DPixelBuffer();

	// 重新扩充缓冲区大小
	virtual bool Resize( uint32 nWidth, uint32 nHeight, EPixelFormat Format ) = 0;

	// 锁定像素缓冲区指定区域
	virtual void* Lock( const F2DRECT& rc, ELockOption lockMode, uint32& nSlice ) = 0;

	// 像素缓冲区解锁
	virtual void Unlock() = 0;

	// 从另一个缓冲区中拷贝数据
	virtual bool Blit( F3DPixelBuffer* pBuffer, const F2DRECT& rcSrc, const F2DRECT& rcDst );
	// 从内存中拷贝数据
	virtual bool BlitFromMemory( EPixelFormat Format, fbyte* pData, uint32 nSlice, const F2DRECT& rcSrc, const F2DRECT& rcDest ) = 0;
	// 拷贝缓冲区内的指定的数据到内存
	virtual bool BlitToMemory( const F2DRECT& rcSrc, fbyte** pData, uint32* nSlice ) = 0;

	// 拷贝整个缓冲区到这里
	bool Blit( F3DPixelBuffer* pBuffer );
	// 从指定内存中拷贝数据到缓冲区的全部
	bool BlitFromMemory( EPixelFormat Format, fbyte* pData, uint32 nSlice, const F2DRECT& rcSrc );
	// 拷贝整个缓冲区到内存
	bool BlitToMemory( fbyte** ppData, uint32* nSlice );

	// 获取渲染纹理
	virtual F3DRenderTexture* GetRenderTexture() = 0;

	// 获取宽度
	uint32 GetWidth() const { return m_nWidth; }
	// 获取高度
	uint32 GetHeight() const { return m_nHeight; }
	// 获取像素格式
	EPixelFormat GetPixelFormat() const { return m_Format; }
	// 缓冲区是否被锁定
	bool IsLocked() const { return m_bLocked; }
	// 获取像素缓冲区的USAGE
	EBufferUsage GetUsage() const { return m_Usage; }
	// 获取内存模式
	EMemoryMode GetMemoryMode() const { return m_MemMode; }

protected:
	uint32 m_nWidth;			// 宽度
	uint32 m_nHeight;			// 高度
	EPixelFormat m_Format;		// 像素格式
	EBufferUsage m_Usage;		// 
	EMemoryMode m_MemMode;		// 内存模式
	bool m_bLocked;				// 是否被锁定
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __F3D_PIXELBUFFER_H__