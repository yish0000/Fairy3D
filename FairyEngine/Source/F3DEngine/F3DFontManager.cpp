/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFontManager.cpp
 *  Desc:   ���ļ�ʵ��������������������
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