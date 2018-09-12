//------------------------------------------------------------------------
// Name: FlyParticleEmitter.cpp
// Desc: This file is to implement the class FlyParticleEmitter.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyParticleEmitter.h"

#include "FlyUtil.h"


//------------------------------------------------------------------------
// Constructor and destructor of the class
//------------------------------------------------------------------------
FlyParticleEmitter::FlyParticleEmitter(void)
{
    m_sName = "Unnamed Emitter";
    m_vPos.Set( 0.0f,0.0f,0.0f );
    m_Shape = EMS_CUBE;
    m_vMinPos.Set( 0.0f,0.0f,0.0f );
    m_vMaxPos.Set( 0.0f,0.0f,0.0f );
    m_fHot = 0.0f;
    m_fAttenuation = 0.0f;
    m_fRandVelX = 0.0f;
    m_fRandVelY = 0.0f;
    m_fRandVelZ = 0.0f;
    m_fEmitScale = 1.0f;
    m_bUseTrack = false;

    m_KFLife.resize( 2 );
    m_KFLife[0].fTime = 0.0f;
    m_KFLife[0].fMinLife = 7.0f;
    m_KFLife[0].fMaxLife = 10.0f;
    m_KFLife[1].fTime = 1.0f;
    m_KFLife[1].fMinLife = 7.0f;
    m_KFLife[1].fMaxLife = 10.0f;

    m_KFVelocityDir.resize( 2 );
    m_KFVelocityDir[0].fTime = 0.0f;
    m_KFVelocityDir[0].vVelocityDir.Set( 0.0f,0.0f,0.0f );
    m_KFVelocityDir[1].fTime = 1.0f;
    m_KFVelocityDir[1].vVelocityDir.Set( 0.0f,0.0f,0.0f );

    m_KFVelocity.resize( 2 );
    m_KFVelocity[0].fTime = 0.0f;
    m_KFVelocity[0].fMinVelocity = 3.5f;
    m_KFVelocity[0].fMaxVelocity = 5.0f;
    m_KFVelocity[1].fTime = 1.0f;
    m_KFVelocity[1].fMinVelocity = 3.5f;
    m_KFVelocity[1].fMaxVelocity = 5.0f;

    m_KFEmitRate.resize( 2 );
    m_KFEmitRate[0].fTime = 0.0f;
    m_KFEmitRate[0].fMinEmitRate = 35.0f;
    m_KFEmitRate[0].fMaxEmitRate = 50.0f;
    m_KFEmitRate[1].fTime = 1.0f;
    m_KFEmitRate[1].fMinEmitRate = 35.0f;
    m_KFEmitRate[1].fMaxEmitRate = 50.0f;
}

FlyParticleEmitter::~FlyParticleEmitter(void)
{
}


//------------------------------------------------------------------------
// Load the shape data from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the shape file.
//------------------------------------------------------------------------
HRESULT FlyParticleEmitter::LoadShapeData( const char* cShapeFile )
{
    int nNumShapes;
    int nNumVerts;
    FlyVector vTemp;
    char cIdentifier[4];

    FILE* pFile = fopen( cShapeFile,"rb" );
    if( !pFile ) return false;

    // Read the file identifier!
    cIdentifier[0] = fgetc( pFile );
    cIdentifier[1] = fgetc( pFile );
    cIdentifier[2] = fgetc( pFile );
    cIdentifier[3] = fgetc( pFile );
    if( strcmp(cIdentifier,"SHP") != 0  )
    {
        fclose( pFile );
        return false;
    }

    // Start to read the data.
    m_UserDefine.clear();

    fread( &nNumShapes,sizeof(int),1,pFile );

    for( int i=0;i<nNumShapes;i++ )
    {
        fread( &nNumVerts,sizeof(int),1,pFile );

        for( int j=0;j<nNumVerts;j++ )
        {
            fread( &vTemp,sizeof(FlyVector),1,pFile );
            m_UserDefine.push_back( vTemp );
        }
    }

    fclose( pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Use a track for the emitter. The emitter's position move according to
// the track during the life.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the track file.
//------------------------------------------------------------------------
void FlyParticleEmitter::UseTrack( const char* cTrackFile )
{
    if( !cTrackFile )
    {
        m_bUseTrack = false;
    }
    else
    {
        m_bUseTrack = true;

        // Load the track data.
        m_Track.LoadFromFile( cTrackFile );
    }
}


//------------------------------------------------------------------------
// Add a keyframe to the life array.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Time of the keyframe.
//      float:      Min life value of the keyframe.
//      float:      Max life value of the keyframe.
//------------------------------------------------------------------------
bool FlyParticleEmitter::AddKFLife( float fTime,float fMinLife,float fMaxLife )
{
    float fT1,fT2;
    sKeyframe_Life kfLife;
    KFLifeList::iterator itr;

    if( fTime < 0.0f || fTime > 1.0f )
        return false;

    if( m_KFLife.size() == 0 )
    {
        kfLife.fTime = fTime;
        kfLife.fMinLife = fMinLife;
        kfLife.fMaxLife = fMaxLife;
        m_KFLife.push_back( kfLife );
        return true;
    }
    else if( m_KFLife.size() == 1 )
    {
        kfLife.fTime = fTime;
        kfLife.fMinLife = fMinLife;
        kfLife.fMaxLife = fMaxLife;

        if( fTime > m_KFLife[0].fTime )
            m_KFLife.push_back( kfLife );
        else
            m_KFLife.insert( m_KFLife.begin(),kfLife );

        return true;
    }

    // Loop through all the keyframe.
    // search the position for new keyframe.
    for( size_t i=0;i<m_KFLife.size()-1;i++ )
    {
        fT1 = m_KFLife[i].fTime;
        fT2 = m_KFLife[i+1].fTime;

        if( fTime == fT1 )
        {
            m_KFLife[i].fTime = fTime;
            m_KFLife[i].fMinLife = fMinLife;
            m_KFLife[i].fMaxLife = fMaxLife;
            return true;
        }
        else if( fTime == fT2 )
        {
            m_KFLife[i+1].fTime = fTime;
            m_KFLife[i+1].fMinLife = fMinLife;
            m_KFLife[i+1].fMaxLife = fMaxLife;
            return true;
        }
        else if( fTime > fT1 && fTime < fT2 )
        {
            itr = m_KFLife.begin() + i + 1;
            kfLife.fTime = fTime;
            kfLife.fMinLife = fMinLife;
            kfLife.fMaxLife = fMaxLife;
            m_KFLife.insert( itr,kfLife );
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Add a keyframe to the velocity direction array.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Time of the keyframe.
//      FlyVector:  Direction of the velocity.
//------------------------------------------------------------------------
bool FlyParticleEmitter::AddKFVelocityDir( float fTime,const FlyVector& vDir )
{
    float fT1,fT2;
    sKeyframe_VelocityDir kfVelocityDir;
    KFVelocityDirList::iterator itr;

    if( fTime < 0.0f || fTime > 1.0f )
        return false;

    if( m_KFVelocityDir.size() == 0 )
    {
        kfVelocityDir.fTime = fTime;
        kfVelocityDir.vVelocityDir = vDir;
        m_KFVelocityDir.push_back( kfVelocityDir );
        return true;
    }
    else if( m_KFVelocityDir.size() == 1 )
    {
        kfVelocityDir.fTime = fTime;
        kfVelocityDir.vVelocityDir = vDir;

        if( fTime > m_KFVelocityDir[0].fTime )
            m_KFVelocityDir.push_back( kfVelocityDir );
        else
            m_KFVelocityDir.insert( m_KFVelocityDir.begin(),kfVelocityDir );

        return true;
    }

    // Loop through all the keyframe.
    // search the position for new keyframe.
    for( size_t i=0;i<m_KFVelocityDir.size()-1;i++ )
    {
        fT1 = m_KFVelocityDir[i].fTime;
        fT2 = m_KFVelocityDir[i+1].fTime;

        if( fTime == fT1 )
        {
            m_KFVelocityDir[i].fTime = fTime;
            m_KFVelocityDir[i].vVelocityDir = vDir;
            return true;
        }
        else if( fTime == fT2 )
        {
            m_KFVelocityDir[i+1].fTime = fTime;
            m_KFVelocityDir[i+1].vVelocityDir = vDir;
            return true;
        }
        else if( fTime > fT1 && fTime < fT2 )
        {
            itr = m_KFVelocityDir.begin() + i + 1;
            kfVelocityDir.fTime = fTime;
            kfVelocityDir.vVelocityDir = vDir;
            m_KFVelocityDir.insert( itr,kfVelocityDir );
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Add a keyframe to the velocity value array.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Time of the keyframe.
//      float:      Min velocity value of the keyframe.
//      float:      Max velocity value of the keyframe.
//------------------------------------------------------------------------
bool FlyParticleEmitter::AddKFVelocity( float fTime,float fMinVelocity,float fMaxVelocity )
{
    float fT1,fT2;
    sKeyframe_Velocity kfVelocity;
    KFVelocityList::iterator itr;

    if( fTime < 0.0f || fTime > 1.0f )
        return false;

    if( m_KFVelocity.size() == 0 )
    {
        kfVelocity.fTime = fTime;
        kfVelocity.fMinVelocity = fMinVelocity;
        kfVelocity.fMaxVelocity = fMaxVelocity;
        m_KFVelocity.push_back( kfVelocity );
        return true;
    }
    else if( m_KFVelocity.size() == 1 )
    {
        kfVelocity.fTime = fTime;
        kfVelocity.fMinVelocity = fMinVelocity;
        kfVelocity.fMaxVelocity = fMaxVelocity;

        if( fTime > m_KFVelocity[0].fTime )
            m_KFVelocity.push_back( kfVelocity );
        else
            m_KFVelocity.insert( m_KFVelocity.begin(),kfVelocity );

        return true;
    }

    // Loop through all the keyframe.
    // search the position for new keyframe.
    for( size_t i=0;i<m_KFVelocity.size()-1;i++ )
    {
        fT1 = m_KFVelocity[i].fTime;
        fT2 = m_KFVelocity[i+1].fTime;

        if( fTime == fT1 )
        {
            m_KFVelocity[i].fTime = fTime;
            m_KFVelocity[i].fMinVelocity = fMinVelocity;
            m_KFVelocity[i].fMaxVelocity = fMaxVelocity;
            return true;
        }
        else if( fTime == fT2 )
        {
            m_KFVelocity[i+1].fTime = fTime;
            m_KFVelocity[i+1].fMinVelocity = fMinVelocity;
            m_KFVelocity[i+1].fMaxVelocity = fMaxVelocity;
            return true;
        }
        else if( fTime > fT1 && fTime < fT2 )
        {
            itr = m_KFVelocity.begin() + i + 1;
            kfVelocity.fTime = fTime;
            kfVelocity.fMinVelocity = fMinVelocity;
            kfVelocity.fMaxVelocity = fMaxVelocity;
            m_KFVelocity.insert( itr,kfVelocity );
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Add a keyframe to the emit rate array.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Time of the keyframe.
//      float:      Min emit rate value of the keyframe.
//      float:      Max emit rate value of the keyframe.
//------------------------------------------------------------------------
bool FlyParticleEmitter::AddKFEmitRate( float fTime,float fMinEmitRate,float fMaxEmitRate )
{
    float fT1,fT2;
    sKeyframe_EmitRate kfEmitRate;
    KFEmitRateList::iterator itr;

    if( fTime < 0.0f || fTime > 1.0f )
        return false;

    if( m_KFEmitRate.size() == 0 )
    {
        kfEmitRate.fTime = fTime;
        kfEmitRate.fMinEmitRate = fMinEmitRate;
        kfEmitRate.fMaxEmitRate = fMaxEmitRate;
        m_KFEmitRate.push_back( kfEmitRate );
        return true;
    }
    else if( m_KFEmitRate.size() == 1 )
    {
        kfEmitRate.fTime = fTime;
        kfEmitRate.fMinEmitRate = fMinEmitRate;
        kfEmitRate.fMaxEmitRate = fMaxEmitRate;

        if( fTime > m_KFEmitRate[0].fTime )
            m_KFEmitRate.push_back( kfEmitRate );
        else
            m_KFEmitRate.insert( m_KFEmitRate.begin(),kfEmitRate );

        return true;
    }

    // Loop through all the keyframe.
    // search the position for new keyframe.
    for( size_t i=0;i<m_KFEmitRate.size()-1;i++ )
    {
        fT1 = m_KFEmitRate[i].fTime;
        fT2 = m_KFEmitRate[i+1].fTime;

        if( fTime == fT1 )
        {
            m_KFEmitRate[i].fTime = fTime;
            m_KFEmitRate[i].fMinEmitRate = fMinEmitRate;
            m_KFEmitRate[i].fMaxEmitRate = fMaxEmitRate;
            return true;
        }
        else if( fTime == fT2 )
        {
            m_KFEmitRate[i+1].fTime = fTime;
            m_KFEmitRate[i+1].fMinEmitRate = fMinEmitRate;
            m_KFEmitRate[i+1].fMaxEmitRate = fMaxEmitRate;
            return true;
        }
        else if( fTime > fT1 && fTime < fT2 )
        {
            itr = m_KFEmitRate.begin() + i + 1;
            kfEmitRate.fTime = fTime;
            kfEmitRate.fMinEmitRate = fMinEmitRate;
            kfEmitRate.fMaxEmitRate = fMaxEmitRate;
            m_KFEmitRate.insert( itr,kfEmitRate );
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Modify the keyframe of life value.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the keyframe.
//      float:      Time of this keyframe.
//      float:      Min life value of this keyframe.
//      float:      Max life value of this keyframe.
//------------------------------------------------------------------------
void FlyParticleEmitter::ModifyKFLife( int nIndex,float fTime,float fMinLife,float fMaxLife )
{
    m_KFLife[nIndex].fTime = fTime;
    m_KFLife[nIndex].fMinLife = fMinLife;
    m_KFLife[nIndex].fMaxLife = fMaxLife;
}


//------------------------------------------------------------------------
// Modify the keyframe of velocity direction.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the keyframe.
//      float:      Time of this keyframe.
//      FlyVector:  Direction of the velocity.
//------------------------------------------------------------------------
void FlyParticleEmitter::ModifyKFVelocityDir( int nIndex,float fTime,const FlyVector& vDir )
{
    m_KFVelocityDir[nIndex].fTime = fTime;
    m_KFVelocityDir[nIndex].vVelocityDir = vDir;
}


//------------------------------------------------------------------------
// Modify the keyframe of the velocity value.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the keyframe.
//      float:      Time of this keyframe.
//      float:      Min velocity value of the keyframe.
//      float:      Max velocity value of the keyframe.
//------------------------------------------------------------------------
void FlyParticleEmitter::ModifyKFVelocity( int nIndex,float fTime,float fMinVelocity,float fMaxVelocity )
{
    m_KFVelocity[nIndex].fTime = fTime;
    m_KFVelocity[nIndex].fMinVelocity = fMinVelocity;
    m_KFVelocity[nIndex].fMaxVelocity = fMaxVelocity;
}


//------------------------------------------------------------------------
// Modify the keyframe of the emit rate value.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the keyframe.
//      float:      Time of this keyframe.
//      float:      Min emit rate value of the keyframe.
//      float:      Max emit rate value of the keyframe.
//------------------------------------------------------------------------
void FlyParticleEmitter::ModifyKFEmitRate( int nIndex,float fTime,float fMinEmitRate,float fMaxEmitRate )
{
    m_KFEmitRate[nIndex].fTime = fTime;
    m_KFEmitRate[nIndex].fMinEmitRate = fMinEmitRate;
    m_KFEmitRate[nIndex].fMaxEmitRate = fMaxEmitRate;
}


//------------------------------------------------------------------------
// Calculate the new particle's life.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Age of the emitter.
//      float*:     To store the life's min value.
//      float*:     To store the life's max value.
//------------------------------------------------------------------------
void FlyParticleEmitter::CalcParticleLife( float fAge,float* pfMin,float* pfMax )
{
    float fT1,fT2;
    float fLifeMin1,fLifeMin2;
    float fLifeMax1,fLifeMax2;
    float fTime = fAge / m_fLife;

    for( size_t i=0;i<m_KFLife.size()-1;i++ )
    {
        fT1 = m_KFLife[i].fTime;
        fT2 = m_KFLife[i+1].fTime;

        if( fTime >= fT1 && fTime <= fT2 )
        {
            fLifeMin1 = m_KFLife[i].fMinLife;
            fLifeMin2 = m_KFLife[i+1].fMinLife;
            fLifeMax1 = m_KFLife[i].fMaxLife;
            fLifeMax2 = m_KFLife[i+1].fMaxLife;
            break;
        }
    }

    (*pfMin) = fLifeMin1 + (fLifeMin2-fLifeMin1)/(fT2-fT1)*(fTime-fT1);
    (*pfMax) = fLifeMax1 + (fLifeMax2-fLifeMax1)/(fT2-fT1)*(fTime-fT1);
}


//------------------------------------------------------------------------
// Calculate the new particle's velocity direction.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Age of the emitter.
//      FlyVector*: To store the velocity direction.
//------------------------------------------------------------------------
void FlyParticleEmitter::CalcParticleVelocityDir( float fAge,FlyVector* pvVelocityDir )
{
    float fT1,fT2;
    FlyVector vVelocityDir1,vVelocityDir2;
    float fTime = fAge / m_fLife;

    for( size_t i=0;i<m_KFVelocityDir.size()-1;i++ )
    {
        fT1 = m_KFVelocityDir[i].fTime;
        fT2 = m_KFVelocityDir[i+1].fTime;

        if( fTime >= fT1 && fTime <= fT2 )
        {
            vVelocityDir1 = m_KFVelocityDir[i].vVelocityDir;
            vVelocityDir2 = m_KFVelocityDir[i+1].vVelocityDir;
            break;
        }
    }

    pvVelocityDir->x = vVelocityDir1.x + (vVelocityDir2.x-vVelocityDir1.x)/(fT2-fT1)*(fTime-fT1);
    pvVelocityDir->y = vVelocityDir1.y + (vVelocityDir2.y-vVelocityDir1.y)/(fT2-fT1)*(fTime-fT1);
    pvVelocityDir->z = vVelocityDir1.z + (vVelocityDir2.z-vVelocityDir1.z)/(fT2-fT1)*(fTime-fT1);
}


//------------------------------------------------------------------------
// Calculate the new particle's velocity value.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Age of the emitter.
//      float*:     To store the min velocity value.
//      float*:     To store the max velocity value.
//------------------------------------------------------------------------
void FlyParticleEmitter::CalcParticleVelocity( float fAge,float* pfMin,float* pfMax )
{
    float fT1,fT2;
    float fVelocityMin1,fVelocityMin2;
    float fVelocityMax1,fVelocityMax2;
    float fTime = fAge / m_fLife;

    for( size_t i=0;i<m_KFVelocity.size()-1;i++ )
    {
        fT1 = m_KFVelocity[i].fTime;
        fT2 = m_KFVelocity[i+1].fTime;

        if( fTime >= fT1 && fTime <= fT2 )
        {
            fVelocityMin1 = m_KFVelocity[i].fMinVelocity;
            fVelocityMin2 = m_KFVelocity[i+1].fMinVelocity;
            fVelocityMax1 = m_KFVelocity[i].fMaxVelocity;
            fVelocityMax2 = m_KFVelocity[i+1].fMaxVelocity;
            break;
        }
    }

    (*pfMin) = fVelocityMin1 + (fVelocityMin2-fVelocityMin1)/(fT2-fT1)*(fTime-fT1);
    (*pfMax) = fVelocityMax1 + (fVelocityMax2-fVelocityMax1)/(fT2-fT1)*(fTime-fT1);
}


//------------------------------------------------------------------------
// Calculate the current emit rate of the emitter.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Age of the emitter.
//      float*:     To store the min emit rate.
//      float*:     To store the max emit rate.
//------------------------------------------------------------------------
void FlyParticleEmitter::CalcEmitRate( float fAge,float* pfMin,float* pfMax )
{
    float fT1,fT2;
    float fERMin1,fERMin2;
    float fERMax1,fERMax2;
    float fTime = fAge / m_fLife;

    for( size_t i=0;i<m_KFEmitRate.size()-1;i++ )
    {
        fT1 = m_KFEmitRate[i].fTime;
        fT2 = m_KFEmitRate[i+1].fTime;

        if( fTime >= fT1 && fTime <= fT2 )
        {
            fERMin1 = m_KFEmitRate[i].fMinEmitRate;
            fERMin2 = m_KFEmitRate[i+1].fMinEmitRate;
            fERMax1 = m_KFEmitRate[i].fMaxEmitRate;
            fERMax2 = m_KFEmitRate[i+1].fMaxEmitRate;
            break;
        }
    }

    (*pfMin) = fERMin1 + (fERMin2-fERMin1)/(fT2-fT1)*(fTime-fT1);
    (*pfMax) = fERMax1 + (fERMax2-fERMax1)/(fT2-fT1)*(fTime-fT1);
}


//------------------------------------------------------------------------
// Calculate the new particle's initial position.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Current track time for the emitter.
//------------------------------------------------------------------------
FlyVector FlyParticleEmitter::CalcParticlePos( float fTrackTime )
{
    int n;
    FlyVector vPos;
    FlyVector vMin,vMax;
    float fRadius,fA,fB;
    FlyVector vOrig;

    if( m_bUseTrack )
        vOrig = m_Track.GetCurPos( fTrackTime );
    else
        vOrig = m_vPos;

    switch( m_Shape )
    {
    case EMS_CUBE:
        vPos.x = FlyUtil::RandNumber( m_vMinPos.x,m_vMaxPos.x );
        vPos.y = FlyUtil::RandNumber( m_vMinPos.y,m_vMaxPos.y );
        vPos.z = FlyUtil::RandNumber( m_vMinPos.z,m_vMaxPos.z );
        vPos += vOrig;
        break;
    case EMS_SPHERE:
        if( FlyUtil::RandNumber(0.0f,1.0f) < 0.75f )
        {
            // Handle the hot area.
            fRadius = FlyUtil::RandNumber( 0.0f,m_fHot );
            fA = FlyUtil::RandNumber( 0.0f,(float)FLY_PI );
            fB = FlyUtil::RandNumber( 0.0f,(float)FLY_2PI );
            vPos.x = fRadius*sinf(fA)*cosf(fB);
            vPos.y = fRadius*sinf(fA)*sinf(fB);
            vPos.z = fRadius*cosf(fA);
            vPos += vOrig;
        }
        else
        {
            // Handle the attenuation area.
            fRadius = FlyUtil::RandNumber( m_fHot,m_fHot+m_fAttenuation );
            fA = FlyUtil::RandNumber( 0.0f,(float)FLY_PI );
            fB = FlyUtil::RandNumber( 0.0f,(float)FLY_2PI );
            vPos.x = fRadius*sinf(fA)*cosf(fB);
            vPos.y = fRadius*sinf(fA)*sinf(fA);
            vPos.z = fRadius*cosf(fA);
            vPos += vOrig;
        }
        break;
    case EMS_USERDEFINE:
        n = FlyUtil::RandNumber( 0,static_cast<int>(m_UserDefine.size()-1) );
        vPos = m_UserDefine[n] + vOrig;
        break;
    }

    return vPos;
}


//------------------------------------------------------------------------
// Get the name of the particle emitter template.
//------------------------------------------------------------------------
const char* FlyParticleEmitter::GetName(void)
{
    return m_sName.c_str();
}

//------------------------------------------------------------------------
// Get the position of the particle emitter.
//------------------------------------------------------------------------
FlyVector FlyParticleEmitter::GetPosition(void)
{
    return m_vPos;
}

//------------------------------------------------------------------------
// Get the particle emitter's shape.
//------------------------------------------------------------------------
FlyParticleEmitter::EMITTERSHAPE FlyParticleEmitter::GetEmitterShape(void)
{
    return m_Shape;
}

//------------------------------------------------------------------------
// Get the minimum position of the emitter's area. (Cube shape)
//------------------------------------------------------------------------
FlyVector FlyParticleEmitter::GetCubeMinPos(void)
{
    return m_vMinPos;
}

//------------------------------------------------------------------------
// Get the maximum position of the emitter's area. (Cube shape)
//------------------------------------------------------------------------
FlyVector FlyParticleEmitter::GetCubeMaxPos(void)
{
    return m_vMaxPos;
}

//------------------------------------------------------------------------
// Get the radius of the emitter's hot area.
//------------------------------------------------------------------------
float FlyParticleEmitter::GetSphereHot(void)
{
    return m_fHot;
}

//------------------------------------------------------------------------
// Get the thickness of the emitter's attenuation area.
//------------------------------------------------------------------------
float FlyParticleEmitter::GetSphereAttenuation(void)
{
    return m_fAttenuation;
}

//------------------------------------------------------------------------
// Get the random value which will be added to the original velocity.
//------------------------------------------------------------------------
float FlyParticleEmitter::GetRandVelocityX(void)
{
    return m_fRandVelX;
}

//------------------------------------------------------------------------
// Get the random value which will be added to the original velocity.
//------------------------------------------------------------------------
float FlyParticleEmitter::GetRandVelocifyY(void)
{
    return m_fRandVelY;
}

//------------------------------------------------------------------------
// Get the random value which will be added to the original velocity.
//------------------------------------------------------------------------
float FlyParticleEmitter::GetRandVelocityZ(void)
{
    return m_fRandVelZ;
}

//------------------------------------------------------------------------
// Get the life of the particle emitter.
//------------------------------------------------------------------------
float FlyParticleEmitter::GetLife(void)
{
    return m_fLife;
}

//------------------------------------------------------------------------
// Get the emit rate's scale of the particle emitter.
//------------------------------------------------------------------------
float FlyParticleEmitter::GetEmitRateScale(void)
{
    return m_fEmitScale;
}

//------------------------------------------------------------------------
// Does the emitter use the track to update its position ?
//------------------------------------------------------------------------
bool FlyParticleEmitter::IsUseTrack(void)
{
    return m_bUseTrack;
}

//------------------------------------------------------------------------
// Get the track's cycle.
//------------------------------------------------------------------------
float FlyParticleEmitter::GetTrackCycle(void)
{
    return m_Track.GetWholeTime();
}


//------------------------------------------------------------------------
// Set the name for the particle emitter.
//------------------------------------------------------------------------
void FlyParticleEmitter::SetName( const char* cName )
{
    m_sName = cName;
}

//------------------------------------------------------------------------
// Set the position of the emitter.
//------------------------------------------------------------------------
void FlyParticleEmitter::SetPosition( const FlyVector& vPos )
{
    m_vPos = vPos;
}

//------------------------------------------------------------------------
// Set the shape of the emitter. (Cube, Sphere, Userdefined)
//------------------------------------------------------------------------
void FlyParticleEmitter::SetEmitterShape( EMITTERSHAPE shape )
{
    m_Shape = shape;

    m_UserDefine.clear();
    m_vMinPos.Set( 0.0f,0.0f,0.0f );
    m_vMaxPos.Set( 0.0f,0.0f,0.0f );
    m_fHot = 0.0f;
    m_fAttenuation = 0.0f;
}

//------------------------------------------------------------------------
// Set the minimum position of the emitter's area. (Cube shape)
//------------------------------------------------------------------------
void FlyParticleEmitter::SetCubeMinPos( const FlyVector& vPos )
{
    m_vMinPos = vPos;
}

//------------------------------------------------------------------------
// Set the maximum position of the emitter's area. (Cube shape)
//------------------------------------------------------------------------
void FlyParticleEmitter::SetCubeMaxPos( const FlyVector& vPos )
{
    m_vMaxPos = vPos;
}

//------------------------------------------------------------------------
// Set the radius of the emitter's hot area. (Sphere shape)
//------------------------------------------------------------------------
void FlyParticleEmitter::SetSphereHot( float fHot )
{
    m_fHot = fHot;
}

//------------------------------------------------------------------------
// Set the thickness of the emitter's attenuation area. (Sphere shape)
//------------------------------------------------------------------------
void FlyParticleEmitter::SetSphereAttenuation( float fAttenuation )
{
    m_fAttenuation = fAttenuation;
}

//------------------------------------------------------------------------
// Set the random value which added to the original velocity.
//------------------------------------------------------------------------
void FlyParticleEmitter::SetRandVelocityX( float fValue )
{
    m_fRandVelX = fValue;
}

//------------------------------------------------------------------------
// Set the random value which added to the original velocity.
//------------------------------------------------------------------------
void FlyParticleEmitter::SetRandVelocityY( float fValue )
{
    m_fRandVelY = fValue;
}

//------------------------------------------------------------------------
// Set the random value which added to the original velocity.
//------------------------------------------------------------------------
void FlyParticleEmitter::SetRandVelocityZ( float fValue )
{
    m_fRandVelZ = fValue;
}

//------------------------------------------------------------------------
// Set the life value of the emitter.
//------------------------------------------------------------------------
void FlyParticleEmitter::SetLife( float fLife )
{
    m_fLife = fLife;
}

//------------------------------------------------------------------------
// Set the emit rate scale of the emitter.
//------------------------------------------------------------------------
void FlyParticleEmitter::SetEmitRateScale( float fEmitScale )
{
    m_fEmitScale = fEmitScale;
}

//------------------------------------------------------------------------
// Set the track's cycle.
//------------------------------------------------------------------------
void FlyParticleEmitter::SetTrackCycle( float fCycle )
{
    m_Track.SetWholeTime( fCycle );
}