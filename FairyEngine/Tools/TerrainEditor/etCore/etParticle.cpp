//------------------------------------------------------------------------
// Name: etParticle.cpp
// Desc: This file is to implement the class etParticle.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etParticle.h"

#include "etSceneManager.h"
#include "etCoreManager.h"

#include "FlyParticleEntity.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etParticle::etParticle( const char* cName,const char* cParTemplate )
{
    std::string sNodeName;
    m_Type = ENTITY_PARTICLE;

    // Get the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    // Create the mesh entity.
    m_pObject = (FlySceneObject*)pSceneMgr->CreateParticleEntity( cName,cParTemplate );

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
etParticle::~etParticle(void)
{
    // Get the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    // Destroy the sceneNode.
    pSceneMgr->DestroySceneNode( m_pNode,true );

    // Destroy the sceneObject.
    pSceneMgr->DestroySceneObject( m_pObject );
}


//------------------------------------------------------------------------
// Get the template name of the particle entity.
//------------------------------------------------------------------------
const char* etParticle::GetTemplateName(void) const
{
    return ((FlyParticleEntity*)m_pObject)->GetParticleEffect()->GetName();
}