/*
 * -----------------------------------------------------------------------
 *	Name:   F3DMaterialManager.cpp
 *  Desc:   本文件用于实现引擎的材质管理器。
 *  Author: Yish
 *  Date:   2010/8/29
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"
#include "F3DMaterialManager.h"


/** Get the singleton
*/
F3DMaterialManager& F3DMaterialManager::GetInstance()
{
	static F3DMaterialManager obj;
	return obj;
}

// 构造函数
F3DMaterialManager::F3DMaterialManager(void)
{
}

// 析构函数
F3DMaterialManager::~F3DMaterialManager(void)
{
}