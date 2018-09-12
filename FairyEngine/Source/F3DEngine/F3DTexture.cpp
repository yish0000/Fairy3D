/*
 * -----------------------------------------------------------------------
 *  Name:   F3DTexture.cpp
 *  Desc:   This file implements the texture classes for engine. 
 *  Author: Yish
 *  Date:   2010/12/9
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DTexture.h"
#include "F3DImageLoader.h"
#include "F3DPixelUtil.h"


///////////////////////////////////////////////////////////////////////////
//// Implement class F3DTextureBase

/** Constructor.
*/
F3DTextureBase::F3DTextureBase( const AString& name )
	: m_sName(name), m_Type(TT_2D), m_Usage(TU_DEFAULT), m_nWidth(0), m_nHeight(0)
	, m_nDepth(0), m_Format(PFT_UNKNOWN), m_nMipCount(0)
{
}

/** Destructor.
*/
F3DTextureBase::~F3DTextureBase(void)
{
}

/** Does the pixel has the alpha channel.
*/
bool F3DTextureBase::HasAlphaChannel() const
{
	return F3D_HaveAlpha(m_Format);
}

///////////////////////////////////////////////////////////////////////////
//// Implement class F3DTexture2D

/** Constructor.
*/
F3DTexture2D::F3DTexture2D(const AString& name)
	: F3DTextureBase(name)
{
}

/** Destructor.
*/
F3DTexture2D::~F3DTexture2D()
{
}

/** 从文件中加载2D纹理
@Param 图像文件的路径
@Param 要创建多少个Mipmap,如果指定为0则创建所有Mipmap
*/
bool F3DTexture2D::LoadTexture(const char* filename, uint32 numMipmaps/* =0 */)
{
    // Load the image.
    F3DImage* image = F3DImageLoader::GetInstance().Load(filename);
    if( !image ) return false;

    // Load texture from image.
    bool ret = LoadTextureFromImage(image, numMipmaps);

    // Destroy the image.
    delete image;

    return ret;
}

/** 从Image对象中加载2D纹理
@Param 图像对象的指针
@Param 要创建多少个Mipmap,如果指定为0则创建所有Mipmap
*/
bool F3DTexture2D::LoadTextureFromImage( F3DImage* image, uint32 numMipmaps/* =0 */ )
{
    if( !image )
    {
        FLOG_WARNING( "F3DTexture2D::Load2DTextureFromImage(), Invalid image pointer!" );
		return false;
    }

    EPixelFormat pixFmt;
    if( image->GetPixelFormat() == PFT_R8G8B8 )
        pixFmt = PFT_X8R8G8B8;
    else
        pixFmt = image->GetPixelFormat();

    // 创建图像对象
    if( !CreateTexture(image->GetWidth(), image->GetHeight(), TU_DEFAULT, pixFmt, numMipmaps) )
    {
        FLOG_WARNING( "F3DTexture2D::Load2DTextureFromImage(), Create the texture object failed!" );
        return false;
    }

    int pitch;
    fbyte* pixelBuf;

    // 构建第0层纹理数据
    if( !Lock(0, TL_WRITEONLY, (void**)&pixelBuf, &pitch) )
    {
        FLOG_WARNING( "F3DTexture2D::Load2DTextureFromImage(), Lock the texture data failed!" );
        return false;
    }

    if( image->GetPixelFormat() == PFT_R8G8B8 )
    {
        size_t srcLineSize = image->GetWidth() * 3;

        for( size_t j=0;j<image->GetHeight();j++ )
        {
            fbyte* src = image->GetImageData() + srcLineSize * j;
            fbyte* dst = &pixelBuf[j*pitch];

            for( size_t i=0;i<image->GetWidth();i++ )
            {
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = 0xff;
            }
        }
    }
    else
    {
        // 获取每行像素的大小
        size_t srcLineSize = F3D_PixelSize(pixFmt) * image->GetWidth();

        for( size_t j=0;j<image->GetHeight();j++ )
        {
            fbyte* src = image->GetImageData() + srcLineSize * j;
            memcpy( &pixelBuf[j*pitch],src,srcLineSize );
        }
    }

    Unlock(0);

    // 创建Mipmap层
    return GenerateMipmaps( numMipmaps );
}

/** 从内存中加载2D纹理
@Param 指向纹理数据的指针
@Param 纹理的宽度
@Param 纹理的高度
@Param 纹理的像素格式
@Param 要创建多少个Mipmap,如果指定为0则创建所有Mipmap
*/
bool F3DTexture2D::LoadTextureFromMemory(fbyte* data, uint32 width, uint32 height, EPixelFormat format,
                                         uint32 numMipmaps/* =0 */)
{
    if( !data || width == 0 || height == 0 || format == PFT_UNKNOWN )
    {
        FLOG_WARNING( "F3DTexture2D::Load2DTextureFromMemory(), Invalid paramters" );
		return false;
    }

    EPixelFormat pixFmt = (format == PFT_R8G8B8) ? PFT_X8R8G8B8 : format;

    // 创建图像对象
    if( !CreateTexture(width, height, TU_DEFAULT, pixFmt, numMipmaps) )
    {
        FLOG_ERROR( "F3DTexture::Load2DTextureFromMemory(), Create the texture object failed!" );
        return false;
    }

    int pitch;
    fbyte* pixelBuf;

    // 构建第0层纹理数据
    if( !Lock(0, TL_WRITEONLY, (void**)&pixelBuf, &pitch) )
    {
        FLOG_ERROR( "F3DTexture::Load2DTextureFromMemory(), Lock the texture data failed!" );
        return false;
    }

    if( format == PFT_R8G8B8 )
    {
        uint32 srcLineSize = width * 3;

        for( uint32 j=0;j<height;j++ )
        {
            fbyte* src = data + srcLineSize * j;
            fbyte* dst = &pixelBuf[j*pitch];

            for( uint32 i=0;i<width;i++ )
            {
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = 0xff;
            }
        }
    }
    else
    {
        // 获取每行像素的大小
        size_t srcLineSize = F3D_PixelSize(pixFmt) * width;

        for( uint32 j=0;j<height;j++ )
        {
            fbyte* src = data + srcLineSize * j;
            memcpy( &pixelBuf[j*pitch], src, srcLineSize );
        }
    }

    Unlock(0);

    // 创建Mipmap层
    return GenerateMipmaps( numMipmaps );
}

/** 保存2D纹理到指定的文件
@Param 指定的图像文件路径
*/
void F3DTexture2D::SaveTexture( const char* filename )
{
	// 创建一个图像对象
	F3DImage* image = new F3DImage( m_nWidth, m_nHeight, m_Format );

	int pitch;
	fbyte* pixelBuf;
	size_t lineSize = F3D_PixelSize(m_Format) * m_nWidth;

	if( !Lock(0, TL_READONLY, (void**)&pixelBuf, &pitch) )
	{
		delete image;
		FLOG_WARNING( "F3DTexture::Save2DTexture(), Lock the texture data failed!" );
		return;
	}

	for( uint32 y=0;y<m_nHeight;y++ )
	{
		fbyte* dest = image->GetImageData() + m_nHeight * lineSize;
		memcpy( dest, pixelBuf+m_nHeight*pitch, lineSize );
	}

	Unlock(0);

	// 保存图像文件
	F3DImageLoader::GetInstance().Save( filename, image );

	// 销毁图像对象
	delete image;
}

///////////////////////////////////////////////////////////////////////////
//// Implement class F3DTexture3D

/** Constructor.
*/
F3DTexture3D::F3DTexture3D(const AString& name)
	: F3DTextureBase(name)
{
}

/** Destructor.
*/
F3DTexture3D::~F3DTexture3D()
{
}

/** 从内存中加载3D纹理
@Param 指向纹理数据的指针
@Param 纹理的宽度
@Param 纹理的高度
@Param 纹理的深度
@Param 纹理的像素格式
@Param 要创建多少个Mipmap,如果指定为0则创建所有Mipmap
*/
bool F3DTexture3D::LoadTextureFromMemory(fbyte* data, uint32 width, uint32 height, uint32 depth,
                                         EPixelFormat format, uint32 numMipmaps/* =0 */ )
{
    if( !data || width == 0 || height == 0 || depth == 0 ||
        format == PFT_UNKNOWN )
    {
        FLOG_WARNING( "F3DTexture::Load3DTextureFromMemory(), Invalid paramters!" );
		return false;
    }

    EPixelFormat pixFmt = (format == PFT_R8G8B8) ? PFT_X8R8G8B8 : format;

    // 创建图像对象
    if( !CreateTexture(width, height, depth, TU_DEFAULT, format, numMipmaps) )
    {
        FLOG_ERROR( "F3DTexture::Load3DTextureFromMemory(), Create the texture object failed!" );
        return false;
    }

    int rowPitch,slicePitch;
    fbyte* pixelBuf;

    // 构建第0层纹理数据
    if( !Lock(0, TL_WRITEONLY, (void**)&pixelBuf, &rowPitch, &slicePitch) )
    {
        FLOG_ERROR( "F3DTexture::Load3DTextureFromMemory(), Lock the texture data failed!" );
        return false;
    }

    if( format == PFT_R8G8B8 )
    {
        uint32 srcSliceSize = width * height * 3;
        uint32 srcRowSize = width * 3;

        for( uint32 j=0;j<depth;j++ )
        {
            for( uint32 i=0;i<height;i++ )
            {
                fbyte* src = data + srcSliceSize*j + srcRowSize*i;
                fbyte* dst = &pixelBuf[slicePitch*j+rowPitch*i];

                for( uint32 k=0;k<width;k++ )
                {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    *dst++ = *src++;
                    *dst++ = 0xff;
                }
            }
        }
    }
    else
    {
        size_t sliceSize = F3D_PixelSize(pixFmt) * width * height;
        size_t rowSize = F3D_PixelSize(pixFmt) * width;

        for( uint32 j=0;j<depth;j++ )
        {
            for( uint32 i=0;i<height;i++ )
            {
                fbyte* src = data + sliceSize*j + rowSize*i;

                memcpy( &pixelBuf[slicePitch*j+rowPitch*i], src, rowSize );
            }
        }
    }

    Unlock(0);

    // 创建Mipmap层
    return GenerateMipmaps( numMipmaps );
}

///////////////////////////////////////////////////////////////////////////
//// Implement class F3DTextureCube

/** Constructor.
*/
F3DTextureCube::F3DTextureCube(const AString& name)
	: F3DTextureBase(name)
{
}

/** Destructor.
*/
F3DTextureCube::~F3DTextureCube()
{
}

/** 从六个图像文件中加载Cube纹理
@Param 指向6个纹理文件的路径
@Param 要创建多少个Mipmap,如果指定为0则创建所有Mipmap
*/
bool F3DTextureCube::LoadTexture( const AStringList& imageList,uint32 numMipmaps/* =0 */ )
{
    if( imageList.size() != 6 )
    {
        FLOG_WARNING( "F3DTexture::LoadCubeTexture(), Six image files needed!" );
		return false;
    }

    F3DImage* images[6];

    // 加载六个图像
    images[0] = F3DImageLoader::GetInstance().Load( imageList[0].c_str() );
    images[1] = F3DImageLoader::GetInstance().Load( imageList[1].c_str() );
    images[2] = F3DImageLoader::GetInstance().Load( imageList[2].c_str() );
    images[3] = F3DImageLoader::GetInstance().Load( imageList[3].c_str() );
    images[4] = F3DImageLoader::GetInstance().Load( imageList[4].c_str() );
    images[5] = F3DImageLoader::GetInstance().Load( imageList[5].c_str() );

    // 通过图像对象加载纹理
    bool ret = LoadTextureFromImage( images, numMipmaps );

    // 销毁六个图像
    F_SAFE_DELETE( images[0] );
    F_SAFE_DELETE( images[0] );
    F_SAFE_DELETE( images[0] );
    F_SAFE_DELETE( images[0] );
    F_SAFE_DELETE( images[0] );
    F_SAFE_DELETE( images[0] );
    return ret;
}

/** 从六个图像文件中加载Cube纹理
@Param 指向6个纹理文件的路径
@Param 要创建多少个Mipmap,如果指定为0则创建所有Mipmap
*/
bool F3DTextureCube::LoadTexture( const char* imageList[6], uint32 numMipmaps/* =0 */ )
{
    for( int i=0;i<6;i++ )
    {
        if( !imageList[i] )
        {
            FLOG_WARNING( "F3DTexture::LoadCubeTexture(), Invalid parameters! (null filename)" );
            return false;
        }
    }

    F3DImage* images[6];

    // 加载6个图像
    images[0] = F3DImageLoader::GetInstance().Load( imageList[0] );
    images[1] = F3DImageLoader::GetInstance().Load( imageList[1] );
    images[2] = F3DImageLoader::GetInstance().Load( imageList[2] );
    images[3] = F3DImageLoader::GetInstance().Load( imageList[3] );
    images[4] = F3DImageLoader::GetInstance().Load( imageList[4] );
    images[5] = F3DImageLoader::GetInstance().Load( imageList[5] );

    bool ret = LoadTextureFromImage( images, numMipmaps );

    F_SAFE_DELETE( images[0] );
    F_SAFE_DELETE( images[1] );
    F_SAFE_DELETE( images[2] );
    F_SAFE_DELETE( images[3] );
    F_SAFE_DELETE( images[4] );
    F_SAFE_DELETE( images[5] );
    return ret;
}

/** 从六个图像中加载Cube纹理
@Param 指向纹理对象的数组
@Param 要创建多少个Mipmap,如果指定为0则创建所有Mipmap
*/
bool F3DTextureCube::LoadTextureFromImage( F3DImage* image[6], uint32 numMipmaps/* =0 */ )
{
    if( !image[0] || !image[1] || !image[2] ||
        !image[3] || !image[4] || !image[5] )
    {
        FLOG_WARNING( "F3DTextureCube::LoadCubeTextureFromImage(), Invalid parameters! (NULL image)" );
		return false;
    }

    if( image[0]->GetWidth() != image[0]->GetHeight() ||
        image[1]->GetWidth() != image[1]->GetHeight() ||
        image[2]->GetWidth() != image[2]->GetHeight() ||
        image[3]->GetWidth() != image[3]->GetHeight() ||
        image[4]->GetWidth() != image[4]->GetHeight() ||
        image[5]->GetWidth() != image[5]->GetHeight() )
    {
        FLOG_WARNING("F3DTextureCube::LoadCubeTextureFromImage, The images widths and heights must be the same!");
		return false;
    }

    if( image[1]->GetWidth() != image[0]->GetWidth() ||
        image[2]->GetWidth() != image[0]->GetWidth() ||
        image[3]->GetWidth() != image[0]->GetWidth() ||
        image[4]->GetWidth() != image[0]->GetWidth() ||
        image[5]->GetWidth() != image[0]->GetWidth() )
    {
		FLOG_WARNING("F3DTextureCube::LoadCubeTextureFromImage, The images widths and heights must be the same!");
		return false;
    }

    if( image[1]->GetPixelFormat() != image[0]->GetPixelFormat() ||
        image[2]->GetPixelFormat() != image[0]->GetPixelFormat() ||
        image[3]->GetPixelFormat() != image[0]->GetPixelFormat() ||
        image[4]->GetPixelFormat() != image[0]->GetPixelFormat() ||
        image[5]->GetPixelFormat() != image[0]->GetPixelFormat() )
    {
		FLOG_WARNING("F3DTextureCube::LoadCubeTextureFromImage, The images pixel formats must be the same!");
		return false;
    }

    uint32 edgeLength = image[0]->GetWidth();
    EPixelFormat format = image[0]->GetPixelFormat();

    // 创建图像对象
    if( !CreateTexture(edgeLength, TU_DEFAULT, format, numMipmaps) )
    {
        FLOG_WARNING( "F3DTextureCube::LoadTextureFromImage(), Create the texture object failed!" );
        return false;
    }

    for( uint32 face=0; face<6; face++ )
    {
        int pitch;
        fbyte* pixelBuf;
        fbyte* data = image[face]->GetImageData();

        // 构建第0层纹理数据
        if( !Lock((ECubeFace)face, 0, TL_WRITEONLY, (void**)&pixelBuf, &pitch) )
        {
            FLOG_WARNINGF( "F3DTextureCube::LoadCubeTextureFromImage(), Lock the cube face (%d) data failed!",face );
            return false;
        }

        size_t lineSize = F3D_PixelSize(format) * edgeLength;

        for( uint32 j=0;j<edgeLength;j++ )
        {
            fbyte* src = data + lineSize * j;
            memcpy( &pixelBuf[j*pitch],src,lineSize );
        }

        Unlock((ECubeFace)face, 0);
    }

    // 创建Mipmap链
    return GenerateMipmaps( numMipmaps );
}

/** 保存Cube纹理指定的面到文件
@Param 要保存那个面
@Param 指定的图像文件路径
*/
void F3DTextureCube::SaveTexture( ECubeFace face, const char* filename )
{
    // 创建一个图像对象
    F3DImage* image = new F3DImage( m_nWidth, m_nHeight, m_Format );

    int pitch;
    fbyte* pixelBuf;
    size_t lineSize = F3D_PixelSize(m_Format) * m_nWidth;

    if( !Lock(face, 0, TL_READONLY, (void**)&pixelBuf, &pitch) )
    {
        delete image;
        FLOG_WARNING( "F3DTexture::SaveCubeTexture(), Lock the texture data failed!" );
        return;
    }

    for( uint32 y=0;y<m_nHeight;y++ )
    {
        fbyte* dest = image->GetImageData() + m_nHeight * lineSize;
        memcpy( dest, pixelBuf+m_nHeight*pitch, lineSize );
    }

    Unlock(face, 0);

    // Save the data to file.
    F3DImageLoader::GetInstance().Save( filename, image );

    // Destroy the image.
    delete image;
}