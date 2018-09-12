/*
 * ------------------------------------------------------------------------
 *	Name:   F3DPixelUtil.h
 *  Desc:   本文件定义一些处理像素的辅助函数。
 *  Author: Yish
 *  Date:   2010/8/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_PIXELUTIL_H__
#define __F3D_PIXELUTIL_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////

extern "C" {

// 获取指定像素格式的大小
FAIRY_API size_t F3D_PixelSize( EPixelFormat format );

// 获取制定宽高的图像有多少层Mipmap
FAIRY_API size_t F3D_NumMipmaps( size_t width, size_t height );

// 检测指定的像素格式是否包含Alpha
FAIRY_API bool F3D_HaveAlpha( EPixelFormat format );

// 获取指定图像像素的类型
FAIRY_API EPixelFormat F3D_PixelFormat( size_t bpp, uint32 nRMask, uint32 nGMask, uint32 nBMask, uint32 nAlphaMask, bool bUseAlpha = true );

// 转变像素格式
FAIRY_API void F3D_ConvertPixelFormat( fbyte* src, EPixelFormat srcFormat, fbyte* dest, EPixelFormat destFormat, size_t width, size_t height );

// 生成图像的Mipmap(目标的宽高必须比源的小)
FAIRY_API void F3D_GenerateMipmap( fbyte* src, size_t srcWidth, size_t srcHeight, EPixelFormat format, fbyte* dest, size_t destWidth, size_t destHeight );

// 线性插值法拉伸纹理(目标的宽高必须比源的大)
FAIRY_API void F3D_StretchLinear( fbyte* src, size_t srcWidth, size_t srcHeight, EPixelFormat format, fbyte* dest, size_t destWidth, size_t destHeight );

// 将单色位图转换为A1R5G5B5格式
FAIRY_API void F3D_1BitToARGB1555( fbyte* src, ushort* dest, size_t width, size_t height, size_t linePad, bool flip = false );

// 将16调色板位图转换为A1R5G5B5格式
FAIRY_API void F3D_4BitToARGB1555( fbyte* src, ushort* dest, size_t width, size_t height, uint32* palette, size_t linePad, bool flip = false );

// 将256调色板图像转换为A1R5G5B5格式
FAIRY_API void F3D_8BitToARGB1555( fbyte* src, ushort* dest, size_t width, size_t height, uint32* palette, size_t linePad, bool flip = false );

// 将文件里的16位数据拷贝到图像缓冲区中(A1R5G5B5)
FAIRY_API void F3D_16BitToImage( fbyte* src, fbyte* dest, size_t width, size_t height, size_t linePad, bool flip = false );

// 将文件里的24位数据拷贝到图像缓冲区中
// Param1: 源数据
// Param2: 目标数据
// Param3: 图像的宽度
// Param4: 图像的高度
// Param5: 每行的字节数
// Param6: 是否翻转
// Param7: 是否交换R和B
// Param8: 是否转换为32位颜色
FAIRY_API void F3D_24BitToImage( fbyte* src, fbyte* dest, size_t width, size_t height, size_t linePad, bool flip = false, bool bgr = false, bool b32 = false );

// 将文件里的32位数据拷贝到图像缓冲区中
FAIRY_API void F3D_32BitToImage( fbyte* src, fbyte* dest, size_t width, size_t height, size_t linePad, bool flip = false );

}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PIXELUTIL_H__