/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9PixelBuffer.h
 *  Desc:   本文件定义了一个D3D9渲染器的像素缓冲区。
 *  Author: Yish
 *  Date:   2012/12/23
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_PIXELBUFFER_H__
#define __FD3D9_PIXELBUFFER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Resource.h"
#include "F3DPixelBuffer.h"

///////////////////////////////////////////////////////////////////////////


/** 像素缓冲区
*/
class FD3D9PixelBuffer : public F3DPixelBuffer, public FD3D9Resource
{
protected:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DSURFACE9 m_pSurface;			// SURFACE数据
	LPDIRECT3DSURFACE9 m_pFSAASurface;
    BYTE* m_pTempBuf;

public:
	FD3D9PixelBuffer( LPDIRECT3DDEVICE9 pd3dDevice, uint32 nWidth, uint32 nHeight, EPixelFormat Format,
		EBufferUsage Usage, EMemoryMode MemMode );
	~FD3D9PixelBuffer();

	// 重新扩充缓冲区的大小
	bool Resize( uint32 nWidth, uint32 nHeight, EPixelFormat Format );

	// 锁定像素缓冲区指定区域
	void* Lock( const F2DRECT& rc, ELockOption lockMode, uint32& nSlice );

	// 像素缓冲区解锁
	void Unlock();

	// 从另一个缓冲区中拷贝数据
	bool Blit( F3DPixelBuffer* pBuffer, const F2DRECT& rcSrc, const F2DRECT& rcDst );
	// 从内存中拷贝数据
	bool BlitFromMemory( EPixelFormat Format, BYTE* pData, uint32 nSlice, const F2DRECT& rcSrc, const F2DRECT& rcDest );
	// 拷贝缓冲区内的指定的数据到内存
	bool BlitToMemory( const F2DRECT& rcSrc, BYTE** pData, uint32* nSlice );

	// 获取渲染纹理
	F3DRenderTexture* GetRenderTexture();

	// 设备丢失和恢复时响应
	void OnDeviceLost(void);
	void OnDeviceReset(void);

	// 获取D3D顶点缓冲区指针
	LPDIRECT3DSURFACE9 _GetD3DPBPtr(void) { return m_pSurface; }

protected:

	// 创建渲染纹理
	bool CreateRenderTexture();
	// 销毁渲染纹理
	void DestroyRenderTexture();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FD3D9_PIXELBUFFER_H__