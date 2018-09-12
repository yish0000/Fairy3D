//------------------------------------------------------------------------
// Name: FlyQuake3Types.h
// Desc: This file define the Types and structures of Quake3's bsp level
//       file. This code is from Quake III's source, id software.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_QUAKE3_TYPES_H__
#define __FLY_QUAKE3_TYPES_H__


#include "FlyTypes.h"


//////////////////////////////////////////////////////////////////////////
//
// bsp contents
//
//////////////////////////////////////////////////////////////////////////

#define BSP_ENTITY_LUMP     0
#define BSP_TEXTURE_LUMP    1
#define BSP_PLANE_LUMP      2
#define BSP_NODE_LUMP       3
#define BSP_LEAF_LUMP       4
#define BSP_LEAFFACE_LUMP   5
#define BSP_LEAFBRUSH_LUMP  6
#define BSP_MODEL_LUMP      7
#define BSP_BRUSH_LUMP      8
#define BSP_BRUSHSIDE_LUMP  9
#define BSP_VERTEX_LUMP     10
#define BSP_MESHINDEX_LUMP  11
#define BSP_EFFECT_LUMP     12
#define BSP_FACE_LUMP       13
#define BSP_LIGHTMAP_LUMP   14
#define BSP_LIGHTVOL_LUMP   15
#define BSP_VISDATA_LUMP    16

struct bsp_lump_entry_t {
    int offset;
    int size;
};

struct bsp_header_t {
    char magic[4];
    int version;
    bsp_lump_entry_t lumps[17];
};

struct bsp_vertex_t {
    float point[3];
    float texture[2];
    float lightmap[2];
    float normal[3];
    int color;
};

#define BSP_FACE_POLYGON    1
#define BSP_FACE_PATCH      2
#define BSP_FACE_MESH       3
#define BSP_FACE_BILLBOARD  4

struct bsp_face_t {
    int shader;         // shader ref
    int unknown;
    int type;           // face type
    int vert_start;
    int vert_count;
    int elem_start;
    int elem_count;
    int lm_texture;     // lightmap
    int lm_offset[2];
    int lm_size[2];
    float org[3];       // facetype_normal only
    float bbox[6];      // facetype_patch only
    float normal[3];    // facetype_normal only
    int mesh_cp[2];     // patch control point dims
};

struct bsp_texture_t {
    char name[64];
    int flags;
    int contents;
};

struct bsp_plane_t {
    float normal[3];
    float dist;
};

struct bsp_lightmap_t {
    unsigned char data[128*128*3];
};

struct bsp_leaf_t {
    int cluster;    // visibility cluster number
    int area;
    int mins[3];
    int maxs[3];
    int face_start;
    int face_count;
    int brush_start;
    int brush_count;
};

struct bsp_node_t {
    int plane;          // dividing plane
    int front;
    int back;
    int mins[3];
    int maxs[3];
};

struct bsp_model_t {
    float bbox[6];
    int face_start;
    int face_count;
    int brush_start;
    int brush_count;
};

struct bsp_vis_t {
    int cluster_count;
    int cluster_size;
    unsigned char* data;
};

struct bsp_brushside_t {
    int planenum;
    int content;
};

struct bsp_brush_t {
    int firstside;
    int numsides;
    int shaderIndex;
};

//////////////////////////////////////////////////////////////////////////

struct sBspVertex
{
    float x,y,z;
    float vN[3];
    float tu0,tv0;
    float tu1,tv1;

    sBspVertex operator + ( const sBspVertex& vert ) const
    {
        sBspVertex res;

        res.x = x + vert.x;
        res.y = y + vert.y;
        res.z = z + vert.z;
        res.vN[0] = vN[0] + vert.vN[0];
        res.vN[1] = vN[1] + vert.vN[1];
        res.vN[2] = vN[2] + vert.vN[2];
        res.tu0 = tu0 + vert.tu0;
        res.tv0 = tv0 + vert.tv0;
        res.tu1 = tu1 + vert.tu1;
        res.tv1 = tv1 + vert.tv1;

        return res;
    }

    sBspVertex operator * ( float f ) const
    {
        sBspVertex res;

        res.x = x * f;
        res.y = y * f;
        res.z = z * f;
        res.vN[0] = vN[0] * f;
        res.vN[1] = vN[1] * f;
        res.vN[2] = vN[2] * f;
        res.tu0 = tu0 * f;
        res.tv0 = tv0 * f;
        res.tu1 = tu1 * f;
        res.tv1 = tv1 * f;

        return res;
    }
};

struct sBspFace
{
    int nMatID;
    int nTextureID;
    int nLightID;
    int nNumVerts;
    int nNumIndis;
    sBspVertex* pVerts;
    WORD* pIndis;
    FlyAabb BBox;
};

struct sBspVisData
{
    int nNumClusters;
    int nRowLength;
    unsigned char* tableData;
};

// Store the brush data to test
// collision.
struct sBspBrush
{
    std::vector<FlyPlane> planes;
};

typedef std::vector<sBspBrush*> NodeBrushList;

//////////////////////////////////////////////////////////////////////////

#endif