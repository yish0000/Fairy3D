//------------------------------------------------------------------------
// Name: etLiquidObject.cpp
// Desc: This file is to implement the class etLiquidObject.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etLiquidObject.h"

#include "etCoreManager.h"
#include "etSceneManager.h"

#include "FlyCamManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etLiquidObject::etLiquidObject( const char* cName )
{
    m_sName = cName;
    m_vPosition.Set( 0.0f,0.0f,0.0f );
    m_fWidth = 0.0f;
    m_fHeight = 0.0f;

    m_pSurface = NULL;
    m_fTexScale = 1.0f;
    m_fFlowSpeed = 1.0f;
    m_pDistort = NULL;
    m_fDistortion = 8.0f;

    m_bReflection = false;
    m_bReflectMesh = false;
    m_bReflectSky = false;
    m_bReflectTerrain = false;
    m_pReflect = NULL;

    m_pRenderable = NULL;
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etLiquidObject::~etLiquidObject(void)
{
    // Delete the surface texture.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    if( m_pSurface ) pResMgr->DeleteTexture( m_pSurface );
    if( m_pDistort ) pResMgr->DeleteTexture( m_pDistort );

    SAFE_DELETE( m_pReflect );
    SAFE_DELETE( m_pRenderable );
}


//------------------------------------------------------------------------
// Initialize the liquid object.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the surface texture.
//      const char*:        Name of the distort texture.
//      const FlyVector&:   Center of the liquid.
//      float:              Width of the liquid object.
//      float:              Height of the liquid object.
//------------------------------------------------------------------------
HRESULT etLiquidObject::Initialize( const char* cSurfaceMap,const char* cDistortMap,
                                    const FlyVector& vCenter,float fWidth,float fHeight )
{
    std::string sName;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    m_pSurface = pResMgr->Load2DTexture( cSurfaceMap,0 );
    m_pDistort = pResMgr->Load2DTexture( cDistortMap,0 );

    // Get the sceneManager.
    etSceneManager* pManager = etCoreManager::Instance().GetSceneManager();

    // Build the reflect target.
    sName = m_sName + "_reflect";
    m_pReflect = new etLiquidTarget( this );
    m_pReflect->BuildRenderTarget( sName.c_str(),256,256 );

    // Add the texture target to the sceneManager.
    pManager->AddRenderTarget( m_pReflect );

    m_vPosition = vCenter;
    m_fWidth = fWidth;
    m_fHeight = fHeight;

    // Create the renderable object.
    m_pRenderable = new etLiquidRenderable( this );
    return m_pRenderable->Initialize( vCenter,fWidth,fHeight );
}


//------------------------------------------------------------------------
// Update the liquid object.
//------------------------------------------------------------------------
void etLiquidObject::UpdateObject(void)
{
    m_WorldBBox = m_pRenderable->GetBoundingBox();
    m_WorldBBox.TransformBox( GetWorldTransform() );
}


//------------------------------------------------------------------------
// Render the liquid object.
//------------------------------------------------------------------------
void etLiquidObject::RenderObject(void)
{
    // Enable the reflection render target.
    if( m_bReflection ) m_pReflect->SetActive( true );

    // Update the render material.
    FlyRenderMaterial* pMaterial = m_pRenderable->GetRenderMaterial();
    FlyRenderEffectInstance* pInstance = pMaterial->GetActiveRenderEffectInstance();

    static float fOffset = 0.0f;
    fOffset += m_fFlowSpeed * etCoreManager::Instance().GetElapsed() * 0.1f;

    FlyCameraBase* pCamera = etCoreManager::Instance().GetSceneManager()->GetCurrentCamera();
    *pInstance->GetParameterByIndex(2) = fOffset;
    *pInstance->GetParameterByIndex(7) = pCamera->GetPosition();

    // Add the renderable object to the renderQueue.
    m_pRenderable->AddToRenderQueue();
}


//------------------------------------------------------------------------
// Get the local boundingBox of the liquid.
//------------------------------------------------------------------------
const FlyAabb& etLiquidObject::GetLocalBoundingBox(void) const
{
    return m_pRenderable->GetBoundingBox();
}

//------------------------------------------------------------------------
// Get the reflection map of the liquid.
//------------------------------------------------------------------------
const FlyTexture* etLiquidObject::GetReflectionMap(void) const
{
    return m_pReflect->GetTextureTarget();
}


//------------------------------------------------------------------------
// Enable the reflection mode.
//------------------------------------------------------------------------
void etLiquidObject::SetReflection( bool bReflection )
{
    FlyRenderMaterial* pMaterial = m_pRenderable->GetRenderMaterial();
    FlyRenderEffectInstance* pInstance = pMaterial->GetActiveRenderEffectInstance();
    *pInstance->GetParameterByIndex(0) = bReflection;
    m_bReflection = bReflection;
}

//------------------------------------------------------------------------
// Set the distort map for the liquid.
//------------------------------------------------------------------------
void etLiquidObject::SetDistortMap( const char* cDistortMap )
{
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    if( m_pDistort ) pResMgr->DeleteTexture( m_pDistort );
    m_pDistort = pResMgr->Load2DTexture( cDistortMap,0 );

    FlyRenderMaterial* pMaterial = m_pRenderable->GetRenderMaterial();
    FlyRenderEffectInstance* pInstance = pMaterial->GetActiveRenderEffectInstance();
    *pInstance->GetParameterByIndex(6) = m_pDistort;
}

//------------------------------------------------------------------------
// Set the surface map for the liquid.
//------------------------------------------------------------------------
void etLiquidObject::SetSurfaceMap( const char* cSurfaceMap )
{
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    if( m_pSurface ) pResMgr->DeleteTexture( m_pSurface );
    m_pSurface = pResMgr->Load2DTexture( cSurfaceMap,0 );

    FlyRenderMaterial* pMaterial = m_pRenderable->GetRenderMaterial();
    FlyRenderEffectInstance* pInstance = pMaterial->GetActiveRenderEffectInstance();
    *pInstance->GetParameterByIndex(4) = m_pSurface;
}

//------------------------------------------------------------------------
// Set the texture scale.
//------------------------------------------------------------------------
void etLiquidObject::SetTexScale( float fTexScale )
{
    m_fTexScale = fTexScale;

    FlyRenderMaterial* pMaterial = m_pRenderable->GetRenderMaterial();
    FlyRenderEffectInstance* pInstance = pMaterial->GetActiveRenderEffectInstance();
    *pInstance->GetParameterByIndex(1) = fTexScale;
}

//------------------------------------------------------------------------
// Set the distortion value.
//------------------------------------------------------------------------
void etLiquidObject::SetDistortion( float fDistortion )
{
    m_fDistortion = fDistortion;

    FlyRenderMaterial* pMaterial = m_pRenderable->GetRenderMaterial();
    FlyRenderEffectInstance* pInstance = pMaterial->GetActiveRenderEffectInstance();
    *pInstance->GetParameterByIndex(3) = fDistortion * 0.1f;
}


//------------------------------------------------------------------------
// Set the center position of the liquid object.
//------------------------------------------------------------------------
void etLiquidObject::UpdateGeometry( const float* pPosition,float fWidth,
                                     float fHeight )
{
    memcpy( &m_vPosition,pPosition,sizeof(float)*3 );
    m_fWidth = fWidth;
    m_fHeight = fHeight;
    m_pRenderable->BuildVertexData( m_vPosition,fWidth,fHeight );
}

//------------------------------------------------------------------------
// Enable reflection of the sky.
//------------------------------------------------------------------------
void etLiquidObject::SetReflectSky( bool bReflect )
{
    m_bReflectSky = bReflect;
    m_pReflect->ShowSkybox( bReflect );
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS ETLIQUIDTARGET.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etLiquidTarget::etLiquidTarget( etLiquidObject* pParent )
{
    m_pParent = pParent;

    etSceneManager* pManager = etCoreManager::Instance().GetSceneManager();
    m_pUtil = pManager->GetSceneNode( "et-Util" );
    m_pTerrain = pManager->GetSceneNode( "et-Terrain" );
    m_pEntity = pManager->GetSceneNode( "et-Entity" );

    // Create a camera for the target.
    FlyCamManager* pCamMgr = FlyKernel::Instance().GetCamManager();
    m_pCamera = pCamMgr->CreateCamera( "water-Reflection" );

    // Add a clip plane.
    FlyPlane plane;
    plane.Set( FlyVector(0.0f,1.0f,0.0f),0.0f );
    m_ClipPlanes.push_back( plane );
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etLiquidTarget::~etLiquidTarget(void)
{
    // Destroy the camera.
    FlyCamManager* pCamMgr = FlyKernel::Instance().GetCamManager();
    pCamMgr->DestroyCamera( m_pCamera );
}


//------------------------------------------------------------------------
// Begin to update the render target.
//------------------------------------------------------------------------
void etLiquidTarget::OnUpdateBegin(void)
{
    FlyVector vPos;
    FlyVector vDirection;

    FlySceneManager* pManager = etCoreManager::Instance().GetSceneManager();
    FlyCameraBase* pCamera = pManager->GetCurrentCamera();

    vPos = pCamera->GetPosition();
    vPos.y = -vPos.y;
    m_pCamera->SetPosition( vPos );

    vDirection = pCamera->GetDirection();
    vDirection.y = -vDirection.y;
    vDirection = vPos + vDirection;
    m_pCamera->LookAt( vDirection );

    if( !m_pParent->IsReflectTerrain() )
        m_pTerrain->SetVisible( false );
    if( !m_pParent->IsReflectMesh() )
        m_pEntity->SetVisible( false );

    m_pUtil->SetVisible( false );
    m_pParent->SetVisible( false );
}

//------------------------------------------------------------------------
// 结束更新渲染目标。
//------------------------------------------------------------------------
void etLiquidTarget::OnUpdateEnd(void)
{
    if( !m_pParent->IsReflectTerrain() )
        m_pTerrain->SetVisible( true );
    if( !m_pParent->IsReflectMesh() )
        m_pEntity->SetVisible( true );

    m_pUtil->SetVisible( true );
    m_pParent->SetVisible( true );
    m_bActive = false;
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS ETLIQUIDRENDERABLE.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etLiquidRenderable::etLiquidRenderable( etLiquidObject* pParent )
{
    m_pParent = pParent;
    m_pVB = NULL;
    memset( &m_Bounds,0,sizeof(FlyAabb) );

    m_nPriority = PRIORITY_ALPHA;
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etLiquidRenderable::~etLiquidRenderable(void)
{
    SAFE_DELETE( m_pVB );
}


//------------------------------------------------------------------------
// Build the vertex data for the object.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Center of the liquid.
//      float:              Width of the water.
//      float:              Height of the water.
//------------------------------------------------------------------------
HRESULT etLiquidRenderable::Initialize( const FlyVector& vCenter,float fWidth,
                                        float fHeight )
{
    std::string sMatName;

    // Build the vertex buffer.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    m_pVB = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*4,BU_WRITEONLY,MM_MANAGED );

    // Build the vertex data.
    BuildVertexData( vCenter,fWidth,fHeight );

    // Create the material.
    sMatName = m_pParent->GetName();
    sMatName += "_material";
    m_pMaterial = pResMgr->CreateMaterial( sMatName.c_str() );

    FlyRenderEffect* pLiquidFX = etCoreManager::Instance().GetLiquidFX();
    m_pMaterial->AddNewTechnique( pLiquidFX->GetTechniqueByIndex(0) );
    m_pMaterial->SetActiveTechnique( 0 );

    FlyRenderEffectInstance* pInstance;
    pInstance = m_pMaterial->GetRenderEffectInstance(0);
    *pInstance->GetParameterByIndex(0) = m_pParent->IsReflection();
    *pInstance->GetParameterByIndex(1) = m_pParent->GetTexScale();
    *pInstance->GetParameterByIndex(3) = m_pParent->GetDistortion()*0.1f;
    *pInstance->GetParameterByIndex(4) = m_pParent->GetLiquidSurface();
    *pInstance->GetParameterByIndex(5) = m_pParent->GetReflectionMap();
    *pInstance->GetParameterByIndex(6) = m_pParent->GetDistortMap();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the vertex data for the renderable.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Center of the liquid.
//      float:              Width of the liquid.
//      float:              Height of the liquid.
//------------------------------------------------------------------------
void etLiquidRenderable::BuildVertexData( const FlyVector& vCenter,float fWidth,
                                          float fHeight )
{
    float fW2 = (float)fWidth / 2.0f;
    float fH2 = (float)fHeight / 2.0f;

    VERTEXL* pVerts;
    pVerts = (VERTEXL*)m_pVB->Lock( LOCK_NORMAL );
    pVerts->x = vCenter.x-fW2;
    pVerts->y = vCenter.y-1.0f;
    pVerts->z = vCenter.z-fH2;
    pVerts->color = 0xffffffff;
    pVerts->tu = 0.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = vCenter.x-fW2;
    pVerts->y = vCenter.y-1.0f;
    pVerts->z = vCenter.z+fH2;
    pVerts->color = 0xffffffff;
    pVerts->tu = 0.0f;
    pVerts->tv = 0.0f;
    pVerts++;
    pVerts->x = vCenter.x+fW2;
    pVerts->y = vCenter.y-1.0f;
    pVerts->z = vCenter.z-fH2;
    pVerts->color = 0xffffffff;
    pVerts->tu = 1.0f;
    pVerts->tv = 1.0f;
    pVerts++;
    pVerts->x = vCenter.x+fW2;
    pVerts->y = vCenter.y-1.0f;
    pVerts->z = vCenter.z+fH2;
    pVerts->color = 0xffffffff;
    pVerts->tu = 1.0f;
    pVerts->tv = 0.0f;
    m_pVB->Unlock();

    // Set the boundingBox.
    m_Bounds.vcMin.Set( vCenter.x-fW2,vCenter.y-5.0f,vCenter.z-fH2 );
    m_Bounds.vcMax.Set( vCenter.x+fW2,vCenter.y+5.0f,vCenter.z+fH2 );
}


//------------------------------------------------------------------------
// Render the liquid object.
//------------------------------------------------------------------------
void etLiquidRenderable::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = etCoreManager::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( RT_TRIANGLESTRIP );
    pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,0,4,m_pVB );
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT etLiquidRenderable::GetNumVerts(void) const
{
    return 4;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT etLiquidRenderable::GetNumIndis(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT etLiquidRenderable::GetNumPrims(void) const
{
    return 2;
}


//------------------------------------------------------------------------
// Get the local boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& etLiquidRenderable::GetBoundingBox(void) const
{
    return m_Bounds;
}


//------------------------------------------------------------------------
// Get the square distance to the camera.
//------------------------------------------------------------------------
float etLiquidRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_Bounds.GetCenter() * m_pParent->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}

//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void etLiquidRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pParent->GetWorldTransform();
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS ETLIQUIDFACTORY.

//------------------------------------------------------------------------
// Create an instance of the liquid object.
//------------------------------------------------------------------------
FlySceneObject* etLiquidFactory::CreateInstance( const char* cName )
{
    FlySceneObject* pObject = new etLiquidObject( cName );
    if( !pObject ) return NULL;

    pObject->SetCreator( this );
    return pObject;
}

//------------------------------------------------------------------------
// Destroy the instance of the liquid object.
//------------------------------------------------------------------------
void etLiquidFactory::DestroyInstance( FlySceneObject* pObject )
{
    SAFE_DELETE( pObject );
}