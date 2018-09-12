//------------------------------------------------------------------------
// Name: etSpriteObject.cpp
// Desc: This file is to implement the class etSpriteObject.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etSpriteObject.h"

#include "etCoreManager.h"
#include "etSceneManager.h"

#include "FlySceneNode.h"
#include "FlyCameraBase.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etSpriteObject::etSpriteObject( const char* cName )
{
    m_sName = cName;

    m_pRenderable = NULL;
    m_bBillboardY = false;
    m_fFPS = 5.0f;
    m_nCurrentFrame = -1;
    m_fTime = 0.0f;
    m_fLastTime = 0.0f;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etSpriteObject::~etSpriteObject(void)
{
    SAFE_DELETE( m_pRenderable );

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Destroy the textures.
    TextureList::iterator tex;
    for( tex=m_Textures.begin();tex!=m_Textures.end();tex++ )
        pResMgr->DeleteTexture( *tex );
}


//------------------------------------------------------------------------
// Initialize the sprite object for the editor.
//------------------------------------------------------------------------
HRESULT etSpriteObject::Initialize( float fWidth,float fHeight,
                                    const std::vector<std::string>& Textures )
{
    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    for( int n=0;n<Textures.size();n++ )
    {
        FlyTexture* pTexture = pResMgr->Load2DTexture( Textures[n].c_str(),0 );
        m_Textures.push_back( pTexture );
    }

    return BuildRenderable( fWidth,fHeight );
}


//------------------------------------------------------------------------
// Build the renderable object for the sprite.
//------------------------------------------------------------------------
HRESULT etSpriteObject::BuildRenderable( float fWidth,float fHeight )
{
    m_pRenderable = new etSpriteRenderable( this );
    if( !m_pRenderable ) return FLY_OUTOFMEMORY;

    return m_pRenderable->BuildGeometry( fWidth,fHeight );
}


//------------------------------------------------------------------------
// Pause the animation of the sprite object.
//------------------------------------------------------------------------
void etSpriteObject::Pause(void)
{
    m_bAnimPaused = true;
}

//------------------------------------------------------------------------
// Play the animation of the sprite object.
//------------------------------------------------------------------------
void etSpriteObject::Play(void)
{
    m_bAnimPaused = false;
}


//------------------------------------------------------------------------
// Update the sprite object.
//------------------------------------------------------------------------
void etSpriteObject::UpdateObject(void)
{
    if( (m_bAnimComplete && !m_bLoop) ||
        m_bAnimPaused )
        return;

    // Get the elapsed time.
    float fElapsed = etCoreManager::Instance().GetElapsed();
    m_fTime += fElapsed;

    // Update the new frame.
    if( m_fTime - m_fLastTime >= 1 / m_fFPS )
    {
        if( m_nCurrentFrame == -1 )
            m_nCurrentFrame = 0;
        else if( m_nCurrentFrame >= m_Textures.size()-1 )
        {
            if( m_bLoop )
                m_nCurrentFrame = 0;
            else
            {
                m_nCurrentFrame = -1;
                m_bAnimComplete = true;
            }
        }
        else m_nCurrentFrame++;

        m_fLastTime = m_fTime;
    }

    if( !m_pParent )
        m_mWorldTM = FlyMatrix::IDENTITY;
    else
    {
        const FlyVector& vTrans = m_pParent->GetDerivedPosition();
        const FlyVector& vScale = m_pParent->GetDerivedScale();
        m_mWorldTM.MakeTransform( vTrans,FlyQuat::IDENTITY,vScale );

        FlyCameraBase* pCamera = etCoreManager::Instance().GetSceneManager()->GetCurrentCamera();
        m_mWorldTM = pCamera->GetBillboardMatrix( m_bBillboardY ) * m_mWorldTM;
    }

    // Update the world boundingBox.
    m_WorldBBox = GetLocalBoundingBox();
    m_WorldBBox.TransformBox( m_mWorldTM );
}


//------------------------------------------------------------------------
// Render the sprite object.
//------------------------------------------------------------------------
void etSpriteObject::RenderObject(void)
{
    if( m_nCurrentFrame == -1 )
        return;

    m_pRenderable->GetRenderMaterial()->SetTexture( 0,0,m_Textures[m_nCurrentFrame] );
    m_pRenderable->AddToRenderQueue();
}


//------------------------------------------------------------------------
// Get the local boundingBox of the entity.
//------------------------------------------------------------------------
const FlyAabb& etSpriteObject::GetLocalBoundingBox(void) const
{
    return m_pRenderable->GetBoundingBox();
}


//------------------------------------------------------------------------
// Get the world matrix of the sprite object.
//------------------------------------------------------------------------
const FlyMatrix& etSpriteObject::GetWorldTransform(void) const
{
    return m_mWorldTM;
}


//------------------------------------------------------------------------
// Get the specified texture.
//------------------------------------------------------------------------
FlyTexture* etSpriteObject::GetTexture( UINT nIndex )
{
    if( nIndex >= m_Textures.size() )
        return NULL;

    return m_Textures[nIndex];
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS ETSPRITERENDERABLE.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etSpriteRenderable::etSpriteRenderable( etSpriteObject* pParent )
{
    m_pParent = pParent;
    m_nPriority = PRIORITY_ALPHA;

    m_pVB = NULL;
    memset( &m_Bounds,0,sizeof(FlyAabb) );
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etSpriteRenderable::~etSpriteRenderable(void)
{
    SAFE_DELETE( m_pVB );
}


//------------------------------------------------------------------------
// Build the geometry data for the sprite.
//------------------------------------------------------------------------
HRESULT etSpriteRenderable::BuildGeometry( float fWidth,float fHeight )
{
    VERTEXL* pVerts;
    std::string sMatName;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    m_pVB = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*4,BU_WRITEONLY,
        MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    pVerts = (VERTEXL*)m_pVB->Lock( LOCK_NORMAL );
    pVerts->x = -fWidth * 0.5f;
    pVerts->y = -fHeight * 0.5f;
    pVerts->z = 0.0f;
    pVerts->color = 0xffffffff;
    pVerts->tu = 0.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = -fWidth * 0.5f;
    pVerts->y = fHeight * 0.5f;
    pVerts->z = 0.0f;
    pVerts->color = 0xffffffff;
    pVerts->tu = 0.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x = fWidth * 0.5f;
    pVerts->y = -fHeight * 0.5f;
    pVerts->z = 0.0f;
    pVerts->color = 0xffffffff;
    pVerts->tu = 1.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = fWidth * 0.5f;
    pVerts->y = fHeight * 0.5f;
    pVerts->z = 0.0f;
    pVerts->color = 0xffffffff;
    pVerts->tu = 1.0f;
    pVerts->tv = 0.0f;
    m_pVB->Unlock();

    // Set the local boundingBox.
    m_Bounds.vcMin.Set( -fWidth*0.5f,-fHeight*0.5f,-1.0f );
    m_Bounds.vcMax.Set(  fWidth*0.5f, fHeight*0.5f, 1.0f );

    // Create the render material.
    sMatName = m_pParent->GetName();
    sMatName += "_material";
    m_pMaterial = pResMgr->CreateMaterial( sMatName.c_str() );
    m_pMaterial->SetSceneAlphaMode( 0,true,false );
    m_pMaterial->SetAlphaBlendMode( 0,SBM_SRCALPHA,SBM_INVSRCALPHA );
    m_pMaterial->SetDepthMode( 0,DEPTH_READONLY );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Render the sprite object for the editor.
//------------------------------------------------------------------------
void etSpriteRenderable::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = etCoreManager::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( RT_TRIANGLESTRIP );
    pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,0,4,m_pVB );
}


//------------------------------------------------------------------------
// Get the local boundingBox of the sprite.
//------------------------------------------------------------------------
const FlyAabb& etSpriteRenderable::GetBoundingBox(void) const
{
    return m_Bounds;
}

//------------------------------------------------------------------------
// Get the square distance to the camera.
//------------------------------------------------------------------------
float etSpriteRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_pParent->GetLocalBoundingBox().GetCenter() *
        m_pParent->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}

//------------------------------------------------------------------------
// Get the world matrices of the sprite object.
//------------------------------------------------------------------------
void etSpriteRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pParent->GetWorldTransform();
}

//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT etSpriteRenderable::GetNumWorldMatrices(void) const
{
    return 1;
}

//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT etSpriteRenderable::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
//------------------------------------------------------------------------
FlyLightObject* etSpriteRenderable::GetLightObject( UINT nIndex )
{
    return NULL;
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS ETSPRITEFACTORY.

//------------------------------------------------------------------------
// Create an instance of the sprite object.
//------------------------------------------------------------------------
FlySceneObject* etSpriteFactory::CreateInstance( const char* cName )
{
    FlySceneObject* pObject = new etSpriteObject( cName );
    if( !pObject ) return NULL;

    pObject->SetCreator( this );
    return pObject;
}

//------------------------------------------------------------------------
// Destroy the specified instance of sprite object.
//------------------------------------------------------------------------
void etSpriteFactory::DestroyInstance( FlySceneObject* pObject )
{
    SAFE_DELETE( pObject );
}