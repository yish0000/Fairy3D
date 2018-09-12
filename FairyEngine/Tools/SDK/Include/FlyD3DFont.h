//------------------------------------------------------------------------
// Name: FlyD3DFont.h
// Desc: This file define a class to act as the Direct3D font object.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DFONT_H__
#define __FLY_D3DFONT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyFont.h"

//////////////////////////////////////////////////////////////////////////

class FlyD3DFont : public FlyFont
{
protected:
    ID3DXFont* m_pD3DFont;
    LPDIRECT3DDEVICE9 m_pd3dDevice;

public:
    // Constructor and destructor.
    FlyD3DFont( LPDIRECT3DDEVICE9 pd3dDevice );
    ~FlyD3DFont(void);

    HRESULT CreateFont( const char* cFontType,int nWeight,bool bItalic, DWORD dwSize );
    HRESULT RenderText( int x,int y,const FLYCOLOR& clr,char* cText, ... );
    void DestroyFont(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DFONT_H__