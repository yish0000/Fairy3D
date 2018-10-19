/*
 * ------------------------------------------------------------------------
 *  Name:   F3DResource.cpp
 *  Desc:   本文件实现了引擎的资源对象基类。
 *  Author: Yish
 *  Date:   2012/8/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "F3DPCH.h"
#include "F3DResource.h"

/** 构造函数
*/
F3DResource::F3DResource()
{
}

/** 析构函数
*/
F3DResource::~F3DResource()
{
}

/** 用于资源排序的运算符
*/
bool F3DResource::operator < ( const F3DResource& res )
{
    return true;
}