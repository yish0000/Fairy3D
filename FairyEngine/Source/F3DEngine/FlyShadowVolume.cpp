//------------------------------------------------------------------------
// Name: FlyShadowVolume.cpp
// Desc: This file is to implement the class FlyShadowVolume.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyShadowVolume.h"

#include "FlyKernel.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyShadowVolume::FlyShadowVolume(void)
{
    m_nNumVerts = 0;
    m_pVerts = NULL;
    m_nNumIndis = 0;
    m_pIndis = NULL;
    m_nStrideSize = 0;

    m_pEdges = NULL;
    m_nNumEdges = 0;
    m_pVolume = NULL;
    m_nNumVolVerts = 0;
    m_nNumTriangles = 0;

    memset( &m_vLight,0,sizeof(FlyVector) );
    m_pRObject = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyShadowVolume::~FlyShadowVolume(void)
{
    SAFE_DELETE_ARRAY( m_pVerts );
    SAFE_DELETE_ARRAY( m_pIndis );

    SAFE_DELETE_ARRAY( m_pEdges );
    SAFE_DELETE_ARRAY( m_pVolume );

    SAFE_DELETE( m_pRObject );
}


//------------------------------------------------------------------------
// A helper function.
//------------------------------------------------------------------------
FlyShadowVolume::_Vertex FlyShadowVolume::Extrude( FlyShadowVolume::_Vertex* pVert,
                                                   const FlyVector& vc )
{
    _Vertex vert;
    vert.x = pVert->x - vc.x;
    vert.y = pVert->y - vc.y;
    vert.z = pVert->z - vc.z;
    return vert;
}


//------------------------------------------------------------------------
// Add indices of an edge to the given list of edges.
//------------------------------------------------------------------------
void FlyShadowVolume::AddEdge( WORD v0,WORD v1 )
{
    // Remove interior edges (which appear in the list twice)
    for( UINT i=0;i<m_nNumEdges;i++ )
    {
        if( (m_pEdges[2*i+0]==v0 && m_pEdges[2*i+1]==v1) ||
            (m_pEdges[2*i+0]==v1 && m_pEdges[2*i+1]==v0) )
        {
            if( m_nNumEdges > 1 )
            {
                m_pEdges[2*i+0] = m_pEdges[2*(m_nNumEdges-1)+0];
                m_pEdges[2*i+1] = m_pEdges[2*(m_nNumEdges-1)+1];
            }

            m_nNumEdges--;
            return;
        }
    }

    m_pEdges[2*m_nNumEdges+0] = v0;
    m_pEdges[2*m_nNumEdges+1] = v1;
    m_nNumEdges++;
}


//------------------------------------------------------------------------
// Build the shadow volume.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:           Count of the vertices.
//      UINT:           Count of the indices.
//      const void*:    Pointer to the vertex data.
//      const WORD*:    Pointer to the index data.
//      int:            Size of the vertex.
//------------------------------------------------------------------------
HRESULT FlyShadowVolume::BuildShadowVolume( UINT nNumVerts,UINT nNumIndis,const void* pVerts,
                                            const WORD* pIndis,int nStrideSize )
{
    m_nNumVerts = nNumVerts;
    m_nStrideSize = nStrideSize;
    m_pVerts = new BYTE[nNumVerts*nStrideSize];
    memcpy( m_pVerts,pVerts,nStrideSize*nNumVerts );
    m_nNumIndis = nNumIndis;
    m_pIndis = new WORD[nNumIndis];
    memcpy( m_pIndis,pIndis,nNumIndis*sizeof(WORD) );

    m_pEdges = new WORD[nNumIndis*2];
    m_pVolume = new _Vertex[nNumIndis*4];
    m_nNumTriangles = m_nNumIndis / 3;
    m_nNumVolVerts = 0;
    m_nNumEdges = 0;

    memset( &m_vLight,0,sizeof(FlyVector) );

    // Build the renderable object.
    m_pRObject = new FlyShadowVolumeRenderable( this );
    m_pRObject->BuildVertexBuffer( nNumIndis*4,sizeof(_Vertex) );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the shadow volume.
//------------------------------------------------------------------------
void FlyShadowVolume::UpdateShadowVolume( const FlyVector& vLight )
{
    if( m_vLight == vLight )
        return;

    m_vLight = vLight;
    m_nNumEdges = 0;
    m_nNumVolVerts = 0;

    for( UINT i=0;i<m_nNumTriangles;i++ )
    {
        FlyVector v0,v1,v2;

        WORD wFace0 = m_pIndis[3*i+0];
        WORD wFace1 = m_pIndis[3*i+1];
        WORD wFace2 = m_pIndis[3*i+2];

        memcpy( &v0,&m_pVerts[wFace0*m_nStrideSize],sizeof(_Vertex) );
        memcpy( &v1,&m_pVerts[wFace1*m_nStrideSize],sizeof(_Vertex) );
        memcpy( &v2,&m_pVerts[wFace2*m_nStrideSize],sizeof(_Vertex) );

        FlyVector vNormal;
        vNormal.Cross( v2-v1,v1-v0 );

        if( (vNormal * vLight) >= 0.0f )
        {
            AddEdge( wFace0,wFace1 );
            AddEdge( wFace1,wFace2 );
            AddEdge( wFace2,wFace0 );
        }
    }

    for( UINT i=0;i<m_nNumEdges;i++ )
    {
        FlyVector v1,v2,v3,v4;

        memcpy( &v1,&m_pVerts[m_pEdges[2*i+0]*m_nStrideSize],sizeof(_Vertex) );
        memcpy( &v2,&m_pVerts[m_pEdges[2*i+1]*m_nStrideSize],sizeof(_Vertex) );
        v3 = v1 - vLight * 10.0f;
        v4 = v2 - vLight * 10.0f;

        memcpy( &m_pVolume[m_nNumVolVerts++],&v1,sizeof(_Vertex) );
        memcpy( &m_pVolume[m_nNumVolVerts++],&v2,sizeof(_Vertex) );
        memcpy( &m_pVolume[m_nNumVolVerts++],&v3,sizeof(_Vertex) );

        memcpy( &m_pVolume[m_nNumVolVerts++],&v2,sizeof(_Vertex) );
        memcpy( &m_pVolume[m_nNumVolVerts++],&v4,sizeof(_Vertex) );
        memcpy( &m_pVolume[m_nNumVolVerts++],&v3,sizeof(_Vertex) );
    }

    // Send the vertex data to renderable object.
    m_pRObject->SendVertexData( (BYTE*)m_pVolume,m_nNumVolVerts );
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS FLYSHADOWVOLUMERENDERABLE


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyShadowVolumeRenderable::FlyShadowVolumeRenderable( FlyShadowVolume* pParent )
{
    m_pParent = pParent;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_pVB = NULL;
    memset( &m_BBox,0,sizeof(FlyAabb) );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    LPFLYRENDERDEVICE pDevice = FlyKernel::Instance().GetRenderDevice();

    // Build the material for the shadowVolume.
    m_pMaterial = pResMgr->GetMaterialByName( "Core/ShadowVolume" );
    if( !m_pMaterial )
    {
        m_pMaterial = pResMgr->CreateMaterial( "Core/ShadowVolume" );

        // Can we use the 2-side stencil buffer ?
        if( pDevice->GetDeviceCaps()->bTwoSideStencil )
        {
            m_pMaterial->SetDepthMode( 0,DEPTH_READONLY );
            m_pMaterial->SetShadeMode( 0,SM_FLAT );
            m_pMaterial->SetStencilMode( 0,true,true );
            m_pMaterial->SetStencilFunc( 0,CMP_ALWAYS,CMP_ALWAYS );
            m_pMaterial->SetStencilOperationF( 0,STOP_KEEP,STOP_KEEP,STOP_INCREMENT );
            m_pMaterial->SetStencilOperationB( 0,STOP_KEEP,STOP_KEEP,STOP_DECREMENT );
            m_pMaterial->SetStencilRef( 0,0x1 );
            m_pMaterial->SetStencilMask( 0,0xffffffff,0xffffffff );

            // Make sure that no pixels get drawn to the frame buffer.
            m_pMaterial->SetSceneAlphaMode( 0,true,false );
            m_pMaterial->SetAlphaBlendMode( 0,SBM_ZERO,SBM_ONE );

            // Draw both sides of shadow volume in stencil/z.
            m_pMaterial->SetCullMode( 0,CULL_NONE );
        }
        else
        {
            // The first pass.
            m_pMaterial->SetDepthMode( 0,DEPTH_READONLY );
            m_pMaterial->SetShadeMode( 0,SM_FLAT );
            m_pMaterial->SetStencilMode( 0,true,false );
            m_pMaterial->SetStencilFunc( 0,CMP_ALWAYS,CMP_ALWAYS );
            m_pMaterial->SetStencilOperationF( 0,STOP_KEEP,STOP_KEEP,STOP_INCREMENT );
            m_pMaterial->SetStencilRef( 0,0x1 );
            m_pMaterial->SetStencilMask( 0,0xffffffff,0xffffffff );

            // Make sure that no pixels get drawn to the frame buffer.
            m_pMaterial->SetSceneAlphaMode( 0,true,false );
            m_pMaterial->SetAlphaBlendMode( 0,SBM_ZERO,SBM_ONE );

            // Cull the back face.
            m_pMaterial->SetCullMode( 0,CULL_CCW );

            // The second pass.
            m_pMaterial->AddNewFixedPass();

            m_pMaterial->SetDepthMode( 1,DEPTH_READONLY );
            m_pMaterial->SetShadeMode( 1,SM_FLAT );
            m_pMaterial->SetStencilMode( 1,true,false );
            m_pMaterial->SetStencilFunc( 1,CMP_ALWAYS,CMP_ALWAYS );
            m_pMaterial->SetStencilOperationF( 1,STOP_KEEP,STOP_KEEP,STOP_DECREMENT );
            m_pMaterial->SetStencilRef( 1,0x1 );
            m_pMaterial->SetStencilMask( 1,0xffffffff,0xffffffff );

            // Make sure that no pixels get drawn to the frame buffer.
            m_pMaterial->SetSceneAlphaMode( 1,true,false );
            m_pMaterial->SetAlphaBlendMode( 1,SBM_ZERO,SBM_ONE );

            // Cull the front face.
            m_pMaterial->SetCullMode( 1,CULL_CW );
        }
    }
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyShadowVolumeRenderable::~FlyShadowVolumeRenderable(void)
{
    SAFE_DELETE( m_pVB );
}


//------------------------------------------------------------------------
// Build the vertex buffer for the shadowVolume.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Count of the vertices.
//      UINT:       Size of each vertex.
//------------------------------------------------------------------------
HRESULT FlyShadowVolumeRenderable::BuildVertexBuffer( UINT nNumVerts,UINT nStrideSize )
{
    SAFE_DELETE( m_pVB );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    m_nNumVerts = nNumVerts;
    m_nNumIndis = 0;
    m_nNumPrims = nNumVerts / 3;

    // Create the buffer.
    m_pVB = pResMgr->MakeVertexBuffer( nNumVerts*nStrideSize,BU_WRITEONLY,MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Send the vertices data to the buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      const BYTE*:    Pointer to the vertex data.
//      UINT:           Size of the vertex buffer.
//------------------------------------------------------------------------
void FlyShadowVolumeRenderable::SendVertexData( const BYTE* pVerts,UINT nNumVerts )
{
    m_nNumVerts = nNumVerts;
    m_nNumPrims = nNumVerts / 3;

    void* ptr = m_pVB->Lock( LOCK_NORMAL );
    memcpy( ptr,pVerts,nNumVerts*12 );
    m_pVB->Unlock();
}


//------------------------------------------------------------------------
// Render the shadow volume to stencil buffer.
//------------------------------------------------------------------------
void FlyShadowVolumeRenderable::Render(void)
{
    // Get the renderer of the engine.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( RT_TRIANGLELIST );
    pRenderer->RenderBuffer( VF_POSITION,m_nNumVerts,0,m_pVB,NULL );
}


//------------------------------------------------------------------------
// Get the count of vertices.
//------------------------------------------------------------------------
UINT FlyShadowVolumeRenderable::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of indices.
//------------------------------------------------------------------------
UINT FlyShadowVolumeRenderable::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of primitives.
//------------------------------------------------------------------------
UINT FlyShadowVolumeRenderable::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the local boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& FlyShadowVolumeRenderable::GetBoundingBox(void) const
{
    return FlyAabb::EMPTY;
}


//------------------------------------------------------------------------
// Get the square distance to the camera.
//------------------------------------------------------------------------
float FlyShadowVolumeRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    return 0.0f;
}


//------------------------------------------------------------------------
// Get the world matrix of the object.
//------------------------------------------------------------------------
void FlyShadowVolumeRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pParent->GetWorldMatrix();
}

//------------------------------------------------------------------------
// Get the count of the world matrix.
//------------------------------------------------------------------------
UINT FlyShadowVolumeRenderable::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT FlyShadowVolumeRenderable::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
//------------------------------------------------------------------------
FlyLightObject* FlyShadowVolumeRenderable::GetLightObject( UINT nIndex )
{
    return NULL;
}

//------------------------------------------------------------------------
// Get the pointer to the vertex buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyShadowVolumeRenderable::GetVertexBuffer(void)
{
    return m_pVB;
}

//------------------------------------------------------------------------
// Get the pointer to the index buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyShadowVolumeRenderable::GetIndexBuffer(void)
{
    return NULL;
}