/*
 * ------------------------------------------------------------------------
 *  Name:   F3DDDSCodec.h
 *  Desc:   本文件为引擎定义了一个DDS文件解码器。
 *  Author: Yish
 *  Date:   2010/11/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_DDSCODEC_H__
#define __F3D_DDSCODEC_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DImageCodec.h"

///////////////////////////////////////////////////////////////////////////


/** DDS图像解码器
*/
class FAIRY_API F3DDDSCodec : public F3DImageCodec
{
public:
    F3DDDSCodec(void);
    ~F3DDDSCodec(void);

    // 从本地磁盘中加载图像
    F3DImage* Load( const char* filename );

    // 保存图像的Mipmap到文件
    void SaveMipmap( const char* filename,F3DImage* image,size_t level );

private:
    // 从虚拟文件流中加载图像对象
    F3DImage* LoadImageFromVFile( FVFile* pFile );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_DDSCODEC_H__