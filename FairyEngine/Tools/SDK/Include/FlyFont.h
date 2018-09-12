//------------------------------------------------------------------------
// Name: FlyFont.h
// Desc: This file define a font class for engine, It have two kinds,
//       TrueType system font and bitmap font.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_FONT_H__
#define __FLY_FONT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


/*
* This algorithm is from the OGRE engine source code.
*/
class _FLY_EXPORT FlyFont
{
public:
    enum FontType { FONT_SYSFONT,FONT_IMAGE };

protected:
    FontType m_FontType;         // Type of the font object.

public:
    // Constructor and destructor
    FlyFont(void) { m_FontType = FONT_SYSFONT; }
    virtual ~FlyFont(void) { /* Nothing to do! */ };

    virtual HRESULT CreateFont( const char* cFontType,int nWeight,bool bItalic,
        DWORD dwSize ) = 0;
    virtual HRESULT RenderText( int x,int y,const FLYCOLOR& clr,char* cText, ... ) = 0;
    virtual void DestroyFont(void) = 0;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_FONT_H__