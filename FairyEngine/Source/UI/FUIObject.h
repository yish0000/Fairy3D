/*
 * ------------------------------------------------------------------------
 *  Name:   FUIObject.h
 *  Desc:   本文件定义了UI系统中控件的基类。
 *  Author: Yish
 *  Date:   2011/11/19
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_OBJECT_H__
#define __FUI_OBJECT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUICommon.h"

///////////////////////////////////////////////////////////////////////////

class FUIWindow;

/** UI对象
*/
class FAIRY_API FUIObject : public FGeneralAlloc
{
public:
    FUIObject();
    virtual ~FUIObject() {};

	FUI_OBJECT_TYPE GetType() const { return m_Type; }

protected:
	FUI_OBJECT_TYPE m_Type;
    FUIWindow* m_pParent;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_OBJECT_H__