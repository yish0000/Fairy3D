/*
 * ------------------------------------------------------------------------
 *	Name:   F3DPixelUtil.h
 *  Desc:   ���ļ�����һЩ�������صĸ���������
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

// ��ȡָ�����ظ�ʽ�Ĵ�С
FAIRY_API size_t F3D_PixelSize( EPixelFormat format );

// ��ȡ�ƶ���ߵ�ͼ���ж��ٲ�Mipmap
FAIRY_API size_t F3D_NumMipmaps( size_t width, size_t height );

// ���ָ�������ظ�ʽ�Ƿ����Alpha
FAIRY_API bool F3D_HaveAlpha( EPixelFormat format );

// ��ȡָ��ͼ�����ص�����
FAIRY_API EPixelFormat F3D_PixelFormat( size_t bpp, uint32 nRMask, uint32 nGMask, uint32 nBMask, uint32 nAlphaMask, bool bUseAlpha = true );

// ת�����ظ�ʽ
FAIRY_API void F3D_ConvertPixelFormat( FBYTE* src, EPixelFormat srcFormat, FBYTE* dest, EPixelFormat destFormat, size_t width, size_t height );

// ����ͼ���Mipmap(Ŀ��Ŀ�߱����Դ��С)
FAIRY_API void F3D_GenerateMipmap( FBYTE* src, size_t srcWidth, size_t srcHeight, EPixelFormat format, FBYTE* dest, size_t destWidth, size_t destHeight );

// ���Բ�ֵ����������(Ŀ��Ŀ�߱����Դ�Ĵ�)
FAIRY_API void F3D_StretchLinear( FBYTE* src, size_t srcWidth, size_t srcHeight, EPixelFormat format, FBYTE* dest, size_t destWidth, size_t destHeight );

// ����ɫλͼת��ΪA1R5G5B5��ʽ
FAIRY_API void F3D_1BitToARGB1555( FBYTE* src, ushort* dest, size_t width, size_t height, size_t linePad, bool flip = false );

// ��16��ɫ��λͼת��ΪA1R5G5B5��ʽ
FAIRY_API void F3D_4BitToARGB1555( FBYTE* src, ushort* dest, size_t width, size_t height, uint32* palette, size_t linePad, bool flip = false );

// ��256��ɫ��ͼ��ת��ΪA1R5G5B5��ʽ
FAIRY_API void F3D_8BitToARGB1555( FBYTE* src, ushort* dest, size_t width, size_t height, uint32* palette, size_t linePad, bool flip = false );

// ���ļ����16λ���ݿ�����ͼ�񻺳�����(A1R5G5B5)
FAIRY_API void F3D_16BitToImage( FBYTE* src, FBYTE* dest, size_t width, size_t height, size_t linePad, bool flip = false );

// ���ļ����24λ���ݿ�����ͼ�񻺳�����
// Param1: Դ����
// Param2: Ŀ������
// Param3: ͼ��Ŀ��
// Param4: ͼ��ĸ߶�
// Param5: ÿ�е��ֽ���
// Param6: �Ƿ�ת
// Param7: �Ƿ񽻻�R��B
// Param8: �Ƿ�ת��Ϊ32λ��ɫ
FAIRY_API void F3D_24BitToImage( FBYTE* src, FBYTE* dest, size_t width, size_t height, size_t linePad, bool flip = false, bool bgr = false, bool b32 = false );

// ���ļ����32λ���ݿ�����ͼ�񻺳�����
FAIRY_API void F3D_32BitToImage( FBYTE* src, FBYTE* dest, size_t width, size_t height, size_t linePad, bool flip = false );

}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PIXELUTIL_H__