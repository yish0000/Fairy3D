//------------------------------------------------------------------------
// Name: FlyManualEntity.h
// Desc: This file define a manual entity for the user to construct their
//       own scene object.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_MANUAL_ENTITY_H__
#define __FLY_MANUAL_ENTITY_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlySceneObject.h"

#include "FlyRenderable.h"
#include "FlyGraphicBuffer.h"
//////////////////////////////////////////////////////////////////////////


class FlyManualSubObj;

/*
* User can use this class to generate manual objects.
*/
class _FLY_EXPORT FlyManualEntity : public FlySceneObject
{
    typedef std::vector<FlyManualSubObj*> SubObjectList;
    typedef std::vector<FlyRenderMaterial*> MaterialList;

protected:
    SubObjectList m_SubObjects;     // Sub object list for entity.
    MaterialList m_Materials;       // Render material list.
    FlyAabb m_LocalBBox;            // Local boundingBox for entity.

public:
    // Constructor and destructor.
    FlyManualEntity( const char* cName );
    ~FlyManualEntity(void);

    // Override the virtual function.
    void UpdateObject(void);
    void RenderObject(void);

    // Get the boundingBox of the entity.
    const FlyAabb& GetLocalBoundingBox(void) const;

    // Add a new render material.
    FlyRenderMaterial* AddMaterial( const char* cMatName );
    void DestroyMaterial( size_t nIndex );
    FlyRenderMaterial* GetRenderMaterial( size_t nIndex );

    // Create a new sub object.
    FlyManualSubObj* BeginSubObject( VERTEXTYPE vertexType=VF_DEFAULT,
        RENDERTYPE renderType=RT_TRIANGLELIST );
    void EndSubObject( FlyManualSubObj* pObject );

    // Destroy the subObject.
    void DestroySubObject( size_t nIndex );

    FlyManualSubObj* GetSubObject( size_t nIndex );
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyManualSubObj : public FlyRenderable
{
protected:
    VERTEXTYPE m_VertexType;    // Vertex type of the object.
    UINT m_nVertexSize;         // Size of the vertex.

    FlyManualEntity* m_pParent; // Pointer to the parent entity.

    FlyAabb m_BBox;             // Local boundingBox of the object.

    UINT m_nMaxNumVerts;        // Max count of the vertices.
    UINT m_nMaxNumIndis;        // Max count of the indices.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    FlyManualSubObj( FlyManualEntity* pEntity );
    ~FlyManualSubObj(void);

    HRESULT AddVertexData( UINT nNumVerts,const void* pVerts,const FlyAabb& box );
    HRESULT AddIndexData( UINT nNumIndis,const WORD* pIndis );

    // Clear the data.
    void ClearData(void);

    // Create the graphic buffer.
    HRESULT CreateGraphicBuffer( UINT nMaxNumVerts,UINT nMaxNumIndis );

    // Render the manual sub object.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    const FlyAabb& GetBoundingBox(void) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );

    VERTEXTYPE GetVertexType(void) const;
    void SetVertexType( VERTEXTYPE vertexType );

    FlyGraphicBuffer* GetVertexBuffer(void);
    FlyGraphicBuffer* GetIndexBuffer(void);

    // Get the parent entity.
    FlyManualEntity* GetParentEntity(void);
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyManualEntityFactory : public FlyEntityFactory
{
public:
    // Constructor and destructor.
    FlyManualEntityFactory(void) { m_sTypename = "manual"; }
    virtual ~FlyManualEntityFactory(void) { /* Nothing to do! */ };

    FlySceneObject* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneObject* pObject );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MANUAL_ENTITY_H__