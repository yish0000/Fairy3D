/*
 * ------------------------------------------------------------------------
 *  Name:   F3DImage.cpp
 *  Desc:   本文件为引擎实现了一个图像对象类。
 *  Author: Yish
 *  Date:   2010/11/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DImage.h"
#include "F3DPixelUtil.h"


/** 缺省的构造函数
*/
F3DImage::F3DImage(void) : m_nWidth(0),m_nHeight(0),m_Format(PFT_UNKNOWN),
    m_nBufferSize(0),m_pBuffer(NULL)
{
}

/** 构造时分配内存
@Param 图像的宽度
@Param 图像的高度
@Param 图像的像素格式
*/
F3DImage::F3DImage( uint32 width, uint32 height, EPixelFormat format )
    : m_nWidth(width),m_nHeight(height),m_Format(format)
{
    m_nBufferSize = F3D_PixelSize(format) * width * height;
    m_pBuffer = new fbyte[m_nBufferSize];
}

/** 赋值构造函数
@Param 另一个图像对象的引用
*/
F3DImage::F3DImage( const F3DImage& image ) : m_nWidth(image.m_nWidth),
    m_nHeight(image.m_nHeight),m_Format(image.m_Format),
    m_nBufferSize(image.m_nBufferSize)
{
    m_pBuffer = new fbyte[m_nBufferSize];
    memcpy( m_pBuffer,image.m_pBuffer,m_nBufferSize );

    m_Mipmaps.resize( image.m_Mipmaps.size() );
    for( size_t i=0;i<m_Mipmaps.size();i++ )
    {
        SMipmap* mipmap = &m_Mipmaps[i];

        mipmap->bufferSize = image.m_Mipmaps[i].bufferSize;
        mipmap->data = new fbyte[mipmap->bufferSize];
        memcpy( mipmap->data,image.m_Mipmaps[i].data,mipmap->bufferSize );
    }
}

/** 析构函数
*/
F3DImage::~F3DImage(void)
{
    F_SAFE_DELETE_ARRAY( m_pBuffer );

    // 清理Mipmap链
    for( size_t i=0;m_Mipmaps.size();i++ )
    {
        F_SAFE_DELETE_ARRAY( m_Mipmaps[i].data );
    }
}

/** 重载等号运算符
*/
F3DImage& F3DImage::operator = ( const F3DImage& image )
{
    // 清理旧的数据
    ResetContent();

    m_nWidth = image.m_nWidth;
    m_nHeight = image.m_nHeight;
    m_Format = image.m_Format;

    m_nBufferSize = image.m_nBufferSize;
    m_pBuffer = new fbyte[m_nBufferSize];
    memcpy( m_pBuffer,image.m_pBuffer,m_nBufferSize );

    // 拷贝Mipmap链
    m_Mipmaps.resize( image.m_Mipmaps.size() );
    for( size_t i=0;i<m_Mipmaps.size();i++ )
    {
        SMipmap* mipmap = &m_Mipmaps[i];

        mipmap->bufferSize = image.m_Mipmaps[i].bufferSize;
        mipmap->data = new fbyte[mipmap->bufferSize];
        memcpy( mipmap->data,image.m_Mipmaps[i].data,mipmap->bufferSize );
    }

    return *this;
}

/** 重置该图像对象的所有资源
*/
void F3DImage::ResetContent(void)
{
    F_SAFE_DELETE_ARRAY( m_pBuffer );

    // 清理Mipmap链
    for( size_t i=0;i<m_Mipmaps.size();i++ )
    {
        F_SAFE_DELETE_ARRAY( m_Mipmaps[i].data );
    }

    m_nBufferSize = 0;
    m_nWidth = m_nHeight = 0;
    m_Format = PFT_UNKNOWN;
    m_Mipmaps.clear();
}

/** 创建一个指定颜色空的图像
@Param 图像的宽度
@Param 图像的高度
@Param 图像的像素格式
@Param 初始颜色值
*/
bool F3DImage::CreateImage( uint32 width, uint32 height, EPixelFormat format,
						    const F3DColor& color/* = F3DColor::Black*/ )
{
    // 清理旧的数据
    ResetContent();

    m_nWidth = width;
    m_nHeight = height;
    m_Format = format;

    // 创建像素数据缓冲区
    m_nBufferSize = F3D_PixelSize(format) * width * height;
    m_pBuffer = new fbyte[m_nBufferSize];

	uint32* pSrcData = new uint32[width * height];
	for( size_t i=0; i<width*height; i++ )
	{
		*pSrcData++ = color.GetARGB();
	}

    // 填充颜色
    F3D_ConvertPixelFormat((fbyte*)pSrcData, PFT_A8R8G8B8, m_pBuffer, format, width, height);
	delete[] pSrcData;
    return true;
}

/** 使用指定内存中的数据创建图像
@Param 数据缓冲区的指针
@Param 要创建的像素格式
@Param 要创建图像的宽度
@Param 要创建图像的高度
*/
bool F3DImage::CreateImageFromMemory( fbyte* pData, EPixelFormat format, uint32 width,
                                      uint32 height )
{
    size_t pixSize = F3D_PixelSize( format );
    if( !pData || !pixSize || !width || !height )
    {
        FLOG_WARNING( "F3DImage::CreateImageFromMemory(), Invalid parameters!" );
        return false;
    }

    // 清除旧的图像数据
    ResetContent();

    m_nWidth = width;
    m_nHeight = height;
    m_Format = format;

    m_nBufferSize = F3D_PixelSize(format)*width*height;
    m_pBuffer = new fbyte[m_nBufferSize];

    memcpy( m_pBuffer,pData,m_nBufferSize );
    return true;
}

/** 围绕X轴做一次镜像
*/
void F3DImage::FlipAroundX(void)
{
    fbyte* dest1;
    ushort* dest2;
    fbyte* dest3;
    ulong* dest4;

    fbyte* src1 = (fbyte*)m_pBuffer;
    ushort* src2 = (ushort*)m_pBuffer;
    fbyte* src3 = (fbyte*)m_pBuffer;
    ulong* src4 = (ulong*)m_pBuffer;

    // 创建一个临时缓冲区
    fbyte* pTempBuffer = new fbyte[m_nBufferSize];

    size_t y;
    switch( F3D_PixelSize(m_Format) )
    {
    case 1:
        for( y=0;y<m_nHeight;y++ )
        {
            dest1 = (fbyte*)(pTempBuffer + m_nWidth * y + (m_nWidth - 1));
            for( size_t x=0;x<m_nWidth;x++ )
                *dest1-- = *src1++;
        }

        memcpy( m_pBuffer,pTempBuffer,m_nBufferSize );
        break;
    case 2:
        for( y=0;y<m_nHeight;y++ )
        {
            dest2 = (ushort*)((ushort*)pTempBuffer + m_nWidth * y + (m_nWidth - 1));
            for( size_t x=0;x<m_nWidth;x++ )
                *dest2-- = *src2++;
        }

        memcpy( m_pBuffer,pTempBuffer,m_nBufferSize );
        break;
    case 3:
        for( y=0;y<m_nHeight;y++ )
        {
            dest3 = (fbyte*)(pTempBuffer + (m_nWidth * y + (m_nWidth - 1)) * 3);
            for( size_t x=0;x<m_nWidth;x++ )
            {
                dest3[0] = src3[0];
                dest3[1] = src3[1];
                dest3[2] = src3[2];
                dest3 -= 3; src3 += 3;
            }
        }

        memcpy( m_pBuffer,pTempBuffer,m_nBufferSize );
        break;
    case 4:
        for( y=0;y<m_nHeight;y++ )
        {
            dest4 = (ulong*)((ulong*)pTempBuffer + m_nWidth * y + (m_nWidth - 1));
            for( size_t x=0;x<m_nWidth;x++ )
                *dest4-- = *src4++;
        }

        memcpy( m_pBuffer,pTempBuffer,m_nBufferSize );
        break;
    default:
        delete[] pTempBuffer;
        FLOG_ERROR("F3DImage::FlipAroundX, Unknown pixel format!");
        return;
    }

    delete[] pTempBuffer;
}

/** 围绕Y轴做一次镜像
*/
void F3DImage::FlipAroundY(void)
{
    // 每行像素的大小
    size_t rowSpan = F3D_PixelSize(m_Format) * m_nWidth;

    // 创建一个临时缓冲区
    fbyte* pTempBuffer = new fbyte[m_nBufferSize];

    fbyte* ptr1 = m_pBuffer;
    fbyte* ptr2 = m_pBuffer + (m_nHeight - 1) * rowSpan;
    for( uint32 i=0;i<m_nHeight;i++ )
    {
        memcpy( ptr2,ptr1,rowSpan );
        ptr1 += rowSpan; ptr2 -= rowSpan;
    }

    delete[] m_pBuffer;
    m_pBuffer = pTempBuffer;
}

/** 缩放当前图像
@Param X方向的放缩倍数
@Param Y方向的放缩倍数
*/
void F3DImage::ScaleImage( float fScaleX, float fScaleY )
{
    if( fScaleX <= 0.0f || fScaleY <= 0.0f )
    {
        FLOG_WARNING( "F3DImage::ScaleImage(), Invalid scale for image!" );
		return;
    }

    uint32 w = m_nWidth * fScaleX;
    uint32 h = m_nHeight * fScaleY;

    Resize(w, h);
}

/** 重新指定图像的大小
@Param 图像的宽度
@Param 图像的高度
*/
void F3DImage::Resize( uint32 width, uint32 height )
{
    if( width <= 0 || height <= 0 )
    {
        FLOG_WARNING( "F3DImage::Resize(), Invalid size for image!" );
        return;
    }

    if( width == m_nWidth && height == m_nHeight )
        return;

    size_t pxSize = F3D_PixelSize( m_Format );

    // 为新的图像分配内存
    fbyte* tempBuf = new fbyte[pxSize*width*height];

    // 缩小图像
    if( width < m_nWidth && height < m_nHeight )
    {
        F3D_GenerateMipmap( m_pBuffer,m_nWidth,m_nHeight,m_Format,tempBuf,width,height );
    }

    // 放大图像
    else if( width > m_nWidth && height > m_nHeight )
    {
        F3D_StretchLinear( m_pBuffer,m_nWidth,m_nHeight,m_Format,tempBuf,width,height );
    }

    // 两个方向一个放大一个缩小
    else
    {
        fbyte* tempBuf2;

        if( width < m_nWidth )
        {
            tempBuf2 = new fbyte[pxSize*width*m_nHeight];
            F3D_GenerateMipmap( m_pBuffer,m_nWidth,m_nHeight,m_Format,tempBuf2,width,m_nHeight );
            F3D_StretchLinear( tempBuf2,width,m_nHeight,m_Format,tempBuf,width,height );
            delete[] tempBuf2;
        }
        else
        {
            tempBuf2 = new fbyte[pxSize*m_nWidth*height];
            F3D_GenerateMipmap( m_pBuffer,m_nWidth,m_nHeight,m_Format,tempBuf2,m_nWidth,height );
            F3D_StretchLinear( tempBuf2,m_nWidth,height,m_Format,tempBuf,width,height );
            delete[] tempBuf2;
        }
    }

    delete[] m_pBuffer;
    m_pBuffer = tempBuf;
    m_nBufferSize = pxSize * width * height;
    m_nWidth = width;
    m_nHeight = height;

    // 销毁旧的Mipmap链
    MipmapList::iterator it = m_Mipmaps.begin();
    while( it != m_Mipmaps.end() )
    {
        F_SAFE_DELETE_ARRAY( it->data );
        it++;
    }

    m_Mipmaps.clear();
}

/** 反转图像的颜色
*/
void F3DImage::InvertColor()
{
    for( size_t i=0;i<m_nBufferSize;i++ )
    {
        m_pBuffer[i] = ~m_pBuffer[i];
    }

    MipmapList::iterator it = m_Mipmaps.begin();
    while( it != m_Mipmaps.end() )
    {
        for( size_t m=0;m<it->bufferSize;m++ )
        {
            it->data[m] = ~it->data[m];
        }

        it++;
    }
}

/** 将当前像素格式转换成另外一个像素格式
@Param 指定的像素格式
*/
void F3DImage::ConvertFormat( EPixelFormat format )
{
	if( format == m_Format )
		return;

    size_t pixelSize = F3D_PixelSize( format );

    // 创建一个缓冲区
    fbyte* tempBuf = new fbyte[pixelSize*m_nWidth*m_nHeight];

    // 改变基本图像层的数据
    F3D_ConvertPixelFormat( m_pBuffer,m_Format,tempBuf,format,m_nWidth,m_nHeight );

    delete[] m_pBuffer;
    m_pBuffer = tempBuf;
    m_nBufferSize = pixelSize * m_nWidth * m_nHeight;

    // 更改Mipmap链的像素格式
    size_t w = m_nWidth >> 1;
    size_t h = m_nHeight >> 1;
    MipmapList::iterator it = m_Mipmaps.begin();
    while( it != m_Mipmaps.end() )
    {
        fbyte* mipBuf = new fbyte[pixelSize*w*h];
        F3D_ConvertPixelFormat( it->data,m_Format,mipBuf,format,w,h );
        delete[] it->data;
        it->data = mipBuf;
        it->bufferSize = pixelSize * w * h;

        w = w >> 1;
        h = h >> 1;
        if( w == 0 || h == 0 ) break;

		it++;
    }

    m_Format = format;
}

/** 为图像手动生成Mipmap链
@Param 生成几个Mipmap
*/
void F3DImage::GenerateMipmaps( uint32 level )
{
    // 舍弃旧的Mipmap链
    MipmapList::iterator it = m_Mipmaps.begin();
    while( it != m_Mipmaps.end() )
    {
        F_SAFE_DELETE_ARRAY( it->data );
        it++;
    }

    m_Mipmaps.clear();

    size_t w = m_nWidth >> 1;
    size_t h = m_nHeight >> 1;

    // 重新生成新的Mipmap链
    size_t i = 0;
    while( i < level )
    {
        SMipmap mipmap;
        mipmap.bufferSize = F3D_PixelSize(m_Format)*w*h;
        mipmap.data = new fbyte[mipmap.bufferSize];
        F3D_GenerateMipmap( m_pBuffer,m_nWidth,m_nHeight,m_Format,mipmap.data,w,h );
        m_Mipmaps.push_back( mipmap );

        if( w == 1 && h == 1 )
            break;
        else
        {
            w = w >> 1;
            h = h >> 1;
            if( w < 1 ) w = 1;
            if( h < 1 ) h = 1;
            i++;
        }
    }
}

/** 获取指定层的宽度
@Param 指定的Mipmap层级
*/
uint32 F3DImage::GetWidth( uint32 level ) const
{
    if( level == 0 )
        return m_nWidth;
    else
    {
        if( level - 1 < m_Mipmaps.size() )
            return m_nWidth >> level;
        else
            return 0;
    }
}

/** 获取指定层的高度
@Param 指定的Mipmap层级
*/
uint32 F3DImage::GetHeight( uint32 level ) const
{
    if( level == 0 )
        return m_nHeight;
    else
    {
        if( level - 1 < m_Mipmaps.size() )
            return m_nHeight >> level;
        else
            return 0;
    }
}

/** 获取指定层的图像数据
@Param 指定的Mipmap层级
*/
fbyte* F3DImage::GetImageData( uint32 level )
{
    if( level == 0 )
        return m_pBuffer;
    else
    {
        if( level - 1 < m_Mipmaps.size() )
            return m_Mipmaps[level-1].data;
        else
            return NULL;
    }
}

/** 获取指定层的图像数据
@Param 指定的Mipmap层级
*/
const fbyte* F3DImage::GetImageData( uint32 level ) const
{
    if( level == 0 )
        return m_pBuffer;
    else
    {
        if( level - 1 < m_Mipmaps.size() )
            return m_Mipmaps[level-1].data;
        else
            return NULL;
    }
}

/** 获取指定图层缓冲区的大小
@Param 指定的Mipmap层级
*/
size_t F3DImage::GetBufferSize( uint32 level ) const
{
    if( level == 0 )
        return m_nBufferSize;
    else
    {
        if( level - 1 < m_Mipmaps.size() )
            return m_Mipmaps[level-1].bufferSize;
        else
            return 0;
    }
}

/** 获取该图像的Mipmap个数(其中包括图像本身的那层)
*/
uint32 F3DImage::GetNumMipmaps(void) const
{
    return m_pBuffer ? (uint32)m_Mipmaps.size() + 1 : 0;
}