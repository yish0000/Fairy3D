/*
 * ------------------------------------------------------------------------
 *  Name:   F3DImageLoader.cpp
 *  Desc:   ���ļ�Ϊ����ʵ����һ��ͼ���������
 *  Author: Yish
 *  Date:   2010/11/15
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DImageLoader.h"

#include "F3DBMPCodec.h"
#include "F3DDDSCodec.h"
#include "F3DJPGCodec.h"
#include "F3DPNGCodec.h"
#include "F3DTGACodec.h"
#include "F3DKTXCodec.h"
#include "F3DPVRCodec.h"


/** Get the singleton.
*/
F3DImageLoader& F3DImageLoader::GetInstance(void)
{
    static F3DImageLoader obj;
    return obj;
}

/** Constructor
*/
F3DImageLoader::F3DImageLoader()
{
	RegisterCodec(new F3DBMPCodec());
	RegisterCodec(new F3DDDSCodec());
	RegisterCodec(new F3DJPGCodec());
	RegisterCodec(new F3DPNGCodec());
	RegisterCodec(new F3DTGACodec());
	RegisterCodec(new F3DPVRCodec());
	RegisterCodec(new F3DKTXCodec());
}

/** Destructor.
*/
F3DImageLoader::~F3DImageLoader(void)
{
	ImageCodecList::iterator it = m_Codecs.begin();
	for(; it != m_Codecs.end(); ++it)
	{
		F_SAFE_DELETE(it->second);
	}
}

/** ע��һ���µ�ͼ�������
@Param ָ��ͼ���������ָ��
*/
void F3DImageLoader::RegisterCodec( F3DImageCodec* pCodec )
{
    if( pCodec )
    {
        ImageCodecList::iterator it;
        it = m_Codecs.find( pCodec->GetCodecName() );
        if( it != m_Codecs.end() )
        {
			FASSERT(0);
            FLOG_ERRORF("F3DImageLoader::RegisterCodec, Image codec(%s) already exists!", pCodec->GetCodecName().c_str());
        }
        else
        {
            // ����µ�ͼ�������
            m_Codecs.insert( ImageCodecList::value_type(pCodec->GetCodecName(), pCodec) );

            // д��ϵͳ��־
            FLOG_INFO( "Image Codec (" + pCodec->GetCodecName() + ") is registered!" );
        }
    }
}

/** ��ע��һ��ָ����ͼ�������
@Param ָ��ͼ�������������
*/
void F3DImageLoader::UnregisterCodec( const char* name )
{
    ImageCodecList::iterator it;
    it = m_Codecs.find( name );

    if( it != m_Codecs.end() )
    {
		F_SAFE_DELETE(it->second);
        m_Codecs.erase( it );

        // д��ϵͳ��־
        FLOG_INFOF( "Image Codec (%s) is unregistered!", name );
    }
}

/** ����ָ�������ֻ�ȡͼ���������ָ��
@Param ָ��ͼ�������������
*/
F3DImageCodec* F3DImageLoader::GetImageCodec( const char* name ) const
{
    ImageCodecList::const_iterator it;
    it = m_Codecs.find( name );
    return it != m_Codecs.end() ? it->second : NULL;
}

/** �ӱ��ش����м���ͼ���ļ�
@Param ָ��ͼ���ļ�������
*/
F3DImage* F3DImageLoader::Load( const char* filename )
{
    AString fileExt;

    // ��ȡ��ͼ���ļ�����չ��
    AStringUtil::FileExt( filename, fileExt );
    AStringUtil::ToUpperCase( fileExt );

    // ���Һ��ʵĽ�����
    ImageCodecList::iterator it;
    it = m_Codecs.find( fileExt );
    if( it == m_Codecs.end() )
    {
        FLOG_ERRORF( "F3DImageLoader::LoadImage(), Can't find the image codec (%s)!",fileExt.c_str() );
		return NULL;
    }

    return it->second->Load( filename );
}

/** ����ָ����ͼ������ļ�
@Param Ҫ������ļ�·��
@Param ָ��ͼ������ָ��
*/
void F3DImageLoader::Save( const char* filename, F3DImage* image )
{
    AString fileExt;

    // ��ȡ��ͼ���ļ�������
    AStringUtil::FileExt( filename, fileExt );
    AStringUtil::ToUpperCase( fileExt );

    // ���Һ��ʵĽ�����
    ImageCodecList::iterator it;
    it = m_Codecs.find( fileExt );
    if( it == m_Codecs.end() )
    {
        FLOG_ERRORF( "F3DImageLoader::SaveImage(), Can't find the image codec (%s)!", fileExt.c_str() );
		return;
    }

    it->second->Save( filename, image );
}

/** �����ƶ�ͼ���Mipmap���ļ�
@Param Ҫ������ļ�·��
@Param ָ��ͼ������ָ��
@Param Ҫ�����Mipmap��
*/
void F3DImageLoader::SaveMipmap( const char* filename, F3DImage* image, size_t level )
{
    AString fileExt;

    // ��ȡ��ͼ���ļ�������
    AStringUtil::FileExt( filename, fileExt );
    AStringUtil::ToUpperCase( fileExt );

    // ���Һ��ʵĽ�����
    ImageCodecList::iterator it;
    it = m_Codecs.find( fileExt );
    if( it == m_Codecs.end() )
    {
        FLOG_ERRORF( "F3DImageLoader::SaveMipmap(), Can't find the image codec (%s)!",fileExt.c_str() );
		return;
    }

    it->second->SaveMipmap( filename,image,level );
}