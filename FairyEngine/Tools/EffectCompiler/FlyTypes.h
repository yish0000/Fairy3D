//------------------------------------------------------------------------
// Name: FlyTypes.h
// Desc: This file define the structures and enumerations for the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_TYPES_H__
#define __FLY_TYPES_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"
#include "stl_headers.h"

#include "FlyColor.h"
#include "FlyMath.h"
//////////////////////////////////////////////////////////////////////////


// Priority for the renderable objects.
#define PRIORITY_ALPHA      999     // Render the objects which have transparency effect.
#define PRIORITY_HIGH       1       // Render the high priority objects.
#define PRIORITY_DEFAULT    0       // Render ordinary objects.
#define PRIORITY_BACKGROUND -1      // Render the background.
#define PRIORITY_SKYBOX     -2      // Render the skybox.
#define PRIORITY_LOW        -3      // Render the low priority objects.


// Shader types for renderdevice.
#define SHADER_VERTEX       0
#define SHADER_PIXEL        1

//////////////////////////////////////////////////////////////////////////
//// Enumerations for this engine.


// Enumerations to define vertex formats.
typedef enum _ELEMENTTYPE
{
    ET_FLOAT1,
    ET_FLOAT2,
    ET_FLOAT3,
    ET_FLOAT4,
    ET_COLOR,
    ET_UBYTE4,
} ELEMENTTYPE;

typedef enum _ELEMENTUSAGE
{
    EU_POSITION,        // Specify the untransformed vertex.
    EU_DIFFUSE,         // Specify the diffuse color.
    EU_NORMAL,          // Specify the normal for lighting.
    EU_SPECULAR,        // Specify the specular color.
    EU_BLENDWEIGHT,     // Use in the animation.
    EU_BLENDINDEX,      // Use in the animation.
    EU_POSITIONT,       // Specify the transformed vertex.

    EU_TEXCOORD,
    EU_TAGENT,
    EU_BINORMAL,
} ELEMENTUSAGE;

// Define some VertexFormat constant values.
typedef int VERTEXTYPE;

#define VF_DEFAULT          0           // Usually use this format. (Position,normal,texCoord).
#define VF_DEFAULTNOLIGHT   1           // Without the light. (Position,color,texCoord).
#define VF_TWOTEXTURE       2           // Render with two texCoords.
#define VF_ANIMATION        3           // Vertex format for the animation.
#define VF_POSITION         4           // Vertex format which only contain the position.
#define VF_STENCILSHADOW    5           // Vertex format only for the stencil shadow.

#define VF_USER(index)      (index+5)   // Macro for the userdefine vFormat.
#define VF_USER1            VF_USER(1)  // Macro for the userdefine vFormat1.
#define VF_USER2            VF_USER(2)  // Macro for the userdefine vFormat2.
#define VF_USER3            VF_USER(3)  // Macro for the userdefine vFormat3.
#define VF_USER4            VF_USER(4)  // Macro for the userdefine vFormat4.

//////////////////////////////////////////////////////////////////////////

typedef enum _RENDERTYPE
{
    RT_POINTLIST,
    RT_LINELIST,
    RT_LINESTRIP,
    RT_TRIANGLELIST,
    RT_TRIANGLESTRIP,
    RT_TRIANGLEFAN,
} RENDERTYPE;

typedef enum _FLYCMPFUNC
{
    CMP_NEVER,
    CMP_LESS,
    CMP_EQUAL,
    CMP_LESSEQUAL,
    CMP_GREATER,
    CMP_NOTEQUAL,
    CMP_GREATEREQUAL,
    CMP_ALWAYS,
} FLYCMPFUNC;

typedef enum _CULLMODE
{
    CM_NONE,
    CM_CW,
    CM_CCW,
} CULLMODE;

typedef enum _SHADEMODE
{
    SM_FLAT,
    SM_GOURAUD,
} SHADEMODE;

typedef enum _POLYGONMODE
{
    PM_POINT,
    PM_WIREFRAME,
    PM_SOLID,
} POLYGONMODE;

//////////////////////////////////////////////////////////////////////////

typedef enum _FILTEROPTION
{
    FILTER_NONE,
    FILTER_POINT,
    FILTER_LINEAR,
    FILTER_ANISOTROPIC,
    FILTER_PYRAMIDALQUAD,
    FILTER_GAUSSIANQUAD,
} FILTEROPTION;

typedef enum _BLENDTYPE
{
    BT_COLOR,
    BT_ALPHA,
} BLENDTYPE;

typedef enum _BLENDSOURCE
{
    BSTA_CURRENT,
    BSTA_DIFFUSE,
    BSTA_TEXTURE,
    BSTA_SPECULAR,
} BLENDSOURCE;

typedef enum _TEXTUREADDRESS
{
    TADDR_WRAP,
    TADDR_MIRROR,
    TADDR_CLAMP,
    TADDR_BORDER,
    TADDR_MIRRORONCE,
} TEXTUREADDRESS;

typedef enum _TEXTUREOPERATION
{
    TOP_DISABLE,
    TOP_SELECTARG1,
    TOP_SELECTARG2,
    TOP_MODULATE,
    TOP_MODULATEX2,
    TOP_MODULATEX4,
    TOP_ADD,
    TOP_ADDSIGNED,
    TOP_ADDSMOOTH,
    TOP_SUBTRACT,
    TOP_BLENDDIFFUSEALPHA,
    TOP_BLENDTEXTUREALPHA,
    TOP_BLENDCURRENTALPHA,
    TOP_DOTPRODUCT3,

    // The same as BLENDDIFFUSEALPHA, 
    // But the alpha value is given by user
    TOP_BLENDMANUAL,
} TEXTUREOPERATION;

typedef struct _TEXTUREBLENDMODE
{
    BLENDTYPE           Type;           // Use which blend type, Color/Alpha.
    BLENDSOURCE         Arg1;           // The first Source.
    BLENDSOURCE         Arg2;           // The second source.
    TEXTUREOPERATION    Operation;      // Use which operation.
    float               fFactor;        // Manual factor.
} TEXTUREBLENDMODE;

typedef enum _SCENEBLENDMODE
{
    SBM_ZERO,
    SBM_ONE,
    SBM_SRCCOLOR,
    SBM_INVSRCCOLOR,
    SBM_SRCALPHA,
    SBM_INVSRCALPHA,
    SBM_DESTCOLOR,
    SBM_INVDESTCOLOR,
    SBM_DESTALPHA,
    SBM_INVDESTALPHA,
} SCENEBLENDMODE;

//////////////////////////////////////////////////////////////////////////

typedef enum _DEPTHMODE
{
    DEPTH_NONE,
    DEPTH_READWRITE,
    DEPTH_READONLY,
} DEPTHMODE;

typedef enum _STENCILOPERATION
{
    STOP_KEEP,
    STOP_ZERO,
    STOP_REPLACE,
    STOP_INCREMENT,
    STOP_DECREMENT,
    STOP_INVERT,
} STENCILOPERATION;

typedef enum _FOGMODE
{
    FOG_NONE,
    FOG_EXP,
    FOG_EXP2,
    FOG_LINEAR,
} FOGMODE;

typedef enum _LIGHTTYPE
{
    LGT_POINT,
    LGT_DIRECTIONAL,
    LGT_SPOT,
} LIGHTTYPE;

typedef enum _PIXELFORMAT
{
    PFT_UNKNOWN,

    PFT_R8G8B8,
    PFT_A8R8G8B8,
    PFT_A8B8G8R8,
    PFT_X8R8G8B8,
    PFT_X8B8G8R8,
    PFT_R5G6B5,

    // Format for the image file.
    PFT_B8G8R8,
    PFT_R8G8B8A8,
    PFT_B8G8R8A8,
} PIXELFORMAT;

typedef enum _INDEXFORMAT
{
    IF_INDEX16,
    IF_INDEX32,
} INDEXFORMAT;

typedef enum _FLYIMAGETYPE
{
    FIMAGE_BMP,     // Bitmap image.
    FIMAGE_DDS,     // DirectDraw surface.
    FIMAGE_JPG,     // JPEG image.
    FIMAGE_TEX,     // Engine's image format.
    FIMAGE_PNG,     // Portable network graphic.
    FIMAGE_TGA,     // TGA image.
} FLYIMAGETYPE;

typedef enum _TEXTUREUSAGE
{
    TU_DEFAULT,
    TU_RENDERTARGET,
} TEXTUREUSAGE;

typedef enum _TEXTUREACCESS
{
    TMA_READONLY,   // Read-only access.
    TMA_WRITEONLY,  // Write-only access.
    TMA_READWRITE,  // Read-write access.
} TEXTUREACESS;

typedef enum _TEXTURETYPE
{
    TT_2D,
    TT_3D,
    TT_CUBEMAP,
} TEXTURETYPE;

typedef enum _CUBEFACE
{
    CF_RIGHT,
    CF_LEFT,
    CF_TOP,
    CF_BOTTOM,
    CF_FRONT,
    CF_BACK,
} CUBEFACE;


//////////////////////////////////////////////////////////////////////////
//// Structures for this engine.

// Light info for the device.
struct FLYLIGHT
{
    LIGHTTYPE   Type;           // Type of the light
    FLYCOLOR    cDiffuse;       // RGBA Diffuse light value
    FLYCOLOR    cAmbient;       // RGBA Ambient light value
    FLYCOLOR    cSpecular;      // RGBA Specular light value
    FlyVector   vPosition;      // Position of the light
    FlyVector   vDirection;     // Direction of the light
    float       fRange;         // Range of the light
    float       fTheta;         // Angle of the spot light inner cone
    float       fPhi;           // Angle of the spot light outer cone
    float       fAttenuation0;  // Change of idensity over distance
    float       fAttenuation1;  // Change of idensity over distance
};

// Standard material for the device.
struct FLYMATERIAL
{
    FLYCOLOR cDiffuse;          // Diffuse color value.
    FLYCOLOR cAmbient;          // Ambient color value.
    FLYCOLOR cSpecular;         // Specular color value.
    FLYCOLOR cEmissive;         // Emissive color value.
    float fPower;               // Sharpness of specular highlights.
};

// Header of the engine texture.
typedef struct _FLYIMAGEHEADER
{
    DWORD   dwVersion;          // File version.
    UINT    nSizePerPixel;      // Size of each Pixel.
    UINT    nWidth;             // Width of image. (Pixel)
    UINT    nHeight;            // Height of image. (Pixel)
    INT     nFlags;             // Flags for image.
} FLYIMAGEHEADER;

// Default vertex structures.
typedef struct _VERTEX
{
    float x,y,z;
    float vN[3];
    float tu,tv;
} VERTEX;

typedef struct _VERTEXL
{
    float x,y,z;
    DWORD color;
    float tu,tv;
} VERTEXL;

typedef struct _VERTEX2T
{
    float x,y,z;
    float vN[3];
    float tu0,tv0;
    float tu1,tv1;
} VERTEX2T;

typedef struct _AVERTEX
{
    float x,y,z;
    float vN[3];
    float tu,tv;
    float fBone1,fWeight1;
    float fBone2,fWeight2;
} AVERTEX;

typedef struct _SVERTEX
{
    float x,y,z,rhw;
    DWORD color;
} SVERTEX;

//////////////////////////////////////////////////////////////////////////

// To store the RenderDevice's capacilities.
typedef struct _DEVICECAPS
{
    DWORD dwVShaderVersion;         // Vertex shader version.
    DWORD dwPShaderVersion;         // Pixel shader version.
    UINT nMaxTextures;              // Max count of the texture stages.
    UINT nMaxPrimitiveCount;        // Max count of primitive count of one call.
    UINT nMaxStreamStride;          // Max count of vertex stride.
    DWORD dwMaxTextureWidth;        // Max width of the texture.
    DWORD dwMaxTextureHeight;       // Max height of the texture.
    UINT nMaxVertexTextures;        // Max number of vertex textures.

    bool bAlphaTexture;             // Can use the alpha effect ?
    bool bCubeMap;                  // Can use the cube map ?
    bool bVolumeMap;                // Can use the volume map ?
    bool bMipCubeMap;               // Can use mip cube map ?
    bool bMipMap;                   // Can use mip map ?
    bool bMipVolumeMap;             // Can use mip volume map ?
    bool bTableFog;                 // Can use table fog ?
    bool bVertexFog;                // Can use vertex fog ?
} DEVICECAPS;


//////////////////////////////////////////////////////////////////////////
//// PRE-DEFINE ALL THE CLASSES.

class FlyAnimation;
class FlyAnimEntity;
class FlyAnimRenderable;
class FlyAnimManager;

class FlyBitset;

class FlyCameraBase;
class FlyCamManager;
class FlyCamFactory;

class FlyMesh;
class FlyMeshEntity;
class FlyMeshManager;
class FlySubMesh;

class FlyManualEntity;

class FlyLightObject;

class FlyParticle;
class FlyParticleController;
class FlyParticleEffect;
class FlyParticleEmitter;
class FlyParticleEntity;
class FlyParticleRenderUnit;
class FlyParticleSystem;

class FlySceneFactory;
class FlySceneManager;
class FlySceneNode;
class FlySceneObject;
class FlyEntityFactory;

class FlyTimer;

class FlyFont;
class FlyRenderable;
class FlyRenderDevice;
class FlyRenderVariable;
class FlyRenderAnnotation;
class FlyRenderAutoParameter;
class FlyRenderParameter;
class FlyRenderEffect;
class FlyRenderEffectCompiler;
class FlyRenderTechnique;
class FlyRenderPass;
class FlyRenderer;
class FlyRenderMaterial;
class FlyRenderQueue;
class FlyResourceManager;
class FlyShaderObject;
class FlyTexture;

typedef class FlyRenderDevice* LPFLYRENDERDEVICE;
typedef class FlyRenderer* LPFLYRENDERER;
typedef class FlyResourceManager* LPFLYRESOURCEMANAGER;
typedef class FlyTexture* LPFLYTEXTURE;

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_TYPES_H__