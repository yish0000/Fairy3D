//------------------------------------------------------------------------
// Name: etEntity.cpp
// Desc: This file is to implement the class etEntity.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etEntity.h"


UINT etEntity::s_nEntityID = 0;

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etEntity::etEntity(void)
{
    m_pObject = NULL;
    m_pNode = NULL;

    m_bSelected = false;
    m_bHide = false;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etEntity::~etEntity(void)
{
}


//------------------------------------------------------------------------
// Select the entity for the editor.
//------------------------------------------------------------------------
void etEntity::Select(void)
{
    m_pNode->ShowBoundingBox( true );
    m_bSelected = true;
}

//------------------------------------------------------------------------
// Deselect the entity for the editor.
//------------------------------------------------------------------------
void etEntity::DeSelect(void)
{
    m_pNode->ShowBoundingBox( false );
    m_bSelected = false;
}


//------------------------------------------------------------------------
// Hide the the entity for the editor.
//------------------------------------------------------------------------
void etEntity::Hide(void)
{
    m_pObject->SetVisible( false );
    m_bHide = true;
}

//------------------------------------------------------------------------
// Dehide the entity for the editor.
//------------------------------------------------------------------------
void etEntity::DeHide(void)
{
    m_pObject->SetVisible( true );
    m_bHide = false;
}


//------------------------------------------------------------------------
// Translate the entity.
//------------------------------------------------------------------------
void etEntity::Translate( const FlyVector& vTrans )
{
    m_pNode->Translate( vTrans,false );
}

//------------------------------------------------------------------------
// Set the position of the entity.
//------------------------------------------------------------------------
void etEntity::SetPosition( const FlyVector& vPos )
{
    m_pNode->SetPosition( vPos );
}

//------------------------------------------------------------------------
// Rotate the entity around the x- axis.
//------------------------------------------------------------------------
void etEntity::RotateX( float fValue )
{
    m_pNode->Pitch( fValue );
}

//------------------------------------------------------------------------
// Rotate the entity around the y- axis.
//------------------------------------------------------------------------
void etEntity::RotateY( float fValue )
{
    m_pNode->Yaw( fValue );
}

//------------------------------------------------------------------------
// Rotate the entity around the z- axis.
//------------------------------------------------------------------------
void etEntity::RotateZ( float fValue )
{
    m_pNode->Roll( fValue );
}

//------------------------------------------------------------------------
// Scale the entity for the editor.
//------------------------------------------------------------------------
void etEntity::Scale( float fX,float fY,float fZ )
{
    m_pNode->Scale( fX,fY,fZ );
}

//------------------------------------------------------------------------
// Set the scale for the editor.
//------------------------------------------------------------------------
void etEntity::SetScale( const FlyVector& vScale )
{
    m_pNode->SetScale( vScale );
}


//------------------------------------------------------------------------
// Get the position of the entity.
//------------------------------------------------------------------------
const FlyVector& etEntity::GetPosition(void) const
{
    return m_pNode->GetPosition();
}

//------------------------------------------------------------------------
// Get the orientation of the entity.
//------------------------------------------------------------------------
const FlyVector& etEntity::GetOrientation(void) const
{
    static FlyVector vRotation;
    m_pNode->GetOrientation().GetEulers( &vRotation.x,&vRotation.y,&vRotation.z );
    return vRotation;
}

//------------------------------------------------------------------------
// Set the orientation for the entity.
//------------------------------------------------------------------------
void etEntity::SetOrientation( const FlyVector& vOrientation )
{
    FlyQuat quat;
    quat.MakeFromEuler( vOrientation.x,vOrientation.y,vOrientation.z );
    m_pNode->SetOrientation( quat );
}

//------------------------------------------------------------------------
// Get the scale of the entity.
//------------------------------------------------------------------------
const FlyVector& etEntity::GetScale(void) const
{
    return m_pNode->GetScale();
}


//------------------------------------------------------------------------
// Get the name of the entity.
//------------------------------------------------------------------------
const char* etEntity::GetName(void) const
{
    return m_pObject->GetName();
}

//------------------------------------------------------------------------
// Get the type of the entity.
//------------------------------------------------------------------------
ENTITYTYPE etEntity::GetEntityType(void) const
{
    return m_Type;
}

//------------------------------------------------------------------------
// Get the boundingBox of the entity.
//------------------------------------------------------------------------
const FlyAabb& etEntity::GetBoundingBox(void) const
{
    return m_pObject->GetWorldBoundingBox();
}


//------------------------------------------------------------------------
// Set the new name for the entity.
//------------------------------------------------------------------------
void etEntity::SetName( const char* cName )
{
    m_pObject->SetName( cName );
}