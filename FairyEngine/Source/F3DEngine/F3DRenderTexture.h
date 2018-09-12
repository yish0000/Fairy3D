/*
 * ------------------------------------------------------------------------
 *  Name:   F3DRenderTexture.h
 *  Desc:   ���ļ�Ϊ���涨����һ��������ʽ����ȾĿ�ꡣ
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


/** ������ȾĿ��
@remarks
    ��������Ⱦ�������ϣ�Ȼ������2D������ʽʹ�á�
    �������������Ч�����磺ˮ�浹Ӱ�ȵȣ�
@note
*/
class FAIRY_API F3DRenderTexture : public F3DRenderTarget
{
protected:
    F3DTextureBase* m_pTexture;     // �洢��Ⱦ���������

public:
    F3DRenderTexture( const AString& name );
    ~F3DRenderTexture(void);

    const F3DTextureBase* GetTextureTarget(void) const { return m_pTexture; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_RENDERTEXTURE_H__