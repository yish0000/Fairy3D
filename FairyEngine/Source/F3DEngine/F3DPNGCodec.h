/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPNGCodec.h
 *  Desc:   本文件为引擎了一个PNG图像解码器。
 *  Author: Yish
 *  Date:   2010/11/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_PNGCODEC_H__
#define __F3D_PNGCODEC_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DImageCodec.h"

///////////////////////////////////////////////////////////////////////////


/** PNG图像解码器
*/
class FAIRY_API F3DPNGCodec : public F3DImageCodec
{
public:
    F3DPNGCodec(void);
    ~F3DPNGCodec(void);

    // 加载图像文件
    F3DImage* Load( const char* filename );

    // 保存图像的Mipmap到文件
    void SaveMipmap( const char* filename,F3DImage* image,size_t level );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PNGCODEC_H__