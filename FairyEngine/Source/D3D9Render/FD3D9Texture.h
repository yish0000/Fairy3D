/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Texture.h
 *  Desc:   本文件定义了一个D3D9下的纹理对象。
 *  Author: Yish
 *  Date:   2012/2/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_TEXTURE_H__
#define __FD3D9_TEXTURE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Resource.h"
#include "F3DTexture.h"

///////////////////////////////////////////////////////////////////////////


/** D3D9纹理对象
@remarks
    属于D3D
*/
class FD3D9Texture : public F3DTexture, public FD3D9Resource 
{
protected:
    LPDIRECT3DDEVICE9 m_pd3dDevice;         // D3D设备指针
    LPDIRECT3DBASETEXTURE9 m_pBaseTexture;  // D3D纹理指针

    // Build the mipmaps.
    HRESULT BuildSubMipmaps( int nLevel );

    // Use this function when Create the normal map.
    DWORD VectorToRGBA( F3DVector* pv,float fHeight );

public:
    // Constructor and destructor.
    FD3D9Texture( LPDIRECT3DDEVICE9 pd3dDevice );
    ~FD3D9Texture(void);

    // Override the interface's functions.
    void ResetContent(void);

    HRESULT Create2DTexture( UINT nWidth,UINT nHeight,ETextureUsage Usage,
                             PIXELFORMAT Format,UINT nMipLevels );
    HRESULT Create3DTexture( UINT nWidth,UINT nHeight,UINT nDepth,TEXTUREUSAGE Usage,
                             PIXELFORMAT Format,UINT nMipLevels );
    HRESULT CreateCubeTexture( UINT nEdgeLength,TEXTUREUSAGE Usage,
                               PIXELFORMAT Format,UINT nMipLevels );

    HRESULT AutoBuildLodData(void);

    bool Load2DTexture( const char* filename,uint32 nMipLevels );
    void Save2DTexture( const char* filename ) const;

    // Only for the DDS format.
    HRESULT Load3DTexture( const char* cFilename,UINT nMipLevels );
    HRESULT LoadCubeTexture( const char* cFilename,UINT nMipLevels );
    HRESULT Save3DTexture( const char* cFilename ) const;
    HRESULT SaveCubeTexture( CUBEFACE Face,const char* cFilename,FLYIMAGETYPE Type ) const;

    // 是否包含ALPHA通道
    bool HasAlphaChannel() const;

    // Modify the 2D texture.
    HRESULT SetAlphaKey( UCHAR r,UCHAR g,UCHAR b,UCHAR alpha );
    HRESULT SetTransparency( UCHAR alpha );
    HRESULT ConvertToNormalmap(void);

    // Access the texture's datas.
    HRESULT Access2DMap( int nLevel,TEXTUREACESS tma,void** ppBits,int* pitch );
    HRESULT Access3DMap( int nLevel,TEXTUREACESS tma,void** ppBits,int* rowPitch,int* slicePitch );
    HRESULT AccessCubeMap( CUBEFACE Face,int nLevel,TEXTUREACESS tma,void** ppBits,int* pitch );
    HRESULT Unaccess2DMap( int nLevel );
    HRESULT Unaccess3DMap( int nLevel );
    HRESULT UnaccessCubeMap( CUBEFACE Face,int nLevel );

    // 获取D3D纹理对象指针
    LPDIRECT3DBASETEXTURE9 _GetD3DTexturePtr(void) { return m_pBaseTexture; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_TEXTURE_H__