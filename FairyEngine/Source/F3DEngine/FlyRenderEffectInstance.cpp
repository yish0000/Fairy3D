//------------------------------------------------------------------------
// Name: FlyRenderEffectInstance.cpp
// Desc: This file is to implement the class FlyRenderEffectInstance.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyRenderEffectInstance.h"


//------------------------------------------------------------------------
// Constructor of the class.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderEffect*:   Pointer to the renderEffect template.
//------------------------------------------------------------------------
FlyRenderEffectInstance::FlyRenderEffectInstance( FlyRenderEffect* pRenderEffect )
{
    m_pEffect = pRenderEffect;

    // Build the parameter list.
    for( UINT i=0;i<m_pEffect->GetNumParameters();i++ )
        m_Parameters.push_back( m_pEffect->GetParameterByIndex(i)->Clone() );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyRenderEffectInstance::~FlyRenderEffectInstance(void)
{
    // Clean the parameter list.
    ParameterList::iterator itr;
    for( itr=m_Parameters.begin();itr!=m_Parameters.end();itr++ )
        SAFE_DELETE( *itr );
}


//------------------------------------------------------------------------
// Update the parameters for the GPU program.
//------------------------------------------------------------------------
void FlyRenderEffectInstance::UpdateParameters( FlyRenderAutoParameter* pAutoParameter )
{
    for( size_t i=0;i<m_Parameters.size();i++ )
    {
        FlyRenderParameter* param = m_Parameters[i];

        if( param->IsAutoUpdateParameter() )
        {
            switch( param->GetAutoUpdateType() )
            {
            case APT_WORLDMATRIX:
                *param = pAutoParameter->GetWorldMatrix();
                break;
            case APT_VIEWMATRIX:
                *param = pAutoParameter->GetViewMatrix();
                break;
            case APT_PROJECTIONMATRIX:
                *param = pAutoParameter->GetProjectionMatrix();
                break;
            case APT_WORLDVIEWPROJMATRIX:
                *param = pAutoParameter->GetWorldViewProjMatrixT();
                break;
            case APT_WORLDMATRIXARRAY:
                break;
            case APT_AMBIENTCOLOR:
                {
                    FlyVector color;
                    memcpy( &color,&pAutoParameter->GetAmbientColor(),sizeof(FlyVector) );
                    *param = color;
                }
                break;
            }
        }

        m_pEffect->SetParameterData( i,m_Parameters[i] );
    }
}


//------------------------------------------------------------------------
// Get the specified parameter by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the parameter.
//------------------------------------------------------------------------
FlyRenderParameter* FlyRenderEffectInstance::GetParameterByName( const char* cName )
{
    ParameterList::iterator itr;
    for( itr=m_Parameters.begin();itr!=m_Parameters.end();itr++ )
    {
        if( strcmp((*itr)->GetName(),cName) == 0 )
            return *itr;
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the specified parameter by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the parameter.
//------------------------------------------------------------------------
FlyRenderParameter* FlyRenderEffectInstance::GetParameterByIndex( UINT nIndex )
{
    if( nIndex >= m_Parameters.size() )
        return NULL;

    return m_Parameters[nIndex];
}


//------------------------------------------------------------------------
// Get the specified parameter by its semantic.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Semantic of the parameter.
//------------------------------------------------------------------------
FlyRenderParameter* FlyRenderEffectInstance::GetParameterBySemantic( const char* cSemantic )
{
    ParameterList::iterator itr;
    for( itr=m_Parameters.begin();itr!=m_Parameters.end();itr++ )
    {
        if( strcmp((*itr)->GetSemantic(),cSemantic) == 0 )
            return *itr;
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the pointer to the render effect.
//------------------------------------------------------------------------
FlyRenderEffect* FlyRenderEffectInstance::GetRenderEffect(void) const
{
    return m_pEffect;
}


//------------------------------------------------------------------------
// Set the render effect template for the instance.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderEffect*:   Pointer to the renderEffect template.
//------------------------------------------------------------------------
void FlyRenderEffectInstance::SetRenderEffect( FlyRenderEffect* pEffect )
{
    if( m_pEffect != pEffect )
    {
        // Destroy the old parameter list.
        ParameterList::iterator itr;
        for( itr=m_Parameters.begin();itr!=m_Parameters.end();itr++ )
            SAFE_DELETE( *itr );
        m_Parameters.clear();

        // Build the parameter list.
        for( UINT i=0;i<pEffect->GetNumParameters();i++ )
            m_Parameters.push_back( pEffect->GetParameterByIndex(i)->Clone() );

        m_pEffect = pEffect;
    }
}