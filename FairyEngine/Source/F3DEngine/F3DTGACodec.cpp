/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTGACodec.cpp
 *  Desc:   ���ļ�Ϊ����ʵ����һ��TGAͼ���������
 *  Author: Yish
 *  Date:   2010/11/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DTGACodec.h"
#include "F3DPixelUtil.h"


/** ���캯��
*/
F3DTGACodec::F3DTGACodec(void)
{
    m_sCodecName = "TGA";
}

/** ��������
*/
F3DTGACodec::~F3DTGACodec(void)
{
}

/** ����ͼ���ļ�
@Param ͼ���ļ���·��
*/
F3DImage* F3DTGACodec::Load( const char* filename )
{
    FVFile file;
	if( !file.Open(filename,FVFile::VFILE_OPENEXIST) )
	{
		FLOG_WARNINGF( "F3DTGACodec::Load, Open image file (%s) failed!",filename );
		return NULL;
	}

    // �������ļ��м���ͼ��
    return LoadImageFromVFile( &file );
}

/** �������ļ����м���ͼ������
@Param �����ļ�����ָ��
*/
F3DImage* F3DTGACodec::LoadImageFromVFile( FVFile* pFile )
{
    STGAHeader header;
    ulong* palette = NULL;

    // ��ȡ�ļ�ͷ
    pFile->Read( &header,sizeof(STGAHeader) );

    if( header.IDLength )
        pFile->Seek( header.IDLength,FVFile::CURRENT );

    if( header.ColorMapType )
    {
        // ����ɫ����������(����Ҫ����TGAͼ��
        // ��Ϊ��ɫ��ͼ�����ǿ�����λͼ)
		pFile->Seek( header.ColorMapEntrySize / 8 * header.ColorMapLength,FVFile::START );
    }

    // ��ȡ��������
    fbyte* data = NULL;

    if( header.ImageType == 2 )
    {
        const size_t imageSize = header.ImageWidth * header.ImageHeight *
            header.PixelDepth / 8;
        data = new fbyte[imageSize];
        pFile->Read( data,imageSize );
    }
    else
    {
        if( header.ImageType == 10 )
        {
            // ����ѹ����������
            data = LoadCompressedImage( pFile,header );
        }
        else
        {
			FLOG_WARNING( "F3DTGACodec::LoadImageFromVFile, Unknown TGA format!" );
			return NULL;
        }
    }

    F3DImage* image;
    switch( header.PixelDepth )
    {
    case 16:
        image = new F3DImage( header.ImageWidth,header.ImageHeight,PFT_A1R5G5B5 );
        F3D_16BitToImage( data,image->GetImageData(),header.ImageWidth,header.ImageHeight,0,
            (header.ImageDescriptor & 0x20) == 0 );
        break;
    case 24:
        image = new F3DImage( header.ImageWidth,header.ImageHeight,PFT_R8G8B8 );
        F3D_24BitToImage( data,image->GetImageData(),header.ImageWidth,header.ImageHeight,0,
            (header.ImageDescriptor & 0x20) == 0,false );
        break;
    case 32:
        image = new F3DImage( header.ImageWidth,header.ImageHeight,PFT_A8R8G8B8 );
        F3D_32BitToImage( data,image->GetImageData(),header.ImageWidth,header.ImageHeight,0,
            (header.ImageDescriptor & 0x20) == 0 );
        break;
    default:
        delete[] data;
		FLOG_WARNING( "F3DTGACodec::LoadImageFromVFile, Unknown TGA Type!" );
		return NULL;
    }

    delete[] data;
    return image;
}

/** ����ѹ���˵�TGAͼ��
@Param �����ļ�����ָ��
@Param TGAͼ����ļ�ͷ
*/
fbyte* F3DTGACodec::LoadCompressedImage( FVFile* pFile,const STGAHeader& header )
{
    size_t pxSize = header.PixelDepth / 8;
    size_t imageSize = header.ImageWidth * header.ImageHeight * pxSize;
    fbyte* data = new fbyte[imageSize];

    size_t curBytes = 0;
    while( curBytes < imageSize )
    {
        fbyte chunkHeader;
        pFile->Read( &chunkHeader,sizeof(fbyte) );

        if( chunkHeader < 128 )
        {
            chunkHeader++;

            pFile->Read( &data[curBytes],pxSize*chunkHeader );
            curBytes += pxSize * chunkHeader;
        }
        else
        {
            chunkHeader -= 127;

            size_t dataOffset = curBytes;
            pFile->Read( &data[dataOffset],pxSize );
            curBytes += pxSize;

            for( size_t i=0;i<chunkHeader;i++ )
            {
                for( size_t em=0;em<pxSize;em++ )
                {
                    data[curBytes+em] = data[dataOffset+em];
                }

                curBytes += pxSize;
            }
        }
    }

    return data;
}

/** ����ָ��ͼ���Mipmap
@Param Ҫ����ͼ���ļ���·��
@Param ָ��ͼ������ָ��
@Param ָ����Mipmap�㼶
*/
void F3DTGACodec::SaveMipmap( const char* filename,F3DImage* image,size_t level )
{
    fbyte* destBuf;
    EPixelFormat destFormat;
    size_t destSize,pxSize;

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

	FVFile file;
	if( file.Open(filename,FVFile::VFILE_CREATENEW|FVFile::VFILE_BINARY) )
    {
        delete[] destBuf;
		FLOG_WARNINGF( "F3DTGACodec::SaveMipmap, Create the image failed(%s) failed!",filename );
		return;
    }

    STGAHeader header;
    header.IDLength = 0;
    header.ColorMapType = 0;
    header.ImageType = 2;
    header.FirstEntryIndex[0] = 0;
    header.FirstEntryIndex[1] = 0;
    header.ColorMapLength = 0;
    header.ColorMapEntrySize = 0;
    header.XOrigin[0] = 0;
    header.XOrigin[1] = 0;
    header.YOrigin[0] = 0;
    header.YOrigin[1] = 0;
    header.ImageWidth = (ushort)image->GetWidth();
    header.ImageHeight = (ushort)image->GetHeight();
    header.ImageDescriptor = 1 << 5;

    if( destFormat == PFT_R8G8B8 )
    {
        header.PixelDepth = 24;
        header.ImageDescriptor |= 0;
    }
    else if( destFormat == PFT_A8R8G8B8 )
    {
        header.PixelDepth = 32;
        header.ImageDescriptor |= 8;
    }

    // д���ļ�ͷ
    file.Write( &header,sizeof(STGAHeader) );

    // д����������
    for( size_t y=height-1;y>=0;y-- )
    {
        fbyte* pData = &destBuf[y*width*pxSize];
        file.Write( pData,width*pxSize );
    }

    file.Close();
    delete[] destBuf;
}