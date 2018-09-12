//------------------------------------------------------------------------
// Name: etSelectionBuffer.h
// Desc: This file define a class to store the selected entities and 
//       modifiy them.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_SELECTIONBUFFER_H__
#define __ET_SELECTIONBUFFER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"
#include "etEntity.h"

#include "FlySceneObject.h"
#include "FlyGraphicBuffer.h"
#include "FlyRenderable.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etSelectionBuffer : public FlySceneObject
{
    typedef std::vector<etEntity*> EntityList;

protected:
    EntityList m_Entities;          // Selected entity list.
    FlyAabb m_Bounds;               // Boundingbox of the selectionBuffer.
    ENTITYTOOLTYPE m_ToolType;      // Tool type of the selectionBuffer.

    etSelectionRect* m_pSelRect;    // Renderable object to render the selectionRect.
    etSelectionAxis* m_pSelAxis;    // Renderable object to render the selectionAxis.

    POINT m_ptStart;                // Start position of the cursor.
    POINT m_ptEnd;                  // End position of the cursor.
    bool m_bDraggingRect;           // Dragging the selection rectangle ?
    bool m_bSelecting;              // Selecting the entities ?

public:
    // Constructor and destructor.
    etSelectionBuffer(void);
    ~etSelectionBuffer(void);

    void AddEntity( etEntity* pEntity );
    void RemoveEntity( etEntity* pEntity );
    void Clear(void);

    void BeginDragRect( const POINT& ptStart );
    void DraggingRect( const POINT& ptEnd );
    void EndDragRect(void);

    void DeleteSelected(void);
    void Translate( const FlyVector& vTrans );
    void RotateX( float fValue );
    void RotateY( float fValue );
    void RotateZ( float fValue );
    void Scale( const FlyVector& vScale );

    bool IsInSelection( const FlyRay& ray );
    bool IsSelecting(void) const { return m_bSelecting; }
    UINT GetNumSelected(void) const;
    const FlyAabb& GetSelEntityBBox(void) const { return m_Bounds; }
    ENTITYTOOLTYPE GetToolType(void) const { return m_ToolType; }
    void SetToolType( ENTITYTOOLTYPE Type ) { m_ToolType = Type; }

    // Get the selected entities.
    etEntity* GetSelectedEntity( UINT nIndex );

    // Update and render the object.
    void UpdateObject(void);
    void RenderObject(void);

    // Get the local boundngBox.
    const FlyAabb& GetLocalBoundingBox(void) const { return FlyAabb::EMPTY; }
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etSelectionRect : public FlyRenderable
{
protected:
    etSelectionBuffer* m_pParent;   // Pointer to the parent.

    UINT m_nNumVerts;           // Count of the vertices.
    UINT m_nNumIndis;           // Count of the indices.
    UINT m_nNumPrims;           // Count of the primitives.

    FlyGraphicBuffer* m_pVB;    // Pointer to the vertex buffer.

public:
    // Constructor and destructor.
    etSelectionRect( etSelectionBuffer* pParent );
    ~etSelectionRect(void);

    // Update the selection rectangle.
    void UpdateSelection( const POINT& ptStart,const POINT& ptEnd );

    // Render the object.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the boundingBox of the object.
    const FlyAabb& GetBoundingBox(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );
};

//////////////////////////////////////////////////////////////////////////

class _ET_EXPORT etSelectionAxis : public FlyRenderable
{
protected:
    etSelectionBuffer* m_pParent;   // Pointer to the parent.

    UINT m_nNumVerts;   // Count of the vertices.
    UINT m_nNumIndis;   // Count of the indices.
    UINT m_nNumPrims;   // Count of the primitives.

    FlyGraphicBuffer* m_pVBMoveXZ;
    FlyGraphicBuffer* m_pVBMoveY;
    FlyGraphicBuffer* m_pVBRotateX;
    FlyGraphicBuffer* m_pVBRotateY;
    FlyGraphicBuffer* m_pVBRotateZ;
    FlyGraphicBuffer* m_pVBScale;

public:
    // Constructor and destructor.
    etSelectionAxis( etSelectionBuffer* pParent );
    ~etSelectionAxis(void);

    // Render the object.
    void Render(void);

    UINT GetNumVerts(void) const;
    UINT GetNumIndis(void) const;
    UINT GetNumPrims(void) const;

    // Get the boundingBox of the object.
    const FlyAabb& GetBoundingBox(void) const;

    float GetSqrCameraDist( const FlyVector& vCamPos ) const;
    void GetWorldMatrices( FlyMatrix* pMatrices ) const;
    UINT GetNumWorldMatrices(void) const;

    UINT GetLightCount(void) const;
    FlyLightObject* GetLightObject( UINT nIndex );

protected:
    HRESULT BuildAxisMoveXZ(void);
    HRESULT BuildAxisMoveY(void);
    HRESULT BuildAxisRotateX(void);
    HRESULT BuildAxisRotateY(void);
    HRESULT BuildAxisRotateZ(void);
    HRESULT BuildAxisScale(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_SELECTIONBUFFER_H__