//------------------------------------------------------------------------
// Name: FlyRenderableHelper.cpp
// Desc: This file is to implement the helper renderable classes.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyRenderableHelper.h"

#include "FlyKernel.h"


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYBBOXHELPER                               *
*                                                                        *
*************************************************************************/

//------------------------------------------------------------------------
// Constructor of the class
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyAabb&:     Bounding box to render.
//      const FLYCOLOR&:    Color of the box.
//------------------------------------------------------------------------
FlyBBoxHelper::FlyBBoxHelper( const FlyAabb& box,const FLYCOLOR& clr )
{
    VERTEXL verts[8];

    // Get the resouce manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    m_RenderType = RT_LINELIST;
    m_nPriority = PRIORITY_DEFAULT;
    m_pMaterial = NULL;

    m_BBox = box;
    m_WireClr = clr;

    for( int i=0;i<8;i++ )
    {
        FlyVector pt = m_BBox.GetCorner( (FlyAabb::CornerID)i );

        verts[i].x = pt.x;
        verts[i].y = pt.y;
        verts[i].z = pt.z;
        verts[i].color = m_WireClr.GetRGBAValue();
        verts[i].tu = verts[i].tv = 0.0f;
    }

    // Build the vertex buffer.
    m_pVB = pResMgr->MakeVertexBuffer( 8*sizeof(VERTEXL),BU_WRITEONLY,MM_MANAGED );
    m_pVB->WriteData( 0,8*sizeof(VERTEXL),verts,false );

    WORD indis[24] =
    {
        0,1,1,2,2,3,3,0,
        4,5,5,6,6,7,7,4,
        0,4,3,7,1,5,2,6,
    };

    // Build the index buffer.
    m_pIB = pResMgr->MakeIndexBuffer( sizeof(WORD)*24,BU_WRITEONLY,MM_MANAGED,IF_INDEX16 );
    m_pIB->WriteData( 0,sizeof(WORD)*24,indis,false );
}

//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyBBoxHelper::~FlyBBoxHelper(void)
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );
}

//------------------------------------------------------------------------
// Render the bounding box for engine.
//------------------------------------------------------------------------
void FlyBBoxHelper::Render(void)
{
    // Get the renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,8,24,m_pVB,m_pIB );
}

//------------------------------------------------------------------------
// Update the geometry data for the bounding box.
//------------------------------------------------------------------------
void FlyBBoxHelper::UpdateBBox( const FlyAabb& box )
{
    VERTEXL verts[8];
    m_BBox = box;

    for( int i=0;i<8;i++ )
    {
        FlyVector pt = m_BBox.GetCorner( (FlyAabb::CornerID)i );

        verts[i].x = pt.x;
        verts[i].y = pt.y;
        verts[i].z = pt.z;
        verts[i].color = m_WireClr.GetRGBAValue();
        verts[i].tu = verts[i].tv = 0.0f;
    }

    // Build the vertex buffer.
    m_pVB->WriteData( 0,8*sizeof(VERTEXL),verts,false );
}

//------------------------------------------------------------------------
// Set the wire color for the bounding box.
//------------------------------------------------------------------------
void FlyBBoxHelper::SetWireColor( const FLYCOLOR& clr )
{
    m_WireClr = clr;

    UpdateBBox( m_BBox );
}


//------------------------------------------------------------------------
// Get the bounding box for the object.
//------------------------------------------------------------------------
const FlyAabb& FlyBBoxHelper::GetBoundingBox(void) const
{
    return m_BBox;
}

//------------------------------------------------------------------------
// Get the square camera distance to the object.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float FlyBBoxHelper::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter();

    return (vTemp - vCamPos).GetSqrLength();
}


//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void FlyBBoxHelper::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    assert( pMatrices );

    *pMatrices = FlyMatrix::IDENTITY;
}


//------------------------------------------------------------------------
// Get the count of world matrices.
//------------------------------------------------------------------------
UINT FlyBBoxHelper::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get the count of lights.
//------------------------------------------------------------------------
UINT FlyBBoxHelper::GetLightCount(void) const
{
    return 0;
}


//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* FlyBBoxHelper::GetLightObject( UINT nIndex )
{
    return NULL;
}


//------------------------------------------------------------------------
// Get the number of the vertices.
//------------------------------------------------------------------------
UINT FlyBBoxHelper::GetNumVerts(void) const
{
    return 8;
}

//------------------------------------------------------------------------
// Get the number of the indices.
//------------------------------------------------------------------------
UINT FlyBBoxHelper::GetNumIndis(void) const
{
    return 24;
}

//------------------------------------------------------------------------
// Get the number of the primitives.
//------------------------------------------------------------------------
UINT FlyBBoxHelper::GetNumPrims(void) const
{
    return 12;
}

//------------------------------------------------------------------------
// Get the vertices buffer of the object.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyBBoxHelper::GetVertexBuffer(void)
{
    return m_pVB;
}

//------------------------------------------------------------------------
// Get the indices buffer of the object.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyBBoxHelper::GetIndexBuffer(void)
{
    return m_pIB;
}


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYSKYBOXHELPER                             *
*                                                                        *
*************************************************************************/

//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlySkyboxHelper::FlySkyboxHelper(void)
{
    m_fSize = 0.0f;
    m_mWorld = FlyMatrix::IDENTITY;

    m_pFront = NULL;
    m_pBack = NULL;
    m_pLeft = NULL;
    m_pRight = NULL;
    m_pTop = NULL;
    m_pBottom = NULL;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_pVB = NULL;
    m_nPriority = PRIORITY_SKYBOX;

    // Create the material for skybox.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    m_pMaterial = pResMgr->GetMaterialByName( "Core/Skybox" );
    if( !m_pMaterial )
    {
        m_pMaterial = pResMgr->CreateMaterial( "Core/Skybox" );
        m_pMaterial->SetDepthMode( 0,DEPTH_NONE );
        m_pMaterial->SetTextureAddressingMode( 0,0,TADDR_CLAMP,TADDR_CLAMP,TADDR_CLAMP );
    }
}

//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlySkyboxHelper::~FlySkyboxHelper(void)
{
    SAFE_DELETE( m_pVB );

    // Destroy the textures.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    if( m_pFront ) pResMgr->DeleteTexture( m_pFront );
    if( m_pBack ) pResMgr->DeleteTexture( m_pBack );
    if( m_pLeft ) pResMgr->DeleteTexture( m_pLeft );
    if( m_pRight ) pResMgr->DeleteTexture( m_pRight );
    if( m_pTop ) pResMgr->DeleteTexture( m_pTop );
    if( m_pBottom ) pResMgr->DeleteTexture( m_pBottom );
}


//------------------------------------------------------------------------
// Build the skybox for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Size of the skybox.
//------------------------------------------------------------------------
HRESULT FlySkyboxHelper::BuildSkybox( float fSize )
{
    float fHalfSize;

    // Clean the old data.
    SAFE_DELETE( m_pVB );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    fSize = fSize;
    fHalfSize = fSize / 2.0f;

    m_nNumVerts = 24;
    m_nNumIndis = 0;
    m_nNumPrims = 12;

    // Create the vertex buffer.
    m_pVB = pResMgr->MakeVertexBuffer( 24*sizeof(VERTEX),BU_WRITEONLY,MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    // Lock the vertex buffer.
    VERTEX* pVerts = (VERTEX*)m_pVB->Lock( LOCK_NORMAL );

    // Front face.
    pVerts->x = -fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = -fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 0.0f;
    pVerts++;

    // Back face.
    pVerts->x =  fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x = -fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = -fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 0.0f;
    pVerts++;

    // Left face.
    pVerts->x = -fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = -fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x = -fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = -fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 0.0f;
    pVerts++;

    // Right face.
    pVerts->x =  fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 0.0f;
    pVerts++;

    // Top face.
    pVerts->x = -fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x = -fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y =  fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 1.0f;
    pVerts++;

    // Bottom face.
    pVerts->x = -fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = -fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 0.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z = -fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x =  fHalfSize;
    pVerts->y = -fHalfSize;
    pVerts->z =  fHalfSize;
    pVerts->tu = 1.0f;
    pVerts->tv = 0.0f;
    pVerts++;

    m_pVB->Unlock();
    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the front texture for the skybox.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Which Face.
//      const char*:    Path of the texture file.
//------------------------------------------------------------------------
void FlySkyboxHelper::SetSkyTexture( CUBEFACE Face,const char* cTexture )
{
    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    switch( Face )
    {
    case CF_FRONT:
        if( cTexture )
        {
            if( m_pFront )
                m_pFront->Load2DTexture( cTexture,0 );
            else
                m_pFront = pResMgr->Load2DTexture( cTexture,0 );
        }
        else
        {
            pResMgr->DeleteTexture( m_pFront );
            m_pFront = NULL;
        }
        break;
    case CF_BACK:
        if( cTexture )
        {
            if( m_pBack )
                m_pBack->Load2DTexture( cTexture,0 );
            else
                m_pBack = pResMgr->Load2DTexture( cTexture,0 );
        }
        else
        {
            pResMgr->DeleteTexture( m_pBack );
            m_pBack = NULL;
        }
        break;
    case CF_LEFT:
        if( cTexture )
        {
            if( m_pLeft )
                m_pLeft->Load2DTexture( cTexture,0 );
            else
                m_pLeft = pResMgr->Load2DTexture( cTexture,0 );
        }
        else
        {
            pResMgr->DeleteTexture( m_pLeft );
            m_pLeft = NULL;
        }
        break;
    case CF_RIGHT:
        if( cTexture )
        {
            if( m_pRight )
                m_pRight->Load2DTexture( cTexture,0 );
            else
                m_pRight = pResMgr->Load2DTexture( cTexture,0 );
        }
        else
        {
            pResMgr->DeleteTexture( m_pRight );
            m_pRight = NULL;
        }
        break;
    case CF_TOP:
        if( cTexture )
        {
            if( m_pTop )
                m_pTop->Load2DTexture( cTexture,0 );
            else
                m_pTop = pResMgr->Load2DTexture( cTexture,0 );
        }
        else
        {
            pResMgr->DeleteTexture( m_pTop );
            m_pTop = NULL;
        }
        break;
    case CF_BOTTOM:
        if( cTexture )
        {
            if( m_pBottom )
                m_pBottom->Load2DTexture( cTexture,0 );
            else
                m_pBottom = pResMgr->Load2DTexture( cTexture,0 );
        }
        else
        {
            pResMgr->DeleteTexture( m_pBottom );
            m_pBottom = NULL;
        }
        break;
    }
}


//------------------------------------------------------------------------
// Set the position of the camera.
//------------------------------------------------------------------------
void FlySkyboxHelper::SetCameraPos( const FlyVector& vCamPos )
{
    m_mWorld._41 = vCamPos.x;
    m_mWorld._42 = vCamPos.y;
    m_mWorld._43 = vCamPos.z;
}


//------------------------------------------------------------------------
// Render the skybox for the engine.
//------------------------------------------------------------------------
void FlySkyboxHelper::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();
    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( RT_TRIANGLESTRIP );

    // Front face.
    m_pMaterial->SetTexture( 0,0,m_pFront );
    pRenderer->RenderBuffer( VF_DEFAULT,0,4,m_pVB );

    // Back face.
    m_pMaterial->SetTexture( 0,0,m_pBack );
    pRenderer->RenderBuffer( VF_DEFAULT,4,4,m_pVB );

    // Left face.
    m_pMaterial->SetTexture( 0,0,m_pLeft );
    pRenderer->RenderBuffer( VF_DEFAULT,8,4,m_pVB );

    // Right face.
    m_pMaterial->SetTexture( 0,0,m_pRight );
    pRenderer->RenderBuffer( VF_DEFAULT,12,4,m_pVB );

    // Top face.
    m_pMaterial->SetTexture( 0,0,m_pTop );
    pRenderer->RenderBuffer( VF_DEFAULT,16,4,m_pVB );

    // Bottom face.
    m_pMaterial->SetTexture( 0,0,m_pBottom );
    pRenderer->RenderBuffer( VF_DEFAULT,20,4,m_pVB );
}


//------------------------------------------------------------------------
// Get the count of vertices.
//------------------------------------------------------------------------
UINT FlySkyboxHelper::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT FlySkyboxHelper::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT FlySkyboxHelper::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& FlySkyboxHelper::GetBoundingBox(void) const
{
    return FlyAabb::EMPTY;
}

//------------------------------------------------------------------------
// Get the square distance to the camera.
//------------------------------------------------------------------------
float FlySkyboxHelper::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    return 0.0f;
}

//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void FlySkyboxHelper::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_mWorld;
}

//------------------------------------------------------------------------
// Get the count of world matrices.
//------------------------------------------------------------------------
UINT FlySkyboxHelper::GetNumWorldMatrices(void) const
{
    return 1;
}

//------------------------------------------------------------------------
// Get the count of the light objects.
//------------------------------------------------------------------------
UINT FlySkyboxHelper::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
//------------------------------------------------------------------------
FlyLightObject* FlySkyboxHelper::GetLightObject( UINT nIndex )
{
    return NULL;
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS FLYSHADOWHELPER

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyShadowHelper::FlyShadowHelper(void)
{
    m_nNumVerts = 4;
    m_nNumPrims = 2;
    m_nNumIndis = 0;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    m_pMaterial = pResMgr->GetMaterialByName( "Core/StencilRect" );
    if( !m_pMaterial )
    {
        m_pMaterial = pResMgr->CreateMaterial( "Core/StencilRect" );
        m_pMaterial->SetDepthMode( 0,DEPTH_NONE );
        m_pMaterial->SetStencilMode( 0,true,false );
        m_pMaterial->SetStencilRef( 0,0x1 );
        m_pMaterial->SetStencilFunc( 0,CMP_LESSEQUAL,CMP_ALWAYS );
        m_pMaterial->SetStencilOperationF( 0,STOP_KEEP,STOP_KEEP,STOP_KEEP );

        m_pMaterial->SetSceneAlphaMode( 0,true,false );
        m_pMaterial->SetAlphaBlendMode( 0,SBM_SRCALPHA,SBM_INVSRCALPHA );

        TEXTUREBLENDMODE blend;
        blend.Type = BT_COLOR;
        blend.Arg1 = BSTA_TEXTURE;
        blend.Arg2 = BSTA_DIFFUSE;
        blend.Operation = TOP_MODULATE;
        m_pMaterial->SetTextureBlendMode( 0,0,blend );
        blend.Type = BT_ALPHA;
        m_pMaterial->SetTextureBlendMode( 0,0,blend );
    }

    // Create the vertex buffer.
    LPFLYRENDERDEVICE pDevice = FlyKernel::Instance().GetRenderDevice();
    float sx = (float)pDevice->GetBackWidth();
    float sy = (float)pDevice->GetBackHeight();
    m_pVB = pResMgr->MakeVertexBuffer( sizeof(SVERTEX)*4,BU_WRITEONLY,MM_MANAGED );
    SVERTEX* ptr = (SVERTEX*)m_pVB->Lock( LOCK_NORMAL );
    ptr->position = FlyVector( 0.0f,sy,0.0f,1.0f );
    ptr->color = 0x7f000000;
    ptr++;
    ptr->position = FlyVector( 0.0f,0.0f,0.0f,1.0f );
    ptr->color = 0x7f000000;
    ptr++;
    ptr->position = FlyVector( sx,sy,0.0f,1.0f );
    ptr->color = 0x7f000000;
    ptr++;
    ptr->position = FlyVector( sx,0.0f,0.0f,1.0f );
    ptr->color = 0x7f000000;
    m_pVB->Unlock();
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyShadowHelper::~FlyShadowHelper(void)
{
    SAFE_DELETE( m_pVB );
}


//------------------------------------------------------------------------
// Add me to the screen surface object.
//------------------------------------------------------------------------
void FlyShadowHelper::AddToRenderQueue(void)
{
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();
    pRenderer->AddLayoutObject( this );
}


//------------------------------------------------------------------------
// Render a big rectangle which cover with the whole screen.
//------------------------------------------------------------------------
void FlyShadowHelper::Render(void)
{
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( RT_TRIANGLESTRIP );
    pRenderer->RenderBuffer( VF_STENCILSHADOW,4,0,m_pVB,NULL );
}


//------------------------------------------------------------------------
// Get the count of vertices.
//------------------------------------------------------------------------
UINT FlyShadowHelper::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of indices.
//------------------------------------------------------------------------
UINT FlyShadowHelper::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of primitives.
//------------------------------------------------------------------------
UINT FlyShadowHelper::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& FlyShadowHelper::GetBoundingBox(void) const
{
    return FlyAabb::EMPTY;
}

//------------------------------------------------------------------------
// Get the square distance to the camera.
//------------------------------------------------------------------------
float FlyShadowHelper::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    return 0.0f;
}

//------------------------------------------------------------------------
// Get the world matrix of the renderable object.
//------------------------------------------------------------------------
void FlyShadowHelper::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = FlyMatrix::IDENTITY;
}

//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT FlyShadowHelper::GetNumWorldMatrices(void) const
{
    return 1;
}

//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT FlyShadowHelper::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
//------------------------------------------------------------------------
FlyLightObject* FlyShadowHelper::GetLightObject( UINT nIndex )
{
    return NULL;
}