//------------------------------------------------------------------------
// Name: etTile.cpp
// Desc: This file is to implement the class etTile.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#include "etTile.h"

#include "etTerrain.h"
#include "etSceneManager.h"
#include "etCoreManager.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etTile::etTile(void)
{
    m_pManager = NULL;
    m_pLevel = NULL;

    m_pNeighbors[0] = m_pNeighbors[1] = NULL;
    m_pNeighbors[2] = m_pNeighbors[3] = NULL;
    memset( &m_Bounds,0,sizeof(FlyAabb) );
    m_nLodLevel = 0;

    m_pVB = NULL;
    m_pMainTexture = NULL;
    m_fMainScale = 0.0f;
    m_pDetails[0] = m_pDetails[1] = NULL;
    m_pDetails[2] = m_pDetails[3] = NULL;
    m_fDetailScale[0] = 1.0f;
    m_fDetailScale[1] = 1.0f;
    m_fDetailScale[2] = 1.0f;
    m_fDetailScale[3] = 1.0f;

    m_pRenderable = NULL;
    m_bModified = false;
    m_bSeleced = false;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etTile::~etTile(void)
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pRenderable );
    SAFE_DELETE( m_pSelected );
}


//------------------------------------------------------------------------
// Initialize the terrain tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      int,int:        Position where we start to read the height data.
//      const float*:   Pointer to the height data.
//------------------------------------------------------------------------
HRESULT etTile::Initialize( int startx,int startz,const float* pHeightData )
{
    m_nStartX = startx;
    m_nStartZ = startz;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Create the shared vertex buffer.
    UINT nTileSize = m_pLevel->GetTileSize();
    m_pVB = pResMgr->MakeVertexBuffer( (nTileSize+1)*(nTileSize+1)*sizeof(VERTEX2T),
        BU_WRITEONLY,MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    float fMinHeight = 0.0f;
    float fMaxHeight = 0.0f;

    int endx = startx + nTileSize + 1;
    int endz = startz + nTileSize + 1;
    VERTEX2T* pVerts = (VERTEX2T*)m_pVB->Lock( 0,(nTileSize+1)*(nTileSize+1)*sizeof(VERTEX2T),LOCK_NORMAL );

    for( int j=startz;j<endz;j++ )
    {
        for( int i=startx;i<endx;i++ )
        {
            float fHeight = pHeightData[j*(m_pLevel->GetTerrainSize()+1)+i];
            fHeight *= m_pLevel->GetHeightScale();

            pVerts->x = (float)i * m_pLevel->GetLengthPerUnit();
            pVerts->y = fHeight;
            pVerts->z = (float)j * m_pLevel->GetLengthPerUnit();
            pVerts->vN[0] = 0.0f;
            pVerts->vN[1] = 1.0f;
            pVerts->vN[2] = 0.0f;
            pVerts->tu0 = ((float)i / (float)nTileSize);
            pVerts->tv0 = ((float)j / (float)nTileSize);
            pVerts->tu1 = ((float)i / (float)m_pLevel->GetTerrainSize());
            pVerts->tv1 = ((float)j / (float)m_pLevel->GetTerrainSize());
            pVerts++;

            if( fHeight < fMinHeight ) fMinHeight = fHeight;
            if( fHeight > fMaxHeight ) fMaxHeight = fHeight;
        }
    }

    m_pVB->Unlock();

    // Set the boundingBox.
    m_Bounds.vcMin.x = (float)startx * m_pLevel->GetLengthPerUnit();
    m_Bounds.vcMin.y = fMinHeight - 1.0f;
    m_Bounds.vcMin.z = (float)startz * m_pLevel->GetLengthPerUnit();
    m_Bounds.vcMax.x = (float)(endx-1) * m_pLevel->GetLengthPerUnit();
    m_Bounds.vcMax.y = fMaxHeight + 1.0f;
    m_Bounds.vcMax.z = (float)(endz-1) * m_pLevel->GetLengthPerUnit();

    // Set the boundingRect.
    m_BRect.SetMinPoint( m_Bounds.vcMin.x,m_Bounds.vcMin.z );
    m_BRect.SetMaxPoint( m_Bounds.vcMax.x,m_Bounds.vcMax.z );

    // Create the renderable object for the tile.
    m_pRenderable = new etTileRenderable( this );
    m_pRenderable->m_BBox = m_Bounds;
    m_pRenderable->m_pVB = m_pVB;
    m_pRenderable->m_nNumVerts = (nTileSize+1)*(nTileSize+1);

    // Create the renderable object when tile is selected.
    m_pSelected = new etTileSelected( this );
    m_pSelected->BuildGeometry();

    FlyRenderEffect* pEffect = etCoreManager::Instance().GetTerrainFX();
    m_pRenderable->m_pMaterial->AddNewTechnique( pEffect->GetTechniqueByIndex(0) );
    m_pRenderable->m_pMaterial->AddNewTechnique( pEffect->GetTechniqueByIndex(1) );
    m_pRenderable->m_pMaterial->AddNewTechnique( pEffect->GetTechniqueByIndex(2) );
    m_pRenderable->m_pMaterial->AddNewTechnique( pEffect->GetTechniqueByIndex(3) );
    m_pRenderable->m_pMaterial->SetActiveTechnique( 0 );

    // Initialize the default GPU parameters.
    FlyRenderEffectInstance* pGPUProgram;
    FlyVector vLightDir = m_pManager->GetLightDirection();
    vLightDir.Normalize();
    for( int i=0;i<4;i++ )
    {
        pGPUProgram = m_pRenderable->m_pMaterial->GetRenderEffectInstance(i);
        *pGPUProgram->GetParameterByIndex(0) = m_pMainTexture;
        *pGPUProgram->GetParameterByIndex(1) = m_pLevel->GetDetailLayer()->GetTexture();
        *pGPUProgram->GetParameterByIndex(2) = m_pDetails[0];
        *pGPUProgram->GetParameterByIndex(3) = m_pDetails[1];
        *pGPUProgram->GetParameterByIndex(4) = m_pDetails[2];
        *pGPUProgram->GetParameterByIndex(5) = m_pDetails[3];
        *pGPUProgram->GetParameterByIndex(6) = m_fMainScale;
        *pGPUProgram->GetParameterByIndex(7) = m_fDetailScale[0];
        *pGPUProgram->GetParameterByIndex(8) = m_fDetailScale[1];
        *pGPUProgram->GetParameterByIndex(9) = m_fDetailScale[2];
        *pGPUProgram->GetParameterByIndex(10) = m_fDetailScale[3];
        *pGPUProgram->GetParameterByIndex(11) = m_pManager->GetLightingMode();
        *pGPUProgram->GetParameterByIndex(12) = vLightDir;
        *pGPUProgram->GetParameterByIndex(13) = *((FlyVector*)&m_pManager->GetLightAmbient());
        *pGPUProgram->GetParameterByIndex(14) = *((FlyVector*)&m_pManager->GetLightDiffuse());
        *pGPUProgram->GetParameterByIndex(16) = *((FlyVector*)&m_pManager->GetWireColor());
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Update the vertices for the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      int,int:        Position where we start to read the height data.
//      const short*:   Pointer to the height data.
//------------------------------------------------------------------------
void etTile::UpdateHeightData(void)
{
    float fHeight;
    FlyVector vNormal;
    float fMinHeight = 0.0f;
    float fMaxHeight = 0.0f;

    int endx = m_nStartX + m_pLevel->GetTileSize() + 1;
    int endz = m_nStartZ + m_pLevel->GetTileSize() + 1;

    // Get pointer to the height data.
    float* pHeightData = m_pLevel->GetHeightData();

    // Lock the vertex buffer.
    VERTEX2T* pContent = static_cast<VERTEX2T*>(m_pVB->Lock(LOCK_NORMAL));
    VERTEX2T* pTemp = pContent;

    // Handle the height of the vertices.
    for( int j=m_nStartZ;j<endz;j++ )
    {
        for( int i=m_nStartX;i<endx;i++ )
        {
            fHeight = pHeightData[j*(m_pLevel->GetTerrainSize()+1)+i];
            pTemp->y = fHeight * m_pLevel->GetHeightScale();
            pTemp++;

            if( fHeight < fMinHeight ) fMinHeight = fHeight;
            if( fHeight > fMaxHeight ) fMaxHeight = fHeight;
        }
    }

    // Calculate the normals.
    pTemp = pContent;
    for( j=m_nStartZ;j<endz;j++ )
    {
        for( int i=m_nStartX;i<endx;i++ )
        {
            m_pLevel->GetNormalAt( i,j,&vNormal );
            pTemp->vN[0] = vNormal.x;
            pTemp->vN[1] = vNormal.y;
            pTemp->vN[2] = vNormal.z;
            pTemp++;
        }
    }

    m_pVB->Unlock();

    m_Bounds.vcMin.y = fMinHeight;
    m_Bounds.vcMax.y = fMaxHeight;
}


//------------------------------------------------------------------------
// Notify the camera's state, Decide the lod of this tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyCameraBase*:     Pointer to the current camera.
//------------------------------------------------------------------------
void etTile::NotifyCamera( FlyCameraBase* pCamera )
{
    FlySceneObject::NotifyCamera( pCamera );

    FlyVector vDiff = pCamera->GetPosition()-m_WorldBBox.GetCenter();
    float fL = vDiff.GetSqrLength();

    m_nLodLevel = -1;
    for( int i=0;i<m_pLevel->GetMaxLodLevel();i++ )
    {
        if( m_pLevel->GetLodSqrDist(i) > fL )
        {
            m_nLodLevel = i - 1;
            break;
        }
    }

    if( m_nLodLevel < 0 )
        m_nLodLevel = m_pLevel->GetMaxLodLevel() - 1;
}


//------------------------------------------------------------------------
// Update the terrain tile for the terrain level.
//------------------------------------------------------------------------
void etTile::UpdateObject(void)
{
    m_WorldBBox = m_Bounds;
    m_WorldBBox.TransformBox( GetWorldTransform() );

    if( m_bModified )
    {
        UpdateHeightData();
        m_pSelected->BuildGeometry();
        m_bModified = false;
    }
}


//------------------------------------------------------------------------
// Render the terrain tile.
//------------------------------------------------------------------------
void etTile::RenderObject(void)
{
    // Add the renderable object to renderQueue.
    m_pRenderable->AddToRenderQueue();

    // Is the tile selected ?
    if( m_bSeleced &&
        etCoreManager::Instance().GetWorkMode() == WKM_TEXTURE )
        m_pSelected->AddToRenderQueue();
}


//------------------------------------------------------------------------
// Get the local boundingBox of the tile.
//------------------------------------------------------------------------
const FlyAabb& etTile::GetLocalBoundingBox(void) const
{
    return m_Bounds;
}


//------------------------------------------------------------------------
// Test the intersect with a segment.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   First point of the segment.
//      const FlyVector&:   Second point of the segment.
//      FlyVector*:         To store the point of intersection.
//------------------------------------------------------------------------
bool etTile::IntersectSegment( const FlyVector& vStart,const FlyVector& vEnd,
                               FlyVector* pOut )
{
    FlyVector vDir = vEnd - vStart;
    FlyVector vRay = vStart;

    // 1. Special case...
    if( vDir.x == 0 && vDir.z == 0 )
    {
        float h = m_pLevel->GetScaledHeightValue( vRay.x,vRay.z );

        if( (vStart.y <= h && vEnd.y >= h) ||
            (vStart.y >= h && vEnd.y <= h) )
        {
            if( pOut )
            {
                *pOut = vRay;
                pOut->y = h;
            }

            return true;
        }
        else
        {
            if( pOut ) *pOut = FlyVector( -1.0f,-1.0f,-1.0f );

            return false;
        }
    }

    vDir.Normalize();
    vRay += vDir;

    float h = m_pLevel->GetScaledHeightValue( vRay.x,vRay.z );
    while( vRay.x >= m_Bounds.vcMin.x &&
        vRay.x <= m_Bounds.vcMax.x &&
        vRay.z >= m_Bounds.vcMin.z &&
        vRay.z <= m_Bounds.vcMax.z )
    {
        if( vRay.y <= h )
        {
            if( pOut ) *pOut = vRay;

            return true;
        }
        else
        {
            vRay += vDir;
        }

        if( vRay.y - vEnd.y < 1.0f ) break;
    }

    if( vRay.x < m_Bounds.vcMin.x && m_pNeighbors[NB_WEST] != NULL )
        return m_pNeighbors[NB_WEST]->IntersectSegment( vRay,vEnd,pOut );
    else if( vRay.z < m_Bounds.vcMin.z && m_pNeighbors[NB_SOUTH] != NULL )
        return m_pNeighbors[NB_SOUTH]->IntersectSegment( vRay,vEnd,pOut );
    else if( vRay.x > m_Bounds.vcMax.x && m_pNeighbors[NB_EAST] != NULL )
        return m_pNeighbors[NB_EAST]->IntersectSegment( vRay,vEnd,pOut );
    else if( vRay.z > m_Bounds.vcMax.z && m_pNeighbors[NB_NORTH] != NULL )
        return m_pNeighbors[NB_NORTH]->IntersectSegment( vRay,vEnd,pOut );
    else
    {
        if( pOut ) *pOut = FlyVector( -1.0f,-1.0f,-1.0f );

        return false;
    }
}


//------------------------------------------------------------------------
// Get the number of neighbors.
//------------------------------------------------------------------------
UINT etTile::GetNumNeighbors(void) const
{
    UINT n = 0;

    for( int i=0;i<4;i++ )
    {
        if( m_pNeighbors[i] )
            n++;
    }

    return n;
}


//------------------------------------------------------------------------
// Get the count of the detail layers.
//------------------------------------------------------------------------
UINT etTile::GetNumDetailLayers(void) const
{
    int n = 0;

    for( int i=0;i<4;i++ )
    {
        if( m_pDetails[i] )
            n++;
    }

    return n;
}


//------------------------------------------------------------------------
// Get the specified detail texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the detail texture.
//------------------------------------------------------------------------
FlyTexture* etTile::GetDetailTexture( int nIndex )
{
    return (nIndex<0 || nIndex>=4) ? NULL : m_pDetails[nIndex];
}


//------------------------------------------------------------------------
// Get the specified detail layer's scale.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the detail layer.
//------------------------------------------------------------------------
float etTile::GetDetailScale( int nIndex ) const
{
    return (nIndex<0 || nIndex>= 4) ? 0.0f : m_fDetailScale[nIndex];
}


//------------------------------------------------------------------------
// Set the detail texture for the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:            Index of the terrain tile.
//      FlyTexture*:    Pointer to the texture.
//------------------------------------------------------------------------
void etTile::SetDetailTexture( int nIndex,FlyTexture* pTexture )
{
    FlyRenderEffectInstance* pGPUProgram;

    if( nIndex >= 0 && nIndex < 4 )
    {
        m_pDetails[nIndex] = pTexture;

        for( int i=0;i<4;i++ )
        {
            pGPUProgram = m_pRenderable->m_pMaterial->GetRenderEffectInstance(i);
            *pGPUProgram->GetParameterByIndex(2+nIndex) = pTexture;
        }

        if( !pTexture ) SetDetailScale( nIndex,0.0f );
    }
}


//------------------------------------------------------------------------
// Set the detail layer's scale for the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the detail layer.
//      float:      Scale of the detail layer.
//------------------------------------------------------------------------
void etTile::SetDetailScale( int nIndex,float fScale )
{
    FlyRenderEffectInstance* pGPUProgram;

    if( nIndex >= 0 && nIndex < 4 )
    {
        m_fDetailScale[nIndex] = fScale;

        for( int i=0;i<4;i++ )
        {
            pGPUProgram = m_pRenderable->m_pMaterial->GetRenderEffectInstance(i);
            *pGPUProgram->GetParameterByIndex(7+nIndex) = fScale;
        }
    }
}


//------------------------------------------------------------------------
// Set the main texture for the terrain tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyTexture*:    Pointer to the main texture.
//------------------------------------------------------------------------
void etTile::SetMainTexture( FlyTexture* pTexture )
{
    if( !pTexture )
        pTexture = m_pLevel->GetDefaultTexture();

    FlyRenderEffectInstance* pGPUProgram;
    for( int i=0;i<4;i++ )
    {
        pGPUProgram = m_pRenderable->m_pMaterial->GetRenderEffectInstance(i);
        *pGPUProgram->GetParameterByIndex(0) = pTexture;
    }

    m_pMainTexture = pTexture;
}


//------------------------------------------------------------------------
// Set the scale for the main texture.
// ----------------------------------------------------------------------
// Param -> IN:
//      float:      Scale of the main texture.
//------------------------------------------------------------------------
void etTile::SetMainScale( float fScale )
{
    m_fMainScale = fScale;

    FlyRenderEffectInstance* pGPUProgram;
    for( int i=0;i<4;i++ )
    {
        pGPUProgram = m_pRenderable->m_pMaterial->GetRenderEffectInstance(i);
        *pGPUProgram->GetParameterByIndex(6) = fScale;
    }
}


//------------------------------------------------------------------------
// Get the current index buffer of the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyGraphicBuffer**:     To store the index buffer.
//      UINT*:                  To store the count of indices.
//------------------------------------------------------------------------
void etTile::GetIndexBuffer( FlyGraphicBuffer** ppIB,UINT* pNumIndices )
{
    UINT nStitchFlags = 0;

    if( m_pNeighbors[NB_EAST] != 0 && m_pNeighbors[NB_EAST]->m_nLodLevel > m_nLodLevel )
    {
        nStitchFlags |= STITCH_EAST;
        nStitchFlags |= (m_pNeighbors[NB_EAST]->m_nLodLevel-m_nLodLevel) << STITCH_EAST_SHIFT;
    }

    if( m_pNeighbors[NB_WEST] != 0 && m_pNeighbors[NB_WEST]->m_nLodLevel > m_nLodLevel )
    {
        nStitchFlags |= STITCH_WEST;
        nStitchFlags |= (m_pNeighbors[NB_WEST]->m_nLodLevel-m_nLodLevel) << STITCH_WEST_SHIFT;
    }

    if( m_pNeighbors[NB_SOUTH] != 0 && m_pNeighbors[NB_SOUTH]->m_nLodLevel > m_nLodLevel )
    {
        nStitchFlags |= STITCH_SOUTH;
        nStitchFlags |= (m_pNeighbors[NB_SOUTH]->m_nLodLevel-m_nLodLevel) << STITCH_SOUTH_SHIFT;
    }

    if( m_pNeighbors[NB_NORTH] != 0 && m_pNeighbors[NB_NORTH]->m_nLodLevel > m_nLodLevel )
    {
        nStitchFlags |= STITCH_NORTH;
        nStitchFlags |= (m_pNeighbors[NB_NORTH]->m_nLodLevel-m_nLodLevel) << STITCH_NORTH_SHIFT;
    }

    // Check whether the index buffer has existed ?
    UINT nNumIndices;
    FlyGraphicBuffer* pIB = NULL;
    m_pLevel->GetIndexBuffer( m_nLodLevel,nStitchFlags,&pIB,&nNumIndices );
    if( !pIB )
    {
        // Create the new indexBuffer.
        GenerateListIndexData( nStitchFlags,&pIB,&nNumIndices );

        // Add the new indexBuffer to the list.
        m_pLevel->AddIndexBuffer( pIB,nNumIndices,m_nLodLevel,nStitchFlags );
    }

    *ppIB = pIB;
    *pNumIndices = nNumIndices;
}


//------------------------------------------------------------------------
// Generate the triangle list indices for the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:               Flags when generating the buffer.
//      FlyGraphicBuffer**: To store the index buffer.
//      UINT*:              To store the count of indices.
//------------------------------------------------------------------------
void etTile::GenerateListIndexData( UINT nStitchFlags,FlyGraphicBuffer** ppIB,
                                    UINT* pNumIndices )
{
    UINT nNumIndices = 0;
    int step = 1 << m_nLodLevel;

    int east = nStitchFlags & STITCH_EAST ? step : 0;
    int west = nStitchFlags & STITCH_WEST ? step : 0;
    int south = nStitchFlags & STITCH_SOUTH ? step : 0;
    int north = nStitchFlags & STITCH_NORTH ? step : 0;

    UINT nTileSize = m_pLevel->GetTileSize();
    UINT nBufferLength = (nTileSize/step) * (nTileSize/step) * 6;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Create the index buffer.
    FlyGraphicBuffer* pIB = pResMgr->MakeIndexBuffer( nBufferLength*sizeof(WORD),
        BU_WRITEONLY,MM_MANAGED,IF_INDEX16 );
    if( !pIB ) return;

    WORD* pIndices = (WORD*)pIB->Lock( 0,nBufferLength*sizeof(WORD),LOCK_NORMAL );
    if( !pIndices )
    {
        SAFE_DELETE( pIB );
        return;
    }

#define _Index(a,b) ((b)*(nTileSize+1)+(a))

    for( int j=south;j<nTileSize-north;j+=step )
    {
        for( int i=west;i<nTileSize-east;i+=step )
        {
            *pIndices++ = _Index( i,j );
            *pIndices++ = _Index( i,j+step );
            *pIndices++ = _Index( i+step,j );

            *pIndices++ = _Index( i,j+step );
            *pIndices++ = _Index( i+step,j+step );
            *pIndices++ = _Index( i+step,j );

            nNumIndices += 6;
        }
    }

#undef _Index

    if( north > 0 )
    {
        nNumIndices += StitchEdge( NB_NORTH,m_nLodLevel,m_pNeighbors[NB_NORTH]->m_nLodLevel,
            east>0,west>0,&pIndices );
    }

    if( east > 0 )
    {
        nNumIndices += StitchEdge( NB_EAST,m_nLodLevel,m_pNeighbors[NB_EAST]->m_nLodLevel,
            south>0,north>0,&pIndices );
    }

    if( south > 0 )
    {
        nNumIndices += StitchEdge( NB_SOUTH,m_nLodLevel,m_pNeighbors[NB_SOUTH]->m_nLodLevel,
            west>0,east>0,&pIndices );
    }

    if( west > 0 )
    {
        nNumIndices += StitchEdge( NB_WEST,m_nLodLevel,m_pNeighbors[NB_WEST]->m_nLodLevel,
            north>0,south>0,&pIndices );
    }

    pIB->Unlock();

    *ppIB = pIB;
    *pNumIndices = nNumIndices;
}


//------------------------------------------------------------------------
// Stitch the edge which two sides have the different lod level.
// This code is from the source of OGRE.
//------------------------------------------------------------------------
UINT etTile::StitchEdge( etTile::Neighbor nb,int hiLod,int loLod,bool omitFirst,
                         bool omitLast,WORD** ppIndex )
{
    WORD* pIndices = *ppIndex;

    int step = 1 << hiLod;
    // Step from one vertex to another in the low detail version
    int superstep = 1 << loLod;
    // Step half way between low detail steps
    int halfsuperstep = superstep >> 1;

    UINT nTileSize = m_pLevel->GetTileSize();

    // Work out the starting points and sign of increments
    // We always work the strip clockwise
    int startx, starty, endx, rowstep;
    bool horizontal;
    switch( nb )
    {
    case etTile::NB_SOUTH:
        startx = starty = 0;
        endx = nTileSize;
        rowstep = step;
        horizontal = true;
        break;
    case etTile::NB_NORTH:
        // invert x AND y direction, helps to keep same winding
        startx = starty = nTileSize;
        endx = 0;
        rowstep = -step;
        step = -step;
        superstep = -superstep;
        halfsuperstep = -halfsuperstep;
        horizontal = true;
        break;
    case etTile::NB_EAST:
        startx = 0;
        endx = nTileSize;
        starty = nTileSize;
        rowstep = -step;
        horizontal = false;
        break;
    case etTile::NB_WEST:
        startx = nTileSize;
        endx = 0;
        starty = 0;
        rowstep = step;
        step = -step;
        superstep = -superstep;
        halfsuperstep = -halfsuperstep;
        horizontal = false;
        break;
    };

#define _Index( a,b ) ((b)*(nTileSize+1)+(a))

    UINT numIndexes = 0;
    for( int j=startx;j!=endx;j+=superstep )
    {
        for( int k=0;k!=halfsuperstep;k+=step )
        {
            int jk = j + k;
            //skip the first bit of the corner?
            if ( j!=startx || k!=0 || !omitFirst )
            {
                if( horizontal )
                {
                    *pIndices++ = _Index( j,starty );
                    *pIndices++ = _Index( jk,starty+rowstep );
                    *pIndices++ = _Index( jk+step,starty+rowstep );

                    numIndexes += 3;
                }
                else
                {
                    *pIndices++ = _Index( starty,j );
                    *pIndices++ = _Index( starty+rowstep,jk );
                    *pIndices++ = _Index( starty+rowstep,jk+step );

                    numIndexes += 3;
                }
            }
        }

        if( horizontal )
        {
            *pIndices++ = _Index( j,starty );
            *pIndices++ = _Index( j+halfsuperstep,starty+rowstep );
            *pIndices++ = _Index( j+superstep,starty );

            numIndexes += 3;
        }
        else
        {
            *pIndices++ = _Index( starty,j );
            *pIndices++ = _Index( starty+rowstep,j+halfsuperstep );
            *pIndices++ = _Index( starty,j+superstep );

            numIndexes += 3;
        }

        for( k=halfsuperstep;k!=superstep;k+=step )
        {
            int jk = j + k;
            if( j!=endx-superstep || k!=superstep-step || !omitLast )
            {
                if( horizontal )
                {
                    *pIndices++ = _Index( j+superstep,starty );
                    *pIndices++ = _Index( jk,starty+rowstep );
                    *pIndices++ = _Index( jk+step,starty+rowstep );

                    numIndexes += 3;
                }
                else
                {
                    *pIndices++ = _Index( starty,j+superstep );
                    *pIndices++ = _Index( starty+rowstep,jk );
                    *pIndices++ = _Index( starty+rowstep,jk+step );

                    numIndexes += 3;
                }
            }
        }
    }

#undef _Index

    *ppIndex = pIndices;
    return numIndexes;
}


//------------------------------------------------------------------------
// Sort the detail layers for the tile.
//------------------------------------------------------------------------
void etTile::SortDetailLayers(void)
{
    float fScale;
    FlyTexture* pTemp;

    for( int i=0;i<3;i++ )
    {
        for( int n=0;n<4-i;n++ )
        {
            if( m_pDetails[i] == NULL &&
                m_pDetails[i+1] != NULL )
            {
                pTemp = m_pDetails[i+1];
                m_pDetails[i+1] = NULL;
                m_pDetails[i] = pTemp;
                fScale = m_fDetailScale[i+1];
                m_fDetailScale[i+1] = 0.0f;
                m_fDetailScale[i] = fScale;
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// IMPLEMENTING THE CLASS ETTILERENDERABLE.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etTileRenderable::etTileRenderable( etTile* pParent )
{
    m_pParent = pParent;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    m_pVB = NULL;
    m_pIB = NULL;

    memset( &m_BBox,0,sizeof(FlyAabb) );

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();

    // Create the material if needed.
    char cMatName[80];
    sprintf( cMatName,"et-%s",m_pParent->GetName() );
    m_pMaterial = pResMgr->CreateMaterial( cMatName );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
etTileRenderable::~etTileRenderable(void)
{
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    pResMgr->DeleteMaterial( m_pMaterial );
}


//------------------------------------------------------------------------
// Render the terrain tile.
//------------------------------------------------------------------------
void etTileRenderable::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = etCoreManager::Instance().GetRenderer();

    // Set the indexBuffer for the tile.
    m_pParent->GetIndexBuffer( &m_pIB,&m_nNumIndis );
    m_nNumPrims = m_nNumIndis / 3;

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( VF_TWOTEXTURE,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT etTileRenderable::GetNumVerts(void) const
{
    return m_nNumVerts;
}


//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT etTileRenderable::GetNumIndis(void) const
{
    return m_nNumIndis;
}


//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT etTileRenderable::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the local boundingBox of the tile.
//------------------------------------------------------------------------
const FlyAabb& etTileRenderable::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the square distance to the camera.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float etTileRenderable::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter() * m_pParent->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}

//------------------------------------------------------------------------
// Get the world matrices of the tile.
//------------------------------------------------------------------------
void etTileRenderable::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pParent->GetWorldTransform();
}

//------------------------------------------------------------------------
// Get Count of the world matrices of the tile.
//------------------------------------------------------------------------
UINT etTileRenderable::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT etTileRenderable::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* etTileRenderable::GetLightObject( UINT nIndex )
{
    return NULL;
}


//------------------------------------------------------------------------
// Get the pointer to the vertex buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* etTileRenderable::GetVertexBuffer(void)
{
    return m_pVB;
}

//------------------------------------------------------------------------
// Get the pointer to the index buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* etTileRenderable::GetIndexBuffer(void)
{
    return m_pIB;
}


//////////////////////////////////////////////////////////////////////////
// IMPLEMENTING THE CLASS ETTILESELECTED.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
etTileSelected::etTileSelected( etTile* pParent )
{
    m_pParent = pParent;
    m_RenderType = RT_LINESTRIP;
    m_nPriority = PRIORITY_HIGH;

    m_nNumVerts = 25;
    m_nNumIndis = 0;
    m_nNumPrims = 24;

    // Get the resourceManager.
    FlyResourceManager* pResMgr = etCoreManager::Instance().GetResourceManager();
    m_pVB = pResMgr->MakeVertexBuffer( 25*sizeof(VERTEXL),BU_WRITEONLY,MM_MANAGED );

    VERTEXL* pContent = (VERTEXL*)m_pVB->Lock( LOCK_NORMAL );
    for( int i=0;i<25;i++ )
    {
        pContent->x = 0.0f;
        pContent->y = 0.0f;
        pContent->z = 0.0f;
        pContent->color = FLYCOLOR::Blue().GetRGBAValue();
        pContent->tu = 0.0f;
        pContent->tv = 0.0f;
        pContent++;
    }
    m_pVB->Unlock();

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
etTileSelected::~etTileSelected(void)
{
    SAFE_DELETE( m_pVB );
}


//------------------------------------------------------------------------
// Build the geometry data for the object.
//------------------------------------------------------------------------
void etTileSelected::BuildGeometry(void)
{
    VERTEXL vTemp;
    FlyVector vCenter;

    float fRadius = m_pParent->GetTerrainLevel()->GetTileSize() *
        m_pParent->GetTerrainLevel()->GetLengthPerUnit() / 2.0f;
    float fLen = fRadius / 3.0f;
    vCenter = m_pParent->GetLocalBoundingBox().GetCenter();

    // Get the pointer to the terrain.
    etTerrain* pTerrain = m_pParent->GetTerrainLevel();

    // Lock the vertex buffer, write the data.
    VERTEXL* pContent = static_cast<VERTEXL*>( m_pVB->Lock(LOCK_NORMAL) );

    pContent->x = vCenter.x - fRadius;
    pContent->z = vCenter.z + fRadius;
    pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;
    pContent++;

    vTemp.x = vCenter.x - fRadius;
    vTemp.z = vCenter.z + fRadius;
    for( int i=1;i<=6;i++ )
    {
        pContent->x = vTemp.x + fLen * i;
        pContent->z = vTemp.z;
        pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;
        pContent++;
    }

    vTemp.x = vCenter.x + fRadius;
    vTemp.z = vCenter.z + fRadius;
    for( i=7;i<=12;i++ )
    {
        pContent->x = vTemp.x;
        pContent->z = vTemp.z - fLen * (i-6);
        pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;
        pContent++;
    }

    vTemp.x = vCenter.x + fRadius;
    vTemp.z = vCenter.z - fRadius;
    for( i=13;i<=18;i++ )
    {
        pContent->x = vTemp.x - fLen * (i-12);
        pContent->z = vTemp.z;
        pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;
        pContent++;
    }

    vTemp.x = vCenter.x - fRadius;
    vTemp.z = vCenter.z - fRadius;
    for( i=19;i<=24;i++ )
    {
        pContent->x = vTemp.x;
        pContent->z = vTemp.z + fLen * (i-18);
        pContent->y = pTerrain->GetScaledHeightValue(pContent->x,pContent->z)+1.0f;
        pContent++;
    }

    m_pVB->Unlock();
}


//------------------------------------------------------------------------
// Render the object when the tile is selected.
//------------------------------------------------------------------------
void etTileSelected::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = etCoreManager::Instance().GetRenderer();

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( VF_DEFAULTNOLIGHT,25,0,m_pVB,NULL );
}


//------------------------------------------------------------------------
// Get the count of the vertices.
//------------------------------------------------------------------------
UINT etTileSelected::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT etTileSelected::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT etTileSelected::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the local boundingBox of the tile.
//------------------------------------------------------------------------
const FlyAabb& etTileSelected::GetBoundingBox(void) const
{
    return m_pParent->GetLocalBoundingBox();
}


//------------------------------------------------------------------------
// Get the square distance to the camera.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float etTileSelected::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    return 0.0f;
}


//------------------------------------------------------------------------
// Get the world matrices of the tile.
//------------------------------------------------------------------------
void etTileSelected::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = FlyMatrix::IDENTITY;
}

//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT etTileSelected::GetNumWorldMatrices(void) const
{
    return 1;
}


//------------------------------------------------------------------------
// Get the count of the light objects.
//------------------------------------------------------------------------
UINT etTileSelected::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:    Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* etTileSelected::GetLightObject( UINT nIndex )
{
    return NULL;
}