/*
 * ------------------------------------------------------------------------
 *  Name:   FNSecure.cpp
 *  Desc:   本文件实现了网络库的数据加密功能。
 *  Author: Yish
 *  Date:   2012/3/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include <time.h>
#include "FNSecure.h"
#include "FCRC32.h"
#include "FMD5.h"


namespace FNET {

    // 定义静态变量
    Secure::SecureMap Secure::map;

    ///////////////////////////////////////////////////////////////////////////
    // 填充随机数

    bool RandomSecure::Init()
    {
        srand( time(0) );
        return true;
    }

    Octets& RandomSecure::Update( Octets& o )
    {
        for( size_t i=0;i<o.size();++i )
        {
            ((uchar*)o.begin())[i] = rand() & 0xff;
        }

        return o;
    }

    ///////////////////////////////////////////////////////////////////////////
    // MD5哈希加密法
}