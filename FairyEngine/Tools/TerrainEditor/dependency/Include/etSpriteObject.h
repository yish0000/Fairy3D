//------------------------------------------------------------------------
// Name: etSpriteObject.h
// Desc: This file define a new sceneObject which draw sprites on the
//       terrain for engine.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_SPRITEOBJECT_H__
#define __ET_SPRITEOBJECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "FlySceneObject.h"
#include "FlyRenderable.h"
#include "FlyGraphicBuffer.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etSpriteObject : public FlySceneObject
{
    typedef std::vector<FlyTexture*> TextureList;

protected:
    FlyMatrix m_mWorldTM;   // World transform matrix.
    etSpriteRenderable* m_pRenderable;  // Pointer to the renderable object.

    bool m_bBillboardY;     // Billboard around the Y- axis.
    float m_fFPS;           // FPS of the sprite.
    bool m_bLoop;           // Have loop ?
    UINT m_nCurrentFrame;   // Current frame of the sprite.

    bool m_bAnimPaused;     // Is the animation paused ?
    bool m_bAnimComplete;   // Is the animation complete ?

    float m_fTime;          // Current time.
    float m_fLastTime;      // Time when update last frame.

    TextureList m_Textures; // Texture of each frames.

    // Build the renderable object.
    HRESULT BuildRenderable( float fWidth,float fHeight );

public:
    // Constructor and destructor.
    etSpriteObject( const char* cName );
    ~etSpriteObject(void);

    // Initialize the sprite object.
    HRESULT Initialize( float fWidth,float fHeight,
        const std::vector<std::string>& Textures );

    // Control the sprite animation.
    void Pause(void);
    void Play(void);

    // Update the sprite object.
    void UpdateObject(void);

    // Render the sprite object.
    void RenderObject(void);

    // Get the local boundingBox of the entity.
    const FlyAabb& GetLocalBoundingBox(void) const;

    // Get the world matrix of the object.
    const FlyMatrix& GetWorldTransform(void) const;

    // Get the specified texture.
    FlyTexture* GetTexture( UINT nIndex );

    bool IsBillboardY(void) const { return m_bBillboardY; }
    float GetAnimFPS(void) const { return m_fFPS; }
    UINT GetCurrentFrame(void) const { return m_nCurrentFrame; }
    bool IsAnimLoop(void) const { return m_bLoop; }

    void SetBillboardY( bool bBillboardY ) { m_bBillboardY = bBillboardY; }
    void SetAnimFPS( float fFPS ) { m_fFPS = fFPS; }
    void SetCurrentFrame( UINT nFrame ) { m_nCurrentFrame = nFrame; }
    void SetAnimLoop( bool bLoop ) { m_bLoop = bLoop; }
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etSpriteRenderable : public FlyRenderable
{
protected:
    etSpriteObject* m_pParent;

    FlyAabb m_Bounds;
    FlyGraphicBuffer* m_pVB;

public:
    // Constructor and destructor.
    etSpriteRenderable( etSpriteObject* pParent );
    ~etSpriteRenderable(void);

    HRESULT BuildGeometry( float fWidth,float fHeight );

    // Render the sprite object.
    void Render(void);

    UINT GetNumVerts(void) const { return 4; }
    UINT GetNumIndis(void) const { return 0; }
    UINT GetNumPrims(void) const { return 2; }

    // Get the local boundingBox of the object.
    const FlyAabb& GetBoundingBox(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etSpriteFactory : public FlyEntityFactory
{
public:
    // Constructor and destructor.
    etSpriteFactory(void) { m_sTypename = "sprite"; }
    ~etSpriteFactory(void) { /* Nothing to do! */ };

    FlySceneObject* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneObject* pObject );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_SPRITEOBJECT_H__