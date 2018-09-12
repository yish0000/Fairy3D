//------------------------------------------------------------------------
// Name: FlyMeshManager.h
// Desc: This file define a class to manage the mesh resources for engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_MESHMANAGER_H__
#define __FLY_MESHMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyMesh.h"
//////////////////////////////////////////////////////////////////////////


/*
* This class is to handles the management of mesh resources.
* You can also create manual meshes by this class.
*/
class _FLY_EXPORT FlyMeshManager
{
    struct MeshNode
    {
        FlyMesh* pMesh; // Pointer to the mesh.
        int nReference; // Reference count of the resource.
    };

    typedef std::vector<MeshNode> MeshResourceList;

protected:
    MeshResourceList m_Meshes;

public:
    // Constructor and destructor.
    FlyMeshManager(void);
    ~FlyMeshManager(void);

    // Reference count of the mesh resource.
    void AddResourceRef( const char* cMeshName );
    void DecResourceRef( const char* cMeshName );

    // Load a mesh from the model file.
    FlyMesh* LoadMesh( const char* cFilename );

    // Create an empty mesh manually.
    FlyMesh* CreateManualMesh( const char* cName );

    // Create a plane mesh.
    FlyMesh* CreatePlane( const char* cName,const FlyPlane& plane,float width,float height,
        int nXSegments,int nYSegments,int nUTile,int nVTile );

    // Create a cylinder mesh.
    FlyMesh* CreateCylinder( const char* cName,float fRadius1,float fRadius2,
        float fHeight,int nSlices,int nStacks );

    // Create a polygon mesh.
    FlyMesh* CreatePolygon( const char* cName,float fRadius,int nSides );

    // Create a sphere mesh.
    FlyMesh* CreateSphere( const char* cName,float fRadius );

    // Get the mesh resource.
    FlyMesh* GetMeshByIndex( UINT nIndex );
    FlyMesh* GetMeshByName( const char* cName );

    // Get the count of the mesh resource.
    UINT GetNumMeshes(void) const;

    // Destroy a mesh resource.
    void DestroyMeshByIndex( UINT nIndex );
    void DestroyMeshByName( const char* cName );

    // Destroy all mesh resources.
    void DestroyAllMeshes(void);

    // Get the reference count of the resource.
    int GetRefCount( const char* cMeshName );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MESHMANAGER_H__