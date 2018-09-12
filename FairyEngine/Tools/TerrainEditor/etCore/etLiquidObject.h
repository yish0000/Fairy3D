//------------------------------------------------------------------------
// Name: etLiquidObject.h
// Desc: This file define a new sceneObject which draw the water effect
//       on the terrain for engine.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_LIQUIDOBJECT_H__
#define __ET_LIQUIDOBJECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "FlySceneObject.h"
#include "FlyRenderable.h"
#include "FlyGraphicBuffer.h"
#include "FlyRenderTarget.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etLiquidObject : public FlySceneObject
{
protected:
    FlyVector m_vPosition;          // Center of the liquid.
    float m_fWidth;                 // Width of the liquid.
    float m_fHeight;                // Height of the liquid.

    FlyTexture* m_pSurface;         // Surface texture of the water.
    float m_fTexScale;              // Scale of the surface texture.
    float m_fFlowSpeed;             // Speed of the water.
    float m_fDistortion;            // Distortion value.
    FlyTexture* m_pDistort;         // Distortion map.

    bool m_bReflection;             // Open the reflection mode ?
    bool m_bReflectMesh;            // Reflect the mesh ?
    bool m_bReflectSky;             // Reflect the skybox ?
    bool m_bReflectTerrain;         // Reflect the terrain ?
    etLiquidTarget* m_pReflect;     // Reflection map.

    etLiquidRenderable* m_pRenderable;

public:
    // Constructor and destructor.
    etLiquidObject( const char* cName );
    ~etLiquidObject(void);

    // Initialize the liquid object.
    HRESULT Initialize( const char* cSurfaceMap,const char* cDistortMap,
        const FlyVector& vCenter,float fWidth,float fHeight );

    // Update the liquid object.
    void UpdateObject(void);

    // Render the liquid object.
    void RenderObject(void);

    // Get the local boundingBox of the entity.
    const FlyAabb& GetLocalBoundingBox(void) const;

    const FlyTexture* GetReflectionMap(void) const;
    const FlyTexture* GetLiquidSurface(void) const { return m_pSurface; }
    float GetTexScale(void) const { return m_fTexScale; }
    float GetLiquidSpeed(void) const { return m_fFlowSpeed; }
    const FlyTexture* GetDistortMap(void) const { return m_pDistort; }
    float GetDistortion(void) const { return m_fDistortion; }
    bool IsReflection(void) const { return m_bReflection; }
    bool IsReflectMesh(void) const { return m_bReflectMesh; }
    bool IsReflectTerrain(void) const { return m_bReflectTerrain; }
    bool IsReflectSky(void) const { return m_bReflectSky; }
    const FlyVector& GetPosition(void) const { return m_vPosition; }
    float GetWidth(void) const { return m_fWidth; }
    float GetHeight(void) const { return m_fHeight; }

    void SetReflection( bool bReflection );
    void SetDistortMap( const char* cDistortMap );
    void SetSurfaceMap( const char* cSurfaceMap );
    void SetTexScale( float fTexScale );
    void SetDistortion( float fDistortion );

    const char* GetLiquidName(void) const { return m_sName.c_str(); }
    void UpdateGeometry( const float* pPosition,float fWidth,float fHeight );

    void SetLiquidSpeed( float fSpeed ) { m_fFlowSpeed = fSpeed; }
    void SetReflectMesh( bool bReflect ) { m_bReflectMesh = bReflect; }
    void SetReflectTerrain( bool bReflect ) { m_bReflectTerrain = bReflect; }
    void SetReflectSky( bool bReflect );
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etLiquidTarget : public FlyRenderTarget
{
protected:
    etLiquidObject* m_pParent;
    FlySceneNode* m_pUtil;
    FlySceneNode* m_pTerrain;
    FlySceneNode* m_pEntity;

public:
    // Constructor and destructor.
    etLiquidTarget( etLiquidObject* pParent );
    ~etLiquidTarget(void);

    void OnUpdateBegin(void);
    void OnUpdateEnd(void);
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etLiquidRenderable : public FlyRenderable
{
    friend class etLiquidObject;

protected:
    etLiquidObject* m_pParent;
    FlyAabb m_Bounds;
    FlyGraphicBuffer* m_pVB;

public:
    // Constructor and destructor.
    etLiquidRenderable( etLiquidObject* pParent );
    ~etLiquidRenderable(void);

    HRESULT Initialize( const FlyVector& vCenter,float fWidth,float fHeight );
    void BuildVertexData( const FlyVector& vCenter,float fWidth,float fHeight );

    // Render the liquid object.
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

class _ET_EXPORT etLiquidFactory : public FlyEntityFactory
{
public:
    // Constructor and destructor.
    etLiquidFactory(void) { m_sTypename = "liquid"; }
    ~etLiquidFactory(void) { /* Nothing to do! */ };

    FlySceneObject* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneObject* pObject );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_LIQUIDOBJECT_H__