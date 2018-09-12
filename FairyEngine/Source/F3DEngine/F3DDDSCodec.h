/*
 * ------------------------------------------------------------------------
 *  Name:   F3DDDSCodec.h
 *  Desc:   ���ļ�Ϊ���涨����һ��DDS�ļ���������
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


/** DDSͼ�������
*/
class FAIRY_API F3DDDSCodec : public F3DImageCodec
{
public:
    F3DDDSCodec(void);
    ~F3DDDSCodec(void);

    // �ӱ��ش����м���ͼ��
    F3DImage* Load( const char* filename );

    // ����ͼ���Mipmap���ļ�
    void SaveMipmap( const char* filename,F3DImage* image,size_t level );

private:
    // �������ļ����м���ͼ�����
    F3DImage* LoadImageFromVFile( FVFile* pFile );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_DDSCODEC_H__