/*
 * ------------------------------------------------------------------------
 *  Name:   F3DShaderManager.cpp
 *  Desc:   本文件为引擎实现了一个着色器管理器。
 *  Author: Yish
 *  Date:   2011/5/21
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DShaderManager.h"


/** Get the singleton.
*/
F3DShaderManager& F3DShaderManager::GetInstance()
{
	static F3DShaderManager obj;
	return obj;
}

/** 构造函数
*/
F3DShaderManager::F3DShaderManager(void)
{
}

/** 析构函数
*/
F3DShaderManager::~F3DShaderManager(void)
{
}