//------------------------------------------------------------------------
// Name: FlyParticle.cpp
// Desc: This file is to implement the class FlyParticle.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyParticle.h"


//------------------------------------------------------------------------
// Constructor and destructor of the class
//------------------------------------------------------------------------
FlyParticle::FlyParticle(void)
{
    m_sName = "Unnamed Particle";
    m_sTexName = "";
    m_nNumAnims = 0;
    m_nNumFrames = 0;
    m_fGravity = 9.81f;
    m_fResistValue = 0.0f;
    m_fMinSpeedResist = 0.0f;
    m_bUseResist = false;

    // Initialize the keyframe data.
    m_KFSize.resize( 2 );
    m_KFSize[0].fTime = 0.0f;
    m_KFSize[0].fSize = 1.0f;
    m_KFSize[1].fTime = 1.0f;
    m_KFSize[1].fSize = 1.0f;

    m_KFAngle.resize( 2 );
    m_KFAngle[0].fTime = 0.0f;
    m_KFAngle[0].fAngle = 0.0f;
    m_KFAngle[1].fTime = 1.0f;
    m_KFAngle[1].fAngle = 0.0f;

    m_KFAlpha.resize( 2 );
    m_KFAlpha[0].fTime = 0.0f;
    m_KFAlpha[0].fAlpha = 1.0f;
    m_KFAlpha[1].fTime = 1.0f;
    m_KFAlpha[1].fAlpha = 1.0f;

    m_KFColor.resize( 2 );
    m_KFColor[0].fTime = 0.0f;
    m_KFColor[0].color.fR = 1.0f;
    m_KFColor[0].color.fG = 1.0f;
    m_KFColor[0].color.fB = 1.0f;
    m_KFColor[0].color.fA = 1.0f;
    m_KFColor[1].fTime = 1.0f;
    m_KFColor[1].color.fR = 1.0f;
    m_KFColor[1].color.fG = 1.0f;
    m_KFColor[1].color.fB = 1.0f;
    m_KFColor[1].color.fA = 1.0f;
}

FlyParticle::~FlyParticle(void)
{
}


//------------------------------------------------------------------------
// Add a new keyframe into the size array. If the new keyframe's time
// has existed, Then cover the old one.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Time of the keyframe.
//      float:      Size of the keyframe.
//------------------------------------------------------------------------
bool FlyParticle::AddKFSize( float fTime,float fSize )
{
    float fT1,fT2;
    sKeyframe_Size kfSize;
    KFSizeList::iterator itr;

    if( fTime < 0.0f || fTime > 1.0f )
        return false;

    if( m_KFSize.size() == 0 )
    {
        kfSize.fTime = fTime;
        kfSize.fSize = fSize;
        m_KFSize.push_back( kfSize );
        return true;
    }
    else if( m_KFSize.size() == 1 )
    {
        kfSize.fTime = fTime;
        kfSize.fSize = fSize;

        if( fTime > m_KFSize[0].fTime )
            m_KFSize.push_back( kfSize );
        else
            m_KFSize.insert( m_KFSize.begin(),kfSize );

        return true;
    }

    // Loop through all the keyframe.
    // search the position for new keyframe.
    for( size_t i=0;i<m_KFSize.size()-1;i++ )
    {
        fT1 = m_KFSize[i].fTime;
        fT2 = m_KFSize[i+1].fTime;

        if( fTime == fT1 )
        {
            m_KFSize[i].fTime = fTime;
            m_KFSize[i].fSize = fSize;
            return true;
        }
        else if( fTime == fT2 )
        {
            m_KFSize[i+1].fTime = fTime;
            m_KFSize[i+1].fSize = fSize;
            return true;
        }
        else if( fTime > fT1 && fTime < fT2 )
        {
            itr = m_KFSize.begin() + i + 1;
            kfSize.fTime = fTime;
            kfSize.fSize = fSize;
            m_KFSize.insert( itr,kfSize );
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Add a keyframe into the rotation angle array.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Time of the keyframe.
//      float:      Rotation angle value.
//------------------------------------------------------------------------
bool FlyParticle::AddKFAngle( float fTime,float fAngle )
{
    float fT1,fT2;
    sKeyframe_Angle kfAngle;
    KFAngleList::iterator itr;

    if( fTime < 0.0f || fTime > 1.0f )
        return false;

    if( m_KFAngle.size() == 0 )
    {
        kfAngle.fTime = fTime;
        kfAngle.fAngle = fAngle;
        m_KFAngle.push_back( kfAngle );
        return true;
    }
    else if( m_KFAngle.size() == 1 )
    {
        kfAngle.fTime = fTime;
        kfAngle.fAngle = fAngle;

        if( fTime > m_KFAngle[0].fTime )
            m_KFAngle.push_back( kfAngle );
        else
            m_KFAngle.insert( m_KFAngle.begin(),kfAngle );

        return true;
    }

    // Loop through all the keyframe.
    // search the position for new keyframe.
    for( size_t i=0;i<m_KFAngle.size()-1;i++ )
    {
        fT1 = m_KFAngle[i].fTime;
        fT2 = m_KFAngle[i+1].fTime;

        if( fTime == fT1 )
        {
            m_KFAngle[i].fTime = fTime;
            m_KFAngle[i].fAngle = fAngle;
            return true;
        }
        else if( fTime == fT2 )
        {
            m_KFAngle[i+1].fTime = fTime;
            m_KFAngle[i+1].fAngle = fAngle;
            return true;
        }
        else if( fTime > fT1 && fTime < fT2 )
        {
            itr = m_KFAngle.begin() + i + 1;
            kfAngle.fTime = fTime;
            kfAngle.fAngle = fAngle;
            m_KFAngle.insert( itr,kfAngle );
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Add a keyframe into the alpha array.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Time of the keyframe.
//      float:      Alpha value of the keyframe.
//------------------------------------------------------------------------
bool FlyParticle::AddKFAlpha( float fTime,float fAlpha )
{
    float fT1,fT2;
    sKeyframe_Alpha kfAlpha;
    KFAlphaList::iterator itr;

    if( fTime < 0.0f || fTime > 1.0f )
        return false;

    if( m_KFAlpha.size() == 0 )
    {
        kfAlpha.fTime = fTime;
        kfAlpha.fAlpha = fAlpha;
        m_KFAlpha.push_back( kfAlpha );
        return true;
    }
    else if( m_KFAlpha.size() == 1 )
    {
        kfAlpha.fTime = fTime;
        kfAlpha.fAlpha = fAlpha;

        if( fTime > m_KFAlpha[0].fTime )
            m_KFAlpha.push_back( kfAlpha );
        else
            m_KFAlpha.insert( m_KFAlpha.begin(),kfAlpha );

        return true;
    }

    // Loop through all the keyframe.
    // search the position for new keyframe.
    for( size_t i=0;i<m_KFAlpha.size()-1;i++ )
    {
        fT1 = m_KFAlpha[i].fTime;
        fT2 = m_KFAlpha[i+1].fTime;

        if( fTime == fT1 )
        {
            m_KFAlpha[i].fTime = fTime;
            m_KFAlpha[i].fAlpha = fAlpha;
            return true;
        }
        else if( fTime == fT2 )
        {
            m_KFAlpha[i+1].fTime = fTime;
            m_KFAlpha[i+1].fAlpha = fAlpha;
            return true;
        }
        else if( fTime > fT1 && fTime < fT2 )
        {
            itr = m_KFAlpha.begin() + i + 1;
            kfAlpha.fTime = fTime;
            kfAlpha.fAlpha = fAlpha;
            m_KFAlpha.insert( itr,kfAlpha );
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Add a keyframe into the color array.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Time of the keyframe.
//      FLYCOLOR:   Color of the keyframe.
//------------------------------------------------------------------------
bool FlyParticle::AddKFColor( float fTime,FLYCOLOR color )
{
    float fT1,fT2;
    sKeyframe_Color kfColor;
    KFColorList::iterator itr;

    if( fTime < 0.0f || fTime > 1.0f )
        return false;

    if( m_KFColor.size() == 0 )
    {
        kfColor.fTime = fTime;
        kfColor.color = color;
        m_KFColor.push_back( kfColor );
        return true;
    }
    else if( m_KFColor.size() == 1 )
    {
        kfColor.fTime = fTime;
        kfColor.color = color;

        if( fTime > m_KFColor[0].fTime )
            m_KFColor.push_back( kfColor );
        else
            m_KFColor.insert( m_KFColor.begin(),kfColor );

        return true;
    }

    // Loop through all the keyframe.
    // search the position for new keyframe.
    for( size_t i=0;i<m_KFColor.size()-1;i++ )
    {
        fT1 = m_KFColor[i].fTime;
        fT2 = m_KFColor[i+1].fTime;

        if( fTime == fT1 )
        {
            m_KFColor[i].fTime = fTime;
            m_KFColor[i].color = color;
            return true;
        }
        else if( fTime == fT2 )
        {
            m_KFColor[i+1].fTime = fTime;
            m_KFColor[i+1].color = color;
            return true;
        }
        else if( fTime > fT1 && fTime < fT2 )
        {
            itr = m_KFColor.begin() + i + 1;
            kfColor.fTime = fTime;
            kfColor.color = color;
            m_KFColor.insert( itr,kfColor );
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------------------
// Modify the keyframe of size.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the keyframe.
//      float:      Time of this keyframe.
//      float:      Size of the keyframe.
//------------------------------------------------------------------------
void FlyParticle::ModifyKFSize( int nIndex,float fTime,float fSize )
{
    m_KFSize[nIndex].fTime = fTime;
    m_KFSize[nIndex].fSize = fSize;
}


//------------------------------------------------------------------------
// Modify the keyframe of rotation angle.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the keyframe.
//      float:      Time of this keyframe.
//      float:      Angle value of the keyframe.
//------------------------------------------------------------------------
void FlyParticle::ModifyKFAngle( int nIndex,float fTime,float fAngle )
{
    m_KFAngle[nIndex].fTime = fTime;
    m_KFAngle[nIndex].fAngle = fAngle;
}


//------------------------------------------------------------------------
// Modify the keyframe of alpha value.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the keyframe.
//      float:      Time of this keyframe.
//      float:      Alpha value of this keyframe.
//------------------------------------------------------------------------
void FlyParticle::ModifyKFAlpha( int nIndex,float fTime,float fAlpha )
{
    m_KFAlpha[nIndex].fTime = fTime;
    m_KFAlpha[nIndex].fAlpha = fAlpha;
}


//------------------------------------------------------------------------
// Modify the keyframe of color value.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the keyframe.
//      float:      Time of this keyframe.
//      FLYCOLOR:   Color value of this keyframe.
//------------------------------------------------------------------------
void FlyParticle::ModifyKFColor( int nIndex,float fTime,FLYCOLOR color )
{
    m_KFColor[nIndex].fTime = fTime;
    m_KFColor[nIndex].color.fR = color.fR;
    m_KFColor[nIndex].color.fG = color.fG;
    m_KFColor[nIndex].color.fB = color.fB;
}


//------------------------------------------------------------------------
// Update the particle's size with its age.
// ----------------------------------------------------------------------
// Param -> IN:
//      sParticle*:     Pointer to the particle.
//------------------------------------------------------------------------
void FlyParticle::UpdateSize( sParticle* pPar )
{
    float fT1,fT2;
    float fSize1,fSize2;

    float fTime = pPar->fAge / pPar->fLife;

    for( size_t i=0;i<m_KFSize.size()-1;i++ )
    {
        fT1 = m_KFSize[i].fTime;
        fT2 = m_KFSize[i+1].fTime;

        if( fTime >= fT1 && fTime <= fT2 )
        {
            fSize1 = m_KFSize[i].fSize;
            fSize2 = m_KFSize[i+1].fSize;
            break;
        }
    }

    pPar->fSize = fSize1 + (fSize2-fSize1)/(fT2-fT1)*(fTime-fT1);
}


//------------------------------------------------------------------------
// Update the particle's rotation angle with its age.
// ----------------------------------------------------------------------
// Param -> IN:
//      sParticle*:     Pointer to the particle.
//------------------------------------------------------------------------
void FlyParticle::UpdateAngle( sParticle* pPar )
{
    float fT1,fT2;
    float fAngle1,fAngle2;

    float fTime = pPar->fAge / pPar->fLife;

    for( size_t i=0;i<m_KFAngle.size()-1;i++ )
    {
        fT1 = m_KFAngle[i].fTime;
        fT2 = m_KFAngle[i+1].fTime;

        if( fTime >= fT1 && fTime <= fT2 )
        {
            fAngle1 = m_KFAngle[i].fAngle;
            fAngle2 = m_KFAngle[i+1].fAngle;
            break;
        }
    }

    pPar->fAngle = fAngle1 + (fAngle2-fAngle1)/(fT2-fT1)*(fTime-fT1);
}


//------------------------------------------------------------------------
// Update the particle's alpha value with its age.
// ----------------------------------------------------------------------
// Param -> IN:
//      sParticle*:     Pointer to the particle.
//------------------------------------------------------------------------
void FlyParticle::UpdateAlpha( sParticle* pPar )
{
    float fT1,fT2;
    float fAlpha1,fAlpha2;

    float fTime = pPar->fAge / pPar->fLife;

    for( size_t i=0;i<m_KFAlpha.size()-1;i++ )
    {
        fT1 = m_KFAlpha[i].fTime;
        fT2 = m_KFAlpha[i+1].fTime;

        if( fTime >= fT1 && fTime <= fT2 )
        {
            fAlpha1 = m_KFAlpha[i].fAlpha;
            fAlpha2 = m_KFAlpha[i+1].fAlpha;
            break;
        }
    }

    pPar->cColor.fA = fAlpha1 + (fAlpha2-fAlpha1)/(fT2-fT1)*(fTime-fT1);
}


//------------------------------------------------------------------------
// Update the particle's color with its age.
// ----------------------------------------------------------------------
// Param -> IN:
//      sParticle*:     Pointer to the particle.
//------------------------------------------------------------------------
void FlyParticle::UpdateColor( sParticle* pPar )
{
    float fT1,fT2;
    FLYCOLOR color1,color2;

    float fTime = pPar->fAge / pPar->fLife;

    for( size_t i=0;i<m_KFColor.size()-1;i++ )
    {
        fT1 = m_KFColor[i].fTime;
        fT2 = m_KFColor[i+1].fTime;

        if( fTime >= fT1 && fTime <= fT2 )
        {
            color1 = m_KFColor[i].color;
            color2 = m_KFColor[i+1].color;
            break;
        }
    }

    pPar->cColor.fR = color1.fR + (color2.fR-color1.fR)/(fT2-fT1)*(fTime-fT1);
    pPar->cColor.fG = color1.fG + (color2.fG-color1.fG)/(fT2-fT1)*(fTime-fT1);
    pPar->cColor.fB = color1.fB + (color2.fB-color1.fB)/(fT2-fT1)*(fTime-fT1);
}


//------------------------------------------------------------------------
// Update the particle's state. If the particle is alive return true,
// if it dies return false, So controller can delete it.
// ----------------------------------------------------------------------
// Param -> IN:
//      sParticle*:     Pointer to the particle.
//------------------------------------------------------------------------
bool FlyParticle::Update( sParticle* pPar,float fElapsed )
{
    float fResist;
    FlyVector vAccelFinal;
    FlyVector vResist;
    float fVel;

    vAccelFinal = pPar->vAccel;
    vAccelFinal += FlyVector( 0.0f,-m_fGravity,0.0f );

    // Calculate the Air resistance.
    fVel = pPar->vVelocity.GetLength();

    if( m_bUseResist && fVel > m_fMinSpeedResist )
    {
        vResist = pPar->vVelocity;
        fResist = m_fResistValue * pPar->fSize * fVel;
        vResist.Normalize();
        vResist.Negate();
        vResist *= fResist;
        vAccelFinal += vResist;
    }

    // Calculate the new position.
    pPar->vPos += pPar->vVelocity*fElapsed+(vAccelFinal*fElapsed*fElapsed)*0.5f;

    // Calculate the new velocity.
    pPar->vVelocity += vAccelFinal*fElapsed;

    // Update the age.
    pPar->fAge += fElapsed;

    // Update the animation for the particle.
    pPar->nFrame = (int)((pPar->fAge / pPar->fLife) * m_nNumFrames);

    // Update the particle's state.
    UpdateSize( pPar );
    UpdateAngle( pPar );
    UpdateAlpha( pPar );
    UpdateColor( pPar );

    return !(pPar->fAge >= pPar->fLife);
}


//------------------------------------------------------------------------
// Get the name of the particle template.
//------------------------------------------------------------------------
const char* FlyParticle::GetName(void) const
{
    return m_sName.c_str();
}

//------------------------------------------------------------------------
// Get the texture path of the particle template.
//------------------------------------------------------------------------
const char* FlyParticle::GetTexName(void) const
{
    return m_sTexName.c_str();
}

//------------------------------------------------------------------------
// Get the count of the animations.
//------------------------------------------------------------------------
int FlyParticle::GetNumAnims(void) const
{
    return m_nNumAnims;
}

//------------------------------------------------------------------------
// Get the count of frames.
//------------------------------------------------------------------------
int FlyParticle::GetNumFrames(void) const
{
    return m_nNumFrames;
}

//------------------------------------------------------------------------
// Get the gravity value of the particle template.
//------------------------------------------------------------------------
float FlyParticle::GetGravity(void) const
{
    return m_fGravity;
}

//------------------------------------------------------------------------
// Get the air resist coefficient of the particle template.
//------------------------------------------------------------------------
float FlyParticle::GetResistValue(void) const
{
    return m_fResistValue;
}

//------------------------------------------------------------------------
// Get the minimum speed that can cause the air resistance.
//------------------------------------------------------------------------
float FlyParticle::GetMinSpeedResist(void) const
{
    return m_fMinSpeedResist;
}

//------------------------------------------------------------------------
// Do we use the air resistance for particles ?
//------------------------------------------------------------------------
bool FlyParticle::IsUseResist(void) const
{
    return m_bUseResist;
}


//------------------------------------------------------------------------
// Set the name of the particle template.
//------------------------------------------------------------------------
void FlyParticle::SetName( const char* cName )
{
    m_sName = cName;
}

//------------------------------------------------------------------------
// Set the path of the texture file.
//------------------------------------------------------------------------
void FlyParticle::SetTexName( const char* cTexName )
{
    m_sTexName = cTexName;
}

//------------------------------------------------------------------------
// Set the count of animations for particle texture.
//------------------------------------------------------------------------
void FlyParticle::SetNumAnims( int nNumAnims )
{
    m_nNumAnims = nNumAnims;
}

//------------------------------------------------------------------------
// Set the count of frames for particle texture.
//------------------------------------------------------------------------
void FlyParticle::SetNumFrames( int nNumFrames )
{
    m_nNumFrames = nNumFrames;
}

//------------------------------------------------------------------------
// Set the gravity value for particle template.
//------------------------------------------------------------------------
void FlyParticle::SetGravity( float fGravity )
{
    m_fGravity = fGravity;
}

//------------------------------------------------------------------------
// Set the air resistance coefficient for particle template.
//------------------------------------------------------------------------
void FlyParticle::SetResistValue( float fResistValue )
{
    m_fResistValue = fResistValue;
}

//------------------------------------------------------------------------
// Set the minimum speed that can cause the air resistance.
//------------------------------------------------------------------------
void FlyParticle::SetMinSpeedResist( float fValue )
{
    m_fMinSpeedResist = fValue;
}

//------------------------------------------------------------------------
// Open or close the air resistance for particle template.
//------------------------------------------------------------------------
void FlyParticle::UseAirResist( bool bUseResist )
{
    m_bUseResist = bUseResist;
}