//------------------------------------------------------------------------
// Name: FlyParticleRenderUnit.cpp
// Desc: This file is to implement the class FlyParticleRenderUnit.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyParticleRenderUnit.h"
#include "FlyParticleController.h"
#include "FlyParticleEntity.h"

#include "FlyKernel.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyParticleRenderUnit::FlyParticleRenderUnit( FlyParticleController* pParent,const char* cMatName )
{
    m_pParent = pParent;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;
    memset( &m_BBox,0,sizeof(FlyAabb) );

    m_pVB = NULL;
    m_pIB = NULL;

    m_nNumParticles = 0;
    m_nMaxParticles = 0;

    // Create the render material.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    m_pMaterial = pResMgr->CreateMaterial( cMatName );
    m_pMaterial->SetDepthMode( 0,DEPTH_READONLY );
    m_pMaterial->SetSceneAlphaMode( 0,true,false );
    m_pMaterial->SetAlphaBlendMode( 0,SBM_ONE,SBM_ONE );

    m_nPriority = PRIORITY_ALPHA;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyParticleRenderUnit::~FlyParticleRenderUnit(void)
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );

    // Delete the render material.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    pResMgr->DeleteMaterial( m_pMaterial );
}


//------------------------------------------------------------------------
// Add the particle geometry data to the buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      VERTEXL*:   Pointer to the vertex data.
//      WORD*:      Pointer to the index data.
//------------------------------------------------------------------------
HRESULT FlyParticleRenderUnit::AddParticleData( VERTEXL* pVert,WORD* pIndis )
{
    if( m_nNumParticles + 4 >= m_nMaxParticles )
        return FLY_BUFFERSIZE;

    // Write the vertex data.
    m_pVB->WriteData( m_nNumParticles*4*sizeof(VERTEXL),4*sizeof(VERTEXL),
        pVert,false );

    for( int i=0;i<6;i++ )
        pIndis[i] += m_nNumVerts;

    // Write the index data.
    m_pIB->WriteData( m_nNumParticles*6*sizeof(WORD),6*sizeof(WORD),
        pIndis,false );

    m_nNumVerts += 4;
    m_nNumIndis += 6;
    m_nNumPrims += 2;
    m_nNumParticles++;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Clear the old particle data.
//------------------------------------------------------------------------
void FlyParticleRenderUnit::ClearParticles(void)
{
    m_nNumParticles = 0;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    // Reset the boundingBox.
    memset( &m_BBox,0,sizeof(FlyAabb) );
}


//------------------------------------------------------------------------
// Get the current number of the active particles.
//------------------------------------------------------------------------
UINT FlyParticleRenderUnit::GetNumParticles(void) const
{
    return m_nNumParticles;
}


//------------------------------------------------------------------------
// Set the new size for the buffer to store particles' data.
//------------------------------------------------------------------------
HRESULT FlyParticleRenderUnit::SetMaxParticles( UINT nMaxParticles )
{
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    if( nMaxParticles != m_nMaxParticles )
    {
        SAFE_DELETE( m_pVB );
        SAFE_DELETE( m_pIB );

        // Create the new vertex buffer.
        m_pVB = pResMgr->MakeVertexBuffer( nMaxParticles*4*sizeof(VERTEXL),
            BU_WRITEONLY,MM_MANAGED );
        if( !m_pVB ) return FLY_CREATEBUFFER;

        m_pIB = pResMgr->MakeIndexBuffer( nMaxParticles*6*sizeof(WORD),BU_WRITEONLY,
            MM_MANAGED,IF_INDEX16 );
        if( !m_pIB ) return FLY_CREATEBUFFER;

        m_nMaxParticles = nMaxParticles;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the texture for the particle render unit.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyTexture*:    Pointer to the particle texture.
//------------------------------------------------------------------------
void FlyParticleRenderUnit::SetTexture( FlyTexture* pTexture )
{
    m_pMaterial->SetTexture( 0,0,pTexture );
}


//------------------------------------------------------------------------
// Set the boundingBox for the render unit.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyAabb&:     BoundingBox of the particle render unit.
//------------------------------------------------------------------------
void FlyParticleRenderUnit::SetBoundingBox( const FlyAabb& box )
{
    m_BBox = box;
}


//------------------------------------------------------------------------
// How many particles can the buffer store ?
//------------------------------------------------------------------------
UINT FlyParticleRenderUnit::GetMaxParticles(void) const
{
    return m_nMaxParticles;
}


//------------------------------------------------------------------------
// Render the graphic buffer. (All particles' geometry data)
//------------------------------------------------------------------------
void FlyParticleRenderUnit::Render(void)
{
    // Get the pointer to renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
}


//------------------------------------------------------------------------
// Get the bounding box of the render unit.
//------------------------------------------------------------------------
const FlyAabb& FlyParticleRenderUnit::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the distance to the camera.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float FlyParticleRenderUnit::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter() * m_pParent->GetParticleEntity()->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}


//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void FlyParticleRenderUnit::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pParent->GetParticleEntity()->GetWorldTransform();
}


//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT FlyParticleRenderUnit::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get the count of the light objects.
//------------------------------------------------------------------------
UINT FlyParticleRenderUnit::GetLightCount(void) const
{
    return m_pParent->GetParticleEntity()->GetNumLights();
}


//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* FlyParticleRenderUnit::GetLightObject( UINT nIndex )
{
    return m_pParent->GetParticleEntity()->GetLightObject( nIndex );
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT FlyParticleRenderUnit::GetNumVerts(void) const
{
    return m_nNumVerts;
}


//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT FlyParticleRenderUnit::GetNumIndis(void) const
{
    return m_nNumIndis;
}


//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT FlyParticleRenderUnit::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the pointer to the vertex buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyParticleRenderUnit::GetVertexBuffer(void)
{
    return m_pVB;
}


//------------------------------------------------------------------------
// Get the pointer to the index buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyParticleRenderUnit::GetIndexBuffer(void)
{
    return m_pIB;
}