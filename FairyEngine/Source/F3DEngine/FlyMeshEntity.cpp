//------------------------------------------------------------------------
// Name: FlyMeshEntity.cpp
// Desc: This file is to implement the class FlyMeshEntity.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyMeshEntity.h"
#include "FlySceneNode.h"

#include "FlyKernel.h"
#include "FlyMeshManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the mesh entity.
//------------------------------------------------------------------------
FlyMeshEntity::FlyMeshEntity( const char* cName )
{
    m_sName = cName;

    m_pMesh = NULL;
    m_bNeedRebuildShadows = true;
    m_bShowBBox = false;
    m_pBBoxHelper = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyMeshEntity::~FlyMeshEntity(void)
{
    SAFE_DELETE( m_pBBoxHelper );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    MaterialList::iterator mobj;
    for( mobj=m_Materials.begin();mobj!=m_Materials.end();mobj++ )
        pResMgr->DeleteMaterial( *mobj );

    RenderableList::iterator robj;
    for( robj=m_Renderables.begin();robj!=m_Renderables.end();robj++ )
        SAFE_DELETE( *robj );

    ShadowVolumeList::iterator sobj;
    for( sobj=m_ShadowVolumes.begin();sobj!=m_ShadowVolumes.end();sobj++ )
        SAFE_DELETE( *sobj );

    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();
    pMeshMgr->DecResourceRef( m_pMesh->GetName() );
}


//------------------------------------------------------------------------
// Initialize the mesh entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyMesh*:   Pointer to the mesh template.
//------------------------------------------------------------------------
HRESULT FlyMeshEntity::Initialize( FlyMesh* pMesh )
{
    HRESULT hr;

    m_pMesh = pMesh;

    // Initialize the dirty state.
    m_nDirtyState = pMesh->GetDirtyState();

    // Add the resource reference.
    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();
    pMeshMgr->AddResourceRef( pMesh->GetName() );

    // Build the renderables.
    hr = BuildRenderables();
    if( FAILED(hr) ) return hr;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the renderable objects for the entity.
//------------------------------------------------------------------------
HRESULT FlyMeshEntity::BuildRenderables(void)
{
    std::string sMatName;

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // 1. Create the render materials.
    for( UINT i=0;i<m_pMesh->GetNumMaterials();i++ )
    {
        FLYMATERIAL mat;
        FlyTexture* pTexture;
        char cTemp[20];
        FlyRenderMaterial* pMaterial;

        sprintf( cTemp,"_%d",i );
        sMatName = "mesh_" + m_sName + cTemp;
        pMaterial = pResMgr->CreateMaterial( sMatName.c_str() );
        if( !pMaterial ) return FLY_OUTOFMEMORY;

        memcpy( &mat,m_pMesh->GetMaterial(i),sizeof(FLYMATERIAL) );
        pMaterial->SetStdMaterial( 0,&mat );

        pTexture = m_pMesh->GetMaterial(i)->pTexture;
        if( pTexture )
        {
            pMaterial->SetTexture( 0,0,pTexture );

            if( pTexture->HasAlphaChannel() )
            {
                pMaterial->SetSceneAlphaMode( 0,true,false );
                pMaterial->SetAlphaBlendMode( 0,SBM_SRCALPHA,SBM_INVSRCALPHA );
                pMaterial->SetDepthMode( 0,DEPTH_READONLY );
            }
        }

        m_Materials.push_back( pMaterial );
    }

    for( UINT i=0;i<m_pMesh->GetNumSubMeshes();i++ )
    {
        FlySubMesh* pSubMesh = m_pMesh->GetSubMesh( i );

        // Create a new renderable.
        FlyMeshRenderable* pObject = new FlyMeshRenderable( this );
        if( !pObject ) return FLY_OUTOFMEMORY;

        pObject->m_nNumVerts = pSubMesh->GetNumVerts();
        pObject->m_nNumIndis = pSubMesh->GetNumIndis();
        pObject->m_nNumPrims = pSubMesh->GetNumPrims();

        pObject->SetVertexBuffer( pSubMesh->GetVertexBuffer() );
        pObject->SetIndexBuffer( pSubMesh->GetIndexBuffer() );

        pObject->SetRenderMaterial( m_Materials[pSubMesh->GetRenderMaterial()] );
        pObject->m_BBox = pSubMesh->GetBoundingBox();

        if( pObject->m_pMaterial->IsTransparent() )
            pObject->SetRenderPriority( PRIORITY_ALPHA );

        // Add the new object to list.
        m_Renderables.push_back( pObject );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the shadow volumes for the mesh entity.
//------------------------------------------------------------------------
HRESULT FlyMeshEntity::BuildShadowVolumes(void)
{
    for( UINT i=0;i<m_pMesh->GetNumSubMeshes();i++ )
    {
        FlySubMesh* pSubMesh = m_pMesh->GetSubMesh( i );

        UINT nNumVerts = pSubMesh->GetNumVerts();
        UINT nNumIndis = pSubMesh->GetNumIndis();

        BYTE* pVerts = (BYTE*)pSubMesh->GetVertexBuffer()->Lock( LOCK_NORMAL );
        WORD* pIndis = (WORD*)pSubMesh->GetIndexBuffer()->Lock( LOCK_NORMAL );

        LightObjectList::iterator itr;
        for( itr=m_ShadowCasters.begin();itr!=m_ShadowCasters.end();itr++ )
        {
            FlyShadowVolume* pShadow = new FlyShadowVolume();
            pShadow->BuildShadowVolume( nNumVerts,nNumIndis,pVerts,pIndis,sizeof(VERTEX) );
            m_ShadowVolumes.push_back( pShadow );
        }

        pSubMesh->GetVertexBuffer()->Unlock();
        pSubMesh->GetIndexBuffer()->Unlock();
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Reinitialize the mesh entity.
//------------------------------------------------------------------------
HRESULT FlyMeshEntity::Reinitialize(void)
{
    HRESULT hr;

    // Destroy the old renderables.
    RenderableList::iterator robj;
    for( robj=m_Renderables.begin();robj!=m_Renderables.end();robj++ )
        SAFE_DELETE( *robj );
    m_Renderables.clear();

    // Rebuild the renderable objects.
    hr = BuildRenderables();
    if( FAILED(hr) ) return hr;

    if( m_ShadowVolumes.size() != 0 )
    {
        // Destroy the old shadow volumes.
        ShadowVolumeList::iterator sobj;
        for( sobj=m_ShadowVolumes.begin();sobj!=m_ShadowVolumes.end();sobj++ )
            SAFE_DELETE( *sobj );
        m_ShadowVolumes.clear();

        // Rebuild the shadow volumes.
        hr = BuildShadowVolumes();
        if( FAILED(hr) ) return hr;
    }

    // Update the dirty state.
    m_nDirtyState = m_pMesh->GetDirtyState();

    return hr;
}


//------------------------------------------------------------------------
// Update the mesh entity.
//------------------------------------------------------------------------
void FlyMeshEntity::UpdateObject(void)
{
    if( m_nDirtyState != m_pMesh->GetDirtyState() )
        Reinitialize();

    m_WorldBBox = m_pMesh->GetBoundingBox();
    m_WorldBBox.TransformBox( GetWorldTransform() );
}


//------------------------------------------------------------------------
// Update the light objects for the mesh entity.
//------------------------------------------------------------------------
void FlyMeshEntity::UpdateLights(void)
{
    if( m_bNeedUpdateLights && !m_bManualLight )
    {
        m_Lights.clear();
        m_pParent->FindLights( m_Lights );

        m_ShadowCasters.clear();
        LightObjectList::iterator itr;
        for( itr=m_Lights.begin();itr!=m_Lights.end();itr++ )
        {
            if( (*itr)->IsCastShadows() )
                m_ShadowCasters.push_back( *itr );
        }

        m_bNeedUpdateLights = false;
        m_bNeedRebuildShadows = true;
    }
}


//------------------------------------------------------------------------
// Render the mesh entity.
//------------------------------------------------------------------------
void FlyMeshEntity::RenderObject(void)
{
    // Update the light objects.
    if( !m_bOpenLight )
    {
        m_Lights.clear();
        m_ShadowCasters.clear();
    }
    else
        UpdateLights();

    // Add all the renderable objects to queue.
    RenderableList::iterator robj;
    for( robj=m_Renderables.begin();robj!=m_Renderables.end();robj++ )
        (*robj)->AddToRenderQueue();

    // Does we cast shadows ?
    if( m_bHaveShadows )
        UpdateShadowVolumes();

    // Render the boundingBox if needed ?
    if( m_bShowBBox )
    {
        if( !m_pBBoxHelper )
            m_pBBoxHelper = new FlyBBoxHelper( m_WorldBBox,FLYCOLOR::White );
        else
            m_pBBoxHelper->UpdateBBox( m_WorldBBox );

        m_pBBoxHelper->AddToRenderQueue();
    }
}


//------------------------------------------------------------------------
// Update the shadow volumes of the mesh entity.
//------------------------------------------------------------------------
void FlyMeshEntity::UpdateShadowVolumes(void)
{
    if( m_ShadowCasters.size() == 0 )
        return;

    // Build the shadow volumes at the first time.
    if( m_bNeedRebuildShadows )
    {
        ShadowVolumeList::iterator itr;
        for( itr=m_ShadowVolumes.begin();itr!=m_ShadowVolumes.end();itr++ )
            SAFE_DELETE( *itr );
        m_ShadowVolumes.clear();

        BuildShadowVolumes();
    }

    int nIndex = 0;
    for( UINT i=0;i<m_pMesh->GetNumSubMeshes();i++ )
    {
        LightObjectList::iterator itr;
        for( itr=m_ShadowCasters.begin();itr!=m_ShadowCasters.end();itr++ )
        {
            FlyMatrix mInverse;
            mInverse.InverseOf( m_pParent->GetWorldTransform() );
            FlyVector vLight = (*itr)->GetPosition() * mInverse;

            m_ShadowVolumes[nIndex]->UpdateShadowVolume( vLight );
            m_ShadowVolumes[nIndex]->SetWorldMatrix( m_pParent->GetWorldTransform() );

            // Add the shadowVolume to the renderQueue.
            m_ShadowVolumes[nIndex]->GetShadowVolumeRenderable()->AddToRenderQueue();
            nIndex++;
        }
    }
}


//------------------------------------------------------------------------
// Set the lighting mode for the mesh entity.
//------------------------------------------------------------------------
void FlyMeshEntity::SetLightingMode( bool bLighting )
{
    m_bOpenLight = bLighting;

    RenderableList::iterator itr;
    for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
        (*itr)->GetRenderMaterial()->SetLightingMode( 0,true );
}


//------------------------------------------------------------------------
// Get the pointer to the mesh template.
//------------------------------------------------------------------------
const FlyMesh* FlyMeshEntity::GetMeshTemplate(void) const
{
    return m_pMesh;
}


//------------------------------------------------------------------------
// Set the mesh template for the entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyMesh*:       Pointer to the mesh template.
//------------------------------------------------------------------------
void FlyMeshEntity::SetMeshTemplate( FlyMesh* pMesh )
{
    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();

    if( m_pMesh != pMesh )
    {
        pMeshMgr->DecResourceRef( m_pMesh->GetName() );
        m_pMesh = pMesh;
        pMeshMgr->AddResourceRef( m_pMesh->GetName() );

        Reinitialize();
    }
}


//------------------------------------------------------------------------
// Set the mesh template for the entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the mesh template.
//------------------------------------------------------------------------
void FlyMeshEntity::SetMeshTemplate( const char* cMeshName )
{
    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();

    // Get the mesh by its name.
    FlyMesh* pMesh = pMeshMgr->GetMeshByName( cMeshName );
    if( !pMesh ) return;

    if( m_pMesh != pMesh )
    {
        pMeshMgr->DecResourceRef( m_pMesh->GetName() );
        m_pMesh = pMesh;
        pMeshMgr->AddResourceRef( m_pMesh->GetName() );

        Reinitialize();
    }
}


//------------------------------------------------------------------------
// Get the local boundingBox of the entity.
//------------------------------------------------------------------------
const FlyAabb& FlyMeshEntity::GetLocalBoundingBox(void) const
{
    return m_pMesh->GetBoundingBox();
}


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYMESHRENDERABLE                           *
*                                                                        *
*************************************************************************/

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyMeshRenderable::FlyMeshRenderable( FlyMeshEntity* pEntity )
{
    m_pEntity = pEntity;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_pVB = NULL;
    m_pIB = NULL;

    memset( &m_BBox,0,sizeof(FlyAabb) );
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyMeshRenderable::~FlyMeshRenderable(void)
{
}

//------------------------------------------------------------------------
// Set the vertex buffer for the object.
//------------------------------------------------------------------------
void FlyMeshRenderable::SetVertexBuffer( FlyGraphicBuffer* pVB )
{
    m_pVB = pVB;
}

//------------------------------------------------------------------------
// Set the index buffer for the object.
//------------------------------------------------------------------------
void FlyMeshRenderable::SetIndexBuffer( FlyGraphicBuffer* pIB )
{
    m_pIB = pIB;
}

//------------------------------------------------------------------------
// Render the object for the entity.
//------------------------------------------------------------------------
void FlyMeshRenderable::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( VF_DEFAULT,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
}

//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT FlyMeshRenderable::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT FlyMeshRenderable::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT FlyMeshRenderable::GetNumPrims(void) const
{
    return m_nNumPrims;
}

//------------------------------------------------------------------------
// Get the local boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& FlyMeshRenderable::GetBoundingBox(void) const
{
    return m_BBox;
}

//------------------------------------------------------------------------
// Get the Distance to the camera.
//------------------------------------------------------------------------
float FlyMeshRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter() * m_pEntity->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}

//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void FlyMeshRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pEntity->GetWorldTransform();
}

//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT FlyMeshRenderable::GetNumWorldMatrices(void) const
{
    return 1;
}

//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT FlyMeshRenderable::GetLightCount(void) const
{
    return m_pEntity->GetNumLights();
}

//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* FlyMeshRenderable::GetLightObject( UINT nIndex )
{
    return m_pEntity->GetLightObject( nIndex );
}


//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Create a mesh entity instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the mesh entity.
//------------------------------------------------------------------------
FlySceneObject* FlyMeshEntityFactory::CreateInstance( const char* cName )
{
    FlyMeshEntity* pEntity = new FlyMeshEntity( cName );
    if( !pEntity ) return NULL;

    pEntity->SetCreator( this );
    return pEntity;
}


//------------------------------------------------------------------------
// Destroy a mesh entity instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneObject*:    Pointer to the mesh entity.
//------------------------------------------------------------------------
void FlyMeshEntityFactory::DestroyInstance( FlySceneObject* pObject )
{
    SAFE_DELETE( pObject );
}