//------------------------------------------------------------------------
// Name: FlyParticleEntity.cpp
// Desc: This file is to implement the class FlyParticleEntity.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyParticleEntity.h"
#include "FlySceneNode.h"

#include "FlyKernel.h"
#include "FlyTimer.h"
#include "FlyParticleSystem.h"


//------------------------------------------------------------------------
// Constructor of the class.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the particle entity.
//------------------------------------------------------------------------
FlyParticleEntity::FlyParticleEntity( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyParticleEntity::~FlyParticleEntity(void)
{
    for( size_t i=0;i<m_Controllers.size();i++ )
        SAFE_DELETE( m_Controllers[i] );

    FlyParticleSystem* pSystem = FlyKernel::Instance().GetParticleSystem();
    pSystem->DecResourceRef( m_pEffect->GetName() );
}


//------------------------------------------------------------------------
// Initialize the particle effect entity.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyParticleEffect*:     Pointer to the particle effect template.
//------------------------------------------------------------------------
HRESULT FlyParticleEntity::Initialize( FlyParticleEffect* pEffect )
{
    m_pEffect = pEffect;

    // Initialize the dirty state.
    m_nDirtyState = pEffect->GetDirtyState();

    // Set the local boundingBox.
    m_LocalBBox.vcMin.Set( -1.0f,-1.0f,-1.0f );
    m_LocalBBox.vcMax.Set(  1.0f, 1.0f, 1.0f );

    FlyParticleSystem* pSystem = FlyKernel::Instance().GetParticleSystem();
    pSystem->AddResourceRef( pEffect->GetName() );

    // Build the controlers.
    return BuildControllers();
}


//------------------------------------------------------------------------
// Reinitialize the particle effect entity.
//------------------------------------------------------------------------
HRESULT FlyParticleEntity::ReInitialize(void)
{
    // Destroy the old controllers.
    for( size_t i=0;i<m_Controllers.size();i++ )
        SAFE_DELETE( m_Controllers[i] );

    m_LocalBBox.vcMin.Set( -1.0f,-1.0f,-1.0f );
    m_LocalBBox.vcMax.Set(  1.0f, 1.0f, 1.0f );

    // Update the new dirty state.
    m_nDirtyState = m_pEffect->GetDirtyState();

    // Build the controllers for entity.
    return BuildControllers();
}


//------------------------------------------------------------------------
// Build the controllers for the entity.
//------------------------------------------------------------------------
HRESULT FlyParticleEntity::BuildControllers(void)
{
    std::string sMatName;
    FlyParticleEffect::sController* pCtrl;

    for( UINT i=0;i<m_pEffect->GetNumControllers();i++ )
    {
        pCtrl = m_pEffect->GetController( i );

        // Create the controller object.
        char cTemp[20];
        sMatName = "particle_";
        sMatName += m_sName;
        sprintf( cTemp,"_%d",i );
        sMatName += cTemp;
        FlyParticleController* pController = new FlyParticleController( this,sMatName.c_str() );
        if( !pController ) return FLY_OUTOFMEMORY;

        pController->SetParticleTemplate( m_pEffect->GetParticle(pCtrl->nParticle) );
        pController->SetEmitterTemplate( m_pEffect->GetEmitter(pCtrl->nEmitter) );
        pController->SetMaxNumParticles( pCtrl->nMaxNumParticles );
        pController->SetEmitterLoop( pCtrl->bEmitterLoop );
        pController->SetAlphaMode( (FlyParticleController::AlphaMode)pCtrl->alphaMode );
        m_Controllers.push_back( pController );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Update the particle effect entity.
//------------------------------------------------------------------------
void FlyParticleEntity::UpdateObject(void)
{
    // Calculate the transform matrix.
    m_mTMNoRotation.Identity();

    if( !m_pParent )
        m_mTMNoRotation = FlyMatrix::IDENTITY;
    else
    {
        const FlyVector& vTrans = m_pParent->GetDerivedPosition();
        const FlyVector& vScale = m_pParent->GetDerivedScale();
        m_mTMNoRotation.MakeTransform( vTrans,FlyQuat::IDENTITY,vScale );
    }

    // Get the elapsed time of this frame.
    float fElapsed = FlyKernel::Instance().GetTimer()->GetElapsed();

    // Update all the controllers.
    for( size_t i=0;i<m_Controllers.size();i++ )
        m_Controllers[i]->Update( fElapsed );

    // Update the boundingBox.
    UpdateBounds();
    m_WorldBBox = m_LocalBBox;
    m_WorldBBox.TransformBox( GetWorldTransform() );
}


//------------------------------------------------------------------------
// Update the boundingBox for the entity.
//------------------------------------------------------------------------
void FlyParticleEntity::UpdateBounds(void)
{
    m_LocalBBox.vcMin.Set( -1.0f,-1.0f,-1.0f );
    m_LocalBBox.vcMax.Set(  1.0f, 1.0f, 1.0f );

    for( size_t i=0;i<m_Controllers.size();i++ )
        m_LocalBBox.Merge( m_Controllers[i]->GetLocalBBox() );
}


//------------------------------------------------------------------------
// Render the particle effect entity.
//------------------------------------------------------------------------
void FlyParticleEntity::RenderObject(void)
{
    if( m_nDirtyState != m_pEffect->GetDirtyState() )
        ReInitialize();

    // Update the light objects.
    if( !m_bOpenLight )
        m_Lights.clear();
    else
        UpdateLights();

    for( size_t i=0;i<m_Controllers.size();i++ )
    {
        m_Controllers[i]->HandleParticleData();
        m_Controllers[i]->GetRenderUnit()->AddToRenderQueue();
    }
}


//------------------------------------------------------------------------
// Get the local boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& FlyParticleEntity::GetLocalBoundingBox(void) const
{
    return m_LocalBBox;
}


//------------------------------------------------------------------------
// Get the world transform matrix of the object.
//------------------------------------------------------------------------
const FlyMatrix& FlyParticleEntity::GetWorldTransform(void) const
{
    return m_mTMNoRotation;
}


//------------------------------------------------------------------------
// Get the count of the particle controllers.
//------------------------------------------------------------------------
size_t FlyParticleEntity::GetNumControllers(void) const
{
    return m_Controllers.size();
}


//------------------------------------------------------------------------
// Get the specified particle controller.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the particle controller.
//------------------------------------------------------------------------
FlyParticleController* FlyParticleEntity::GetController( size_t nIndex )
{
    if( nIndex >= m_Controllers.size() )
        return NULL;

    return m_Controllers[nIndex];
}


//------------------------------------------------------------------------
// Get the count of active particles.
//------------------------------------------------------------------------
UINT FlyParticleEntity::GetNumActiveParticles(void) const
{
    UINT nCount = 0;

    for( size_t i=0;i<m_Controllers.size();i++ )
        nCount += m_Controllers[i]->GetNumActiveParticles();

    return nCount;
}


//------------------------------------------------------------------------
// Get the pointer to the particle effect template.
//------------------------------------------------------------------------
FlyParticleEffect* FlyParticleEntity::GetParticleEffect(void)
{
    return m_pEffect;
}


//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Create a particle entity instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle entity.
//------------------------------------------------------------------------
FlySceneObject* FlyParticleEntityFactory::CreateInstance( const char* cName )
{
    FlyParticleEntity* pEntity = new FlyParticleEntity( cName );
    if( !pEntity ) return NULL;

    pEntity->SetCreator( this );
    return pEntity;
}


//------------------------------------------------------------------------
// Destroy the specified particle entity instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlySceneObject*:    Pointer to the particle entity instance.
//------------------------------------------------------------------------
void FlyParticleEntityFactory::DestroyInstance( FlySceneObject* pObject )
{
    SAFE_DELETE( pObject );
}