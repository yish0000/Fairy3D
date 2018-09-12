/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9PixelBuffer.h
 *  Desc:   ���ļ�������һ��D3D9��Ⱦ�������ػ�������
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


/** ���ػ�����
*/
class FD3D9PixelBuffer : public F3DPixelBuffer, public FD3D9Resource
{
protected:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DSURFACE9 m_pSurface;			// SURFACE����
	LPDIRECT3DSURFACE9 m_pFSAASurface;
    BYTE* m_pTempBuf;

public:
	FD3D9PixelBuffer( LPDIRECT3DDEVICE9 pd3dDevice, uint32 nWidth, uint32 nHeight, EPixelFormat Format,
		EBufferUsage Usage, EMemoryMode MemMode );
	~FD3D9PixelBuffer();

	// �������仺�����Ĵ�С
	bool Resize( uint32 nWidth, uint32 nHeight, EPixelFormat Format );

	// �������ػ�����ָ������
	void* Lock( const F2DRECT& rc, ELockOption lockMode, uint32& nSlice );

	// ���ػ���������
	void Unlock();

	// ����һ���������п�������
	bool Blit( F3DPixelBuffer* pBuffer, const F2DRECT& rcSrc, const F2DRECT& rcDst );
	// ���ڴ��п�������
	bool BlitFromMemory( EPixelFormat Format, BYTE* pData, uint32 nSlice, const F2DRECT& rcSrc, const F2DRECT& rcDest );
	// �����������ڵ�ָ�������ݵ��ڴ�
	bool BlitToMemory( const F2DRECT& rcSrc, BYTE** pData, uint32* nSlice );

	// ��ȡ��Ⱦ����
	F3DRenderTexture* GetRenderTexture();

	// �豸��ʧ�ͻָ�ʱ��Ӧ
	void OnDeviceLost(void);
	void OnDeviceReset(void);

	// ��ȡD3D���㻺����ָ��
	LPDIRECT3DSURFACE9 _GetD3DPBPtr(void) { return m_pSurface; }

protected:

	// ������Ⱦ����
	bool CreateRenderTexture();
	// ������Ⱦ����
	void DestroyRenderTexture();
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FD3D9_PIXELBUFFER_H__