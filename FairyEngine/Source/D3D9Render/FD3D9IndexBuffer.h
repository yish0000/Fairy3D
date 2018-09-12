/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9IndexBuffer.h
 *  Desc:   ���ļ�������һ��D3D9��Ⱦ����������������
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


/** ����������
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

	// ���·��仺�����Ĵ�С
	bool Resize( uint32 nSize );

	// ����������
	void* Lock( uint32 nOffset,uint32 nLength,ELockOption options );
	void Unlock(void);

	// ��ȡ����
	void ReadData( uint32 nOffset,uint32 nLength,void* pDest );

	// д������
	void WriteData( uint32 nOffset,uint32 nLength,const void* pSource,bool bDiscard );

	// ���豸��ʧ�ͻָ�ʱ��Ӧ
	void OnDeviceLost(void);
	void OnDeviceReset(void);

	// ��ȡINDEX BUFFERָ��
	LPDIRECT3DINDEXBUFFER9 _GetD3DIBPtr(void) { return m_pIB; }
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FD3D9_INDEXBUFFER_H__