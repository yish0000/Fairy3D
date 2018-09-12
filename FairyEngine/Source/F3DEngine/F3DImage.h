/*
 * ------------------------------------------------------------------------
 *  Name:   F3DImage.h
 *  Desc:   本文件为引擎定义了一个图像资源类。
 *  Author: Yish
 *  Date:   2010/11/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_IMAGE_H__
#define __F3D_IMAGE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 2D Image resource.
*/
class FAIRY_API F3DImage : public FGeneralAlloc
{
    struct SMipmap
    {
        size_t bufferSize;  // Mipmap数据缓冲区大小
        fbyte* data;        // Mipmap像素数据缓冲区
    };

    typedef std::vector<SMipmap> MipmapList;

protected:
    uint32          m_nWidth;       // 图像的宽度(以像素为单位)
    uint32          m_nHeight;      // 图像的高度(以像素为单位)
    EPixelFormat    m_Format;       // 该图像的像素格式
    size_t          m_nBufferSize;  // 数据缓冲区的大小
    fbyte*          m_pBuffer;      // 像素数据缓冲区的指针

    MipmapList      m_Mipmaps;      // 手动生成的Mipmap链

public:

    // Constructor.
    F3DImage(void);
    F3DImage( uint32 width, uint32 height, EPixelFormat format );
    F3DImage( const F3DImage& image );

    // Destructor.
    virtual ~F3DImage(void);

    // Operator =
    F3DImage& operator = ( const F3DImage& image );

	// Reset the content of image.
    void ResetContent(void);

	// Create a image with the specified color.
    bool CreateImage( uint32 width, uint32 height, EPixelFormat format, const F3DColor& color = F3DColor::Black );
	// Create a image with the specified data.
    bool CreateImageFromMemory( fbyte* pData, EPixelFormat format, uint32 width, uint32 height );

	// Flip the image around y axis.
    void FlipAroundY(void);
    // Flip the image around x axis.
    void FlipAroundX(void);

    // Scale the image. (All the mipmaps will lost !!!)
    void ScaleImage( float fScaleX, float fScaleY );
	// Resize the image. (All the mipmaps will lost !!!)
    void Resize( uint32 width, uint32 height );

	// Invert the color.
    void InvertColor(void);

	// Convert the pixel format to another.
    void ConvertFormat( EPixelFormat format );

	// Generate the mipmap data for sepecified level.
    void GenerateMipmaps( uint32 level );

    // Get the size of specified mipmap.
    uint32 GetWidth( uint32 level = 0 ) const;
    uint32 GetHeight( uint32 level = 0 ) const;

    // Get the image data of specified mipmap.
    fbyte* GetImageData( uint32 level = 0 );
    const fbyte* GetImageData( uint32 level = 0 ) const;

	// Get the buffer size of specified mipmap.
    size_t GetBufferSize( uint32 level = 0 ) const;

	// Get the count of mipmaps.
    uint32 GetNumMipmaps(void) const;

    // Get the pixel format of me.
    EPixelFormat GetPixelFormat(void) const { return m_Format; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_IMAGE_H__
