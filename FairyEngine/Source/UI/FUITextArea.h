/*
 * ------------------------------------------------------------------------
 *  Name:   FUITextArea.h
 *  Desc:   本文件定义了一个文本域控件。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_TEXTAREA_H__
#define __FUI_TEXTAREA_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 文本域控件
*/
class FAIRY_API FUITextArea : public FUIObject
{
public:
    FUITextArea();
    virtual ~FUITextArea() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_TEXTAREA_H__