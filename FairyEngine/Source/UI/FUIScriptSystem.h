/*
 * ------------------------------------------------------------------------
 *  Name:   FUIScriptSystem.h
 *  Desc:   本文件定义了一个UI模块的脚本系统基类。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_SCRIPTSYSTEM_H__
#define __FUI_SCRIPTSYSTEM_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"

///////////////////////////////////////////////////////////////////////////

/** 脚本系统基类
*/
class FAIRY_API FUIScriptSystem
{
public:
    FUIScriptSystem();
    virtual ~FUIScriptSystem() {};

    void Tick( uint32 nTick );

    bool ExecuteFile( const char* szFile );
    bool ExecuteString( const char* szBuffer );

    void RegisterAPI( const char* szLib,const char* szAPI );
    bool Call( const char* szFunc );


};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_SCRIPTSYSTEM_H__