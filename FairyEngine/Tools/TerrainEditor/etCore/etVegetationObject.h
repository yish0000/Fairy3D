//------------------------------------------------------------------------
// Name: etVegetationObject.h
// Desc: This file define a class to implement a new scene object type 
//       for the engine to draw vegetations.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_VEGETATION_OBJECT_H__
#define __ET_VEGETATION_OBJECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"
#include "etFloatRect.h"

#include "FlyGraphicBuffer.h"
#include "FlySceneObject.h"
#include "FlyRenderable.h"
//////////////////////////////////////////////////////////////////////////


enum VEGETATIONTYPE
{
    VGT_POLYGON,
    VGT_MESH,
};

class _ET_EXPORT etVegetationObject : public FlySceneObject
{
    typedef std::vector<etVegetationRenderable*> RenderableList;
    typedef std::vector<FlyRenderMaterial*> MaterialList;

protected:
    VEGETATIONTYPE m_Type;

    // Mesh vegetation object.
    FlyMesh* m_pMesh;
    int m_nDirtyState;
    MaterialList m_Materials;

    // Polygon vegetation object.
    FlyTexture* m_pTexture;

    FlyAabb m_Bounds;
    RenderableList m_Renderables;

    //////////////////////////////////////////////////////////////////////

    HRESULT BuildMeshRenderable(void);
    HRESULT BuildPolygonRenderable( float fWidth,float fHeight,UINT nNumPolygons,
        const etFloatRect& texRect );

    HRESULT Reinitialize(void);

public:
    // Constructor and destructor.
    etVegetationObject( const char* cName );
    ~etVegetationObject(void);

    HRESULT Initialize( VEGETATIONTYPE Type,const char* cMeshName,
        float fWidth,float fHeight,const etFloatRect& texRect,
        const char* cTexture,UINT nNumPolygons );

    // Update the object.
    void UpdateObject(void);

    // Render the object.
    void RenderObject(void);

    // Get the local boundingBox of the entity.
    const FlyAabb& GetLocalBoundingBox(void) const;

    FlyMesh* GetMeshTemplate(void) { return m_pMesh; }
    VEGETATIONTYPE GetVegetationType(void) const { return m_Type; }
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etVegetationRenderable : public FlyRenderable
{
    friend class etVegetationObject;

protected:
    etVegetationObject* m_pParent;  // Pointer to the parent object.
    FlyAabb m_BBox;         // BoundingBox of the object.

    UINT m_nNumVerts;       // Count of the vertices.
    UINT m_nNumIndis;       // Count of the indices.
    UINT m_nNumPrims;       // Count of the primitives.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    etVegetationRenderable( etVegetationObject* pParent );
    ~etVegetationRenderable(void);

    // Render the renderable object.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the local boundingBox of the object.
    const FlyAabb& GetBoundingBox(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const { return 1; }

    UINT GetLightCount(void) const { return 0; }
    FlyLightObject* GetLightObject( UINT nIndex ) { return NULL; }
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etVegetationFactory : public FlyEntityFactory
{
public:
    // Constructor and destructor.
    etVegetationFactory(void) { m_sTypename = "vegetation"; }
    ~etVegetationFactory(void) { /* Nothing to do! */ };

    FlySceneObject* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneObject* pObject );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_VEGETATION_OBJECT_H__