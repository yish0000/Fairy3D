/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFontManager.h
 *  Desc:   本文件定义了引擎的字体管理器。
 *  Author: Yish
 *  Date:   2012/1/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_FONTMANAGER_H__
#define __F3D_FONTMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DFont.h"

///////////////////////////////////////////////////////////////////////////


/** 字体管理器
@remarks
*/
class FAIRY_API F3DFontManager
{
public:		// Types

public:
    ~F3DFontManager();

public:

    static F3DFontManager& GetInstance();

private:
	F3DFontManager();
	F_DISABLE_COPY_ASSIGNMENT(F3DFontManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_FONTMANAGER_H__