/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Util.h
 *  Desc:   本文件为渲染设备模块定义了一些实用函数。
 *  Author: Yish
 *  Date:   2011/7/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FAIRY_D3D9UTIL_H__
#define __FAIRY_D3D9UTIL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FD3D9Common.h"

//////////////////////////////////////////////////////////////////////////


/** D3D9模块实用函数集
*/
struct FD3D9Util
{
    // 获取指定像素格式的位数
    static size_t GetFormatBits( D3DFORMAT format );
    static size_t GetFormatBits( EPixelFormat format );

    // 获取指定窗口的客户区坐标(相对于屏幕)
    static void GetClientRectEx( HWND hWnd,FRectI& rcClient );

    // 将D3D枚举变量转换为字符串
    static const char* ToString( D3DDEVTYPE devType );
    static const char* ToString( D3DFORMAT format );
    static const char* ToString( DWORD dwDeviceType );
    static const char* ToString( D3DMULTISAMPLE_TYPE msType );

    // 将指定字符串转换为D3D枚举
    static D3DFORMAT StrToD3DFormat( const TCHAR* str );
    static D3DMULTISAMPLE_TYPE StrToMSType( const TCHAR* str );

    // 将D3D格式枚举转换为自己的枚举类型
    static EPixelFormat D3DFormatToFormat( D3DFORMAT format );

    // 将指定的枚举转换为Direct3D类型
    static D3DFORMAT ToD3DType( EPixelFormat format );
    static D3DTEXTUREFILTERTYPE ToD3DType( EFilterOption option );
    static DWORD ToD3DType( EBlendSource src );
    static D3DTEXTUREOP ToD3DType( ETextureOperation operation );
    static D3DTEXTUREADDRESS ToD3DType( ETextureAddress addr );
    static D3DBLEND ToD3DType( EAlphaBlendMode blend );
    static D3DSTENCILOP ToD3DType( EStencilOperation operation );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FD3D9_UTIL_H__