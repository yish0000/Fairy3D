//------------------------------------------------------------------------
// Name: etSprite.cpp
// Desc: This file is to implement the class etSprite.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etSprite.h"
#include "etSpriteObject.h"

#include "etSceneManager.h"
#include "etCoreManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etSprite::etSprite( const char* cName,const char* cTemName )
{
    std::string sNodeName;
    m_Type = ENTITY_SPRITE;
    m_sTemName = cTemName;

    // Get the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    // Create the sprite entity.
    m_pObject = pSceneMgr->CreateSceneObject( cName,"sprite" );

    // Create the sceneNode.
    FlySceneNode* pEntityRoot = pSceneMgr->GetSceneNode( "et-Entity" );
    sNodeName = cName;
    sNodeName += "_node";
    m_pNode = pEntityRoot->CreateChildNode( sNodeName.c_str() );
    m_pNode->AttachObject( m_pObject );

    // Start to play the sprite object.
    ((etSpriteObject*)m_pObject)->Play();
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etSprite::~etSprite(void)
{
    // Get the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    // Destroy the sceneNode.
    pSceneMgr->DestroySceneNode( m_pNode,true );

    // Destroy the sceneObject.
    pSceneMgr->DestroySceneObject( m_pObject );
}


//------------------------------------------------------------------------
// Get the name of the template.
//------------------------------------------------------------------------
const char* etSprite::GetTemplateName(void) const
{
    return m_sTemName.c_str();
}