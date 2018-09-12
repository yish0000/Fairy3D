/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9Util.cpp
 *  Desc:   ���ļ�Ϊ��Ⱦ�豸ģ��ʵ����ʵ�ú�������
 *  Author: Yish
 *  Date:   2011/7/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FD3D9Util.h"


/** ��ȡָ�����ڵĿͻ�������
@Param ���ڵľ��
@Param Ҫ����Ŀͻ�������
*/
void FD3D9Util::GetClientRectEx( HWND hWnd,FRectI& rcClient )
{
    RECT rc;
    POINT pt;

    ClientToScreen( hWnd,&pt );
    GetClientRect( hWnd,&rc );

    rcClient.m_l = pt.x;
    rcClient.m_t = pt.y;
    rcClient.m_r = pt.x + rc.right;
    rcClient.m_b = pt.y + rc.bottom;
}

/** ��ȡָ�����ظ�ʽ��λ��
@Param ָ�������ظ�ʽ
*/
size_t FD3D9Util::GetFormatBits( D3DFORMAT format )
{
    switch( format )
    {
    case D3DFMT_A2R10G10B10:	return 32;
    case D3DFMT_R8G8B8:			return 24;
    case D3DFMT_A8R8G8B8:		return 32;
    case D3DFMT_X8R8G8B8:		return 32;
    case D3DFMT_R5G6B5:			return 16;
    case D3DFMT_X1R5G5B5:		return 16;
    case D3DFMT_A1R5G5B5:		return 16;
    case D3DFMT_X4R4G4B4:		return 16;
    case D3DFMT_A4R4G4B4:		return 16;
    case D3DFMT_R3G3B2:			return 8;
    case D3DFMT_A8R3G3B2:		return 16;
    default:					return 0;
    }
}

/** ��ȡָ�����ظ�ʽ��λ��
@Param ָ�������ظ�ʽ
*/
size_t FD3D9Util::GetFormatBits( EPixelFormat format )
{
    switch( format )
    {
    case PFT_R8G8B8:        return 24;
    case PFT_A8R8G8B8:      return 32;
    case PFT_A8B8G8R8:      return 32;
    case PFT_X8R8G8B8:      return 32;
    case PFT_X8B8G8R8:      return 32;
    case PFT_R5G6B5:        return 16;
    default:                return 0;
    }
}

/** ��D3Dö��ת��Ϊ�ַ���
*/
const TCHAR* FD3D9Util::ToString( D3DDEVTYPE devType )
{
    switch( devType )
    {
    case D3DDEVTYPE_HAL:	return _T("D3DDEVTYPE_HAL");
    case D3DDEVTYPE_SW:		return _T("D3DDEVTYPE_SW");
    case D3DDEVTYPE_REF:	return _T("D3DDEVTYPE_REF");
    default:				return _T("Unknown DevType");
    }
}

/** ��D3Dö��ת��Ϊ�ַ���
*/
const TCHAR* FD3D9Util::ToString( D3DFORMAT format )
{
    switch( format )
    {
    case D3DFMT_UNKNOWN:		return _T("D3DFMT_UNKNOWN");
    case D3DFMT_R8G8B8:			return _T("D3DFMT_R8G8B8");
    case D3DFMT_A8R8G8B8:		return _T("D3DFMT_A8R8G8B8");
    case D3DFMT_X8R8G8B8:		return _T("D3DFMT_X8R8G8B8");
    case D3DFMT_R5G6B5:			return _T("D3DFMT_R5G6B5");
    case D3DFMT_X1R5G5B5:		return _T("D3DFMT_X1R5G5B5");
    case D3DFMT_A1R5G5B5:		return _T("D3DFMT_A1R5G5B5");
    case D3DFMT_A4R4G4B4:		return _T("D3DFMT_A4R4G4B4");
    case D3DFMT_R3G3B2:			return _T("D3DFMT_R3G3B2");
    case D3DFMT_A8R3G3B2:		return _T("D3DFMT_A8R3G3B2");
    case D3DFMT_X4R4G4B4:		return _T("D3DFMT_X4R4G4B4");
    case D3DFMT_A2B10G10R10:	return _T("D3DFMT_A2B10G10R10");
    case D3DFMT_D16_LOCKABLE:	return _T("D3DFMT_D16_LOCKABLE");
    case D3DFMT_D32:			return _T("D3DFMT_D32");
    case D3DFMT_D15S1:			return _T("D3DFMT_D15S1");
    case D3DFMT_D24S8:			return _T("D3DFMT_D24S8");
    case D3DFMT_D16:			return _T("D3DFMT_D16");
    case D3DFMT_D24X8:			return _T("D3DFMT_D24X8");
    case D3DFMT_D24X4S4:		return _T("D3DFMT_D24X4S4");
    case D3DFMT_VERTEXDATA:		return _T("D3DFMT_VERTEXDATA");
    case D3DFMT_INDEX16:		return _T("D3DFMT_INDEX16");
    case D3DFMT_INDEX32:		return _T("D3DFMT_INDEX32");
    default:					return _T("Unknown format");
    }
}

/** ��D3Dö��ת��Ϊ�ַ���
*/
const TCHAR* FD3D9Util::ToString( DWORD dwDeviceType )
{
    switch( dwDeviceType )
    {
    case D3DCREATE_SOFTWARE_VERTEXPROCESSING:	return _T("SOFTWARE_VP");
    case D3DCREATE_MIXED_VERTEXPROCESSING:		return _T("MIXED_VP");
    case D3DCREATE_HARDWARE_VERTEXPROCESSING:	return _T("HARDWARE_VP");
    case D3DCREATE_PUREDEVICE:					return _T("PURE_HARDWARE_VP");
    default:									return _T("UNKNOWN_VP");
    }
}

/** ��D3Dö��ת��Ϊ�ַ���
*/
const TCHAR* FD3D9Util::ToString( D3DMULTISAMPLE_TYPE msType )
{
    switch( msType )
    {
    case D3DMULTISAMPLE_NONE:           return _T("None");
    case D3DMULTISAMPLE_NONMASKABLE:    return _T("NonMaskable");
    case D3DMULTISAMPLE_2_SAMPLES:      return _T("Level2");
    case D3DMULTISAMPLE_3_SAMPLES:      return _T("Level3");
    case D3DMULTISAMPLE_4_SAMPLES:      return _T("Level4");
    case D3DMULTISAMPLE_5_SAMPLES:      return _T("Level5");
    case D3DMULTISAMPLE_6_SAMPLES:      return _T("Level6");
    case D3DMULTISAMPLE_7_SAMPLES:      return _T("Level7");
    case D3DMULTISAMPLE_8_SAMPLES:      return _T("Level8");
    case D3DMULTISAMPLE_9_SAMPLES:      return _T("Level9");
    case D3DMULTISAMPLE_10_SAMPLES:     return _T("Level10");
    case D3DMULTISAMPLE_11_SAMPLES:     return _T("Level11");
    case D3DMULTISAMPLE_12_SAMPLES:     return _T("Level12");
    case D3DMULTISAMPLE_13_SAMPLES:     return _T("Level13");
    case D3DMULTISAMPLE_14_SAMPLES:     return _T("Level14");
    case D3DMULTISAMPLE_15_SAMPLES:     return _T("Level15");
    case D3DMULTISAMPLE_16_SAMPLES:     return _T("Level16");
    default:                            return _T("Unknown Type!");
    }
}

/** ���ַ���ת��ΪD3DFormat
*/
D3DFORMAT FD3D9Util::StrToD3DFormat( const TCHAR* str )
{
    if( F_Strcmp(str,_T("D3DFMT_R8G8B8")) == 0 )
        return D3DFMT_R8G8B8;
    else if( F_Strcmp(str,_T("D3DFMT_A8R8G8B8")) == 0 )
        return D3DFMT_A8R8G8B8;
    else if( F_Strcmp(str,_T("D3DFMT_X8R8G8B8")) == 0 )
        return D3DFMT_X8R8G8B8;
    else if( F_Strcmp(str,_T("D3DFMT_R5G6B5")) == 0 )
        return D3DFMT_R5G6B5;
    else if( F_Strcmp(str,_T("D3DFMT_X1R5G5B5")) == 0 )
        return D3DFMT_X1R5G5B5;
    else if( F_Strcmp(str,_T("D3DFMT_A1R5G5B5")) == 0 )
        return D3DFMT_A1R5G5B5;
    else if( F_Strcmp(str,_T("D3DFMT_A4R4G4B4")) == 0 )
        return D3DFMT_A4R4G4B4;
    else if( F_Strcmp(str,_T("D3DFMT_R3G3B2")) == 0 )
        return D3DFMT_R3G3B2;
    else if( F_Strcmp(str,_T("D3DFMT_A8R3G3B2")) == 0 )
        return D3DFMT_A8R3G3B2;
    else if( F_Strcmp(str,_T("D3DFMT_X4R4G4B4")) == 0 )
        return D3DFMT_X4R4G4B4;
    else if( F_Strcmp(str,_T("D3DFMT_A2B10G10R10")) == 0 )
        return D3DFMT_A2B10G10R10;
    else if( F_Strcmp(str,_T("D3DFMT_D16_LOCKABLE")) == 0 )
        return D3DFMT_D16_LOCKABLE;
    else if( F_Strcmp(str,_T("D3DFMT_D32")) == 0 )
        return D3DFMT_D32;
    else if( F_Strcmp(str,_T("D3DFMT_D15S1")) == 0 )
        return D3DFMT_D15S1;
    else if( F_Strcmp(str,_T("D3DFMT_D24S8")) == 0 )
        return D3DFMT_D24S8;
    else if( F_Strcmp(str,_T("D3DFMT_D16")) == 0 )
        return D3DFMT_D16;
    else if( F_Strcmp(str,_T("D3DFMT_D24X8")) == 0 )
        return D3DFMT_D24X8;
    else if( F_Strcmp(str,_T("D3DFMT_D24X4S4")) == 0 )
        return D3DFMT_D24X4S4;
    else if( F_Strcmp(str,_T("D3DFMT_VERTEXDATA")) == 0 )
        return D3DFMT_VERTEXDATA;
    else if( F_Strcmp(str,_T("D3DFMT_INDEX16")) == 0 )
        return D3DFMT_INDEX16;
    else if( F_Strcmp(str,_T("D3DFMT_INDEX32")) == 0 )
        return D3DFMT_INDEX32;
    else
        return D3DFMT_UNKNOWN;
}

/** ���ַ���ת��ΪD3Dö��
*/
D3DMULTISAMPLE_TYPE FD3D9Util::StrToMSType( const TCHAR* str )
{
    if( F_Strcmp(str,_T("None")) == 0 )
        return D3DMULTISAMPLE_NONE;
    else if( F_Strcmp(str,_T("NonMaskable")) == 0 )
        return D3DMULTISAMPLE_NONMASKABLE;
    else if( F_Strcmp(str,_T("Level2")) == 0 )
        return D3DMULTISAMPLE_2_SAMPLES;
    else if( F_Strcmp(str,_T("Level3")) == 0 )
        return D3DMULTISAMPLE_3_SAMPLES;
    else if( F_Strcmp(str,_T("Level4")) == 0 )
        return D3DMULTISAMPLE_4_SAMPLES;
    else if( F_Strcmp(str,_T("Level5")) == 0 )
        return D3DMULTISAMPLE_5_SAMPLES;
    else if( F_Strcmp(str,_T("Level6")) == 0 )
        return D3DMULTISAMPLE_6_SAMPLES;
    else if( F_Strcmp(str,_T("Level7")) == 0 )
        return D3DMULTISAMPLE_7_SAMPLES;
    else if( F_Strcmp(str,_T("Level8")) == 0 )
        return D3DMULTISAMPLE_8_SAMPLES;
    else if( F_Strcmp(str,_T("Level9")) == 0 )
        return D3DMULTISAMPLE_9_SAMPLES;
    else if( F_Strcmp(str,_T("Level10")) == 0 )
        return D3DMULTISAMPLE_10_SAMPLES;
    else if( F_Strcmp(str,_T("Level11")) == 0 )
        return D3DMULTISAMPLE_11_SAMPLES;
    else if( F_Strcmp(str,_T("Level12")) == 0 )
        return D3DMULTISAMPLE_12_SAMPLES;
    else if( F_Strcmp(str,_T("Level13")) == 0 )
        return D3DMULTISAMPLE_13_SAMPLES;
    else if( F_Strcmp(str,_T("Level14")) == 0 )
        return D3DMULTISAMPLE_14_SAMPLES;
    else if( F_Strcmp(str,_T("Level15")) == 0 )
        return D3DMULTISAMPLE_15_SAMPLES;
    else if( F_Strcmp(str,_T("Level16")) == 0 )
        return D3DMULTISAMPLE_16_SAMPLES;
    else
        return (D3DMULTISAMPLE_TYPE)-1;
}

/** ��ö������ת��ΪD3D����
*/
D3DFORMAT FD3D9Util::ToD3DType( EPixelFormat format )
{
    switch( format )
    {
    case PFT_R8G8B8:        return D3DFMT_R8G8B8;
    case PFT_A8R8G8B8:      return D3DFMT_A8R8G8B8;
    case PFT_A8B8G8R8:      return D3DFMT_A8B8G8R8;
    case PFT_X8R8G8B8:      return D3DFMT_X8R8G8B8;
    case PFT_X8B8G8R8:      return D3DFMT_X8B8G8R8;
    case PFT_R5G6B5:        return D3DFMT_R5G6B5;
    default:                return D3DFMT_UNKNOWN;
    }
}

/** ��D3DFORMAT����ת��Ϊ�Լ���ö������
*/
EPixelFormat FD3D9Util::D3DFormatToFormat( D3DFORMAT format )
{
    switch( format )
    {
    case D3DFMT_R8G8B8:     return PFT_R8G8B8;
    case D3DFMT_A8R8G8B8:   return PFT_A8R8G8B8;
    case D3DFMT_A8B8G8R8:   return PFT_A8B8G8R8;
    case D3DFMT_X8R8G8B8:   return PFT_X8R8G8B8;
    case D3DFMT_X8B8G8R8:   return PFT_X8B8G8R8;
    case D3DFMT_R5G6B5:     return PFT_R5G6B5;
    default:                return PFT_UNKNOWN;
    }
}



/** ��ö������ת��ΪD3D����
*/
DWORD FD3D9Util::ToD3DType( EBlendSource src )
{
    switch( src )
    {
    case BSTA_CURRENT:      return D3DTA_CURRENT;
    case BSTA_TEXTURE:      return D3DTA_TEXTURE;
    case BSTA_SPECULAR:     return D3DTA_SPECULAR;
    case BSTA_DIFFUSE:      return D3DTA_DIFFUSE;
    default:                return -1;
    }
}

/** ��ö������ת��ΪD3D����
*/
D3DTEXTUREOP FD3D9Util::ToD3DType( ETextureOperation operation )
{
    switch( operation )
    {
    case TOP_DISABLE:               return D3DTOP_DISABLE;
    case TOP_SELECTARG1:            return D3DTOP_SELECTARG1;
    case TOP_SELECTARG2:            return D3DTOP_SELECTARG2;
    case TOP_MODULATE:              return D3DTOP_MODULATE;
    case TOP_MODULATEX2:            return D3DTOP_MODULATE2X;
    case TOP_MODULATEX4:            return D3DTOP_MODULATE4X;
    case TOP_ADD:                   return D3DTOP_ADD;
    case TOP_ADDSIGNED:             return D3DTOP_ADDSIGNED;
    case TOP_ADDSMOOTH:             return D3DTOP_ADDSMOOTH;
    case TOP_SUBTRACT:              return D3DTOP_SUBTRACT;
    case TOP_BLENDDIFFUSEALPHA:     return D3DTOP_BLENDDIFFUSEALPHA;
    case TOP_BLENDTEXTUREALPHA:     return D3DTOP_BLENDTEXTUREALPHA;
    case TOP_BLENDCURRENTALPHA:     return D3DTOP_BLENDCURRENTALPHA;
    case TOP_DOTPRODUCT3:           return D3DTOP_DOTPRODUCT3;
    default:                        return (D3DTEXTUREOP)-1;
    }
}

/** ��ö������ת��ΪD3D����
*/
D3DSTENCILOP FD3D9Util::ToD3DType( EStencilOperation operation )
{
    switch( operation )
    {
    case STOP_KEEP:         return D3DSTENCILOP_KEEP;
    case STOP_ZERO:         return D3DSTENCILOP_ZERO;
    case STOP_REPLACE:      return D3DSTENCILOP_REPLACE;
    case STOP_INCREMENT:    return D3DSTENCILOP_INCR;
    case STOP_DECREMENT:    return D3DSTENCILOP_DECR;
    case STOP_INVERT:       return D3DSTENCILOP_INVERT;
    default:                return (D3DSTENCILOP)-1;
    }
}

/** ��ö������ת��ΪD3D����
*/
D3DTEXTUREADDRESS FD3D9Util::ToD3DType( ETextureAddress addr )
{
    switch( addr )
    {
    case TADDR_WRAP:        return D3DTADDRESS_WRAP;
    case TADDR_MIRROR:      return D3DTADDRESS_MIRROR;
    case TADDR_MIRRORONCE:  return D3DTADDRESS_MIRRORONCE;
    case TADDR_CLAMP:       return D3DTADDRESS_CLAMP;
    case TADDR_BORDER:      return D3DTADDRESS_BORDER;
    default:                return (D3DTEXTUREADDRESS)-1;
    }
}

/** ��ö������ת��ΪD3D����
*/
D3DTEXTUREFILTERTYPE FD3D9Util::ToD3DType( EFilterOption option )
{
    switch( option )
    {
    case FILTER_NONE:           return D3DTEXF_NONE;
    case FILTER_POINT:          return D3DTEXF_POINT;
    case FILTER_LINEAR:         return D3DTEXF_LINEAR;
    case FILTER_ANISOTROPIC:    return D3DTEXF_ANISOTROPIC;
    case FILTER_PYRAMIDALQUAD:  return D3DTEXF_PYRAMIDALQUAD;
    case FILTER_GAUSSIANQUAD:   return D3DTEXF_GAUSSIANQUAD;
    default:                    return (D3DTEXTUREFILTERTYPE)-1;
    }
}

/** ��ö������ת��ΪD3D����
*/
D3DBLEND FD3D9Util::ToD3DType( EAlphaBlendMode blend )
{
    switch( blend )
    {
    case ABM_ZERO:              return D3DBLEND_ZERO;
    case ABM_ONE:               return D3DBLEND_ONE;
    case ABM_SRCCOLOR:          return D3DBLEND_SRCCOLOR;
    case ABM_INVSRCCOLOR:       return D3DBLEND_INVSRCCOLOR;
    case ABM_SRCALPHA:          return D3DBLEND_SRCALPHA;
    case ABM_INVSRCALPHA:       return D3DBLEND_INVSRCALPHA;
    case ABM_DESTCOLOR:         return D3DBLEND_DESTCOLOR;
    case ABM_INVDESTCOLOR:      return D3DBLEND_INVDESTCOLOR;
    case ABM_DESTALPHA:         return D3DBLEND_DESTALPHA;
    case ABM_INVDESTALPHA:      return D3DBLEND_INVDESTALPHA;
    default:                    return (D3DBLEND)-1;
    }
}