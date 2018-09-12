//------------------------------------------------------------------------
// Name: FlyD3DGraphicBuffer.h
// Desc: This file defined two classes for the vertex buffer and index
//       buffer in the Direct3D platform.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DGRAPHIC_BUFFER_H__
#define __FLY_D3DGRAPHIC_BUFFER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyGraphicBuffer.h"

#include "FlyD3DResource.h"
//////////////////////////////////////////////////////////////////////////


class FlyD3DVertexBuffer : public FlyGraphicBuffer, public FlyD3DResource
{
protected:
    LPDIRECT3DDEVICE9 m_pd3dDevice;
    LPDIRECT3DVERTEXBUFFER9 m_pVB;

public:
    // Constructor and destructor.
    FlyD3DVertexBuffer( LPDIRECT3DDEVICE9 pd3dDevice,UINT nSize,BUFFERUSAGE Usage,MEMORYMODE MemMode );
    ~FlyD3DVertexBuffer(void);

    HRESULT Resize( UINT nSize );
    void* Lock( UINT nOffset,UINT nLength,LOCKOPTION options );
    HRESULT Unlock(void);

    HRESULT ReadData( UINT nOffset,UINT nLength,void* pDest );
    HRESULT WriteData( UINT nOffset,UINT nLength,const void* pSource,bool bDiscard );

    // Override the virtual function to lost and reset the buffer.
    HRESULT OnDeviceLost(void);
    HRESULT OnDeviceReset(void);

    LPDIRECT3DVERTEXBUFFER9 GetD3DVBPtr(void);
};

//////////////////////////////////////////////////////////////////////////

class FlyD3DIndexBuffer : public FlyGraphicBuffer, public FlyD3DResource
{
protected:
    INDEXFORMAT m_IFormat;
    LPDIRECT3DDEVICE9 m_pd3dDevice;
    LPDIRECT3DINDEXBUFFER9 m_pIB;

public:
    // Constructor and destructor.
    FlyD3DIndexBuffer( LPDIRECT3DDEVICE9 pd3dDevice,UINT nSize,BUFFERUSAGE Usage,
                       MEMORYMODE MemMode,INDEXFORMAT Format );
    ~FlyD3DIndexBuffer(void);

    HRESULT Resize( UINT nSize );
    void* Lock( UINT nOffset,UINT nLength,LOCKOPTION options );
    HRESULT Unlock(void);

    HRESULT ReadData( UINT nOffset,UINT nLength,void* pDest );
    HRESULT WriteData( UINT nOffset,UINT nLength,const void* pSource,bool bDiscard );

    // Override the virtual function to lost and reset the buffer.
    HRESULT OnDeviceLost(void);
    HRESULT OnDeviceReset(void);

    LPDIRECT3DINDEXBUFFER9 GetD3DIBPtr(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DGRAPHIC_BUFFER_H__