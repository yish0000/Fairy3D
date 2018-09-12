/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9GraphicBuffer.h
 *  Desc:   ���ļ�������һ��D3D9���㻺��������
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


/** ���㻺����
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

	// ���·��仺�����Ĵ�С
    bool Resize( uint32 nSize );

	// ��������
    void* Lock( uint32 nOffset,uint32 nLength,ELockOption options );
    void Unlock(void);

	// ��ȡ����
    void ReadData( uint32 nOffset,uint32 nLength,void* pDest );

	// д������
    void WriteData( uint32 nOffset,uint32 nLength,const void* pSource,bool bDiscard );

    // �豸��ʧ�ͻָ�ʱ��Ӧ
    void OnDeviceLost(void);
    void OnDeviceReset(void);

	// ��ȡD3D���㻺����ָ��
	LPDIRECT3DVERTEXBUFFER9 _GetD3DVBPtr(void) { return m_pVB; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_VERTEXBUFFER_H__