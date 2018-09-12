/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPass.h
 *  Desc:   本文件为引擎定义了一个渲染通路对象。
 *  Author: Yish
 *  Date:   2011/3/23
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_PASS_H__
#define __F3D_PASS_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DRenderStateSet.h"

///////////////////////////////////////////////////////////////////////////


class F3DTechnique;

/** 渲染通路
@remarks
    每个渲染通路
@note
*/
class FAIRY_API F3DPass : public FGeneralAlloc
{
    // 用于为渲染通路产生一个哈希值，我们用此值对渲染通路
    // 进行排序，以减少当切换通路时渲染状态的改变
    struct MakeHash
    {
        uint32 operator () ( const F3DPass* p ) const;
    };

protected:
    AString m_sName;            // 渲染通路的名字
    F3DTechnique* m_pParent;    // 该通路所属的技术
    uint32 m_nHash;             // 该通路的哈希值

    F3DRenderStateSet m_RSSet;    // 渲染状态集
//    F3DSamplerStateSet m_Samplers[MAX_TEXTURE_STAGES];   // 纹理层数组

    F3DShaderInstance* m_pVS;   // 顶点着色器程序实例
    F3DShaderInstance* m_pPS;   // 像素着色器程序实例

public:
    F3DPass( const AString& name,F3DTechnique* technique );
	virtual ~F3DPass(void) {}

    // 是否为可编程渲染管线(有一个VS或者PS实例)
    bool IsProgrammable(void) const;

    // 获取背面剔除模式
    ECullMode GetCullMode(void) const { return m_RSSet.CullMode; }

    // 获取多边形填充模式
    EFillMode GetFillMode(void) const { return m_RSSet.FillMode; }

    // 获取着色模式
    EShadeMode GetShadeMode(void) const { return m_RSSet.ShadeMode; }

    // 获取深度缓冲模式
    EDepthMode GetDepthMode(void) const { return m_RSSet.DepthMode; }

    // 获取深度缓冲比较模式
    void GetDepthFunc( ECompareFunc& cmpFunc,float& bias,float& slopeScale ) const;

    // 是否开启模板测试
    bool IsStencilEnable(void) const { return m_RSSet.bFrontStencilEnable; }

    // 是否开启背面模板测试
    bool IsCCWStencilEnable(void) const { return m_RSSet.bBackStencilEnable; }

    // 获取模板测试比较方法
    void GetStencilFunc( ECompareFunc& funcF,ECompareFunc& funcB ) const;

    // 获取模板测试参考值
    int GetStencilRef(void) const { return m_RSSet.nStencilRef; }

    // 获取模板测试掩码
    void GetStencilMask( uint32& mask,uint32& writeMask ) const;

    // 获取模板测试操作
    void GetStencilOperationF( EStencilOperation& fail,EStencilOperation& zFail,
        EStencilOperation& pass ) const;

    // 获取背面模板测试操作
    void GetStencilOperationB( EStencilOperation& fail,EStencilOperation& zFail,
        EStencilOperation& pass ) const;

    // 是否开启了半透明混合
//    bool IsAlphaBlendEnable(void) const { return m_RSSet.bAlphaBlend; }

    // 是否开启了半透明测试
//    bool IsAlphaTestEnable(void) const { return m_RSSet.bAlphaTest; }

    // 获取半透明测试比较方法
    void GetAlphaTestFunc( ECompareFunc& cmpFunc,uint32& cmpValue ) const;

    // 获取半透明混合模式
//    void GetAlphaBlendMode( EAlphaBlendMode& srcBlend,EAlphaBlendMode& destBlend ) const;

    // 是否开启动态光照
//    bool IsDynLighting(void) const { return m_RSSet.bLighting; }

    // 是否开启镜面高光
   // bool IsSpecularLighting(void) const { return m_RSSet.bSpecularLight; }

    // 获取物体表面材质(用于设备动态光照)
    void GetSurfaceMaterial( SMaterialDesc& material ) const;

    // 获取渲染通路的名字
    const AString& GetName(void) const { return m_sName; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PASS_H__