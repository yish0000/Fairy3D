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

    const char* GetName(void);
    void SetName( const char* cName );

    UINT GetNumAnnotations(void);
    FlyRenderAnnotation* GetAnnotationByIndex( int nIndex );
    FlyRenderAnnotation* GetAnnotationByName( const char* cName );

    UINT GetNumPasses(void);
    FlyRenderPass* GetPassByIndex( int nIndex );
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

    const char* GetName(void);
    void SetName( const char* cName );

    UINT GetNumAnnotations(void);
    FlyRenderAnnotation* GetAnnotationByIndex( int nIndex );
    FlyRenderAnnotation* GetAnnotationByName( const char* cName );

    FlyShaderObject* GetShaderObject(void);
    FlyRenderTechnique* GetTechnique(void);

    //////////////////////////////////////////////////////////////////////

    // Get the attributes for render material.
    CULLMODE GetCullMode(void);
    POLYGONMODE GetPolygonMode(void);
    SHADEMODE GetShadeMode(void);
    DEPTHMODE GetDepthMode(void);
    void GetDepthFunc( FLYCMPFUNC* pCmpFunc,float* pBias,float* pSlopeScale );
    void GetStencilMode( bool* pStencilEnableF,bool* pStencilEnableB );
    void GetStencilFunc( FLYCMPFUNC* pFuncF,FLYCMPFUNC* pFuncB );
    int GetStencilRef(void);
    void GetStencilMask( DWORD* pMask,DWORD* pWriteMask );
    void GetStencilOperationF( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass );
    void GetStencilOperationB( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass );
    bool IsAlphaBlendOpen(void);
    bool IsAlphaTestOpen(void);
    void GetAlphaTestFunc( FLYCMPFUNC* pCmpFunc,DWORD* pCmpValue );
    void GetAlphaBlendMode( SCENEBLENDMODE* pSrc,SCENEBLENDMODE* pDest );
    void GetStdMaterial( FLYMATERIAL* pMaterial );
    bool IsLighting(void);
    bool IsSpecularEnabled(void);

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
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_TECHNIQUE_H__