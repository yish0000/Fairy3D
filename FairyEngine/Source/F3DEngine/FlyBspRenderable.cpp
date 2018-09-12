//------------------------------------------------------------------------
// Name: FlyBspRenderable.cpp
// Desc: This file is to implement the class FlyBspRenderable.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyBspRenderable.h"

#include "FlyKernel.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyBspRenderable::FlyBspRenderable( const char* cMatName )
{
    m_nMaxNumVerts = 0;
    m_nMaxNumIndis = 0;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_pVB = NULL;
    m_pIB = NULL;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    pResMgr->CreateMaterial( cMatName );

    m_BBox.SetEmpty();
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyBspRenderable::~FlyBspRenderable(void)
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );

    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    pResMgr->DeleteMaterial( m_pMaterial );
}


//------------------------------------------------------------------------
// Create the graphic buffer for the object.
//------------------------------------------------------------------------
HRESULT FlyBspRenderable::CreateGraphicBuffer(void)
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );

    // Get the resource manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Create the vertex buffer.
    m_pVB = pResMgr->MakeVertexBuffer( m_nMaxNumVerts*sizeof(VERTEX2T),BU_WRITEONLY,
        MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    // Create the index buffer.
    m_pIB = pResMgr->MakeIndexBuffer( m_nMaxNumIndis*sizeof(WORD),BU_WRITEONLY,
        MM_MANAGED,IF_INDEX16 );
    if( !m_pIB ) return FLY_CREATEBUFFER;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Add a bsp face data to the buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:           Count of the vertices.
//      UINT:           Count of the indices.
//      const void*:    Pointer to the vertex data.
//      const WORD*:    Pointer to the index data.
//      const FlyAabb&: BoundingBox of the face.
//------------------------------------------------------------------------
HRESULT FlyBspRenderable::AddFaceData( UINT nNumVerts,UINT nNumIndis,const void* pVerts,
                                       const WORD* pIndis,const FlyAabb& box )
{
    if( m_nNumVerts + nNumVerts > m_nMaxNumVerts )
        return FLY_BUFFERSIZE;

    // Write the vertex data.
    m_pVB->WriteData( m_nNumVerts*sizeof(VERTEX2T),nNumVerts*sizeof(VERTEX2T),
        pVerts,false );

    // Write the index data.
    WORD* pTemp = (WORD*)m_pIB->Lock( m_nNumIndis*sizeof(WORD),nNumIndis*sizeof(WORD),LOCK_NORMAL );
    for( UINT i=0;i<nNumIndis;i++ )
    {
        *pTemp = pIndis[i] + m_nNumVerts;
        pTemp++;
    }
    m_pIB->Unlock();

    m_nNumVerts += nNumVerts;
    m_nNumIndis += nNumIndis;
    m_nNumPrims += nNumIndis / 3;

    // Merge the new boundingBox.
    m_BBox.Merge( box );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Clear the old geometry data.
//------------------------------------------------------------------------
void FlyBspRenderable::ClearData(void)
{
    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_BBox.SetEmpty();
}


//------------------------------------------------------------------------
// Render the bsp face for the engine.
//------------------------------------------------------------------------
void FlyBspRenderable::Render(void)
{
    // Get the pointer to renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( VF_TWOTEXTURE,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT FlyBspRenderable::GetNumVerts(void) const
{
    return m_nNumVerts;
}


//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT FlyBspRenderable::GetNumIndis(void) const
{
    return m_nNumIndis;
}


//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT FlyBspRenderable::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the distance to the camera.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float FlyBspRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter();

    return (vTemp - vCamPos).GetSqrLength();
}


//------------------------------------------------------------------------
// Get the boundingBox of the renderable.
//------------------------------------------------------------------------
const FlyAabb& FlyBspRenderable::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void FlyBspRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    assert( pMatrices );

    *pMatrices = FlyMatrix::IDENTITY;
}


//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT FlyBspRenderable::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get count of the light object.
//------------------------------------------------------------------------
UINT FlyBspRenderable::GetLightCount(void) const
{
    return 0;
}


//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:    Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* FlyBspRenderable::GetLightObject( UINT nIndex )
{
    return NULL;
}


//------------------------------------------------------------------------
// Get the texture of the renderable.
//------------------------------------------------------------------------
FlyTexture* FlyBspRenderable::GetTexture(void)
{
    return m_pMaterial->GetTexture( 0,0 );
}

//------------------------------------------------------------------------
// Get the texture of the lightmap.
//------------------------------------------------------------------------
FlyTexture* FlyBspRenderable::GetLightmap(void)
{
    return m_pMaterial->GetTexture( 0,1 );
}


//------------------------------------------------------------------------
// Get the max count of vertices.
//------------------------------------------------------------------------
UINT FlyBspRenderable::GetMaxNumVerts(void) const
{
    return m_nMaxNumVerts;
}

//------------------------------------------------------------------------
// Get the max count of indices.
//------------------------------------------------------------------------
UINT FlyBspRenderable::GetMaxNumIndis(void) const
{
    return m_nMaxNumIndis;
}

//------------------------------------------------------------------------
// Set the max count of vertices.
//------------------------------------------------------------------------
void FlyBspRenderable::SetMaxNumVerts( UINT nMaxNumVerts )
{
    m_nMaxNumVerts = nMaxNumVerts;
}

//------------------------------------------------------------------------
// Set the max count of indices.
//------------------------------------------------------------------------
void FlyBspRenderable::SetMaxNumIndis( UINT nMaxNumIndis )
{
    m_nMaxNumIndis = nMaxNumIndis;
}


//------------------------------------------------------------------------
// Set the texture for the renderable.
//------------------------------------------------------------------------
void FlyBspRenderable::SetTexture( FlyTexture* pTexture )
{
    m_pMaterial->SetTexture( 0,0,pTexture );

    if( pTexture && pTexture->HasAlphaChannel() )
    {
        m_pMaterial->SetSceneAlphaMode( 0,true,false );
        m_pMaterial->SetAlphaBlendMode( 0,SBM_SRCALPHA,SBM_INVSRCALPHA );

        m_nPriority = PRIORITY_ALPHA;
    }
}

//------------------------------------------------------------------------
// Set the lightmap for the renderable.
//------------------------------------------------------------------------
void FlyBspRenderable::SetLightmap( FlyTexture* pTexture )
{
    TEXTUREBLENDMODE blend;
    blend.Type = BT_COLOR;
    blend.Arg1 = BSTA_TEXTURE;
    blend.Arg2 = BSTA_CURRENT;
    blend.Operation = TOP_MODULATE;
    blend.fFactor = 0.0f;
    m_pMaterial->SetTextureBlendMode( 0,1,blend );

    m_pMaterial->SetTexture( 0,1,pTexture );
}


//------------------------------------------------------------------------
// Get the pointer to the vertex buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyBspRenderable::GetVertexBuffer(void)
{
    return m_pVB;
}

//------------------------------------------------------------------------
// Get the pointer to the index buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyBspRenderable::GetIndexBuffer(void)
{
    return m_pIB;
}