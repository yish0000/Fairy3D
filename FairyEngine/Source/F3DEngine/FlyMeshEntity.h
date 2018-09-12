//------------------------------------------------------------------------
// Name: FlyMeshEntity.h
// Desc: This file define a real instance of mesh in the 3D world.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_MESHENTITY_H__
#define __FLY_MESHENTITY_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlySceneObject.h"
#include "FlyRenderable.h"
#include "FlyMesh.h"

#include "FlyRenderableHelper.h"
#include "FlyShadowVolume.h"
//////////////////////////////////////////////////////////////////////////


class FlyMeshRenderable;

/*
* This is a real instance of static mesh in the 3D world.
* Many mesh entities can have a common mesh template.
*/
class _FLY_EXPORT FlyMeshEntity : public FlySceneObject
{
    typedef std::vector<FlyMeshRenderable*> RenderableList;
    typedef std::vector<FlyShadowVolume*> ShadowVolumeList;
    typedef std::vector<FlyRenderMaterial*> MaterialList;

protected:
    FlyMesh* m_pMesh;   // Pointer to the mesh template.

    MaterialList m_Materials;       // Render material list.
    RenderableList m_Renderables;   // Renderable object list.

    bool m_bNeedRebuildShadows;         // Need rebuild the shadow volumes.
    ShadowVolumeList m_ShadowVolumes;   // Shadow volume list.
    LightObjectList m_ShadowCasters;    // Shadow caster list.

    bool m_bShowBBox; // Does we show the boundingBox.
    FlyBBoxHelper* m_pBBoxHelper; // Pointer to the bbox renderable.
    int m_nDirtyState; // Dirty state of the entity.

    // Build the renderable objects.
    HRESULT BuildRenderables(void);

    // Build the shadow volumes for the mesh entity.
    HRESULT BuildShadowVolumes(void);

    // Update the shadow volumes for the mesh entity.
    void UpdateShadowVolumes(void);

    // Reinitialize the entity.
    HRESULT Reinitialize(void);

    // Update the light objects.
    void UpdateLights(void);

public:
    // Constructor and destructor.
    FlyMeshEntity( const char* cName );
    ~FlyMeshEntity(void);

    // Initialize the mesh entity.
    HRESULT Initialize( FlyMesh* pMesh );

    // Update the mesh entity.
    void UpdateObject(void);

    // Render the mesh entity.
    void RenderObject(void);

    // Get the local boundingBox of the entity.
    const FlyAabb& GetLocalBoundingBox(void) const;

    bool IsShowBBox(void) const { return m_bShowBBox; }
    void ShowBBox( bool bShow ) { m_bShowBBox = bShow; }

    // Set the lighting mode.
    void SetLightingMode( bool bLighting );

    // Get the mesh template of the entity.
    const FlyMesh* GetMeshTemplate(void) const;

    void SetMeshTemplate( FlyMesh* pMesh );
    void SetMeshTemplate( const char* cMeshName );
};

//////////////////////////////////////////////////////////////////////////

class FlyMeshRenderable : public FlyRenderable
{
    friend class FlyMeshEntity;

protected:
    FlyMeshEntity* m_pEntity;   // Pointer to the parent entity.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.
    FlyAabb m_BBox;             // BoundingBox of the object.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    FlyMeshRenderable( FlyMeshEntity* pEntity );
    ~FlyMeshRenderable(void);

    void SetVertexBuffer( FlyGraphicBuffer* pVB );
    void SetIndexBuffer( FlyGraphicBuffer* pIB );

    // Render the object.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the local boundingBox of the object.
    const FlyAabb& GetBoundingBox(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyMeshEntityFactory : public FlyEntityFactory
{
public:
    // Constructor and destructor.
    FlyMeshEntityFactory(void) { m_sTypename = "mesh"; }
    ~FlyMeshEntityFactory(void) { /* Nothing to do! */ };

    FlySceneObject* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneObject* pObject );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MESHENTITY_H__