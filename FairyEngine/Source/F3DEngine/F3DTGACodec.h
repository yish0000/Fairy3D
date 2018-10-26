/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTGACodec.h
 *  Desc:   本文件为引擎定义了一个TGA图像解码器。
 *  Author: Yish
 *  Date:   11/16/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_TGACODEC_H__
#define __F3D_TGACODEC_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DImageCodec.h"

///////////////////////////////////////////////////////////////////////////


/** TGA文件头
*/
struct STGAHeader
{
    uchar IDLength;
    uchar ColorMapType;
    uchar ImageType;
    uchar FirstEntryIndex[2];
    ushort ColorMapLength;
    uchar ColorMapEntrySize;
    uchar XOrigin[2];
    uchar YOrigin[2];
    ushort ImageWidth;
    ushort ImageHeight;
    uchar PixelDepth;
    uchar ImageDescriptor;
};

/** TGA的文件脚
*/
struct STGAFooter
{
    uint32 ExtensionOffset;
    uint32 DeveloperOffset;
    char Signature[18];
};

/** TGA图像解码器
*/
class FAIRY_API F3DTGACodec : public F3DImageCodec
{
public:
    F3DTGACodec(void);
    ~F3DTGACodec(void);

    // 加载图像文件
    F3DImage* Load( const char* filename );

    // 保存图像的Mipmap到磁盘文件
    void SaveMipmap( const char* filename, F3DImage* image, size_t level );

private:
    // 从虚拟文件流中加载图像对象
    F3DImage* LoadImageFromVFile( FVFile* pFile );

    // 加载压缩了的TGA图像
    FBYTE* LoadCompressedImage( FVFile* pFile, const STGAHeader& header );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TGACODEC_H__