/*
 * ------------------------------------------------------------------------
 *  Name:   FNTimer.cpp
 *  Desc:   本文件实现了网络库的计时器对象。
 *  Author: Yish
 *  Date:   2012/5/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNTimer.h"


namespace FNET {

    // 定义静态变量
    time_t Timer::NOW;
    Timer::ObserverList Timer::Observers;
}