/*
 * ------------------------------------------------------------------------
 *  Name:   FUIEditBox.h
 *  Desc:   本文件定义了一个文本框控件。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_EDITBOX_H__
#define __FUI_EDITBOX_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 文本框控件
*/
class FAIRY_API FUIEditBox : public FUIObject
{
public:
    FUIEditBox();
    virtual ~FUIEditBox() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_EDITBOX_H__