/*
 * ------------------------------------------------------------------------
 *	Name:   F3DPixelUtil.cpp
 *  Desc:   本文件用于为引擎实现处理像素的类。
 *  Author: Yish
 *  Date:   2010/8/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DPixelUtil.h"


extern "C" {

///////////////////////////////////////////////////////////////////////////
// 模块内部使用的辅助函数

typedef void (*ARGB8888ToFormatFunc) ( FBYTE* src,FBYTE* dest,size_t numPixels );
typedef void (*FormatToARGB8888Func) ( FBYTE* src,FBYTE* dest,size_t numPixels );
typedef void (*GenMipmapFunc) ( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
typedef void (*StretchLinearFunc) ( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );

// 亮度图分量计算比率常量
static const float L_R = 0.299f;
static const float L_G = 0.587f;
static const float L_B = 0.114f;

static void ARGB8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );

// 将PFT_A8R8G8B8转换为其他像素格式
static void ARGB8888ToRGB888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToABGR8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToXRGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToXBGR8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToRGB565( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToXRGB1555( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToARGB1555( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToARGB4444( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToRGB332( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToL8( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToL16( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToAL44( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToAL88( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToA8( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToVU88( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB8888ToQWVU8888( FBYTE* src,FBYTE* dest,size_t numPixels );

// 将其他像素格式转换为PFT_A8R8G8B8
static void RGB888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ABGR8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void XRGB8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void XBGR8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void RGB565ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void XRGB1555ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB1555ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void ARGB4444ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void RGB332ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void L8ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void L16ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void AL44ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void AL88ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void A8ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void VU88ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );
static void QWVU8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels );

// 生成Mipmap的函数
static void GenMipmapRGB888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapARGB8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapABGR8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapXRGB8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapXBGR8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapRGB565( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapXRGB1555( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapARGB1555( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapARGB4444( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapRGB332( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapL8( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapL16( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapAL44( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapAL88( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapA8( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapVU88( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void GenMipmapQWVU8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );

// 线性拉伸图像的函数
static void StretchLinearRGB888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearARGB8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearABGR8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearXRGB8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearXBGR8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearRGB565( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearXRGB1555( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearARGB1555( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearARGB4444( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearRGB332( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearL8( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearL16( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearAL44( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearAL88( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearA8( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearVU88( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );
static void StretchLinearQWVU8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH );

/** 获取指定的像素转换函数
@Param 目标像素格式
*/
ARGB8888ToFormatFunc GetARGB8888ToFormatFunc( EPixelFormat destFormat )
{
    switch( destFormat )
    {
    case PFT_R8G8B8:    return ARGB8888ToRGB888;
    case PFT_A8R8G8B8:  return ARGB8888ToARGB8888;
    case PFT_A8B8G8R8:  return ARGB8888ToABGR8888;
    case PFT_X8R8G8B8:  return ARGB8888ToXRGB8888;
    case PFT_X8B8G8R8:  return ARGB8888ToXBGR8888;
    case PFT_R5G6B5:    return ARGB8888ToRGB565;
    case PFT_X1R5G5B5:  return ARGB8888ToXRGB1555;
    case PFT_A1R5G5B5:  return ARGB8888ToARGB1555;
    case PFT_A4R4G4B4:  return ARGB8888ToARGB4444;
    case PFT_R3G3B2:    return ARGB8888ToRGB332;
    case PFT_L8:        return ARGB8888ToL8;
    case PFT_L16:       return ARGB8888ToL16;
    case PFT_A4L4:      return ARGB8888ToAL44;
    case PFT_A8L8:      return ARGB8888ToAL88;
    case PFT_A8:        return ARGB8888ToA8;
    case PFT_V8U8:      return ARGB8888ToVU88;
    case PFT_Q8W8V8U8:  return ARGB8888ToQWVU8888;
    default:            return NULL;
    }
}

/** 获取指定的像素转换函数
@Param 目标像素格式
*/
FormatToARGB8888Func GetFormatToARGB8888Func( EPixelFormat srcFormat )
{
    switch( srcFormat )
    {
    case PFT_R8G8B8:    return RGB888ToARGB8888;
    case PFT_A8R8G8B8:  return ARGB8888ToARGB8888;
    case PFT_A8B8G8R8:  return ABGR8888ToARGB8888;
    case PFT_X8R8G8B8:  return XRGB8888ToARGB8888;
    case PFT_X8B8G8R8:  return XBGR8888ToARGB8888;
    case PFT_R5G6B5:    return RGB565ToARGB8888;
    case PFT_X1R5G5B5:  return XRGB1555ToARGB8888;
    case PFT_A1R5G5B5:  return ARGB1555ToARGB8888;
    case PFT_A4R4G4B4:  return ARGB4444ToARGB8888;
    case PFT_R3G3B2:    return RGB332ToARGB8888;
    case PFT_L8:        return L8ToARGB8888;
    case PFT_L16:       return L16ToARGB8888;
    case PFT_A4L4:      return AL44ToARGB8888;
    case PFT_A8L8:      return AL88ToARGB8888;
    case PFT_A8:        return AL88ToARGB8888;
    case PFT_V8U8:      return VU88ToARGB8888;
    case PFT_Q8W8V8U8:  return QWVU8888ToARGB8888;
    default:            return NULL;
    }
}

/** 获取指定的生成Mipmap的函数
@Param 指定的像素格式
*/
GenMipmapFunc GetGenMipmapFunc( EPixelFormat format )
{
    switch( format )
    {
    case PFT_R8G8B8:    return GenMipmapRGB888;
    case PFT_A8R8G8B8:  return GenMipmapARGB8888;
    case PFT_A8B8G8R8:  return GenMipmapABGR8888;
    case PFT_X8R8G8B8:  return GenMipmapXRGB8888;
    case PFT_X8B8G8R8:  return GenMipmapXBGR8888;
    case PFT_R5G6B5:    return GenMipmapRGB565;
    case PFT_X1R5G5B5:  return GenMipmapXRGB1555;
    case PFT_A1R5G5B5:  return GenMipmapARGB1555;
    case PFT_A4R4G4B4:  return GenMipmapARGB4444;
    case PFT_R3G3B2:    return GenMipmapRGB332;
    case PFT_L8:        return GenMipmapL8;
    case PFT_L16:       return GenMipmapL16;
    case PFT_A4L4:      return GenMipmapAL44;
    case PFT_A8L8:      return GenMipmapAL88;
    case PFT_A8:        return GenMipmapA8;
    case PFT_V8U8:      return GenMipmapVU88;
    case PFT_Q8W8V8U8:  return GenMipmapQWVU8888;
    default:            return NULL;
    }
}

/** 获取指定像素的线性拉伸函数
@Param 指定的像素格式
*/
StretchLinearFunc GetStretchLinearFunc( EPixelFormat format )
{
    switch( format )
    {
    case PFT_R8G8B8:    return StretchLinearRGB888;
    case PFT_A8R8G8B8:  return StretchLinearARGB8888;
    case PFT_A8B8G8R8:  return StretchLinearABGR8888;
    case PFT_X8R8G8B8:  return StretchLinearXRGB8888;
    case PFT_X8B8G8R8:  return StretchLinearXBGR8888;
    case PFT_R5G6B5:    return StretchLinearRGB565;
    case PFT_X1R5G5B5:  return StretchLinearXRGB1555;
    case PFT_A1R5G5B5:  return StretchLinearARGB1555;
    case PFT_A4R4G4B4:  return StretchLinearARGB4444;
    case PFT_R3G3B2:    return StretchLinearRGB332;
    case PFT_L8:        return StretchLinearL8;
    case PFT_L16:       return StretchLinearL16;
    case PFT_A4L4:      return StretchLinearAL44;
    case PFT_A8L8:      return StretchLinearAL88;
    case PFT_A8:        return StretchLinearA8;
    case PFT_V8U8:      return StretchLinearVU88;
    case PFT_Q8W8V8U8:  return StretchLinearQWVU8888;
    default:            return NULL;
    }
}

///////////////////////////////////////////////////////////////////////////


/** 获取指定图像像素的类型。
@Param 每个像素的字节数。
@Param R通道的掩码。
@Param G通道的掩码。
@Param B通道的掩码。
@Param Alpha通道的掩码。
@Param 是否使用Alpha通道。
*/
EPixelFormat F3D_PixelFormat( size_t bpp, uint32 nRMask, uint32 nGMask, uint32 nBMask,
                              uint32 nAlphaMask, bool bUseAlpha/* =true */ )
{
    switch( bpp )
    {
    case 1:
        if( (nRMask == 0x000000ff) )
            return PFT_L8;
        else if( (nRMask == 0x0000000f) && (nAlphaMask == 0x000000f0) && bUseAlpha )
            return PFT_A4L4;
        else if( (nRMask == 0x00000000) && (nAlphaMask == 0x000000ff) && bUseAlpha )
            return PFT_A8;
        else if( (nRMask == 0x000000e0) && (nGMask & 0x0000001c) &&
                 (nBMask == 0x00000003) )
            return PFT_R3G3B2;
        break;
    case 2:
        if( (nRMask == 0x0000f800) && (nGMask == 0x000007e0) &&
            (nBMask == 0x0000001f) )
            return PFT_R5G6B5;
        else if( (nRMask == 0x00007c00) && (nGMask == 0x000003e0) &&
                 (nBMask == 0x0000001f) )
            return PFT_X1R5G5B5;
        else if( (nRMask == 0x00007c00) && (nGMask == 0x000003e0) &&
                 (nBMask == 0x0000001f) && (nAlphaMask == 0x00008000)
                 && bUseAlpha )
            return PFT_A1R5G5B5;
        else if( (nRMask == 0x00000f00) && (nGMask == 0x000000f0) &&
                 (nBMask == 0x0000000f) && (nAlphaMask == 0x0000f000)
                 && bUseAlpha )
            return PFT_A4R4G4B4;
        else if( nRMask == 0x0000ffff )
            return PFT_L16;
        else if( (nRMask == 0x000000ff) && (nAlphaMask == 0x0000ff00)
                 && bUseAlpha )
            return PFT_A8L8;
        break;
    case 3:
        if( (nRMask == 0x00ff0000) && (nGMask == 0x0000ff00) &&
            (nBMask == 0x000000ff) )
            return PFT_R8G8B8;
        break;
    case 4:
        if( (nRMask == 0x00ff0000) && (nGMask == 0x0000ff00) &&
            (nBMask == 0x000000ff) && (nAlphaMask == 0xff000000)
            && bUseAlpha )
            return PFT_A8R8G8B8;
        else if( (nBMask == 0x00ff0000) && (nGMask == 0x0000ff00) &&
                 (nRMask == 0x000000ff) && (nAlphaMask == 0xff000000)
                 && bUseAlpha )
            return PFT_A8B8G8R8;
        else if( (nRMask == 0x00ff0000) && (nGMask == 0x0000ff00) &&
                 (nBMask == 0x000000ff) )
            return PFT_X8R8G8B8;
        else if( (nBMask == 0x00ff0000) && (nGMask == 0x0000ff00) &&
                 (nRMask == 0x000000ff) )
            return PFT_X8B8G8R8;
        break;
    default: return PFT_UNKNOWN;
    }

    return PFT_UNKNOWN;
}

/** 检测指定的像素格式是否含有Alpha
@Param 指定的像素格式
*/
bool F3D_HaveAlpha( EPixelFormat format )
{
    switch( format )
    {
    case PFT_A8R8G8B8:
    case PFT_A8B8G8R8:
    case PFT_A1R5G5B5:
    case PFT_A4R4G4B4:
    case PFT_A4L4:
    case PFT_A8L8:
    case PFT_A8:
    case PFT_A16B16G16R16F:
    case PFT_A32B32G32R32F:
        return true;
    case PFT_R8G8B8:
    case PFT_X8R8G8B8:
    case PFT_X8B8G8R8:
    case PFT_R5G6B5:
    case PFT_X1R5G5B5:
    case PFT_R3G3B2:
    case PFT_L8:
    case PFT_L16:
    case PFT_R16F:
    case PFT_R32F:
    case PFT_V8U8:
    case PFT_Q8W8V8U8:
    default:
        return false;
    }
}

/** 获取制定像素格式的大小
@Param 指定的像素格式
*/
size_t F3D_PixelSize( EPixelFormat format )
{
    switch( format )
    {
    case PFT_R3G3B2:
    case PFT_L8:
    case PFT_A4L4:
    case PFT_A8:
        return 1;
    case PFT_R5G6B5:
    case PFT_X1R5G5B5:
    case PFT_A1R5G5B5:
    case PFT_A4R4G4B4:
    case PFT_L16:
    case PFT_A8L8:
    case PFT_R16F:
    case PFT_V8U8:
        return 2;
    case PFT_R8G8B8:
        return 3;
    case PFT_A8R8G8B8:
    case PFT_A8B8G8R8:
    case PFT_X8R8G8B8:
    case PFT_X8B8G8R8:
    case PFT_R32F:
    case PFT_Q8W8V8U8:
        return 4;
    case PFT_A16B16G16R16F:
        return 8;
    case PFT_A32B32G32R32F:
        return 16;
    default: return 0;
    }
}

/** 获取制定宽高的图像有多少层Mipmap
@Param 指定图像的宽度
@Param 指定图像的高度
*/
size_t F3D_NumMipmaps( size_t width,size_t height )
{
    if( width < 1 || height < 1 )
        return 0;

    size_t level = 1;
    while( true )
    {
        if( width == 1 && height == 1 )
            break;

        width = width >> 1;
        height = height >> 1;

        if( width < 1 ) width = 1;
        if( height < 1 ) height = 1;

        level++;
    }

    return level;
}

/** 将当前的像素格式转换成指定的另一种像素格式
@Param 指向源数据的指针
@Param 源数据的像素格式
@Param 指向目标数据的指针
@Param 目标数据的像素格式
@Param 图像的宽度
@Param 图像的高度
*/
void F3D_ConvertPixelFormat( FBYTE* src,EPixelFormat srcFormat,FBYTE* dest,EPixelFormat destFormat,
                             size_t width,size_t height )
{
    if( srcFormat == destFormat )
    {
        memcpy( dest,src,F3D_PixelSize(srcFormat)*width*height );
    }
    else
    {
        size_t srcStride = F3D_PixelSize(srcFormat) * width;
        size_t destStride = F3D_PixelSize(destFormat) * width;

        FormatToARGB8888Func func1 = GetFormatToARGB8888Func( srcFormat );
        ARGB8888ToFormatFunc func2 = GetARGB8888ToFormatFunc( destFormat );

        // 分配一块临时内存
        FBYTE* tempBuf = new FBYTE[4*width];

        if( func1 && func2 )
        {
            for( size_t i=0;i<height;i++ )
            {
                func1( src+srcStride*i,tempBuf,width );
                func2( tempBuf,dest+srcStride*i,width );
            }
        }

        delete[] tempBuf;
    }
}

/** 生成图像的Mipmap(目标的宽高必须比源的小)
@Param 指向源数据的缓冲区
@Param 源图像的宽度
@Param 源图像的高度
@Param 源图像和目标图像的像素格式
@Param 指定目标数据的缓冲区
@Param 目标图像的宽度
@Param 目标图像的高度
*/
void F3D_GenerateMipmap( FBYTE* src,size_t srcWidth,size_t srcHeight,EPixelFormat format,
                         FBYTE* dest,size_t destWidth,size_t destHeight )
{
    GenMipmapFunc func;

    func = GetGenMipmapFunc( format );
    if( func )
    {
        func( src,srcWidth,srcHeight,dest,destWidth,destHeight );
    }
}

/** 线性插值法拉伸纹理(目标的宽高必须比源的大)
@Param 指向源数据的缓冲区
@Param 源图像的宽度
@Param 源图像的高度
@Param 源图像和目标图像的像素格式
@Param 指定目标数据的缓冲区
@Param 目标图像的宽度
@Param 目标图像的高度
*/
void F3D_StretchLinear( FBYTE* src,size_t srcWidth,size_t srcHeight,EPixelFormat format,
                        FBYTE* dest,size_t destWidth,size_t destHeight )
{
    StretchLinearFunc func;

    func = GetStretchLinearFunc( format );
    if( func )
    {
        func( src,srcWidth,srcHeight,dest,destWidth,destHeight );
    }
}

/** 将单色位图转换为A1R5G5B5格式
@Param 指向位图数据的指针
@Param 指向图像缓冲区的指针
@Param 图像的宽度
@Param 图像的高度
@Param 每行多余填充的字节数
@Param 是否对图像进行翻转
*/
void F3D_1BitToARGB1555( FBYTE* src,ushort* dest,size_t width,size_t height,
                         size_t linePad,bool flip )
{
    if( flip )
        dest += width * height;

    for( size_t y=0;y<height;y++ )
    {
        int shift = 7;

        if( flip )
            dest -= width;

        for( size_t x=0;x<width;x++ )
        {
            dest[x] = ((*src >> shift) & 0x1) ? 0xffff : 0x8000;

            if( shift-- < 0 )
            {
                shift = 7;
                src++;
            }
        }

        // 没有覆盖完最后一个字节
        if( shift != 7 ) src++;

        if( !flip ) dest += width;

        // 跳过填充的多余字节
        src += linePad;
    }
}

/** 将16色调色板位图数据转换为A1R5G5B5格式
@Param 指向位图数据的指针
@Param 指向图像缓冲区的指针
@Param 图像的宽度
@Param 图像的高度
@Param 每行多余填充的字节数
@Param 是否对图像进行翻转
*/
void F3D_4BitToARGB1555( FBYTE* src,ushort* dest,size_t width,size_t height,
                         uint32* palette,size_t linePad,bool flip )
{
#define XRGB8888ToARGB1555(c) (0x8000 | (((c) & 0xf80000) >> 9) | (((c) & 0xf800) >> 6) | (((c) & 0xf8) >> 3))

    if( flip )
        dest += width * height;

    for( size_t y=0;y<height;y++ )
    {
        int shift = 4;

        if( flip )
            dest -= width;

        for( size_t x=0;x<width;x++ )
        {
            dest[x] = XRGB8888ToARGB1555( palette[(*src >> shift) & 0xf] );

            if( shift == 0 )
            {
                shift = 4;
                src++;
            }
            else shift = 0;
        }

        if( shift == 0 ) src++;

        if( !flip ) src += width;

        // 跳过填充的多余字节
        src += linePad;
    }

#undef XRGB8888ToARGB1555
}

/** 将256色调色板位图数据转换为A1R5G5B5格式
@Param 指向位图数据的指针
@Param 指向图像缓冲区的指针
@Param 图像的宽度
@Param 图像的高度
@Param 每行多余填充的字节数
@Param 是否对图像进行翻转
*/
void F3D_8BitToARGB1555( FBYTE* src,ushort* dest,size_t width,size_t height,
                         uint32* palette,size_t linePad,bool flip )
{
#define XRGB8888ToARGB1555(c) (0x8000 | (((c) & 0xf80000) >> 9) | (((c) & 0xf800) >> 6) | (((c) & 0xf8) >> 3))

    if( flip )
        dest += width * height;

    for( size_t y=0;y<height;y++ )
    {
        if( flip )
            dest -= width;

        for( size_t x=0;x<width;x++ )
        {
            dest[x] = XRGB8888ToARGB1555( palette[*src] );
            src++;
        }

        if( !flip ) dest += width;

        // 跳过填充的多余字节
        src += linePad;
    }
}

/** 将16位图像文件里的数据拷贝到图像缓冲区中(A1R5G5B5)
@Param 文件里加载的图像数据的指针
@Param 指向图像缓冲区的指针
@Param 图像的宽度
@Param 图像的高度
@Param 每行多余填充的字节数
@Param 是否对图像进行翻转
*/
void F3D_16BitToImage( FBYTE* src,FBYTE* dest,size_t width,size_t height,
                       size_t linePad,bool flip )
{
    const size_t lineSize = width * 2;

    if( flip )
        dest += lineSize * height;

    for( size_t y=0;y<height;y++ )
    {
        if( flip )
            dest -= lineSize;

        memcpy( dest,src,lineSize );

        if( !flip )
            dest += lineSize;

        src += lineSize;
        src += linePad;
    }
}

/** 将24位图像文件里的数据拷贝到图像缓冲区中
@Param 从图像文件中加载的数据
@Param 指向图像缓冲区的指针
@Param 图像的宽度
@Param 图像的高度
@Param 每行多余填充的字节数
@Param 是否对图像进行翻转
@Param 是否交换红蓝通道
@Param 是否自动补齐为32位(X8R8G8B8)
*/
void F3D_24BitToImage( FBYTE* src,FBYTE* dest,size_t width,size_t height,
                       size_t linePad,bool flip,bool bgr,bool b32 )
{
    const size_t srcLineSize = width * 3;
    const size_t dstLineSize = width * (b32 ? 4 : 3);

    if( flip )
        dest += dstLineSize * height;

    for( size_t y=0;y<height;y++ )
    {
        if( flip )
            dest -= dstLineSize;

//         if( bgr )
//         {
        for( size_t x=0;x<srcLineSize;x+=3 )
        {
            dest[x+0] = src[x+0];
            dest[x+1] = src[x+1];
            dest[x+2] = src[x+2];
            if( bgr ) std::swap( dest[x],dest[x+2] );
            if( b32 ) dest[x+3] = 0xff;
        }
//         }
//         else
//         {
//             memcpy( dest,src,lineSize );
//         }

        if( !flip )
            dest += dstLineSize;

        src += srcLineSize;
        src += linePad;
    }
}

/** 将32位图像文件里的数据拷贝到图像缓冲区中
@Param 指向位图数据的指针
@Param 指向图像缓冲区的指针
@Param 图像的宽度
@Param 图像的高度
@Param 每行多余填充的字节数
@Param 是否对图像进行翻转
*/
void F3D_32BitToImage( FBYTE* src,FBYTE* dest,size_t width,size_t height,
                       size_t linePad,bool flip )
{
    const size_t lineSize = width * 4;

    if( flip )
        dest += lineSize * height;

    for( size_t y=0;y<height;y++ )
    {
        if( flip )
            dest -= lineSize;

        memcpy( dest,src,lineSize );

        if( !flip )
            dest += lineSize;

        src += lineSize;
        src += linePad;
    }
}


///////////////////////////////////////////////////////////////////////////
// 转换像素辅助函数的实现

void ARGB8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    memcpy( dest,src,numPixels*4 );
}

void ARGB8888ToRGB888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=3 )
    {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
    }
}

void ARGB8888ToABGR8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=4 )
    {
        dest[0] = src[2];
        dest[1] = src[1];
        dest[2] = src[0];
        dest[3] = src[3];
    }
}

void ARGB8888ToXRGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=4 )
    {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
        dest[3] = 0xff;
    }
}

void ARGB8888ToXBGR8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=4 )
    {
        dest[0] = src[2];
        dest[1] = src[1];
        dest[2] = src[0];
        dest[3] = 0xff;
    }
}

void ARGB8888ToRGB565( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* destW = (ushort*)dest;
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,destW++ )
    {
        *destW = (ushort)(((src[2] >> 3) << 11) |
            ((src[1] >> 2) << 5) | (src[0] >> 3));
    }
}

void ARGB8888ToXRGB1555( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* destW = (ushort*)dest;
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,destW++ )
    {
        *destW = (ushort)((0x1 << 15) | ((src[2] >> 3) << 10) |
            ((src[1] >> 3) << 5) | (src[0] >> 3));
    }
}

void ARGB8888ToARGB1555( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* destW = (ushort*)dest;
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,destW++ )
    {
        *destW = (ushort)(((src[3] >> 7) << 15) | ((src[2] >> 3) << 10) |
            ((src[1] >> 3) << 5) | (src[0] >> 3));
    }
}

void ARGB8888ToARGB4444( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* destW = (ushort*)dest;
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,destW++ )
    {
        *destW = (ushort)(((src[3] >> 4) << 12) | ((src[2] >> 4) << 8) |
            ((src[1] >> 4) << 4) | (src[0] >> 4));
    }
}

void ARGB8888ToRGB332( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest++ )
    {
        *dest = ((src[2] >> 5) << 5) | ((src[1] >> 5) << 2) | (src[0] >> 6);
    }
}

void ARGB8888ToL8( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest++ )
    {
        *dest = src[2] * L_R + src[1] * L_G + src[0] * L_B;
    }
}

void ARGB8888ToL16( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* destW = (ushort*)dest;
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,destW++ )
    {
        ushort srcW = (ushort)(src[2] * L_R + src[1] * L_G + src[0] * L_B);
        *destW = (srcW << 8) | srcW;
    }
}

void ARGB8888ToAL44( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest++ )
    {
        FBYTE srcL = src[2] * L_R + src[1] * L_G + src[0] * L_B;
        *dest = ((src[3] >> 4) << 4) | (srcL >> 4);
    }
}

void ARGB8888ToAL88( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=2 )
    {
        dest[0] = src[2] * L_R + src[1] * L_G + src[0] * L_B;
        dest[1] = src[0];
    }
}

void ARGB8888ToA8( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest++ )
    {
        *dest = src[3];
    }
}

// 当转换为凹凸贴图时，r=u,g=v,b=w,alpha=q
void ARGB8888ToVU88( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=2 )
    {
        dest[0] = src[2];
        dest[1] = src[1];
    }
}

void ARGB8888ToQWVU8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=4 )
    {
        dest[0] = src[2];
        dest[1] = src[1];
        dest[2] = src[0];
        dest[3] = src[3];
    }
}

void RGB888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 3;
    for( ;src<srcEnd;src+=3,dest+=4 )
    {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
        dest[3] = 0xff;
    }
}

void ABGR8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=4 )
    {
        dest[0] = src[2];
        dest[1] = src[1];
        dest[2] = src[0];
        dest[3] = src[3];
    }
}

void XRGB8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=4 )
    {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
        dest[3] = 0xff;
    }
}

void XBGR8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=4 )
    {
        dest[0] = src[2];
        dest[1] = src[1];
        dest[2] = src[0];
        dest[3] = 0xff;
    }
}

void RGB565ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* srcW = (ushort*)src;
    ushort* srcEnd = srcW + numPixels;
    for( ;srcW<srcEnd;srcW++,dest+=4 )
    {
        ushort r = (*srcW & 0xf800) >> 11;
        ushort g = (*srcW & 0x7e0) >> 5;
        ushort b = (*srcW & 0x1f);

        dest[0] = (b << 3) | (b >> 2);
        dest[1] = (g << 2) | (g >> 4);
        dest[2] = (r << 3) | (r >> 2);
        dest[3] = 0xff;
    }
}

void XRGB1555ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* srcW = (ushort*)src;
    ushort* srcEnd = srcW + numPixels;
    for( ;srcW<srcEnd;srcW++,dest+=4 )
    {
        ushort r = (*srcW & 0x7c00) >> 10;
        ushort g = (*srcW & 0x3e0) >> 5;
        ushort b = (*srcW & 0x1f);

        dest[0] = (b << 3) | (b >> 2);
        dest[1] = (g << 3) | (g >> 2);
        dest[2] = (r << 3) | (r >> 2);
        dest[3] = 0xff;
    }
}

void ARGB1555ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* srcW = (ushort*)src;
    ushort* srcEnd = srcW + numPixels;
    for( ;srcW<srcEnd;srcW++,dest+=4 )
    {
        ushort r = (*srcW & 0x7c00) >> 10;
        ushort g = (*srcW & 0x3e0) >> 5;
        ushort b = (*srcW & 0x1f);
        ushort alpha = (*srcW & 0x8000) >> 15;

        dest[0] = (b << 3) | (b >> 2);
        dest[1] = (g << 3) | (g >> 2);
        dest[2] = (r << 3) | (r >> 2);
        dest[3] = alpha ? 0 : 0xff;
    }
}

void ARGB4444ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* srcW = (ushort*)src;
    ushort* srcEnd = srcW + numPixels;
    for( ;srcW<srcEnd;srcW++,dest+=4 )
    {
        ushort r = (*srcW & 0xf00) >> 8;
        ushort g = (*srcW & 0xf0) >> 4;
        ushort b = (*srcW & 0xf);
        ushort alpha = (*srcW & 0xf000) >> 12;

        dest[0] = (r << 4) | r;
        dest[1] = (g << 4) | g;
        dest[2] = (b << 4) | b;
        dest[3] = (alpha << 4) | alpha;
    }
}

void RGB332ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels;
    for( ;src<srcEnd;src++,dest+=4 )
    {
        FBYTE r = (*src & 0xe0) >> 5;
        FBYTE g = (*src & 0x1c) >> 2;
        FBYTE b = (*src & 0x3);

        dest[0] = (b << 6) | (b << 4) | (b << 2) | b;
        dest[1] = (g << 5) | (g << 2) | (g >> 1);
        dest[2] = (r << 5) | (r >> 2) | (r >> 1);
        dest[3] = 0xff;
    }
}

void L8ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels;
    for( ;src<srcEnd;src++,dest+=4 )
    {
        dest[0] = src[0];
        dest[1] = src[0];
        dest[2] = src[0];
        dest[3] = 0xff;
    }
}

void L16ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    ushort* srcW = (ushort*)src;
    ushort* srcEnd = srcW + numPixels;
    for( ;srcW<srcEnd;srcW++,dest+=4 )
    {
        dest[0] = (*srcW >> 8);
        dest[1] = (*srcW >> 8);
        dest[2] = (*srcW >> 8);
        dest[3] = 0xff;
    }
}

void AL44ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels;
    for( ;src<srcEnd;src++,dest+=4 )
    {
        FBYTE luminance = (*src & 0xf);
        FBYTE alpha = (*src & 0xf0) >> 4;

        dest[0] = (luminance << 4) | luminance;
        dest[1] = (luminance << 4) | luminance;
        dest[2] = (luminance << 4) | luminance;
        dest[3] = (alpha << 4) | alpha;
    }
}

void AL88ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 2;
    for( ;src<srcEnd;src+=2,dest+=4 )
    {
        dest[0] = src[0];
        dest[1] = src[0];
        dest[2] = src[0];
        dest[3] = src[1];
    }
}

void A8ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels;
    for( ;src<srcEnd;src++,dest+=4 )
    {
        dest[0] = 0xff;
        dest[1] = 0xff;
        dest[2] = 0xff;
        dest[3] = *src;
    }
}

// 当转换为凹凸贴图时，r=u,g=v,b=w,alpha=q
void VU88ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 2;
    for( ;src<srcEnd;src+=2,dest+=4 )
    {
        dest[0] = 0;
        dest[1] = src[1];
        dest[2] = src[0];
        dest[3] = 0;
    }
}

void QWVU8888ToARGB8888( FBYTE* src,FBYTE* dest,size_t numPixels )
{
    FBYTE* srcEnd = src + numPixels * 4;
    for( ;src<srcEnd;src+=4,dest+=4 )
    {
        dest[0] = src[2];
        dest[1] = src[1];
        dest[2] = src[0];
        dest[3] = src[3];
    }
}


///////////////////////////////////////////////////////////////////////////
// 生成Mipmap辅助函数的实现

void GenMipmapARGB8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y,c)     (4*((y)*srcW+(x))+(c))
#define DESTINDEX(x,y,c)    (4*((y)*destW+(x))+(c))

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            dest[DESTINDEX(x,y,0)] = (
                src[SRCINDEX(u+0,v+0,0)] +
                src[SRCINDEX(u+1,v+0,0)] +
                src[SRCINDEX(u+0,v+1,0)] +
                src[SRCINDEX(u+1,v+1,0)]) >> 2;
            dest[DESTINDEX(x,y,1)] = (
                src[SRCINDEX(u+0,v+0,1)] +
                src[SRCINDEX(u+1,v+0,1)] +
                src[SRCINDEX(u+0,v+1,1)] +
                src[SRCINDEX(u+1,v+1,1)]) >> 2;
            dest[DESTINDEX(x,y,2)] = (
                src[SRCINDEX(u+0,v+0,2)] +
                src[SRCINDEX(u+1,v+0,2)] +
                src[SRCINDEX(u+0,v+1,2)] +
                src[SRCINDEX(u+1,v+1,2)]) >> 2;
            dest[DESTINDEX(x,y,3)] = (
                src[SRCINDEX(u+0,v+0,3)] +
                src[SRCINDEX(u+1,v+0,3)] +
                src[SRCINDEX(u+0,v+1,3)] +
                src[SRCINDEX(u+1,v+1,3)]) >> 2;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
}

void GenMipmapRGB888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y,c)     (3*((y)*srcW+(x))+(c))
#define DESTINDEX(x,y,c)    (3*((y)*destW+(x))+(c))

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            dest[DESTINDEX(x,y,0)] = (
                src[SRCINDEX(u+0,v+0,0)] +
                src[SRCINDEX(u+1,v+0,0)] +
                src[SRCINDEX(u+0,v+1,0)] +
                src[SRCINDEX(u+1,v+1,0)]) >> 2;
            dest[DESTINDEX(x,y,1)] = (
                src[SRCINDEX(u+0,v+0,1)] +
                src[SRCINDEX(u+1,v+0,1)] +
                src[SRCINDEX(u+0,v+1,1)] +
                src[SRCINDEX(u+1,v+1,1)]) >> 2;
            dest[DESTINDEX(x,y,2)] = (
                src[SRCINDEX(u+0,v+0,2)] +
                src[SRCINDEX(u+1,v+0,2)] +
                src[SRCINDEX(u+0,v+1,2)] +
                src[SRCINDEX(u+1,v+1,2)]) >> 2;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
}

void GenMipmapABGR8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    GenMipmapARGB8888( src,srcW,srcH,dest,destW,destH );
}

void GenMipmapXRGB8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    GenMipmapARGB8888( src,srcW,srcH,dest,destW,destH );
}

void GenMipmapXBGR8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    GenMipmapARGB8888( src,srcW,srcH,dest,destW,destH );
}

void GenMipmapRGB565( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y)   ((y)*srcW+(x))
#define DESTINDEX(x,y)  ((y)*destW+(x))
#define RED(c)          (((c) & 0xf800) >> 11)
#define GREEN(c)        (((c) & 0x7e0) >> 5)
#define BLUE(c)         ((c) & 0x1f)

    ushort r,g,b;
    ushort* srcShort = (ushort*)src;
    ushort* destShort = (ushort*)dest;

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            r = ( RED(srcShort[SRCINDEX(u+0,v+0)]) + 
                  RED(srcShort[SRCINDEX(u+1,v+0)]) +
                  RED(srcShort[SRCINDEX(u+0,v+1)]) +
                  RED(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;
            g = ( GREEN(srcShort[SRCINDEX(u+0,v+0)]) +
                  GREEN(srcShort[SRCINDEX(u+1,v+0)]) +
                  GREEN(srcShort[SRCINDEX(u+0,v+1)]) +
                  GREEN(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;
            b = ( BLUE(srcShort[SRCINDEX(u+0,v+0)]) +
                  BLUE(srcShort[SRCINDEX(u+1,v+0)]) +
                  BLUE(srcShort[SRCINDEX(u+0,v+1)]) +
                  BLUE(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;

            destShort[DESTINDEX(x,y)] = (r << 11) | (g << 5) | b;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
#undef RED
#undef GREEN
#undef BLUE
}

void GenMipmapXRGB1555( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    GenMipmapARGB1555( src,srcW,srcH,dest,destW,destH );
}

void GenMipmapARGB1555( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y)   ((y)*srcW+(x))
#define DESTINDEX(x,y)  ((y)*destW+(x))
#define ALPHA(c)        (((c) & 0x8000) >> 15)
#define RED(c)          (((c) & 0x7c00) >> 10)
#define GREEN(c)        (((c) & 0x3e0) >> 5)
#define BLUE(c)         ((c) & 0x1f)

    ushort r,g,b,alpha;
    ushort* srcShort = (ushort*)src;
    ushort* destShort = (ushort*)dest;

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            r = ( RED(srcShort[SRCINDEX(u+0,v+0)]) +
                  RED(srcShort[SRCINDEX(u+1,v+0)]) +
                  RED(srcShort[SRCINDEX(u+0,v+1)]) +
                  RED(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;
            g = ( GREEN(srcShort[SRCINDEX(u+0,v+0)]) +
                  GREEN(srcShort[SRCINDEX(u+1,v+0)]) +
                  GREEN(srcShort[SRCINDEX(u+0,v+1)]) +
                  GREEN(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;
            b = ( BLUE(srcShort[SRCINDEX(u+0,v+0)]) +
                  BLUE(srcShort[SRCINDEX(u+1,v+0)]) +
                  BLUE(srcShort[SRCINDEX(u+0,v+1)]) +
                  BLUE(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;
            alpha = ( ALPHA(srcShort[SRCINDEX(u+0,v+0)]) +
                      ALPHA(srcShort[SRCINDEX(u+1,v+0)]) +
                      ALPHA(srcShort[SRCINDEX(u+0,v+1)]) +
                      ALPHA(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;

            destShort[DESTINDEX(x,y)] = (alpha << 15) | (r << 10) | (g << 5) | b;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
#undef ALPHA
#undef RED
#undef GREEN
#undef BLUE
}

void GenMipmapARGB4444( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y)   ((y)*srcW+(x))
#define DESTINDEX(x,y)  ((y)*destW+(x))
#define ALPHA(c)        (((c) & 0xf000) >> 12)
#define RED(c)          (((c) & 0x0f00) >> 8)
#define GREEN(c)        (((c) & 0x00f0) >> 4)
#define BLUE(c)         ((c) & 0x000f)

    ushort r,g,b,alpha;
    ushort* srcShort = (ushort*)src;
    ushort* destShort = (ushort*)dest;

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            r = ( RED(srcShort[SRCINDEX(u+0,v+0)]) +
                  RED(srcShort[SRCINDEX(u+1,v+0)]) +
                  RED(srcShort[SRCINDEX(u+0,v+1)]) +
                  RED(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;
            g = ( GREEN(srcShort[SRCINDEX(u+0,v+0)]) +
                  GREEN(srcShort[SRCINDEX(u+1,v+0)]) +
                  GREEN(srcShort[SRCINDEX(u+0,v+1)]) +
                  GREEN(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;
            b = ( BLUE(srcShort[SRCINDEX(u+0,v+0)]) +
                  BLUE(srcShort[SRCINDEX(u+1,v+0)]) +
                  BLUE(srcShort[SRCINDEX(u+0,v+1)]) +
                  BLUE(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;
            alpha = ( ALPHA(srcShort[SRCINDEX(u+0,v+0)]) +
                      ALPHA(srcShort[SRCINDEX(u+1,v+0)]) +
                      ALPHA(srcShort[SRCINDEX(u+0,v+1)]) +
                      ALPHA(srcShort[SRCINDEX(u+1,v+1)]) ) >> 2;

            destShort[DESTINDEX(x,y)] = (alpha << 12) | (r << 8) | (g << 4) | b;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
#undef ALPHA
#undef RED
#undef GREEN
#undef BLUE
}

void GenMipmapRGB332( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y)   ((y)*srcW+(x))
#define DESTINDEX(x,y)  ((y)*destW+(x))
#define RED(c)          (((c) & 0xe0) >> 5)
#define GREEN(c)        (((c) & 0x1c) >> 2)
#define BLUE(c)         ((c) & 0x3)

    FBYTE r,g,b;

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            r = ( RED(src[SRCINDEX(u+0,v+0)]) +
                  RED(src[SRCINDEX(u+1,v+0)]) +
                  RED(src[SRCINDEX(u+0,v+1)]) +
                  RED(src[SRCINDEX(u+1,v+1)]) ) >> 2;
            g = ( GREEN(src[SRCINDEX(u+0,v+0)]) +
                  GREEN(src[SRCINDEX(u+1,v+0)]) +
                  GREEN(src[SRCINDEX(u+0,v+1)]) +
                  GREEN(src[SRCINDEX(u+1,v+1)]) ) >> 2;
            b = ( BLUE(src[SRCINDEX(u+0,v+0)]) +
                  BLUE(src[SRCINDEX(u+1,v+0)]) +
                  BLUE(src[SRCINDEX(u+0,v+1)]) +
                  BLUE(src[SRCINDEX(u+1,v+1)]) ) >> 2;

            dest[DESTINDEX(x,y)] = (r << 5) | (g << 2) | b;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
#undef RED
#undef GREEN
#undef BLUE
}

void GenMipmapL8( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y)   ((y)*srcW+(x))
#define DESTINDEX(x,y)  ((y)*destW+(x))

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            dest[DESTINDEX(x,y)] = (
                src[SRCINDEX(u+0,v+0)] +
                src[SRCINDEX(u+1,v+0)] +
                src[SRCINDEX(u+0,v+1)] +
                src[SRCINDEX(u+1,v+1)] ) >> 2;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
}

void GenMipmapL16( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y)   ((y)*srcW+(x))
#define DESTINDEX(x,y)  ((y)*destW+(x))

    ushort* srcShort = (ushort*)src;
    ushort* destShort = (ushort*)dest;

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            destShort[DESTINDEX(x,y)] = (
                srcShort[SRCINDEX(u+0,v+0)] +
                srcShort[SRCINDEX(u+1,v+0)] +
                srcShort[SRCINDEX(u+0,v+1)] +
                srcShort[SRCINDEX(u+1,v+1)] ) >> 2;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
}

void GenMipmapAL44( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y)   ((y)*srcW+(x))
#define DESTINDEX(x,y)  ((y)*destW+(x))
#define ALPHA(c)        (((c) & 0xf0) >> 4)
#define LUMINANCE(c)    ((c) & 0x0f)

    FBYTE alpha,luminance;

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            alpha = ( ALPHA(src[SRCINDEX(u+0,v+0)]) +
                      ALPHA(src[SRCINDEX(u+1,v+0)]) +
                      ALPHA(src[SRCINDEX(u+0,v+1)]) +
                      ALPHA(src[SRCINDEX(u+1,v+1)]) ) >> 2;
            luminance = ( LUMINANCE(src[SRCINDEX(u+0,v+0)]) +
                          LUMINANCE(src[SRCINDEX(u+1,v+0)]) +
                          LUMINANCE(src[SRCINDEX(u+0,v+1)]) +
                          LUMINANCE(src[SRCINDEX(u+1,v+1)]) ) >> 2;

            dest[DESTINDEX(x,y)] = (alpha << 4) | luminance;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
#undef ALPHA
#undef LUMINANCE
}

void GenMipmapAL88( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define SRCINDEX(x,y,c)   (2*((y)*srcW+(x))+(c))
#define DESTINDEX(x,y,c)  (2*((y)*destW+(x))+(c))

    const size_t dx = srcW / destW;
    const size_t dy = srcH / destH;

    for( size_t y=0;y<destH;y++ )
    {
        for( size_t x=0;x<destW;x++ )
        {
            const size_t u = x * dx;
            const size_t v = y * dy;

            // Luminance
            dest[DESTINDEX(x,y,0)] = (
                src[SRCINDEX(u+0,v+0,0)] +
                src[SRCINDEX(u+1,v+0,0)] +
                src[SRCINDEX(u+0,v+1,0)] +
                src[SRCINDEX(u+1,v+1,0)] ) >> 2;

            // Alpha
            dest[DESTINDEX(x,y,1)] = (
                src[SRCINDEX(u+0,v+0,1)] +
                src[SRCINDEX(u+1,v+0,1)] +
                src[SRCINDEX(u+0,v+1,1)] +
                src[SRCINDEX(u+1,v+1,1)] ) >> 2;
        }
    }

#undef SRCINDEX
#undef DESTINDEX
}

void GenMipmapA8( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    GenMipmapL8( src,srcW,srcH,dest,destW,destH );
}

void GenMipmapVU88( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    GenMipmapAL88( src,srcW,srcH,dest,destW,destH );
}

void GenMipmapQWVU8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    GenMipmapARGB8888( src,srcW,srcH,dest,destW,destH );
}


///////////////////////////////////////////////////////////////////////////
// 线性拉伸图像辅助函数的实现

void StretchLinearARGB8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define RED(c)      (((c) & 0x00ff0000) >> 16)
#define GREEN(c)    (((c) & 0x0000ff00) >> 8)
#define BLUE(c)     ((c) & 0x000000ff)
#define ALPHA(c)    (((c) & 0xff000000) >> 24)

    uint32* srcData = (uint32*)src;
    uint32* destData = (uint32*)dest;

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            uint32 color0 = 0;
            uint32 color1 = 0;
            uint32 color2 = 0;
            uint32 color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color0 = srcData[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color1 = srcData[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = srcData[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = srcData[(sy+1)*srcW+(sx+1)];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            FBYTE r = (FBYTE)(pm0*RED(color0) + pm1*RED(color1) + pm2*RED(color2) + pm3*RED(color3));
            FBYTE g = (FBYTE)(pm0*GREEN(color0) + pm1*GREEN(color1) + pm2*GREEN(color2) + pm3*GREEN(color3));
            FBYTE b = (FBYTE)(pm0*BLUE(color0) + pm1*BLUE(color1) + pm2*BLUE(color2) + pm3*BLUE(color3));
            FBYTE a = (FBYTE)(pm0*ALPHA(color0) + pm1*ALPHA(color1) + pm2*ALPHA(color2) + pm3*ALPHA(color3));

            destData[dy*(destH)+dx] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

#undef RED
#undef GREEN
#undef BLUE
#undef ALPHA
}

void StretchLinearRGB888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define RED(c)      (((c) & 0x00ff0000) >> 16)
#define GREEN(c)    (((c) & 0x0000ff00) >> 8)
#define BLUE(c)     ((c) & 0x000000ff)

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            FBYTE color0[3] = {0};
            FBYTE color1[3] = {0};
            FBYTE color2[3] = {0};
            FBYTE color3[3] = {0};
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) memcpy( color0,&src[3*(sy*srcW+sx)],sizeof(FBYTE)*3 );

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) memcpy( color1,&src[3*(sy*srcW+sx+1)],sizeof(FBYTE)*3 );

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) memcpy( color2,&src[3*((sy+1)*srcW+sx)],sizeof(FBYTE)*3 );

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) memcpy( color3,&src[3*((sy+1)*srcW+(sx+1))],sizeof(FBYTE)*3 );

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            dest[3*(dy*destW+dx)+0] = (FBYTE)(pm0*color0[0] + pm1*color1[0] + pm2*color2[0] + pm3*color3[0]);
            dest[3*(dy*destW+dx)+1] = (FBYTE)(pm0*color0[1] + pm1*color1[1] + pm2*color2[1] + pm3*color3[1]);
            dest[3*(dy*destW+dx)+2] = (FBYTE)(pm0*color0[2] + pm1*color1[2] + pm2*color2[2] + pm3*color3[2]);
        }
    }

#undef RED
#undef GREEN
#undef BLUE
}

void StretchLinearABGR8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    StretchLinearARGB8888( src,srcW,srcH,dest,destW,destH );
}

void StretchLinearXRGB8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    StretchLinearARGB8888( src,srcW,srcH,dest,destW,destH );
}

void StretchLinearXBGR8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    StretchLinearARGB8888( src,srcW,srcH,dest,destW,destH );
}

void StretchLinearRGB565( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define RED(c)      (((c) & 0xf800) >> 11)
#define GREEN(c)    (((c) & 0x7e0) >> 5)
#define BLUE(c)     ((c) & 0x1f)

    ushort* srcData = (ushort*)src;
    ushort* destData = (ushort*)dest;

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            ushort color0 = 0;
            ushort color1 = 0;
            ushort color2 = 0;
            ushort color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color0 = srcData[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color1 = srcData[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = srcData[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = srcData[(sy+1)*srcW+sx+1];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            FBYTE r = (FBYTE)(pm0*RED(color0) + pm1*RED(color1) + pm2*RED(color2) + pm3*RED(color3));
            FBYTE g = (FBYTE)(pm0*GREEN(color0) + pm1*GREEN(color1) + pm2*GREEN(color2) + pm3*GREEN(color3));
            FBYTE b = (FBYTE)(pm0*BLUE(color0) + pm1*BLUE(color1) + pm2*BLUE(color2) + pm3*BLUE(color3));

            destData[dy*destW+dx] = (r << 11) | (g << 5) | b;
        }
    }

#undef RED
#undef GREEN
#undef BLUE
}

void StretchLinearARGB1555( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define RED(c)      (((c) & 0x7c00) >> 10)
#define GREEN(c)    (((c) & 0x3e0) >> 5)
#define BLUE(c)     ((c) & 0x1f)
#define ALPHA(c)    (((c) & 0x8000) >> 15)

    ushort* srcData = (ushort*)src;
    ushort* destData = (ushort*)dest;

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            ushort color0 = 0;
            ushort color1 = 0;
            ushort color2 = 0;
            ushort color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color0 = srcData[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color1 = srcData[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = srcData[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = srcData[(sy+1)*srcW+(sx+1)];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            FBYTE r = (FBYTE)(pm0*RED(color0) + pm1*RED(color1) + pm2*RED(color2) + pm3*RED(color3));
            FBYTE g = (FBYTE)(pm0*GREEN(color0) + pm1*GREEN(color1) + pm2*GREEN(color2) + pm3*GREEN(color3));
            FBYTE b = (FBYTE)(pm0*BLUE(color0) + pm1*BLUE(color1) + pm2*BLUE(color2) + pm3*BLUE(color3));
            FBYTE a = (FBYTE)(pm0*ALPHA(color0) + pm1*ALPHA(color1) + pm2*ALPHA(color2) + pm3*ALPHA(color3));

            destData[dy*destH+dx] = (a << 15) | (r << 5) | (g << 5) | b;
        }
    }

#undef RED
#undef GREEN
#undef BLUE
#undef ALPHA
}

void StretchLinearXRGB1555( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    StretchLinearARGB1555( src,srcW,srcH,dest,destW,destH );
}

void StretchLinearARGB4444( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define RED(c)      (((c) & 0x0f00) >> 8)
#define GREEN(c)    (((c) & 0x00f0) >> 4)
#define BLUE(c)     ((c) & 0x000f)
#define ALPHA(c)    (((c) & 0xf000) >> 12)

    ushort* srcData = (ushort*)src;
    ushort* destData = (ushort*)dest;

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            ushort color0 = 0;
            ushort color1 = 0;
            ushort color2 = 0;
            ushort color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color0 = srcData[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color1 = srcData[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = srcData[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = srcData[(sy+1)*srcW+(sx+1)];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            FBYTE r = (FBYTE)(pm0*RED(color0) + pm1*RED(color1) + pm2*RED(color2) + pm3*RED(color3));
            FBYTE g = (FBYTE)(pm0*GREEN(color0) + pm1*GREEN(color1) + pm2*GREEN(color2) + pm3*GREEN(color3));
            FBYTE b = (FBYTE)(pm0*BLUE(color0) + pm1*BLUE(color1) + pm2*BLUE(color2) + pm3*BLUE(color3));
            FBYTE a = (FBYTE)(pm0*ALPHA(color0) + pm1*ALPHA(color1) + pm2*ALPHA(color2) + pm3*ALPHA(color3));

            destData[dy*destW+dx] = (a << 12) | (r << 8) | (g << 4) | b;
        }
    }

#undef RED
#undef GREEN
#undef BLUE
#undef ALPHA
}

void StretchLinearRGB332( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define RED(c)      (((c) & 0xe0) >> 5)
#define GREEN(c)    (((c) & 0x1c) >> 2)
#define BLUE(c)     ((c) & 0x3)

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            FBYTE color0 = 0;
            FBYTE color1 = 0;
            FBYTE color2 = 0;
            FBYTE color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color0 = src[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color1 = src[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = src[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = src[(sy+1)*srcW+sx+1];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            FBYTE r = (FBYTE)(pm0*RED(color0) + pm1*RED(color1) + pm2*RED(color2) + pm3*RED(color3));
            FBYTE g = (FBYTE)(pm0*GREEN(color0) + pm1*GREEN(color1) + pm2*GREEN(color2) + pm3*GREEN(color3));
            FBYTE b = (FBYTE)(pm0*BLUE(color0) + pm1*BLUE(color1) + pm3*BLUE(color2) + pm3*BLUE(color3));

            dest[dy*destW+dx] = (r << 5) | (g << 2) | b;
        }
    }

#undef RED
#undef GREEN
#undef ALPHA
}

void StretchLinearL8( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            FBYTE color0 = 0;
            FBYTE color1 = 0;
            FBYTE color2 = 0;
            FBYTE color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcx || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcy || sy < 0) ? false : true;
            if( inSrc ) color0 = src[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcx || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcy || sy < 0) ? false : true;
            if( inSrc ) color1 = src[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcx || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcy || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = src[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcx || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcy || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = src[(sy+1)*srcW+sx+1];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            dest[dy*destW+dx] = pm0*color0 + pm1*color1 + pm2*color2 + pm3*color3;
        }
    }
}

void StretchLinearL16( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    ushort* srcData = (ushort*)src;
    ushort* destData = (ushort*)dest;

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            ushort color0 = 0;
            ushort color1 = 0;
            ushort color2 = 0;
            ushort color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color0 = srcData[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color1 = srcData[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = srcData[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = srcData[(sy+1)*srcW+sx+1];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            destData[dy*destW+dx] = pm0*color0 + pm1*color1 + pm2*color2 + pm3*color3;
        }
    }
}

void StretchLinearAL44( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define ALPHA(c)        (((c) & 0xf0) >> 4)
#define LUMINANCE(c)    ((c) & 0x0f)

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx<destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW / destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            FBYTE color0 = 0;
            FBYTE color1 = 0;
            FBYTE color2 = 0;
            FBYTE color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color0 = src[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color1 = src[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = src[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = src[(sy+1)*srcW+sx+1];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            FBYTE a = (FBYTE)(pm0*ALPHA(color0) + pm1*ALPHA(color1) + pm2*ALPHA(color2) + pm3*ALPHA(color3));
            FBYTE l = (FBYTE)(pm0*LUMINANCE(color0) + pm1*LUMINANCE(color1) + pm2*LUMINANCE(color2) + pm3*LUMINANCE(color3));

            dest[dy*destW+dx] = (a << 4) | l;
        }
    }

#undef ALPHA
#undef LUMINANCE
}

void StretchLinearAL88( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
#define ALPHA(c)        (((c) & 0xff00) >> 8)
#define LUMINANCE(c)    ((c) & 0x00ff)

    ushort* srcData = (ushort*)src;
    ushort* destData = (ushort*)dest;

    for( size_t dy=0;dy<destH;dy++ )
    {
        float srcy = (dy + 0.4999999f) * (srcH / destH) - 0.5f;

        for( size_t dx=0;dx>destW;dx++ )
        {
            float srcx = (dx + 0.4999999f) * (srcW /destW) - 0.5f;

            int sx = static_cast<int>(srcx);
            int sy = static_cast<int>(srcy);

            ushort color0 = 0;
            ushort color1 = 0;
            ushort color2 = 0;
            ushort color3 = 0;
            bool inSrc = true;

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color0 = srcData[sy*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy >= (int)srcH || sy < 0) ? false : true;
            if( inSrc ) color1 = srcData[sy*srcW+sx+1];

            inSrc = (sx >= (int)srcW || sx < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color2 = srcData[(sy+1)*srcW+sx];

            inSrc = (sx+1 >= (int)srcW || sx+1 < 0) ? false : true;
            inSrc = (sy+1 >= (int)srcH || sy+1 < 0) ? false : true;
            if( inSrc ) color3 = srcData[(sy+1)*srcW+sx+1];

            float u = srcx - sx;
            float v = srcy - sy;
            float pm0 = (1.0f - u) * (1.0f - v);
            float pm1 = u * (1.0f - v);
            float pm2 = v * (1.0f - u);
            float pm3 = u * v;

            ushort a = (ushort)(pm0*ALPHA(color0) + pm1*ALPHA(color1) + pm2*ALPHA(color2) + pm3*ALPHA(color3));
            ushort l = (ushort)(pm0*LUMINANCE(color0) + pm1*LUMINANCE(color1) + pm2*LUMINANCE(color2) + pm3*LUMINANCE(color3));

            destData[dy*destW+dx] = (a << 8) | l;
        }
    }

#undef ALPHA
#undef LUMINANCE
}

void StretchLinearA8( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    StretchLinearL8( src,srcW,srcH,dest,destW,destH );
}

void StretchLinearVU88( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    StretchLinearAL88( src,srcW,srcH,dest,destW,destH );
}

void StretchLinearQWVU8888( FBYTE* src,size_t srcW,size_t srcH,FBYTE* dest,size_t destW,size_t destH )
{
    StretchLinearARGB8888( src,srcW,srcH,dest,destW,destH );
}

}