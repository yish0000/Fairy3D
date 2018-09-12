//------------------------------------------------------------------------
// Name: FlyParticleController.h
// Desc: This file define a class Particle controller. The controller is
//       the basic unit to update the particles.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_PARTICLE_CONTROLLER_H__
#define __FLY_PARTICLE_CONTROLLER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyParticle.h"
#include "FlyParticleEmitter.h"

#include "FlyParticleRenderUnit.h"
#include "FlyUtil.h"
//////////////////////////////////////////////////////////////////////////


/*
* The controller is the basic unit to update the particles, every controller
* Have one particle template and one emitter template. Make new particles
* and update the particles.
*/
class _FLY_EXPORT FlyParticleController
{
    friend class FlyParticleEntity;

    typedef std::vector<sParticle*> ParticleQueue;

public:
    enum AlphaMode { ALPHA_ADD,ALPHA_BLEND };

    // Function to sort the particles.
    struct ParCompare
    {
        bool operator() ( const sParticle* rhs1,const sParticle* rhs2 ) const;
    };

protected:
    FlyParticleEntity* m_pParent;       // Pointer to the particle entity.

    // Current active particle list.
    FlyRecylingArray<sParticle> m_ParticleList;

    ParticleQueue m_ParQueue;           // Particle priority queue.
    FlyParticleRenderUnit* m_pUnit;     // Pointer to the render unit.

    FlyParticle* m_pParticle;           // Pointer to the particle template.
    FlyParticleEmitter* m_pEmitter;     // Pointer to the emitter template.

    FlyAabb m_LocalBBox;                // Local boundingBox of the controller.

    float m_fEmitterAge;                // Current age of the emitter.
    float m_fTrackAge;                  // Current age of the track.
    float m_fNewParExcess;              // Use this when create the new particles.
    bool m_bEmitterLoop;                // Turn on the loop mode ?
    AlphaMode m_AlphaMode;              // Alpha blend mode when rendering.

    // Update the boundingBox.
    void UpdateBounds( sParticle* par );

public:
    // Constructor and destructor.
    FlyParticleController( FlyParticleEntity* pParent,const char* cMatName );
    ~FlyParticleController(void);

    // Clear the particles.
    void ResetParticles(void);

    // Update the controller.
    void Update( float fElapsed );

    // Handle the particle data.
    void HandleParticleData(void);

    // Handle the single particle.
    void HandleParticle( sParticle* pPar,FlyMatrix* pBillboard );

    UINT GetNumActiveParticles(void);
    float GetEmitterAge(void);
    float GetTrackAge(void);
    bool IsEmitterLoop(void);
    UINT GetMaxNumParticles(void);

    void SetEmitterLoop( bool bLoop );
    void SetMaxNumParticles( UINT nMaxNum );
    void SetEmitterAge( float fAge );
    void SetTrackAge( float fAge );

    // Set the alpha blend mode.
    void SetAlphaMode( AlphaMode mode );
    AlphaMode GetAlphaMode(void);

    const FlyAabb& GetLocalBBox(void) const;

    FlyParticle* GetParticleTemplate(void);
    FlyParticleEmitter* GetEmitterTemplate(void);
    void SetParticleTemplate( FlyParticle* pTemplate );
    void SetEmitterTemplate( FlyParticleEmitter* pTemplate );

    FlyParticleRenderUnit* GetRenderUnit(void);
    FlyParticleEntity* GetParticleEntity(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_PARTICLE_CONTROLLER_H__