//------------------------------------------------------------------------
// Name: FlyAnimEntity.cpp
// Desc: This file is to implement the class FlyAnimEntity.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyAnimEntity.h"
#include "FlySceneNode.h"

#include "FlyKernel.h"
#include "FlyAnimManager.h"
#include "FlyTimer.h"


//------------------------------------------------------------------------
// Constructor of the class.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the animation entity.
//------------------------------------------------------------------------
FlyAnimEntity::FlyAnimEntity( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyAnimEntity::~FlyAnimEntity(void)
{
    RenderableList::iterator itr;
    for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
        SAFE_DELETE( (*itr) );

    FlyAnimManager* pAnimMgr = FlyKernel::Instance().GetAnimManager();
    pAnimMgr->DecResourceRef( m_pAnimation->GetName() );
}


//------------------------------------------------------------------------
// Initialize the animation entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyAnimation*:      Pointer to the animation template.
//------------------------------------------------------------------------
HRESULT FlyAnimEntity::Initialize( FlyAnimation* pAnimation )
{
    m_pAnimation = pAnimation;

    // Set the local boundingBox.
    m_LocalBBox = pAnimation->GetBoundingBox();

    m_fFrame = 0.0f;
    m_bAnimPaused = false;
    m_bAnimCompleted = true;
    m_nCurrentAnim = -1;
    m_fAnimFPS = 5.0f;
    m_bAnimLoop = false;
    m_bShowBBox = false;

    // Set the dirty state.
    m_nDirtyState = pAnimation->GetDirtyState();

    // Add the resource reference.
    FlyAnimManager* pAnimMgr = FlyKernel::Instance().GetAnimManager();
    pAnimMgr->AddResourceRef( pAnimation->GetName() );

    // Build the renderable objects.
    return BuildRenderables();
}


//------------------------------------------------------------------------
// Reinitialize the animation entity.
//------------------------------------------------------------------------
HRESULT FlyAnimEntity::Reinitialize(void)
{
    // Update the new dirty state.
    m_nDirtyState = m_pAnimation->GetDirtyState();

    // If user destroy a animation node.
    if( m_nCurrentAnim >= m_pAnimation->GetNumAnimations() )
        m_nCurrentAnim = -1;

    // Set the local BoundingBox.
    m_LocalBBox = m_pAnimation->GetBoundingBox();

    // Destroy all the renderables.
    RenderableList::iterator itr;
    for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
        SAFE_DELETE( (*itr) );
    m_Renderables.clear();

    // Build the renderable objects.
    return BuildRenderables();
}


//------------------------------------------------------------------------
// Build the renderable objects for the entity.
//------------------------------------------------------------------------
HRESULT FlyAnimEntity::BuildRenderables(void)
{
    HRESULT hr;
    FlyAnimRenderable* pObject;

    for( UINT i=0;i<m_pAnimation->GetNumMeshes();i++ )
    {
        pObject = new FlyAnimRenderable( this );
        if( !pObject ) return FLY_OUTOFMEMORY;

        // Get the vertex and index data.
        const AVERTEX* pVerts = m_pAnimation->GetVertexData();
        const sFace* pFaces = m_pAnimation->GetFaceData();

        // Get the mesh data.
        const sMesh* pMesh = m_pAnimation->GetMeshData(i);

        // Create the graphic buffer.
        hr = pObject->CreateGraphicBuffer( pMesh->nNumVerts,pMesh->nNumFaces*3,pMesh->nNumFaces );
        if( FAILED(hr) )
        {
            delete pObject;
            return hr;
        }

        // Write the vertex data.
        pObject->m_pVB->WriteData( 0,pMesh->nNumVerts*sizeof(AVERTEX),
            &pVerts[pMesh->nOffsetVerts],false );

        // Write the index data.
        WORD* pIndis = new WORD[pMesh->nNumFaces*3];
        if( !pIndis )
        {
            delete pObject;
            return FLY_OUTOFMEMORY;
        }

        for( UINT j=0;j<pMesh->nNumFaces;j++ )
        {
            pIndis[j*3+0] = pFaces[pMesh->nOffsetFaces+j].wIndices[0] - pMesh->nOffsetVerts;
            pIndis[j*3+1] = pFaces[pMesh->nOffsetFaces+j].wIndices[1] - pMesh->nOffsetVerts;
            pIndis[j*3+2] = pFaces[pMesh->nOffsetFaces+j].wIndices[2] - pMesh->nOffsetVerts;
        }

        pObject->m_pIB->WriteData( 0,pMesh->nNumFaces*3*sizeof(WORD),pIndis,false );
        delete[] pIndis;

        // Set the render material for the object.
        pObject->SetRenderMaterial( m_pAnimation->GetRenderMaterial(pMesh->nMaterialID) );
        if( pObject->m_pMaterial->GetTexture(0,0) &&
            pObject->m_pMaterial->GetTexture(0,0)->HasAlphaChannel() )
            pObject->m_nPriority = PRIORITY_ALPHA;

        // Calculate the bounding box for submesh.
        FlyVector vMin = FlyVector(  999999.0f, 999999.0f, 999999.0f );
        FlyVector vMax = FlyVector( -999999.0f,-999999.0f,-999999.0f );

        for( UINT j=0;j<pMesh->nNumVerts;j++ )
        {
            const AVERTEX* pVert = &pVerts[pMesh->nOffsetVerts+j];

            if( pVert->x < vMin.x ) vMin.x = pVert->x;
            if( pVert->y < vMin.y ) vMin.y = pVert->y;
            if( pVert->z < vMin.z ) vMin.z = pVert->z;
            if( pVert->x > vMax.x ) vMax.x = pVert->x;
            if( pVert->y > vMax.y ) vMax.y = pVert->y;
            if( pVert->z > vMax.z ) vMax.z = pVert->z;
        }

        pObject->m_BBox.vcMin = vMin;
        pObject->m_BBox.vcMax = vMax;

        // Add the renderable object.
        m_Renderables.push_back( pObject );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Update the animation entity.
//------------------------------------------------------------------------
void FlyAnimEntity::UpdateObject(void)
{
    m_WorldBBox = m_LocalBBox;
    m_WorldBBox.TransformBox( GetWorldTransform() );

    // If the current animation node is null, exit the function.
    if( m_nCurrentAnim == -1 )
        return;

    // Is the animation entity paused ?
    if( m_bAnimPaused ) return;

    // The animation complete ?
    if( m_bAnimCompleted && !m_bAnimLoop )
        return;

    // Get the elapsed time.
    float fElapsed = FlyKernel::Instance().GetTimer()->GetElapsed();

    // Calculate the current frame value.
    m_fFrame += m_fAnimFPS * fElapsed;

    if( m_fFrame >= (float)m_pAnimation->GetNumAnimFrames(m_nCurrentAnim) )
    {
        m_fFrame = 0.0f;

        if( !m_bAnimLoop )
            m_bAnimCompleted = true;
    }
}


//------------------------------------------------------------------------
// Render the animation entity.
//------------------------------------------------------------------------
void FlyAnimEntity::RenderObject(void)
{
    if( m_nDirtyState != m_pAnimation->GetDirtyState() )
        Reinitialize();

    if( !m_bOpenLight )
        m_Lights.clear();
    else
        UpdateLights();

    // Render the entity.
    if( IsVisible() )
    {
        // Update the entity's data.
        UpdateEntity();

        // Get the pointer to the renderer.
        LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

        RenderableList::iterator itr;
        for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
            (*itr)->AddToRenderQueue();
    }
}


//------------------------------------------------------------------------
// Update the geometry data for the entity.
//------------------------------------------------------------------------
void FlyAnimEntity::UpdateEntity(void)
{
    if( m_nCurrentAnim == -1 )
        return;

    if( (m_bAnimCompleted && !m_bAnimLoop)/* ||
        m_bAnimPaused*/ )
        return;

    // Animate the vertices.
    m_pAnimation->GoAnimation( m_nCurrentAnim,m_fFrame );

    // Update the local BBox.
    m_LocalBBox = m_pAnimation->GetBoundingBox();
    m_fBRadius = m_LocalBBox.GetBoundingRadius();

    // Get the animated vertex data.
    const AVERTEX* pVerts = m_pAnimation->GetVertexData();

    // Update the renderable objects.
    for( UINT i=0;i<m_Renderables.size();i++ )
    {
        const sMesh* pMesh = m_pAnimation->GetMeshData( i );

        // Write the data to buffer.
        m_Renderables[i]->WriteVertexData( &pVerts[pMesh->nOffsetVerts],
            pMesh->nNumVerts );
    }
}


//------------------------------------------------------------------------
// Get the local boundingBox of the entity.
//------------------------------------------------------------------------
const FlyAabb& FlyAnimEntity::GetLocalBoundingBox(void) const
{
    return m_LocalBBox;
}


//------------------------------------------------------------------------
// Pause the animation.
//------------------------------------------------------------------------
void FlyAnimEntity::Pause(void)
{
    m_bAnimPaused = true;
}

//------------------------------------------------------------------------
// Play the animation.
//------------------------------------------------------------------------
void FlyAnimEntity::Play(void)
{
    m_bAnimPaused = false;
}


//------------------------------------------------------------------------
// Set the current frame for the animation entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Current frame value.
//------------------------------------------------------------------------
void FlyAnimEntity::SetCurrentFrame( float fFrame )
{
    m_fFrame = fFrame;
}


//------------------------------------------------------------------------
// Set the current animation for the entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:      Id of the animation.
//------------------------------------------------------------------------
void FlyAnimEntity::SetAnimation( UINT nAnimID )
{
    if( nAnimID >= m_pAnimation->GetNumAnimations() )
        m_nCurrentAnim = -1;
    else
        m_nCurrentAnim = nAnimID;
}


//------------------------------------------------------------------------
// Set the loop mode for the entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      bool:       Play the animation with loop ?
//------------------------------------------------------------------------
void FlyAnimEntity::SetAnimLoop( bool bLoop )
{
    m_bAnimLoop = bLoop;
}


//------------------------------------------------------------------------
// Set the FPS for the entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      FPS when playing the animation.
//------------------------------------------------------------------------
void FlyAnimEntity::SetAnimFPS( float fAnimFPS )
{
    m_fAnimFPS = fAnimFPS;
}


//------------------------------------------------------------------------
// Show the boundingBox ?
//------------------------------------------------------------------------
void FlyAnimEntity::ShowBoundingBox( bool bShowBBox )
{
    m_bShowBBox = bShowBBox;
}


//------------------------------------------------------------------------
// Get the current animation ID.
//------------------------------------------------------------------------
int FlyAnimEntity::GetCurrentAnimation(void) const
{
    return m_nCurrentAnim;
}


//------------------------------------------------------------------------
// Get the current frame value.
//------------------------------------------------------------------------
float FlyAnimEntity::GetCurrentFrame(void) const
{
    return m_fFrame;
}


//------------------------------------------------------------------------
// Play the animation with loop ?
//------------------------------------------------------------------------
bool FlyAnimEntity::IsAnimationLoop(void) const
{
    return m_bAnimLoop;
}


//------------------------------------------------------------------------
// Get the animation's FPS.
//------------------------------------------------------------------------
float FlyAnimEntity::GetAnimFPS(void) const
{
    return m_fAnimFPS;
}


//------------------------------------------------------------------------
// Is the animation completed ?
//------------------------------------------------------------------------
bool FlyAnimEntity::IsAnimationCompleted(void) const
{
    return m_bAnimCompleted;
}


//------------------------------------------------------------------------
// Get the animation template.
//------------------------------------------------------------------------
FlyAnimation* FlyAnimEntity::GetAnimation(void)
{
    return m_pAnimation;
}


/*************************************************************************
*                                                                        *
*      IMPLEMENTING THE CLASS FLYANIMRENDERABLE                          *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyAnimRenderable::FlyAnimRenderable( FlyAnimEntity* pParent )
{
    m_pParent = pParent;

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
FlyAnimRenderable::~FlyAnimRenderable(void)
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );
}


//------------------------------------------------------------------------
// Create the graphic buffer for the object.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Count of the vertex element.
//      UINT:       Count of the index element.
//      UINT:       Count of the primitives.
//------------------------------------------------------------------------
HRESULT FlyAnimRenderable::CreateGraphicBuffer( UINT nNumVerts,UINT nNumIndis,UINT nNumPrims )
{
    // Clean the old data.
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );

    // Get the resource manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    m_nNumVerts = nNumVerts;
    m_nNumIndis = nNumIndis;
    m_nNumPrims = nNumPrims;

    // Create the vertex buffer.
    m_pVB = pResMgr->MakeVertexBuffer( nNumVerts*sizeof(AVERTEX),BU_WRITEONLY,MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    // Create the index buffer.
    m_pIB = pResMgr->MakeIndexBuffer( nNumIndis*sizeof(WORD),BU_WRITEONLY,MM_MANAGED,IF_INDEX16 );
    if( !m_pIB ) return FLY_CREATEBUFFER;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Write the vertex data to buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      const AVERTEX*:     Pointer to the vertex data.
//      UINT:               Count of the vertex.
//------------------------------------------------------------------------
void FlyAnimRenderable::WriteVertexData( const AVERTEX* pVerts,UINT nCount )
{
    void* pData = m_pVB->Lock( 0,nCount*sizeof(AVERTEX),LOCK_NORMAL );
    if( !pData ) return;

    memcpy( pData,pVerts,sizeof(AVERTEX)*nCount );
    m_pVB->Unlock();
}


//------------------------------------------------------------------------
// Render the anim renderable object.
//------------------------------------------------------------------------
void FlyAnimRenderable::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( VF_ANIMATION,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
}


//------------------------------------------------------------------------
// Get the count of the vertex.
//------------------------------------------------------------------------
UINT FlyAnimRenderable::GetNumVerts(void) const
{
    return m_nNumVerts;
}


//------------------------------------------------------------------------
// Get the count of index.
//------------------------------------------------------------------------
UINT FlyAnimRenderable::GetNumIndis(void) const
{
    return m_nNumIndis;
}


//------------------------------------------------------------------------
// Get the count of primitive.
//------------------------------------------------------------------------
UINT FlyAnimRenderable::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the local boundingBox.
//------------------------------------------------------------------------
const FlyAabb& FlyAnimRenderable::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the distance to the camera.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float FlyAnimRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter() * m_pParent->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}


//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void FlyAnimRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    assert( pMatrices );

    *pMatrices = m_pParent->GetWorldTransform();
}


//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT FlyAnimRenderable::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get the count of the light objects.
//------------------------------------------------------------------------
UINT FlyAnimRenderable::GetLightCount(void) const
{
    return m_pParent->GetNumLights();
}


//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* FlyAnimRenderable::GetLightObject( UINT nIndex )
{
    return m_pParent->GetLightObject( nIndex );
}


//------------------------------------------------------------------------
// Get the pointer to the vertex buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyAnimRenderable::GetVertexBuffer(void)
{
    return m_pVB;
}


//------------------------------------------------------------------------
// Get the pointer to the index buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyAnimRenderable::GetIndexBuffer(void)
{
    return m_pIB;
}


//------------------------------------------------------------------------
// Get the pointer to the animation entity.
//------------------------------------------------------------------------
FlyAnimEntity* FlyAnimRenderable::GetParentEntity(void)
{
    return m_pParent;
}


//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Create an instance for the animation entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the animation entity.
//------------------------------------------------------------------------
FlySceneObject* FlyAnimEntityFactory::CreateInstance( const char* cName )
{
    FlyAnimEntity* pEntity = new FlyAnimEntity( cName );
    if( !pEntity ) return NULL;

    pEntity->SetCreator( this );
    return pEntity;
}


//------------------------------------------------------------------------
// Destroy the specified animation entity instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneObject*:    Pointer to the animation entity instance.
//------------------------------------------------------------------------
void FlyAnimEntityFactory::DestroyInstance( FlySceneObject* pObject )
{
    SAFE_DELETE( pObject );
}