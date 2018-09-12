//------------------------------------------------------------------------
// Name: FlyRenderMaterial.h
// Desc: This file define an interface RenderMaterial, It contain all
//       the states which can have effect on the rendering, Light,
//       material, RenderState, TextureState, SamplerState, fog,
//       cullmode, shader ...
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDERMATIERAL_H__
#define __FLY_RENDERMATIERAL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTexture.h"

#include "FlyRenderEffectInstance.h"
#include "FlyTechnique.h"
//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyRenderMaterial
{
    struct _Pass
    {
        FlySampler Samplers[MAX_TEXTURES];
        FlyRenderStateBuffer RenderStates;
        bool bModified;
    };
    typedef std::vector<_Pass> FixedPipelinePassList;

    struct _Technique
    {
        FlyRenderEffectInstance* pGPUProgram;
        FlyRenderTechnique* pTechnique;
    };
    typedef std::vector<_Technique> TechniqueList;

protected:
    std::string m_sName;                    // Name of the material.
    LPFLYRENDERDEVICE m_pDevice;            // Pointer to the Render Device.

    // Member for the fixed pipeline.
    FixedPipelinePassList m_FixedPLPasses;  // Render passes for the fixed pipeline.

    // Member for the programed pipeline.
    TechniqueList m_Techniques;             // Render technique list.
    UINT m_nActiveTechnique;                // Current active technique.

public:
    // Constructor and destructor.
    FlyRenderMaterial(void);
    ~FlyRenderMaterial(void);

    // Create a new render pass for the material.
    int AddNewFixedPass(void);

    // Destroy the render pass.
    void RemoveFixedPass( int nPass );

    // Get the count of render passes.
    UINT GetNumPasses(void) const;

    // Is the material Use fixed pipeline or
    // programable pipeline ?
    bool IsFixedPipeline(void) const;

    // Check whether the material has transparent effect ?
    bool IsTransparent(void) const;

    // Apply the render material.
    HRESULT ApplyPass( int nPass );

    // Insert a new technique to the material.
    void AddNewTechnique( FlyRenderTechnique* pTechnique );
    void RemoveTechnique( UINT nIndex );
    void RemoveTechniqueByName( const char* cName );
    void RemoveAllTechniques(void);

    // Get and set the technique pointer.
    FlyRenderTechnique* GetTechnique( UINT nIndex );
    FlyRenderTechnique* GetTechniqueByName( const char* cName );
    FlyRenderTechnique* GetActiveTechnique(void);

    FlyRenderEffectInstance* GetRenderEffectInstance( UINT nIndex );
    FlyRenderEffectInstance* GetActiveRenderEffectInstance(void);

    void SetActiveTechnique( UINT nIndex );
    void SetActiveTechniqueByName( const char* cName );

    // Get the attributes for render material.
    CULLMODE GetCullMode( int nPass ) const;
    POLYGONMODE GetPolygonMode( int nPass ) const;
    SHADEMODE GetShadeMode( int nPass ) const;
    DEPTHMODE GetDepthMode( int nPass ) const;
    void GetDepthFunc( int nPass,FLYCMPFUNC* pCmpFunc,float* pBias,float* pSlopeScale ) const;
    bool IsStencilEnable( int nPass ) const;
    bool IsCCWStencilEnable( int nPass ) const;
    void GetStencilFunc( int nPass,FLYCMPFUNC* pFuncF,FLYCMPFUNC* pFuncB ) const;
    int GetStencilRef( int nPass ) const;
    void GetStencilMask( int nPass,DWORD* pMask,DWORD* pWriteMask ) const;
    void GetStencilOperationF( int nPass,STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass ) const;
    void GetStencilOperationB( int nPass,STENCILOPERATION* pFail,STENCILOPERATION* pZFail,STENCILOPERATION* pPass ) const;
    bool IsAlphaBlendOpen( int nPass ) const;
    bool IsAlphaTestOpen( int nPass ) const;
    void GetAlphaTestFunc( int nPass,FLYCMPFUNC* pCmpFunc,DWORD* pCmpValue ) const;
    void GetAlphaBlendMode( int nPass,SCENEBLENDMODE* pSrc,SCENEBLENDMODE* pDest ) const;
    bool IsLighting( int nPass ) const;
    bool IsSpecularEnabled( int nPass ) const;
    void GetStdMaterial( int nPass,FLYMATERIAL* pMaterial ) const;
    void GetTextureBlendMode( int nPass,int nStage,BLENDTYPE Type,TEXTUREBLENDMODE* pBlendMode ) const;
    int GetTextureCoordIndex( int nPass,int nStage ) const;

    FlyTexture* GetTexture( int nPass,int nStage );
    void GetTextureAddressingMode( int nPass,int nStage,TEXTUREADDRESS* pAddrU,TEXTUREADDRESS* pAddrV,TEXTUREADDRESS* pAddrW ) const;
    void GetTextureFilterMode( int nPass,int nStage,FILTEROPTION* pMagFO,FILTEROPTION* pMinFO,FILTEROPTION* pMipFO ) const;

    // Set the attributes for render material.
    void SetCullMode( int nPass,CULLMODE mode );
    void SetPolygonMode( int nPass,POLYGONMODE mode );
    void SetShadeMode( int nPass,SHADEMODE mdoe );
    void SetDepthMode( int nPass,DEPTHMODE mode );
    void SetDepthFunc( int nPass,FLYCMPFUNC func,float fBias,float fSlopeScale );
    void SetStencilMode( int nPass,bool bFront,bool bBack );
    void SetStencilFunc( int nPass,FLYCMPFUNC funcF,FLYCMPFUNC funcB );
    void SetStencilRef( int nPass,int nStencilRef );
    void SetStencilMask( int nPass,DWORD dwMask,DWORD dwWriteMask );
    void SetStencilOperationF( int nPass,STENCILOPERATION failOp,STENCILOPERATION zFailOp,STENCILOPERATION passOp );
    void SetStencilOperationB( int nPass,STENCILOPERATION failOp,STENCILOPERATION zFailOp,STENCILOPERATION passOp );
    void SetSceneAlphaMode( int nPass,bool bAlphaBlend,bool bAlphaTest );
    void SetAlphaTestFunc( int nPass,FLYCMPFUNC func,DWORD dwRef );
    void SetAlphaBlendMode( int nPass,SCENEBLENDMODE src,SCENEBLENDMODE dest );
    void SetLightingMode( int nPass,bool bLighting );
    void SetSpecularMode( int nPass,bool bEnable );
    void SetStdMaterial( int nPass,const FLYMATERIAL* pMaterial );
    void SetTextureCoordIndex( int nPass,int nStage,int nCoordIndex );
    void SetTextureBlendMode( int nPass,int nStage,const TEXTUREBLENDMODE& blend );

    void SetTexture( int nPass,int nStage,FlyTexture* pTexture );
    void SetTextureAddressingMode( int nPass,int nStage,TEXTUREADDRESS addrU,TEXTUREADDRESS addrV,TEXTUREADDRESS addrW );
    void SetTextureFilterMode( int nPass,int nStage,FILTEROPTION magFO,FILTEROPTION minFO,FILTEROPTION mipFO );

    const char* GetName(void) const;
    void SetName( const char* cName );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDERMATERIAL_H__