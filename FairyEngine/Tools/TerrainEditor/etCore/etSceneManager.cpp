//------------------------------------------------------------------------
// Name: etSceneManager.cpp
// Desc: This file is to implement the class etSceneManager.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etSceneManager.h"

#include "etCoreManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etSceneManager::etSceneManager(void)
{
    m_pLevel = NULL;

    m_RenderMode = RM_TEXTURE;
    m_bLighting = true;
    m_vLightDir.Set( 1.0f,1.0f,1.0f );
    m_cLightAmbient = FLYCOLOR::Black();
    m_cLightDiffuse = FLYCOLOR::White();

    m_cWireColor = etCoreManager::Instance().GetConfigData()->GetWireColor();
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etSceneManager::~etSceneManager(void)
{
    // Destroy the terrain level.
    SAFE_DELETE( m_pLevel );
}


//------------------------------------------------------------------------
// Clear the current scene data.
//------------------------------------------------------------------------
void etSceneManager::ClearScene(void)
{
    // Destroy the terrain level.
    SAFE_DELETE( m_pLevel );

    FlyOctreeSceneManager::ClearScene();
}


//------------------------------------------------------------------------
// Create a new map for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Size of the terrain.
//      UINT:       Size of the tile.
//      UINT:       Size of the alphamap.
//      float:      Initial height value of the scene.
//------------------------------------------------------------------------
HRESULT etSceneManager::CreateNewScene( UINT nTerrainSize,UINT nTileSize,UINT nAlphaSize,
                                        float fInitHeight /*=0.0f*/ )
{
    HRESULT hr;

    // Clean the old scene data.
    ClearScene();

    // Create a new terrain level.
    m_pLevel = new etTerrain();
    if( !m_pLevel ) return FLY_OUTOFMEMORY;

    // Initialize the terrain.
    hr = m_pLevel->Initialize( nTerrainSize,nTileSize,nAlphaSize,
        fInitHeight );
    if( FAILED(hr) )
    {
        SAFE_DELETE( m_pLevel );
        return hr;
    }

    m_pLevel->SetHeightScale( 0.5f );
    m_pLevel->SetLengthPerUnit( 1.0f );

    // Build the terrain tiles.
    return SetupTerrainTiles();
}


//------------------------------------------------------------------------
// Load the terrain geometry data from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the terrain map.
//------------------------------------------------------------------------
HRESULT etSceneManager::LoadWorldGeometry( const char* cFilename )
{
    HRESULT hr;

    // Clean the old scene data.
    ClearScene();

    // Create a new terrain level.
    m_pLevel = new etTerrain();
    if( !m_pLevel ) return FLY_OUTOFMEMORY;
    m_pLevel->SetName( cFilename );

    // Initialize the level data.
    hr = m_pLevel->LoadTerrainMap( cFilename );
    if( FAILED(hr) )
    {
        SAFE_DELETE( m_pLevel );
        return hr;
    }

    // Build the terrain tiles.
    return SetupTerrainTiles();
}


//------------------------------------------------------------------------
// Build the terrain tiles for the sceneManager.
//------------------------------------------------------------------------
HRESULT etSceneManager::SetupTerrainTiles(void)
{
    char cTemp[40];
    UINT nNumTiles = m_pLevel->GetTerrainSize() / m_pLevel->GetTileSize();

    // Create a sceneNode for the tiles.
    FlySceneNode* pTerrain = m_pSceneRoot->CreateChildNode( "et-Terrain" );
    pTerrain->SetVisible( false );

    // Initialize the tiles.
    for( int j=0;j<nNumTiles;j++ )
    {
        for( int i=0;i<nNumTiles;i++ )
        {
            // Get the terrain tile.
            etTile* pTile = m_pLevel->GetTerrainTile( j*nNumTiles+i );

            // Attach the terrain tile to sceneNode.
            sprintf( cTemp,"TileNode(%d,%d)",i,j );
            FlySceneNode* pTileNode = pTerrain->CreateChildNode( cTemp );
            pTileNode->AttachObject( pTile );
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Test the collistion with the terrain scene.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyRay&:      Ray to test the collision.
//      float:              Length of the ray.
//      float&:             To store the nearest intersecting point.
//------------------------------------------------------------------------
bool etSceneManager::TestCollision( const FlyRay& ray,float fL,float& fDist )
{
    return false;
}


//------------------------------------------------------------------------
// Get the height at the given position.
//------------------------------------------------------------------------
float etSceneManager::GetHeightValue( const FlyVector& vPostion ) const
{
    return m_pLevel->GetScaledHeightValue( vPostion.x,vPostion.z );
}


//------------------------------------------------------------------------
// Set the lighting mode for the editor.
//------------------------------------------------------------------------
void etSceneManager::SetLightingMode( bool bOpenLight )
{
    FlyRenderMaterial* pMaterial;
    FlyRenderEffectInstance* pGPUProgram;

    if( bOpenLight != m_bLighting )
    {
        for( int i=0;i<m_pLevel->GetNumTiles();i++ )
        {
            pMaterial = m_pLevel->GetTerrainTile(i)->GetRenderable()
                ->GetRenderMaterial();

            for( int n=0;n<4;n++ )
            {
                pGPUProgram = pMaterial->GetRenderEffectInstance(n);
                *pGPUProgram->GetParameterByIndex(11) = bOpenLight;
            }
        }

        m_bLighting = bOpenLight;
    }
}


//------------------------------------------------------------------------
// Set the light direction for the editor.
//------------------------------------------------------------------------
void etSceneManager::SetLightDirection( const FlyVector& vLightDir )
{
    FlyRenderMaterial* pMaterial;
    FlyRenderEffectInstance* pGPUProgram;
    FlyVector vDirection;

    vDirection = vLightDir;
    vDirection.Normalize();

    for( int i=0;i<m_pLevel->GetNumTiles();i++ )
    {
        pMaterial = m_pLevel->GetTerrainTile(i)->GetRenderable()
            ->GetRenderMaterial();

        for( int n=0;n<4;n++ )
        {
            pGPUProgram = pMaterial->GetRenderEffectInstance(n);
            *pGPUProgram->GetParameterByIndex(12) = vDirection;
        }
    }

    m_vLightDir = vLightDir;
}


//------------------------------------------------------------------------
// Set the light ambient color for the editor.
//------------------------------------------------------------------------
void etSceneManager::SetLightAmbient( const FLYCOLOR& color )
{
    FlyRenderMaterial* pMaterial;
    FlyRenderEffectInstance* pGPUProgram;
    FlyVector vAmbient;

    for( int i=0;i<m_pLevel->GetNumTiles();i++ )
    {
        pMaterial = m_pLevel->GetTerrainTile(i)->GetRenderable()
            ->GetRenderMaterial();

        for( int n=0;n<4;n++ )
        {
            memcpy( &vAmbient,&color,sizeof(FLYCOLOR) );
            pGPUProgram = pMaterial->GetRenderEffectInstance(n);
            *pGPUProgram->GetParameterByIndex(13) = vAmbient;
        }
    }

    m_cLightAmbient = color;
}


//------------------------------------------------------------------------
// Set the light diffuse color for the editor.
//------------------------------------------------------------------------
void etSceneManager::SetLightDiffuse( const FLYCOLOR& color )
{
    FlyRenderMaterial* pMaterial;
    FlyRenderEffectInstance* pGPUProgram;
    FlyVector vDiffuse;

    for( int i=0;i<m_pLevel->GetNumTiles();i++ )
    {
        pMaterial = m_pLevel->GetTerrainTile(i)->GetRenderable()
            ->GetRenderMaterial();

        for( int n=0;n<4;n++ )
        {
            memcpy( &vDiffuse,&color,sizeof(FLYCOLOR) );
            pGPUProgram = pMaterial->GetRenderEffectInstance(n);
            *pGPUProgram->GetParameterByIndex(14) = vDiffuse;
        }
    }

    m_cLightDiffuse = color;
}


//------------------------------------------------------------------------
// Set the color of the wireframe.
//------------------------------------------------------------------------
void etSceneManager::SetWireColor( const FLYCOLOR& color )
{
    FlyRenderMaterial* pMaterial;
    FlyRenderEffectInstance* pGPUProgram;
    FlyVector vWireColor;

    for( int i=0;i<m_pLevel->GetNumTiles();i++ )
    {
        pMaterial = m_pLevel->GetTerrainTile(i)->GetRenderable()
            ->GetRenderMaterial();

        for( int n=0;n<4;n++ )
        {
            memcpy( &vWireColor,&color,sizeof(FLYCOLOR) );
            pGPUProgram = pMaterial->GetRenderEffectInstance(n);
            *pGPUProgram->GetParameterByIndex(16) = vWireColor;
        }
    }

    m_cWireColor = color;
}


//------------------------------------------------------------------------
// Set the render mode for the editor.
//------------------------------------------------------------------------
void etSceneManager::SetRenderMode( RENDERMODE renderMode )
{
    FlyRenderMaterial* pMaterial;

    if( renderMode != m_RenderMode )
    {
        for( int i=0;i<m_pLevel->GetNumTiles();i++ )
        {
            pMaterial = m_pLevel->GetTerrainTile(i)->GetRenderable()
                ->GetRenderMaterial();

            switch( renderMode )
            {
            case RM_TEXTURE:
                pMaterial->SetActiveTechnique( 0 );
                break;
            case RM_WIREFRAME:
                pMaterial->SetActiveTechnique( 2 );
                break;
            case RM_SOLID:
                pMaterial->SetActiveTechnique( 1 );
                break;
            case RM_TEXTUREWIRE:
                pMaterial->SetActiveTechnique( 3 );
                break;
            }
        }

        m_RenderMode = renderMode;
    }
}


//////////////////////////////////////////////////////////////////////////
//// IMPLEMENTING THE CLASS ETSCENEFACTORY.

//------------------------------------------------------------------------
// Create an instance of the sceneManager.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the sceneManager.
//------------------------------------------------------------------------
FlySceneManager* etSceneFactory::CreateInstance( const char* cName )
{
    etSceneManager* pManager = new etSceneManager();
    pManager->SetName( cName );
    pManager->SetCreator( this );
    return pManager;
}

//------------------------------------------------------------------------
// Destroy the instance of the sceneManager.
//------------------------------------------------------------------------
void etSceneFactory::DestroyInstance( FlySceneManager* pManager )
{
    SAFE_DELETE( pManager );
}