//------------------------------------------------------------------------
// Name: FlyTerrainSceneManager.cpp
// Desc: This file is to implement the class FlyTerrainSceneManager.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "F3DTerrainSceneManager.h"

//#include "FlyKernel.h"
//#include "FlyMiscBasis.h"
 

// Content of the effect text to render terrain.
const std::string FlyTerrainSceneManager::s_sEffectText = 
"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
"<effect version=\"1.0\">\n"
"    <parameter type=\"sampler2D\" name=\"g_MainTexture\" />\n"
"    <parameter type=\"sampler2D\" name=\"g_MaskTexture\" />\n"
"    <parameter type=\"sampler2D\" name=\"g_Detail0\" />\n"
"    <parameter type=\"sampler2D\" name=\"g_Detail1\" />\n"
"    <parameter type=\"sampler2D\" name=\"g_Detail2\" />\n"
"    <parameter type=\"sampler2D\" name=\"g_Detail3\" />\n"
"    <parameter type=\"sampler2D\" name=\"g_LightMap\" />\n"
"    <parameter type=\"bool\" name=\"g_UseLight\" />\n"
"    <parameter type=\"float\" name=\"g_MainScale\" />\n"
"    <parameter type=\"float\" name=\"g_DetailScale0\" />\n"
"    <parameter type=\"float\" name=\"g_DetailScale1\" />\n"
"    <parameter type=\"float\" name=\"g_DetailScale2\" />\n"
"    <parameter type=\"float\" name=\"g_DetailScale3\" />\n"
"    <shader>\n"
"        <function name=\"TextureBlend\" />\n"
"        <function name=\"WhiteFrame\" />\n"
"        <code><![CDATA[\n"
"        float4 TextureBlend( float2 vTexCoord0 : TEXCOORD0,\n"
"                             float2 vTexCoord1 : TEXCOORD1 ) : COLOR\n"
"        {\n"
"            float4 finalColor;\n"
"            float4 mainColor = tex2D( g_MainTexture,vTexCoord0*g_MainScale );\n"
"            float4 maskColor = tex2D( g_MaskTexture,vTexCoord1 );\n"
"            float4 detail0 = tex2D( g_Detail0,vTexCoord0*g_DetailScale0 );\n"
"            float4 detail1 = tex2D( g_Detail1,vTexCoord0*g_DetailScale1 );\n"
"            float4 detail2 = tex2D( g_Detail2,vTexCoord0*g_DetailScale2 );\n"
"            float4 detail3 = tex2D( g_Detail3,vTexCoord0*g_DetailScale3 );\n"
"            finalColor = mainColor;\n"
"            finalColor = detail0*maskColor[2] + (1-maskColor[2])*finalColor;\n"
"            finalColor = detail1*maskColor[1] + (1-maskColor[1])*finalColor;\n"
"            finalColor = detail2*maskColor[0] + (1-maskColor[0])*finalColor;\n"
"            finalColor = detail3*maskColor[3] + (1-maskColor[3])*finalColor;\n"
"            if( g_UseLight ) finalColor = finalColor * tex2D(g_LightMap,vTexCoord1);\n"
"            return finalColor;\n"
"        }\n"
"        float4 WhiteFrame() : COLOR { return float4(1.0f,1.0f,1.0f,1.0f); } \n"
"        ]]></code>\n"
"    </shader>\n"
"    <technique name=\"MultiTexture\">\n"
"        <pass name=\"p0\">\n"
"            <state name=\"vertex_shader\" value=\"null\" />\n"
"            <state name=\"pixel_shader\" value=\"TextureBlend\" profile=\"ps_2_0\" />\n"
"        </pass>\n"
"    </technique>\n"
"    <technique name=\"Second\">\n"
"        <pass name=\"p0\">\n"
"            <state name=\"polygonmode\" value=\"wireframe\" />\n"
"            <state name=\"vertex_shader\" value=\"null\" />\n"
"            <state name=\"pixel_shader\" value=\"WhiteFrame\" />\n"
"        </pass>\n"
"    </technique>\n"
"</effect>\n";

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyTerrainSceneManager::FlyTerrainSceneManager(void)
{
    m_pLevel = NULL;
    m_pTerrainTiles = NULL;
    m_pRenderEffect = NULL;

    m_nMaxLodLevel = 0;
    m_fLodSqrDist = NULL;
    m_nRenderQuality = 1;
    m_nMaxPixelError = 3;
    m_bUseTriStrips = false;
    m_bOpenLight = false;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Create the renderEffect for sceneManager.
    m_pRenderEffect = pResMgr->GetEffectByName( "FRCore/MultiTexture-Terrain" );
    if( !m_pRenderEffect )
    {
        m_pRenderEffect = pResMgr->AddNullEffect( "FRCore/MultiTexture-Terrain" );
        m_pRenderEffect->Compile( s_sEffectText.c_str() );
    }
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyTerrainSceneManager::~FlyTerrainSceneManager(void)
{
    SAFE_DELETE_ARRAY( m_fLodSqrDist );

    // Destroy all the indexBuffers.
    LevelIndexData::iterator itr;
    for( itr=m_IndexData.begin();itr!=m_IndexData.end();itr++ )
    {
        IndexBufferMap bufferMap = *itr;

        IndexBufferMap::iterator buf;
        for( buf=bufferMap.begin();buf!=bufferMap.end();buf++ )
            SAFE_DELETE( buf->second.pIB );
    }

    // Destroy all the terrail tiles.
    SAFE_DELETE_ARRAY( m_pTerrainTiles );

    // Destroy the terrain level.
    SAFE_DELETE( m_pLevel );
}


//------------------------------------------------------------------------
// Clear the scene data.
//------------------------------------------------------------------------
void FlyTerrainSceneManager::ClearScene(void)
{
    SAFE_DELETE_ARRAY( m_fLodSqrDist );

    // Destroy all the indexBuffers.
    LevelIndexData::iterator itr;
    for( itr=m_IndexData.begin();itr!=m_IndexData.end();itr++ )
    {
        IndexBufferMap bufferMap = *itr;

        IndexBufferMap::iterator buf;
        for( buf=bufferMap.begin();buf!=bufferMap.end();buf++ )
            SAFE_DELETE( buf->second.pIB );
    }
    m_IndexData.clear();

    // Destroy all the terrail tiles.
    SAFE_DELETE_ARRAY( m_pTerrainTiles );

    // Destroy the terrain level.
    SAFE_DELETE( m_pLevel );

    FlyOctreeSceneManager::ClearScene();
}


//------------------------------------------------------------------------
// Load the terrain geometry data from the file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the terrain map.
//------------------------------------------------------------------------
HRESULT FlyTerrainSceneManager::LoadWorldGeometry( const char* cFilename )
{
    HRESULT hr;

    // Clean the old scene data.
    ClearScene();

    // Create a new terrain level.
    m_pLevel = new FlyTerrainLevel();
    if( !m_pLevel ) return FLY_OUTOFMEMORY;
    m_pLevel->SetName( m_sName.c_str() );

    // Initialize the level.
    hr = m_pLevel->LoadTerrainMap( cFilename );
    if( FAILED(hr) )
    {
        SAFE_DELETE( m_pLevel );
        return hr;
    }

    m_pLevel->SetHeightScale( 0.3f );
    m_pLevel->SetLengthPerUnit( 2.0f );

    // Build the terrain tiles.
    return BuildTerrainTiles();
}

//------------------------------------------------------------------------
// Build the terrain tiles for the sceneManager.
//------------------------------------------------------------------------
HRESULT FlyTerrainSceneManager::BuildTerrainTiles(void)
{
    UINT i,j;
    UINT nNumTiles = m_pLevel->GetTerrainSize() / m_pLevel->GetTileSize();

    // Calculate the lod square distance.
    CalcLodSqrDistance();

    // Prepare the indices for the sceneManager.
    // Initialize the indices for terrain.
    for( i=0;i<m_nMaxLodLevel;i++ )
        m_IndexData.push_back( IndexBufferMap() );

    // Create a sceneNode for the tiles.
    FlySceneNode* pTerrain = m_pSceneRoot->CreateChildNode( "Terrain" );

    // Create the terrain tiles.
    m_pTerrainTiles = new FlyTerrainTile[nNumTiles*nNumTiles];
    if( !m_pTerrainTiles ) return FLY_OUTOFMEMORY;

    // Initialize the tiles.
    for( j=0;j<nNumTiles;j++ )
    {
        for( i=0;i<nNumTiles;i++ )
        {
            char cTemp[40];

            FlyTerrainTile* pTile = &m_pTerrainTiles[j*nNumTiles+i];
            FRSprintf( cTemp,sizeof(cTemp),"Tile(%d,%d)",i,j );
            pTile->SetName( cTemp );
            pTile->SetSceneManager( this );
            pTile->SetCreator( NULL );
            pTile->SetTerrainLevel( m_pLevel );

            pTile->Initialize( m_pLevel->GetTileSize()*i,
                m_pLevel->GetTileSize()*j,m_pLevel->GetHeightData() );

            const FlyTerrainLevel::TileInfo* pInfo = m_pLevel->GetTileInfo( i,j );
            pTile->SetMainTexture( m_pLevel->GetTexture(pInfo->nMainTexture) );
            pTile->SetMainScale( pInfo->fMainScale );
            pTile->SetDetailTexture( 0,m_pLevel->GetTexture(pInfo->nDetailTexture[0]) );
            pTile->SetDetailTexture( 1,m_pLevel->GetTexture(pInfo->nDetailTexture[1]) );
            pTile->SetDetailTexture( 2,m_pLevel->GetTexture(pInfo->nDetailTexture[2]) );
            pTile->SetDetailTexture( 3,m_pLevel->GetTexture(pInfo->nDetailTexture[3]) );
            pTile->SetDetailScale( 0,pInfo->fDetailScale[0] );
            pTile->SetDetailScale( 1,pInfo->fDetailScale[1] );
            pTile->SetDetailScale( 2,pInfo->fDetailScale[2] );
            pTile->SetDetailScale( 3,pInfo->fDetailScale[3] );

            if( i - 1 >= 0 )
                pTile->SetNeighbor( FlyTerrainTile::NB_WEST,&m_pTerrainTiles[j*nNumTiles+(i-1)] );
            else
                pTile->SetNeighbor( FlyTerrainTile::NB_WEST,NULL );

            if( j - 1 >= 0 )
                pTile->SetNeighbor( FlyTerrainTile::NB_SOUTH,&m_pTerrainTiles[(j-1)*nNumTiles+i] );
            else
                pTile->SetNeighbor( FlyTerrainTile::NB_SOUTH,NULL );

            if( i + 1 < nNumTiles )
                pTile->SetNeighbor( FlyTerrainTile::NB_EAST,&m_pTerrainTiles[j*nNumTiles+(i+1)] );
            else
                pTile->SetNeighbor( FlyTerrainTile::NB_EAST,NULL );

            if( j + 1 < nNumTiles )
                pTile->SetNeighbor( FlyTerrainTile::NB_NORTH,&m_pTerrainTiles[(j+1)*nNumTiles+i] );
            else
                pTile->SetNeighbor( FlyTerrainTile::NB_NORTH,NULL );

            // Attach the terrain tile to sceneNode.
            FRSprintf( cTemp,sizeof(cTemp),"TileNode(%d,%d)",i,j );
            FlySceneNode* pTileNode = pTerrain->CreateChildNode( cTemp );
            pTileNode->AttachObject( pTile );
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Calculate the lod square distance.
//------------------------------------------------------------------------
void FlyTerrainSceneManager::CalcLodSqrDistance(void)
{
    // Calculate the max lod level.
    m_nMaxLodLevel = 0;

    UINT nTemp = m_pLevel->GetTileSize();
    while( nTemp != 0 )
    {
        nTemp = nTemp / 2;
        m_nMaxLodLevel++;
    }

    m_fLodSqrDist = new float[m_nMaxLodLevel];
    m_fLodSqrDist[0] = 0.0f;
    m_fLodSqrDist[1] = 64.0f * 64.0f;

    for( UINT i=2;i<m_nMaxLodLevel;i++ )
        m_fLodSqrDist[i] = m_fLodSqrDist[i-1] * 4.0f;
}


//------------------------------------------------------------------------
// Add a new indexBuffer for the terrain tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyGraphicBuffer*:  Pointer to the index buffer.
//      UINT:               Count of the indices.
//      int:                Which lod level ?
//      DWORD:              How to stitch the edge ?
//------------------------------------------------------------------------
void FlyTerrainSceneManager::AddIndexBuffer( FlyGraphicBuffer* pIB,UINT nNumIndices,int nLodLevel,
                                             DWORD dwStitchFlags )
{
    IndexBuffer buffer;
    buffer.pIB = pIB;
    buffer.nNumIndices = nNumIndices;

    m_IndexData[nLodLevel].insert( IndexBufferMap::value_type(dwStitchFlags,
        buffer) );
}


//------------------------------------------------------------------------
// Get a specified index buffer to tile to render.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                Which lod level ?
//      UINT:               How to stitch the edge ?
//      FlyGraphicBuffer**: To store the index buffer.
//      UINT*:              To store the count of indices.
//------------------------------------------------------------------------
void FlyTerrainSceneManager::GetIndexBuffer( int nLodLevel,UINT nStitchFlags,FlyGraphicBuffer** ppIB,
                                             UINT* pNumIndices )
{
    IndexBufferMap::iterator itr;
    itr = m_IndexData[nLodLevel].find( nStitchFlags );

    if( itr != m_IndexData[nLodLevel].end() )
    {
        *ppIB = itr->second.pIB;
        *pNumIndices = itr->second.nNumIndices;
    }
    else
    {
        *ppIB = NULL;
        *pNumIndices = 0;
    }
}


//------------------------------------------------------------------------
// Get the terrain tile at the specified point.
//------------------------------------------------------------------------
FlyTerrainTile* FlyTerrainSceneManager::GetTerrainTile( const FlyVector& vPoint )
{
    if( vPoint.x < 0.0f || 
        vPoint.x >= m_pLevel->GetTerrainSize()*m_pLevel->GetLengthPerUnit() ||
        vPoint.z < 0.0f ||
        vPoint.z >= m_pLevel->GetTerrainSize()*m_pLevel->GetLengthPerUnit() )
    {
        return NULL;
    }

    int nX = (int)(vPoint.x / m_pLevel->GetLengthPerUnit());
    int nZ = (int)(vPoint.z / m_pLevel->GetLengthPerUnit());
    int nTileX = nX / m_pLevel->GetTileSize();
    int nTileZ = nZ / m_pLevel->GetTileSize();

    int nSize = m_pLevel->GetTerrainSize() / m_pLevel->GetTileSize();
    if( nTileX == nSize ) nTileX--;
    if( nTileZ == nSize ) nTileZ--;

    return &m_pTerrainTiles[nTileZ*nSize+nTileX];
}


//------------------------------------------------------------------------
// Get the height at the given position.
//------------------------------------------------------------------------
float FlyTerrainSceneManager::GetHeightValue( const FlyVector& vPostion ) const
{
    return m_pLevel->GetScaledHeightValue( vPostion.x,vPostion.z );
}


//------------------------------------------------------------------------
// Test the collistion with the terrain scene.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyRay&:      Ray to test the collision.
//      float:              Length of the ray.
//      float&:             To store the nearest intersecting point.
//------------------------------------------------------------------------
bool FlyTerrainSceneManager::TestCollision( const FlyRay& ray,float fL,float& fDist )
{
    return false;
}


//------------------------------------------------------------------------
// Set the light mode for the terrain.
//------------------------------------------------------------------------
void FlyTerrainSceneManager::SetLightMode( bool bOpenLight )
{
    if( m_bOpenLight != bOpenLight )
    {
        m_bOpenLight = bOpenLight;

        UINT nNumTiles = m_pLevel->GetTerrainSize() / m_pLevel->GetTileSize();
        nNumTiles *= nNumTiles;
        for( UINT i=0;i<nNumTiles;i++ )
        {
            FlyRenderEffectInstance* pGPUProgram = m_pTerrainTiles[i].GetRenderUnit()
                ->GetRenderMaterial()->GetRenderEffectInstance(0);
            *pGPUProgram->GetParameterByName("g_UseLight") = m_bOpenLight;
        }
    }
}

void FlyTerrainSceneManager::SetWireMode( bool bWireMode )
{
    UINT nNumTiles = m_pLevel->GetTerrainSize() / m_pLevel->GetTileSize();
    nNumTiles *= nNumTiles;

    for( UINT i=0;i<nNumTiles;i++ )
    {
        if( bWireMode )
        {
            FlyRenderMaterial* pMaterial = m_pTerrainTiles[i].GetRenderUnit()->GetRenderMaterial();
            pMaterial->SetActiveTechnique( 1 );
        }
        else
        {
            FlyRenderMaterial* pMaterial = m_pTerrainTiles[i].GetRenderUnit()->GetRenderMaterial();
            pMaterial->SetActiveTechnique( 0 );
        }
    }
}


//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
// Create an instance for the terrain sceneManager.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the terrain sceneManager.
//------------------------------------------------------------------------
FlySceneManager* FlyTerrainSceneFactory::CreateInstance( const char* cName )
{
    FlyTerrainSceneManager* pManager;
    pManager = new FlyTerrainSceneManager();
    pManager->SetName( cName );
    pManager->SetCreator( this );
    return pManager;
}


//------------------------------------------------------------------------
// Destroy a terrainSceneManager instance.
//------------------------------------------------------------------------
void FlyTerrainSceneFactory::DestroyInstance( FlySceneManager* pManager )
{
    SAFE_DELETE( pManager );
}