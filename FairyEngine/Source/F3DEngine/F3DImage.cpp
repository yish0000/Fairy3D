/*
 * ------------------------------------------------------------------------
 *  Name:   F3DImage.cpp
 *  Desc:   ���ļ�Ϊ����ʵ����һ��ͼ������ࡣ
 *  Author: Yish
 *  Date:   2010/11/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DImage.h"
#include "F3DPixelUtil.h"


/** ȱʡ�Ĺ��캯��
*/
F3DImage::F3DImage(void) : m_nWidth(0),m_nHeight(0),m_Format(PFT_UNKNOWN),
    m_nBufferSize(0),m_pBuffer(NULL)
{
}

/** ����ʱ�����ڴ�
@Param ͼ��Ŀ��
@Param ͼ��ĸ߶�
@Param ͼ������ظ�ʽ
*/
F3DImage::F3DImage( uint32 width, uint32 height, EPixelFormat format )
    : m_nWidth(width),m_nHeight(height),m_Format(format)
{
    m_nBufferSize = F3D_PixelSize(format) * width * height;
    m_pBuffer = new fbyte[m_nBufferSize];
}

/** ��ֵ���캯��
@Param ��һ��ͼ����������
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

/** ��������
*/
F3DImage::~F3DImage(void)
{
    F_SAFE_DELETE_ARRAY( m_pBuffer );

    // ����Mipmap��
    for( size_t i=0;m_Mipmaps.size();i++ )
    {
        F_SAFE_DELETE_ARRAY( m_Mipmaps[i].data );
    }
}

/** ���صȺ������
*/
F3DImage& F3DImage::operator = ( const F3DImage& image )
{
    // ����ɵ�����
    ResetContent();

    m_nWidth = image.m_nWidth;
    m_nHeight = image.m_nHeight;
    m_Format = image.m_Format;

    m_nBufferSize = image.m_nBufferSize;
    m_pBuffer = new fbyte[m_nBufferSize];
    memcpy( m_pBuffer,image.m_pBuffer,m_nBufferSize );

    // ����Mipmap��
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

/** ���ø�ͼ������������Դ
*/
void F3DImage::ResetContent(void)
{
    F_SAFE_DELETE_ARRAY( m_pBuffer );

    // ����Mipmap��
    for( size_t i=0;i<m_Mipmaps.size();i++ )
    {
        F_SAFE_DELETE_ARRAY( m_Mipmaps[i].data );
    }

    m_nBufferSize = 0;
    m_nWidth = m_nHeight = 0;
    m_Format = PFT_UNKNOWN;
    m_Mipmaps.clear();
}

/** ����һ��ָ����ɫ�յ�ͼ��
@Param ͼ��Ŀ��
@Param ͼ��ĸ߶�
@Param ͼ������ظ�ʽ
@Param ��ʼ��ɫֵ
*/
bool F3DImage::CreateImage( uint32 width, uint32 height, EPixelFormat format,
						    const F3DColor& color/* = F3DColor::Black*/ )
{
    // ����ɵ�����
    ResetContent();

    m_nWidth = width;
    m_nHeight = height;
    m_Format = format;

    // �����������ݻ�����
    m_nBufferSize = F3D_PixelSize(format) * width * height;
    m_pBuffer = new fbyte[m_nBufferSize];

	uint32* pSrcData = new uint32[width * height];
	for( size_t i=0; i<width*height; i++ )
	{
		*pSrcData++ = color.GetARGB();
	}

    // �����ɫ
    F3D_ConvertPixelFormat((fbyte*)pSrcData, PFT_A8R8G8B8, m_pBuffer, format, width, height);
	delete[] pSrcData;
    return true;
}

/** ʹ��ָ���ڴ��е����ݴ���ͼ��
@Param ���ݻ�������ָ��
@Param Ҫ���������ظ�ʽ
@Param Ҫ����ͼ��Ŀ��
@Param Ҫ����ͼ��ĸ߶�
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

    // ����ɵ�ͼ������
    ResetContent();

    m_nWidth = width;
    m_nHeight = height;
    m_Format = format;

    m_nBufferSize = F3D_PixelSize(format)*width*height;
    m_pBuffer = new fbyte[m_nBufferSize];

    memcpy( m_pBuffer,pData,m_nBufferSize );
    return true;
}

/** Χ��X����һ�ξ���
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

    // ����һ����ʱ������
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

/** Χ��Y����һ�ξ���
*/
void F3DImage::FlipAroundY(void)
{
    // ÿ�����صĴ�С
    size_t rowSpan = F3D_PixelSize(m_Format) * m_nWidth;

    // ����һ����ʱ������
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

/** ���ŵ�ǰͼ��
@Param X����ķ�������
@Param Y����ķ�������
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

/** ����ָ��ͼ��Ĵ�С
@Param ͼ��Ŀ��
@Param ͼ��ĸ߶�
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

    // Ϊ�µ�ͼ������ڴ�
    fbyte* tempBuf = new fbyte[pxSize*width*height];

    // ��Сͼ��
    if( width < m_nWidth && height < m_nHeight )
    {
        F3D_GenerateMipmap( m_pBuffer,m_nWidth,m_nHeight,m_Format,tempBuf,width,height );
    }

    // �Ŵ�ͼ��
    else if( width > m_nWidth && height > m_nHeight )
    {
        F3D_StretchLinear( m_pBuffer,m_nWidth,m_nHeight,m_Format,tempBuf,width,height );
    }

    // ��������һ���Ŵ�һ����С
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

    // ���پɵ�Mipmap��
    MipmapList::iterator it = m_Mipmaps.begin();
    while( it != m_Mipmaps.end() )
    {
        F_SAFE_DELETE_ARRAY( it->data );
        it++;
    }

    m_Mipmaps.clear();
}

/** ��תͼ�����ɫ
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

/** ����ǰ���ظ�ʽת��������һ�����ظ�ʽ
@Param ָ�������ظ�ʽ
*/
void F3DImage::ConvertFormat( EPixelFormat format )
{
	if( format == m_Format )
		return;

    size_t pixelSize = F3D_PixelSize( format );

    // ����һ��������
    fbyte* tempBuf = new fbyte[pixelSize*m_nWidth*m_nHeight];

    // �ı����ͼ��������
    F3D_ConvertPixelFormat( m_pBuffer,m_Format,tempBuf,format,m_nWidth,m_nHeight );

    delete[] m_pBuffer;
    m_pBuffer = tempBuf;
    m_nBufferSize = pixelSize * m_nWidth * m_nHeight;

    // ����Mipmap�������ظ�ʽ
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

/** Ϊͼ���ֶ�����Mipmap��
@Param ���ɼ���Mipmap
*/
void F3DImage::GenerateMipmaps( uint32 level )
{
    // �����ɵ�Mipmap��
    MipmapList::iterator it = m_Mipmaps.begin();
    while( it != m_Mipmaps.end() )
    {
        F_SAFE_DELETE_ARRAY( it->data );
        it++;
    }

    m_Mipmaps.clear();

    size_t w = m_nWidth >> 1;
    size_t h = m_nHeight >> 1;

    // ���������µ�Mipmap��
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

/** ��ȡָ����Ŀ��
@Param ָ����Mipmap�㼶
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

/** ��ȡָ����ĸ߶�
@Param ָ����Mipmap�㼶
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

/** ��ȡָ�����ͼ������
@Param ָ����Mipmap�㼶
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

/** ��ȡָ�����ͼ������
@Param ָ����Mipmap�㼶
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

/** ��ȡָ��ͼ�㻺�����Ĵ�С
@Param ָ����Mipmap�㼶
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

/** ��ȡ��ͼ���Mipmap����(���а���ͼ������ǲ�)
*/
uint32 F3DImage::GetNumMipmaps(void) const
{
    return m_pBuffer ? (uint32)m_Mipmaps.size() + 1 : 0;
}