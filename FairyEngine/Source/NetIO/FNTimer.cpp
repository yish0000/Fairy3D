/*
 * ------------------------------------------------------------------------
 *  Name:   FNTimer.cpp
 *  Desc:   ���ļ�ʵ���������ļ�ʱ������
 *  Author: Yish
 *  Date:   2012/5/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FNTimer.h"


namespace FNET {

    // ���徲̬����
    time_t Timer::NOW;
    Timer::ObserverList Timer::Observers;
}