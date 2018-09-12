/*
 * ------------------------------------------------------------------------
 *  Name:   F3DImageLoader.h
 *  Desc:   本文件为引擎定义了一个图像加载器。
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


/** 图像加载器
@remarks
    目前系统内置的支持五种纹理(BMP,JPG,DDS,PNG,TGA)的加载。
    用户可以通过开发插件来实现自定义的图像格式加载。
@note
    这是一个单件，请勿自行创建。
*/
class FAIRY_API F3DImageLoader : public FGeneralAlloc
{
    typedef std::map<AString, F3DImageCodec*> ImageCodecList;

protected:
    ImageCodecList m_Codecs;    // 图像解码器列表

public:
    ~F3DImageLoader(void);

    // 注册/反注册 图像解码器
    void RegisterCodec( F3DImageCodec* pCodec );
    void UnregisterCodec( const char* name );

    // 通过文件扩展名获取图像解码器
    F3DImageCodec* GetImageCodec( const char* name ) const;

    // 加载图像文件
    F3DImage* Load( const char* filename );

    // 保存指定的图像对象到文件
    void Save( const char* filename,F3DImage* image );

    // 保存指定图像的Mipmap到文件
    void SaveMipmap( const char* filename,F3DImage* image,size_t level );

    // 获取全局的单个实例
    static F3DImageLoader& GetInstance(void);

private:
    F3DImageLoader(void);
	F_DISABLE_COPY_ASSIGNMENT(F3DImageLoader);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_IMAGELOADER_H__