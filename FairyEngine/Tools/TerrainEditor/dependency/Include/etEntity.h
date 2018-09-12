//------------------------------------------------------------------------
// Name: etEntity.h
// Desc: This file define a base class for the entity.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_ENTITY_H__
#define __ET_ENTITY_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"

#include "FlySceneObject.h"
#include "FlySceneNode.h"

#include "FlyRenderableHelper.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etEntity
{
protected:
    ENTITYTYPE m_Type;          // Entity type.

    FlySceneObject* m_pObject;  // Pointer to the sceneObject.
    FlySceneNode* m_pNode;      // Pointer to the sceneNode.

    bool m_bSelected;           // Is the entity selected ?
    bool m_bHide;               // Is the entity hidden ?

public:
    // Constructor and destructor.
    etEntity(void);
    virtual ~etEntity(void);

    void Select(void);
    void DeSelect(void);
    void Hide(void);
    void DeHide(void);

    bool IsSelected(void) const { return m_bSelected; }
    bool IsHidden(void) const { return m_bHide; }

    virtual void Translate( const FlyVector& vTrans );
    virtual void SetPosition( const FlyVector& vPos );
    virtual void RotateX( float fValue );
    virtual void RotateY( float fValue );
    virtual void RotateZ( float fValue );
    virtual void SetOrientation( const FlyVector& vOrientation );
    virtual void Scale( float fX,float fY,float fZ );
    virtual void SetScale( const FlyVector& vScale );

    // Get the name of the template name.
    virtual const char* GetTemplateName(void) const { return NULL; }

    const FlyVector& GetPosition(void) const;
    const FlyVector& GetOrientation(void) const;
    const FlyVector& GetScale(void) const;

    // Set the name for the entity.
    void SetName( const char* cName );

    const char* GetName(void) const;
    ENTITYTYPE GetEntityType(void) const;
    const FlyAabb& GetBoundingBox(void) const;

    FlySceneObject* GetSceneObject(void) { return m_pObject; }
    FlySceneNode* GetSceneNode(void) { return m_pNode; }

    static UINT s_nEntityID;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_ENTITY_H__