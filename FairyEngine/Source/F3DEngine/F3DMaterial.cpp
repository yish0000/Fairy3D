/*
 * ------------------------------------------------------------------------
 *  Name:   F3DMaterial.cpp
 *  Desc:   本文件实现了引擎的材质对象。
 *  Author: Yish
 *  Date:   2012/7/30
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DMaterial.h"
#include "F3DRenderDevice.h"


#if 0
//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyRenderMaterial::FlyRenderMaterial(void)
{
    _Pass pass;
    pass.RenderStates.InitRenderStates();
    for( int i=0;i<MAX_TEXTURES;i++ )
        pass.Samplers[i].InitSamplerStates();
    pass.bModified = true;
    m_FixedPLPasses.push_back( pass );

    m_nActiveTechnique = -1;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyRenderMaterial::~FlyRenderMaterial(void)
{
    TechniqueList::iterator itr;
    for( itr=m_Techniques.begin();itr!=m_Techniques.end();itr++ )
        SAFE_DELETE( itr->pGPUProgram );
}


//------------------------------------------------------------------------
// Create a new render pass for the material.
//------------------------------------------------------------------------
UINT FlyRenderMaterial::AddNewFixedPass(void)
{
    _Pass pass;
    pass.RenderStates.InitRenderStates();

    for( int i=0;i<MAX_TEXTURES;i++ )
        pass.Samplers[i].InitSamplerStates();

    pass.bModified = true;
    m_FixedPLPasses.push_back( pass );
    return (UINT)m_FixedPLPasses.size()-1;
}


//------------------------------------------------------------------------
// Destroy the specified render pass.
//------------------------------------------------------------------------
void FlyRenderMaterial::RemoveFixedPass( size_t nPass )
{
    if( nPass < m_FixedPLPasses.size() )
        m_FixedPLPasses.erase( m_FixedPLPasses.begin()+nPass );
}


//------------------------------------------------------------------------
// Get the count of render passes.
//------------------------------------------------------------------------
UINT FlyRenderMaterial::GetNumPasses(void) const
{
    return (UINT)m_FixedPLPasses.size();
}


//------------------------------------------------------------------------
// Does the render material use fixed pipeline ?
//------------------------------------------------------------------------
bool FlyRenderMaterial::IsFixedPipeline(void) const
{
    return (m_nActiveTechnique == -1);
}

//------------------------------------------------------------------------
// Check whether the material has transparent effect ?
//------------------------------------------------------------------------
bool FlyRenderMaterial::IsTransparent(void) const
{
    if( m_nActiveTechnique != -1 )
    {
        FlyRenderPass* pPass = m_Techniques[m_nActiveTechnique].pTechnique->GetPassByIndex(0);
        return pPass->IsAlphaBlendOpen() || pPass->IsAlphaTestOpen();
    }
    else
    {
        return m_FixedPLPasses[0].RenderStates.bAlphaBlend ||
            m_FixedPLPasses[0].RenderStates.bAlphaTest;
    }
}


//------------------------------------------------------------------------
// Add a new technique to the material.
//------------------------------------------------------------------------
void FlyRenderMaterial::AddNewTechnique( FlyRenderTechnique* pTechnique )
{
    // Check whether the technique existed.
    TechniqueList::iterator itr;
    for( itr=m_Techniques.begin();itr!=m_Techniques.end();itr++ )
    {
        if( itr->pTechnique == pTechnique )
            return;
    }

    _Technique newTech;
    newTech.pTechnique = pTechnique;
    newTech.pGPUProgram = new FlyRenderEffectInstance( pTechnique->GetRenderEffect() );
    m_Techniques.push_back( newTech );
}


//------------------------------------------------------------------------
// Remove the specified technique by its index.
//------------------------------------------------------------------------
void FlyRenderMaterial::RemoveTechnique( UINT nIndex )
{
    if( nIndex >= m_Techniques.size() )
        return;

    if( nIndex == m_nActiveTechnique )
        m_nActiveTechnique = -1;

    SAFE_DELETE( m_Techniques[nIndex].pGPUProgram );
    m_Techniques.erase( m_Techniques.begin() + nIndex );
}


//------------------------------------------------------------------------
// Remove the specified technique by its name.
//------------------------------------------------------------------------
void FlyRenderMaterial::RemoveTechniqueByName( const char* cName )
{
    for( size_t i=0;i<m_Techniques.size();i++ )
    {
        if( strcmp(m_Techniques[i].pTechnique->GetName(),cName) == 0 )
        {
            if( m_nActiveTechnique == i )
                m_nActiveTechnique = -1;

            SAFE_DELETE( m_Techniques[i].pGPUProgram );
            m_Techniques.erase( m_Techniques.begin() + i );
            break;
        }
    }
}


//------------------------------------------------------------------------
// Remove all the techniques.
//------------------------------------------------------------------------
void FlyRenderMaterial::RemoveAllTechniques(void)
{
    TechniqueList::iterator itr;
    for( itr=m_Techniques.begin();itr!=m_Techniques.end();itr++ )
        SAFE_DELETE( itr->pGPUProgram );
    m_Techniques.clear();
    m_nActiveTechnique = -1;
}


//------------------------------------------------------------------------
// Get the technique pointer of the material.
//------------------------------------------------------------------------
FlyRenderTechnique* FlyRenderMaterial::GetTechnique( UINT nIndex )
{
    return nIndex < m_Techniques.size() ? m_Techniques[nIndex].pTechnique : NULL;
}


//------------------------------------------------------------------------
// Get the technique by its name.
//------------------------------------------------------------------------
FlyRenderTechnique* FlyRenderMaterial::GetTechniqueByName( const char* cName )
{
    TechniqueList::iterator itr;
    for( itr=m_Techniques.begin();itr!=m_Techniques.end();itr++ )
    {
        if( strcmp(itr->pTechnique->GetName(),cName) == 0 )
            return itr->pTechnique;
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the current active technique.
//------------------------------------------------------------------------
FlyRenderTechnique* FlyRenderMaterial::GetActiveTechnique(void)
{
    return m_Techniques[m_nActiveTechnique].pTechnique;
}


//------------------------------------------------------------------------
// Set the active technique for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the active technique.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetActiveTechnique( UINT nIndex )
{
    if( nIndex >= m_Techniques.size() )
        return;

    m_nActiveTechnique = nIndex;
}


//------------------------------------------------------------------------
// Set the active technique by its name.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetActiveTechniqueByName( const char* cName )
{
    for( UINT i=0;i<m_Techniques.size();i++ )
    {
        if( strcmp(m_Techniques[i].pTechnique->GetName(),cName) == 0 )
        {
            m_nActiveTechnique = i;
            return;
        }
    }
}


//------------------------------------------------------------------------
// Get the specified renderEffect instance.
//------------------------------------------------------------------------
FlyRenderEffectInstance* FlyRenderMaterial::GetRenderEffectInstance( UINT nIndex )
{
    return m_Techniques[nIndex].pGPUProgram;
}

//------------------------------------------------------------------------
// Get the active renderEffect instance.
//------------------------------------------------------------------------
FlyRenderEffectInstance* FlyRenderMaterial::GetActiveRenderEffectInstance(void)
{
    return m_Techniques[m_nActiveTechnique].pGPUProgram;
}


//------------------------------------------------------------------------
// Apply the render material for the render device.
//------------------------------------------------------------------------
HRESULT FlyRenderMaterial::ApplyPass( int nPass )
{
    // 获取设备指针
    LPFLYRENDERDEVICE pDevice = FlyKernel::Instance().GetRenderDevice();

    if( pDevice->GetActiveMaterial() == this &&
        pDevice->GetActiveFixedPass() == nPass &&
        !m_FixedPLPasses[nPass].bModified )
        return FLY_OK;

    FlyRenderStateBuffer* pRS = &m_FixedPLPasses[nPass].RenderStates;
    FlySampler* pSamplers = m_FixedPLPasses[nPass].Samplers;

    // 1. Handle the rasterizer state.
    pDevice->SetCullMode( pRS->CullMode );
    pDevice->SetPolygonMode( pRS->PolygonMode );
    pDevice->SetShadeMode( pRS->ShadeMode );

    // 2. Handle the depth buffer states.
    pDevice->SetDepthBufferMode( pRS->DepthMode );
    pDevice->SetDepthFunc( pRS->DepthFunc,pRS->fDepthBias,pRS->fSlopeScale );

    // 3. Handle the stencil buffer states.
    pDevice->SetStencilMode( pRS->bFrontStencilEnable,pRS->bBackStencilEnable );
    pDevice->SetStencilFunc( pRS->StencilFunc,pRS->BackStencilFunc );
    pDevice->SetStencilRef( pRS->nStencilRef );
    pDevice->SetStencilMask( pRS->dwStencilMask,pRS->dwStencilWriteMask );
    pDevice->SetStencilOperationF( pRS->StencilFail,pRS->StencilDepthFail,pRS->StencilPass );
    pDevice->SetStencilOperationB( pRS->BackStencilFail,pRS->BackStencilDepthFail,pRS->BackStencilPass );

    // 4. Handle the blend states.
    pDevice->SetSceneAlphaMode( pRS->bAlphaBlend,pRS->bAlphaTest );
    pDevice->SetAlphaBlendMode( pRS->SourceBlend,pRS->DestBlend );
    pDevice->SetAlphaTestFunc( pRS->AlphaTestFunc,pRS->dwAlphaTestRef );

    // 5. Handle the light stuffs.
    pDevice->SetLightingMode( pRS->bLighting );
    pDevice->SetSpecularMode( pRS->bSpecularLight );
    pDevice->SetStdMaterial( &pRS->StdMaterial );

    // 6. Handle the samplers.
    for( int i=0;i<MAX_TEXTURES;i++ )
    {
        pDevice->SetTexture( i,pSamplers[i].pTexture );
        pDevice->SetTextureAddressingMode( i,pSamplers[i].AddressU,pSamplers[i].AddressV,pSamplers[i].AddressW );
        pDevice->SetTextureFilterMode( i,pSamplers[i].MagFilter,pSamplers[i].MinFilter,pSamplers[i].MipFilter );

        pDevice->SetTextureBlendingMode( i,pRS->ColorBlend[i] );
        pDevice->SetTextureBlendingMode( i,pRS->AlphaBlend[i] );
        pDevice->SetTextureCoordIndex( i,pRS->CoordSetIndex[i] );
    }

    // 6. Shutdown the shaders.
    pDevice->SetShaderObject( NULL );

    pDevice->SetActiveMaterial( this );
    pDevice->SetActiveFixedPass( nPass );
    m_FixedPLPasses[nPass].bModified = false;

    return FLY_OK;
}


/*************************************************************************
*                                                                        *
*     FUNCTIONS TO GET THE ATTRIBUTES FOR THE RENDER MATERIAL            *
*                                                                        *
*************************************************************************/

//------------------------------------------------------------------------
CULLMODE FlyRenderMaterial::GetCullMode( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.CullMode;
}

//------------------------------------------------------------------------
POLYGONMODE FlyRenderMaterial::GetPolygonMode( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.PolygonMode;
}

//------------------------------------------------------------------------
SHADEMODE FlyRenderMaterial::GetShadeMode( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.ShadeMode;
}

//------------------------------------------------------------------------
DEPTHMODE FlyRenderMaterial::GetDepthMode( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.DepthMode;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetDepthFunc( int nPass,FLYCMPFUNC* pCmpFunc,float* pBias,
                                      float* pSlopeScale ) const
{
    *pCmpFunc = m_FixedPLPasses[nPass].RenderStates.DepthFunc;
    *pBias = m_FixedPLPasses[nPass].RenderStates.fDepthBias;
    *pSlopeScale = m_FixedPLPasses[nPass].RenderStates.fSlopeScale;
}

//------------------------------------------------------------------------
bool FlyRenderMaterial::IsStencilEnable( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.bFrontStencilEnable;
}

//------------------------------------------------------------------------
bool FlyRenderMaterial::IsCCWStencilEnable( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.bBackStencilEnable;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetStencilFunc( int nPass,FLYCMPFUNC* pFuncF,FLYCMPFUNC* pFuncB ) const
{
    *pFuncF = m_FixedPLPasses[nPass].RenderStates.StencilFunc;
    *pFuncB = m_FixedPLPasses[nPass].RenderStates.BackStencilFunc;
}

//------------------------------------------------------------------------
int FlyRenderMaterial::GetStencilRef( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.nStencilRef;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetStencilMask( int nPass,DWORD* pMask,DWORD* pWriteMask ) const
{
    *pMask = m_FixedPLPasses[nPass].RenderStates.dwStencilMask;
    *pWriteMask = m_FixedPLPasses[nPass].RenderStates.dwStencilWriteMask;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetStencilOperationF( int nPass,STENCILOPERATION* pFail,STENCILOPERATION* pZFail,
                                              STENCILOPERATION* pPass ) const
{
    *pFail = m_FixedPLPasses[nPass].RenderStates.StencilFail;
    *pZFail = m_FixedPLPasses[nPass].RenderStates.StencilDepthFail;
    *pPass = m_FixedPLPasses[nPass].RenderStates.StencilPass;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetStencilOperationB( int nPass,STENCILOPERATION* pFail,STENCILOPERATION* pZFail,
                                              STENCILOPERATION* pPass ) const
{
    *pFail = m_FixedPLPasses[nPass].RenderStates.BackStencilFail;
    *pZFail = m_FixedPLPasses[nPass].RenderStates.BackStencilDepthFail;
    *pPass = m_FixedPLPasses[nPass].RenderStates.BackStencilPass;
}

//------------------------------------------------------------------------
bool FlyRenderMaterial::IsAlphaBlendOpen( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.bAlphaBlend;
}

//------------------------------------------------------------------------
bool FlyRenderMaterial::IsAlphaTestOpen( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.bAlphaTest;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetAlphaTestFunc( int nPass,FLYCMPFUNC* pCmpFunc,DWORD* pCmpValue ) const
{
    *pCmpFunc = m_FixedPLPasses[nPass].RenderStates.AlphaTestFunc;
    *pCmpValue = m_FixedPLPasses[nPass].RenderStates.dwAlphaTestRef;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetAlphaBlendMode( int nPass,SCENEBLENDMODE* pSrc,SCENEBLENDMODE* pDest ) const
{
    *pSrc = m_FixedPLPasses[nPass].RenderStates.SourceBlend;
    *pDest = m_FixedPLPasses[nPass].RenderStates.DestBlend;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetStdMaterial( int nPass,FLYMATERIAL* pMaterial ) const
{
    memcpy( pMaterial,&m_FixedPLPasses[nPass].RenderStates.StdMaterial,sizeof(FLYMATERIAL) );
}

//------------------------------------------------------------------------
bool FlyRenderMaterial::IsLighting( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.bLighting;
}

//------------------------------------------------------------------------
bool FlyRenderMaterial::IsSpecularEnabled( int nPass ) const
{
    return m_FixedPLPasses[nPass].RenderStates.bSpecularLight;
}

//------------------------------------------------------------------------
FlyTexture* FlyRenderMaterial::GetTexture( int nPass,int nStage )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return NULL;

    return m_FixedPLPasses[nPass].Samplers[nStage].pTexture;
}

//------------------------------------------------------------------------
int FlyRenderMaterial::GetTextureCoordIndex( int nPass,int nStage ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return -1;

    return m_FixedPLPasses[nPass].RenderStates.CoordSetIndex[nStage];
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetTextureAddressingMode( int nPass,int nStage,TEXTUREADDRESS* pAddrU,TEXTUREADDRESS* pAddrV,
                                                  TEXTUREADDRESS* pAddrW ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    *pAddrU = m_FixedPLPasses[nPass].Samplers[nStage].AddressU;
    *pAddrV = m_FixedPLPasses[nPass].Samplers[nStage].AddressV;
    *pAddrW = m_FixedPLPasses[nPass].Samplers[nStage].AddressW;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetTextureFilterMode( int nPass,int nStage,FILTEROPTION* pMagFO,FILTEROPTION* pMinFO,
                                              FILTEROPTION* pMipFO ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    *pMagFO = m_FixedPLPasses[nPass].Samplers[nStage].MagFilter;
    *pMinFO = m_FixedPLPasses[nPass].Samplers[nStage].MinFilter;
    *pMipFO = m_FixedPLPasses[nPass].Samplers[nStage].MipFilter;
}

//------------------------------------------------------------------------
void FlyRenderMaterial::GetTextureBlendMode( int nPass,int nStage,BLENDTYPE Type,
                                             TEXTUREBLENDMODE* pBlendMode ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    if( Type == BT_COLOR )
        memcpy( pBlendMode,&m_FixedPLPasses[nPass].RenderStates.ColorBlend[nStage],sizeof(TEXTUREBLENDMODE) );
    else
        memcpy( pBlendMode,&m_FixedPLPasses[nPass].RenderStates.AlphaBlend[nStage],sizeof(TEXTUREBLENDMODE) );
}


/*************************************************************************
*                                                                        *
*      SET THE ATTRIBUTES FOR THE RENDER MATERIAL                        *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Set the cull mode for the material.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetCullMode( int nPass,CULLMODE mode )
{
    m_FixedPLPasses[nPass].RenderStates.CullMode = mode;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the polygon fill mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      POLYGONMODE:    New polygon fill mode to set.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetPolygonMode( int nPass,POLYGONMODE mode )
{
    m_FixedPLPasses[nPass].RenderStates.PolygonMode = mode;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the shade mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      SHADEMODE:      New shade mode to set.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetShadeMode( int nPass,SHADEMODE mode )
{
    m_FixedPLPasses[nPass].RenderStates.ShadeMode = mode;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the depth buffer mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      DEPTHMODE:      New depth buffer mode to set.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetDepthMode( int nPass,DEPTHMODE mode )
{
    m_FixedPLPasses[nPass].RenderStates.DepthMode = mode;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the depth buffer compare func and ref value.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC:     New compare function to set.
//      float:          New depth bias to set.
//      float:          New depth slope scale value to set.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetDepthFunc( int nPass,FLYCMPFUNC func,float fBias,float fSlopeScale )
{
    m_FixedPLPasses[nPass].RenderStates.DepthFunc = func;
    m_FixedPLPasses[nPass].RenderStates.fSlopeScale = fSlopeScale;
    m_FixedPLPasses[nPass].RenderStates.fDepthBias = fBias;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the stencil mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Which pass to set the stencil mode.
//      bool:           Open the front stencil buffer.
//      bool:           Open the back stencil buffer.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetStencilMode( int nPass,bool bFront,bool bBack )
{
    m_FixedPLPasses[nPass].RenderStates.bFrontStencilEnable = bFront;
    m_FixedPLPasses[nPass].RenderStates.bBackStencilEnable = bBack;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the stencil compare function for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Which pass to set.
//      FLYCMPFUNC:     Compare function of the front buffer.
//      FLYCMPFUNC:     Compare function of the back buffer.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetStencilFunc( int nPass,FLYCMPFUNC funcF,FLYCMPFUNC funcB )
{
    m_FixedPLPasses[nPass].RenderStates.StencilFunc = funcF;
    m_FixedPLPasses[nPass].RenderStates.BackStencilFunc = funcB;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the stencil reference value for the material.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetStencilRef( int nPass,int nStencilRef )
{
    m_FixedPLPasses[nPass].RenderStates.nStencilRef = nStencilRef;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the stencil mask value for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Which pass to set.
//      DWORD:      Stencil mask value.
//      DWORD:      Stencil write mask value.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetStencilMask( int nPass,DWORD dwMask,DWORD dwWriteMask )
{
    m_FixedPLPasses[nPass].RenderStates.dwStencilMask = dwMask;
    m_FixedPLPasses[nPass].RenderStates.dwStencilWriteMask = dwWriteMask;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the stencil operation for the material. (Front side)
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Which pass to set.
//      STENCILOPERATION:   Operation when stencil test failed.
//      STENCILOPERATION:   Operation when test passed and zTest failed.
//      STENCILOPERATION:   Operation when test and zTest passed.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetStencilOperationF( int nPass,STENCILOPERATION failOp,STENCILOPERATION zFailOp,
                                              STENCILOPERATION passOp )
{
    m_FixedPLPasses[nPass].RenderStates.StencilFail = failOp;
    m_FixedPLPasses[nPass].RenderStates.StencilDepthFail = zFailOp;
    m_FixedPLPasses[nPass].RenderStates.StencilPass = passOp;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the stencil operation for the material. (Back side)
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Which pass to set.
//      STENCILOPERATION:   Operation when stencil test failed.
//      STENCILOPERATION:   Operation when test passed and zTest failed.
//      STENCILOPERATION:   Operation when test and zTest passed.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetStencilOperationB( int nPass,STENCILOPERATION failOp,STENCILOPERATION zFailOp,
                                              STENCILOPERATION passOp )
{
    m_FixedPLPasses[nPass].RenderStates.BackStencilFail = failOp;
    m_FixedPLPasses[nPass].RenderStates.BackStencilDepthFail = zFailOp;
    m_FixedPLPasses[nPass].RenderStates.BackStencilPass = passOp;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the alpha blend mode and alpha test mode.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:           Open the alpha blend mode ?
//      bool:           Open the alpha test mode ?
//------------------------------------------------------------------------
void FlyRenderMaterial::SetSceneAlphaMode( int nPass,bool bAlphaBlend,bool bAlphaTest )
{
    m_FixedPLPasses[nPass].RenderStates.bAlphaBlend = bAlphaBlend;
    m_FixedPLPasses[nPass].RenderStates.bAlphaTest = bAlphaTest;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the alpha test func and compare value.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC:     New compare func for the alpha test.
//      DWORD:          New compare value for the alpha test.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetAlphaTestFunc( int nPass,FLYCMPFUNC func,DWORD dwRef )
{
    m_FixedPLPasses[nPass].RenderStates.AlphaTestFunc = func;
    m_FixedPLPasses[nPass].RenderStates.dwAlphaTestRef = dwRef;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the alpha blend mode, source factor and dest factor.
// ----------------------------------------------------------------------
// Param -> IN:
//      SCENEBLENDMODE: New source factor for alpha blend.
//      SCENEBLENDMODE: New destination factor for alpha blend.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetAlphaBlendMode( int nPass,SCENEBLENDMODE src,SCENEBLENDMODE dest )
{
    if( src == SBM_ONE && dest == SBM_ZERO )
        m_FixedPLPasses[nPass].RenderStates.bAlphaBlend = false;

    m_FixedPLPasses[nPass].RenderStates.SourceBlend = src;
    m_FixedPLPasses[nPass].RenderStates.DestBlend = dest;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the standard material for the render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FLYMATERIAL*: Const reference for the material.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetStdMaterial( int nPass,const FLYMATERIAL* pMaterial )
{
    memcpy( &m_FixedPLPasses[nPass].RenderStates.StdMaterial,pMaterial,sizeof(FLYMATERIAL) );
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the lighting mode for the render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Open the light effect ?
//------------------------------------------------------------------------
void FlyRenderMaterial::SetLightingMode( int nPass,bool bLighting )
{
    m_FixedPLPasses[nPass].RenderStates.bLighting = bLighting;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Open or close the specular light.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetSpecularMode( int nPass,bool bEnable )
{
    m_FixedPLPasses[nPass].RenderStates.bSpecularLight = bEnable;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the texture for the render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            The stage of the texture.
//      FlyTexture*:    Pointer to the texture.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetTexture( int nPass,int nStage,FlyTexture* pTexture )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    m_FixedPLPasses[nPass].Samplers[nStage].pTexture = pTexture;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the texture coord index for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        The stage of the coord index.
//      int:        The index of the coord.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetTextureCoordIndex( int nPass,int nStage,int nCoordIndex )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    m_FixedPLPasses[nPass].RenderStates.CoordSetIndex[nStage] = nCoordIndex;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the texture addressing mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            The stage of the addressing mode.
//      TEXTUREADDRESS: The texture U address mode.
//      TEXTUREADDRESS: The texture V address mode.
//      TEXTUREADDRESS: The texture W address mode.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetTextureAddressingMode( int nPass,int nStage,TEXTUREADDRESS addrU,TEXTUREADDRESS addrV,
                                                  TEXTUREADDRESS addrW )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    m_FixedPLPasses[nPass].Samplers[nStage].AddressU = addrU;
    m_FixedPLPasses[nPass].Samplers[nStage].AddressV = addrV;
    m_FixedPLPasses[nPass].Samplers[nStage].AddressW = addrW;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the texture filter mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            The stage of the filter mode.
//      FILTEROPTION:   The texture mag filter option.
//      FILTEROPTION:   The texture min filter option.
//      FILTEROPTION:   The texture mip filter option.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetTextureFilterMode( int nPass,int nStage,FILTEROPTION magFO,FILTEROPTION minFO,
                                              FILTEROPTION mipFO )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    m_FixedPLPasses[nPass].Samplers[nStage].MagFilter = magFO;
    m_FixedPLPasses[nPass].Samplers[nStage].MinFilter = minFO;
    m_FixedPLPasses[nPass].Samplers[nStage].MipFilter = mipFO;
    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Set the texture blend mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                The stage of the blend mode.
//      TEXTUREBLENDMODE:   The blend mode, color or alpha.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetTextureBlendMode( int nPass,int nStage,const TEXTUREBLENDMODE& blend )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    if( blend.Type == BT_COLOR )
        memcpy( &m_FixedPLPasses[nPass].RenderStates.ColorBlend[nStage],&blend,sizeof(TEXTUREBLENDMODE) );
    else
        memcpy( &m_FixedPLPasses[nPass].RenderStates.AlphaBlend[nStage],&blend,sizeof(TEXTUREBLENDMODE) );

    m_FixedPLPasses[nPass].bModified = true;
}


//------------------------------------------------------------------------
// Get the name of the render material.
//------------------------------------------------------------------------
const char* FlyRenderMaterial::GetName(void) const
{
    return m_sName.c_str();
}


//------------------------------------------------------------------------
// Set the name for the render material.
//------------------------------------------------------------------------
void FlyRenderMaterial::SetName( const char* cName )
{
    m_sName = cName;
}

#endif