//------------------------------------------------------------------------
// Name: etEntityManager.cpp
// Desc: This file is to implement the class etEntityManager.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etEntityManager.h"
#include "etSelectionBuffer.h"

#include "etMesh.h"
#include "etAnimation.h"
#include "etParticle.h"
#include "etSprite.h"
#include "etSpriteObject.h"
#include "etVegetation.h"
#include "etVegetationObject.h"

#include "etCoreManager.h"
#include "etSceneManager.h"

#include "FlyMeshManager.h"
#include "FlyAnimManager.h"
#include "FlyParticleSystem.h"
#include "FlyXmlReader.h"
#include "FlyStringUtil.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etEntityManager::etEntityManager(void)
{
    m_CurrentType = ENTITY_UNKNOWN;
    m_nCurrentIndex = -1;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etEntityManager::~etEntityManager(void)
{
    // Get the sceneManager.
    etSceneManager* pManager = etCoreManager::Instance().GetSceneManager();

    // Destroy the liquid objects.
    LiquidObjectList::iterator itr;
    for( itr=m_Liquids.begin();itr!=m_Liquids.end();itr++ )
    {
        FlySceneNode* pNode = (*itr)->GetParentNode();
        pManager->DestroySceneNode( pNode,true );
        pManager->DestroySceneObject( (*itr) );
    }

    // Destroy the entitys.
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
        SAFE_DELETE( *ent );
}


//------------------------------------------------------------------------
// Add a new mesh template for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the mesh file.
//------------------------------------------------------------------------
HRESULT etEntityManager::AddMeshTemplate( const char* cMeshName )
{
    for( size_t n=0;n<m_Meshes.size();n++ )
    {
        if( m_Meshes[n] == cMeshName )
            return ET_TEMPLATEEXISTED;
    }

    // Load the mesh from the file.
    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();
    FlyMesh* pMesh = pMeshMgr->LoadMesh( cMeshName );
    if( !pMesh ) return ET_LOADTEMPLATEFAILED;

    m_Meshes.push_back( cMeshName );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Delete the specified mesh template.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the mesh template.
//------------------------------------------------------------------------
void etEntityManager::DeleteMeshTemplate( UINT nIndex )
{
    if( nIndex >= m_Meshes.size() )
        return;

    // Delete all the entities which use this template.
    for( size_t n=0;n<m_Entities.size();n++ )
    {
        if( m_Entities[n]->GetEntityType() != ENTITY_MESH )
            continue;

        if( m_Meshes[nIndex] == m_Entities[n]->GetTemplateName() )
        {
            SAFE_DELETE( m_Entities[n] );
            m_Entities.erase( m_Entities.begin()+n );
            n--;
        }
    }

    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();
    pMeshMgr->DestroyMeshByName( m_Meshes[nIndex].c_str() );

    // Delete the template.
    m_Meshes.erase( m_Meshes.begin() + nIndex );
}


//------------------------------------------------------------------------
// Add a mesh entity for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the mesh entity.
//      UINT:           ID of the mesh template.
//------------------------------------------------------------------------
etMesh* etEntityManager::AddMeshEntity( const char* cName,UINT nTemplateID )
{
    etEntity* pEntity;

    // Check whether the entity existed.
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( strcmp((*ent)->GetName(),cName) == 0 )
            return NULL;
    }

    pEntity = new etMesh( cName,m_Meshes[nTemplateID].c_str() );
    if( !pEntity ) return NULL;
    m_Entities.push_back( pEntity );
    return (etMesh*)pEntity;
}


//------------------------------------------------------------------------
// Add an animation template for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the animation model file.
//      const char*:    Name of the animation node file.
//------------------------------------------------------------------------
HRESULT etEntityManager::AddAnimTemplate( const char* cAnimName,const char* cAnimNode,
                                          float fFPS )
{
    sAnimTemplate anim;

    // Check whether the template exists.
    AnimTemplateList::iterator itr;
    for( itr=m_Animations.begin();itr!=m_Animations.end();itr++ )
    {
        if( (*itr).sModelFile == cAnimName &&
            (*itr).sAnimFile == cAnimNode )
            return ET_TEMPLATEEXISTED;
    }

    // Load the animation resource.
    FlyAnimManager* pAnimMgr = FlyKernel::Instance().GetAnimManager();
    FlyAnimation* pAnimation = pAnimMgr->LoadAnimation( cAnimName );
    if( !pAnimation ) return ET_LOADTEMPLATEFAILED;
    if( FAILED(pAnimation->AddAnimNode(cAnimNode)) ) return ET_LOADTEMPLATEFAILED;
    pAnimMgr->AddResourceRef( cAnimName );

    anim.sModelFile = cAnimName;
    anim.sAnimFile = cAnimNode;
    anim.fAnimFPS = fFPS;
    m_Animations.push_back( anim );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Delete the specified animation template.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the model file.
//      const char*:    Path to the animation node file.
//------------------------------------------------------------------------
void etEntityManager::DeleteAnimTemplate( UINT nIndex )
{
    if( nIndex >= m_Animations.size() )
        return;

    // Delete all the entities which use this template.
    for( size_t n=0;n<m_Entities.size();n++ )
    {
        if( m_Entities[n]->GetEntityType() != ENTITY_ANIMATION )
            continue;

        if( m_Animations[nIndex].sModelFile == m_Entities[n]->GetTemplateName() )
        {
            SAFE_DELETE( m_Entities[n] );
            m_Entities.erase( m_Entities.begin() + n );
            n--;
        }
    }

    FlyAnimManager* pAnimMgr = FlyKernel::Instance().GetAnimManager();
    pAnimMgr->DestroyAnimationByName( m_Animations[nIndex].sModelFile.c_str() );

    // Delete the template.
    m_Animations.erase( m_Animations.begin() + nIndex );
}


//------------------------------------------------------------------------
// Add an animation entity for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the animation entity.
//      UINT:           Index of the template.
//------------------------------------------------------------------------
etAnimation* etEntityManager::AddAnimEntity( const char* cName,UINT nTemplateID )
{
    etEntity* pEntity;

    // Check whether the entity existed.
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( strcmp((*ent)->GetName(),cName) == 0 )
            return NULL;
    }

    pEntity = new etAnimation( cName,m_Animations[nTemplateID].sModelFile.c_str() );
    if( !pEntity ) return NULL;
    m_Entities.push_back( pEntity );
    return (etAnimation*)pEntity;
}


//------------------------------------------------------------------------
// Add a particle template for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the particle file.
//------------------------------------------------------------------------
HRESULT etEntityManager::AddParticleTemplate( const char* cParName )
{
    for( size_t i=0;i<m_Particles.size();i++ )
    {
        if( m_Particles[i] == cParName )
            return ET_TEMPLATEEXISTED;
    }

    // Load the particle effect resource.
    FlyParticleSystem* pSystem = FlyKernel::Instance().GetParticleSystem();
    FlyParticleEffect* pEffect = pSystem->LoadParticleEffect( cParName );
    if( !pEffect ) return ET_LOADTEMPLATEFAILED;

    m_Particles.push_back( cParName );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Delete the specified particle template.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the particle template.
//------------------------------------------------------------------------
void etEntityManager::DeleteParticleTemplate( UINT nIndex )
{
    if( nIndex >= m_Particles.size() )
        return;

    // Delete all the entities which use this template.
    for( size_t n=0;n<m_Entities.size();n++ )
    {
        if( m_Entities[n]->GetEntityType() != ENTITY_PARTICLE )
            continue;

        if( m_Particles[nIndex] == m_Entities[n]->GetTemplateName() )
        {
            SAFE_DELETE( m_Entities[n] );
            m_Entities.erase( m_Entities.begin() + n );
            n--;
        }
    }

    FlyParticleSystem* pSystem = FlyKernel::Instance().GetParticleSystem();
    pSystem->DestroyEffectByName( m_Particles[nIndex].c_str() );

    // Delete the template.
    m_Particles.erase( m_Particles.begin() + nIndex );
}


//------------------------------------------------------------------------
// Add a particle entity to the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the particle effect template.
//      UINT:           Index of the template.
//------------------------------------------------------------------------
etParticle* etEntityManager::AddParticleEntity( const char* cName,UINT nTemplateID )
{
    etEntity* pEntity;

    // Check whether the entity existed.
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( strcmp((*ent)->GetName(),cName) == 0 )
            return NULL;
    }

    pEntity = new etParticle( cName,m_Particles[nTemplateID].c_str() );
    if( !pEntity ) return NULL;
    m_Entities.push_back( pEntity );
    return (etParticle*)pEntity;
}


//------------------------------------------------------------------------
// Add a sprite template for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the sprite template.
//      float:          Width of the sprite.
//      float:          Height of the sprite.
//      bool:           Billboard around Y- axis ?
//      float:          FPS of the animation.
//      const std::vector<std::string>&: Texture path of each frame.
//------------------------------------------------------------------------
HRESULT etEntityManager::AddSpriteTemplate( const char* cSpriteName,float fWidth,float fHeight,
                                            bool bBillboardY,float fFPS,
                                            const std::vector<std::string>& Textures )
{
    sSpriteTemplate sprite;

    // Check whether the template existed.
    SpriteTemplateList::iterator itr;
    for( itr=m_Sprites.begin();itr!=m_Sprites.end();itr++ )
    {
        if( itr->sName == cSpriteName )
            return ET_TEMPLATEEXISTED;
    }

    sprite.sName = cSpriteName;
    sprite.fWidth = fWidth;
    sprite.fHeight = fHeight;
    sprite.bBillboardY = bBillboardY;
    sprite.fFPS = fFPS;

    for( int n=0;n<Textures.size();n++ )
        sprite.Textures.push_back( Textures[n].c_str() );

    m_Sprites.push_back( sprite );

    return FLY_OK;
}

//------------------------------------------------------------------------
// Add a sprite template from the specified XML document.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the XML document.
//------------------------------------------------------------------------
HRESULT etEntityManager::AddSpriteTemplateFromXML( const char* cXMLDoc )
{
    HRESULT hr;
    FlyXmlReader xml;
    FlyXmlReader::XmlNodeAccess root;
    sSpriteTemplate sprite;

    // Load from the xml document.
    hr = xml.LoadDocument( cXMLDoc );
    if( FAILED(hr) ) return hr;

    root = xml.GetRootNode();
    if( strcmp(root.GetName(),"sprite") != 0 )
        return ET_INVALIDSPRITE;

    sprite.sName = cXMLDoc;
    sscanf( root.GetParamValueByName("width"),"%f",&sprite.fWidth );
    sscanf( root.GetParamValueByName("height"),"%f",&sprite.fHeight );
    sscanf( root.GetParamValueByName("FPS"),"%f",&sprite.fFPS );
    sprite.bBillboardY = (strcmp(root.GetParamValueByName("billboardY"),"true") == 0);

    for( int n=0;n<root.GetChildCount();n++ )
    {
        char cTemp[MAX_PATH];
        FlyXmlReader::XmlNodeAccess child;
        child = root.GetChild( n );

        if( strcmp(child.GetName(),"frame") == 0 )
        {
            strcpy( cTemp,FlyStringUtil::GetDirFromPath(cXMLDoc) );
            strcat( cTemp,child.GetParamValueByName("texture") );
            sprite.Textures.push_back( cTemp );
        }
    }

    m_Sprites.push_back( sprite );
    return FLY_OK;
}

//------------------------------------------------------------------------
// Delete the specified sprite template.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the sprite template.
//------------------------------------------------------------------------
void etEntityManager::DeleteSpriteTemplate( UINT nIndex )
{
    if( nIndex >= m_Sprites.size() )
        return;

    // Delete all the entities which use this template.
    for( size_t n=0;n<m_Entities.size();n++ )
    {
        if( m_Entities[n]->GetEntityType() != ENTITY_SPRITE )
            continue;

        if( m_Sprites[nIndex].sName == m_Entities[n]->GetTemplateName() )
        {
            SAFE_DELETE( m_Entities[n] );
            m_Entities.erase( m_Entities.begin() + n );
            n--;
        }
    }

    // Delete the template.
    m_Sprites.erase( m_Sprites.begin() + nIndex );
}


//------------------------------------------------------------------------
// Add a sprite entity for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the sprite entity.
//      UINT:           Index of the sprite template.
//------------------------------------------------------------------------
etSprite* etEntityManager::AddSpriteEntity( const char* cName,UINT nTemplateID )
{
    etEntity* pEntity;

    // Check whether the entity existed.
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( strcmp((*ent)->GetName(),cName) == 0 )
            return NULL;
    }

    sSpriteTemplate* pTemplate = &m_Sprites[nTemplateID];

    // Create the new entity.
    pEntity = new etSprite( cName,pTemplate->sName.c_str() );
    if( !pEntity ) return NULL;

    etSpriteObject* pObject = (etSpriteObject*)pEntity->GetSceneObject();
    pObject->Initialize( pTemplate->fWidth,pTemplate->fHeight,pTemplate->Textures );
    pObject->SetBillboardY( pTemplate->bBillboardY );
    pObject->SetAnimFPS( pTemplate->fFPS );
    pObject->SetAnimLoop( true );

    m_Entities.push_back( pEntity );
    return (etSprite*)pEntity;
}


//------------------------------------------------------------------------
// Add a polygon-vegetation template for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:          Width of the vegetation object.
//      float:          Height of the vegetation object.
//      const etFloatRect&: Texture coords.
//      const char*:    Path to the texture file.
//      UINT:           Count of the polygons.
//------------------------------------------------------------------------
HRESULT etEntityManager::AddPolygonVegetation( float fWidth,float fHeight,
                                               const etFloatRect& texRect,
                                               const char* cTexture,
                                               UINT nNumPolygons )
{
    sVegetationTemplate vegetation;

    vegetation.Type = VGT_POLYGON;
    vegetation.fWidth = fWidth;
    vegetation.fHeight = fHeight;
    vegetation.texRect = texRect;
    vegetation.sName = cTexture;
    vegetation.nNumPolygons = nNumPolygons;

    m_Vegetations.push_back( vegetation );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Add a mesh-vegetation template for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the mesh file.
//------------------------------------------------------------------------
HRESULT etEntityManager::AddMeshVegetation( const char* cMeshName )
{
    sVegetationTemplate vegetation;

    vegetation.Type = VGT_MESH;
    vegetation.sName = cMeshName;

    // Load the mesh from the file.
    FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();
    FlyMesh* pMesh = pMeshMgr->LoadMesh( cMeshName );
    if( !pMesh ) return ET_LOADTEMPLATEFAILED;

    m_Vegetations.push_back( vegetation );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Delete the specified vegetation template.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the vegetation template.
//------------------------------------------------------------------------
void etEntityManager::DeleteVegetationTemplate( UINT nIndex )
{
    if( nIndex >= m_Sprites.size() )
        return;

    // Delete all the entities which use this template.
    for( size_t n=0;n<m_Entities.size();n++ )
    {
        if( m_Entities[n]->GetEntityType() != ENTITY_VEGETATION )
            continue;

        if( m_Vegetations[nIndex].sName == m_Entities[n]->GetTemplateName() )
        {
            SAFE_DELETE( m_Entities[n] );
            m_Entities.erase( m_Entities.begin() + n );
            n--;
        }
    }

    if( m_Vegetations[nIndex].Type == VGT_MESH )
    {
        FlyMeshManager* pMeshMgr = FlyKernel::Instance().GetMeshManager();
        pMeshMgr->DestroyMeshByName( m_Vegetations[nIndex].sName.c_str() );
    }

    // Delete the template.
    m_Vegetations.erase( m_Vegetations.begin() + nIndex );
}


//------------------------------------------------------------------------
// Add a vegetation entity for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the vegetation entity.
//      UINT:           Index of the template.
//------------------------------------------------------------------------
etVegetation* etEntityManager::AddVegetationEntity( const char* cName,UINT nTemplateID )
{
    etEntity* pEntity;

    // Check whether the entity existed.
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( strcmp((*ent)->GetName(),cName) == 0 )
            return NULL;
    }

    sVegetationTemplate* pTemplate = &m_Vegetations[nTemplateID];

    // Create the new entity.
    pEntity = new etVegetation( cName,pTemplate->sName.c_str() );
    if( !pEntity ) return NULL;

    etVegetationObject* pObject = (etVegetationObject*)pEntity->GetSceneObject();
    pObject->Initialize( (VEGETATIONTYPE)pTemplate->Type,pTemplate->sName.c_str(),
        pTemplate->fWidth,pTemplate->fHeight,pTemplate->texRect,pTemplate->sName.c_str(),
        pTemplate->nNumPolygons );

    m_Entities.push_back( pEntity );
    return (etVegetation*)pEntity;
}


//------------------------------------------------------------------------
// Add a liquid object to the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Name of the liquid object.
//      const char*:        Path to the surface texture.
//      const char*:        Path to the distort texture.
//      const FlyVector&:   Center of the liquid.
//      float:              Width of the liquid.
//      float:              Height of the liquid.
//------------------------------------------------------------------------
etLiquidObject* etEntityManager::AddLiquidEntity( const char* cName,const char* cSurfaceMap,
                                                  const char* cDistortMap,const float* pfCenter,
                                                  float fWidth,float fHeight )
{
    HRESULT hr;
    FlySceneNode* pNode;
    char cTemp[40];

    // Get the sceneManager.
    etSceneManager* pManager = etCoreManager::Instance().GetSceneManager();

    // Check whether the object existed ?
    if( pManager->GetSceneObject(cName) )
        return NULL;

    FlyVector vCenter( pfCenter[0],pfCenter[1],pfCenter[2] );
    etLiquidObject* pObject = (etLiquidObject*)pManager->CreateSceneObject( cName,"liquid" );
    hr = pObject->Initialize( cSurfaceMap,cDistortMap,vCenter,fWidth,fHeight );
    if( FAILED(hr) )
    {
        pManager->DestroySceneObject( pObject );
        return NULL;
    }

    // Create a sceneNode for the liquid.
    sprintf( cTemp,"%s_node",cName );
    FlySceneNode* pEntityRoot = pManager->GetSceneNode( "et-Entity" );
    pNode = pEntityRoot->CreateChildNode( cTemp );
    pNode->AttachObject( pObject );

    m_Liquids.push_back( pObject );
    return pObject;
}

//------------------------------------------------------------------------
// Destroy the specified liquid object.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the liquid object.
//------------------------------------------------------------------------
void etEntityManager::DestroyLiquidObject( UINT nIndex )
{
    if( nIndex >= m_Liquids.size() )
        return;

    // Get the sceneManager.
    etSceneManager* pManager = etCoreManager::Instance().GetSceneManager();

    FlySceneNode* pNode = m_Liquids[nIndex]->GetParentNode();
    pManager->DestroySceneNode( pNode,true );
    pManager->DestroySceneObject( m_Liquids[nIndex] );
}

//------------------------------------------------------------------------
// Get the specified liquid object.
//------------------------------------------------------------------------
etLiquidObject* etEntityManager::GetLiquidObject( UINT nIndex )
{
    return (nIndex >= m_Liquids.size()) ? NULL : m_Liquids[nIndex];
}


//------------------------------------------------------------------------
// Put an entity to the terrain with the selected template.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position to put the entity.
//------------------------------------------------------------------------
void etEntityManager::PutEntityTerrain( const FlyVector& vPosition )
{
    char cTemp[40];

    if( m_CurrentType == ENTITY_UNKNOWN ||
        m_nCurrentIndex == -1 )
        return;

    // Set the default name for the entity.
    sprintf( cTemp,"Object%d",etEntity::s_nEntityID );

    switch( m_CurrentType )
    {
    case ENTITY_MESH:
        {
            etMesh* pMesh = AddMeshEntity( cTemp,m_nCurrentIndex );
            if( pMesh ) pMesh->SetPosition( vPosition );
        }
        break;
    case ENTITY_ANIMATION:
        {
            etAnimation* pAnimation = AddAnimEntity( cTemp,m_nCurrentIndex );
            if( pAnimation ) pAnimation->SetPosition( vPosition );
        }
        break;
    case ENTITY_PARTICLE:
        {
            etParticle* pParticle = AddParticleEntity( cTemp,m_nCurrentIndex );
            if( pParticle ) pParticle->SetPosition( vPosition );
        }
        break;
    case ENTITY_VEGETATION:
        {
            etVegetation* pVegetation = AddVegetationEntity( cTemp,m_nCurrentIndex );
            if( pVegetation ) pVegetation->SetPosition( vPosition );
        }
        break;
    case ENTITY_SPRITE:
        {
            etSprite* pSprite = AddSpriteEntity( cTemp,m_nCurrentIndex );
            if( pSprite ) pSprite->SetPosition( vPosition );
        }
        break;
    }

    etEntity::s_nEntityID++;
}


//------------------------------------------------------------------------
// Delete the specified entity by its pointer.
//------------------------------------------------------------------------
void etEntityManager::DeleteEntity( etEntity* pEntity )
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( *ent == pEntity )
        {
            SAFE_DELETE( pEntity );
            m_Entities.erase( ent );
            break;
        }
    }
}


//------------------------------------------------------------------------
// Delete the specified entity by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the specified entity.
//------------------------------------------------------------------------
void etEntityManager::DeleteEntity( UINT nIndex )
{
    if( nIndex >= m_Entities.size() )
        return;

    SAFE_DELETE( m_Entities[nIndex] );
    m_Entities.erase( m_Entities.begin() + nIndex );
}


//------------------------------------------------------------------------
// Delete the specified entity by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the entity.
//------------------------------------------------------------------------
void etEntityManager::DeleteEntity( const char* cName )
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( strcmp((*ent)->GetName(),cName) == 0 )
        {
            SAFE_DELETE( *ent );
            m_Entities.erase( ent );
            break;
        }
    }
}


//------------------------------------------------------------------------
// Drag a renctange to select the entities.
// ----------------------------------------------------------------------
// Param -> IN:
//      const RECT&:        The selection rectangle.
//      etSelectionBuffer*: Pointer to the selection buffer.
//------------------------------------------------------------------------
bool etEntityManager::SelectIntersects( const RECT& rcSel,etSelectionBuffer* pSelBuffer )
{
    FlyAabb bbox;
    POINT pt1,pt2;
    RECT rcEntity;

    // Get the pointer to the render device.
    LPFLYRENDERDEVICE pDevice = etCoreManager::Instance().GetRenderDevice();

    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( (*ent)->IsHidden() ||
            (*ent)->GetEntityType() == ENTITY_LIQUID )
            continue;

        bbox = (*ent)->GetBoundingBox();
        pt1 = pDevice->Transfer3DTo2D( bbox.vcMin );
        pt2 = pDevice->Transfer3DTo2D( bbox.vcMax );

        if( pt1.x < pt2.x )
        {
            rcEntity.left = pt1.x;
            rcEntity.right = pt2.x;
        }
        else
        {
            rcEntity.left = pt2.x;
            rcEntity.right = pt1.x;
        }

        if( pt1.y < pt2.y )
        {
            rcEntity.bottom = pt1.y;
            rcEntity.top = pt2.y;
        }
        else
        {
            rcEntity.bottom = pt2.y;
            rcEntity.top = pt1.y;
        }

        if( IsRectIntersecting(rcEntity,rcSel) )
            pSelBuffer->AddEntity( *ent );
    }

    return true;
}


//------------------------------------------------------------------------
// Select the actors which are click by the mouse.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyRay&:      Make a ray from the camera to actor.
//      SelectionBuffer*:   Pointer to the selectionBuffer.
//------------------------------------------------------------------------
bool etEntityManager::SelectPicked( const FlyRay& ray,etSelectionBuffer* pSelBuffer )
{
    etEntity* pSelected = NULL;
    float fDist,fL = 999999.0f;
    float fMin = 999999.0f;
    
    // Loop through all the entity.
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( (*ent)->IsHidden() ||
            (*ent)->GetEntityType() == ENTITY_LIQUID )
            continue;

        FlyAabb bbox = (*ent)->GetBoundingBox();
        if( ray.Intersects(bbox,fL,&fDist) )
        {
            if( fDist < fMin )
            {
                fMin = fDist;
                pSelected = *ent;
            }
        }
    }

    if( pSelected )
    {
        pSelBuffer->AddEntity( pSelected );
        return true;
    }
    else
        return false;
}


//------------------------------------------------------------------------
// Check whether two rectangle are intersecting ?
// ----------------------------------------------------------------------
// Param -> IN:
//      RECT*:      Pointer to the first rectangle.
//      RECT*:      Pointer to the second rectangle.
//------------------------------------------------------------------------
bool etEntityManager::IsRectIntersecting( const RECT& rc1,const RECT& rc2 )
{
    int nW1 = abs((rc1.right-rc1.left)) / 2;
    int nH1 = abs((rc1.bottom-rc1.top)) / 2;
    int nW2 = abs((rc2.right-rc2.left)) / 2;
    int nH2 = abs((rc2.bottom-rc2.top)) / 2;

    POINT cen1 = { (rc1.left+rc1.right)/2,(rc1.top+rc1.bottom)/2 };
    POINT cen2 = { (rc2.left+rc2.right)/2,(rc2.top+rc2.bottom)/2 };

    if( abs(cen1.x-cen2.x) > nW1+nW2 ) return false;
    if( abs(cen1.y-cen2.y) > nH1+nH2 ) return false;

    return true;
}


//------------------------------------------------------------------------
// Select all the entities for the editor.
//------------------------------------------------------------------------
void etEntityManager::SelectAll( etSelectionBuffer* pBuffer )
{
    // Clear the selection buffer.
    pBuffer->Clear();

    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( (*ent)->GetEntityType() == ENTITY_LIQUID ||
            (*ent)->IsHidden() )
            continue;

        pBuffer->AddEntity( *ent );
    }
}


//------------------------------------------------------------------------
// Select the entities invertly for the editor.
//------------------------------------------------------------------------
void etEntityManager::SelectInvert( etSelectionBuffer* pSelBuffer )
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( (*ent)->GetEntityType() == ENTITY_LIQUID ||
            (*ent)->IsHidden() )
            continue;

        if( (*ent)->IsSelected() )
            pSelBuffer->RemoveEntity( *ent );
        else
            pSelBuffer->AddEntity( *ent );
    }
}


//------------------------------------------------------------------------
// Hide all the entities for the editor.
//------------------------------------------------------------------------
void etEntityManager::HideAll(void)
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( (*ent)->GetEntityType() == ENTITY_LIQUID )
            continue;

        (*ent)->Hide();
    }
}


//------------------------------------------------------------------------
// Show all the entities for the editor.
//------------------------------------------------------------------------
void etEntityManager::UnHideAll(void)
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( (*ent)->GetEntityType() == ENTITY_LIQUID )
            continue;

        (*ent)->DeHide();
    }
}


//------------------------------------------------------------------------
// Hide all the selected entities for the editor.
//------------------------------------------------------------------------
void etEntityManager::HideSelected(void)
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( (*ent)->GetEntityType() == ENTITY_LIQUID )
            continue;

        if( (*ent)->IsSelected() )
            (*ent)->Hide();
    }
}


//------------------------------------------------------------------------
// Hide the selected entities invertly for the editor.
//------------------------------------------------------------------------
void etEntityManager::HideInvert(void)
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( (*ent)->GetEntityType() == ENTITY_LIQUID )
            continue;

        if( (*ent)->IsHidden() )
            (*ent)->DeHide();
        else
            (*ent)->Hide();
    }
}


//------------------------------------------------------------------------
// Get the count of the entities.
//------------------------------------------------------------------------
UINT etEntityManager::GetNumEntities(void) const
{
    return m_Entities.size();
}

//------------------------------------------------------------------------
// Get the entity by its name.
//------------------------------------------------------------------------
etEntity* etEntityManager::GetEntityByName( const char* cName )
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        if( strcmp((*ent)->GetName(),cName) == 0 )
            return *ent;
    }

    return NULL;
}

//------------------------------------------------------------------------
// Get the entity by its index.
//------------------------------------------------------------------------
etEntity* etEntityManager::GetEntityByIndex( UINT nIndex )
{
    if( nIndex >= m_Entities.size() )
        return NULL;

    return m_Entities[nIndex];
}