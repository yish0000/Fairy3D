/*
 * -----------------------------------------------------------------------
 *  Name:   FlyShaderObject.cpp
 *  Desc:   本文件用于实现引擎的着色器对象基类。
 *  Author: Yish
 *  Date:   2011/1/10
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FlyPrecompile.h"

#include "FlyShaderObject.h"


/** 构造函数
*/
FlyShaderObject::FlyShaderObject( const FString& name ) : m_sName(name)
{
}

/** 析构函数
*/
FlyShaderObject::~FlyShaderObject(void)
{
}