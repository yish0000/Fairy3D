/*
 * ------------------------------------------------------------------------
 *  Name:   F3DJPGCodec.h
 *  Desc:   ���ļ�Ϊ���涨����һ��JPEGͼ���������
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


/** JPEGͼ�������
*/
class FAIRY_API F3DJPGCodec : public F3DImageCodec
{

#ifdef USE_IJL_LIB
private:
    F_DLIB_HANDLE m_hDll;   // Intel JPEG ��̬����
#endif

public:
    F3DJPGCodec(void);
    ~F3DJPGCodec(void);

    // �ӱ��ش����м���ͼ��
    F3DImage* Load( const char* filename );

    // ����ͼ���Mipmap���ļ�
    void SaveMipmap( const char* filename,F3DImage* image,size_t level );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_JPGCODEC_H__