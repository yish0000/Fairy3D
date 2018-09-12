/*
 * ------------------------------------------------------------------------
 *  Name:   F3DImageLoader.h
 *  Desc:   ���ļ�Ϊ���涨����һ��ͼ���������
 *  Author: Yish
 *  Date:   2010/11/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_IMAGELOADER_H__
#define __F3D_IMAGELOADER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DImageCodec.h"

///////////////////////////////////////////////////////////////////////////


/** ͼ�������
@remarks
    Ŀǰϵͳ���õ�֧����������(BMP,JPG,DDS,PNG,TGA)�ļ��ء�
    �û�����ͨ�����������ʵ���Զ����ͼ���ʽ���ء�
@note
    ����һ���������������д�����
*/
class FAIRY_API F3DImageLoader : public FGeneralAlloc
{
    typedef std::map<AString, F3DImageCodec*> ImageCodecList;

protected:
    ImageCodecList m_Codecs;    // ͼ��������б�

public:
    ~F3DImageLoader(void);

    // ע��/��ע�� ͼ�������
    void RegisterCodec( F3DImageCodec* pCodec );
    void UnregisterCodec( const char* name );

    // ͨ���ļ���չ����ȡͼ�������
    F3DImageCodec* GetImageCodec( const char* name ) const;

    // ����ͼ���ļ�
    F3DImage* Load( const char* filename );

    // ����ָ����ͼ������ļ�
    void Save( const char* filename,F3DImage* image );

    // ����ָ��ͼ���Mipmap���ļ�
    void SaveMipmap( const char* filename,F3DImage* image,size_t level );

    // ��ȡȫ�ֵĵ���ʵ��
    static F3DImageLoader& GetInstance(void);

private:
    F3DImageLoader(void);
	F_DISABLE_COPY_ASSIGNMENT(F3DImageLoader);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_IMAGELOADER_H__