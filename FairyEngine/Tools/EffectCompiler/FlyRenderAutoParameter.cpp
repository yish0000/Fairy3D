//------------------------------------------------------------------------
// Name: FlyRenderAutoParameter.cpp
// Desc: This file is to implement the class FlyRenderAutoParameter.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "FlyRenderAutoParameter.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyRenderAutoParameter::FlyRenderAutoParameter(void)
{
    m_bMatrixDirty = true;

    m_LightList.resize( 8,NULL );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyRenderAutoParameter::~FlyRenderAutoParameter(void)
{
}


//------------------------------------------------------------------------
// Get the product of the three matrix.
//------------------------------------------------------------------------
const FlyMatrix& FlyRenderAutoParameter::GetWorldViewProjMatrix(void)
{
    if( m_bMatrixDirty )
    {
        m_mWorldViewProj = m_mWorld * m_mView;
        m_mWorldViewProj = m_mWorldViewProj * m_mProjection;
        m_mWorldViewProjT.TransposeOf( m_mWorldViewProj );
        m_bMatrixDirty = false;
    }

    return m_mWorldViewProj;
}


//------------------------------------------------------------------------
// Get the product of the three matrix, transpose it.
//------------------------------------------------------------------------
const FlyMatrix& FlyRenderAutoParameter::GetWorldViewProjMatrixT(void)
{
    if( m_bMatrixDirty )
    {
        m_mWorldViewProj = m_mWorld * m_mView;
        m_mWorldViewProj = m_mWorldViewProj * m_mProjection;
        m_mWorldViewProjT.TransposeOf( m_mWorldViewProj );
        m_bMatrixDirty = false;
    }

    return m_mWorldViewProjT;
}


//------------------------------------------------------------------------
// Set the current world matrix.
//------------------------------------------------------------------------
void FlyRenderAutoParameter::SetWorldMatrix( const FlyMatrix& mWorld )
{
    m_mWorld = mWorld;

    m_bMatrixDirty = true;
}


//------------------------------------------------------------------------
// Set the current view matrix.
//------------------------------------------------------------------------
void FlyRenderAutoParameter::SetViewMatrix( const FlyMatrix& mView )
{
    m_mView = mView;

    m_bMatrixDirty = true;
}


//------------------------------------------------------------------------
// Set the current projection matrix.
//------------------------------------------------------------------------
void FlyRenderAutoParameter::SetProjectionMatrix( const FlyMatrix& mProjection )
{
    m_mProjection = mProjection;

    m_bMatrixDirty = true;
}


//------------------------------------------------------------------------
// Set the light object.
//------------------------------------------------------------------------
void FlyRenderAutoParameter::SetLightObject( int nIndex,FlyLightObject* pObject )
{
    m_LightList[nIndex] = pObject;
}