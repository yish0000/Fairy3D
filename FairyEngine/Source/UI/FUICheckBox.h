/*
 * ------------------------------------------------------------------------
 *  Name:   FUICheckBox.h
 *  Desc:   本文件定义了一个复选框控件。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_CHECKBOX_H__
#define __FUI_CHECKBOX_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 复选框控件
*/
class FAIRY_API FUICheckBox : public FUIObject
{
public:
    FUICheckBox();
    virtual ~FUICheckBox() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_CHECKBOX_H__