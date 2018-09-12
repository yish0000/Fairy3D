/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTechnique.h
 *  Desc:   本文件定义了材质系统的渲染技术对象。
 *  Author: Yish
 *  Date:   2010/11/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_TECHNIQUE_H__
#define __F3D_TECHNIQUE_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"
#include "F3DShaderObject.h"

///////////////////////////////////////////////////////////////////////////


class F3DPass;

#if 0
/** 渲染技术类
*/
class _F3D_EXPORT F3DTechnique : public FGeneralAlloc
{
    typedef std::vector<FlyRenderAnnotation*> AnnotationList;
    typedef std::vector<F3DPass*> PassList;

protected:
    FString m_sName;                // 技术的名字
    FlyRenderEffect* m_pParent;     // 父渲染效果的指针

    AnnotationList m_Annotations;   // 技术的评注列表
    PassList m_Passes;              // 批次列表

public:
    FlyRenderTechnique( FlyRenderEffect* pParent );
    ~FlyRenderTechnique(void);

    FlyRenderPass* AddPass( const FString& name );
    FlyRenderAnnotation* AddAnnotation( const FString& name );

    HRESULT ApplyPass( UINT nPass );

    const FString& GetName(void) const { return m_sName; }
    void SetName( const FString& name ) { m_sName = name; }

    size_t GetNumAnnotations(void) const { return m_Annotations.size(); }
    FlyRenderAnnotation* GetAnnotationByIndex( size_t nIndex );
    FlyRenderAnnotation* GetAnnotationByName( const FString& name );

    size_t GetNumPasses(void) const { return m_Passes.size(); }
    FlyRenderPass* GetPassByIndex( size_t nIndex );
    FlyRenderPass* GetPassByName( const FString& name );

    FlyRenderEffect* GetRenderEffect(void) { return m_pParent; }
};


/** 渲染批次
@remarks
@note
*/
class _FLY_EXPORT FlyRenderPass
{
    friend class FlyRenderEffect;

protected:
    FString m_sName;                                    // 批次的名字
    FlyRenderTechnique* m_pParent;                      // 父技术的指针
    std::vector<FlyRenderAnnotation*> m_Annotations;    // 批次的评注列表

    FlyRenderStateBuffer m_RenderStates;                // 本批次的渲染状态集
    FlyShaderObject::ShaderDesc m_ShaderDesc[2];        // 本批次的Shader描述
    FlyShaderObject* m_pShaderObj;                      // Shader对象指针

public:
    FlyRenderPass( FlyRenderTechnique* pParent );
    ~FlyRenderPass(void);

    FlyRenderAnnotation* AddAnnotation( const FString& name );
    void SetVShaderDesc( const FString& funcName,const FString& profile );
    void SetPShaderDesc( const FString& funcName,const FString& profile );
    HRESULT CreateShader( const FString& shaderText,FString& errInfo );

    // 应用这个渲染批次
    HRESULT Apply(void);

    const FString& GetName(void) const { return m_sName; }
    void SetName( const FString& name ) { m_sName = name; }

    size_t GetNumAnnotations(void) const;
    FlyRenderAnnotation* GetAnnotationByIndex( size_t nIndex );
    FlyRenderAnnotation* GetAnnotationByName( const FString& name );

    const FlyShaderObject* GetShaderObject(void) const;
    FlyRenderTechnique* GetTechnique(void);

    //////////////////////////////////////////////////////////////////////

    // Get the attributes for render material.
    CULLMODE GetCullMode(void) const;
    POLYGONMODE GetPolygonMode(void) const;
    SHADEMODE GetShadeMode(void) const;
    DEPTHMODE GetDepthMode(void) const;
    void GetDepthFunc( FLYCMPFUNC* pCmpFunc,float* pBias,float* pSlopeScale ) const;
    bool IsStencilEnable(void) const;
    bool IsCCWStencilEnable(void) const;
    void GetStencilFunc( FLYCMPFUNC* pFuncF,FLYCMPFUNC* pFuncB ) const;
    int GetStencilRef(void) const;
    void GetStencilMask( DWORD* pMask,DWORD* pWriteMask ) const;
    void GetStencilOperationF( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass ) const;
    void GetStencilOperationB( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass ) const;
    bool IsAlphaBlendOpen(void) const;
    bool IsAlphaTestOpen(void) const;
    void GetAlphaTestFunc( FLYCMPFUNC* pCmpFunc,DWORD* pCmpValue ) const;
    void GetAlphaBlendMode( SCENEBLENDMODE* pSrc,SCENEBLENDMODE* pDest ) const;
    void GetStdMaterial( FLYMATERIAL* pMaterial ) const;
    bool IsLighting(void) const;
    bool IsSpecularEnabled(void) const;
    int GetTextureCoordIndex( int nStage ) const;
    void GetTextureBlendMode( int nStage,BLENDTYPE Type,TEXTUREBLENDMODE* pBlendMode ) const;

    // Set the attributes for render material.
    void SetCullMode( CULLMODE mode );
    void SetPolygonMode( POLYGONMODE mode );
    void SetShadeMode( SHADEMODE mdoe );
    void SetDepthMode( DEPTHMODE mode );
    void SetDepthFunc( FLYCMPFUNC func,float fBias,float fSlopeScale );
    void SetStencilMode( bool bFront,bool bBack );
    void SetStencilFunc( FLYCMPFUNC frontFunc,FLYCMPFUNC backFunc );
    void SetStencilOperationF( STENCILOPERATION failOp,STENCILOPERATION zFailOp,STENCILOPERATION passOp );
    void SetStencilOperationB( STENCILOPERATION failOp,STENCILOPERATION zFailOp,STENCILOPERATION passOp );
    void SetStencilRef( int nStencilRef );
    void SetStencilMask( DWORD dwMask,DWORD dwWriteMask );
    void SetSceneAlphaMode( bool bAlphaBlend,bool bAlphaTest );
    void SetAlphaTestFunc( FLYCMPFUNC func,DWORD dwRef );
    void SetAlphaBlendMode( SCENEBLENDMODE src,SCENEBLENDMODE dest );
    void SetStdMaterial( const FLYMATERIAL* pMaterial );
    void SetLightingMode( bool bLighting );
    void SetSpecularMode( bool bSpecular );
    void SetTextureCoordIndex( int nStage,int nCoordIndex );
    void SetTextureBlendMode( int nStage,const TEXTUREBLENDMODE& blend );
};

#endif

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TECHNIQUE_H__