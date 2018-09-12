/*
 * ------------------------------------------------------------------------
 *  Name:   FUIRadioButton.h
 *  Desc:   本文件定义了一个单选框控件。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_RADIOBUTTON_H__
#define __FUI_RADIOBUTTON_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 单选框控件
*/
class FAIRY_API FUIRadioButton : public FUIObject
{
public:
    FUIRadioButton();
    virtual ~FUIRadioButton() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_RADIOBUTTON_H__