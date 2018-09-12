//------------------------------------------------------------------------
// Name: FlyTechnique.h
// Desc: This file define a class contain the render technique.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_TECHNIQUE_H__
#define __FLY_TECHNIQUE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyRenderState.h"
#include "FlyShaderObject.h"
//////////////////////////////////////////////////////////////////////////


class FlyRenderTechnique;
class FlyRenderPass;

//////////////////////////////////////////////////////////////////////////
// Class contain some pass.

class _FLY_EXPORT FlyRenderTechnique
{
protected:
    std::string m_sName; // Name of the technique.
    FlyRenderEffect* m_pParent; // Pointer to the render effect.
    std::vector<FlyRenderAnnotation*> m_Annotations; // Annotations of the technique.
    std::vector<FlyRenderPass*> m_Passes; // Render passes of the technique.

public:
    // Constructor and destructor.
    FlyRenderTechnique( FlyRenderEffect* pParent );
    ~FlyRenderTechnique(void);

    FlyRenderPass* AddPass( const char* cName );
    FlyRenderAnnotation* AddAnnotation( const char* cName );

    HRESULT ApplyPass( int nPass );

    const char* GetName(void) const;
    void SetName( const char* cName );

    UINT GetNumAnnotations(void) const;
    FlyRenderAnnotation* GetAnnotationByIndex( UINT nIndex );
    FlyRenderAnnotation* GetAnnotationByName( const char* cName );

    UINT GetNumPasses(void) const;
    FlyRenderPass* GetPassByIndex( UINT nIndex );
    FlyRenderPass* GetPassByName( const char* cName );

    FlyRenderEffect* GetRenderEffect(void);
};

//////////////////////////////////////////////////////////////////////////
// Class contain the render states for the device.

class _FLY_EXPORT FlyRenderPass
{
    friend class FlyRenderEffect;

protected:
    LPFLYRENDERDEVICE m_pDevice;    // Pointer to the RENDERDEVICE.

    std::string m_sName;                                // Name of the render pass.
    FlyRenderTechnique* m_pParent;                      // Parent of the render pass.
    std::vector<FlyRenderAnnotation*> m_Annotations;    // Annotation list.

    FlyRenderStateBuffer m_RenderStates;                // Render state buffer.
    FlyShaderObject::ShaderDesc m_ShaderDesc[2];        // Shader desc for this pass.
    FlyShaderObject* m_pShaderObj;                      // Pointer to the Shader object.

public:
    // Constructor and destructor.
    FlyRenderPass( FlyRenderTechnique* pParent );
    ~FlyRenderPass(void);

    FlyRenderAnnotation* AddAnnotation( const char* cName );
    void SetVShaderDesc( const char* cFuncName,const char* cProfile );
    void SetPShaderDesc( const char* cFuncName,const char* cProfile );
    HRESULT CreateShader( const char* cShaderText,std::string& errInfo );

    // Apply the render pass.
    HRESULT Apply(void);

    const char* GetName(void) const;
    void SetName( const char* cName );

    UINT GetNumAnnotations(void) const;
    FlyRenderAnnotation* GetAnnotationByIndex( UINT nIndex );
    FlyRenderAnnotation* GetAnnotationByName( const char* cName );

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

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_TECHNIQUE_H__