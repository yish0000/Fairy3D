/*
 * ------------------------------------------------------------------------
 *  Name:   FUIPictureBox.h
 *  Desc:   本文件定义了一个图片框控件。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_PICTUREBOX_H__
#define __FUI_PICTUREBOX_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 图片框控件
*/
class FAIRY_API FUIPictureBox : public FUIObject
{
public:
    FUIPictureBox();
    virtual ~FUIPictureBox() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_PICTUREBOX_H__