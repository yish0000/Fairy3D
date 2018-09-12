/*
 * -----------------------------------------------------------------------
 *	Name:   FlyParticle.h
 *  Desc:   本文件定义了粒子系统中的粒子模板，粒子模板用于指导粒子一生中
 *          的各种属性变化。
 *  Author: Yish
 *  Date:   8/9/2010
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FLY_PARTICLE_H__
#define __FLY_PARTICLE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


/*
* Basic struct of the particle to render in the system.
*/
struct sParticle
{
    FlyVector   vPos;       // Position of the particle
    FLYCOLOR    cColor;     // Color of the particle.
    float       fSize;      // Size of the particle.
    float       fAngle;     // Rotation angle of the particle.
    int         nAnim;      // Animation ID of the particle.
    int         nFrame;     // Frame ID of the particle.
    float       fAge;       // Age of the particle.
    float       fLife;      // Life of the particle.
    FlyVector   vVelocity;  // Velocity of the particle.
    FlyVector   vAccel;     // Acceleration of the particle.
};

/*
* Class to control the particle template data.
* Every particle template has only one texture. The texture can
* be parted in many little images. They composed as an MxN matrix.
* The M is the number of animations, and N is the number of
* frames.
*/
class _FLY_EXPORT FlyParticle
{
    friend class FlyParticleController;
    friend class FlyParticleEffect;

public:
    struct sKeyframe_Size
    {
        float fTime;    // Time of current frame. (0.0 - 1.0)
        float fSize;    // Size value of the particle at this frame.
    };

    struct sKeyframe_Angle
    {
        float fTime;    // Time of current frame. (0.0 - 1.0)
        float fAngle;   // Angle value of the particle at this frame.
    };

    struct sKeyframe_Alpha
    {
        float fTime;    // Time of current frame. (0.0 - 1.0)
        float fAlpha;   // Alpha value of the particle at this frame.
    };

    struct sKeyframe_Color
    {
        float fTime;    // Time of current frame. (0.0 - 1.0)
        FLYCOLOR color; // Color value of the particle at this frame.
    };

    typedef std::vector<sKeyframe_Size> KFSizeList;
    typedef std::vector<sKeyframe_Angle> KFAngleList;
    typedef std::vector<sKeyframe_Alpha> KFAlphaList;
    typedef std::vector<sKeyframe_Color> KFColorList;

protected:
    std::string m_sName;        // Name of the particle template.

    std::string m_sTexName;     // File path of the texture file.
    int m_nNumAnims;            // Number of the animations.
    int m_nNumFrames;           // Number of the frames.

    float m_fGravity;           // Gravity for the particles.
    float m_fResistValue;       // Air resistance gene.
    float m_fMinSpeedResist;    // The minimum speed that can cause the air resistance.
    bool m_bUseResist;          // Turn on the air resistance ?

    KFSizeList m_KFSize;        // Size's change during the life.
    KFAngleList m_KFAngle;      // Angle's change during the life.
    KFAlphaList m_KFAlpha;      // Alpha's change during the life.
    KFColorList m_KFColor;      // Color's change during the life.

    // Update the particle's attributes.
    void UpdateSize( sParticle* pPar );
    void UpdateAngle( sParticle* pPar );
    void UpdateAlpha( sParticle* pPar );
    void UpdateColor( sParticle* pPar );

public:
    // Constructor and destructor.
    FlyParticle(void);
    ~FlyParticle(void);

    bool AddKFSize( float fTime,float fSize );
    bool AddKFAngle( float fTime,float fAngle );
    bool AddKFAlpha( float fTime,float fAlpha );
    bool AddKFColor( float fTime,FLYCOLOR color );

    void ModifyKFSize( int nIndex,float fTime,float fSize );
    void ModifyKFAngle( int nIndex,float fTime,float fAngle );
    void ModifyKFAlpha( int nIndex,float fTime,float fAlpha );
    void ModifyKFColor( int nIndex,float fTime,FLYCOLOR color );

    // Update the particle's attributes.
    bool Update( sParticle* pPar,float fElapsed );

    const char* GetName(void) const;
    const char* GetTexName(void) const;
    int GetNumAnims(void) const;
    int GetNumFrames(void) const;
    float GetGravity(void) const;
    float GetResistValue(void) const;
    float GetMinSpeedResist(void) const;
    bool IsUseResist(void) const;

    void SetName( const char* cName );
    void SetTexName( const char* cTexName );
    void SetNumAnims( int nNumAnims );
    void SetNumFrames( int nNumFrames );
    void SetGravity( float fGravity );
    void SetResistValue( float fResistValue );
    void SetMinSpeedResist( float fValue );
    void UseAirResist( bool bUseResist );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_PARTICLE_H__