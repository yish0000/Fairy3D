//------------------------------------------------------------------------
// Name: FlyQuake3Level.cpp
// Desc: This file is to implement the class FlyQuake3Level.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyQuake3Level.h"

#include "FlyStringUtil.h"


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyQuake3Level::FlyQuake3Level(void)
{
    m_pEntities = NULL;
    m_pFaces = NULL;
    m_pLeaves = NULL;
    m_pLightmaps = NULL;
    m_pNodes = NULL;
    m_pPlanes = NULL;
    m_pTextures = NULL;
    m_pVerts = NULL;
    m_pMeshIndices = NULL;
    m_pLeafFaces = NULL;
    m_pLeafBrushes = NULL;
    m_pBrushes = NULL;
    m_pBrushSides = NULL;
    m_VisData.data = NULL;

    m_nNumFaces = 0;
    m_nNumLeaves = 0;
    m_nNumLightmaps = 0;
    m_nNumNodes = 0;
    m_nNumPlanes = 0;
    m_nNumTextures = 0;
    m_nNumVerts = 0;
    m_nNumBrushes = 0;
    m_nNumBrushSides = 0;
    m_nNumLeafBrushes = 0;
    m_nNumLeafFaces = 0;
    m_nNumMeshIndices = 0;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyQuake3Level::~FlyQuake3Level(void)
{
    SAFE_DELETE_ARRAY( m_VisData.data );

    SAFE_DELETE_ARRAY( m_pEntities );
    SAFE_DELETE_ARRAY( m_pFaces );
    SAFE_DELETE_ARRAY( m_pLeaves );
    SAFE_DELETE_ARRAY( m_pLightmaps );
    SAFE_DELETE_ARRAY( m_pNodes );
    SAFE_DELETE_ARRAY( m_pPlanes );
    SAFE_DELETE_ARRAY( m_pTextures );
    SAFE_DELETE_ARRAY( m_pVerts );
    SAFE_DELETE_ARRAY( m_pMeshIndices );
    SAFE_DELETE_ARRAY( m_pLeafFaces );
    SAFE_DELETE_ARRAY( m_pLeafBrushes );
    SAFE_DELETE_ARRAY( m_pBrushes );
    SAFE_DELETE_ARRAY( m_pBrushSides );
}


//------------------------------------------------------------------------
// Load the bsp indoor scene from the Quake III map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::LoadQuake3Map( const char* cFilename )
{
    HRESULT hr;

    // Store the filename.
    m_sFilename = FlyStringUtil::GetDirFromPath(cFilename);

    FILE* pFile = fopen( cFilename,"rb" );
    if( !pFile ) return FLY_FILENOTFOUND;

    // Read the file header.
    fread( &m_Header,sizeof(bsp_header_t),1,pFile );

    // Check the bsp file.
    if( m_Header.magic[0] != 'I' || m_Header.magic[1] != 'B' ||
        m_Header.magic[2] != 'S' || m_Header.magic[3] != 'P' ||
        m_Header.version != 0x2E )
    {
        fclose( pFile );
        return FLY_INVALIDFILE;
    }

    hr = ReadVertices( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadFaces( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadEntities( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadLeaves( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadLightmaps( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadNodes( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadPlanes( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadTextures( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadBrushes( pFile );
    if( FAILED(hr) ) return hr;

    hr = ReadVisData( pFile );
    if( FAILED(hr) ) return hr;

    // Load the mesh indices.
    m_nNumMeshIndices = m_Header.lumps[BSP_MESHINDEX_LUMP].size / sizeof(int);
    m_pMeshIndices = new int[m_nNumMeshIndices];
    if( !m_pMeshIndices ) return FLY_OUTOFMEMORY;
    fseek( pFile,m_Header.lumps[BSP_MESHINDEX_LUMP].offset,SEEK_SET );
    fread( m_pMeshIndices,sizeof(int),m_nNumMeshIndices,pFile );

    // Load the leaf faces.
    m_nNumLeafFaces = m_Header.lumps[BSP_LEAFFACE_LUMP].size / sizeof(int);
    m_pLeafFaces = new int[m_nNumLeafFaces];
    if( !m_pLeafFaces ) return FLY_OUTOFMEMORY;
    fseek( pFile,m_Header.lumps[BSP_LEAFFACE_LUMP].offset,SEEK_SET );
    fread( m_pLeafFaces,sizeof(int),m_nNumLeafFaces,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Load the vertex data from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadVertices( FILE* pFile )
{
    // Get the number of vertices.
    m_nNumVerts = m_Header.lumps[BSP_VERTEX_LUMP].size / sizeof(bsp_vertex_t);

    m_pVerts = new bsp_vertex_t[m_nNumVerts];
    if( !m_pVerts ) return FLY_OUTOFMEMORY;

    // Read the data from the file.
    fseek( pFile,m_Header.lumps[BSP_VERTEX_LUMP].offset,SEEK_SET );
    fread( m_pVerts,sizeof(bsp_vertex_t),m_nNumVerts,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Load the face data from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadFaces( FILE* pFile )
{
    // Get the number of faces.
    m_nNumFaces = m_Header.lumps[BSP_FACE_LUMP].size / sizeof(bsp_face_t);

    m_pFaces = new bsp_face_t[m_nNumFaces];
    if( !m_pFaces ) return FLY_OUTOFMEMORY;

    // Read the data from the file.
    fseek( pFile,m_Header.lumps[BSP_FACE_LUMP].offset,SEEK_SET );
    fread( m_pFaces,sizeof(bsp_face_t),m_nNumFaces,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Read the entity string from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadEntities( FILE* pFile )
{
    // Allocate the new memory.
    m_pEntities = new char[m_Header.lumps[BSP_ENTITY_LUMP].size];
    if( !m_pEntities ) return FLY_OUTOFMEMORY;

    fseek( pFile,m_Header.lumps[BSP_ENTITY_LUMP].offset,SEEK_SET );
    fread( m_pEntities,1,m_Header.lumps[BSP_ENTITY_LUMP].size,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Read the texture array from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadTextures( FILE* pFile )
{
    // Get the number of textures.
    m_nNumTextures = m_Header.lumps[BSP_TEXTURE_LUMP].size / sizeof(bsp_texture_t);

    m_pTextures = new bsp_texture_t[m_nNumTextures];
    if( !m_pTextures ) return FLY_OUTOFMEMORY;

    // Read the data from file.
    fseek( pFile,m_Header.lumps[BSP_TEXTURE_LUMP].offset,SEEK_SET );
    fread( m_pTextures,sizeof(bsp_texture_t),m_nNumTextures,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Read the lightmap data from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadLightmaps( FILE* pFile )
{
    // Get the number of light maps.
    m_nNumLightmaps = m_Header.lumps[BSP_LIGHTMAP_LUMP].size / sizeof(bsp_lightmap_t);

    m_pLightmaps = new bsp_lightmap_t[m_nNumLightmaps];
    if( !m_pLightmaps ) return FLY_OUTOFMEMORY;

    // Read the data from the file.
    fseek( pFile,m_Header.lumps[BSP_LIGHTMAP_LUMP].offset,SEEK_SET );
    fread( m_pLightmaps,sizeof(bsp_lightmap_t),m_nNumLightmaps,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Read the splitter plane data from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadPlanes( FILE* pFile )
{
    // Get the number of planes.
    m_nNumPlanes = m_Header.lumps[BSP_PLANE_LUMP].size / sizeof(bsp_plane_t);

    m_pPlanes = new bsp_plane_t[m_nNumPlanes];
    if( !m_pPlanes ) return FLY_OUTOFMEMORY;

    // Read the data from the file.
    fseek( pFile,m_Header.lumps[BSP_PLANE_LUMP].offset,SEEK_SET );
    fread( m_pPlanes,sizeof(bsp_plane_t),m_nNumPlanes,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Read the leaf data from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadLeaves( FILE* pFile )
{
    // Get the number of leaves.
    m_nNumLeaves = m_Header.lumps[BSP_LEAF_LUMP].size / sizeof(bsp_leaf_t);

    m_pLeaves = new bsp_leaf_t[m_nNumLeaves];
    if( !m_pLeaves ) return FLY_OUTOFMEMORY;

    // Read the data from the file.
    fseek( pFile,m_Header.lumps[BSP_LEAF_LUMP].offset,SEEK_SET );
    fread( m_pLeaves,sizeof(bsp_leaf_t),m_nNumLeaves,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Read the scene node from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadNodes( FILE* pFile )
{
    // Get the number of nodes.
    m_nNumNodes = m_Header.lumps[BSP_NODE_LUMP].size / sizeof(bsp_node_t);

    m_pNodes = new bsp_node_t[m_nNumNodes];
    if( !m_pNodes ) return FLY_OUTOFMEMORY;

    // Read the data from the file.
    fseek( pFile,m_Header.lumps[BSP_NODE_LUMP].offset,SEEK_SET );
    fread( m_pNodes,sizeof(bsp_node_t),m_nNumNodes,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Read the brush data from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadBrushes( FILE* pFile )
{
    // Get the number of leaf brush index.
    m_nNumLeafBrushes = m_Header.lumps[BSP_LEAFBRUSH_LUMP].size / sizeof(int);

    m_pLeafBrushes = new int[m_nNumLeafBrushes];
    if( !m_pLeafBrushes ) return FLY_OUTOFMEMORY;

    // Read the data.
    fseek( pFile,m_Header.lumps[BSP_LEAFBRUSH_LUMP].offset,SEEK_SET );
    fread( m_pLeafBrushes,sizeof(int),m_nNumLeafBrushes,pFile );

    // Get the number of brushes.
    m_nNumBrushes = m_Header.lumps[BSP_BRUSH_LUMP].size / sizeof(bsp_brush_t);

    m_pBrushes = new bsp_brush_t[m_nNumBrushes];
    if( !m_pBrushes ) return FLY_OUTOFMEMORY;

    // Read the data.
    fseek( pFile,m_Header.lumps[BSP_BRUSH_LUMP].offset,SEEK_SET );
    fread( m_pBrushes,sizeof(bsp_brush_t),m_nNumBrushes,pFile );

    // Get the number of brush sides.
    m_nNumBrushSides = m_Header.lumps[BSP_BRUSHSIDE_LUMP].size / sizeof(bsp_brushside_t);

    m_pBrushSides = new bsp_brushside_t[m_nNumBrushSides];
    if( !m_pBrushSides ) return FLY_OUTOFMEMORY;

    // Read the data.
    fseek( pFile,m_Header.lumps[BSP_BRUSHSIDE_LUMP].offset,SEEK_SET );
    fread( m_pBrushSides,sizeof(bsp_brushside_t),m_nNumBrushSides,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Read the leaf view test (PVS) data from the map file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FILE*:      Pointer to the map file.
//------------------------------------------------------------------------
HRESULT FlyQuake3Level::ReadVisData( FILE* pFile )
{
    int nByteSize;

    fseek( pFile,m_Header.lumps[BSP_VISDATA_LUMP].offset,SEEK_SET );
    fread( &m_VisData,sizeof(int),2,pFile );

    nByteSize = m_VisData.cluster_count * m_VisData.cluster_size;

    // Create the visible data table.
    m_VisData.data = new unsigned char[nByteSize];
    if( !m_VisData.data ) return FLY_OUTOFMEMORY;

    // Read the data from file.
    fread( m_VisData.data,1,nByteSize,pFile );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Dump the map file's info into a text file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Text file to store the infos.
//------------------------------------------------------------------------
void FlyQuake3Level::DumpContents( const char* cFilename )
{
    FILE* pFile = fopen( cFilename,"w" );

    fprintf( pFile,"Quake3 Bsp level infos\n" );
    fprintf( pFile,"-----------------------\n" );
    fprintf( pFile,"Vertices:       %d\n",m_nNumVerts );
    fprintf( pFile,"Faces:          %d\n",m_nNumFaces );
    fprintf( pFile,"Planes:         %d\n",m_nNumPlanes );
    fprintf( pFile,"Textures:       %d\n",m_nNumTextures );
    fprintf( pFile,"Lightmaps:      %d\n",m_nNumLightmaps );
    fprintf( pFile,"Nodes:          %d\n",m_nNumNodes );
    fprintf( pFile,"Leaves:         %d\n",m_nNumLeaves );
    fprintf( pFile,"Vis Clousters:  %d\n",m_VisData.cluster_count );
    fprintf( pFile,"\n" );

    fprintf( pFile,"Entities {\n" );
    fprintf( pFile,"%s",m_pEntities );
    fprintf( pFile,"}\n\n" );

    fprintf( pFile,"Textures {\n" );
    for( int i=0;i<m_nNumTextures;i++ )
    {
        fprintf( pFile,"    %d: %s\n",i,m_pTextures[i].name );
    }
    fprintf( pFile,"}\n\n" );

    fclose( pFile );
}