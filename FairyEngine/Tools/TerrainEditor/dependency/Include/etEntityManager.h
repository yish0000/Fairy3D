//------------------------------------------------------------------------
// Name: etEntityManager.h
// Desc: This file define a class to manage the entities for the editor.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_ENTITYMANAGER_H__
#define __ET_ENTITYMANAGER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etEntity.h"

#include "etFloatRect.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etEntityManager
{
    typedef std::vector<etEntity*> EntityList;

    struct sAnimTemplate
    {
        std::string sModelFile;
        std::string sAnimFile;
        float fAnimFPS;
    };
    typedef std::vector<sAnimTemplate> AnimTemplateList;

    struct sSpriteTemplate
    {
        std::string sName;

        float fWidth;
        float fHeight;
        bool bBillboardY;
        float fFPS;
        std::vector<std::string> Textures;
    };
    typedef std::vector<sSpriteTemplate> SpriteTemplateList;

    struct sVegetationTemplate
    {
        int Type;           // Vegetation type.
        std::string sName;  // Model name or texture name.

        float fWidth;
        float fHeight;
        etFloatRect texRect;
        UINT nNumPolygons;
    };
    typedef std::vector<sVegetationTemplate> VegetationTemplateList;

    typedef std::vector<etLiquidObject*> LiquidObjectList;

protected:
    EntityList m_Entities;

    std::vector<std::string> m_Meshes;
    AnimTemplateList m_Animations;
    std::vector<std::string> m_Particles;
    SpriteTemplateList m_Sprites;
    VegetationTemplateList m_Vegetations;

    LiquidObjectList m_Liquids; // Liquid object list.

    ENTITYTYPE m_CurrentType;   // Current selected entity type.
    UINT m_nCurrentIndex;       // Current selected entity template index.

    // Check whether the two rectangles are
    // intersecting.
    bool IsRectIntersecting( const RECT& rc1,const RECT& rc2 );

public:
    // Constructor and destructor.
    etEntityManager(void);
    ~etEntityManager(void);

    // Functions of the Mesh entity.
    HRESULT AddMeshTemplate( const char* cMeshName );
    void DeleteMeshTemplate( UINT nIndex );
    etMesh* AddMeshEntity( const char* cName,UINT nTemplateID );

    // Functions of the animation entity.
    HRESULT AddAnimTemplate( const char* cAnimName,const char* cAnimNode,float fFPS );
    void DeleteAnimTemplate( UINT nIndex );
    etAnimation* AddAnimEntity( const char* cName,UINT nTemplateID );

    // Functions of the particle entity.
    HRESULT AddParticleTemplate( const char* cParName );
    void DeleteParticleTemplate( UINT nIndex );
    etParticle* AddParticleEntity( const char* cName,UINT nTemplateID );

    // Functions of the sprite entity.
    HRESULT AddSpriteTemplate( const char* cSpriteName,float fWidth,float fHeight,
        bool bBillboardY,float fFPS,const std::vector<std::string>& Textures );
    HRESULT AddSpriteTemplateFromXML( const char* cXMLDoc );
    void DeleteSpriteTemplate( UINT nIndex );
    etSprite* AddSpriteEntity( const char* cName,UINT nTemplateID );

    // Functions of the vegetation entity.
    HRESULT AddPolygonVegetation( float fWidth,float fHeight,const etFloatRect& texRect,
        const char* cTexture,UINT nNumPolygons );
    HRESULT AddMeshVegetation( const char* cMeshName );
    void DeleteVegetationTemplate( UINT nIndex );
    etVegetation* AddVegetationEntity( const char* cName,UINT nTemplateID );

    etLiquidObject* AddLiquidEntity( const char* cName,const char* cSurfaceMap,
        const char* cDistortMap,const float* pfCenter,float fWidth,float fHeight );
    void DestroyLiquidObject( UINT nIndex );
    etLiquidObject* GetLiquidObject( UINT nIndex );

    void DeleteEntity( etEntity* pEntity );
    void DeleteEntity( UINT nIndex );
    void DeleteEntity( const char* cName );

    // Drag a rectangle to select the entities.
    bool SelectIntersects( const RECT& rcSel,etSelectionBuffer* pSelBuffer );

    // Pick an entity by clicking the mouse.
    bool SelectPicked( const FlyRay& ray,etSelectionBuffer* pSelBuffer );

    void SelectAll( etSelectionBuffer* pSelBuffer );
    void SelectInvert( etSelectionBuffer* pSelBuffer );
    void HideSelected(void);
    void HideInvert(void);
    void HideAll(void);
    void UnHideAll(void);

    // Put the entity on the terrain.
    void PutEntityTerrain( const FlyVector& vPosition );

    ENTITYTYPE GetCurrentType(void) const { return m_CurrentType; }
    UINT GetCurrentIndex(void) const { return m_nCurrentIndex; }
    void SetCurrentType( ENTITYTYPE Type ) { m_CurrentType = Type; }
    void SetCurrentIndex( UINT nIndex ) { m_nCurrentIndex = nIndex; }

    UINT GetNumEntities(void) const;
    etEntity* GetEntityByName( const char* cName );
    etEntity* GetEntityByIndex( UINT nIndex );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_ENTITYMANAGER_H__