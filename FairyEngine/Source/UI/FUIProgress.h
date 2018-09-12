/*
 * ------------------------------------------------------------------------
 *  Name:   FUIProgress.h
 *  Desc:   本文件定义了UI系统的进度条控件。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_PROGRESS_H__
#define __FUI_PROGRESS_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 进度条控件
*/
class FAIRY_API FUIProgress : public FUIObject
{
public:
    FUIProgress();
    virtual ~FUIProgress() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_PROGRESS_H__