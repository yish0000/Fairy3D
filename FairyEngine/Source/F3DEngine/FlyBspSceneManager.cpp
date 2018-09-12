//------------------------------------------------------------------------
// Name: FlyBspSceneManager.cpp
// Desc: This file is to implement the class FlyBspSceneManager.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyBspSceneManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyBspSceneManager::FlyBspSceneManager(void)
{
    m_pLevel = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyBspSceneManager::~FlyBspSceneManager(void)
{
    RenderableList::iterator itr;
    for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
        SAFE_DELETE( *itr );

    SAFE_DELETE( m_pLevel );
}


//------------------------------------------------------------------------
// Load the world geometry data from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the bsp file.
//------------------------------------------------------------------------
HRESULT FlyBspSceneManager::LoadWorldGeometry( const char* cFilename )
{
    HRESULT hr;

    // Delete the old data.
    SAFE_DELETE( m_pLevel );

    RenderableList::iterator itr;
    for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
        SAFE_DELETE( *itr );

    // Create the new bsp level.
    m_pLevel = new FlyBspLevel();
    hr = m_pLevel->LoadQuake3Level( cFilename );
    if( FAILED(hr) )
    {
        SAFE_DELETE( m_pLevel );
        return hr;
    }

    // Build the renderable objects.
    return BuildRenderables();
}


//------------------------------------------------------------------------
// Build the renderable objects for the bsp level.
//------------------------------------------------------------------------
HRESULT FlyBspSceneManager::BuildRenderables(void)
{
    int nMatID = 0;
    char cMatName[80];

    for( UINT i=0;i<m_pLevel->GetNumFaces();i++ )
    {
        sBspFace* pFace = m_pLevel->GetBspFace( i );

        // Set the material name.
        sprintf( cMatName,"bsp_%d_%d",pFace->nTextureID,pFace->nLightID );

        // Check whether the renderable is created ?
        RenderableList::iterator itr;
        for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
        {
            if( strcmp((*itr)->GetRenderMaterial()->GetName(),cMatName) == 0 )
                break;
            else
                nMatID++;
        }

        if( itr != m_Renderables.end() )
        {
            UINT nMaxNumVerts = (*itr)->GetMaxNumVerts();
            UINT nMaxNumIndis = (*itr)->GetMaxNumIndis();

            nMaxNumVerts += pFace->nNumVerts;
            nMaxNumIndis += pFace->nNumIndis;

            (*itr)->SetMaxNumVerts( nMaxNumVerts );
            (*itr)->SetMaxNumIndis( nMaxNumIndis );

            pFace->nMatID = nMatID;
        }
        else
        {
            // Create a new renderable object.
            FlyBspRenderable* pRenderable = new FlyBspRenderable( cMatName );
            if( !pRenderable ) return FLY_OUTOFMEMORY;

            pRenderable->SetMaxNumVerts( pFace->nNumVerts );
            pRenderable->SetMaxNumIndis( pFace->nNumIndis );

            pRenderable->SetTexture( m_pLevel->GetTexture(pFace->nTextureID) );
            pRenderable->SetLightmap( m_pLevel->GetLightmap(pFace->nLightID) );

            // Add the new object to list.
            m_Renderables.push_back( pRenderable );

            pFace->nMatID = (int)m_Renderables.size() - 1;
        }
    }

    // Create the graphic buffers.
    RenderableList::iterator obj;
    for( obj=m_Renderables.begin();obj!=m_Renderables.end();obj++ )
    {
        HRESULT hr;
        hr = (*obj)->CreateGraphicBuffer();
        if( FAILED(hr) ) return hr;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Render the world geometry for the engine.
//------------------------------------------------------------------------
void FlyBspSceneManager::RenderWorldLevel(void)
{
    // Get the camera's position.
    FlyVector vCamPos = m_pCamera->GetPosition();

    // Get the leaf which the camera is in.
    FlyBspNode* pCamLeaf = m_pLevel->FindLeaf( vCamPos );

    // Clear the data of last frame.
    RenderableList::iterator obj;
    for( obj=m_Renderables.begin();obj!=m_Renderables.end();obj++ )
        (*obj)->ClearData();

    // Search the visible world geometry from the
    // root node.
    ProcessNode( m_pLevel->GetRootNode(),pCamLeaf );

    // Add the renderables to renderQueue.
    for( obj=m_Renderables.begin();obj!=m_Renderables.end();obj++ )
    {
        if( (*obj)->GetNumPrims() > 0 )
            (*obj)->AddToRenderQueue();
    }
}


//------------------------------------------------------------------------
// Clear the scene data, free all the memory.
//------------------------------------------------------------------------
void FlyBspSceneManager::ClearScene(void)
{
    // Clear the world geometry.
    RenderableList::iterator itr;
    for( itr=m_Renderables.begin();itr!=m_Renderables.end();itr++ )
        SAFE_DELETE( *itr );

    m_Renderables.clear();
    SAFE_DELETE( m_pLevel );

    FlySceneManager::ClearScene();
}


//------------------------------------------------------------------------
// Process the node, search the visible world geometry.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyBspNode*:    Current node to process.
//      FlyBspNode*:    Leaf which the camera is in.
//------------------------------------------------------------------------
void FlyBspSceneManager::ProcessNode( FlyBspNode* pNode,FlyBspNode* pCamLeaf )
{
    if( !m_Frustum.IsBBoxVisible(pNode->GetBoundingBox()) )
        return;

    if( pNode->IsLeaf() )
    {
        if( !pCamLeaf->IsLeafVisible(pNode) )
            return;

        // Handle all the faces.
        for( int i=0;i<pNode->GetNumFaceGroup();i++ )
        {
            int nFaceID = m_pLevel->m_pLeafFaces[pNode->m_nFaceStart+i];

            HandleSingleFace( &m_pLevel->m_pFaces[nFaceID] );
        }
    }
    else
    {
        ProcessNode( pNode->m_pBack,pCamLeaf );
        ProcessNode( pNode->m_pFront,pCamLeaf );
    }
}


//------------------------------------------------------------------------
// Handle the single face, send the geometry data to buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      sBspFace*:      Pointer to the bsp face.
//------------------------------------------------------------------------
void FlyBspSceneManager::HandleSingleFace( sBspFace* pFace )
{
    m_Renderables[pFace->nMatID]->AddFaceData( pFace->nNumVerts,pFace->nNumIndis,
        pFace->pVerts,pFace->pIndis,pFace->BBox );
}


//------------------------------------------------------------------------
// Test the collision in the 3D world.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyRay&:  Ray used to test the collision.
//      float:          Length of the ray.
//      float&:         To store the nearest intersection point.
//------------------------------------------------------------------------
bool FlyBspSceneManager::TestCollision( const FlyRay& ray,float fL,float& fDist )
{
    return TestNode( m_pLevel->GetRootNode(),ray,fL,fDist );
}


//------------------------------------------------------------------------
// Test the ray's collision with the node.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyBspNode*:        Pointer to the current node.
//      const FlyRay&:      Ray to test the collision.
//      float:              Length of the ray.
//      float&:             To store the min distance to the inter-point.
//------------------------------------------------------------------------
bool FlyBspSceneManager::TestNode( FlyBspNode* pNode,const FlyRay& ray,float fL,
                                   float& fDist )
{
    int nSide;
    FlyRay rayFront,rayBack;
    FlyPlane splitter;
    float fRatio;

    // 1. This is a leaf.
    if( pNode->IsLeaf() )
    {
        return TestLeaf( pNode,ray,fL,fDist );
    }

    // 2. This is not a leaf.
    splitter = pNode->GetSplitter();
    nSide = splitter.Classify( ray.m_vcOrig );

    if( splitter.Clip(&ray,fL,&rayFront,&rayBack,&fRatio) )
    {
        if( nSide == FLY_BACK )
        {
            if( TestNode(pNode->GetBack(),rayBack,fRatio,fDist) )
                return true;

            fDist += fRatio;
            return TestNode( pNode->GetFront(),rayFront,fL-fRatio,fDist );
        }
        else
        {
            if( TestNode(pNode->GetFront(),rayFront,fRatio,fDist) )
                return true;

            fDist += fRatio;
            return TestNode( pNode->GetBack(),rayBack,fL-fRatio,fDist );
        }
    }
    else
    {
        if( nSide == FLY_BACK )
        {
            return TestNode( pNode->GetBack(),ray,fL,fDist );
        }
        else
        {
            return TestNode( pNode->GetFront(),ray,fL,fDist );
        }
    }

    return true;
}


//------------------------------------------------------------------------
// Test the ray's collision with the leaf.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyBspNode*:        Pointer to the leaf.
//      const FlyRay&:      Ray to test the collision.
//      float:              Length of the ray.
//      float&:             To store the min distance of the inter-point.
//------------------------------------------------------------------------
bool FlyBspSceneManager::TestLeaf( FlyBspNode* pLeaf,const FlyRay& ray,float fL,
                                  float& fDist )
{
    bool bInserted = false;
    std::pair<bool,float> res;
    const NodeBrushList& brushList = pLeaf->GetSolidBrushes();
    float fMin = 999999.0f;

    NodeBrushList::const_iterator itr;
    for( itr=brushList.begin();itr!=brushList.end();itr++ )
    {
        sBspBrush* pBrush = *itr;

        res = m_pLevel->IntersectBrush( pBrush,ray,fL );
        if( res.first )
        {
            bInserted = true;

            fMin = min( fMin,res.second );
        }
    }

    if( bInserted ) fDist += fMin;

    return bInserted;
}


//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Create a bsp sceneManager.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the scene Manager.
//------------------------------------------------------------------------
FlySceneManager* FlyBspSceneFactory::CreateInstance( const char* cName )
{
    FlyBspSceneManager* pManager = new FlyBspSceneManager();
    if( !pManager ) return NULL;

    pManager->SetName( cName );
    pManager->SetCreator( this );
    return pManager;
}


//------------------------------------------------------------------------
// Destroy a sepecified sceneManager instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneManager*:   Pointer to the sceneManager.
//------------------------------------------------------------------------
void FlyBspSceneFactory::DestroyInstance( FlySceneManager* pManager )
{
    SAFE_DELETE( pManager );
}