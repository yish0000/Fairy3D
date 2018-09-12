//------------------------------------------------------------------------
// Name: etAnimation.cpp
// Desc: This file is to implement the class etAnimation.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etAnimation.h"

#include "etSceneManager.h"
#include "etCoreManager.h"

#include "FlyAnimEntity.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etAnimation::etAnimation( const char* cName,const char* cAnimName )
{
    std::string sNodeName;
    m_Type = ENTITY_ANIMATION;

    // Get the pointer to the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    // Create the animation entity.
    m_pObject = (FlySceneObject*)pSceneMgr->CreateAnimation( cName,cAnimName );

    // Create the sceneNode.
    FlySceneNode* pEntityRoot = pSceneMgr->GetSceneNode( "et-Entity" );
    sNodeName = cName;
    sNodeName += "_node";
    m_pNode = pEntityRoot->CreateChildNode( sNodeName.c_str() );
    m_pNode->AttachObject( m_pObject );

    FlyAnimEntity* pEntity = (FlyAnimEntity*)m_pObject;
    pEntity->SetAnimation( 0 );
    pEntity->SetAnimFPS( 10.0f );
    pEntity->SetAnimLoop( true );
    pEntity->Play();
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etAnimation::~etAnimation(void)
{
    // Get the pointer to the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    pSceneMgr->DestroySceneNode( m_pNode,true );
    pSceneMgr->DestroySceneObject( m_pObject );
}


//------------------------------------------------------------------------
// Get the name of the template.
//------------------------------------------------------------------------
const char* etAnimation::GetTemplateName(void) const
{
    return ((FlyAnimEntity*)m_pObject)->GetAnimation()->GetName();
}


//------------------------------------------------------------------------
// Set the FPS for the animation.
//------------------------------------------------------------------------
void etAnimation::SetAnimFPS( float fFPS )
{
    ((FlyAnimEntity*)m_pObject)->SetAnimFPS( fFPS );
}