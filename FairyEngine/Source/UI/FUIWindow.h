/*
 * ------------------------------------------------------------------------
 *  Name:   FUIWindow.h
 *  Desc:   本文件为UI系统定义了一个窗口基类，每个控件元素都需从此类派生。
 *  Author: Yish
 *  Date:   2010/8/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_WINDOW_H__
#define __FUI_WINDOW_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"
#include "FUIObject.h"

///////////////////////////////////////////////////////////////////////////

/** 窗口类
@remarks
@note
*/
class FAIRY_API FUIWindow : public FGeneralAlloc
{
public:     // Types

    typedef std::vector<FUIObject*> ObjectList;
    typedef std::map<AString, FUIObject*> ObjectMap;

public:
    FUIWindow();
    virtual ~FUIWindow() {}

    bool Load( const char* filename );
    void Save( const char* filename );

    FUIObject* GetUIObject( const char* name );

protected:
    ObjectList m_Objects;
    ObjectMap m_ObjectMap;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_WINDOW_H__