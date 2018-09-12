/*************************************************************************
*  Name: SMDStructs.h                                                    *
*                                                                        *
*  Desc: A file to define some structs for our SMD model file            *
*        Exporter!                                                       *
*                                                                        *
*  CopyRight (C) FlySoft Corp. All right Observed.                       *
*************************************************************************/


#ifndef __SMD_STRUCTS_H__
#define __SMD_STRUCTS_H__


//// STRUCTS OF THIS EXPORTER ////////////////////////////////////////////

/*************************************************************************
*    Structs to store the vertex data                                    *
*************************************************************************/
typedef struct _VERTEXDATA
{
	float x;
	float y;
	float z;
} VERTEXDATA;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs to store the face data                                      *
*************************************************************************/
typedef struct _FACEDATA
{
	WORD	v1;			// Index of vertex1
	WORD	v2;			// Index of vertex2
	WORD	v3;			// Index of vertex3
	float	vN[3];		// Normal of this face
} FACEDATA;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs to store the TextureCoords                                  *
*************************************************************************/
typedef struct _TEXTUREDATA
{
	float	tu;
	float	tv;
} TEXTUREDATA;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs to store the texture coords face data                       *
*************************************************************************/
typedef struct _TEXFACEDATA
{
	WORD	t1;
	WORD	t2;
	WORD	t3;
} TEXFACEDATA;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs of color value                                              *
*************************************************************************/
typedef struct _SMDCOLOR
{
	float	fR;		// Red color value
	float	fG;		// Green color value
	float	fB;		// Blue color value
	float	fA;		// Alpha value
} SMDCOLOR;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs of exported vertices                                        *
*************************************************************************/
typedef struct _SMDVERTEX
{
    float x,y,z;    // Position of the vertex
    float vN[3];    // Normal of the vertex.
    float tu,tv;    // Coords of texture
} SMDVERTEX;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs of exported faces                                           *
*************************************************************************/
typedef struct _SMDFACE
{
    WORD    v1;     // Vertex 1
    WORD    v2;     // Vertex 2
    WORD    v3;     // Vertex 3
    LONG    nMatID; // ID of the material
} SMDFACE;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs of file header                                              *
*************************************************************************/
typedef struct _SMDHEADER
{
    DWORD   dwVersion;      // Version of this file
    LONG    nNumMaterials;  // Number of Materials
    LONG    nNumMeshes;     // Number of meshes
} SMDHEADER;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs to store the material data                                  *
*************************************************************************/
typedef struct _SMDMATERIAL
{
    bool        bAlpha;         // Use the alpha map ?
	SMDCOLOR	cAmbient;       // Ambient light of material
	SMDCOLOR	cDiffuse;       // Diffuse light of material
	SMDCOLOR	cEmissive;      // Emissive light of material
	SMDCOLOR	cSpecular;      // Specular light of material
	float		fPower;         // Power value of specular
	TSTR        sTexture;       // Path of the texture file.
} SMDMATERIAL;

//------------------------------------------------------------------------

/*************************************************************************
*    Structs to store the MESH array                                     *
*************************************************************************/
typedef struct _SMDMESH
{
    UINT    nMaterialID;        // Material ID
    UINT    nNumVertices;       // Number of vertices
    UINT    nNumFaces;          // Number of faces
    SMDVERTEX* pVertices;       // Vertices arrays
    WORD*   pIndices;           // Index arrays
} SMDMESH, *LPSMDMESH;

//////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __SMD_STRUCTS_H__