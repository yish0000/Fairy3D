//------------------------------------------------------------------------
// Name: FlyRenderable.h
// Desc: This file define an interface to act as the objects can be
//       rendered in the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDERABLE_H__
#define __FLY_RENDERABLE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyRenderMaterial.h"
//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyRenderable
{
protected:
    RENDERTYPE m_RenderType;        // Render type of the object.
    FlyRenderMaterial* m_pMaterial; // Pointer to the RenderMaterial.
    int m_nPriority;                // Render priority of the object.

public:
    // Constructor and destructor.
    FlyRenderable(void);
    virtual ~FlyRenderable(void);

    // Add the object to queue.
    virtual void AddToRenderQueue(void);

    // Render the renderable object.
    virtual void Render(void) = 0;

    virtual UINT GetNumVerts(void) const = 0;
    virtual UINT GetNumIndis(void) const = 0;
    virtual UINT GetNumPrims(void) const = 0;

    virtual float GetSqrCameraDist( const FlyVector& vCamPos ) const = 0;
    virtual const FlyAabb& GetBoundingBox(void) const = 0;
    virtual void GetWorldMatrices( FlyMatrix* pMatrices ) const = 0;
    virtual UINT GetNumWorldMatrices(void) const = 0;

    virtual UINT GetLightCount(void) const = 0;
    virtual FlyLightObject* GetLightObject( UINT nIndex ) = 0;

    // Is the lighting mode open ?
    bool IsLighting(void) const;

    // Is the object rendered with a fixed-pipeline ?
    bool IsFixedPipeline(void) const;

    void SetRenderType( RENDERTYPE Type );
    void SetRenderMaterial( FlyRenderMaterial* pMaterial );
    void SetRenderPriority( int nPriority );

    RENDERTYPE GetRenderType(void) const;
    FlyRenderMaterial* GetRenderMaterial(void) const;
    int GetRenderPriority(void) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDERABLE_H__