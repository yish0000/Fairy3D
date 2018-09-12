/*
 * ------------------------------------------------------------------------
 *  Name:   F3DJPGCodec.cpp
 *  Desc:   本文件为引擎实现了一个JPEG图像解码器。
 *  Author: Yish
 *  Date:   11/16/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DJPGCodec.h"
#include "F3DPixelUtil.h"

#ifdef USE_IJL_LIB
#include "ijl.h"


// 声明加载JPG文件的函数指针
typedef IJLERR (*IJLINIT) ( JPEG_CORE_PROPERTIES* jcprops );
typedef IJLERR (*IJLREAD) ( JPEG_CORE_PROPERTIES* jcprops,IJLIOTYPE iotype );
typedef IJLERR (*IJLWRITE) ( JPEG_CORE_PROPERTIES* jcprops,IJLIOTYPE iotype );
typedef IJLERR (*IJLFREE) ( JPEG_CORE_PROPERTIES* jcprops );

/** 构造函数
*/
F3DJPGCodec::F3DJPGCodec(void)
{
    m_sCodecName = "JPG";

    // 加载intel jpeg库
#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    m_hDll = F_DLIB_LOAD( "ijl15.dll" );
#elif FAIRY_PLATFORM == FAIRY_PLATFORM_LINUX
    m_hDll = F_DLIB_LOAD( "ijl15.o" );
#elif FAIRY_PLATFORM == FAIRY_PLATFORM_MACOS
    m_hDll = F_DLIB_LOAD( "ijl15" );
#endif

    if( !m_hDll )
    {
        F_EXCEPTION("F3DJPGCodec::F3DJPGCodec", "Load intel jpeg lib failed, Can't initialize the JPG Codec!");
    }
}

/** 析构函数
*/
F3DJPGCodec::~F3DJPGCodec(void)
{
    if( m_hDll )
    {
        F_DLIB_UNLOAD( m_hDll );
        m_hDll = NULL;
    }
}

/** 从本地磁盘中加载图像
@Param 图像文件的路径
*/
F3DImage* F3DJPGCodec::Load( const char* filename )
{
	if( !m_hDll )
	{
		FLOG_WARNINGF( "F3DJPGCodec::Load, Null Library, Load the JPG image(%s) failed!",filename );
		return NULL;
	}

    // 加载JPEG库内的函数
    IJLINIT funcInit = (IJLINIT)F_DLIB_GETSYMBOL( m_hDll,"ijlInit" );
    IJLREAD funcRead = (IJLREAD)F_DLIB_GETSYMBOL( m_hDll,"ijlRead" );
    IJLWRITE funcWrite = (IJLWRITE)F_DLIB_GETSYMBOL( m_hDll,"ijlWrite" );
    IJLFREE funcFree = (IJLFREE)F_DLIB_GETSYMBOL( m_hDll,"ijlFree" );

    if( !funcInit || !funcRead || !funcWrite || !funcFree )
    {
        F_EXCEPTION("F3DJPGCodec::Load", "Get the intel jpeg function failed!");
		return NULL;
    }

    IJLERR jpgerr;
    JPEG_CORE_PROPERTIES jpeg;
    memset( &jpeg,0,sizeof(JPEG_CORE_PROPERTIES) );

    // 加载指定的JPEG文件
    FVFile file;
    if( !file.Open(filename, FVFile::VFILE_OPENEXIST|FVFile::VFILE_BINARY) )
    {
        FLOG_WARNINGF("F3DJPGCodec::Load(), Failed to load the file %s!", filename);
        return NULL;
    }

    size_t srcSize = file.GetFileSize();
    fbyte* pSrcBuf = new fbyte[srcSize];
    file.Read(pSrcBuf, srcSize);
    file.Close();

    funcInit( &jpeg );
    jpeg.JPGBytes = pSrcBuf;
    jpeg.JPGSizeBytes = (int)srcSize;
    jpgerr = funcRead( &jpeg,IJL_JFILE_READPARAMS );

    // 处理错误
    if( jpgerr < 0 )
    {
        switch( jpgerr )
        {
        case IJL_INVALID_FILENAME:
            funcFree( &jpeg );
            F_EXCEPTION("F3DJPGCodec::Load", "Failed to load the image, (IJL_INVALID_FILENAME)");
            break;
        case IJL_INVALID_ENCODER:
            funcFree( &jpeg );
            F_EXCEPTION("F3DJPGCodec::Load", "Failed to load the image, (IJL_INVALID_ENCODER)");
            break;
        case IJL_MEMORY_ERROR:
            funcFree( &jpeg );
            F_EXCEPTION("F3DJPGCodec::Load", "Failed to load the image, (IJL_MEMORY_ERROR)");
            break;
        default:
            funcFree( &jpeg );
            F_EXCEPTION("F3DJPGCodec::Load", "Failed to load the image, (Unknown)");
            break;
        }

		return NULL;
    }

    switch( jpeg.JPGChannels )
    {
    case 1:
        jpeg.JPGColor = IJL_G;
        jpeg.DIBChannels = 3;
        jpeg.DIBColor = IJL_RGB;
        break;
    case 3:
        jpeg.JPGColor = IJL_YCBCR;
        jpeg.DIBChannels = 3;
        jpeg.DIBColor = IJL_RGB;
        break;
    case 4:
        jpeg.JPGColor = IJL_YCBCRA_FPX;
        jpeg.DIBChannels = 4;
        jpeg.DIBColor = IJL_RGBA_FPX;
        break;
    default:
        funcFree( &jpeg );
        F_EXCEPTION("F3DJPGCodec::Load", "Unknown JPEG image format!");
		return NULL;
    }

    jpeg.DIBWidth = jpeg.JPGWidth;
    jpeg.DIBHeight = jpeg.JPGHeight;
    jpeg.DIBPadBytes = IJL_DIB_PAD_BYTES( jpeg.DIBWidth,jpeg.DIBChannels );

    // 计算整个图像所需缓冲区大小
    size_t bufSize = (jpeg.DIBWidth*jpeg.DIBChannels+jpeg.DIBPadBytes)*jpeg.DIBHeight;

    // 读取像素数据
    fbyte* imageData = new fbyte[bufSize];
    jpeg.DIBBytes = imageData;
    funcRead( &jpeg,IJL_JFILE_READWHOLEIMAGE );

    // 创建一个图像对象
    F3DImage* image = new F3DImage( jpeg.DIBWidth,jpeg.DIBHeight,(jpeg.DIBChannels == 3) ?
        PFT_R8G8B8 : PFT_A8R8G8B8 );

    if( jpeg.DIBChannels == 3 )
    {
        F3D_24BitToImage( imageData,image->GetImageData(),jpeg.DIBWidth,jpeg.DIBHeight,
            jpeg.DIBPadBytes,false,false );
    }
    else if( jpeg.DIBChannels == 4 )
    {
        F3D_32BitToImage( imageData,image->GetImageData(),jpeg.DIBWidth,jpeg.DIBHeight,
            jpeg.DIBPadBytes,false );
    }

    // 关闭JPEG文件
    funcFree( &jpeg );
    delete[] imageData;

    return image;
}

/** 保存指定图像的Mipmap
@Param 要保存图像文件的路径
@Param 指向图像对象的指针
@Param 指定的Mipmap层级
*/
void F3DJPGCodec::SaveMipmap( const char* filename,F3DImage* image,size_t level )
{
	if( !m_hDll )
	{
		FLOG_WARNINGF( "F3DJPGCodec::SaveMipmap, Null Library, Save the JPG image(%s) failed!",filename );
		return;
	}

    // 获取JPEG库内的函数
    IJLINIT funcInit = (IJLINIT)F_DLIB_GETSYMBOL( m_hDll,"ijlInit" );
    IJLREAD funcRead = (IJLREAD)F_DLIB_GETSYMBOL( m_hDll,"ijlRead" );
    IJLWRITE funcWrite = (IJLWRITE)F_DLIB_GETSYMBOL( m_hDll,"ijlWrite" );
    IJLFREE funcFree = (IJLFREE)F_DLIB_GETSYMBOL( m_hDll,"ijlFree" );

    if( !funcInit || !funcRead || !funcWrite || !funcFree )
    {
        F_EXCEPTION("F3DJPGCodec::SaveMipmap", "Get the intel jpeg function failed!");
		return;
    }

    fbyte* destBuf;
    EPixelFormat destFormat;
    size_t destSize,pxSize;

    // 获取源图像的一些信息
    size_t width = image->GetWidth( level );
    size_t height = image->GetHeight( level );
    EPixelFormat srcFormat = image->GetPixelFormat();

    if( F3D_HaveAlpha(srcFormat) )
        destFormat = PFT_A8R8G8B8;
    else
        destFormat = PFT_R8G8B8;

    // 将数据转换为指定像素格式
    pxSize = F3D_PixelSize( destFormat );
    destSize = width*height*pxSize;
    destBuf = new fbyte[destSize];
    F3D_ConvertPixelFormat( image->GetImageData(level),srcFormat,destBuf,destFormat,width,height );

    // 保存数据到JPEG文件
    JPEG_CORE_PROPERTIES jpeg;
    memset( &jpeg,0,sizeof(JPEG_CORE_PROPERTIES) );

    funcInit( &jpeg );

    jpeg.DIBWidth = (int)width;
    jpeg.DIBHeight = (int)height;
    jpeg.DIBBytes = destBuf;
    jpeg.JPGWidth = (int)width;
    jpeg.JPGHeight = (int)height;
    jpeg.JPGFile = filename;

    if( destFormat == PFT_R8G8B8 )
    {
        jpeg.DIBColor = IJL_RGB;
        jpeg.DIBChannels = 3;
        jpeg.DIBPadBytes = IJL_DIB_PAD_BYTES( (int)width,3 );
        jpeg.JPGColor = IJL_YCBCR;
        jpeg.JPGChannels = 3;
        jpeg.JPGSubsampling = IJL_411;
    }
    else if( destFormat == PFT_A8R8G8B8 )
    {
        jpeg.DIBColor = IJL_RGBA_FPX;
        jpeg.DIBChannels = 4;
        jpeg.DIBPadBytes = IJL_DIB_PAD_BYTES( (int)width,4 );
        jpeg.JPGColor = IJL_YCBCRA_FPX;
        jpeg.JPGChannels = 4;
        jpeg.JPGSubsampling = IJL_4114;
    }

    funcWrite( &jpeg,IJL_JFILE_WRITEWHOLEIMAGE );
    funcFree( &jpeg );

    delete[] destBuf;
}

#else

///////////////////////////////////////////////////////////////////////////
// 下面利用JPEGLIB库来实现JPG图片格式的加载和保存

#if FAIRY_PLATFORM == FAIRY_PLATFORM_WINDOWS
    #define HAVE_BOOLEAN
#endif

#include <stdio.h>
#include "jpeglib/jpeglib.h"
#include <setjmp.h>


//  错误处理
struct fjpg_error_t
{
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

typedef struct fjpg_error_t* fjpg_error_ptr;

// 错误处理函数
static void fjpg_error_exit( j_common_ptr cinfo )
{
    fjpg_error_ptr myerr = (fjpg_error_ptr)cinfo->err;
    (*cinfo->err->output_message)( cinfo );
    longjmp(myerr->setjmp_buffer, 1);
}

/** 构造函数
*/
F3DJPGCodec::F3DJPGCodec(void)
{
}

/** 析构函数
*/
F3DJPGCodec::~F3DJPGCodec(void)
{
}

/** 加载JPG文件
@Param JPG图片的路径
*/
F3DImage* F3DJPGCodec::Load( const char* filename )
{
    struct jpeg_decompress_struct cinfo;
    struct fjpg_error_t jerr;

    F3DImage* pImage;
    JSAMPARRAY buffer;
    int row_stride;

    // 加载图像
    FVFile file;
    if( !file.Open(filename, FVFile::VFILE_OPENEXIST|FVFile::VFILE_BINARY) )
    {
        FLOG_WARNINGF("F3DJPGCodec::Load(), Open image file (%s) failed!", filename);
        return NULL;
    }

    // 分配初始化JPEG解压对象
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = fjpg_error_exit;
    if( setjmp(jerr.setjmp_buffer) )
    {
        jpeg_destroy_decompress(&cinfo);
        FLOG_WARNINGF("F3DJPGCodec::Load(), Exception occur when loading %s!", filename);
        return NULL;
    }

    jpeg_create_decompress(&cinfo);

    // 指定数据源
    size_t srcSize = file.GetFileSize();
    fbyte* pSrcData = new fbyte[srcSize];
    file.Read(pSrcData, srcSize);
    jpeg_mem_src(&cinfo, pSrcData, (unsigned long)srcSize);

    // 读取文件的属性
    jpeg_read_header(&cinfo, TRUE);

    // 开始解压
    jpeg_start_decompress(&cinfo);

    // 创建图像对象
    if( cinfo.output_components == 3 )
        pImage = new F3DImage(cinfo.image_width, cinfo.image_height, PFT_R8G8B8);
    else
    {
        delete[] pSrcData;
        FLOG_WARNINGF("F3DJPGCodec::Load(), Unknown JPEG format when loading %s!", filename);
        jpeg_destroy_decompress(&cinfo);
        return NULL;
    }

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    // 读取每一行数据
    int iCurLine = 0;
    fbyte* pImageData = pImage->GetImageData();
    while( cinfo.output_scanline < cinfo.output_height )
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(&pImageData[iCurLine*row_stride], buffer[0], row_stride);
        iCurLine++;
    }

    jpeg_finish_decompress(&cinfo);

    // 销毁JPEG对象
    jpeg_destroy_decompress(&cinfo);

    delete[] pSrcData;
    return pImage;
}

/** 保存指定图像的Mipmap
@Param 要保存图像文件的路径
@Param 指向图像对象的指针
@Param 指定的Mipmap层级
*/
void F3DJPGCodec::SaveMipmap( const char* filename,F3DImage* image,size_t level )
{
    fbyte* destBuf;
    unsigned long destSize;

    const int JPEG_QUALITY = 0;     // 输出JPEG质量

    // 获取源图像的一些信息
    size_t width = image->GetWidth( level );
    size_t height = image->GetHeight( level );
    EPixelFormat srcFormat = image->GetPixelFormat();

    // 将数据转换为R8G8B8
    destSize = (unsigned long)(width*height*3);
    destBuf = new fbyte[destSize];
    F3D_ConvertPixelFormat( image->GetImageData(level),srcFormat,destBuf,PFT_R8G8B8,width,height );

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    size_t row_stride;
    JSAMPROW row_pointer[1];

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    // 设置输入数据
    jpeg_mem_dest(&cinfo, &destBuf, &destSize);

    // 图像参数
    cinfo.image_width = (JDIMENSION)width;
    cinfo.image_height = (JDIMENSION)height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE);

    // 开始压缩
    jpeg_start_compress(&cinfo, TRUE);

    row_stride = width * 3;
    while( cinfo.next_scanline < cinfo.image_height )
    {
        row_pointer[0] = &destBuf[cinfo.next_scanline*row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);

    // 写入图像数据
    FVFile file;
    if( !file.Open(filename, FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
    {
        delete[] destBuf;
        jpeg_destroy_compress(&cinfo);
        FLOG_WARNINGF("F3DJPGCodec::SaveMipmap(), Create file failed when saving %s!", filename);
        return;
    }

    file.Write(destBuf, destSize);
    file.Close();

    jpeg_destroy_compress(&cinfo);
    delete[] destBuf;
}

#endif