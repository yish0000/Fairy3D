/*
 * ------------------------------------------------------------------------
 *  Name:   F3DTypes.h
 *  Desc:   This file define some basic types for 3d engine.
 *  Author: Yish
 *  Date:   2011/6/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __F3D_TYPES_H__
#define __F3D_TYPES_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"
#include "FTemplate.h"

#include "F3DColor.h"
#include "F3DMath.h"

///////////////////////////////////////////////////////////////////////////

//
#define MAX_TEXTURE_STAGES  8

// Max count of dynamic lights of each pass.
#define MAX_DYNAMIC_LIGHTS  8

///////////////////////////////////////////////////////////////////////////
//  
//  Some enumerations for engine.
//  
///////////////////////////////////////////////////////////////////////////

/** Element type for vertex.
*/
enum EElementType
{
    ET_FLOAT1,
    ET_FLOAT2,
    ET_FLOAT3,
    ET_FLOAT4,
    ET_COLOR,
    ET_UBYTE4,
};

/** Usage of the vertex element.
*/
enum EElementUsage
{
    EU_POSITION,        // Position(Not transformed)
    EU_DIFFUSE,         // Color
    EU_NORMAL,          // Normal
    EU_SPECULAR,        // Specular color
    EU_BLENDWEIGHT,     // Blend weight for vertex(SkinModel)
    EU_BLENDINDEX,      // Bone index(SkinModel)
    EU_POSITIONT,       // Position(Transformed)
	EU_PSIZE,			// Size of the point.
	EU_TESSFACTOR,		// 

    EU_TEXCOORD,        // Texture coordinates
    EU_TANGENT,         // Tangent
    EU_BINORMAL,        // 
};

///////////////////////////////////////////////////////////////////////////

/** Primitive types.
*/
enum ERenderType
{
    RT_POINTLIST,       // Renders the vertices as a collection of isolated points.
	RT_LINELIST,        // Renders the vertices as a list of isolated straight line segments. 
    RT_LINESTRIP,       // Renders the vertices as a single polyline. 
    RT_TRIANGLELIST,    // Renders the specified vertices as a sequence of isolated triangles.
	RT_TRIANGLESTRIP,   // Renders the vertices as a triangle strip. The backface-culling flag is automatically flipped on even-numbered triangles.
    RT_TRIANGLEFAN,     // Renders the vertices as a triangle fan. 
};

/** Compare function for some operations.
For example: Z-test, alpha-test, stencil-test...
*/
enum ECompareFunc
{
    CMP_NEVER,          // Always fail the test. 
    CMP_LESS,           // Accept the new pixel if its value is less than the value of the current pixel. 
    CMP_EQUAL,          // Accept the new pixel if its value equals the value of the current pixel.
    CMP_LESSEQUAL,      // Accept the new pixel if its value is less than or equal to the value of the current pixel.
    CMP_GREATER,        // Accept the new pixel if its value is greater than the value of the current pixel.
    CMP_NOTEQUAL,       // Accept the new pixel if its value does not equal the value of the current pixel.
    CMP_GREATEREQUAL,   // Accept the new pixel if its value is greater than or equal to the value of the current pixel.
    CMP_ALWAYS,         // Always pass the test.
};

/** Culling modes.
*/
enum ECullMode
{
    CULL_NONE,          // Do not cull back faces. 
    CULL_CW,            // Cull back faces with clockwise vertices. 
    CULL_CCW,           // Cull back faces with counterclockwise vertices. 
};

/** Shading mode.
*/
enum EShadeMode
{
	// Flat shading mode. The color and specular component of the first vertex in the triangle
	// are used to determine the color and specular component of the face. 
    SM_FLAT,

	// Gouraud shading mode. The color and specular components of the face are determined by
	// a linear interpolation between all three of the triangle's vertices. 
    SM_GOURAUD,
};

/** Fill mode.
*/
enum EFillMode
{
    FM_POINT,           // Fill points. 
    FM_WIREFRAME,       // Fill wireframes. 
    FM_SOLID,           // Fill solids. 
};

///////////////////////////////////////////////////////////////////////////

/** Texture filter options.
*/
enum EFilterOption
{
    FILTER_NONE,            // Disables mipmapping. (Only for mipmap filter)
    FILTER_POINT,           // Nearest point.
    FILTER_LINEAR,          // Linear filtering.
    FILTER_ANISOTROPIC,     // Anisotropic texture filtering. (Only for magfilter and minfilter)

	// A 4-sample tent filter used as a texture magnification or minification filter
	// Only for magfilter and minfilter.
	FILTER_PYRAMIDALQUAD,

	// A 4-sample Gaussian filter used as a texture magnification or minification filter
	// Only for magfilter and minfilter.
    FILTER_GAUSSIANQUAD,
};

/** Texture-addressing modes.
*/
enum ETextureAddress
{
    TADDR_WRAP,             // Tile the texture at every integer junction.
    TADDR_MIRROR,           // Similar to TADDR_WRAP, except that the texture is flipped at every integer junction.
    TADDR_CLAMP,            // Texture coordinates outside the range[0.0, 1.0] are set to the texture color at [0.0, 1.0], respectively.
    TADDR_BORDER,           // Texture coordinates outside the range[0.0, 1.0] are set to the border color.
    TADDR_MIRRORONCE,       // Mirror once, and then clamps to the maximum value.
};

///////////////////////////////////////////////////////////////////////////

/** Depth buffer mode.
*/
enum EDepthMode
{
    DEPTH_NONE,             // Disable the depth buffer.
    DEPTH_READWRITE,        // We can read and write the buffer.
    DEPTH_READONLY,         // The depth buffer will not be write.
};

/** Stencil buffer operation.
*/
enum EStencilOperation
{
    STOP_KEEP,
    STOP_ZERO,
    STOP_REPLACE,
    STOP_INCREMENT,
    STOP_DECREMENT,
    STOP_INVERT,
	STOP_INCREMENT_WRAP,
	STOP_DECREMENT_WRAP,
};

/** Blend option for output-merger.
*/
enum EBlendOption
{
	BLEND_ZERO = 1,
	BLEND_ONE = 2,
	BLEND_SRC_COLOR = 3,
	BLEND_INV_SRC_COLOR = 4,
	BLEND_SRC_ALPHA = 5,
	BLEND_INV_SRC_ALPHA = 6,
	BLEND_DEST_ALPHA = 7,
	BLEND_INV_DEST_ALPHA = 8,
	BLEND_DEST_COLOR = 9,
	BLEND_INV_DEST_COLOR = 10,
	BLEND_SRC_ALPHA_SAT = 11,
	BLEND_BLEND_FACTOR = 14,
	BLEND_INV_BLEND_FACTOR = 15,
	BLEND_SRC1_COLOR = 16,
	BLEND_INV_SRC1_COLOR = 17,
	BLEND_SRC1_ALPHA = 18,
	BLEND_INV_SRC1_ALPHA = 19,
};

/** Blend operation for output-merger.
*/
enum EBlendOperation
{
	BLEND_OP_ADD = 1,
	BLEND_OP_SUBTRACT = 2,
	BLEND_OP_REV_SUBTRACT = 3,
	BLEND_OP_MIN = 4,
	BLEND_OP_MAX = 5,
};

/** Color write mask
*/
enum EColorWriteMask
{
	CMASK_RED = F_BIT(0),
	CMASK_GREEN = F_BIT(1),
	CMASK_BLUE = F_BIT(2),
	CMASK_ALPHA = F_BIT(3),

	CMASK_ALL = CMASK_RED | CMASK_GREEN | CMASK_BLUE | CMASK_ALPHA,
};

///////////////////////////////////////////////////////////////////////////

/** Fog mode.
*/
enum EFogMode
{
    FOG_NONE,               //
    FOG_EXP,                //
    FOG_EXP2,               //
    FOG_LINEAR,             // 
};

/** Type of light source.
*/
enum ELightType
{
    LGT_POINT,              //
    LGT_DIRECTIONAL,        //
    LGT_SPOT,               //
};

/** Pixel format.
*/
enum EPixelFormat
{
    PFT_UNKNOWN,            // 0 Bytes

    PFT_R8G8B8,             // 3 Bytes
    PFT_A8R8G8B8,           // 4 Bytes
    PFT_A8B8G8R8,           // 4 Bytes
    PFT_X8R8G8B8,           // 4 Bytes
    PFT_X8B8G8R8,           // 4 Bytes
    PFT_R5G6B5,             // 2 Bytes
    PFT_X1R5G5B5,           // 2 Bytes
    PFT_A1R5G5B5,           // 2 Bytes
    PFT_A4R4G4B4,           // 2 Bytes
    PFT_R3G3B2,             // 1 Bytes

    PFT_L8,                 // 1 Bytes
    PFT_L16,                // 2 Bytes
    PFT_A4L4,               // 1 Bytes
    PFT_A8L8,               // 2 Bytes
    PFT_A8,                 // 1 Bytes

    // Float format
    PFT_R16F,               // 2 Bytes
    PFT_A16B16G16R16F,      // 8 Bytes
    PFT_R32F,               // 4 Bytes
    PFT_A32B32G32R32F,      // 16 Bytes

    // BumpMap
    PFT_V8U8,               // 2 Bytes
    PFT_Q8W8V8U8,           // 4 Bytes
};

/** Format of index buffer.
*/
enum EIndexFormat
{
    IF_INDEX16,             // 16 bits
    IF_INDEX32,             // 32 bits
};

/** Usage of texture.
*/
enum ETextureUsage
{
    TU_DEFAULT,             // For normal texture.
    TU_RENDERTARGET,        // For render target.
};

/** How to lock the texture.
*/
enum ETextureLock
{
    TL_READONLY,            // The buffer will not be written.
    TL_WRITEONLY,           // The buffer will only be written.
    TL_READWRITE,           // The buffer will be read and written.
};

/** Texture types.
*/
enum ETextureType
{
    TT_2D,                  // 2D texture.
    TT_3D,                  // 3D texture.
    TT_CUBEMAP,             // Cube texture.
};

/** Face of the cube texture.
*/
enum ECubeFace
{
    CF_RIGHT,
    CF_LEFT,
    CF_TOP,
    CF_BOTTOM,
    CF_FRONT,
    CF_BACK,
};

/** Shader types.
*/
enum EShaderType
{
    ST_VERTEXSHADER,        // Vertex shader.
    ST_PIXELSHADER,         // Pixel shader.
};

/** 着色器对象描述
*/
struct SShaderDesc
{
    AString sProfile;       // 着色器版本字符串
    AString sFuncName;      // 着色器程序主函数名
};

/** How to clear the frame buffer.
*/
enum EFrameClearType
{
    FBC_COLOR = F_BIT(0),   // Clear the color.
    FBC_DEPTH = F_BIT(1),   // Clear the depth.
    FBC_STENCIL = F_BIT(2), // Clear the stencil.
};

///////////////////////////////////////////////////////////////////////////
//// 为引擎定义的一些结构体

typedef FPointI F2DPOINT;
typedef FRectI F2DRECT;

// 光源的描述
struct SLightDesc
{
    ELightType Type;        // 光源的类型
    F3DColor cDiffuse;      // 漫射光颜色
    F3DColor cSpecular;     // 镜面光颜色
	F3DColor cAmbient;      // 环境光颜色
    F3DVector3 vPosition;   // 光照的位置
    F3DVector3 vDirection;  // 光照的方向
    float fRange;           // 光照的范围
    float fFalloff;			// 聚光灯的
    float fAttenuation0;    // 光照的衰减参数1
    float fAttenuation1;    // 光照的衰减参数2
	float fAttenuation2;	// 光照的衰减参数3
	float fTheta;           // 聚光的内角度
	float fPhi;				// 聚光的外角度
};

// 标准材质的描述
struct SMaterialDesc
{
    F3DColor cDiffuse;      // 漫射光颜色
    F3DColor cAmbient;      // 环境光颜色
    F3DColor cSpecular;     // 镜面光颜色
    F3DColor cEmissive;     // 自发光颜色
    float fPower;           // 镜面高光的强度
};

// Viewport parameters.
struct SViewportDesc
{
	uint32 nX;
	uint32 nY;
	uint32 nWidth;
	uint32 nHeight;
	float fMinZ;
	float fMaxZ;
};

// Clip plane list.
typedef std::vector<F3DPlane> F3DClipPlaneList;

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_TYPES_H__