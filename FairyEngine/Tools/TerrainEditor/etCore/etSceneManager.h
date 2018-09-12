//------------------------------------------------------------------------
// Name: etSceneManager.h
// Desc: This class define a sceneManager for the editable terrain.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_SCENEMANAGER_H__
#define __ET_SCENEMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "FlyOctreeSceneManager.h"
#include "etTerrain.h"
#include "etTile.h"
#include "etLiquidObject.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etSceneManager : public FlyOctreeSceneManager
{
    friend class etTile;

protected:
    etTerrain* m_pLevel;                // Pointer to the terrain level.

    RENDERMODE m_RenderMode;            // Current render mode.
    bool m_bLighting;                   // Open the lighting mode.
    FlyVector m_vLightDir;              // Direction of the light.
    FLYCOLOR m_cLightAmbient;           // Ambient color of the light.
    FLYCOLOR m_cLightDiffuse;           // Diffuse color of the light.
    FLYCOLOR m_cWireColor;              // Color of the wireframe.

    // Build the terrain tiles for manager.
    HRESULT SetupTerrainTiles(void);

public:
    // Constructor and destructor.
    etSceneManager(void);
    ~etSceneManager(void);

    HRESULT CreateNewScene( UINT nTerrainSize,UINT nTileSize,UINT nAlphaSize,
        float fInitHeight=0.0f );

    // Load the terrain geometry data.
    HRESULT LoadWorldGeometry( const char* cFilename );

    // Clear the scene data.
    void ClearScene(void);

    // Test the collistion in the 3D world.
    bool TestCollision( const FlyRay& ray,float fL,float& fDist );

    // Get the height value.
    float GetHeightValue( const FlyVector& vPostion ) const;

    bool GetLightingMode(void) const { return m_bLighting; }
    const FlyVector& GetLightDirection(void) const { return m_vLightDir; }
    const FLYCOLOR& GetLightAmbient(void) const { return m_cLightAmbient; }
    const FLYCOLOR& GetLightDiffuse(void) const { return m_cLightDiffuse; }

    void SetLightingMode( bool bOpenLight );
    void SetLightDirection( const FlyVector& vLightDir );
    void SetLightAmbient( const FLYCOLOR& color );
    void SetLightDiffuse( const FLYCOLOR& color );

    RENDERMODE GetRenderMode(void) const { return m_RenderMode; }
    void SetRenderMode( RENDERMODE renderMode );

    const FLYCOLOR& GetWireColor(void) const { return m_cWireColor; }
    void SetWireColor( const FLYCOLOR& color );

    // Get the pointer to the terrain level.
    etTerrain* GetTerrainLevel(void) const { return m_pLevel; }
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etSceneFactory : public FlySceneFactory
{
public:
    // Constructor and destructor.
    etSceneFactory(void) { m_sTypename = "et_scene"; }
    ~etSceneFactory(void) { /* Nothing to do! */ };

    FlySceneManager* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneManager* pManager );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_SCENEMANAGER_H__