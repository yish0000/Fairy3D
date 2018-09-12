/*
 * ------------------------------------------------------------------------
 *  Name:   F3DOccluderQuery.h
 *  Desc:   本文件定义了一个3D引擎的遮挡查询对象基类。
 *  Author: Yish
 *  Date:   2012/6/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_OCCLUDERQUERY_H__
#define __F3D_OCCLUDERQUERY_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 遮挡查询
*/
class FAIRY_API F3DOccluderQuery
{
public:
    F3DOccluderQuery();
    virtual ~F3DOccluderQuery() {}

    virtual void BeginQuery() = 0;
    virtual void EndQuery() = 0;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_OCCLUDERQUERY_H__