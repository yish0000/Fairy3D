/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9GraphicBuffer.h
 *  Desc:   本文件定义了一个D3D9顶点缓冲区对象。
 *  Author: Yish
 *  Date:   2012/12/22
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_GRAPHICBUFFER_H__
#define __FD3D9_GRAPHICBUFFER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Common.h"
#include "FD3D9Resource.h"

#include "F3DGraphicBuffer.h"

///////////////////////////////////////////////////////////////////////////


/** 顶点缓冲区
*/
class FD3D9VertexBuffer : public F3DGraphicBuffer, public FD3D9Resource
{
protected:
    LPDIRECT3DDEVICE9 m_pd3dDevice;
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
	BYTE* m_pTempBuf;

public:
    FD3D9VertexBuffer( LPDIRECT3DDEVICE9 pd3dDevice,uint32 nSize,EBufferUsage Usage,EMemoryMode MemMode );
    ~FD3D9VertexBuffer(void);

	// 重新分配缓冲区的大小
    bool Resize( uint32 nSize );

	// 锁定缓冲
    void* Lock( uint32 nOffset,uint32 nLength,ELockOption options );
    void Unlock(void);

	// 读取数据
    void ReadData( uint32 nOffset,uint32 nLength,void* pDest );

	// 写入数据
    void WriteData( uint32 nOffset,uint32 nLength,const void* pSource,bool bDiscard );

    // 设备丢失和恢复时响应
    void OnDeviceLost(void);
    void OnDeviceReset(void);

	// 获取D3D顶点缓冲区指针
	LPDIRECT3DVERTEXBUFFER9 _GetD3DVBPtr(void) { return m_pVB; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_VERTEXBUFFER_H__