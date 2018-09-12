//------------------------------------------------------------------------
// Name: FlyParticleSystem.cpp
// Desc: This file is to implement the class FlyParticleSystem.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyParticleSystem.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyParticleSystem::FlyParticleSystem(void)
{
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyParticleSystem::~FlyParticleSystem(void)
{
    ParticleEffectList::iterator itr;
    for( itr=m_ParticleEffects.begin();itr!=m_ParticleEffects.end();itr++ )
        SAFE_DELETE( itr->pEffect );
}


//------------------------------------------------------------------------
// Add the reference count for the particle effect resource.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the particle effect.
//------------------------------------------------------------------------
void FlyParticleSystem::AddResourceRef( const char* cParName )
{
    ParticleEffectList::iterator itr;
    for( itr=m_ParticleEffects.begin();itr!=m_ParticleEffects.end();itr++ )
    {
        if( strcmp(itr->pEffect->GetName(),cParName) == 0 )
        {
            itr->nReference++;
            break;
        }
    }
}


//------------------------------------------------------------------------
// Dec the reference count for the particle effect resource.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the particle effect.
//------------------------------------------------------------------------
void FlyParticleSystem::DecResourceRef( const char* cParName )
{
    ParticleEffectList::iterator itr;
    for( itr=m_ParticleEffects.begin();itr!=m_ParticleEffects.end();itr++ )
    {
        if( strcmp(itr->pEffect->GetName(),cParName) == 0 )
        {
            itr->nReference--;

            if( itr->nReference <= 0 )
            {
                SAFE_DELETE( itr->pEffect );
                m_ParticleEffects.erase( itr );
            }

            break;
        }
    }
}


//------------------------------------------------------------------------
// Create a null effect to the system.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the particle effect.
//------------------------------------------------------------------------
FlyParticleEffect* FlyParticleSystem::CreateManualEffect( const char* cName )
{
    ParticleEffectNode node;
    FlyParticleEffect* pEffect;

    // Check whether the effect has existed.
    if( (pEffect = GetParticleEffectByName(cName)) )
        return pEffect;

    // Create the new effect.
    pEffect = new FlyParticleEffect();
    if( !pEffect ) return NULL;
    pEffect->SetName( cName );

    node.pEffect = pEffect;
    node.nReference = 0;
    m_ParticleEffects.push_back( node );

    return pEffect;
}


//------------------------------------------------------------------------
// Load a particle effect from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the particle file.
//------------------------------------------------------------------------
FlyParticleEffect* FlyParticleSystem::LoadParticleEffect( const char* cFilename )
{
    HRESULT hr;
    ParticleEffectNode node;
    FlyParticleEffect* pEffect;

    // Check whether the effect is loaded ?
    if( (pEffect = GetParticleEffectByName(cFilename)) )
        return pEffect;

    // Create the new particle effect.
    pEffect = new FlyParticleEffect();
    if( !pEffect ) return NULL;
    pEffect->SetName( cFilename );

    // Load the particle file.
    hr = pEffect->LoadParticleData( cFilename );
    if( FAILED(hr) )
    {
        delete pEffect;
        return NULL;
    }

    node.pEffect = pEffect;
    node.nReference = 0;
    m_ParticleEffects.push_back( node );

    return pEffect;
}


//------------------------------------------------------------------------
// Destroy an particle effect from the system.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the particle effect.
//------------------------------------------------------------------------
void FlyParticleSystem::DestroyEffect( size_t nIndex )
{
    if( nIndex >= m_ParticleEffects.size() )
        return;

    SAFE_DELETE( m_ParticleEffects[nIndex].pEffect );
    m_ParticleEffects.erase( m_ParticleEffects.begin() + nIndex );
}


//------------------------------------------------------------------------
// Destroy an particle effect from the system.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle effect.
//------------------------------------------------------------------------
void FlyParticleSystem::DestroyEffectByName( const char* cName )
{
    ParticleEffectList::iterator itr;
    for( itr=m_ParticleEffects.begin();itr!=m_ParticleEffects.end();itr++ )
    {
        if( strcmp(itr->pEffect->GetName(),cName) == 0 )
        {
            SAFE_DELETE( itr->pEffect );
            m_ParticleEffects.erase( itr );
            break;
        }
    }
}


//------------------------------------------------------------------------
// Get the count of the effect nodes.
//------------------------------------------------------------------------
size_t FlyParticleSystem::GetNumEffects(void) const
{
    return m_ParticleEffects.size();
}


//------------------------------------------------------------------------
// Get the specified particle effect node.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the particle effect node.
//------------------------------------------------------------------------
FlyParticleEffect* FlyParticleSystem::GetParticleEffect( size_t nIndex )
{
    if( nIndex >= m_ParticleEffects.size() )
        return NULL;

    return m_ParticleEffects[nIndex].pEffect;
}


//------------------------------------------------------------------------
// Get the specified particle effect node.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle effect node.
//------------------------------------------------------------------------
FlyParticleEffect* FlyParticleSystem::GetParticleEffectByName( const char* cName )
{
    ParticleEffectList::iterator itr;
    for( itr=m_ParticleEffects.begin();itr!=m_ParticleEffects.end();itr++ )
    {
        if( strcmp(itr->pEffect->GetName(),cName) == 0 )
            return itr->pEffect;
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the particle effect's reference count.
//------------------------------------------------------------------------
int FlyParticleSystem::GetRefCount( const char* cParName )
{
    ParticleEffectList::iterator itr;
    for( itr=m_ParticleEffects.begin();itr!=m_ParticleEffects.end();itr++ )
    {
        if( strcmp(itr->pEffect->GetName(),cParName) == 0 )
            return itr->nReference;
    }

    return -1;
}