/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPNGCodec.cpp
 *  Desc:   ���ļ�Ϊ����ʵ����һ��PNGͼ���������
 *  Author: Yish
 *  Date:   11/16/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DPNGCodec.h"
#include "F3DPixelUtil.h"

#include "libpng/png.h"


/** �ӱ��ش����ļ��ж�ȡPNG���ݵĻص�����
@Param PNG�ṹ���ָ��
@Param �洢���ݵ�ָ��
@Param Ҫ��ȡ��������
*/
void PNGAPI PNGReadFunc( png_structp png_ptr,png_bytep data,png_size_t length )
{
    FVFile* pFile = (FVFile*)png_ptr->io_ptr;

    if( pFile->Read(data,length) != length )
        png_error( png_ptr,"Read error" );
}

/** ��ͼ������д��PNG�ļ��Ļص�����
@Param PNG�ṹ���ָ��
@Param �洢���ݵ�ָ��
@Param Ҫ��ȡ��������
*/
void PNGAPI PNGWriteFunc( png_structp png_ptr,png_bytep data,png_size_t length )
{
    FVFile* pFile = (FVFile*)png_ptr->io_ptr;
    pFile->Write( data,length );
}


/** ���캯��
*/
F3DPNGCodec::F3DPNGCodec(void)
{
    m_sCodecName = "PNG";
}

/** ��������
*/
F3DPNGCodec::~F3DPNGCodec(void)
{
}

/** �ӱ��ش����м���ͼ��
@Param ͼ���ļ���·��
*/
F3DImage* F3DPNGCodec::Load( const char* filename )
{
    F3DImage* image;

    // ��ָ�����ļ�
	FVFile file;
	if( !file.Open(filename,FVFile::VFILE_OPENEXIST) )
    {
		FLOG_WARNINGF( "F3DPNGCodec::Load, Open the image file (%s) failed!",filename );
		return NULL;
    }

    fbyte** row_pointers;
    png_byte buffer[8];

    // �ж��Ƿ�Ϊһ����ȷ��PNG�ļ�
    if( file.Read(buffer,8) != 8 || !png_sig_cmp(buffer,0,8) )
    {
		FLOG_WARNINGF( "F3DPNGCodec::Load, Invalid PNG format! (%s)",filename );
        return NULL;
    }

    // ����һ��PNG��ȡ����
    png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING,NULL,NULL,NULL );

    // ����һ��PNG��Ϣ�ṹ��
    png_infop info_ptr = png_create_info_struct( png_ptr );

    if( setjmp(png_jmpbuf(png_ptr)) )
    {
        png_destroy_read_struct( &png_ptr,&info_ptr,NULL );
        if( row_pointers ) delete[] row_pointers;

		FLOG_ERROR("F3DPNGCodec::Load, An error occurs when reading...");
		return NULL;
    }

    // ���ö�ȡ���ݵĻص�����
    png_set_read_fn( png_ptr,&file,PNGReadFunc );

    // ����PNG�����Ѿ���ȡ���ļ���ʶ
    png_set_sig_bytes( png_ptr,8 );

    // ��ȡPNG�ļ�����Ϣ
    png_read_info( png_ptr,info_ptr );

    png_uint_32 width,height;
    int bitDepth,colorType;
    png_get_IHDR( png_ptr,info_ptr,&width,&height,&bitDepth,&colorType,NULL,NULL,NULL );

    // ����ǵ�ɫ��ͼ������ת��Ϊ���ɫ
    if( colorType == PNG_COLOR_TYPE_PALETTE )
        png_set_palette_to_rgb( png_ptr );

    if( bitDepth < 8 )
    {
        if ( colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA )
            png_set_gray_1_2_4_to_8( png_ptr );
        else
            png_set_packing( png_ptr );
    }

    if( png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS) )
        png_set_tRNS_to_alpha( png_ptr );

    if( bitDepth == 16 )
		png_set_strip_16( png_ptr );

    // ����ɫת��Ϊ���ɫ
    if( colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA )
        png_set_gray_to_rgb( png_ptr );

    // ������Ϣ�ĸı�
    png_read_update_info( png_ptr,info_ptr );

    // ��RGBA��ʽת��ΪBGRA
    if( colorType == PNG_COLOR_TYPE_RGB_ALPHA )
        png_set_bgr( png_ptr );

    png_get_IHDR( png_ptr,info_ptr,&width,&height,&bitDepth,&colorType,NULL,NULL,NULL );

    // ����ͼ�����
    if( colorType == PNG_COLOR_TYPE_RGB_ALPHA )
        image = new F3DImage( width,height,PFT_A8R8G8B8 );
    else
        image = new F3DImage( width,height,PFT_R8G8B8 );

    row_pointers = new png_bytep[height];
    fbyte* imageData = image->GetImageData();
    for( size_t i=0;i<height;i++ )
    {
        row_pointers[i] = imageData;
        imageData += F3D_PixelSize(image->GetPixelFormat()) * image->GetWidth();
    }

    if( setjmp(png_jmpbuf(png_ptr)) )
    {
        png_destroy_read_struct( &png_ptr,&info_ptr,NULL );
        delete image;
        delete[] row_pointers;

        FLOG_ERROR("F3DPNGCodec::Load, An error occurs when reading...");
		return NULL;
    }

    // ��ȡ��������
    png_read_image( png_ptr,row_pointers );
    png_read_end( png_ptr,NULL );

    png_destroy_read_struct( &png_ptr,&info_ptr,NULL );
    delete[] row_pointers;
	file.Close();

    return image;
}

/** ����ָ��ͼ���Mipmap
@Param Ҫ����ͼ���ļ���·��
@Param ָ��ͼ������ָ��
@Param ָ����Mipmap�㼶
*/
void F3DPNGCodec::SaveMipmap( const char* filename,F3DImage* image,size_t level )
{
    fbyte* destBuf;
    EPixelFormat destFormat;
    size_t destSize,pxSize;

    // ����һ��PNG�ļ�
	FVFile file;
	if( file.Open(filename,FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
    {
		FLOG_WARNINGF( "F3DPNGCodec::SaveMipMap, Create the PNG file (%s) failed!",filename );
        return;
    }

    // ����һ��PNGд����
    png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING,
        NULL,NULL,NULL);

    // ����һ��PNG��Ϣ�ṹ��
    png_infop info_ptr = png_create_info_struct( png_ptr );

    if( setjmp(png_jmpbuf(png_ptr)) )
    {
        png_destroy_write_struct( &png_ptr,&info_ptr );

		FLOG_ERROR("F3DPNGCodec::SaveMipmap, An error occurs when writing...");
		return;
    }

    // ����PNG��д��ص�����
    png_set_write_fn( png_ptr,&file,PNGWriteFunc,NULL );

    // ��ȡԴͼ���һЩ��Ϣ
    size_t width = image->GetWidth( level );
    size_t height = image->GetHeight( level );
    EPixelFormat srcFormat = image->GetPixelFormat();

    if( F3D_HaveAlpha(srcFormat) )
        destFormat = PFT_A8R8G8B8;
    else
        destFormat = PFT_R8G8B8;

    // ������ת��Ϊָ�����ظ�ʽ
    pxSize = F3D_PixelSize( destFormat );
    destSize = width*height*pxSize;
    destBuf = new fbyte[destSize];
    F3D_ConvertPixelFormat( image->GetImageData(level),srcFormat,destBuf,destFormat,width,height );

    // ����PNGͼ�����Ϣ
    if( destFormat == PFT_A8R8G8B8 )
    {
        png_set_IHDR( png_ptr,info_ptr,(png_uint_32)image->GetWidth(level),
            (png_uint_32)image->GetHeight(level),8,PNG_COLOR_TYPE_RGB_ALPHA,
            PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT );
    }
    else
    {
        png_set_IHDR( png_ptr,info_ptr,(png_uint_32)image->GetWidth(level),
            (png_uint_32)image->GetHeight(level),8,PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT );
    }

    fbyte** row_pointers = new png_bytep[height];

    fbyte* data = destBuf;
    for( size_t i=0;i<height;i++ )
    {
        row_pointers[i] = data;
        data += width * pxSize;
    }

    if( setjmp(png_jmpbuf(png_ptr)) )
    {
        png_destroy_write_struct( &png_ptr,&info_ptr );
        delete[] row_pointers;
        delete[] destBuf;

		FLOG_ERROR("F3DPNGCodec::SaveMipmap, An error occurs when writing...");
		return;
    }

    png_set_rows( png_ptr,info_ptr,row_pointers );

    if( destFormat == PFT_A8R8G8B8 )
        png_write_png( png_ptr,info_ptr,PNG_TRANSFORM_BGR,NULL );
    else
        png_write_png( png_ptr,info_ptr,PNG_TRANSFORM_IDENTITY,NULL );

    // �ر�PNG�ļ�
    png_destroy_write_struct( &png_ptr,&info_ptr );
    file.Close();
    delete[] row_pointers;
    delete[] destBuf;
}