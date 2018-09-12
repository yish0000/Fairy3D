/*
 * ------------------------------------------------------------------------
 *  Name:   FNBase.h
 *  Desc:   ���ļ����������������Ļ����������͡�
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
//// ���������Ϊʲô���͵Ŀ�

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    // Ҫ����Ŀ�����������Ӧ�ĺꡣ
    // ��̬��(_FNETIO_LIB)����̬��(_FNETIO_DLL)
    // ���������������꣬��˵���ǿͻ�Ӧ�ó��������ø���Ŀ
    #ifdef _FNETIO_LIB
        // ����Ϊ��̬��
        #define _FNET_EXPORT
    #else
        // ����Ϊ��̬��
        #ifdef _FNETIO_DLL
            #define _FNET_EXPORT __declspec(dllexport)
        #else
            // �ͻ�Ӧ�ó����������Ŀ
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
// Ԥ����������������

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