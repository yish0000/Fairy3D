/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTechnique.cpp
 *  Desc:   本文件用于实现材质系统中的技术类。
 *  Author: Yish
 *  Date:   2010/11/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DTechnique.h"


#if 0
#include "FlyRenderEffect.h"
#include "FlyTechnique.h"

#include "FlyKernel.h"


/** 构造函数
@Param 指向父渲染效果的指针
*/
FlyRenderTechnique::FlyRenderTechnique( FlyRenderEffect* pParent )
    : m_pParent(pParent)
{
}

/** 析构函数
*/
FlyRenderTechnique::~FlyRenderTechnique(void)
{
    AnnotationList::iterator i1;
    for( i1=m_Annotations.begin();i1!=m_Annotations.end();i1++ )
        SAFE_DELETE( *i1 );

    PassList::iterator i2;
    for( i2=m_Passes.begin();i2!=m_Passes.end();i2++ )
        SAFE_DELETE( *i2 );
}

/** 添加一个注解
@Param 注解的名字
*/
FlyRenderAnnotation* FlyRenderTechnique::AddAnnotation( const FString& name )
{
    // 检查该注解是否存在
    AnnotationList::iterator itr;
    for( itr=m_Annotations.begin();itr!=m_Annotations.end();itr++ )
    {
        if( itr->GetName() == name )
            return *itr;
    }

    // 添加一个新的注解
    FlyRenderAnnotation* pAnno = new FlyRenderAnnotation();
    pAnno->SetName( name );
    m_Annotations.push_back( pAnno );
    return m_Annotations[m_Annotations.size()-1];
}


/** 为该技术添加一个新的渲染批次
@Param 新渲染批次的名字
*/
FlyRenderPass* FlyRenderTechnique::AddPass( const FString& name )
{
    // 检查该渲染批次是否存在
    PassList::iterator itr;
    for( itr=m_Passes.begin();itr!=m_Passes.end();itr++ )
    {
        if( itr->GetName() == name )
            return *itr;
    }

    // 添加一个新的渲染批次
    FlyRenderPass* pPass = new FlyRenderPass( this );
    pPass->SetName( name );
    m_Passes.push_back( pPass );
    return m_Passes[m_Passes.size()-1];
}


/** 将制定的渲染批次应用到设备
@Param 渲染批次的编号
*/
HRESULT FlyRenderTechnique::ApplyPass( UINT nPass )
{
    return nPass < m_Passes.size() ? m_Passes[nPass]->Apply() : FLY_OK;
}


/** 获取指定注解的指针
@Param 注解的编号
*/
FlyRenderAnnotation* FlyRenderTechnique::GetAnnotationByIndex( size_t nIndex )
{
    return nIndex < m_Annotations.size() ? m_Annotations[nIndex] : NULL;
}


/** 通过名字获取注解的指针
@Param 注解的名字
*/
FlyRenderAnnotation* FlyRenderTechnique::GetAnnotationByName( const FString& name )
{
    AnnotationList::iterator itr;
    for( itr=m_Annotations.begin();itr!=m_Annotations.end();itr++ )
    {
        if( itr->GetName() == name )
            return *itr;
    }

    return NULL;
}


/** 由指定批次的编号获取其指针
@Param 渲染批次的编号
*/
FlyRenderPass* FlyRenderTechnique::GetPassByIndex( size_t nIndex )
{
    return nIndex < m_Passes.size() ? m_Passes[nIndex] : NULL;
}


/** 通过指定批次的名字获取其指针
@Param 渲染批次的名字
*/
FlyRenderPass* FlyRenderTechnique::GetPassByName( const FString& name )
{
    PassList::iterator itr;
    for( itr=m_Passes.begin();itr!=m_Passes.end();itr++ )
    {
        if( itr->GetName() == name )
            return *itr;
    }

    return NULL;
}


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYRENDERPASS                               *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor and destructor of the class.
//------------------------------------------------------------------------
FlyRenderPass::FlyRenderPass( FlyRenderTechnique* pParent )
{
    m_pParent = pParent;
    m_pShaderObj = NULL;

    m_RenderStates.InitRenderStates();
}

FlyRenderPass::~FlyRenderPass(void)
{
    for( size_t i=0;i<m_Annotations.size();i++ )
        SAFE_DELETE( m_Annotations[i] );

    SAFE_DELETE( m_pShaderObj );
}


//------------------------------------------------------------------------
// Add the annotation for the render pass.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the annotation.
//------------------------------------------------------------------------
FlyRenderAnnotation* FlyRenderPass::AddAnnotation( const char* cName )
{
    // Check whether the annotation existed ?
    for( size_t i=0;i<m_Annotations.size();i++ )
    {
        if( strcmp(m_Annotations[i]->GetName(),cName) == 0 )
            return m_Annotations[i];
    }

    FlyRenderAnnotation* pAnno = new FlyRenderAnnotation();
    pAnno->SetName( cName );
    m_Annotations.push_back( pAnno );
    return m_Annotations[m_Annotations.size()-1];
}


//------------------------------------------------------------------------
// Create the shader object for the render pass.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Text of the shader code.
//      std::string&:       To store the HLSL compile error.
//------------------------------------------------------------------------
HRESULT FlyRenderPass::CreateShader( const char* cShaderText,std::string& errInfo )
{
    HRESULT hr;

    // Delete the old datas.
    SAFE_DELETE( m_pShaderObj );

    // Get the resource Manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Create the new shader object.
    m_pShaderObj = pResMgr->MakeShaderObject();
    if( !m_pShaderObj ) return FLY_OUTOFMEMORY;

    size_t nLen = m_pParent->GetRenderEffect()->GetShaderLength();
    const char* shader_text = m_pParent->GetRenderEffect()->GetShaderText();

    // Create the vertex shader object.
    if( m_ShaderDesc[0].sFuncName != "" )
    {
        hr = m_pShaderObj->CreateShader( m_pParent->GetRenderEffect(),SHADER_VERTEX,&m_ShaderDesc[0],
                                         shader_text,nLen,errInfo );
        if( FAILED(hr) ) return FLY_COMPILEHLSL;
    }

    // Create the pixel shader object.
    if( m_ShaderDesc[1].sFuncName != "" )
    {
        hr = m_pShaderObj->CreateShader( m_pParent->GetRenderEffect(),SHADER_PIXEL,&m_ShaderDesc[1],
                                         shader_text,nLen,errInfo );
        if( FAILED(hr) ) return FLY_COMPILEHLSL;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Apply the render states for the render pass.
//------------------------------------------------------------------------
HRESULT FlyRenderPass::Apply(void)
{
    // 获取图形设备指针
    LPFLYRENDERDEVICE pDevice = FlyKernel::Instance().GetRenderDevice();

    if( pDevice->GetActivePass() != this )
    {
        // 1. Handle the rasterizer state.
        pDevice->SetCullMode( m_RenderStates.CullMode );
        pDevice->SetPolygonMode( m_RenderStates.PolygonMode );
        pDevice->SetShadeMode( m_RenderStates.ShadeMode );

        // 2. Handle the depth buffer states.
        pDevice->SetDepthBufferMode( m_RenderStates.DepthMode );
        pDevice->SetDepthFunc( m_RenderStates.DepthFunc,m_RenderStates.fDepthBias,m_RenderStates.fSlopeScale );

        // 3. Handle the stencil buffer states.
        pDevice->SetStencilMode( m_RenderStates.bFrontStencilEnable,m_RenderStates.bBackStencilEnable );
        pDevice->SetStencilFunc( m_RenderStates.StencilFunc,m_RenderStates.BackStencilFunc );
        pDevice->SetStencilRef( m_RenderStates.nStencilRef );
        pDevice->SetStencilMask( m_RenderStates.dwStencilMask,m_RenderStates.dwStencilWriteMask );
        pDevice->SetStencilOperationF( m_RenderStates.StencilFail,m_RenderStates.StencilDepthFail,m_RenderStates.StencilPass );
        pDevice->SetStencilOperationB( m_RenderStates.BackStencilFail,m_RenderStates.BackStencilDepthFail,m_RenderStates.BackStencilPass );

        // 4. Handle the blend states.
        pDevice->SetSceneAlphaMode( m_RenderStates.bAlphaBlend,m_RenderStates.bAlphaTest );
        pDevice->SetAlphaBlendMode( m_RenderStates.SourceBlend,m_RenderStates.DestBlend );
        pDevice->SetAlphaTestFunc( m_RenderStates.AlphaTestFunc,m_RenderStates.dwAlphaTestRef );

        // 5. Handle the light stuff states.
        pDevice->SetLightingMode( m_RenderStates.bLighting );
        pDevice->SetSpecularMode( m_RenderStates.bSpecularLight );
        pDevice->SetStdMaterial( &m_RenderStates.StdMaterial );

        // 6. Handle the texture states.
        for( int i=0;i<MAX_TEXTURES;i++ )
        {
            pDevice->SetTextureBlendingMode( i,m_RenderStates.ColorBlend[i] );
            pDevice->SetTextureBlendingMode( i,m_RenderStates.AlphaBlend[i] );
            pDevice->SetTextureCoordIndex( i,m_RenderStates.CoordSetIndex[i] );
        }
    }

    // 5. Handle the shader objects.
    m_pShaderObj->Apply();

    pDevice->SetActivePass( this );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the name of the render pass.
//------------------------------------------------------------------------
const char* FlyRenderPass::GetName(void) const
{
    return m_sName.c_str();
}

//------------------------------------------------------------------------
// Set the name for the render pass.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the render pass.
//------------------------------------------------------------------------
void FlyRenderPass::SetName( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Get the number of the annotations.
//------------------------------------------------------------------------
size_t FlyRenderPass::GetNumAnnotations(void) const
{
    return m_Annotations.size();
}

//------------------------------------------------------------------------
// Get the annotation pointer by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the annotation.
//------------------------------------------------------------------------
FlyRenderAnnotation* FlyRenderPass::GetAnnotationByIndex( size_t nIndex )
{
    if( nIndex >= m_Annotations.size() )
        return NULL;

    return m_Annotations[nIndex];
}

//------------------------------------------------------------------------
// Get the annotation pointer by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the annotation.
//------------------------------------------------------------------------
FlyRenderAnnotation* FlyRenderPass::GetAnnotationByName( const char* cName )
{
    for( size_t i=0;i<m_Annotations.size();i++ )
    {
        if( strcmp(m_Annotations[i]->GetName(),cName) == 0 )
            return m_Annotations[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the shader object pointer.
//------------------------------------------------------------------------
const FlyShaderObject* FlyRenderPass::GetShaderObject(void) const
{
    return m_pShaderObj;
}

//------------------------------------------------------------------------
// Which technique own the pass.
//------------------------------------------------------------------------
FlyRenderTechnique* FlyRenderPass::GetTechnique(void)
{
    return m_pParent;
}


//------------------------------------------------------------------------
// Set the vertex shader description for the pass.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Function name of the vertex shader.
//      const char*:    Profile string of the vertex shader.
//------------------------------------------------------------------------
void FlyRenderPass::SetVShaderDesc( const char* cFuncName,const char* cProfile )
{
    m_ShaderDesc[SHADER_VERTEX].sFuncName = cFuncName;
    m_ShaderDesc[SHADER_VERTEX].sProfile = cProfile;
}


//------------------------------------------------------------------------
// Set the pixel shader description for the pass.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Function name of the pixel shader.
//      const char*:    Profile string of the pixel shader.
//------------------------------------------------------------------------
void FlyRenderPass::SetPShaderDesc( const char* cFuncName,const char* cProfile )
{
    m_ShaderDesc[SHADER_PIXEL].sFuncName = cFuncName;
    m_ShaderDesc[SHADER_PIXEL].sProfile = cProfile;
}


/*************************************************************************
*                                                                        *
*     FUNCTIONS TO GET THE ATTRIBUTES FOR THE RENDER MATERIAL            *
*                                                                        *
*************************************************************************/

//------------------------------------------------------------------------
CULLMODE FlyRenderPass::GetCullMode(void) const
{
    return m_RenderStates.CullMode;
}

//------------------------------------------------------------------------
POLYGONMODE FlyRenderPass::GetPolygonMode(void) const
{
    return m_RenderStates.PolygonMode;
}

//------------------------------------------------------------------------
SHADEMODE FlyRenderPass::GetShadeMode(void) const
{
    return m_RenderStates.ShadeMode;
}

//------------------------------------------------------------------------
DEPTHMODE FlyRenderPass::GetDepthMode(void) const
{
    return m_RenderStates.DepthMode;
}

//------------------------------------------------------------------------
void FlyRenderPass::GetDepthFunc( FLYCMPFUNC* pCmpFunc,float* pBias,float* pSlopeScale ) const
{
    *pCmpFunc = m_RenderStates.DepthFunc;
    *pBias = m_RenderStates.fDepthBias;
    *pSlopeScale = m_RenderStates.fSlopeScale;
}

//------------------------------------------------------------------------
bool FlyRenderPass::IsStencilEnable(void) const
{
    return m_RenderStates.bFrontStencilEnable;
}

//------------------------------------------------------------------------
bool FlyRenderPass::IsCCWStencilEnable(void) const
{
    return m_RenderStates.bBackStencilEnable;
}

//------------------------------------------------------------------------
void FlyRenderPass::GetStencilFunc( FLYCMPFUNC* pFuncF,FLYCMPFUNC* pFuncB ) const
{
    *pFuncF = m_RenderStates.StencilFunc;
    *pFuncB = m_RenderStates.BackStencilFunc;
}

//------------------------------------------------------------------------
int FlyRenderPass::GetStencilRef(void) const
{
    return m_RenderStates.nStencilRef;
}

//------------------------------------------------------------------------
void FlyRenderPass::GetStencilMask( DWORD* pMask,DWORD* pWriteMask ) const
{
    *pMask = m_RenderStates.dwStencilMask;
    *pWriteMask = m_RenderStates.dwStencilWriteMask;
}

//------------------------------------------------------------------------
void FlyRenderPass::GetStencilOperationF( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,
                                          STENCILOPERATION* pPass ) const
{
    *pFail = m_RenderStates.StencilFail;
    *pZFail = m_RenderStates.StencilDepthFail;
    *pPass = m_RenderStates.StencilPass;
}

//------------------------------------------------------------------------
void FlyRenderPass::GetStencilOperationB( STENCILOPERATION* pFail,STENCILOPERATION* pZFail,
                                          STENCILOPERATION* pPass ) const
{
    *pFail = m_RenderStates.BackStencilFail;
    *pZFail = m_RenderStates.BackStencilDepthFail;
    *pPass = m_RenderStates.BackStencilPass;
}

//------------------------------------------------------------------------
bool FlyRenderPass::IsAlphaBlendOpen(void) const
{
    return m_RenderStates.bAlphaBlend;
}

//------------------------------------------------------------------------
bool FlyRenderPass::IsAlphaTestOpen(void) const
{
    return m_RenderStates.bAlphaTest;
}

//------------------------------------------------------------------------
void FlyRenderPass::GetAlphaTestFunc( FLYCMPFUNC* pCmpFunc,DWORD* pCmpValue ) const
{
    *pCmpFunc = m_RenderStates.AlphaTestFunc;
    *pCmpValue = m_RenderStates.dwAlphaTestRef;
}

//------------------------------------------------------------------------
void FlyRenderPass::GetAlphaBlendMode( SCENEBLENDMODE* pSrc,SCENEBLENDMODE* pDest ) const
{
    *pSrc = m_RenderStates.SourceBlend;
    *pDest = m_RenderStates.DestBlend;
}

//------------------------------------------------------------------------
void FlyRenderPass::GetStdMaterial( FLYMATERIAL* pMaterial ) const
{
    memcpy( pMaterial,&m_RenderStates.StdMaterial,sizeof(FLYMATERIAL) );
}

//------------------------------------------------------------------------
bool FlyRenderPass::IsLighting(void) const
{
    return m_RenderStates.bLighting;
}

//------------------------------------------------------------------------
bool FlyRenderPass::IsSpecularEnabled(void) const
{
    return m_RenderStates.bSpecularLight;
}

//------------------------------------------------------------------------
int FlyRenderPass::GetTextureCoordIndex( int nStage ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return -1;

    return m_RenderStates.CoordSetIndex[nStage];
}

//------------------------------------------------------------------------
void FlyRenderPass::GetTextureBlendMode( int nStage,BLENDTYPE Type,TEXTUREBLENDMODE* pBlendMode ) const
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    if( Type == BT_COLOR )
        memcpy( pBlendMode,&m_RenderStates.ColorBlend,sizeof(TEXTUREBLENDMODE) );
    else
        memcpy( pBlendMode,&m_RenderStates.AlphaBlend,sizeof(TEXTUREBLENDMODE) );
}


/*************************************************************************
*                                                                        *
*      SET THE ATTRIBUTES FOR THE RENDER MATERIAL                        *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Set the cull mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      CULLMODE:       New cull mode to set.
//------------------------------------------------------------------------
void FlyRenderPass::SetCullMode( CULLMODE mode )
{
    m_RenderStates.CullMode = mode;
}


//------------------------------------------------------------------------
// Set the polygon fill mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      POLYGONMODE:    New polygon fill mode to set.
//------------------------------------------------------------------------
void FlyRenderPass::SetPolygonMode( POLYGONMODE mode )
{
    m_RenderStates.PolygonMode = mode;
}


//------------------------------------------------------------------------
// Set the shade mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      SHADEMODE:      New shade mode to set.
//------------------------------------------------------------------------
void FlyRenderPass::SetShadeMode( SHADEMODE mode )
{
    m_RenderStates.ShadeMode = mode;
}


//------------------------------------------------------------------------
// Set the depth buffer mode for the material.
// ----------------------------------------------------------------------
// Param -> IN:
//      DEPTHMODE:      New depth buffer mode to set.
//------------------------------------------------------------------------
void FlyRenderPass::SetDepthMode( DEPTHMODE mode )
{
    m_RenderStates.DepthMode = mode;
}


//------------------------------------------------------------------------
// Set the depth buffer compare func and ref value.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC:     New compare function to set.
//      float:          New depth bias to set.
//      float:          New depth slope scale value to set.
//------------------------------------------------------------------------
void FlyRenderPass::SetDepthFunc( FLYCMPFUNC func,float fBias,float fSlopeScale )
{
    m_RenderStates.DepthFunc = func;
    m_RenderStates.fSlopeScale = fSlopeScale;
    m_RenderStates.fDepthBias = fBias;
}


//------------------------------------------------------------------------
// Set the stencil buffer mode for the device.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Open the front stencil ?
//      bool:       Open the back stencil ?
//------------------------------------------------------------------------
void FlyRenderPass::SetStencilMode( bool bFront,bool bBack )
{
    m_RenderStates.bFrontStencilEnable = bFront;
    m_RenderStates.bBackStencilEnable = bBack;
}


//------------------------------------------------------------------------
// Set the stencil func for the device.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC:     Compare function of the front side.
//      FLYCMPFUNC:     Compare function of the back side.
//------------------------------------------------------------------------
void FlyRenderPass::SetStencilFunc( FLYCMPFUNC frontFunc,FLYCMPFUNC backFunc )
{
    m_RenderStates.StencilFunc = frontFunc;
    m_RenderStates.BackStencilFunc = backFunc;
}


//------------------------------------------------------------------------
// Set the stencil operation for the device. (Front side)
// ----------------------------------------------------------------------
// Param -> IN:
//      STENCILOPERATION:   Operation when test failed.
//      STENCILOPERATION:   Operation when test passed and z-Test failed.
//      STENCILOPERATION:   Operation when both test and z-Test passed.
//------------------------------------------------------------------------
void FlyRenderPass::SetStencilOperationF( STENCILOPERATION failOp,STENCILOPERATION zFailOp,
                                          STENCILOPERATION passOp )
{
    m_RenderStates.StencilFail = failOp;
    m_RenderStates.StencilDepthFail = zFailOp;
    m_RenderStates.StencilPass = passOp;
}


//------------------------------------------------------------------------
// Set the stencil operation for the device. (Back side)
// ----------------------------------------------------------------------
// Param -> IN:
//      STENCILOPERATION:   Operation when test failed.
//      STENCILOPERATION:   Operation when test passed and z-Test failed.
//      STENCILOPERATION:   Operation when both test and z-Test passed.
//------------------------------------------------------------------------
void FlyRenderPass::SetStencilOperationB( STENCILOPERATION failOp,STENCILOPERATION zFailOp,
                                          STENCILOPERATION passOp )
{
    m_RenderStates.BackStencilFail = failOp;
    m_RenderStates.BackStencilDepthFail = zFailOp;
    m_RenderStates.BackStencilPass = passOp;
}


//------------------------------------------------------------------------
// Set the stencil reference value.
//------------------------------------------------------------------------
void FlyRenderPass::SetStencilRef( int nStencilRef )
{
    m_RenderStates.nStencilRef = nStencilRef;
}


//------------------------------------------------------------------------
// Set the stencil mask value for device.
// ----------------------------------------------------------------------
// Param -> IN:
//      DWORD:      Stencil mask value (Front side).
//      DWORD:      Stencil mask value (Back side).
//------------------------------------------------------------------------
void FlyRenderPass::SetStencilMask( DWORD dwMask,DWORD dwWriteMask )
{
    m_RenderStates.dwStencilMask = dwMask;
    m_RenderStates.dwStencilWriteMask = dwWriteMask;
}


//------------------------------------------------------------------------
// Set the alpha blend mode and alpha test mode.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:           Open the alpha blend mode ?
//      bool:           Open the alpha test mode ?
//------------------------------------------------------------------------
void FlyRenderPass::SetSceneAlphaMode( bool bAlphaBlend,bool bAlphaTest )
{
    m_RenderStates.bAlphaBlend = bAlphaBlend;
    m_RenderStates.bAlphaTest = bAlphaTest;
}


//------------------------------------------------------------------------
// Set the alpha test func and compare value.
// ----------------------------------------------------------------------
// Param -> IN:
//      FLYCMPFUNC:     New compare func for the alpha test.
//      DWORD:          New compare value for the alpha test.
//------------------------------------------------------------------------
void FlyRenderPass::SetAlphaTestFunc( FLYCMPFUNC func,DWORD dwRef )
{
    m_RenderStates.AlphaTestFunc = func;
    m_RenderStates.dwAlphaTestRef = dwRef;
}


//------------------------------------------------------------------------
// Set the alpha blend mode, source factor and dest factor.
// ----------------------------------------------------------------------
// Param -> IN:
//      SCENEBLENDMODE: New source factor for alpha blend.
//      SCENEBLENDMODE: New destination factor for alpha blend.
//------------------------------------------------------------------------
void FlyRenderPass::SetAlphaBlendMode( SCENEBLENDMODE src,SCENEBLENDMODE dest )
{
    if( src == SBM_ONE && dest == SBM_ZERO )
        m_RenderStates.bAlphaBlend = false;

    m_RenderStates.SourceBlend = src;
    m_RenderStates.DestBlend = dest;
}


//------------------------------------------------------------------------
// Set the standard material for the render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FLYMATERIAL*: Const reference for the material.
//------------------------------------------------------------------------
void FlyRenderPass::SetStdMaterial( const FLYMATERIAL* pMaterial )
{
    memcpy( &m_RenderStates.StdMaterial,pMaterial,sizeof(FLYMATERIAL) );
}


//------------------------------------------------------------------------
// Set the lighting mode for the render pass.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Open the light effect ?
//------------------------------------------------------------------------
void FlyRenderPass::SetLightingMode( bool bLighting )
{
    m_RenderStates.bLighting = bLighting;
}


//------------------------------------------------------------------------
// Enable the specular light.
//------------------------------------------------------------------------
void FlyRenderPass::SetSpecularMode( bool bSpecular )
{
    m_RenderStates.bSpecularLight = true;
}


//------------------------------------------------------------------------
// Set the texture coordset index.
//------------------------------------------------------------------------
void FlyRenderPass::SetTextureCoordIndex( int nStage,int nCoordIndex )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    m_RenderStates.CoordSetIndex[nStage] = nCoordIndex;
}


//------------------------------------------------------------------------
// Set the texture blending mode. (Color and alpha)
//------------------------------------------------------------------------
void FlyRenderPass::SetTextureBlendMode( int nStage,const TEXTUREBLENDMODE& blend )
{
    if( nStage < 0 || nStage >= MAX_TEXTURES )
        return;

    if( blend.Type == BT_COLOR )
        memcpy( &m_RenderStates.ColorBlend[nStage],&blend,sizeof(TEXTUREBLENDMODE) );
    else
        memcpy( &m_RenderStates.AlphaBlend[nStage],&blend,sizeof(TEXTUREBLENDMODE) );
}
#endif