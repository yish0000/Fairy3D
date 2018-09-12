/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPostManager.cpp
 *  Desc:   本文件实现了F3D引擎的后期特效管理器。
 *  Author: Yish
 *  Date:   2012/4/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DPostManager.h"


/** Get the singleton.
*/
F3DPostManager& F3DPostManager::GetInstance()
{
	static F3DPostManager obj;
	return obj;
}

/** 构造函数
*/
F3DPostManager::F3DPostManager()
{
}

/** 析构函数
*/
F3DPostManager::~F3DPostManager()
{
}