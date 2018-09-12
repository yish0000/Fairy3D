/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFontManager.cpp
 *  Desc:   本文件实现了引擎的字体管理器。
 *  Author: Yish
 *  Date:   2012/1/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DFontManager.h"


/** Get the singleton.
*/
F3DFontManager& F3DFontManager::GetInstance()
{
	static F3DFontManager obj;
	return obj;
}

/** Constructor.
*/
F3DFontManager::F3DFontManager()
{
}

/** Destructor.
*/
F3DFontManager::~F3DFontManager()
{
}