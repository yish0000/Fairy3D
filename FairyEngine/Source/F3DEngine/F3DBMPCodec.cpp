/*
 * ------------------------------------------------------------------------
 *  Name:   F3DBMPCodec.cpp
 *  Desc:   本文件为引擎实现了一个位图解码器。
 *  Author: Yish
 *  Date:   2010/11/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DBMPCodec.h"
#include "F3DPixelUtil.h"


///////////////////////////////////////////////////////////////////////////
// 位图的数据结构

struct BMPFileHeader
{
    ushort  bfType;
    uint32  bfSize;
    ushort  bfReserved1;
    ushort  bfReserved2;
    uint32  bfOffBits;
};

struct BMPInfoHeader
{
    uint32  biSize;
    int     biWidth;
    int     biHeight;
    ushort  biPlanes;
    ushort  biBitCount;
    uint32  biCompression;
    uint32  biSizeImage;
    int     biXPelsPerMeter;
    int     biYPelsPerMeter;
    uint32  biClrUsed;
    uint32  biClrImportant;
};

///////////////////////////////////////////////////////////////////////////

/** 构造函数
*/
F3DBMPCodec::F3DBMPCodec(void)
{
    m_sCodecName = "BMP";
}

/** 析构函数
*/
F3DBMPCodec::~F3DBMPCodec(void)
{
}

/** 加载图像文件
@Param 图像文件的路径
*/
F3DImage* F3DBMPCodec::Load( const char* filename )
{
	FVFile file;
	if( !file.Open(filename,FVFile::VFILE_OPENEXIST) )
	{
		FLOG_WARNINGF( "F3DBMPCodec::Load, Open image file (%s) failed!",filename );
		return NULL;
	}

    // 从虚拟文件流加载图像
    return LoadImageFromVFile( &file );
}

/** 从虚拟文件中加载图像数据
@Param 虚拟文件的指针
*/
F3DImage* F3DBMPCodec::LoadImageFromVFile( FVFile* pFile )
{
    BMPFileHeader bmpHead;
    BMPInfoHeader infoHead;

    // 读取文件头
    pFile->Read( &bmpHead,sizeof(BMPFileHeader) );

    // 检查是否为一个有效的位图文件
    if( bmpHead.bfType != 0x4d42 )
    {
		FLOG_WARNING( "F3DBMPCodec::LoadImageFromVFile, Invalid bitmap format, Failed to load!" );
		return NULL;
    }

    // 读取信息头
    pFile->Read( &infoHead,sizeof(BMPInfoHeader) );

    if( infoHead.biCompression > 2 )
    {
		FLOG_WARNING( "F3DBMPCodec::LoadImageFromVFile, Failed to load, Invalid compress format!" );
		return NULL;
    }

    // 读取调色板数据(如果存在)
    size_t pos = pFile->Tell();
    size_t paletteSize = (bmpHead.bfOffBits - pos);

    FBYTE* paletteData = NULL;
    if( paletteSize > 0 )
    {
        paletteData = new FBYTE[paletteSize];
        pFile->Read( paletteData,paletteSize );
    }

    if( !bmpHead.bfSize )
    {
        // 有些图像编辑软件并不写入位图数据的大小，
        // 我们尝试着自行计算。
        bmpHead.bfSize = static_cast<uint32>(pFile->GetFileSize()) - bmpHead.bfOffBits;
    }

    // 将位图数据按照4字节对齐
    bmpHead.bfSize += 4 - (bmpHead.bfSize % 4);

    float t = infoHead.biWidth * (infoHead.biBitCount / 8.0f);
    size_t widthBytes = (size_t)t;
    t -= widthBytes;
    if( t != 0 ) widthBytes++;

    size_t lineData = widthBytes + (4 - widthBytes % 4) % 4;
    size_t pitch = lineData - widthBytes;

    // 读取像素数据
    FBYTE* bmpData = new FBYTE[bmpHead.bfSize];
    pFile->Seek( bmpHead.bfOffBits,FVFile::START );
    pFile->Read( bmpData,bmpHead.bfSize );

    // 如果需要，则对数据进行解压缩
    switch( infoHead.biCompression )
    {
    case 1:
        Decompress8Bit( bmpData, bmpHead.bfSize, infoHead.biWidth, infoHead.biHeight, (uint32)pitch );
        break;
    case 2:
        Decompress4Bit( bmpData, bmpHead.bfSize, infoHead.biWidth, infoHead.biHeight, (uint32)pitch );
        break;
    }

    F3DImage* image = NULL;
    switch( infoHead.biBitCount )
    {
    case 1:
        image = new F3DImage( infoHead.biWidth,infoHead.biHeight,PFT_A1R5G5B5 );
        F3D_1BitToARGB1555( bmpData,(ushort*)image->GetImageData(),infoHead.biWidth,
            infoHead.biHeight,pitch,true );
        break;
    case 4:
        image = new F3DImage( infoHead.biWidth,infoHead.biHeight,PFT_A1R5G5B5 );
        F3D_4BitToARGB1555( bmpData,(ushort*)image->GetImageData(),infoHead.biWidth,
            infoHead.biHeight,(uint32*)paletteData,pitch,true );
        break;
    case 8:
        image = new F3DImage( infoHead.biWidth,infoHead.biHeight,PFT_A1R5G5B5 );
        F3D_8BitToARGB1555( bmpData,(ushort*)image->GetImageData(),infoHead.biWidth,
            infoHead.biHeight,(uint32*)paletteData,pitch,true );
        break;
    case 16:
        image = new F3DImage( infoHead.biWidth,infoHead.biHeight,PFT_A1R5G5B5 );
        F3D_16BitToImage( bmpData,image->GetImageData(),infoHead.biWidth,infoHead.biHeight,pitch,true );
        break;
    case 24:
        image = new F3DImage( infoHead.biWidth,infoHead.biHeight,PFT_R8G8B8 );
        F3D_24BitToImage( bmpData,image->GetImageData(),infoHead.biWidth,infoHead.biHeight,pitch,true,false );
        break;
    case 32:
        image = new F3DImage( infoHead.biWidth,infoHead.biHeight,PFT_A8R8G8B8 );
        F3D_32BitToImage( bmpData,image->GetImageData(),infoHead.biWidth,infoHead.biHeight,pitch,true );
        break;
    }

    // 销毁临时数据
    F_SAFE_DELETE_ARRAY( paletteData );
    F_SAFE_DELETE_ARRAY( bmpData );

    return image;
}

/** 保存指定图像的Mipmap
@Param 要保存图像文件的路径
@Param 指向图像对象的指针
@Param 指定的Mipmap层级
*/
void F3DBMPCodec::SaveMipmap( const char* filename, F3DImage* image, size_t level )
{
    FBYTE* destBuf;
    EPixelFormat destFormat;
    size_t destSize, pxSize;

    // 获取源图像的一些信息
    uint32 width = image->GetWidth( level );
    uint32 height = image->GetHeight( level );
    EPixelFormat srcFormat = image->GetPixelFormat();

    if( F3D_HaveAlpha(srcFormat) )
        destFormat = PFT_A8R8G8B8;
    else
        destFormat = PFT_R8G8B8;

    // 将数据转换为指定像素格式
    pxSize = F3D_PixelSize( destFormat );
    destSize = width*height*pxSize;
    destBuf = new FBYTE[destSize];
    F3D_ConvertPixelFormat( image->GetImageData(level),srcFormat,destBuf,destFormat,width,height );

	FVFile file;
	if( !file.Open(filename,FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
    {
        delete[] destBuf;
		FLOG_WARNINGF( "F3DBMPCodec::SaveMipmap, Create the image file (%s) failed!",filename );
		return;
    }

    // 开始写入位图数据
    long bmofs;
    BMPFileHeader bmpHead;
    BMPInfoHeader infoHead;

    // 先写入一个空的文件头(占位置)
	file.Write( &bmpHead,sizeof(BMPFileHeader) );

    // 写入位图的信息头
    infoHead.biSize = sizeof(BMPInfoHeader);
    infoHead.biWidth = (int)width;
    infoHead.biHeight = (int)height;
    infoHead.biPlanes = 1;
    infoHead.biBitCount = (ushort)pxSize * 8;
    infoHead.biCompression = 0L;
    infoHead.biSizeImage = (uint32)(width * height);
    infoHead.biXPelsPerMeter = 0;
    infoHead.biYPelsPerMeter = 0;
    infoHead.biClrUsed = 0;
    infoHead.biClrImportant = 0;
	file.Write( &infoHead,sizeof(BMPInfoHeader) );

    // 像素数据的偏移
	bmofs = (long)file.Tell();

    for( size_t i=height-1;i>=0;i-- )
    {
        FBYTE* pData = &destBuf[width*pxSize*i];

        size_t nBytes;
		file.Write( pData,width*pxSize );

        if( pxSize == 3 )
        {
            nBytes = ((width * 3) + 3) / 4;
            nBytes *= 4;
            nBytes -= width * 3;
            while( nBytes-- > 0 ) file << '\0';
        }
    }

    // 重新写入文件头
    bmpHead.bfType = 0x4d42;
    bmpHead.bfSize = (uint32)file.Tell();
    bmpHead.bfOffBits = bmofs;

	file.Seek( 0,FVFile::START );
    file.Write( &bmpHead,sizeof(BMPFileHeader) );
    file.Close();

    // 回收分配的内存
    delete[] destBuf;
}

/** 针对压缩模式1的解压缩算法
*/
void F3DBMPCodec::Decompress8Bit( FBYTE*& data, size_t bufSize, uint32 width, uint32 height, uint32 pitch )
{
    FBYTE* p = data;
    FBYTE* newBmp = new FBYTE[(width+pitch)*height];
    FBYTE* d = newBmp;
    FBYTE* destEnd = newBmp + (width+pitch) * height;
    size_t line = 0;

    while( (size_t)(data - p) < bufSize && d < destEnd )
    {
        if( *p == 0 )
        {
            p++;

            switch( *p )
            {
            case 0:     // 该行的末尾
                p++; line++;
                d = newBmp + (line * (width + pitch));
                break;
            case 1:     // 整个位图的结尾
                delete[] data;
                data = newBmp;
                return;
            case 2:
                p++; d += *p;
                p++; d += (*p) * (width + pitch);
                p++;
                break;
            default:
                {
                    size_t count = (size_t)*p; p++;
                    size_t readAdditional = (2 - count % 2) % 2;

                    size_t i;
                    for( i=0;i<count;i++ )
                    {
                        *d = *p;
                        p++; d++;
                    }

                    for( i=0;i<readAdditional;i++ )
                        p++;
                }
            }
        }
        else
        {
            size_t count = (size_t)*p; p++;
            FBYTE color = *p; p++;
            for( size_t i=0;i<count;i++ )
            {
                *d = color;
                d++;
            }
        }
    }

    delete[] data;
    data = newBmp;
}

/** 针对压缩模式2的解压缩算法
*/
void F3DBMPCodec::Decompress4Bit( FBYTE*& data, size_t bufSize, uint32 width, uint32 height, uint32 pitch )
{
    size_t lineWidth = (width + 1) / 2 + pitch;
    FBYTE* p = data;
    FBYTE* newBmp = new FBYTE[lineWidth*height];
    FBYTE* d = newBmp;
    FBYTE* destEnd = newBmp + lineWidth * height;
    size_t line = 0;
    size_t shift = 4;

    while( (size_t)(data-p) < bufSize && d < destEnd )
    {
        if( *p == 0 )
        {
            p++;

            switch( *p )
            {
            case 0:     // 像素的末尾
                p++; line++;
                d = newBmp + (line * lineWidth);
                shift = 4;
                break;
            case 1:     // 位图的末尾
                delete[] data;
                data = newBmp;
                return;
            case 2:
                {
                    p++;
                    size_t x = (size_t)*p; p++;
                    size_t y = (size_t)*p; p++;
                    d += x / 2 + y * lineWidth;
                    shift = (x % 2 == 0) ? 4 : 0;
                    break;
                }
            default:
                {
                    size_t count = (size_t)*p; p++;
                    size_t readAdditional = (2 - count % 2) % 2;
                    size_t readShift = 4;

                    size_t i;
                    for( i=0;i<count;i++ )
                    {
                        size_t color = ((size_t)*p >> readShift) & 0x0f;
                        readShift -= 4;
                        if( readShift < 0 )
                        {
                            ++(*p);
                            readShift = 4;
                        }

                        FBYTE mask = (FBYTE)(0x0f << shift);
                        *d = (*d & (~mask)) | (FBYTE)((color << shift) & mask);

                        shift -= 4;
                        if( shift < 0 )
                        {
                            shift = 4;
                            d++;
                        }
                    }

                    for( i=0;i<readAdditional;i++ )
                        p++;
                }
            }
        }
        else
        {
            size_t count = (size_t)*p; p++;
            size_t color1 = (size_t)*p; color1 = color1 & 0x0f;
            size_t color2 = (size_t)*p; color2 = (color2 >> 4) & 0x0f;
            p++;

            for( size_t i=0;i<count;i++ )
            {
                FBYTE mask = (FBYTE)(0x0f << shift);
                FBYTE toSet = (FBYTE)((shift == 0 ? color1 : color2) << shift);
                *d = (*d & (~mask)) | (toSet & mask);

                shift -= 4;
                if( shift < 0 )
                {
                    shift = 4;
                    ++d;
                }
            }
        }
    }

    delete[] data;
    data = newBmp;
}