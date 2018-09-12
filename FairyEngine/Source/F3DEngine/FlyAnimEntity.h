//------------------------------------------------------------------------
// Name: FlyAnimEntity.h
// Desc: This file define a real instance of the skeleton animation.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_ANIMENTITY_H__
#define __FLY_ANIMENTITY_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlySceneObject.h"
#include "FlyRenderable.h"
#include "FlyGraphicBuffer.h"

#include "FlyAnimation.h"
//////////////////////////////////////////////////////////////////////////


class FlyAnimRenderable;

/*
* A real instance of the skeleton animation in the 
* 3D world.
*/
class _FLY_EXPORT FlyAnimEntity : public FlySceneObject
{
    typedef std::vector<FlyAnimRenderable*> RenderableList;

protected:
    FlyAnimation* m_pAnimation; // Pointer to the animation template.

    FlyAabb m_LocalBBox;        // Current bounding Box.
    float m_fBRadius;           // Current bounding radius.

    float m_fFrame;             // Current frame of the entity.
    bool m_bAnimPaused;         // Is the entity paused ?
    bool m_bAnimCompleted;      // Is the entity completed ?
    UINT m_nCurrentAnim;         // Current animation ID.
    float m_fAnimFPS;           // Current animation FPS.
    bool m_bAnimLoop;           // Play the animation with loop.
    bool m_bShowBBox;           // Show the bounding box ?

    int m_nDirtyState;          // Dirty state of the entity.

    // Renderable object list for the entity.
    RenderableList m_Renderables;

    // Update the data for the entity.
    void UpdateEntity(void);

    // Build the renderable objects.
    HRESULT BuildRenderables(void);

    // Reinitialize the entity.
    HRESULT Reinitialize(void);

public:
    // Constructor and destructor.
    FlyAnimEntity( const char* cName );
    ~FlyAnimEntity(void);

    // Initialize the entity.
    HRESULT Initialize( FlyAnimation* pAnimation );

    // Update the animation entity.
    void UpdateObject(void);

    // Render the animation entity.
    void RenderObject(void);

    // Get the local boudingBox of the entity.
    const FlyAabb& GetLocalBoundingBox(void) const;

    // Control the animation entity.
    void Pause(void);
    void Play(void);
    void SetCurrentFrame( float fFrame );
    void SetAnimation( UINT nAnimID );
    void SetAnimLoop( bool bLoop );
    void SetAnimFPS( float fAnimFPS );
    void ShowBoundingBox( bool bShowBBox );

    int GetCurrentAnimation(void) const;
    float GetCurrentFrame(void) const;
    bool IsAnimationLoop(void) const;
    float GetAnimFPS(void) const;
    bool IsAnimationCompleted(void) const;

    FlyAnimation* GetAnimation(void);
};

//////////////////////////////////////////////////////////////////////////

/*
* This class is the basic unit to render the animation.
* inhert from the class FlyRenderable.
*/
class FlyAnimRenderable : public FlyRenderable
{
    friend class FlyAnimEntity;

protected:
    FlyAnimEntity* m_pParent;   // Pointer to the parent.

    UINT m_nNumVerts;           // Number of the vertices.
    UINT m_nNumIndis;           // Number of the indices.
    UINT m_nNumPrims;           // Number of the primitives.
    FlyAabb m_BBox;             // Bounding box of the object.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.
    FlyGraphicBuffer* m_pIB;    // Pointer to the index buffer.

public:
    // Constructor and destructor.
    FlyAnimRenderable( FlyAnimEntity* pParent );
    ~FlyAnimRenderable(void);

    // Create the graphic buffer.
    HRESULT CreateGraphicBuffer( UINT nNumVerts,UINT nNumIndis,UINT nNumPrims );

    // Send the vertex data to buffer.
    void WriteVertexData( const AVERTEX* pVerts,UINT nCount );

    // Render the object.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the local boundingBox.
    const FlyAabb& GetBoundingBox(void) const;

    // Get the distance to the camera.
    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );

    FlyGraphicBuffer* GetVertexBuffer(void);
    FlyGraphicBuffer* GetIndexBuffer(void);

    FlyAnimEntity* GetParentEntity(void);
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyAnimEntityFactory : public FlyEntityFactory
{
public:
    // Constructor and destructor.
    FlyAnimEntityFactory(void) { m_sTypename = "animation"; }
    ~FlyAnimEntityFactory(void) { /* Nothing to do! */ };

    FlySceneObject* CreateInstance( const char* cName );
    void DestroyInstance( FlySceneObject* pObject );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_ANIMENTITY_H__