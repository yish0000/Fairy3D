//------------------------------------------------------------------------
// Name: etVegetationObject.cpp
// Desc: This file is to implement the class etVegetationObject.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etVegetationObject.h"

#include "etCoreManager.h"
#include "etSceneManager.h"

#include "FlyMesh.h"
#include "FlyMeshManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etVegetationObject::etVegetationObject( const char* cName )
{
    m_sName = cName;
    m_Type = VGT_POLYGON;

    m_pMesh = NULL;
    m_nDirtyState = -1;
    m_pTexture = NULL;

    m_fBeyondDist = 100.0f;
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etVegetationObject::~etVegetationObject(void)
{
    RenderableList::iterator itr;
    for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
        SAFE_DELETE( *itr );

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Get the pointer to the meshManager.
    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();

    if( m_Type == VGT_POLYGON )
        pResMgr->DeleteTexture( m_pTexture );
    else
    {
        MaterialList::iterator mat;
        for( mat=m_Materials.begin();mat!=m_Materials.end();mat++ )
        pResMgr->DeleteMaterial( *mat );

        pMeshMgr->DecResourceRef( m_pMesh->GetName() );
    }
}


//------------------------------------------------------------------------
// Initialize the vegetation object.
// ----------------------------------------------------------------------
// Param -> IN:
//      VEGETATIONTYPE: Vegetation object type.
//      const char*:    Name of the mesh resource. (Mesh vegetation)
//      float:          Width of the polygon. (Polygon vegetation)
//      float:          Height of the polygon. (Polygon vegetation)
//      const etFloatRect&: Texture coord rect.
//      const char*:    Path to the texture.
//      UINT:           Count of the polygons.
//------------------------------------------------------------------------
HRESULT etVegetationObject::Initialize( VEGETATIONTYPE Type,const char* cMeshName,
                                        float fWidth,float fHeight,const etFloatRect& texRect,
                                        const char* cTexture,UINT nNumPolygons )
{
    m_Type = Type;

    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();

    if( Type == VGT_POLYGON )
    {
        // Load the texture from file.
        m_pTexture = pResMgr->Load2DTexture( cTexture,0 );

        // Build the renderable object.
        return BuildPolygonRenderable( fWidth,fHeight,nNumPolygons,texRect );
    }
    else
    {
        m_pMesh = pMeshMgr->GetMeshByName( cMeshName );
        pMeshMgr->AddResourceRef( cMeshName );
        m_nDirtyState = m_pMesh->GetDirtyState();

        // Build the mesh object.
        return BuildMeshRenderable();
    }
}


//------------------------------------------------------------------------
// Build the mesh renderable object.
//------------------------------------------------------------------------
HRESULT etVegetationObject::BuildMeshRenderable(void)
{
    std::string sMatName;

    // Get the pointer to the resouceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // 1. Create the render materials.
    for( UINT i=0;i<m_pMesh->GetNumMaterials();i++ )
    {
        char cTemp[20];
        FlyRenderMaterial* pMaterial;

        sprintf( cTemp,"_%d",i );
        sMatName = "vegetation_" + m_sName + cTemp;
        pMaterial = pResMgr->CreateMaterial( sMatName.c_str() );
        if( !pMaterial ) return FLY_OUTOFMEMORY;

        FlyRenderEffect* pEffect = etCoreManager::Instance().GetVegetationFX();
        pMaterial->AddNewTechnique( pEffect->GetTechniqueByIndex(1) );
        pMaterial->SetActiveTechnique( 0 );

        FlyRenderEffectInstance* pInstance = pMaterial->GetActiveRenderEffectInstance();
        *pInstance->GetParameterByIndex(2) = m_pMesh->GetMaterial(i)->pTexture;

        m_Materials.push_back( pMaterial );
    }

    // 2. Create the renderable objects.
    for( i=0;i<m_pMesh->GetNumSubMeshes();i++ )
    {
        FlySubMesh* pSubMesh = m_pMesh->GetSubMesh( i );

        // Create a new renderable.
        etVegetationRenderable* pObject = new etVegetationRenderable( this );
        if( !pObject ) return FLY_OUTOFMEMORY;

        pObject->m_nNumVerts = pSubMesh->GetNumVerts();
        pObject->m_nNumIndis = pSubMesh->GetNumIndis();
        pObject->m_nNumPrims = pSubMesh->GetNumPrims();

        pObject->m_pVB = pSubMesh->GetVertexBuffer();
        pObject->m_pIB = pSubMesh->GetIndexBuffer();

        pObject->SetRenderMaterial( m_Materials[pSubMesh->GetRenderMaterial()] );
        pObject->m_BBox = pSubMesh->GetBoundingBox();

        // Add the new object to list.
        m_Renderables.push_back( pObject );
    }

    // Set the local boundingBox.
    m_Bounds = m_pMesh->GetBoundingBox();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the polygon renderable object.
//------------------------------------------------------------------------
HRESULT etVegetationObject::BuildPolygonRenderable( float fWidth,float fHeight,UINT nNumPolygons,
                                                    const etFloatRect& texRect )
{
    std::string sMatName;

    // Get the pointer to the sceneManager.
    etSceneManager* pManager = (etSceneManager*)etCoreManager::Instance().GetSceneManager();

    // Create the renderable object.
    etVegetationRenderable* pRenderable = new etVegetationRenderable( this );
    if( !pRenderable ) return FLY_OUTOFMEMORY;

    pRenderable->m_nNumVerts = nNumPolygons * 4;
    pRenderable->m_nNumIndis = nNumPolygons * 6;
    pRenderable->m_nNumPrims = nNumPolygons * 2;

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Create the vertex buffer.
    pRenderable->m_pVB = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*pRenderable->m_nNumVerts,
        BU_WRITEONLY,MM_MANAGED );
    if( !pRenderable->m_pVB ) return FLY_CREATEBUFFER;
    VERTEXL* pVerts = (VERTEXL*)pRenderable->m_pVB->Lock( LOCK_NORMAL );

    // Create the index buffer.
    pRenderable->m_pIB = pResMgr->MakeIndexBuffer( sizeof(WORD)*pRenderable->m_nNumIndis,
        BU_WRITEONLY,MM_MANAGED,IF_INDEX16 );
    if( !pRenderable->m_pIB ) return FLY_CREATEBUFFER;
    WORD* pIndis = (WORD*)pRenderable->m_pIB->Lock( LOCK_NORMAL );

    float fAngle = FLY_PI / nNumPolygons;
    float fRadius = fWidth / 2.0f;
    UINT nVertOffset = 0;

    for( int i=0;i<nNumPolygons;i++ )
    {
        float fX1 = -fRadius * cosf( fAngle*i );
        float fZ1 =  fRadius * sinf( fAngle*i );
        float fX2 = -fRadius * cosf( fAngle*i+FLY_PI );
        float fZ2 =  fRadius * sinf( fAngle*i+FLY_PI );

        pVerts->x = fX1;
        pVerts->y = -fHeight / 2.0f;
        pVerts->z = fZ1;
        pVerts->color = 0xffffffff;
        pVerts->tu = texRect.GetMinPointX();
        pVerts->tv = texRect.GetMaxPointZ();
        pVerts++;
        pVerts->x = fX1;
        pVerts->y = fHeight / 2.0f;
        pVerts->z = fZ1;
        pVerts->color = 0xffffffff;
        pVerts->tu = texRect.GetMinPointX();
        pVerts->tv = texRect.GetMinPointZ();
        pVerts++;
        pVerts->x = fX2;
        pVerts->y = fHeight / 2.0f;
        pVerts->z = fZ2;
        pVerts->color = 0xffffffff;
        pVerts->tu = texRect.GetMaxPointX();
        pVerts->tv = texRect.GetMinPointZ();
        pVerts++;
        pVerts->x = fX2;
        pVerts->y = -fHeight / 2.0f;
        pVerts->z = fZ2;
        pVerts->color = 0xffffffff;
        pVerts->tu = texRect.GetMaxPointX();
        pVerts->tv = texRect.GetMaxPointZ();
        pVerts++;

        *pIndis++ = 0 + nVertOffset;
        *pIndis++ = 1 + nVertOffset;
        *pIndis++ = 2 + nVertOffset;
        *pIndis++ = 2 + nVertOffset;
        *pIndis++ = 3 + nVertOffset;
        *pIndis++ = 0 + nVertOffset;

        nVertOffset += 4;
    }

    pRenderable->m_pVB->Unlock();
    pRenderable->m_pIB->Unlock();

    // Calculate the boundingBox.
    pRenderable->m_BBox.vcMin.Set( -fRadius,-fHeight/2.0f,-fRadius );
    pRenderable->m_BBox.vcMax.Set(  fRadius, fHeight/2.0f, fRadius );

    // Create the render material for the object.
    sMatName = "vegetation_" + m_sName;
    pRenderable->m_pMaterial = pResMgr->CreateMaterial( sMatName.c_str() );

    FlyRenderEffect* pEffect = etCoreManager::Instance().GetVegetationFX();
    pRenderable->m_pMaterial->AddNewTechnique( pEffect->GetTechniqueByIndex(0) );
    pRenderable->m_pMaterial->SetActiveTechnique( 0 );

    FlyRenderEffectInstance* pInstance = pRenderable->m_pMaterial->GetActiveRenderEffectInstance();
    *pInstance->GetParameterByIndex(2) = m_pTexture;

    // Set the local boundingBox.
    m_Bounds = pRenderable->m_BBox;

    m_Renderables.push_back( pRenderable );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Reinitialize the vegetation object. (Only for Mesh type)
//------------------------------------------------------------------------
HRESULT etVegetationObject::Reinitialize(void)
{
    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Destroy the old renderables.
    RenderableList::iterator obj;
    for( obj=m_Renderables.begin();obj!=m_Renderables.end();obj++ )
        SAFE_DELETE( *obj );
    m_Renderables.clear();

    // Destroy the render materials.
    MaterialList::iterator mat;
    for( mat=m_Materials.begin();mat!=m_Materials.end();mat++ )
        pResMgr->DeleteMaterial( *mat );
    m_Materials.clear();

    // Update the dirty state.
    m_nDirtyState = m_pMesh->GetDirtyState();

    return BuildMeshRenderable();
}


//------------------------------------------------------------------------
// Update the vegetation object.
//------------------------------------------------------------------------
void etVegetationObject::UpdateObject(void)
{
    if( m_Type == VGT_MESH && 
        m_nDirtyState != m_pMesh->GetDirtyState() )
        Reinitialize();

    m_WorldBBox = m_Bounds;
    m_WorldBBox.TransformBox( GetWorldTransform() );
}


//------------------------------------------------------------------------
// Render the vegetation object.
//------------------------------------------------------------------------
void etVegetationObject::RenderObject(void)
{
    FlyRenderEffectInstance* pInstance;
    float fCamDist = sqrtf(m_fSqrDistToCamera) - m_WorldBBox.GetBoundingRadius();

    if( m_Type == VGT_POLYGON )
    {
        FlyRenderMaterial* pMaterial = m_Renderables[0]->GetRenderMaterial();
        pInstance = pMaterial->GetActiveRenderEffectInstance();
        *pInstance->GetParameterByIndex(0) = fCamDist;
        *pInstance->GetParameterByIndex(1) = m_fBeyondDist;

        m_Renderables[0]->AddToRenderQueue();
    }
    else
    {
        MaterialList::iterator mat;
        for( mat=m_Materials.begin();mat!=m_Materials.end();mat++ )
        {
            pInstance = (*mat)->GetActiveRenderEffectInstance();
            *pInstance->GetParameterByIndex(0) = fCamDist;
            *pInstance->GetParameterByIndex(1) = m_fBeyondDist;
        }

        RenderableList::iterator itr;
        for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
            (*itr)->AddToRenderQueue();
    }
}


//------------------------------------------------------------------------
// Get the local boundingBox of the vegetation object.
//------------------------------------------------------------------------
const FlyAabb& etVegetationObject::GetLocalBoundingBox(void) const
{
    return m_Bounds;
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS ETVEGETATIONRENDERABLE.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etVegetationRenderable::etVegetationRenderable( etVegetationObject* pParent )
{
    m_pParent = pParent;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_pVB = m_pIB = NULL;
    m_nPriority = PRIORITY_ALPHA;
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etVegetationRenderable::~etVegetationRenderable(void)
{
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    if( m_pParent->GetVegetationType() == VGT_POLYGON )
    {
        SAFE_DELETE( m_pVB );
        SAFE_DELETE( m_pIB );

        pResMgr->DeleteMaterial( m_pMaterial );
    }
}


//------------------------------------------------------------------------
// Render the vegetation renderable object.
//------------------------------------------------------------------------
void etVegetationRenderable::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = etCoreManager::Instance().GetRenderer();
    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( RT_TRIANGLELIST );

    if( m_pParent->GetVegetationType() == VGT_POLYGON )
        pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
    else
        pRenderer->RenderBuffer( VF_DEFAULT,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT etVegetationRenderable::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT etVegetationRenderable::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT etVegetationRenderable::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the local boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& etVegetationRenderable::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the square distance to the camera.
//------------------------------------------------------------------------
float etVegetationRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter() * m_pParent->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}

//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void etVegetationRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pParent->GetWorldTransform();
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS ETVEGETATIONFACTORY.

//------------------------------------------------------------------------
// Create an instance of the vegetation object.
//------------------------------------------------------------------------
FlySceneObject* etVegetationFactory::CreateInstance( const char* cName )
{
    FlySceneObject* pEntity = new etVegetationObject( cName );
    if( !pEntity ) return NULL;

    pEntity->SetCreator( this );
    return pEntity;
}

//------------------------------------------------------------------------
// Destroy the specified vegetation object.
//------------------------------------------------------------------------
void etVegetationFactory::DestroyInstance( FlySceneObject* pObject )
{
    SAFE_DELETE( pObject );
}