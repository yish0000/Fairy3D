//------------------------------------------------------------------------
// Name: FlyManualEntity.cpp
// Desc: This file is to implement the class FlyManualEntity.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyManualEntity.h"

#include "FlyKernel.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyManualEntity::FlyManualEntity( const char* cName )
{
    m_sName = cName;

    m_LocalBBox.SetEmpty();
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyManualEntity::~FlyManualEntity(void)
{
    // Get the resource manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    MaterialList::iterator itr;
    for( itr=m_Materials.begin();itr!=m_Materials.end();itr++ )
        pResMgr->DeleteMaterial( *itr );

    // Destroy the sub objects.
    SubObjectList::iterator obj;
    for( obj=m_SubObjects.begin();obj!=m_SubObjects.end();obj++ )
        SAFE_DELETE( *obj );
}


//------------------------------------------------------------------------
// Update the manual entity.
//------------------------------------------------------------------------
void FlyManualEntity::UpdateObject(void)
{
    m_WorldBBox = m_LocalBBox;
    m_WorldBBox.TransformBox( GetWorldTransform() );
}


//------------------------------------------------------------------------
// Render the manual entity.
//------------------------------------------------------------------------
void FlyManualEntity::RenderObject(void)
{
    // Get the pointer to the Renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    // Update the light objects.
    if( !m_bOpenLight )
        m_Lights.clear();
    else
        UpdateLights();

    // Add all the sub objects to renderQueue.
    SubObjectList::iterator obj;
    for( obj=m_SubObjects.begin();obj!=m_SubObjects.end();obj++ )
        (*obj)->AddToRenderQueue();
}


//------------------------------------------------------------------------
// Get the local boundingBox of the entity.
//------------------------------------------------------------------------
const FlyAabb& FlyManualEntity::GetLocalBoundingBox(void) const
{
    return m_LocalBBox;
}


//------------------------------------------------------------------------
// Add a new render material to the entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the material.
//------------------------------------------------------------------------
FlyRenderMaterial* FlyManualEntity::AddMaterial( const char* cMatName )
{
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    FlyRenderMaterial* pMaterial = pResMgr->CreateMaterial( cMatName );
    m_Materials.push_back( pMaterial );
    return pMaterial;
}


//------------------------------------------------------------------------
// Destroy a specified render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the render material.
//------------------------------------------------------------------------
void FlyManualEntity::DestroyMaterial( size_t nIndex )
{
    if( nIndex >= m_Materials.size() )
        return;

    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    pResMgr->DeleteMaterial( m_Materials[nIndex] );
    m_Materials.erase( m_Materials.begin() + nIndex );
}


//------------------------------------------------------------------------
// Get the specified render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the render material.
//------------------------------------------------------------------------
FlyRenderMaterial* FlyManualEntity::GetRenderMaterial( size_t nIndex )
{
    if( nIndex >= m_Materials.size() )
        return NULL;

    return m_Materials[nIndex];
}


//------------------------------------------------------------------------
// Create a new sub object for the user, and begin to edit it.
// ----------------------------------------------------------------------
// Param -> IN:
//      VERTEXTYPE:     Vertex type of the sub object.
//      RENDERTYPE:     Render type of the sub object.
//------------------------------------------------------------------------
FlyManualSubObj* FlyManualEntity::BeginSubObject( VERTEXTYPE vertexType/* =VF_DEFAULT */,
                                                  RENDERTYPE renderType/* =RT_TRIANGLELIST */ )
{
    // Create a new sub object.
    FlyManualSubObj* pNewObj = new FlyManualSubObj( this );
    if( !pNewObj ) return NULL;

    pNewObj->SetVertexType( vertexType );
    pNewObj->SetRenderType( renderType );

    m_SubObjects.push_back( pNewObj );
    return pNewObj;
}


//------------------------------------------------------------------------
// Complete a renderable object, Update the BBox for entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyManualSubObj*:   Pointer to the sub object.
//------------------------------------------------------------------------
void FlyManualEntity::EndSubObject( FlyManualSubObj* pObject )
{
    m_LocalBBox.Merge( pObject->GetBoundingBox() );
}


//------------------------------------------------------------------------
// Destroy the specified sub object.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the sub object.
//------------------------------------------------------------------------
void FlyManualEntity::DestroySubObject( size_t nIndex )
{
    if( nIndex >= m_SubObjects.size() )
        return;

    SAFE_DELETE( m_SubObjects[nIndex] );
    m_SubObjects.erase( m_SubObjects.begin() + nIndex );
}


//------------------------------------------------------------------------
// Get the specified sub object.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the sub object.
//------------------------------------------------------------------------
FlyManualSubObj* FlyManualEntity::GetSubObject( size_t nIndex )
{
    if( nIndex >= m_SubObjects.size() )
        return NULL;

    return m_SubObjects[nIndex];
}


//////////////////////////////////////////////////////////////////////////
// Implementing the class FlyManualSubObj.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyManualSubObj::FlyManualSubObj( FlyManualEntity* pEntity )
{
    m_pParent = pEntity;
    m_VertexType = VF_DEFAULT;
    m_nVertexSize = 0;

    m_pVB = NULL;
    m_pIB = NULL;

    m_nMaxNumVerts = 0;
    m_nMaxNumIndis = 0;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_BBox.SetEmpty();
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyManualSubObj::~FlyManualSubObj(void)
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );
}


//------------------------------------------------------------------------
// Create the vertex buffer and index buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Max count of the vertices.
//      UINT:       Max count of the indices.
//------------------------------------------------------------------------
HRESULT FlyManualSubObj::CreateGraphicBuffer( UINT nMaxNumVerts,UINT nMaxNumIndis )
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );

    // Get the resource Manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    if( nMaxNumIndis >= 65536 )
        return FLY_BUFFERSIZE;

    // Create the vertex buffer.
    m_pVB = pResMgr->MakeVertexBuffer( m_nVertexSize*nMaxNumVerts,BU_WRITEONLY,MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    // Create the index buffer.
    m_pIB = pResMgr->MakeIndexBuffer( sizeof(WORD)*nMaxNumIndis,BU_WRITEONLY,
        MM_MANAGED,IF_INDEX16 );
    if( !m_pIB ) return FLY_CREATEBUFFER;

    m_nMaxNumVerts = nMaxNumVerts;
    m_nMaxNumIndis = nMaxNumIndis;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Add the vertex data to buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:           Count of the vertices.
//      const void*:    Pointer to the vertices data.
//      const FlyAabb&: BoudingBox of the vertices.
//------------------------------------------------------------------------
HRESULT FlyManualSubObj::AddVertexData( UINT nNumVerts,const void* pVerts,
                                        const FlyAabb& box )
{
    if( m_nNumVerts + nNumVerts > m_nMaxNumVerts )
        return FLY_BUFFERSIZE;

    // Write the data.
    m_pVB->WriteData( m_nNumVerts*m_nVertexSize,m_nVertexSize*nNumVerts,
        pVerts,false );

    m_nNumVerts += nNumVerts;
    if( m_RenderType == RT_POINTLIST ) m_nNumPrims += nNumVerts;

    // Update the boundingBox.
    m_BBox.Merge( box );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Add the index data to buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:           Count of the indices.
//      const WORD*:    Pointer to the indices data.
//------------------------------------------------------------------------
HRESULT FlyManualSubObj::AddIndexData( UINT nNumIndis,const WORD* pIndis )
{
    if( m_nNumIndis + nNumIndis > m_nMaxNumIndis )
        return FLY_BUFFERSIZE;

    // Write the data.
    m_pIB->WriteData( sizeof(WORD)*m_nNumIndis,sizeof(WORD)*nNumIndis,
        pIndis,false );

    m_nNumIndis += nNumIndis;

    switch( m_RenderType )
    {
    case RT_LINELIST:
        m_nNumPrims += nNumIndis / 2;
        break;
    case RT_LINESTRIP:
        m_nNumPrims += nNumIndis - 1;
        break;
    case RT_TRIANGLELIST:
        m_nNumPrims += nNumIndis / 3;
        break;
    case RT_TRIANGLESTRIP:
        m_nNumPrims += nNumIndis - 2;
        break;
    case RT_TRIANGLEFAN:
        m_nNumPrims += nNumIndis - 2;
        break;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Clear the vertex and index data.
//------------------------------------------------------------------------
void FlyManualSubObj::ClearData(void)
{
    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_BBox.SetEmpty();
}


//------------------------------------------------------------------------
// Render the manual sub object.
//------------------------------------------------------------------------
void FlyManualSubObj::Render(void)
{
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( m_VertexType,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
}


//------------------------------------------------------------------------
// Get the count of vertices.
//------------------------------------------------------------------------
UINT FlyManualSubObj::GetNumVerts(void) const
{
    return m_nNumVerts;
}


//------------------------------------------------------------------------
// Get the count of indices.
//------------------------------------------------------------------------
UINT FlyManualSubObj::GetNumIndis(void) const
{
    return m_nNumIndis;
}


//------------------------------------------------------------------------
// Get the count of primitives.
//------------------------------------------------------------------------
UINT FlyManualSubObj::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the distance to the camera.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float FlyManualSubObj::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter() * m_pParent->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}


//------------------------------------------------------------------------
// Get the local boundingBox.
//------------------------------------------------------------------------
const FlyAabb& FlyManualSubObj::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void FlyManualSubObj::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    assert( pMatrices );

    *pMatrices = m_pParent->GetWorldTransform();
}

//------------------------------------------------------------------------
// Get the count of world matrices.
//------------------------------------------------------------------------
UINT FlyManualSubObj::GetNumWorldMatrices(void) const
{
    return 1;
}

//------------------------------------------------------------------------
// Get the count of light object.
//------------------------------------------------------------------------
UINT FlyManualSubObj::GetLightCount(void) const
{
    return m_pParent->GetNumLights();
}

//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* FlyManualSubObj::GetLightObject( UINT nIndex )
{
    return m_pParent->GetLightObject( nIndex );
}

//------------------------------------------------------------------------
// Get the vertex type of the manual sub object.
//------------------------------------------------------------------------
VERTEXTYPE FlyManualSubObj::GetVertexType(void) const
{
    return m_VertexType;
}

//------------------------------------------------------------------------
// Set the vertex type for the manual sub object.
//------------------------------------------------------------------------
void FlyManualSubObj::SetVertexType( VERTEXTYPE vertexType )
{
    m_VertexType = vertexType;

    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();
    m_nVertexSize = pRenderer->GetVertexSize( vertexType );
}

//------------------------------------------------------------------------
// Get the pointer to the vertex buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyManualSubObj::GetVertexBuffer(void)
{
    return m_pVB;
}

//------------------------------------------------------------------------
// Get the pointer to the index buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyManualSubObj::GetIndexBuffer(void)
{
    return m_pIB;
}

//------------------------------------------------------------------------
// Get the pointer to the  parent entity.
//------------------------------------------------------------------------
FlyManualEntity* FlyManualSubObj::GetParentEntity(void)
{
    return m_pParent;
}


//////////////////////////////////////////////////////////////////////////
// Implementing the class FlyManualEntityFactory.

//------------------------------------------------------------------------
// Create a manual entity instance for the scene.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the manual entity.
//------------------------------------------------------------------------
FlySceneObject* FlyManualEntityFactory::CreateInstance( const char* cName )
{
    FlyManualEntity* pObject = new FlyManualEntity( cName );
    if( !pObject ) return NULL;

    pObject->SetCreator( this );
    return pObject;
}


//------------------------------------------------------------------------
// Destroy a manual entity instance for the scene.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneObject*:    Pointer to the scene object.
//------------------------------------------------------------------------
void FlyManualEntityFactory::DestroyInstance( FlySceneObject* pObject )
{
    SAFE_DELETE( pObject );
}