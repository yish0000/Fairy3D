/*
 * -----------------------------------------------------------------------
 *  Name:   F3DShaderObject.cpp
 *  Desc:   本文件用于实现引擎的着色器对象基类。
 *  Author: Yish
 *  Date:   2011/1/10
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DShaderObject.h"


/** 构造函数
*/
F3DShaderObject::F3DShaderObject( const AString& name ) : m_sName(name)
{
}

/** 析构函数
*/
F3DShaderObject::~F3DShaderObject(void)
{
}