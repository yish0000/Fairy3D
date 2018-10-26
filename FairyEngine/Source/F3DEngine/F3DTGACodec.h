/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTGACodec.h
 *  Desc:   ���ļ�Ϊ���涨����һ��TGAͼ���������
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


/** TGA�ļ�ͷ
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

/** TGA���ļ���
*/
struct STGAFooter
{
    uint32 ExtensionOffset;
    uint32 DeveloperOffset;
    char Signature[18];
};

/** TGAͼ�������
*/
class FAIRY_API F3DTGACodec : public F3DImageCodec
{
public:
    F3DTGACodec(void);
    ~F3DTGACodec(void);

    // ����ͼ���ļ�
    F3DImage* Load( const char* filename );

    // ����ͼ���Mipmap�������ļ�
    void SaveMipmap( const char* filename, F3DImage* image, size_t level );

private:
    // �������ļ����м���ͼ�����
    F3DImage* LoadImageFromVFile( FVFile* pFile );

    // ����ѹ���˵�TGAͼ��
    FBYTE* LoadCompressedImage( FVFile* pFile, const STGAHeader& header );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TGACODEC_H__