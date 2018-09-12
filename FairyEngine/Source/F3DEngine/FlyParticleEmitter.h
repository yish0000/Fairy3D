//------------------------------------------------------------------------
// Name: FlyParticleEmitter.h
// Desc: This file define a class to store the particle emitter's 
//       template data. Emitters can Emit different particles during 
//       their life according to the template data.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_PARTICLE_EMITTER_H__
#define __FLY_PARTICLE_EMITTER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyTrack.h"
//////////////////////////////////////////////////////////////////////////


/*
* Class to control the emitter template data.
*/
class _FLY_EXPORT FlyParticleEmitter
{
    friend class FlyParticleController;
    friend class FlyParticleEffect;

public:
    struct sKeyframe_Life
    {
        float fTime;            // Time of current frame. (0.0 - 1.0)
        float fMinLife;         // Minimum value of the life at this frame.
        float fMaxLife;         // Maximum value of the life at this frame.
    };

    struct sKeyframe_VelocityDir
    {
        float fTime;            // Time of current frame. (0.0 - 1.0)
        FlyVector vVelocityDir; // Velocity direction at this frame.
    };

    struct sKeyframe_Velocity
    {
        float fTime;            // Time of current frame. (0.0 - 1.0)
        float fMinVelocity;     // Minimum value of the velocity at this frame.
        float fMaxVelocity;     // Maximum value of the velocity at this frame.
    };

    struct sKeyframe_EmitRate
    {
        float fTime;            // Time of current frame. (0.0 - 1.0)
        float fMinEmitRate;     // Minimum value of the emit rate at this frame.
        float fMaxEmitRate;     // Maximum value of the emit rate at this frame.
    };

    typedef std::vector<sKeyframe_Life> KFLifeList;
    typedef std::vector<sKeyframe_VelocityDir> KFVelocityDirList;
    typedef std::vector<sKeyframe_Velocity> KFVelocityList;
    typedef std::vector<sKeyframe_EmitRate> KFEmitRateList;
    typedef std::vector<FlyVector> ShapeVertex;

    enum EMITTERSHAPE
    {
        EMS_CUBE,
        EMS_SPHERE,
        EMS_USERDEFINE,
    };

protected:
    std::string m_sName;        // Name of the emitter template.
    FlyVector m_vPos;           // Position of the emitter.
    float m_fLife;              // Life of the emitter.
    bool m_bUseTrack;           // Use the track for the emitter.
    FlyTrack m_Track;           // Track data of the emitter.

    EMITTERSHAPE m_Shape;       // Shape of the emitter.
    FlyVector m_vMinPos;        // Minimum position (For cube shape)
    FlyVector m_vMaxPos;        // Maximum position (For cube shape)
    float m_fHot;               // Hot area's radius
    float m_fAttenuation;       // Attenuation area.
    ShapeVertex m_UserDefine;   // To store the user-defined shape data.

    float m_fRandVelX;          // Rand value, Add to the new particle's original velocity.
    float m_fRandVelY;          // Rand value, Add to the new particle's original velocity.
    float m_fRandVelZ;          // Rand value, Add to the new particle's original velocity.

    KFLifeList m_KFLife;                // Par's original life's change during the emitter life.
    KFVelocityDirList m_KFVelocityDir;  // Par's original velocity dir's change during the emitter life.
    KFVelocityList m_KFVelocity;        // Par's original velocity's change during the emitter life.
    KFEmitRateList m_KFEmitRate;        // Par's original emit rate's change during the emitter life.

    float m_fEmitScale;         // Scale for the emit rate.

    //////////////////////////////////////////////////////////////////////

    // Calculate the new particle's attributes.
    void CalcParticleLife( float fAge,float* pfMin,float* pfMax );
    void CalcParticleVelocityDir( float fAge,FlyVector* pvVelocityDir );
    void CalcParticleVelocity( float fAge,float* pfMin,float* pfMax );
    void CalcEmitRate( float fAge,float* pfMin,float* pfMax );

    // Get the new particle's position according to the current
    // track age.
    FlyVector CalcParticlePos( float fTrackTime );

public:
    // Constructor and destructor.
    FlyParticleEmitter(void);
    ~FlyParticleEmitter(void);

    // Load the user-defined shape and track data.
    HRESULT LoadShapeData( const char* cShapeFile );
    void UseTrack( const char* cTrackFile );

    // Add a new keyframe to emitter template.
    bool AddKFLife( float fTime,float fMinLife,float fMaxLife );
    bool AddKFVelocityDir( float fTime,const FlyVector& vDir );
    bool AddKFVelocity( float fTime,float fMinVelocity,float fMaxVelocity );
    bool AddKFEmitRate( float fTime,float fMinEmitRate,float fMaxEmitRate );

    void ModifyKFLife( int nIndex,float fTime,float fMinLife,float fMaxLife );
    void ModifyKFVelocityDir( int nIndex,float fTime,const FlyVector& vDir );
    void ModifyKFVelocity( int nIndex,float fTime,float fMinVelocity,float fMaxVelocity );
    void ModifyKFEmitRate( int nIndex,float fTime,float fMinEmitRate,float fMaxEmitRate );

    const char* GetName(void);
    FlyVector GetPosition(void);
    EMITTERSHAPE GetEmitterShape(void);
    FlyVector GetCubeMinPos(void);
    FlyVector GetCubeMaxPos(void);
    float GetSphereHot(void);
    float GetSphereAttenuation(void);
    float GetRandVelocityX(void);
    float GetRandVelocifyY(void);
    float GetRandVelocityZ(void);
    float GetLife(void);
    float GetEmitRateScale(void);
    bool IsUseTrack(void);
    float GetTrackCycle(void);

    void SetName( const char* cName );
    void SetPosition( const FlyVector& vPos );
    void SetEmitterShape( EMITTERSHAPE shape );
    void SetCubeMinPos( const FlyVector& vPos );
    void SetCubeMaxPos( const FlyVector& vPos );
    void SetSphereHot( float fHot );
    void SetSphereAttenuation( float fAttenuation );
    void SetRandVelocityX( float fValue );
    void SetRandVelocityY( float fValue );
    void SetRandVelocityZ( float fValue );
    void SetLife( float fLife );
    void SetEmitRateScale( float fEmitScale );
    void SetTrackCycle( float fCycle );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_PARTICLE_EMITTER_H__