//------------------------------------------------------------------------
// Name: FlyMesh.h
// Desc: This file define a static model class for engine.
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_MESH_H__
#define __FLY_MESH_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyGraphicBuffer.h"
//////////////////////////////////////////////////////////////////////////


class FlySubMesh;

class _FLY_EXPORT FlyMesh
{
public:
    struct sMeshMaterial
    {
        FLYCOLOR cAmbient;
        FLYCOLOR cDiffuse;
        FLYCOLOR cSpecular;
        FLYCOLOR cEmissive;
        float fPower;
        FlyTexture* pTexture;
    };

    struct sMeshBlock
    {
        UINT nMatID;
        UINT nNumVertices;
        UINT nNumFaces;
        VERTEX* pVertices;
        WORD* pIndices;

        // Destructor of the struct.
        ~sMeshBlock(void)
        {
            SAFE_DELETE_ARRAY( pVertices );
            SAFE_DELETE_ARRAY( pIndices );
        }
    };

    typedef std::vector<FlySubMesh*> SubMeshList;
    typedef std::vector<sMeshMaterial> MaterialList;

protected:
    std::string m_sName;        // Name of the mesh.

    MaterialList m_Materials;   // Render material list.
    SubMeshList m_SubMeshes;    // Sub Mesh list.
    FlyAabb m_BBox;             // Local boundingBox of the mesh.

    int m_nDirtyState;          // Dirty state of the mesh.

    // Build the subMesh data.
    HRESULT BuildMeshInfo( UINT nNumBlocks,sMeshBlock* pBlocks );

public:
    // Constructor and destructor.
    FlyMesh(void);
    ~FlyMesh(void);

    // Destroy the sub meshes.
    void Destroy(void);

    // Create a new sub mesh.
    FlySubMesh* CreateSubMesh(void);

    // Destroy a specified sub mesh.
    void DestroySubMesh( UINT nIndex );

    // Get the subMesh pointer.
    FlySubMesh* GetSubMesh( UINT nIndex );
    UINT GetNumSubMeshes(void) const;

    // Create a new material.
    void CreateMaterial( const FLYCOLOR& cAmbient,const FLYCOLOR& cDiffuse,
        const FLYCOLOR& cSpecular,const FLYCOLOR& cEmissive,
        float fPower,const char* cTexture );

    // Destroy a specified material.
    void DestroyMaterial( UINT nIndex );

    // Get the materials.
    UINT GetNumMaterials(void) const;
    const sMeshMaterial* GetMaterial( UINT nIndex ) const;

    // Load or save the mesh data.
    HRESULT LoadMeshFromFile( const char* cModelFile );
    void SaveMeshToFile( const char* cModelFile,bool bSaveTexture=false );

    void RebuildBBox(void);
    const FlyAabb& GetBoundingBox(void) const;
    float GetBoundingRadius(void) const;

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    const char* GetName(void) const;
    void SetName( const char* cName );

    // Get the dirty state.
    int GetDirtyState(void) const;
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlySubMesh
{
    friend class FlyMesh;

protected:
    FlyMesh* m_pParent;     // Pointer to the parent.

    UINT m_nNumVerts;       // Number of the vertices.
    UINT m_nNumIndis;       // Number of the indices.
    UINT m_nNumPrims;       // Number of the primitives.
    FlyAabb m_BBox;         // Bounding box of the submesh.
    UINT m_nMaterial;       // Index of the material.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    FlySubMesh( FlyMesh* pParent );
    ~FlySubMesh(void);

    // Create the graphic buffers.
    HRESULT CreateGraphicBuffer( UINT nNumVerts,UINT nNumIndis,UINT nNumPrims );

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the boundingBox of the subMesh.
    const FlyAabb& GetBoundingBox(void) const;

    UINT GetRenderMaterial(void) const;
    void SetRenderMaterial( UINT nMaterial );

    FlyGraphicBuffer* GetVertexBuffer(void);
    FlyGraphicBuffer* GetIndexBuffer(void);
};

//////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FLY_MODELL_H__