/*
 * ------------------------------------------------------------------------
 *  Name:   F3DImageLoader.cpp
 *  Desc:   本文件为引擎实现了一个图像加载器。
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

/** 注册一个新的图像解码器
@Param 指向图像解码器的指针
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
            // 添加新的图像解码器
            m_Codecs.insert( ImageCodecList::value_type(pCodec->GetCodecName(), pCodec) );

            // 写入系统日志
            FLOG_INFO( "Image Codec (" + pCodec->GetCodecName() + ") is registered!" );
        }
    }
}

/** 反注册一个指定的图像解码器
@Param 指定图像解码器的名字
*/
void F3DImageLoader::UnregisterCodec( const char* name )
{
    ImageCodecList::iterator it;
    it = m_Codecs.find( name );

    if( it != m_Codecs.end() )
    {
		F_SAFE_DELETE(it->second);
        m_Codecs.erase( it );

        // 写入系统日志
        FLOG_INFOF( "Image Codec (%s) is unregistered!", name );
    }
}

/** 根据指定的名字获取图像解码器的指针
@Param 指定图像解码器的名字
*/
F3DImageCodec* F3DImageLoader::GetImageCodec( const char* name ) const
{
    ImageCodecList::const_iterator it;
    it = m_Codecs.find( name );
    return it != m_Codecs.end() ? it->second : NULL;
}

/** 从本地磁盘中加载图像文件
@Param 指定图像文件的名字
*/
F3DImage* F3DImageLoader::Load( const char* filename )
{
    AString fileExt;

    // 获取该图像文件的扩展名
    AStringUtil::FileExt( filename, fileExt );
    AStringUtil::ToUpperCase( fileExt );

    // 查找合适的解码器
    ImageCodecList::iterator it;
    it = m_Codecs.find( fileExt );
    if( it == m_Codecs.end() )
    {
        FLOG_ERRORF( "F3DImageLoader::LoadImage(), Can't find the image codec (%s)!",fileExt.c_str() );
		return NULL;
    }

    return it->second->Load( filename );
}

/** 保存指定的图像对象到文件
@Param 要保存的文件路径
@Param 指向图像对象的指针
*/
void F3DImageLoader::Save( const char* filename, F3DImage* image )
{
    AString fileExt;

    // 获取该图像文件的名字
    AStringUtil::FileExt( filename, fileExt );
    AStringUtil::ToUpperCase( fileExt );

    // 查找合适的解码器
    ImageCodecList::iterator it;
    it = m_Codecs.find( fileExt );
    if( it == m_Codecs.end() )
    {
        FLOG_ERRORF( "F3DImageLoader::SaveImage(), Can't find the image codec (%s)!", fileExt.c_str() );
		return;
    }

    it->second->Save( filename, image );
}

/** 保存制定图像的Mipmap到文件
@Param 要保存的文件路径
@Param 指向图像对象的指针
@Param 要保存的Mipmap层
*/
void F3DImageLoader::SaveMipmap( const char* filename, F3DImage* image, size_t level )
{
    AString fileExt;

    // 获取该图像文件的名字
    AStringUtil::FileExt( filename, fileExt );
    AStringUtil::ToUpperCase( fileExt );

    // 查找合适的解码器
    ImageCodecList::iterator it;
    it = m_Codecs.find( fileExt );
    if( it == m_Codecs.end() )
    {
        FLOG_ERRORF( "F3DImageLoader::SaveMipmap(), Can't find the image codec (%s)!",fileExt.c_str() );
		return;
    }

    it->second->SaveMipmap( filename,image,level );
}