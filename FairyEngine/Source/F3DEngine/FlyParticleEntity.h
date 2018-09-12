//------------------------------------------------------------------------
// Name: FlyParticleEntity.h
// Desc: This file define a real instance of the particle effect.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_PARTICLE_ENTITY_H__
#define __FLY_PARTICLE_ENTITY_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlySceneObject.h"

#include "FlyParticleController.h"
#include "FlyParticleEffect.h"
//////////////////////////////////////////////////////////////////////////


/*
* A real instance of the particle effect in the
* 3D world.
*/
class _FLY_EXPORT FlyParticleEntity : public FlySceneObject
{
    typedef std::vector<FlyParticleController*> ControllerList;

protected:
    FlyParticleEffect* m_pEffect;   // Pointer to the effect template.
    ControllerList m_Controllers;   // Particle controller list.

    FlyAabb m_LocalBBox;            // Local boundingBox of the entity.
    int m_nDirtyState;              // Dirty state of the entity.

    FlyMatrix m_mTMNoRotation;      // Transform matrix without rotation.

    // Update the entity's boundingBox.
    void UpdateBounds(void);

    // Build the particle controllers.
    HRESULT BuildControllers(void);

    // Reinitialize the entity.
    HRESULT ReInitialize(void);

public:
    // Constructor and destructor.
    FlyParticleEntity( const char* cName );
    ~FlyParticleEntity(void);

    // Initialize the particle entity.
    HRESULT Initialize( FlyParticleEffect* pEffect );

    // Update the particle entity.
    void UpdateObject(void);

    // Render the particle entity.
    void RenderObject(void);

    const FlyAabb& GetLocalBoundingBox(void) const;
    const FlyMatrix& GetWorldTransform(void) const;

    size_t GetNumControllers(void) const;
    FlyParticleController* GetController( size_t nIndex );

    // Get the count of the active particles.
    UINT GetNumActiveParticles(void) const;

    FlyParticleEffect* GetParticleEffect(void);
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyParticleEntityFactory : public FlyEntityFactory
{
public:
    // Constructor and destructor.
    FlyParticleEntityFactory(void) { m_sTypename = "particle"; }
    ~FlyParticleEntityFactory(void) { /* Nothing to do! */ };

    FlySceneObject* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneObject* pObject );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_PARTICLE_ENTITY_H__