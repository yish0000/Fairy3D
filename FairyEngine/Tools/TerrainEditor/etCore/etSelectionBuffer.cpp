//------------------------------------------------------------------------
// Name: etSelectionBuffer.cpp
// Desc: This file is to implement the class etSelectionBuffer.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etSelectionBuffer.h"

#include "etCoreManager.h"
#include "etEntityManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etSelectionBuffer::etSelectionBuffer(void)
{
    m_Bounds.SetEmpty();
    m_ToolType = ETT_SELECT;

    m_bDraggingRect = false;
    m_bSelecting = false;
    m_ptStart.x = m_ptEnd.x = 0;
    m_ptStart.y = m_ptEnd.y = 0;

    // Set the world boundingBox.
    m_WorldBBox.vcMin.Set( -999999.0f,-999999.0f,-999999.0f );
    m_WorldBBox.vcMax.Set(  999999.0f, 999999.0f, 999999.0f );

    m_pSelRect = new etSelectionRect( this );
    m_pSelAxis = new etSelectionAxis( this );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etSelectionBuffer::~etSelectionBuffer(void)
{
    SAFE_DELETE( m_pSelRect );
    SAFE_DELETE( m_pSelAxis );
}


//------------------------------------------------------------------------
// Add the specified entity to the selection buffer.
//------------------------------------------------------------------------
void etSelectionBuffer::AddEntity( etEntity* pEntity )
{
    if( pEntity->IsSelected() )
        return;

    pEntity->Select();
    m_Entities.push_back( pEntity );
    m_Bounds.Merge( pEntity->GetBoundingBox() );
    m_bSelecting = true;
}


//------------------------------------------------------------------------
// Remove the specified entity.
//------------------------------------------------------------------------
void etSelectionBuffer::RemoveEntity( etEntity* pEntity )
{
    EntityList::iterator itr;
    itr = std::find( m_Entities.begin(),m_Entities.end(),pEntity );
    (*itr)->DeSelect();
    m_Entities.erase( itr );

    // Update the new boundingBox.
    m_Bounds.SetEmpty();
    for( itr=m_Entities.begin();itr!=m_Entities.end();itr++ )
        m_Bounds.Merge( (*itr)->GetBoundingBox() );

    if( m_Entities.size() == 0 )
        m_bSelecting = false;
}


//------------------------------------------------------------------------
// Clear the selection buffer.
//------------------------------------------------------------------------
void etSelectionBuffer::Clear(void)
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
        (*ent)->DeSelect();

    m_Entities.clear();
    m_Bounds.SetEmpty();
    m_bSelecting = false;
}


//------------------------------------------------------------------------
// Delete the selected entities.
//------------------------------------------------------------------------
void etSelectionBuffer::DeleteSelected(void)
{
    // Get the pointer to the entityManager.
    etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();

    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
        pEntityMgr->DeleteEntity( *ent );

    // Clear the selection buffer.
    m_Entities.clear();
    m_Bounds.SetEmpty();
    m_bSelecting = false;
}


//------------------------------------------------------------------------
// Translate the selected entities.
//------------------------------------------------------------------------
void etSelectionBuffer::Translate( const FlyVector& vTrans )
{
    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
        (*ent)->Translate( vTrans );

    if( m_Entities.size() != 0 )
    {
        m_Bounds.vcMin += vTrans;
        m_Bounds.vcMax += vTrans;
    }
}


//------------------------------------------------------------------------
// Rotate the selected entities around the X- axis.
//------------------------------------------------------------------------
void etSelectionBuffer::RotateX( float fValue )
{
    FlyAabb tempBox;
    tempBox.SetEmpty();

    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        (*ent)->RotateX( fValue );
        tempBox.Merge( (*ent)->GetBoundingBox() );
    }

    if( m_Entities.size() != 0 )
        m_Bounds = tempBox;
}


//------------------------------------------------------------------------
// Rotate the selected entities around the Y- axis.
//------------------------------------------------------------------------
void etSelectionBuffer::RotateY( float fValue )
{
    FlyAabb tempBox;
    tempBox.SetEmpty();

    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        (*ent)->RotateY( fValue );
        tempBox.Merge( (*ent)->GetBoundingBox() );
    }

    if( m_Entities.size() != 0 )
        m_Bounds = tempBox;
}


//------------------------------------------------------------------------
// Rotate the selected entities around the Z- axis.
//------------------------------------------------------------------------
void etSelectionBuffer::RotateZ( float fValue )
{
    FlyAabb tempBox;
    tempBox.SetEmpty();

    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        (*ent)->RotateZ( fValue );
        tempBox.Merge( (*ent)->GetBoundingBox() );
    }

    if( m_Entities.size() != 0 )
        m_Bounds = tempBox;
}


//------------------------------------------------------------------------
// Scale the selected entities.
//------------------------------------------------------------------------
void etSelectionBuffer::Scale( const FlyVector& vScale )
{
    FlyVector vOldScale;
    FlyAabb tempBox;
    tempBox.SetEmpty();

    EntityList::iterator ent;
    for( ent=m_Entities.begin();ent!=m_Entities.end();ent++ )
    {
        vOldScale = (*ent)->GetScale();
        vOldScale += vScale;
        (*ent)->SetScale( vOldScale );
        tempBox.Merge( (*ent)->GetBoundingBox() );
    }

    if( m_Entities.size() != 0 )
        m_Bounds = tempBox;
}


//------------------------------------------------------------------------
// Get the count of the selected entities.
//------------------------------------------------------------------------
UINT etSelectionBuffer::GetNumSelected(void) const
{
    return m_Entities.size();
}

//------------------------------------------------------------------------
// Is the cursor in the selection rectangle.
//------------------------------------------------------------------------
bool etSelectionBuffer::IsInSelection( const FlyRay& ray )
{
    return m_Bounds.Intersects( ray,999999.0,NULL );
}


//------------------------------------------------------------------------
// Update the selectionBuffer for the editor.
//------------------------------------------------------------------------
void etSelectionBuffer::UpdateObject(void)
{
}


//------------------------------------------------------------------------
// Render the selectionBuffer for the editor.
//------------------------------------------------------------------------
void etSelectionBuffer::RenderObject(void)
{
    if( m_bDraggingRect )
    {
        m_pSelRect->UpdateSelection( m_ptStart,m_ptEnd );
        m_pSelRect->AddToRenderQueue();
    }

    if( m_bSelecting )
        m_pSelAxis->AddToRenderQueue();
}


//------------------------------------------------------------------------
// Begin to drag the selection rectangle.
// ----------------------------------------------------------------------
// Param -> IN:
//      const POINT&:   Start position of the cursor.
//------------------------------------------------------------------------
void etSelectionBuffer::BeginDragRect( const POINT& ptStart )
{
    m_bDraggingRect = true;
    m_ptStart = ptStart;
    m_ptEnd = ptStart;
}

//------------------------------------------------------------------------
// Dragging the rectangle for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      const POINT&:   Current end position of the cursor.
//------------------------------------------------------------------------
void etSelectionBuffer::DraggingRect( const POINT& ptEnd )
{
    m_ptEnd = ptEnd;
}

//------------------------------------------------------------------------
// End to drag the rectangle for the editor.
//------------------------------------------------------------------------
void etSelectionBuffer::EndDragRect(void)
{
    m_bDraggingRect = false;

    RECT rcSel;
    rcSel.left = m_ptStart.x;
    rcSel.top = m_ptStart.y;
    rcSel.right = m_ptEnd.x;
    rcSel.bottom = m_ptEnd.y;

    // Get the EntityManager.
    etEntityManager* pEntityMgr = etCoreManager::Instance().GetEntityManager();
    pEntityMgr->SelectIntersects( rcSel,this );
}


//------------------------------------------------------------------------
// Get the selected entities for the editor.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the entity.
//------------------------------------------------------------------------
etEntity* etSelectionBuffer::GetSelectedEntity( UINT nIndex )
{
    if( nIndex >= m_Entities.size() )
        return NULL;

    return m_Entities[nIndex];
}


//////////////////////////////////////////////////////////////////////////
// IMPLEMENTING THE CLASS ETSELECTIONRECT.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etSelectionRect::etSelectionRect( etSelectionBuffer* pParent )
{
    m_pParent = pParent;
    m_nPriority = PRIORITY_SURFACE;
    m_nNumVerts = 4;
    m_nNumIndis = 0;
    m_nNumPrims = 2;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Create the vertex buffer.
    m_pVB = pResMgr->MakeVertexBuffer( sizeof(SVERTEX)*4,BU_WRITEONLY,MM_MANAGED );

    // Create the material for the rectangle.
    m_pMaterial = pResMgr->CreateMaterial( "et-SelectionRect" );
    m_pMaterial->SetSceneAlphaMode( 0,true,false );
    m_pMaterial->SetAlphaBlendMode( 0,SBM_SRCALPHA,SBM_INVSRCALPHA );
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etSelectionRect::~etSelectionRect(void)
{
    SAFE_DELETE( m_pVB );

    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    pResMgr->DeleteMaterial( m_pMaterial );
}


//------------------------------------------------------------------------
// Update the selection rectangle.
// ----------------------------------------------------------------------
// Param -> IN:
//      const POINT&:   The first point of the rectangle.
//      const POINT&:   The second point of the rectangle.
//------------------------------------------------------------------------
void etSelectionRect::UpdateSelection( const POINT& ptStart,const POINT& ptEnd )
{
    SVERTEX verts[4];
    verts[0].position = FlyVector((float)ptStart.x,(float)ptEnd.y,0.0f,1.0f);
    verts[0].color = 0x66ff00ff;
    verts[1].position = FlyVector((float)ptStart.x,(float)ptStart.y,0.0f,1.0f);
    verts[1].color = 0x66ff00ff;
    verts[2].position = FlyVector((float)ptEnd.x,(float)ptEnd.y,0.0f,1.0f);
    verts[2].color = 0x66ff00ff;
    verts[3].position = FlyVector((float)ptEnd.x,(float)ptStart.y,0.0f,1.0f);
    verts[3].color = 0x66ff00ff;

    void* ptr = m_pVB->Lock( LOCK_NORMAL );
    memcpy( ptr,verts,sizeof(SVERTEX)*4 );
    m_pVB->Unlock();
}


//------------------------------------------------------------------------
// Render the selection rectangle.
//------------------------------------------------------------------------
void etSelectionRect::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = etCoreManager::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( RT_TRIANGLESTRIP );
    pRenderer->RenderBuffer( VF_STENCILSHADOW,0,4,m_pVB );
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT etSelectionRect::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT etSelectionRect::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT etSelectionRect::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& etSelectionRect::GetBoundingBox(void) const
{
    return FlyAabb::EMPTY;
}

//------------------------------------------------------------------------
// Get the square distance to the camera.
//------------------------------------------------------------------------
float etSelectionRect::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    return 0.0f;
}

//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void etSelectionRect::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = FlyMatrix::IDENTITY;
}

//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT etSelectionRect::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT etSelectionRect::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
//------------------------------------------------------------------------
FlyLightObject* etSelectionRect::GetLightObject( UINT nIndex )
{
    return NULL;
}


//////////////////////////////////////////////////////////////////////////
// IMPLEMENTING THE CLASS ETSELECTIONAXIS.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etSelectionAxis::etSelectionAxis( etSelectionBuffer* pParent )
{
    m_pParent = pParent;
    m_nPriority = PRIORITY_HIGH;
    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_pVBMoveY = NULL;
    m_pVBRotateX = NULL;
    m_pVBRotateY = NULL;
    m_pVBRotateZ = NULL;
    m_pVBScale = NULL;

    BuildAxisMoveXZ();
    BuildAxisMoveY();
    BuildAxisRotateX();
    BuildAxisRotateY();
    BuildAxisRotateZ();
    BuildAxisScale();

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Create the material.
    m_pMaterial = pResMgr->GetMaterialByName( "et-Brush" );
    if( !m_pMaterial )
    {
        m_pMaterial = pResMgr->CreateMaterial( "et-Brush" );
        m_pMaterial->SetDepthMode( 0,DEPTH_NONE );
    }
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etSelectionAxis::~etSelectionAxis(void)
{
    SAFE_DELETE( m_pVBMoveXZ );
    SAFE_DELETE( m_pVBMoveY );
    SAFE_DELETE( m_pVBRotateX );
    SAFE_DELETE( m_pVBRotateY );
    SAFE_DELETE( m_pVBRotateZ );
    SAFE_DELETE( m_pVBScale );
}


//------------------------------------------------------------------------
// Build the selection axis. (MoveXZ)
//------------------------------------------------------------------------
HRESULT etSelectionAxis::BuildAxisMoveXZ(void)
{
    VERTEXL* pContent;

    // Get the the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    m_pVBMoveXZ = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*12,BU_WRITEONLY,
        MM_MANAGED );
    if( !m_pVBMoveXZ ) return FLY_CREATEBUFFER;
    pContent = (VERTEXL*)m_pVBMoveXZ->Lock( LOCK_NORMAL );

#define _SET_VERTEX( a,b,c ) pContent->x = a;\
                             pContent->y = b;\
                             pContent->z = c;\
                             pContent->color = 0xff00ffff;\
                             pContent++;

    _SET_VERTEX( 0.0f,0.0f,0.0f )
    _SET_VERTEX( 1.5f,0.0f,0.0f )
    _SET_VERTEX( 0.0f,0.0f,0.0f )
    _SET_VERTEX( 0.0f,0.0f,1.5f )
    _SET_VERTEX( 1.5f,0.0f,0.0f )
    _SET_VERTEX( 1.2f,0.0f,0.3f )
    _SET_VERTEX( 1.5f,0.0f,0.0f )
    _SET_VERTEX( 1.2f,0.0f,-0.3f )
    _SET_VERTEX( 0.0f,0.0f,1.5f )
    _SET_VERTEX( -0.3f,0.0f,1.2f )
    _SET_VERTEX( 0.0f,0.0f,1.5f )
    _SET_VERTEX( 0.3f,0.0f,1.2f )

#undef _SET_VERTEX

    m_pVBMoveXZ->Unlock();
    return FLY_OK;
}

//------------------------------------------------------------------------
// Build the selection axis. (MoveY)
//------------------------------------------------------------------------
HRESULT etSelectionAxis::BuildAxisMoveY(void)
{
    VERTEXL* pContent;

    // Get the the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    m_pVBMoveY = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*10,BU_WRITEONLY,
        MM_MANAGED );
    if( !m_pVBMoveY ) return FLY_CREATEBUFFER;
    pContent = (VERTEXL*)m_pVBMoveY->Lock( LOCK_NORMAL );

#define _SET_VERTEX( a,b,c ) pContent->x = a;\
                             pContent->y = b;\
                             pContent->z = c;\
                             pContent->color = 0xff00ffff;\
                             pContent++;

    _SET_VERTEX( 0.0f,-1.5f,0.0f )
    _SET_VERTEX( 0.0f,1.5f,0.0f )
    _SET_VERTEX( 0.0f,-1.5f,0.0f )
    _SET_VERTEX( -0.3f,-1.2f,0.0f )
    _SET_VERTEX( 0.0f,-1.5f,0.0f )
    _SET_VERTEX( 0.3f,-1.2f,0.0f )
    _SET_VERTEX( 0.0f,1.5f,0.0f )
    _SET_VERTEX( -0.3f,1.2f,0.0f )
    _SET_VERTEX( 0.0f,1.5f,0.0f )
    _SET_VERTEX( 0.3f,1.2f,0.0f )

#undef _SET_VERTEX

    m_pVBMoveY->Unlock();
    return FLY_OK;
}

//------------------------------------------------------------------------
// Build the selection axis. (RotateX)
//------------------------------------------------------------------------
HRESULT etSelectionAxis::BuildAxisRotateX(void)
{
    VERTEXL* pContent;

    // Get the the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    m_pVBRotateX = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*22,BU_WRITEONLY,
        MM_MANAGED );
    if( !m_pVBRotateX ) return FLY_CREATEBUFFER;
    pContent = (VERTEXL*)m_pVBRotateX->Lock( LOCK_NORMAL );

#define _SET_VERTEX( a,b,c ) pContent->x = a;\
                             pContent->y = b;\
                             pContent->z = c;\
                             pContent->color = 0xff00ffff;\
                             pContent++;

    _SET_VERTEX( -1.5f,0.0f,0.0f )
    _SET_VERTEX( 1.5f,0.0f,0.0f )
    _SET_VERTEX( 1.5f,0.0f,0.0f )
    _SET_VERTEX( 1.2f,0.0f,-0.3f )
    _SET_VERTEX( 1.5f,0.0f,0.0f )
    _SET_VERTEX( 1.2f,0.0f,0.3f )

    const float PI6 = FLY_PI / 6.0f;
    for( int i=0;i<6;i++ )
    {
        _SET_VERTEX( 0.0f,-cosf(PI6*i)*0.9f,-sinf(PI6*i)*0.9f )
        _SET_VERTEX( 0.0f,-cosf(PI6*(i+1))*0.9f,-sinf(PI6*(i+1))*0.9f )
    }

    _SET_VERTEX( 0.0f,0.9f,0.0f )
    _SET_VERTEX( 0.0f,1.2f,-0.3f )
    _SET_VERTEX( 0.0f,0.9f,0.0f )
    _SET_VERTEX( 0.0f,0.6f,-0.3f )

#undef _SET_VERTEX

    m_pVBRotateX->Unlock();
    return FLY_OK;
}

//------------------------------------------------------------------------
// Build the selection axis. (RotateY)
//------------------------------------------------------------------------
HRESULT etSelectionAxis::BuildAxisRotateY(void)
{
    VERTEXL* pContent;

    // Get the the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    m_pVBRotateY = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*22,BU_WRITEONLY,
        MM_MANAGED );
    if( !m_pVBRotateY ) return FLY_CREATEBUFFER;
    pContent = (VERTEXL*)m_pVBRotateY->Lock( LOCK_NORMAL );

#define _SET_VERTEX( a,b,c ) pContent->x = a;\
                             pContent->y = b;\
                             pContent->z = c;\
                             pContent->color = 0xff00ffff;\
                             pContent++;

    _SET_VERTEX( 0.0f,-1.5f,0.0f )
    _SET_VERTEX( 0.0f,1.5f,0.0f )
    _SET_VERTEX( 0.0f,1.5f,0.0f )
    _SET_VERTEX( -0.3f,1.2f,0.0f )
    _SET_VERTEX( 0.0f,1.5f,0.0f )
    _SET_VERTEX( 0.3f,1.2f,0.0f )

    const float PI6 = FLY_PI / 6.0f;
    for( int i=0;i<6;i++ )
    {
        _SET_VERTEX( -cosf(PI6*i)*0.9f,0.0f,-sinf(PI6*i)*0.9f )
        _SET_VERTEX( -cosf(PI6*(i+1))*0.9f,0.0f,-sinf(PI6*(i+1))*0.9f )
    }

    _SET_VERTEX( 0.9f,0.0f,0.0f )
    _SET_VERTEX( 1.2f,0.0f,-0.3f )
    _SET_VERTEX( 0.9f,0.0f,0.0f )
    _SET_VERTEX( 0.6f,0.0f,-0.0f )

#undef _SET_VERTEX

    m_pVBRotateY->Unlock();
    return FLY_OK;
}

//------------------------------------------------------------------------
// Build the selection axis. (RotateZ)
//------------------------------------------------------------------------
HRESULT etSelectionAxis::BuildAxisRotateZ(void)
{
    VERTEXL* pContent;

    // Get the the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    m_pVBRotateZ = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*22,BU_WRITEONLY,
        MM_MANAGED );
    if( !m_pVBRotateZ ) return FLY_CREATEBUFFER;
    pContent = (VERTEXL*)m_pVBRotateZ->Lock( LOCK_NORMAL );

#define _SET_VERTEX( a,b,c ) pContent->x = a;\
                             pContent->y = b;\
                             pContent->z = c;\
                             pContent->color = 0xff00ffff;\
                             pContent++;

    _SET_VERTEX( 0.0f,0.0f,-1.5f )
    _SET_VERTEX( 0.0f,0.0f,1.5f )
    _SET_VERTEX( 0.0f,0.0f,1.5f )
    _SET_VERTEX( 0.0f,-0.3f,1.2f )
    _SET_VERTEX( 0.0f,0.0f,1.5f )
    _SET_VERTEX( 0.0f,0.3f,1.2f )

    const float PI6 = FLY_PI / 6.0f;
    for( int i=0;i<6;i++ )
    {
        _SET_VERTEX( -sinf(PI6*i)*0.9f,-cosf(PI6*i)*0.9f,0.0f )
        _SET_VERTEX( -sinf(PI6*(i+1))*0.9f,-cosf(PI6*(i+1))*0.9f,0.0f )
    }

    _SET_VERTEX( 0.0f,0.9f,0.0f )
    _SET_VERTEX( -0.3f,1.2f,0.0f )
    _SET_VERTEX( 0.0f,0.9f,0.0f )
    _SET_VERTEX( -0.3f,0.6f,0.0f )

#undef _SET_VERTEX

    m_pVBRotateZ->Unlock();
    return FLY_OK;
}

//------------------------------------------------------------------------
// Build the selection axis. (Scale)
//------------------------------------------------------------------------
HRESULT etSelectionAxis::BuildAxisScale(void)
{
    VERTEXL* pContent;

    // Get the the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    m_pVBScale = pResMgr->MakeVertexBuffer( sizeof(VERTEXL)*12,BU_WRITEONLY,
        MM_MANAGED );
    if( !m_pVBScale ) return FLY_CREATEBUFFER;
    pContent = (VERTEXL*)m_pVBScale->Lock( LOCK_NORMAL );

#define _SET_VERTEX( a,b,c ) pContent->x = a;\
                             pContent->y = b;\
                             pContent->z = c;\
                             pContent->color = 0xff00ffff;\
                             pContent++;

    const float fCos1 = cosf( FLY_2PI/3 );
    const float fSin1 = sinf( FLY_2PI/3 );
    const float fCos2 = cosf( FLY_2PI/3*2 );
    const float fSin2 = sinf( FLY_2PI/3*2 );

    _SET_VERTEX( 0.0f,0.9f,0.0f )
    _SET_VERTEX( fSin1*0.9f,fCos1*0.9f,0.0f )
    _SET_VERTEX( fSin1*0.9f,fCos1*0.9f,0.0f )
    _SET_VERTEX( fSin2*0.9f,fCos2*0.9f,0.0f )
    _SET_VERTEX( fSin2*0.9f,fCos2*0.9f,0.0f )
    _SET_VERTEX( 0.0f,0.9f,0.0f )

    _SET_VERTEX( 0.0f,0.6f,0.0f )
    _SET_VERTEX( fSin1*0.6f,fCos1*0.6f,0.0f )
    _SET_VERTEX( fSin1*0.6f,fCos1*0.6f,0.0f )
    _SET_VERTEX( fSin2*0.6f,fCos2*0.6f,0.0f )
    _SET_VERTEX( fSin2*0.6f,fCos2*0.6f,0.0f )
    _SET_VERTEX( 0.0f,0.6f,0.0f )

#undef _SET_VERTEX

    m_pVBScale->Unlock();
    return FLY_OK;
}


//------------------------------------------------------------------------
// Render the selection axis.
//------------------------------------------------------------------------
void etSelectionAxis::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = etCoreManager::Instance().GetRenderer();
    pRenderer->SetRenderMaterial( m_pMaterial );

    switch( m_pParent->GetToolType() )
    {
    case ETT_MOVE_XZ:
        m_nNumVerts = 12;
        m_nNumPrims = 6;
        pRenderer->SetRenderType( RT_LINELIST );
        pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,0,12,m_pVBMoveXZ );
        break;
    case ETT_MOVE_Y:
        m_nNumVerts = 10;
        m_nNumPrims = 5;
        pRenderer->SetRenderType( RT_LINELIST );
        pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,0,10,m_pVBMoveY );
        break;
    case ETT_ROTATION_X:
        m_nNumVerts = 22;
        m_nNumPrims = 11;
        pRenderer->SetRenderType( RT_LINELIST );
        pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,0,22,m_pVBRotateX );
        break;
    case ETT_ROTATION_Y:
        m_nNumVerts = 22;
        m_nNumPrims = 11;
        pRenderer->SetRenderType( RT_LINELIST );
        pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,0,22,m_pVBRotateY );
        break;
    case ETT_ROTATION_Z:
        m_nNumVerts = 22;
        m_nNumPrims = 11;
        pRenderer->SetRenderType( RT_LINELIST );
        pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,0,22,m_pVBRotateZ );
        break;
    case ETT_SCALE_XYZ:
        m_nNumVerts = 12;
        m_nNumPrims = 6;
        pRenderer->SetRenderType( RT_LINELIST );
        pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,0,12,m_pVBScale );
        break;
    }
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT etSelectionAxis::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT etSelectionAxis::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT etSelectionAxis::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& etSelectionAxis::GetBoundingBox(void) const
{
    return m_pParent->GetLocalBoundingBox();
}

//------------------------------------------------------------------------
// Get the square distance to the camera.
//------------------------------------------------------------------------
float etSelectionAxis::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    return 0.0f;
}

//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void etSelectionAxis::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    const FlyAabb& box = m_pParent->GetSelEntityBBox();

    float fRadius = box.vcMax.x-box.vcMin.x;
    fRadius = FLYMATH_MIN( fRadius,box.vcMax.y-box.vcMin.y );
    fRadius = FLYMATH_MIN( fRadius,box.vcMax.z-box.vcMin.z );

    pMatrices->Identity();
    pMatrices->_11 = fRadius;
    pMatrices->_22 = fRadius;
    pMatrices->_33 = fRadius;

    FlyVector vCenter = box.GetCenter();
    pMatrices->_41 = vCenter.x;
    pMatrices->_42 = vCenter.y;
    pMatrices->_43 = vCenter.z;
}

//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT etSelectionAxis::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT etSelectionAxis::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the pointer to the light object.
//------------------------------------------------------------------------
FlyLightObject* etSelectionAxis::GetLightObject( UINT nIndex )
{
    return NULL;
}