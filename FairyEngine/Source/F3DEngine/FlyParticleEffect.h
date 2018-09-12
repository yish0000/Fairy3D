//------------------------------------------------------------------------
// Name: FlyParticleEffect.h
// Desc: This file define a particle effect class. The particle effect is
//       the basic scene node in the particle system.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_PARTICLE_EFFECT_H__
#define __FLY_PARTICLE_EFFECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyParticle.h"
#include "FlyParticleEmitter.h"

//////////////////////////////////////////////////////////////////////////


/*
* The class store the particle effect data. 
* This is a basic scene node in the particle system.
*/
class _FLY_EXPORT FlyParticleEffect
{
public:
    enum AlphaMode { ALPHA_ADD,ALPHA_BLEND };

    struct sController
    {
        std::string sName;
        int nParticle;
        int nEmitter;
        UINT nMaxNumParticles;
        AlphaMode alphaMode;
        bool bEmitterLoop;
    };

    typedef std::vector<sController> ControllerList;

    typedef std::vector<FlyParticle*> ParticleList;
    typedef std::vector<FlyParticleEmitter*> EmitterList;
    typedef std::vector<FlyTexture*> TextureList;

protected:
    std::string m_sName;            // Name of the effect.
    ParticleList m_Particles;       // Particle template list.
    TextureList m_Textures;         // Texture list for the scene node.
    EmitterList m_Emitters;         // Emitter template list.
    ControllerList m_Controllers;   // Particle Controller list.

    int m_nDirtyState;              // Current dirty state of the effect.

public:
    // Constructor and destructor.
    FlyParticleEffect(void);
    ~FlyParticleEffect(void);

    // Reset the particle effect.
    void Reset(void);

    // Load and save the particle data.
    HRESULT LoadParticleData( const char* cFilename );
    HRESULT SaveParticleData( const char* cFilename );

    FlyParticle* AddParticle( const char* cName,const char* cTexName );
    FlyParticleEmitter* AddEmitter( const char* cName );
    sController* AddController( const char* cName,const char* cParName,const char* cEmitterName );
    sController* AddController( const char* cName,size_t nParIndex,size_t nEmitterIndex );

    void DeleteParticle( size_t nIndex );
    void DeleteEmitter( size_t nIndex );
    void DeleteController( size_t nIndex );

    // Set the new texture for the particle template.
    void SetParticleTexture( const char* cParName,const char* cTexName );

    const char* GetName(void) const;
    void SetName( const char* cName );

    // Get the particle's texture.
    FlyTexture* GetParticleTexture( size_t nIndex );
    FlyTexture* GetParticleTextureByName( const char* cParName );

    FlyParticle* GetParticle( size_t nIndex );
    FlyParticle* GetParticleByName( const char* cName );
    FlyParticleEmitter* GetEmitter( size_t nIndex );
    FlyParticleEmitter* GetEmitterByName( const char* cName );
    sController* GetController( size_t nIndex );
    sController* GetControllerByName( const char* cName );

    size_t GetNumParticles(void) const;
    size_t GetNumEmitters(void) const;
    size_t GetNumControllers(void) const;

    int GetDirtyState(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_PARTICLE_EFFECT_H__