/*
 * ------------------------------------------------------------------------
 *  Name:   F3DBMPCodec.h
 *  Desc:   ���ļ�������һ������λͼ�Ľ�������
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


/** λͼ������
@remarks
@note
*/
class FAIRY_API F3DBMPCodec : public F3DImageCodec
{
public:
    F3DBMPCodec(void);
    ~F3DBMPCodec(void);

    // ����ͼ���ļ�
    F3DImage* Load( const char* filename );

    // ����ͼ���Mipmap�������ļ�
    void SaveMipmap( const char* filename, F3DImage* image, size_t level );

private:

    // �������ļ��м���ͼ�����
    F3DImage* LoadImageFromVFile( FVFile* pFile );
    // ���ѹ��ģʽ1�Ľ�ѹ���㷨
    void Decompress8Bit( fbyte*& data, size_t bufSize, uint32 width, uint32 height, uint32 pitch );
    // ���ѹ��ģʽ2�Ľ�ѹ���㷨
    void Decompress4Bit( fbyte*& data, size_t bufSize, uint32 width, uint32 height, uint32 pitch );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_BMPCODEC_H__