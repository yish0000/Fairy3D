/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderTexture.h
 *  Desc:   本文件为引擎定义了一个纹理形式的渲染目标。
 *  Author: Yish
 *  Date:   2010/12/18
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_RENDERTEXTURE_H__
#define __F3D_RENDERTEXTURE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DRenderTarget.h"
#include "F3DTexture.h"

///////////////////////////////////////////////////////////////////////////


/** 纹理渲染目标
@remarks
    将场景渲染到纹理上，然后再以2D纹理形式使用。
    可以做出相关特效（例如：水面倒影等等）
@note
*/
class FAIRY_API F3DRenderTexture : public F3DRenderTarget
{
protected:
    F3DTextureBase* m_pTexture;     // 存储渲染结果的纹理

public:
    F3DRenderTexture( const AString& name );
    ~F3DRenderTexture(void);

    const F3DTextureBase* GetTextureTarget(void) const { return m_pTexture; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RENDERTEXTURE_H__