/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFont.h
 *  Desc:   本文件定义了一个字体对象。
 *  Author: Yish
 *  Date:   2012/1/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_FONT_H__
#define __F3D_FONT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 字体对象
@remarks
	通过FreeType字体库来实现TTF字体的加载。
*/
class FAIRY_API F3DFont : public FGeneralAlloc
{
public:		// Types

	struct TEXTPROP
	{
		wchar_t ch;
	};

public:
    F3DFont();
	virtual ~F3DFont() {}
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_FONT_H__