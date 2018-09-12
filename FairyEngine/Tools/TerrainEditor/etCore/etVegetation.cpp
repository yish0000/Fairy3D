//------------------------------------------------------------------------
// Name: etVegetation.cpp
// Desc: This file is to implement the class etVegetation.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etVegetation.h"

#include "etSceneManager.h"
#include "etCoreManager.h"

#include "etVegetationObject.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etVegetation::etVegetation( const char* cName,const char* cVegetationName )
{
    std::string sNodeName;
    m_Type = ENTITY_VEGETATION;
    m_sTemName = cVegetationName;

    // Get the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    // Create the vegetation entity.
    m_pObject = pSceneMgr->CreateSceneObject( cName,"vegetation" );

    // Create the sceneNode.
    FlySceneNode* pEntityRoot = pSceneMgr->GetSceneNode( "et-Entity" );
    sNodeName = cName;
    sNodeName += "_node";
    m_pNode = pEntityRoot->CreateChildNode( sNodeName.c_str() );
    m_pNode->AttachObject( m_pObject );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etVegetation::~etVegetation(void)
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
const char* etVegetation::GetTemplateName(void) const
{
    return m_sTemName.c_str();
}