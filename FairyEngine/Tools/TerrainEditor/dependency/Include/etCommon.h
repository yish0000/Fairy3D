//------------------------------------------------------------------------
// Name: etCommon.h
// Desc: Common header for the core module of the editor.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_COMMON_H__
#define __ET_COMMON_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "Res/resource.h"
//////////////////////////////////////////////////////////////////////////

// Compile this project as a dynamic link library ?
//#define _ET_DYN_LIB

#ifdef _ET_DYN_LIB
#define _ET_EXPORT __declspec(dllexport)
#else
#define _ET_EXPORT __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////

#define ET_TEXTUREEXISTED       0x83000000
#define ET_TEXTURELOADFAIL      0x83000001
#define ET_INVALIDCFGFILE       0x83000002
#define ET_ENTITYEXISTED        0x83000003
#define ET_CREATESCENEMANAGER   0x83000004
#define ET_CREATECORE           0x83000005
#define ET_TEMPLATEEXISTED      0x83000006
#define ET_LOADTEMPLATEFAILED   0x83000006
#define ET_INVALIDSPRITE        0x83000007

//////////////////////////////////////////////////////////////////////////

enum ENTITYTYPE
{
    ENTITY_UNKNOWN,
    ENTITY_MESH,
    ENTITY_ANIMATION,
    ENTITY_PARTICLE,
    ENTITY_VEGETATION,
    ENTITY_SPRITE,
    ENTITY_LIQUID,
};

enum TOOLSHAPE
{
    TS_CIRCLE,
    TS_RECTANGLE,
};

enum TOOLTYPE
{
    TT_RAISE,
    TT_LOWER,
    TT_FLATTEN,
    TT_SMOOTH,
    TT_DIG,
    TT_PAINT,
    TT_ERASE,
    TT_BLEND,
};

enum RENDERMODE
{
    RM_WIREFRAME,
    RM_SOLID,
    RM_TEXTURE,
    RM_TEXTUREWIRE,
};

enum CAMERAMODE
{
    CM_EDITCAMERA,
    CM_RAMBLECAMERA,
};

enum WORKMODE
{
    WKM_TERRAIN,
    WKM_TEXTURE,
    WKM_ENTITY,
};

enum ENTITYTOOLTYPE
{
    ETT_SELECT,
    ETT_MOVE_XZ,
    ETT_MOVE_Y,
    ETT_ROTATION_X,
    ETT_ROTATION_Y,
    ETT_ROTATION_Z,
    ETT_SCALE_XYZ,
};

//////////////////////////////////////////////////////////////////////////
// KEY MACROS FOR EDITOR.

#define ETKEY_UP        0
#define ETKEY_DOWN      1
#define ETKEY_LEFT      2
#define ETKEY_RIGHT     3
#define ETKEY_W         4
#define ETKEY_S         5
#define ETKEY_A         6
#define ETKEY_D         7
#define ETKEY_Q         8
#define ETKEY_E         9
#define ETKEY_SPACE     10
#define ETKEY_SHIFT     11

//////////////////////////////////////////////////////////////////////////

// Pre-define all the classes.
class etAnimation;
class etBrush;
class etBrushRenderable;
class etConfig;
class etCoreManager;
class etDetailLayer;
class etEntity;
class etEntityManager;
class etLightLayer;
class etLiquidObject;
class etLiquidRenderable;
class etLiquidFactory;
class etLiquidTarget;
class etMesh;
class etParticle;
class etPlugin;
class etSceneFactory;
class etSceneManager;
class etSelectionAxis;
class etSelectionBuffer;
class etSelectionRect;
class etSprite;
class etSpriteObject;
class etSpriteRenderable;
class etTerrain;
class etTile;
class etTileRenderable;
class etTileSelected;
class etVegetation;
class etVegetationObject;
class etVegetationRenderable;
class etVegetationFactory;

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_COMMON_H__