/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPNGCodec.h
 *  Desc:   ���ļ�Ϊ������һ��PNGͼ���������
 *  Author: Yish
 *  Date:   2010/11/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_PNGCODEC_H__
#define __F3D_PNGCODEC_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DImageCodec.h"

///////////////////////////////////////////////////////////////////////////


/** PNGͼ�������
*/
class FAIRY_API F3DPNGCodec : public F3DImageCodec
{
public:
    F3DPNGCodec(void);
    ~F3DPNGCodec(void);

    // ����ͼ���ļ�
    F3DImage* Load( const char* filename );

    // ����ͼ���Mipmap���ļ�
    void SaveMipmap( const char* filename,F3DImage* image,size_t level );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PNGCODEC_H__