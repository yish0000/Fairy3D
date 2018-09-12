/*
 * ------------------------------------------------------------------------
 *  Name:   FlyParticleSystem.h
 *  Desc:   本文件定义了引擎的特效系统中的粒子系统。
 *  Author: Yish
 *  Date:   2012/3/20
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_PARTICLESYSTEM_H__
#define __F3D_PARTICLESYSTEM_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** 粒子系统
*/
class _F3D_EXPORT F3DParticleSystem
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
    F3DParticleSystem();
    virtual ~F3DParticleSystem();

    void AddResourceRef( const char* cParName );
    void DecResourceRef( const char* cParName );

    // Create a null effect to the system.
    FlyParticleEffect* CreateManualEffect( const char* cName );

    // Load a particle effect from the file.
    FlyParticleEffect* LoadParticleEffect( const char* cFilename );

    // Delete an effect.
    void DestroyEffect( size_t nIndex );
    void DestroyEffectByName( const char* cName );

    FlyParticleEffect* GetParticleEffect( size_t nIndex );
    FlyParticleEffect* GetParticleEffectByName( const char* cName );

    // Count of the particle effect.
    size_t GetNumEffects(void) const;

    // Get the particle effect's ref count.
    int GetRefCount( const char* cParName );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_PARTICLESYSTEM_H__