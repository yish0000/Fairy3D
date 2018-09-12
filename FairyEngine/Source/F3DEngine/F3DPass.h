/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPass.h
 *  Desc:   ���ļ�Ϊ���涨����һ����Ⱦͨ·����
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

/** ��Ⱦͨ·
@remarks
    ÿ����Ⱦͨ·
@note
*/
class FAIRY_API F3DPass : public FGeneralAlloc
{
    // ����Ϊ��Ⱦͨ·����һ����ϣֵ�������ô�ֵ����Ⱦͨ·
    // ���������Լ��ٵ��л�ͨ·ʱ��Ⱦ״̬�ĸı�
    struct MakeHash
    {
        uint32 operator () ( const F3DPass* p ) const;
    };

protected:
    AString m_sName;            // ��Ⱦͨ·������
    F3DTechnique* m_pParent;    // ��ͨ·�����ļ���
    uint32 m_nHash;             // ��ͨ·�Ĺ�ϣֵ

    F3DRenderStateSet m_RSSet;    // ��Ⱦ״̬��
//    F3DSamplerStateSet m_Samplers[MAX_TEXTURE_STAGES];   // ���������

    F3DShaderInstance* m_pVS;   // ������ɫ������ʵ��
    F3DShaderInstance* m_pPS;   // ������ɫ������ʵ��

public:
    F3DPass( const AString& name,F3DTechnique* technique );
	virtual ~F3DPass(void) {}

    // �Ƿ�Ϊ�ɱ����Ⱦ����(��һ��VS����PSʵ��)
    bool IsProgrammable(void) const;

    // ��ȡ�����޳�ģʽ
    ECullMode GetCullMode(void) const { return m_RSSet.CullMode; }

    // ��ȡ��������ģʽ
    EFillMode GetFillMode(void) const { return m_RSSet.FillMode; }

    // ��ȡ��ɫģʽ
    EShadeMode GetShadeMode(void) const { return m_RSSet.ShadeMode; }

    // ��ȡ��Ȼ���ģʽ
    EDepthMode GetDepthMode(void) const { return m_RSSet.DepthMode; }

    // ��ȡ��Ȼ���Ƚ�ģʽ
    void GetDepthFunc( ECompareFunc& cmpFunc,float& bias,float& slopeScale ) const;

    // �Ƿ���ģ�����
    bool IsStencilEnable(void) const { return m_RSSet.bFrontStencilEnable; }

    // �Ƿ�������ģ�����
    bool IsCCWStencilEnable(void) const { return m_RSSet.bBackStencilEnable; }

    // ��ȡģ����ԱȽϷ���
    void GetStencilFunc( ECompareFunc& funcF,ECompareFunc& funcB ) const;

    // ��ȡģ����Բο�ֵ
    int GetStencilRef(void) const { return m_RSSet.nStencilRef; }

    // ��ȡģ���������
    void GetStencilMask( uint32& mask,uint32& writeMask ) const;

    // ��ȡģ����Բ���
    void GetStencilOperationF( EStencilOperation& fail,EStencilOperation& zFail,
        EStencilOperation& pass ) const;

    // ��ȡ����ģ����Բ���
    void GetStencilOperationB( EStencilOperation& fail,EStencilOperation& zFail,
        EStencilOperation& pass ) const;

    // �Ƿ����˰�͸�����
//    bool IsAlphaBlendEnable(void) const { return m_RSSet.bAlphaBlend; }

    // �Ƿ����˰�͸������
//    bool IsAlphaTestEnable(void) const { return m_RSSet.bAlphaTest; }

    // ��ȡ��͸�����ԱȽϷ���
    void GetAlphaTestFunc( ECompareFunc& cmpFunc,uint32& cmpValue ) const;

    // ��ȡ��͸�����ģʽ
//    void GetAlphaBlendMode( EAlphaBlendMode& srcBlend,EAlphaBlendMode& destBlend ) const;

    // �Ƿ�����̬����
//    bool IsDynLighting(void) const { return m_RSSet.bLighting; }

    // �Ƿ�������߹�
   // bool IsSpecularLighting(void) const { return m_RSSet.bSpecularLight; }

    // ��ȡ����������(�����豸��̬����)
    void GetSurfaceMaterial( SMaterialDesc& material ) const;

    // ��ȡ��Ⱦͨ·������
    const AString& GetName(void) const { return m_sName; }
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PASS_H__