/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Util.h
 *  Desc:   ���ļ�Ϊ��Ⱦ�豸ģ�鶨����һЩʵ�ú�����
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


/** D3D9ģ��ʵ�ú�����
*/
struct FD3D9Util
{
    // ��ȡָ�����ظ�ʽ��λ��
    static size_t GetFormatBits( D3DFORMAT format );
    static size_t GetFormatBits( EPixelFormat format );

    // ��ȡָ�����ڵĿͻ�������(�������Ļ)
    static void GetClientRectEx( HWND hWnd,FRectI& rcClient );

    // ��D3Dö�ٱ���ת��Ϊ�ַ���
    static const char* ToString( D3DDEVTYPE devType );
    static const char* ToString( D3DFORMAT format );
    static const char* ToString( DWORD dwDeviceType );
    static const char* ToString( D3DMULTISAMPLE_TYPE msType );

    // ��ָ���ַ���ת��ΪD3Dö��
    static D3DFORMAT StrToD3DFormat( const TCHAR* str );
    static D3DMULTISAMPLE_TYPE StrToMSType( const TCHAR* str );

    // ��D3D��ʽö��ת��Ϊ�Լ���ö������
    static EPixelFormat D3DFormatToFormat( D3DFORMAT format );

    // ��ָ����ö��ת��ΪDirect3D����
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