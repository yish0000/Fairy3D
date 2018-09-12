//------------------------------------------------------------------------
// Name: etMesh.cpp
// Desc: This file is to implement the class etMesh.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etMesh.h"

#include "etSceneManager.h"
#include "etCoreManager.h"

#include "FlyMeshEntity.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etMesh::etMesh( const char* cName,const char* cMeshName )
{
    std::string sNodeName;
    m_Type = ENTITY_MESH;

    // Get the sceneManager.
    etSceneManager* pSceneMgr = etCoreManager::Instance().GetSceneManager();

    // Create the mesh entity.
    m_pObject = pSceneMgr->CreateMeshEntity( cName,cMeshName );

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
etMesh::~etMesh(void)
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
const char* etMesh::GetTemplateName(void) const
{
    return ((FlyMeshEntity*)m_pObject)->GetMeshTemplate()->GetName();
}