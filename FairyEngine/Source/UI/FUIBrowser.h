/*
 * ------------------------------------------------------------------------
 *  Name:   FUIBrowser.h
 *  Desc:   本文件封装了一个浏览器模块，用于在FUI中绘制网页。
 *  Author: Yish
 *  Date:   2012/3/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_BROWSER_H__
#define __FUI_BROWSER_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 浏览器控件
*/
class FAIRY_API FUIBrowser : public FUIObject
{
public:
    FUIBrowser();
    virtual ~FUIBrowser() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_BROWSER_H__