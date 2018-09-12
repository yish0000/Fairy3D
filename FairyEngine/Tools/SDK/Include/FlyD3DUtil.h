//------------------------------------------------------------------------
// Name: FlyD3DUtil.h
// Desc: This file define a class contain some static util functions.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DUTIL_H__
#define __FLY_D3DUTIL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////

class FlyD3DUtil
{
public:
    // Constructor and destructor.
    FlyD3DUtil(void)  { /* Nothing to do! */ };
    ~FlyD3DUtil(void) { /* Nothing to do! */ };

    static UINT GetFormatBits( D3DFORMAT format );
    static const char* D3DDevTypeToString( D3DDEVTYPE devType );
    static const char* D3DFormatToString( D3DFORMAT format );
    static D3DFORMAT StringToD3DFormat( const char* cString );
    static const char* BehaviorTypeToString( DWORD vpt );
    static D3DFORMAT FormatToD3DFormat( PIXELFORMAT format );
    static PIXELFORMAT D3DFormatToFormat( D3DFORMAT format );
    static UINT GetFormatBits( PIXELFORMAT format );
    static void GetClientRectEx( HWND hWnd,RECT* pRect );

    // Engine to Direct3D Mapping functions.
    static D3DTEXTUREFILTERTYPE GetD3DType( FILTEROPTION option );
    static DWORD GetD3DType( BLENDSOURCE src );
    static D3DTEXTUREOP GetD3DType( TEXTUREOPERATION operation );
    static D3DTEXTUREADDRESS GetD3DType( TEXTUREADDRESS addr );
    static D3DBLEND GetD3DType( SCENEBLENDMODE blend );
    static D3DSTENCILOP GetD3DType( STENCILOPERATION operation );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DUTIL_H__
