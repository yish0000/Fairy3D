//------------------------------------------------------------------------
// Name: FlyMeshManager.cpp
// Desc: This file is to implement the class FlyMeshManager.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyMeshManager.h"


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyMeshManager::FlyMeshManager(void)
{
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyMeshManager::~FlyMeshManager(void)
{
    MeshResourceList::iterator itr;
    for( itr=m_Meshes.begin();itr!=m_Meshes.end();itr++ )
        SAFE_DELETE( itr->pMesh );
}


//------------------------------------------------------------------------
// Destroy all the mesh resources.
//------------------------------------------------------------------------
void FlyMeshManager::DestroyAllMeshes(void)
{
    MeshResourceList::iterator itr;
    for( itr=m_Meshes.begin();itr!=m_Meshes.end();itr++ )
        SAFE_DELETE( itr->pMesh );

    m_Meshes.clear();
}


//------------------------------------------------------------------------
// Load a mesh from the model file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the model file.
//------------------------------------------------------------------------
FlyMesh* FlyMeshManager::LoadMesh( const char* cFilename )
{
    FlyMesh* pMesh;
    MeshNode node;

    // Check whether the mesh is loaded ?
    if( (pMesh = GetMeshByName(cFilename)) )
        return pMesh;

    // Create a new mesh.
    pMesh = new FlyMesh();
    if( !pMesh ) return NULL;

    // Load the mesh file.
    if( FAILED(pMesh->LoadMeshFromFile(cFilename)) )
    {
        delete pMesh;
        return NULL;
    }

    // Add the new mesh resource node.
    node.pMesh = pMesh;
    node.nReference = 0;
    m_Meshes.push_back( node );

    return pMesh;
}


//------------------------------------------------------------------------
// Create a null mesh manually.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the mesh.
//------------------------------------------------------------------------
FlyMesh* FlyMeshManager::CreateManualMesh( const char* cName )
{
    FlyMesh* pMesh;
    MeshNode node;

    // Check whether the mesh has existed ?
    if( (pMesh = GetMeshByName(cName)) )
        return pMesh;

    // Create a new mesh.
    pMesh = new FlyMesh();
    if( !pMesh ) return NULL;
    pMesh->SetName( cName );

    // Add the new mesh resource node.
    node.pMesh = pMesh;
    node.nReference = 0;
    m_Meshes.push_back( node );

    return pMesh;
}


//------------------------------------------------------------------------
// Get the pointer to the mesh by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the mesh.
//------------------------------------------------------------------------
FlyMesh* FlyMeshManager::GetMeshByIndex( UINT nIndex )
{
    if( nIndex >= m_Meshes.size() )
        return NULL;

    return m_Meshes[nIndex].pMesh;
}


//------------------------------------------------------------------------
// Get the pointer to the mesh by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the mesh resource.
//------------------------------------------------------------------------
FlyMesh* FlyMeshManager::GetMeshByName( const char* cName )
{
    MeshResourceList::iterator itr;
    for( itr=m_Meshes.begin();itr!=m_Meshes.end();itr++ )
    {
        if( strcmp(itr->pMesh->GetName(),cName) == 0 )
            return itr->pMesh;
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the count of the mesh resources.
//------------------------------------------------------------------------
size_t FlyMeshManager::GetNumMeshes(void) const
{
    return m_Meshes.size();
}


//------------------------------------------------------------------------
// Add the reference count for the mesh resource.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the mesh resource.
//------------------------------------------------------------------------
void FlyMeshManager::AddResourceRef( const char* cMeshName )
{
    MeshResourceList::iterator itr;
    for( itr=m_Meshes.begin();itr!=m_Meshes.end();itr++ )
    {
        if( strcmp(itr->pMesh->GetName(),cMeshName) == 0 )
        {
            itr->nReference++;
            break;
        }
    }
}


//------------------------------------------------------------------------
// Dec the reference count for the mesh resource.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the mesh resource.
//------------------------------------------------------------------------
void FlyMeshManager::DecResourceRef( const char* cMeshName )
{
    MeshResourceList::iterator itr;
    for( itr=m_Meshes.begin();itr!=m_Meshes.end();itr++ )
    {
        if( strcmp(itr->pMesh->GetName(),cMeshName) == 0 )
        {
            itr->nReference--;

            if( itr->nReference <= 0 )
            {
                SAFE_DELETE( itr->pMesh );
                m_Meshes.erase( itr );
            }

            break;
        }
    }
}


//------------------------------------------------------------------------
// Destroy a mesh resource by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:   Index of the mesh resource.
//------------------------------------------------------------------------
void FlyMeshManager::DestroyMeshByIndex( size_t nIndex )
{
    if( nIndex >= m_Meshes.size() )
        return;

    SAFE_DELETE( m_Meshes[nIndex].pMesh );
    m_Meshes.erase( m_Meshes.begin() + nIndex );
}


//------------------------------------------------------------------------
// Destroy a mesh resource by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the mesh.
//------------------------------------------------------------------------
void FlyMeshManager::DestroyMeshByName( const char* cName )
{
    MeshResourceList::iterator itr;
    for( itr=m_Meshes.begin();itr!=m_Meshes.end();itr++ )
    {
        if( strcmp(itr->pMesh->GetName(),cName) == 0 )
        {
            SAFE_DELETE( itr->pMesh );
            m_Meshes.erase( itr );
            break;
        }
    }
}


//------------------------------------------------------------------------
// Get the reference count of the resource.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the mesh resource.
//------------------------------------------------------------------------
int FlyMeshManager::GetRefCount( const char* cMeshName )
{
    MeshResourceList::iterator itr;
    for( itr=m_Meshes.begin();itr!=m_Meshes.end();itr++ )
    {
        if( strcmp(itr->pMesh->GetName(),cMeshName) == 0 )
            return itr->nReference;
    }

    return -1;
}