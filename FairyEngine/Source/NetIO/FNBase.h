/*
 * ------------------------------------------------------------------------
 *  Name:   FNBase.h
 *  Desc:   本文件定义了引擎网络库的基本数据类型。
 *  Author: Yish
 *  Date:   2012/2/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FNET_BASE_H__
#define __FNET_BASE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    #include <winsock2.h>
#else
    // Linux and others
#endif

#include "FBase.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//// 将引擎编译为什么类型的库

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    // 要在项目设置里添加相应的宏。
    // 静态库(_FNETIO_LIB)，动态库(_FNETIO_DLL)
    // 如果不添加这两个宏，则说明是客户应用程序在引用该项目
    #ifdef _FNETIO_LIB
        // 编译为静态库
        #define _FNET_EXPORT
    #else
        // 编译为动态库
        #ifdef _FNETIO_DLL
            #define _FNET_EXPORT __declspec(dllexport)
        #else
            // 客户应用程序引入该项目
            #if defined(__MINGW32__)
                #define _FNET_EXPORT
            #else
                #define _FNET_EXPORT __declspec(dllimport)
            #endif
        #endif
    #endif
#elif (FAIRY_PLATFORM == FAIRY_PLATFORM_LINUX) || (FAIRY_PLATFORM == FAIRY_PLATFORM_MACOS)
    #ifdef _FNET_GCC_VISIBILITY
        #define _FNET_EXPORT __attribute__((visibility(default)))
    #else
        #define _FNET_EXPORT
    #endif
#endif

///////////////////////////////////////////////////////////////////////////
// 预定义网络库的所有类

namespace FNET {

    class Octets;
    class OctetsStream;
    class Marshal;
    class StreamIO;
    class DiagramIO;
    class ActiveIO;
    class PassiveIO;
    class Session;
    class ConnectedIO;
    class PollIO;
    class PollControl;
    class NetIO;
    class Protocol;
    class NetSession;
    class NetManager;
    class Rpc;
    class RpcData;
    class Timer;
}

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FNET_BASE_H__