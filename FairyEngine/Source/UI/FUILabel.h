/*
 * ------------------------------------------------------------------------
 *  Name:   FUILabel.h
 *  Desc:   本文件定义了UI系统的标签控件。
 *  Author: Yish
 *  Date:   2012/3/5
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_LABEL_H__
#define __FUI_LABEL_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 静态标签
*/
class FAIRY_API FUILabel : public FUIObject
{
public:
    FUILabel();
    virtual ~FUILabel() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_LABEL_H__