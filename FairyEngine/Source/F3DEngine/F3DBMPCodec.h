/*
 * ------------------------------------------------------------------------
 *  Name:   F3DBMPCodec.h
 *  Desc:   本文件定义了一个解析位图的解码器。
 *  Author: Yish
 *  Date:   2010/11/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_BMPCODEC_H__
#define __F3D_BMPCODEC_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DImageCodec.h"

///////////////////////////////////////////////////////////////////////////


/** 位图解码器
@remarks
@note
*/
class FAIRY_API F3DBMPCodec : public F3DImageCodec
{
public:
    F3DBMPCodec(void);
    ~F3DBMPCodec(void);

    // 加载图像文件
    F3DImage* Load( const char* filename );

    // 保存图像的Mipmap到磁盘文件
    void SaveMipmap( const char* filename, F3DImage* image, size_t level );

private:

    // 从虚拟文件中加载图像对象
    F3DImage* LoadImageFromVFile( FVFile* pFile );
    // 针对压缩模式1的解压缩算法
    void Decompress8Bit( fbyte*& data, size_t bufSize, uint32 width, uint32 height, uint32 pitch );
    // 针对压缩模式2的解压缩算法
    void Decompress4Bit( fbyte*& data, size_t bufSize, uint32 width, uint32 height, uint32 pitch );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_BMPCODEC_H__