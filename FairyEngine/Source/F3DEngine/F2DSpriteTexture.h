/*
* ------------------------------------------------------------------------
*  Name:   F2DSpriteTexture.h
*  Desc:   本文件定义了F3D引擎的二维精灵纹理对象。
*  Author: Yish
*  Date:   2012/4/4
*  ----------------------------------------------------------------------
*  CopyRight (C) YishSoft. All right Observed.
* ------------------------------------------------------------------------
*/


#ifndef __F2D_SPRITETEXTURE_H__
#define __F2D_SPRITETEXTURE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTexture.h"

///////////////////////////////////////////////////////////////////////////


/** 二维精灵纹理
@remarks
    二维精灵有时候需要一张很大的纹理，有可能超出硬件的限制，该类用于解决
    硬件对纹理大小的限制，该类将大纹理分成小块来管理，但对于用户来说，这些是
    无需关心的，可以作为一张纹理通过UV坐标来索引某一部分。
*/
class FAIRY_API F2DSpriteTexture : public FGeneralAlloc
{
public:
    F2DSpriteTexture();
    virtual ~F2DSpriteTexture() {};
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F2D_SPRITETEXTURE_H__