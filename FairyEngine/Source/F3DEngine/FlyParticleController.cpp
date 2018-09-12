//------------------------------------------------------------------------
// Name: FlyParticleController.cpp
// Desc: This file is to implement the class FlyParticleController.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyParticleController.h"

#include "FlyParticleEntity.h"
#include "FlyKernel.h"


// Define some constant variables.
const float fPI1 = FLY_PI / 4.0f;
const float fPI2 = fPI1 * 3.0f;
const float fPI3 = fPI1 * 5.0f;
const float fPI4 = fPI1 * 7.0f;
const float fSQRT2 = (float)sqrt( 2.0f ) / 2.0f;

//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyParticleController::FlyParticleController( FlyParticleEntity* pParent,const char* cMatName )
{
    m_pParent = pParent;

    m_pParticle = NULL;
    m_pEmitter = NULL;
    m_fNewParExcess = 0.0f;
    m_fEmitterAge = 0.0f;
    m_fTrackAge = 0.0f;
    m_bEmitterLoop = true;

    m_ParticleList.Create( 1000 );
    m_ParQueue.reserve( 1000 );

    // Create the render unit.
    m_pUnit = new FlyParticleRenderUnit( this,cMatName );
    m_pUnit->SetMaxParticles( 1000 );

    memset( &m_LocalBBox,0,sizeof(FlyAabb) );
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyParticleController::~FlyParticleController(void)
{
    SAFE_DELETE( m_pUnit );
}


//------------------------------------------------------------------------
// Reset the controller, Delete all the active particles.
//------------------------------------------------------------------------
void FlyParticleController::ResetParticles(void)
{
    m_fEmitterAge = 0.0f;
    m_fTrackAge = 0.0f;
    m_fNewParExcess = 0.0f;
    m_ParticleList.DeleteAll();

    memset( &m_LocalBBox,0,sizeof(FlyAabb) );
}


//------------------------------------------------------------------------
// Update the particle controller.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Elapsed time of this frame.
//------------------------------------------------------------------------
void FlyParticleController::Update( float fElapsed )
{
    float fMin,fMax;
    float fEmitRate;
    sParticle* pPar;
    FlyVector vec;

    m_LocalBBox.vcMin.Set( -1.0f,-1.0f,-1.0f );
    m_LocalBBox.vcMax.Set(  1.0f, 1.0f, 1.0f );

    // 1. Update the active particles.
    for( UINT i=0;i<m_ParticleList.GetNumMaxElements();i++ )
    {
        if( m_ParticleList.IsAlive(i) )
        {
            pPar = m_ParticleList.GetAt( i );

            if( !m_pParticle->Update(pPar,fElapsed) )
                m_ParticleList.Delete( i );
            else
            {
                // Update the boundingBox.
                UpdateBounds( pPar );
            }
        }
    }

    // 2. Create the new particles.
    // If not loop and dead exit the function.
    if( m_fEmitterAge >= m_pEmitter->m_fLife )
    {
        if( !m_bEmitterLoop )
            return;

        // Restart the emitter.
        m_fEmitterAge = 0.0f;
    }

    // Update the track age is needed.
    if( m_pEmitter->m_bUseTrack && m_fTrackAge >= m_pEmitter->GetTrackCycle() )
        m_fTrackAge = 0.0f;

    // Get the current emit rate.
    m_pEmitter->CalcEmitRate( m_fEmitterAge,&fMin,&fMax );
    fEmitRate = FlyUtil::RandNumber( fMin,fMax );
    fEmitRate *= m_pEmitter->GetEmitRateScale();

    int nNumNewPars = (int)(fEmitRate*fElapsed);
    m_fNewParExcess += fEmitRate * fElapsed - nNumNewPars;

    if( m_fNewParExcess > 1.0f )
    {
        nNumNewPars += (int)m_fNewParExcess;
        m_fNewParExcess -= (int)m_fNewParExcess;
    }

    for( int i=0;i<nNumNewPars&&m_ParticleList.GetNumFreeElements()>0;i++ )
    {
        pPar = m_ParticleList.AddNew();
        pPar->vPos = m_pEmitter->CalcParticlePos( m_fTrackAge );
        pPar->vAccel.Set( 0.0f,0.0f,0.0f );

        m_pEmitter->CalcParticleLife( m_fEmitterAge,&fMin,&fMax );
        pPar->fLife = FlyUtil::RandNumber( fMin,fMax );
        pPar->fAge = 0.0f;
        pPar->nAnim = FlyUtil::RandNumber( 0,m_pParticle->m_nNumAnims );
        pPar->nFrame = 0;

        m_pEmitter->CalcParticleVelocityDir( m_fEmitterAge,&vec );
        m_pEmitter->CalcParticleVelocity( m_fEmitterAge,&fMin,&fMax );
        vec *= FlyUtil::RandNumber( fMin,fMax );
        vec.x += FlyUtil::RandNumber( -m_pEmitter->m_fRandVelX,m_pEmitter->m_fRandVelX );
        vec.y += FlyUtil::RandNumber( -m_pEmitter->m_fRandVelY,m_pEmitter->m_fRandVelY );
        vec.z += FlyUtil::RandNumber( -m_pEmitter->m_fRandVelZ,m_pEmitter->m_fRandVelZ );
        pPar->vVelocity = vec;

        m_pParticle->UpdateSize( pPar );
        m_pParticle->UpdateAngle( pPar );
        m_pParticle->UpdateAlpha( pPar );
        m_pParticle->UpdateColor( pPar );

        // Update the boundingBox.
        UpdateBounds( pPar );
    }

    // 3. Update the emitter.
    m_fEmitterAge += fElapsed;
    if( m_pEmitter->m_bUseTrack ) m_fTrackAge += fElapsed;
}


//------------------------------------------------------------------------
// Update the boundingBox for the controller.
//------------------------------------------------------------------------
void FlyParticleController::UpdateBounds( sParticle* par )
{
    if( par->vPos.x < m_LocalBBox.vcMin.x ) m_LocalBBox.vcMin.x = par->vPos.x;
    if( par->vPos.y < m_LocalBBox.vcMin.y ) m_LocalBBox.vcMin.y = par->vPos.y;
    if( par->vPos.z < m_LocalBBox.vcMin.z ) m_LocalBBox.vcMin.z = par->vPos.z;
    if( par->vPos.x > m_LocalBBox.vcMax.x ) m_LocalBBox.vcMax.x = par->vPos.x;
    if( par->vPos.y > m_LocalBBox.vcMax.y ) m_LocalBBox.vcMax.y = par->vPos.y;
    if( par->vPos.z > m_LocalBBox.vcMax.z ) m_LocalBBox.vcMax.z = par->vPos.z;
}


//------------------------------------------------------------------------
// Handle all the particle data.
//------------------------------------------------------------------------
void FlyParticleController::HandleParticleData(void)
{
    FlyMatrix mBillboard;

    // Get the pointer to the current camera.
    LPFLYRENDERDEVICE pDevice = FlyKernel::Instance().GetRenderDevice();

    m_pUnit->ClearParticles();

    // Make the billboard matrix.
    mBillboard.TransposeOf( pDevice->GetViewMatrix() );
    mBillboard._41 = 0.0f;
    mBillboard._42 = 0.0f;
    mBillboard._43 = 0.0f;

    // Clear the old particles.
    m_ParQueue.clear();

    for( UINT i=0;i<m_ParticleList.GetNumMaxElements();i++ )
    {
        if( m_ParticleList.IsAlive(i) )
            m_ParQueue.push_back( m_ParticleList.GetAt(i) );
    }

    // Sort the particles by their distance to the camera.
    std::sort( m_ParQueue.begin(),m_ParQueue.end(),ParCompare() );

    // Construct the geometry data for particles.
    ParticleQueue::iterator itr;
    for( itr=m_ParQueue.begin();itr!=m_ParQueue.end();itr++ )
        HandleParticle( *itr,&mBillboard );

    // Set the boundingBox for render unit.
    m_pUnit->SetBoundingBox( m_LocalBBox );
}


//------------------------------------------------------------------------
// Handle the single particle, Add the geometry data to buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      sParticle*:     Pointer to the specified particle.
//      FlyMatrix*:     Pointer to the billboard matrix.
//------------------------------------------------------------------------
void FlyParticleController::HandleParticle( sParticle* pPar,FlyMatrix* pBillboard )
{
    FlyVector vTmp;
    VERTEXL vert[4];
    WORD indis[6] = { 0,1,2,2,3,0 };

    float fRadius = pPar->fSize * fSQRT2;
    float fW = 1.0f / m_pParticle->m_nNumFrames;
    float fH = 1.0f / m_pParticle->m_nNumAnims;

    // Construct the vertices.
    vert[0].x = fRadius * cosf(fPI2+pPar->fAngle) + pPar->vPos.x;
    vert[0].y = fRadius * sinf(fPI2+pPar->fAngle) + pPar->vPos.y;
    vert[0].z = pPar->vPos.z;
    vert[0].color = pPar->cColor.GetRGBAValue();
    vert[0].tu = pPar->nFrame * fW;
    vert[0].tv = pPar->nAnim * fH;

    vert[1].x = fRadius * cosf(fPI1+pPar->fAngle) + pPar->vPos.x;
    vert[1].y = fRadius * sinf(fPI1+pPar->fAngle) + pPar->vPos.y;
    vert[1].z = pPar->vPos.z;
    vert[1].color = pPar->cColor.GetRGBAValue();
    vert[1].tu = (pPar->nFrame+1) * fW;
    vert[1].tv = pPar->nAnim * fH;

    vert[2].x = fRadius * cosf(fPI4+pPar->fAngle) + pPar->vPos.x;
    vert[2].y = fRadius * sinf(fPI4+pPar->fAngle) + pPar->vPos.y;
    vert[2].z = pPar->vPos.z;
    vert[2].color = pPar->cColor.GetRGBAValue();
    vert[2].tu = (pPar->nFrame+1) * fW;
    vert[2].tv = (pPar->nAnim+1) * fH;

    vert[3].x = fRadius * cosf(fPI3+pPar->fAngle) + pPar->vPos.x;
    vert[3].y = fRadius * sinf(fPI3+pPar->fAngle) + pPar->vPos.y;
    vert[3].z = pPar->vPos.z;
    vert[3].color = pPar->cColor.GetRGBAValue();
    vert[3].tu = pPar->nFrame * fW;
    vert[3].tv = (pPar->nAnim+1) * fH;

    memcpy( &vTmp,&vert[0],12 );
    vTmp -= pPar->vPos;
    vTmp.RotateWith( *pBillboard );
    vTmp += pPar->vPos;
    memcpy( &vert[0],&vTmp,12 );
    memcpy( &vTmp,&vert[1],12 );
    vTmp -= pPar->vPos;
    vTmp.RotateWith( *pBillboard );
    vTmp += pPar->vPos;
    memcpy( &vert[1],&vTmp,12 );
    memcpy( &vTmp,&vert[2],12 );
    vTmp -= pPar->vPos;
    vTmp.RotateWith( *pBillboard );
    vTmp += pPar->vPos;
    memcpy( &vert[2],&vTmp,12 );
    memcpy( &vTmp,&vert[3],12 );
    vTmp -= pPar->vPos;
    vTmp.RotateWith( *pBillboard );
    vTmp += pPar->vPos;
    memcpy( &vert[3],&vTmp,12 );

    // Add the vertex and index data to buffer.
    m_pUnit->AddParticleData( vert,indis );
}


//------------------------------------------------------------------------
// Get the local boundingBox of the controller.
//------------------------------------------------------------------------
const FlyAabb& FlyParticleController::GetLocalBBox(void) const
{
    return m_LocalBBox;
}


//------------------------------------------------------------------------
// Get the count of the active particles.
//------------------------------------------------------------------------
UINT FlyParticleController::GetNumActiveParticles(void)
{
    return m_ParticleList.GetNumUsedElements();
}

//------------------------------------------------------------------------
// Get the current age of the emitter.
//------------------------------------------------------------------------
float FlyParticleController::GetEmitterAge(void)
{
    return m_fEmitterAge;
}

//------------------------------------------------------------------------
// Get the current age of the track.
//------------------------------------------------------------------------
float FlyParticleController::GetTrackAge(void)
{
    return m_fTrackAge;
}

//------------------------------------------------------------------------
// Does the emitter have a circular cycle.
//------------------------------------------------------------------------
bool FlyParticleController::IsEmitterLoop(void)
{
    return m_bEmitterLoop;
}

//------------------------------------------------------------------------
// How many particles can the controller make.
//------------------------------------------------------------------------
UINT FlyParticleController::GetMaxNumParticles(void)
{
    return m_ParticleList.GetNumMaxElements();
}


//------------------------------------------------------------------------
// Set the emitter's loop mode.
//------------------------------------------------------------------------
void FlyParticleController::SetEmitterLoop( bool bLoop )
{
    m_bEmitterLoop = bLoop;
}

//------------------------------------------------------------------------
// Set the Maximum count of the particles, and resize the buffer.
//------------------------------------------------------------------------
void FlyParticleController::SetMaxNumParticles( UINT nMaxNum )
{
    m_ParticleList.Create( nMaxNum );
    m_ParQueue.reserve( nMaxNum );

    m_pUnit->SetMaxParticles( nMaxNum );
}

//------------------------------------------------------------------------
// Set the alpha mode when rendering the particles.
//------------------------------------------------------------------------
void FlyParticleController::SetAlphaMode( FlyParticleController::AlphaMode mode )
{
    // Get the render material.
    FlyRenderMaterial* pMaterial = m_pUnit->GetRenderMaterial();

    if( mode == FlyParticleController::ALPHA_ADD )
        pMaterial->SetAlphaBlendMode( 0,SBM_ONE,SBM_ONE );
    else
        pMaterial->SetAlphaBlendMode( 0,SBM_SRCALPHA,SBM_INVSRCALPHA );

    m_AlphaMode = mode;
}

//------------------------------------------------------------------------
// Get the alpha mode when rendering the particles.
//------------------------------------------------------------------------
FlyParticleController::AlphaMode FlyParticleController::GetAlphaMode(void)
{
    return m_AlphaMode;
}

//------------------------------------------------------------------------
// Set the current age of the emitter.
//------------------------------------------------------------------------
void FlyParticleController::SetEmitterAge( float fAge )
{
    m_fEmitterAge = fAge;
}

//------------------------------------------------------------------------
// Set the current age of the track.
//------------------------------------------------------------------------
void FlyParticleController::SetTrackAge( float fAge )
{
    m_fTrackAge = fAge;
}


//------------------------------------------------------------------------
// Get the pointer to the particle template.
//------------------------------------------------------------------------
FlyParticle* FlyParticleController::GetParticleTemplate(void)
{
    return m_pParticle;
}

//------------------------------------------------------------------------
// Get the pointer to the emitter template.
//------------------------------------------------------------------------
FlyParticleEmitter* FlyParticleController::GetEmitterTemplate(void)
{
    return m_pEmitter;
}

//------------------------------------------------------------------------
// Set the particle template for the controller.
//------------------------------------------------------------------------
void FlyParticleController::SetParticleTemplate( FlyParticle* pTemplate )
{
    m_pParticle = pTemplate;

    // Get the particle texture.
    FlyTexture* pTexture = m_pParent->GetParticleEffect()->GetParticleTextureByName( pTemplate->GetName() );
    m_pUnit->GetRenderMaterial()->SetTexture( 0,0,pTexture );
}

//------------------------------------------------------------------------
// Set the emitter template for the controller.
//------------------------------------------------------------------------
void FlyParticleController::SetEmitterTemplate( FlyParticleEmitter* pTemplate )
{
    m_pEmitter = pTemplate;
}


//------------------------------------------------------------------------
// Get the pointer to the controller's render unit.
//------------------------------------------------------------------------
FlyParticleRenderUnit* FlyParticleController::GetRenderUnit(void)
{
    return m_pUnit;
}


//------------------------------------------------------------------------
// Get the pointer to the parent entity.
//------------------------------------------------------------------------
FlyParticleEntity* FlyParticleController::GetParticleEntity(void)
{
    return m_pParent;
}


// Function class to compare the particles.
bool FlyParticleController::ParCompare::operator() (const sParticle* rhs1,const sParticle* rhs2) const
{
    FlyVector vTemp1;
    FlyVector vTemp2;

    // Get the camera position.
    FlyVector vCamPos = FlyKernel::Instance().GetRenderDevice()->GetCameraPos();

    vTemp1 = rhs1->vPos - vCamPos;
    vTemp2 = rhs2->vPos - vCamPos;

    return (vTemp1.GetSqrLength() > vTemp2.GetSqrLength());
}