//------------------------------------------------------------------------
// Name: FlyAnimFormat.h
// Desc: This file is the Definition of the animation *.amd and *.anm
//       file format. 
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_ANIM_FORMAT_H__
#define __FLY_ANIM_FORMAT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


// Define some macros for each chunks.
#define CHK_HEADER              0x0100
#define CHK_VERTEX              0x0200
#define CHK_VERTEX_ORIG         0x0210
#define CHK_FACE                0x0300
#define CHK_MESH                0x0400
#define CHK_MATERIAL            0x0500

#define CHK_ANIMHEADER          0x0600
#define CHK_JOINT               0x0700
#define CHK_JOINT_MAIN          0x0710
#define CHK_JOINT_KEYFRAME_POS  0x0720
#define CHK_JOINT_KEYFRAME_ROT  0x0730
#define CHK_ANIMATION           0x0800
#define CHK_END                 0x9999

#define AMD_VERSION( a,b )      (DWORD)( (a<<16) | b )

#define NULL_BONEID             65535
#define MAX_BONECOUNT           4

//////////////////////////////////////////////////////////////////////////

// Structure for chunk header.
typedef struct sChunk
{
    WORD wIdentifier;       // Identifier of the data chunk.
    LONG nChunkSize;        // Size of the data chunk.
} *LPCHUNK;

// Structure for the model header.
typedef struct sModelHeader
{
    std::string sAuthor;    // Author name.
    std::string sModelName; // Name of the model.
    UINT nBonesPerVert;     // Number of bones per vertex.
    UINT nNumVerts;         // Whole number of vertices.
    UINT nNumFaces;         // Whole number of Faces.
    UINT nNumMeshes;        // Whole number of meshes.
    UINT nNumMaterials;     // Whole number of materials.
} *LPMODELHEADER;

// Structure for the model vertices.
typedef struct sVertex
{
    float fXYZ[3];          // vertex Position data.
    float fUV0[2];          // vertex texmap coords.
    FlyVector vNormal;      // vertex normal data.
    UINT nBoneID[4];        // Bone ID array.
    float fWeight[4];       // Bone weight array.
} *LPVERTEX;

typedef struct sVertexOrig
{
    float fXYZ[3];          // Original position data. (The 1st frame)
} *LPVERTEXORIG;

// Structure for the faces.
typedef struct sFace
{
    WORD wIndices[3];       // Indices of the face.
    UINT nMeshID;           // Which mesh belong to ?
    UINT nMaterialID;       // Material ID of the face.
} *LPFACE;

// Structure for the mesh.
typedef struct sMesh
{
    std::string sName;      // Name of the mesh.
    UINT nOffsetVerts;      // Offset of the vertex datas.
    UINT nNumVerts;         // Number of the vertices.
    UINT nOffsetFaces;      // Offset of the face datas.
    UINT nNumFaces;         // Number of the faces.
    UINT nMaterialID;       // Material ID of the mesh.
} *LPMESH;

// Structure for the material.
typedef struct sMaterial
{
    float fAmbient[4];      // Ambient color value.
    float fDiffuse[4];      // Diffuse color value.
    float fSpecular[4];     // Specular color value.
    float fEmissive[4];     // Emissive color value.
    float fPower;           // Specular power value.
    std::string sTexture_A; // The first texture file name.
    std::string sTexture_B; // The second texture file name.
} *LPMATERIAL;

// Structure for the Animation file header.
typedef struct sAnimHeader
{
    std::string sAuthor;    // Author of the animation file.
    std::string sAnimName;  // Name of the animation.
    UINT nNumJoints;        // Number of joints.
    UINT nNumFrames;        // Number of frames.
} *LPANIMHEADER;

// Structure for the keyframe position.
typedef struct sKeyframe_Pos
{
    float fTime;            // Time of the keyframe.
    FlyVector vPosition;    // Position data.
} *LPKEYFRAME_POS;

// Structure for the keyframe rotation.
typedef struct sKeyframe_Rot
{
    float fTime;            // Time of the keyframe.
    FlyQuat qRotation;      // Rotation data.
} *LPKEYFRAME_ROT;

// Structure for the joints.
typedef struct sJoint
{
    std::string sName;              // Name of the joint.
    std::string sParentName;        // Name of the parent joint.
    WORD wParentID;                 // Parent joint's ID.
    FlyVector vPosition;            // Current position value.
    FlyQuat qRotation;              // Current rotation value.
    WORD wNumKF_Position;           // Number of position keyframe.
    WORD wNumKF_Rotation;           // Number of rotation keyframe.
    LPKEYFRAME_POS pKF_Position;    // Position keyframe array.
    LPKEYFRAME_ROT pKF_Rotation;    // Rotation keyframe array.
    FlyMatrix mMatrix_Relative;     // 
    FlyMatrix mMatrix_Absolute;
} *LPJOINT;

// Structure for the whole model.
typedef struct sModel
{
    sModelHeader header;    // Header of the model.
    LPVERTEX pVerts;        // Vertices datas.
    LPVERTEX pVerts_orig;   // Original vertices datas.
    LPFACE pFaces;          // Face datas.
    LPMESH pMeshes;         // Mesh datas.
    LPMATERIAL pMaterials;  // Material datas.
    sAnimHeader animHeader; // Header of the animation.
    LPJOINT pJoints;        // Joint datas.
} *LPMODEL;

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_ANIM_FORMAT_H__