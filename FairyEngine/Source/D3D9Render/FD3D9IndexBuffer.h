/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9IndexBuffer.h
 *  Desc:   本文件定义了一个D3D9渲染器的索引缓冲区。
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_INDEXBUFFER_H__
#define __FD3D9_INDEXBUFFER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Common.h"
#include "FD3D9Resource.h"

#include "F3DGraphicBuffer.h"

///////////////////////////////////////////////////////////////////////////


/** 索引缓冲区
*/
class FD3D9IndexBuffer : public F3DGraphicBuffer, public FD3D9Resource
{
protected:
	EIndexFormat m_IFormat;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	BYTE* m_pTempBuf;

public:
	FD3D9IndexBuffer( LPDIRECT3DDEVICE9 pd3dDevice,uint32 nSize,EBufferUsage Usage,
		EMemoryMode MemMode,EIndexFormat Format );
	~FD3D9IndexBuffer(void);

	// 重新分配缓冲区的大小
	bool Resize( uint32 nSize );

	// 锁定缓冲区
	void* Lock( uint32 nOffset,uint32 nLength,ELockOption options );
	void Unlock(void);

	// 读取数据
	void ReadData( uint32 nOffset,uint32 nLength,void* pDest );

	// 写入数据
	void WriteData( uint32 nOffset,uint32 nLength,const void* pSource,bool bDiscard );

	// 当设备丢失和恢复时响应
	void OnDeviceLost(void);
	void OnDeviceReset(void);

	// 获取INDEX BUFFER指针
	LPDIRECT3DINDEXBUFFER9 _GetD3DIBPtr(void) { return m_pIB; }
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FD3D9_INDEXBUFFER_H__