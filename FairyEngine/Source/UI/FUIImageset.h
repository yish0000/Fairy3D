/*
 * ------------------------------------------------------------------------
 *  Name:   FUIImageset.h
 *  Desc:   图像集对象：每个图像集是一张纹理，而图像则是纹理上的一个矩形，
 *          图像是用于UI绘制时的基本元素。
 *  Author: Yish
 *  Date:   2010/8/21
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_IMAGESET_H__
#define __FUI_IMAGESET_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"

///////////////////////////////////////////////////////////////////////////

class F2DSprite;

/** 图片集
@remarks
    每个图片集包含了多个图片元素，指定一个个的矩形位置标记多个图片元素
@note
*/
class FAIRY_API FUIImageset : public FGeneralAlloc
{
public:

    struct Element
    {
        int left;
        int top;
        int right;
        int bottom;
    };

    typedef std::vector<Element> ElementList;

public:
    FUIImageset();
    ~FUIImageset();

protected:
    F2DSprite* m_p2DSprite;
    ElementList m_ImageElements;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_IMAGESET_H__