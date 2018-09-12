//------------------------------------------------------------------------
// Name: FlyParticleEffect.cpp
// Desc: This file is to implement the class FlyParticleEffect.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyParticleEffect.h"

#include "FlyStringUtil.h"
#include "FlyKernel.h"
#include "FlyXmlReader.h"


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyParticleEffect::FlyParticleEffect(void)
{
    m_sName = "Unnamed effect";

    // Initialize the dirty state.
    m_nDirtyState = 0;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyParticleEffect::~FlyParticleEffect(void)
{
    for( size_t i=0;i<m_Particles.size();i++ )
        SAFE_DELETE( m_Particles[i] );

    for( size_t i=0;i<m_Emitters.size();i++ )
        SAFE_DELETE( m_Emitters[i] );

    // Delete the textures.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    for( size_t i=0;i<m_Textures.size();i++ )
        pResMgr->DeleteTexture( m_Textures[i] );
}


//------------------------------------------------------------------------
// Add a particle template for the particle effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle template.
//      const char*:    Path of the particle texture.
//------------------------------------------------------------------------
FlyParticle* FlyParticleEffect::AddParticle( const char* cName,const char* cTexName )
{
    FlyTexture* pTexture;

    // Get the resource manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Create the template.
    FlyParticle* pParticle = new FlyParticle();
    if( !pParticle ) return NULL;

    pParticle->SetName( cName );
    pParticle->SetTexName( cTexName );
    m_Particles.push_back( pParticle );

    // Load the particle texture.
    pTexture = pResMgr->Load2DTexture( cTexName,1 );
    m_Textures.push_back( pTexture );

    return pParticle;
}


//------------------------------------------------------------------------
// Add a emitter template for the particle effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the emitter template.
//------------------------------------------------------------------------
FlyParticleEmitter* FlyParticleEffect::AddEmitter( const char* cName )
{
    FlyParticleEmitter* pEmitter = new FlyParticleEmitter();
    if( !pEmitter ) return NULL;

    pEmitter->SetName( cName );
    m_Emitters.push_back( pEmitter );

    return pEmitter;
}


//------------------------------------------------------------------------
// Add a particle controller for the particle effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle controller.
//      const char*:    Name of the particle template.
//      const char*:    Name of the emitter template.
//------------------------------------------------------------------------
FlyParticleEffect::sController* FlyParticleEffect::AddController( const char* cName,const char* cParName,
                                                                  const char* cEmitterName )
{
    size_t nParIndex = -1;
    size_t nEmitterIndex = -1;

    // Search the particle template.
    size_t i = 0;
    for( ;i<m_Particles.size();i++ )
    {
        if( strcmp(m_Particles[i]->GetName(),cParName) == 0 )
        {
            nParIndex = i;
            break;
        }
    }

    // Search the emitter template.
    for( i=0;i<m_Emitters.size();i++ )
    {
        if( strcmp(m_Emitters[i]->GetName(),cEmitterName) == 0 )
        {
            nEmitterIndex = i;
            break;
        }
    }

    if( nParIndex == -1 || nEmitterIndex == -1 )
        return NULL;

    // Create the particle controller.
    sController controller;
    controller.sName = cName;
    controller.nParticle = (int)nParIndex;
    controller.nEmitter = (int)nEmitterIndex;

    // Add the new controller to the list.
    m_Controllers.push_back( controller );

    // Update the dirty state.
    m_nDirtyState++;

    return &m_Controllers[m_Controllers.size()-1];
}


//------------------------------------------------------------------------
// Add a particle controller for the particle effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle controller.
//      size_t:         Index of the particle template.
//      size_t:         Index of the emitter template.
//------------------------------------------------------------------------
FlyParticleEffect::sController* FlyParticleEffect::AddController( const char* cName,size_t nParIndex,
                                                                  size_t nEmitterIndex )
{
    if( nParIndex >= m_Particles.size() ||
        nEmitterIndex >= m_Emitters.size() )
        return NULL;

    // Create a new controller object.
    sController controller;
    memset( &controller,0,sizeof(sController) );
    controller.sName = cName;
    controller.nParticle = (int)nParIndex;
    controller.nEmitter = (int)nEmitterIndex;

    // Add the new controller to the list.
    m_Controllers.push_back( controller );

    // Update the dirty state.
    m_nDirtyState++;

    return &m_Controllers[m_Controllers.size()-1];
}


//------------------------------------------------------------------------
// Delete the specified particle template.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the particle template.
//------------------------------------------------------------------------
void FlyParticleEffect::DeleteParticle( UINT nIndex )
{
    if( nIndex >= m_Particles.size() )
        return;

    // Delete the controller which use this particle template.
    size_t ctrl = 0;
    while( ctrl < m_Controllers.size() )
    {
        if( m_Controllers[ctrl].nParticle == nIndex )
            m_Controllers.erase( m_Controllers.begin()+ctrl );
        else
            ctrl++;
    }

    SAFE_DELETE( m_Particles[nIndex] );
    m_Particles.erase( m_Particles.begin()+nIndex );

    // Delete the texture.
    m_Textures.erase( m_Textures.begin()+nIndex );

    // Update the dirty state.
    m_nDirtyState++;
}


//------------------------------------------------------------------------
// Delete the specified emitter template.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the emitter template.
//------------------------------------------------------------------------
void FlyParticleEffect::DeleteEmitter( UINT nIndex )
{
    if( nIndex >= m_Emitters.size() )
        return;

    // Delete the controller which use this particle template.
    size_t ctrl = 0;
    while( ctrl < m_Controllers.size() )
    {
        if( m_Controllers[ctrl].nEmitter == nIndex )
            m_Controllers.erase( m_Controllers.begin()+ctrl );
        else
            ctrl++;
    }

    SAFE_DELETE( m_Emitters[nIndex] );
    m_Emitters.erase( m_Emitters.begin()+nIndex );

    // Update the dirty state.
    m_nDirtyState++;
}


//------------------------------------------------------------------------
// Delete the specified particle controller.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the particle controller.
//------------------------------------------------------------------------
void FlyParticleEffect::DeleteController( UINT nIndex )
{
    if( nIndex >= m_Controllers.size() )
        return;

    m_Controllers.erase( m_Controllers.begin() + nIndex );

    // Update the dirty state.
    m_nDirtyState++;
}


//------------------------------------------------------------------------
// Load the particle effect data from the *.par file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the *.par file.
//------------------------------------------------------------------------
HRESULT FlyParticleEffect::LoadParticleData( const char* cFilename )
{
    UINT nNum;
    char cIdentifier[4];

    // Open the particle file.
    FILE* pFile = fopen( cFilename,"rb" );
    if( !pFile ) return FLY_FILENOTFOUND;

    cIdentifier[0] = fgetc( pFile );
    cIdentifier[1] = fgetc( pFile );
    cIdentifier[2] = fgetc( pFile );
    cIdentifier[3] = fgetc( pFile );
    if( strcmp(cIdentifier,"PAR") != 0 )
    {
        fclose( pFile );
        return FLY_INVALIDFILE;
    }

    // 1. Read the particle templates.
    fread( &nNum,sizeof(UINT),1,pFile );
    UINT i = 0;
    for( ;i<nNum;i++ )
    {
        size_t nLen;
        std::string str,tex;
        std::string texture;

        fread( &nLen,sizeof(size_t),1,pFile );
        str.resize( nLen,'\0' );
        fread( &str[0],nLen,1,pFile );

        fread( &nLen,sizeof(size_t),1,pFile );
        tex.resize( nLen,'\0' );
        fread( &tex[0],nLen,1,pFile );

        texture = FlyStringUtil::GetDirFromPath( cFilename );
        texture += tex;

        // Create a new particle template.
        FlyParticle* pPar = AddParticle( str.c_str(),texture.c_str() );
        if( !pPar )
        {
            fclose( pFile );
            return FLY_OUTOFMEMORY;
        }

        fread( &pPar->m_nNumAnims,sizeof(int),1,pFile );
        fread( &pPar->m_nNumFrames,sizeof(int),1,pFile );
        fread( &pPar->m_fGravity,sizeof(float),1,pFile );
        fread( &pPar->m_bUseResist,sizeof(bool),1,pFile );
        fread( &pPar->m_fResistValue,sizeof(float),1,pFile );
        fread( &pPar->m_fMinSpeedResist,sizeof(float),1,pFile );

        // Read the keyframe size.
        fread( &nLen,sizeof(size_t),1,pFile );

        size_t n = 0;
        for( ;n<nLen;n++ )
        {
            FlyParticle::sKeyframe_Size kf;

            fread( &kf,sizeof(FlyParticle::sKeyframe_Size),1,pFile );
            pPar->AddKFSize( kf.fTime,kf.fSize );
        }

        // Read the keyframe angle.
        fread( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyParticle::sKeyframe_Angle kf;

            fread( &kf,sizeof(FlyParticle::sKeyframe_Angle),1,pFile );
            pPar->AddKFAngle( kf.fTime,kf.fAngle );
        }

        // Read the keyframe alpha.
        fread( &nLen,sizeof(int),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyParticle::sKeyframe_Alpha kf;

            fread( &kf,sizeof(FlyParticle::sKeyframe_Alpha),1,pFile );
            pPar->AddKFAlpha( kf.fTime,kf.fAlpha );
        }

        // Read the keyframe color.
        fread( &nLen,sizeof(int),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyParticle::sKeyframe_Color kf;

            fread( &kf,sizeof(FlyParticle::sKeyframe_Color),1,pFile );
            pPar->AddKFColor( kf.fTime,kf.color );
        }
    }

    // 2. Read the emitter templates.
    fread( &nNum,sizeof(UINT),1,pFile );
    for( i=0;i<nNum;i++ )
    {
        size_t nLen;
        float fTrackCycle;
        std::string str;

        // Read the name of the emitter template.
        fread( &nLen,sizeof(size_t),1,pFile );
        str.resize( nLen,'\0' );
        fread( &str[0],nLen,1,pFile );

        // Create a new emitter template.
        FlyParticleEmitter* pEmitter = AddEmitter( str.c_str() );
        if( !pEmitter )
        {
            fclose( pFile );
            return FLY_OUTOFMEMORY;
        }

        fread( &pEmitter->m_vPos,sizeof(FlyVector),1,pFile );
        fread( &pEmitter->m_fLife,sizeof(float),1,pFile );
        fread( &pEmitter->m_fEmitScale,sizeof(float),1,pFile );

        // Read the emitter's shape data.
        fread( &pEmitter->m_Shape,sizeof(FlyParticleEmitter::EMITTERSHAPE),1,pFile );
        fread( &pEmitter->m_vMinPos,sizeof(FlyVector),1,pFile );
        fread( &pEmitter->m_vMaxPos,sizeof(FlyVector),1,pFile );
        fread( &pEmitter->m_fHot,sizeof(float),1,pFile );
        fread( &pEmitter->m_fAttenuation,sizeof(float),1,pFile );
        fread( &nLen,sizeof(size_t),1,pFile );
        size_t n = 0;
        for( ;n<nLen;n++ )
        {
            FlyVector vTemp;

            fread( &vTemp,sizeof(FlyVector),1,pFile );
            pEmitter->m_UserDefine.push_back( vTemp );
        }

        // Read the track data.
        fread( &pEmitter->m_bUseTrack,sizeof(bool),1,pFile );
        fread( &fTrackCycle,sizeof(float),1,pFile );
        pEmitter->SetTrackCycle( fTrackCycle );
        fread( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyVector vTemp;

            fread( &vTemp,sizeof(FlyVector),1,pFile );
            pEmitter->m_Track.AddVertex( vTemp );
        }

        fread( &pEmitter->m_fRandVelX,sizeof(float),1,pFile );
        fread( &pEmitter->m_fRandVelY,sizeof(float),1,pFile );
        fread( &pEmitter->m_fRandVelZ,sizeof(float),1,pFile );

        // Read the keyframe life.
        fread( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyParticleEmitter::sKeyframe_Life kf;

            fread( &kf,sizeof(FlyParticleEmitter::sKeyframe_Life),1,pFile );
            pEmitter->AddKFLife( kf.fTime,kf.fMinLife,kf.fMaxLife );
        }

        // Read the keyframe velocity.
        fread( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyParticleEmitter::sKeyframe_Velocity kf;

            fread( &kf,sizeof(FlyParticleEmitter::sKeyframe_Velocity),1,pFile );
            pEmitter->AddKFVelocity( kf.fTime,kf.fMinVelocity,kf.fMaxVelocity );
        }

        // Read the keyframe velocity direction.
        fread( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyParticleEmitter::sKeyframe_VelocityDir kf;

            fread( &kf,sizeof(FlyParticleEmitter::sKeyframe_VelocityDir),1,pFile );
            pEmitter->AddKFVelocityDir( kf.fTime,kf.vVelocityDir );
        }

        // Read the keyframe emit rate.
        fread( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyParticleEmitter::sKeyframe_EmitRate kf;

            fread( &kf,sizeof(FlyParticleEmitter::sKeyframe_EmitRate),1,pFile );
            pEmitter->AddKFEmitRate( kf.fTime,kf.fMinEmitRate,kf.fMaxEmitRate );
        }
    }

    // 3. Read the particle controller data.
    fread( &nNum,sizeof(UINT),1,pFile );
    for( i=0;i<nNum;i++ )
    {
        size_t nLen;
        int nIndex1,nIndex2;
        std::string str;

        // Read the name of the controller.
        fread( &nLen,sizeof(size_t),1,pFile );
        str.resize( nLen,'\0' );
        fread( &str[0],nLen,1,pFile );

        fread( &nIndex1,sizeof(int),1,pFile );
        fread( &nIndex2,sizeof(int),1,pFile );

        // Create a new particle controller.
        sController* pController = AddController( str.c_str(),nIndex1,nIndex2 );
        if( !pController )
        {
            fclose( pFile );
            return FLY_OUTOFMEMORY;
        }

        fread( &pController->nMaxNumParticles,sizeof(size_t),1,pFile );
        fread( &pController->bEmitterLoop,sizeof(bool),1,pFile );
        fread( &pController->alphaMode,sizeof(AlphaMode),1,pFile );
    }

    fclose( pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the particle effect data to the *.par file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the *.par file.
//------------------------------------------------------------------------
HRESULT FlyParticleEffect::SaveParticleData( const char* cFilename )
{
    size_t nNum;

    // Create a new file.
    FILE* pFile = fopen( cFilename,"wb" );
    if( !pFile ) return FLY_INVALIDPARAM;

    fputc( 'P',pFile );
    fputc( 'A',pFile );
    fputc( 'R',pFile );
    fputc( '\0',pFile );

    // 1. Write the particle template data.
    nNum = m_Particles.size();
    fwrite( &nNum,sizeof(UINT),1,pFile );

    UINT i = 0;
    for( ;i<nNum;i++ )
    {
        size_t nLen;

        nLen = m_Particles[i]->m_sName.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        fwrite( m_Particles[i]->m_sName.c_str(),nLen,1,pFile );

        nLen = m_Particles[i]->m_sTexName.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        fwrite( m_Particles[i]->m_sTexName.c_str(),nLen,1,pFile );

        fwrite( &m_Particles[i]->m_nNumAnims,sizeof(int),1,pFile );
        fwrite( &m_Particles[i]->m_nNumFrames,sizeof(int),1,pFile );
        fwrite( &m_Particles[i]->m_fGravity,sizeof(float),1,pFile );
        fwrite( &m_Particles[i]->m_bUseResist,sizeof(bool),1,pFile );
        fwrite( &m_Particles[i]->m_fResistValue,sizeof(float),1,pFile );
        fwrite( &m_Particles[i]->m_fMinSpeedResist,sizeof(float),1,pFile );

        nLen = m_Particles[i]->m_KFSize.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );

        size_t n = 0;
        for( ;n<nLen;n++ )
            fwrite( &m_Particles[i]->m_KFSize[n],sizeof(FlyParticle::sKeyframe_Size),1,pFile );

        nLen = m_Particles[i]->m_KFAngle.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
            fwrite( &m_Particles[i]->m_KFAngle[n],sizeof(FlyParticle::sKeyframe_Angle),1,pFile );

        nLen = m_Particles[i]->m_KFAlpha.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
            fwrite( &m_Particles[i]->m_KFAlpha[n],sizeof(FlyParticle::sKeyframe_Alpha),1,pFile );

        nLen = m_Particles[i]->m_KFColor.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
            fwrite( &m_Particles[i]->m_KFColor[n],sizeof(FlyParticle::sKeyframe_Color),1,pFile );
    }

    // 3. Write the emitter template data.
    nNum = m_Emitters.size();
    fwrite( &nNum,sizeof(UINT),1,pFile );
    for( i=0;i<nNum;i++ )
    {
        size_t nLen;
        float fTrackCycle;

        nLen = m_Emitters[i]->m_sName.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        fwrite( m_Emitters[i]->m_sName.c_str(),nLen,1,pFile );

        fwrite( &m_Emitters[i]->m_vPos,sizeof(FlyVector),1,pFile );
        fwrite( &m_Emitters[i]->m_fLife,sizeof(float),1,pFile );
        fwrite( &m_Emitters[i]->m_fEmitScale,sizeof(float),1,pFile );

        // Write the shape data.
        fwrite( &m_Emitters[i]->m_Shape,sizeof(FlyParticleEmitter::EMITTERSHAPE),1,pFile );
        fwrite( &m_Emitters[i]->m_vMinPos,sizeof(FlyVector),1,pFile );
        fwrite( &m_Emitters[i]->m_vMaxPos,sizeof(FlyVector),1,pFile );
        fwrite( &m_Emitters[i]->m_fHot,sizeof(float),1,pFile );
        fwrite( &m_Emitters[i]->m_fAttenuation,sizeof(float),1,pFile );
        nLen = m_Emitters[i]->m_UserDefine.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        size_t n = 0;
        for( ;n<nLen;n++ )
        {
            FlyVector vTemp = m_Emitters[i]->m_UserDefine[n];

            fwrite( &vTemp,sizeof(FlyVector),1,pFile );
        }

        // Write the track data.
        fwrite( &m_Emitters[i]->m_bUseTrack,sizeof(bool),1,pFile );
        fTrackCycle = m_Emitters[i]->GetTrackCycle();
        fwrite( &fTrackCycle,sizeof(float),1,pFile );
        nLen = m_Emitters[i]->m_Track.GetNumVerts();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
        {
            FlyVector vTemp = m_Emitters[i]->m_Track.GetVertex( n );

            fwrite( &vTemp,sizeof(FlyVector),1,pFile );
        }

        fwrite( &m_Emitters[i]->m_fRandVelX,sizeof(float),1,pFile );
        fwrite( &m_Emitters[i]->m_fRandVelY,sizeof(float),1,pFile );
        fwrite( &m_Emitters[i]->m_fRandVelZ,sizeof(float),1,pFile );

        nLen = m_Emitters[i]->m_KFLife.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
            fwrite( &m_Emitters[i]->m_KFLife[n],sizeof(FlyParticleEmitter::sKeyframe_Life),1,pFile );

        nLen = m_Emitters[i]->m_KFVelocity.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
            fwrite( &m_Emitters[i]->m_KFVelocity[n],sizeof(FlyParticleEmitter::sKeyframe_Velocity),1,pFile );

        nLen = m_Emitters[i]->m_KFVelocityDir.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
            fwrite( &m_Emitters[i]->m_KFVelocityDir[n],sizeof(FlyParticleEmitter::sKeyframe_VelocityDir),1,pFile );

        nLen = m_Emitters[i]->m_KFEmitRate.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( n=0;n<nLen;n++ )
            fwrite( &m_Emitters[i]->m_KFEmitRate[n],sizeof(FlyParticleEmitter::sKeyframe_EmitRate),1,pFile );
    }

    // 3. Write the particle controller data.
    nNum = m_Controllers.size();
    fwrite( &nNum,sizeof(UINT),1,pFile );
    for( i=0;i<nNum;i++ )
    {
        size_t nLen;

        nLen = m_Controllers[i].sName.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        fwrite( m_Controllers[i].sName.c_str(),nLen,1,pFile );

        fwrite( &m_Controllers[i].nParticle,sizeof(int),1,pFile );
        fwrite( &m_Controllers[i].nEmitter,sizeof(int),1,pFile );

        fwrite( &m_Controllers[i].nMaxNumParticles,sizeof(UINT),1,pFile );
        fwrite( &m_Controllers[i].bEmitterLoop,sizeof(bool),1,pFile );
        fwrite( &m_Controllers[i].alphaMode,sizeof(AlphaMode),1,pFile );
    }

    fclose( pFile );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Reset the particle effect.
//------------------------------------------------------------------------
void FlyParticleEffect::Reset(void)
{
    size_t i = 0;
    for( ;i<m_Particles.size();i++ )
        SAFE_DELETE( m_Particles[i] );

    for( i=0;i<m_Emitters.size();i++ )
        SAFE_DELETE( m_Emitters[i] );

    // Delete the textures.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    for( i=0;i<m_Textures.size();i++ )
        pResMgr->DeleteTexture( m_Textures[i] );

    m_Particles.clear();
    m_Emitters.clear();
    m_Controllers.clear();
    m_Textures.clear();

    // Update the dirty state.
    m_nDirtyState++;
}


//------------------------------------------------------------------------
// Set the new texture for the particle template.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle template.
//      const char*:    Path to the texture file.
//------------------------------------------------------------------------
void FlyParticleEffect::SetParticleTexture( const char* cName,const char* cTexName )
{
    size_t nParIndex = -1;

    // Search the particle template.
    for( size_t i=0;i<m_Particles.size();i++ )
    {
        if( strcmp(m_Particles[i]->GetName(),cName) == 0 )
        {
            nParIndex = i;
            break;
        }
    }

    if( nParIndex == -1 ) return;

    // Change the particle's texture.
    m_Particles[nParIndex]->SetTexName( cTexName );
    m_Textures[nParIndex]->Load2DTexture( cTexName,1 );
}


//------------------------------------------------------------------------
// Get the name of the particle effect.
//------------------------------------------------------------------------
const char* FlyParticleEffect::GetName(void) const
{
    return m_sName.c_str();
}


//------------------------------------------------------------------------
// Set the name for the particle effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle effect.
//------------------------------------------------------------------------
void FlyParticleEffect::SetName( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Get the particle's texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the particle template.
//------------------------------------------------------------------------
FlyTexture* FlyParticleEffect::GetParticleTexture( UINT nIndex )
{
    if( nIndex >= m_Textures.size() )
        return NULL;

    return m_Textures[nIndex];
}


//------------------------------------------------------------------------
// Get the particle's texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle template.
//------------------------------------------------------------------------
FlyTexture* FlyParticleEffect::GetParticleTextureByName( const char* cParName )
{
    for( size_t i=0;i<m_Particles.size();i++ )
    {
        if( strcmp(m_Particles[i]->GetName(),cParName) == 0 )
            return m_Textures[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the pointer to the particle template.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the particle template.
//------------------------------------------------------------------------
FlyParticle* FlyParticleEffect::GetParticle( UINT nIndex )
{
    if( nIndex >= m_Particles.size() )
        return NULL;

    return m_Particles[nIndex];
}


//------------------------------------------------------------------------
// Get the pointer to the particle template.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle template.
//------------------------------------------------------------------------
FlyParticle* FlyParticleEffect::GetParticleByName( const char* cName )
{
    for( size_t i=0;i<m_Particles.size();i++ )
    {
        if( strcmp(cName,m_Particles[i]->GetName()) == 0 )
            return m_Particles[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the pointer to the emitter template.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the emitter template.
//------------------------------------------------------------------------
FlyParticleEmitter* FlyParticleEffect::GetEmitter( UINT nIndex )
{
    if( nIndex >= m_Emitters.size() )
        return NULL;

    return m_Emitters[nIndex];
}


//------------------------------------------------------------------------
// Get the pointer to the emitter template.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the emitter template.
//------------------------------------------------------------------------
FlyParticleEmitter* FlyParticleEffect::GetEmitterByName( const char* cName )
{
    for( size_t i=0;i<m_Emitters.size();i++ )
    {
        if( strcmp(cName,m_Emitters[i]->GetName()) == 0 )
            return m_Emitters[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the pointer to the particle controller.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the particle controller.
//------------------------------------------------------------------------
FlyParticleEffect::sController* FlyParticleEffect::GetController( UINT nIndex )
{
    if( nIndex >= m_Controllers.size() )
        return NULL;

    return &m_Controllers[nIndex];
}


//------------------------------------------------------------------------
// Get the pointer to the particle controller.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the particle controller.
//------------------------------------------------------------------------
FlyParticleEffect::sController* FlyParticleEffect::GetControllerByName( const char* cName )
{
    for( size_t i=0;i<m_Controllers.size();i++ )
    {
        if( m_Controllers[i].sName == cName )
            return &m_Controllers[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the count of the particle template.
//------------------------------------------------------------------------
size_t FlyParticleEffect::GetNumParticles(void) const
{
    return m_Particles.size();
}

//------------------------------------------------------------------------
// Get the count of the emitter template.
//------------------------------------------------------------------------
size_t FlyParticleEffect::GetNumEmitters(void) const
{
    return m_Emitters.size();
}

//------------------------------------------------------------------------
// Get the count of the controller.
//------------------------------------------------------------------------
size_t FlyParticleEffect::GetNumControllers(void) const
{
    return m_Controllers.size();
}


//------------------------------------------------------------------------
// Get the dirty state of the particle effect.
//------------------------------------------------------------------------
int FlyParticleEffect::GetDirtyState(void)
{
    return m_nDirtyState;
}