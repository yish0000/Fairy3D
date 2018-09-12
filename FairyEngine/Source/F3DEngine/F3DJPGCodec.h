/*
 * ------------------------------------------------------------------------
 *  Name:   F3DJPGCodec.h
 *  Desc:   本文件为引擎定义了一个JPEG图像解码器。
 *  Author: Yish
 *  Date:   2010/11/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_JPGCODEC_H__
#define __F3D_JPGCODEC_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DImageCodec.h"

///////////////////////////////////////////////////////////////////////////


/** JPEG图像解码器
*/
class FAIRY_API F3DJPGCodec : public F3DImageCodec
{

#ifdef USE_IJL_LIB
private:
    F_DLIB_HANDLE m_hDll;   // Intel JPEG 动态库句柄
#endif

public:
    F3DJPGCodec(void);
    ~F3DJPGCodec(void);

    // 从本地磁盘中加载图像
    F3DImage* Load( const char* filename );

    // 保存图像的Mipmap到文件
    void SaveMipmap( const char* filename,F3DImage* image,size_t level );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_JPGCODEC_H__