//------------------------------------------------------------------------
// Name: FlyQuake3Level.h
// Desc: This file define a class to load the Quake III's map data.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_QUAKE3_LEVEL_H__
#define __FLY_QUAKE3_LEVEL_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyQuake3Types.h"

//////////////////////////////////////////////////////////////////////////


class FlyBspLevel;
class FlyBspNode;
class FlyBspSceneManager;

/*
* Class To load and store the Quake III's map data.
*/
class _FLY_EXPORT FlyQuake3Level
{
    friend class FlyBspLevel;

protected:
    std::string m_sFilename;        // To store the filename of the map.

    bsp_header_t m_Header;          // To store the header.
    int m_nNumVerts;                // Count of the vertices.
    bsp_vertex_t* m_pVerts;         // Vertex data.
    int m_nNumFaces;                // Count of faces.
    bsp_face_t* m_pFaces;           // Face data.
    char* m_pEntities;              // To store entity data.

    int m_nNumLeafFaces;            // Count of leaf faces.
    int* m_pLeafFaces;              // Leaf face index array.
    int m_nNumMeshIndices;          // Count of mesh index.
    int* m_pMeshIndices;            // Mesh index array.
    int m_nNumLeafBrushes;          // Count of leaf brushes.
    int* m_pLeafBrushes;            // Leaf brushes array.

    int m_nNumPlanes;               // Count of planes.
    bsp_plane_t* m_pPlanes;         // Plane array.
    int m_nNumNodes;                // Count of bsp nodes.
    bsp_node_t* m_pNodes;           // Bsp node array.
    int m_nNumLeaves;               // Count of leaves.
    bsp_leaf_t* m_pLeaves;          // Leaf array.
    int m_nNumTextures;             // Count of textures.
    bsp_texture_t* m_pTextures;     // Texture array.
    int m_nNumLightmaps;            // Count of lightmaps.
    bsp_lightmap_t* m_pLightmaps;   // Lightmap array.
    bsp_vis_t m_VisData;            // Visible face data.
    int m_nNumBrushes;              // Count of brushes.
    bsp_brush_t* m_pBrushes;        // Brush array.
    int m_nNumBrushSides;           // Count of brush sides.
    bsp_brushside_t* m_pBrushSides; // Brush side array.

    HRESULT ReadVertices( FILE* pFile );
    HRESULT ReadFaces( FILE* pFile );
    HRESULT ReadPlanes( FILE* pFile );
    HRESULT ReadEntities( FILE* pFile );
    HRESULT ReadNodes( FILE* pFile );
    HRESULT ReadLeaves( FILE* pFile );
    HRESULT ReadVisData( FILE* pFile );
    HRESULT ReadTextures( FILE* pFile );
    HRESULT ReadLightmaps( FILE* pFile );
    HRESULT ReadBrushes( FILE* pFile );
    HRESULT ReadShaders( FILE* pFile );

public:
    // Constructor and destructor.
    FlyQuake3Level(void);
    ~FlyQuake3Level(void);

    HRESULT LoadQuake3Map( const char* cFilename );
    void DumpContents( const char* cFilename );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_QUAKE3_LEVEL_H__