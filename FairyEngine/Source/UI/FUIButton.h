﻿/*
 * ------------------------------------------------------------------------
 *  Name:   FUIButton.h
 *  Desc:   本文件定义了一个按钮控件。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_BUTTON_H__
#define __FUI_BUTTON_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 按钮控件
*/
class FAIRY_API FUIButton : public FUIObject
{
public:
    FUIButton();
    virtual ~FUIButton() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_BUTTON_H__