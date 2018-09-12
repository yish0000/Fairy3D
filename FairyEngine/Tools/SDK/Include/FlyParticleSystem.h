//------------------------------------------------------------------------
// Name: FlyParticleSystem.h
// Desc: This file define a class to control the particle effect system.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_PARTICLE_SYSTEM_H__
#define __FLY_PARTICLE_SYSTEM_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyParticleEffect.h"

//////////////////////////////////////////////////////////////////////////


/*
* The class is the interface for user to control the
* particle effect system.
*/
class _FLY_EXPORT FlyParticleSystem
{
    struct ParticleEffectNode
    {
        FlyParticleEffect* pEffect;
        int nReference;
    };

    typedef std::vector<ParticleEffectNode> ParticleEffectList;

protected:
    ParticleEffectList m_ParticleEffects;

public:
    // Constructor and destructor.
    FlyParticleSystem(void);
    ~FlyParticleSystem(void);

    void AddResourceRef( const char* cParName );
    void DecResourceRef( const char* cParName );

    // Create a null effect to the system.
    FlyParticleEffect* CreateManualEffect( const char* cName );

    // Load a particle effect from the file.
    FlyParticleEffect* LoadParticleEffect( const char* cFilename );

    // Delete an effect.
    void DestroyEffect( UINT nIndex );
    void DestroyEffectByName( const char* cName );

    FlyParticleEffect* GetParticleEffect( UINT nIndex );
    FlyParticleEffect* GetParticleEffectByName( const char* cName );

    // Count of the particle effect.
    UINT GetNumEffects(void);

    // Get the particle effect's ref count.
    int GetRefCount( const char* cParName );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_PARTICLE_SYSTEM_H__