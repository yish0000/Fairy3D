/*
 * ------------------------------------------------------------------------
 *  Name:   FUIListBox.h
 *  Desc:   本文件定义了一个列表框控件。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_LISTBOX_H__
#define __FUI_LISTBOX_H__

//// HERADERS OF THIS FILE ////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 列表框控件
*/
class FAIRY_API FUIListBox : public FUIObject
{
public:
    FUIListBox();
    virtual ~FUIListBox() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_LISTBOX_H__