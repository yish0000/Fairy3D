//------------------------------------------------------------------------
// Name: FlyTerrainTile.cpp
// Desc: This file is to implement the class FlyTerrainTile.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "F3DPCH.h"

#if 0
#include "FlyTerrainTile.h"

#include "FlyTerrainLevel.h"
#include "FlyTerrainSceneManager.h"

#include "FlyKernel.h"


//------------------------------------------------------------------------
// Default constructor of the class.
//------------------------------------------------------------------------
FlyTerrainTile::FlyTerrainTile(void)
{
    m_pManager = NULL;
    m_pLevel = NULL;

    memset( m_pNeighbors,0,sizeof(FlyTerrainTile*)*4 );
    memset( &m_Bounds,0,sizeof(FlyAabb) );
    m_nLodLevel = 0;

    m_pVB = NULL;
    m_pMainTexture = NULL;
    memset( m_pDetail,0,sizeof(FlyTexture*)*4 );
    m_fMainScale = 1.0f;
    m_fDetailScale[0] = 1.0f;
    m_fDetailScale[1] = 1.0f;
    m_fDetailScale[2] = 1.0f;
    m_fDetailScale[3] = 1.0f;
    m_pUnit = NULL;
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyTerrainTile::~FlyTerrainTile(void)
{
    SAFE_DELETE( m_pVB );

    SAFE_DELETE( m_pUnit );
}


//------------------------------------------------------------------------
// Initialize the data for terrain tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      int,int:    Position where we start to read the height data.
//      short*:     Pointer to the height data.
//------------------------------------------------------------------------
HRESULT FlyTerrainTile::Initialize( int startx,int startz,const short* pHeightData )
{
    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Create the shared vertex buffer.
    UINT nTileSize = m_pLevel->GetTileSize();
    m_pVB = pResMgr->MakeVertexBuffer( (nTileSize+1)*(nTileSize+1)*sizeof(VERTEX2T),
        BU_WRITEONLY,MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    float fMinHeight = 0.0f;
    float fMaxHeight = 0.0f;

    int endx = startx + nTileSize + 1;
    int endz = startz + nTileSize + 1;
    VERTEX2T* pVert = (VERTEX2T*)m_pVB->Lock( 0,(nTileSize+1)*(nTileSize+1)*sizeof(VERTEX2T),LOCK_NORMAL );

    for( int j=startz;j<endz;j++ )
    {
        for( int i=startx;i<endx;i++ )
        {
            float fHeight = (float)pHeightData[j*(m_pLevel->GetTerrainSize()+1)+i];
            fHeight *= m_pLevel->GetHeightScale();

            pVert->x = (float)i * m_pLevel->GetLengthPerUnit();
            pVert->y = fHeight;
            pVert->z = (float)j * m_pLevel->GetLengthPerUnit();
            pVert->vN[0] = 0.0f;
            pVert->vN[1] = 0.0f;
            pVert->vN[2] = 0.0f;
            pVert->tu0 = ((float)i / (float)nTileSize);
            pVert->tv0 = ((float)j / (float)nTileSize);
            pVert->tu1 = ((float)i / (float)m_pLevel->GetTerrainSize());
            pVert->tv1 = ((float)j / (float)m_pLevel->GetTerrainSize());
            pVert++;

            if( fHeight < fMinHeight )
                fMinHeight = fHeight;

            if( fHeight > fMaxHeight )
                fMaxHeight = fHeight;
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

    // Create the renderable object for the tile.
    m_pUnit = new FlyTerrainRenderUnit( this );
    m_pUnit->m_BBox = m_Bounds;
    m_pUnit->m_pVB = m_pVB;
    m_pUnit->m_nNumVerts = (nTileSize+1)*(nTileSize+1);

    FlyRenderEffect* pEffect = m_pManager->GetRenderEffect();
    m_pUnit->m_pMaterial->AddNewTechnique( pEffect->GetTechniqueByIndex(0) );

    // Initialize the default GPU parameters.
    FlyRenderEffectInstance* pGPUProgram = m_pUnit->m_pMaterial->GetRenderEffectInstance(0);
    *pGPUProgram->GetParameterByIndex(0) = m_pMainTexture;
    *pGPUProgram->GetParameterByIndex(1) = m_pLevel->GetAlphaMap();
    *pGPUProgram->GetParameterByIndex(2) = m_pDetail[0];
    *pGPUProgram->GetParameterByIndex(3) = m_pDetail[1];
    *pGPUProgram->GetParameterByIndex(4) = m_pDetail[2];
    *pGPUProgram->GetParameterByIndex(5) = m_pDetail[3];
    *pGPUProgram->GetParameterByIndex(6) = m_pLevel->GetLightMap();
    *pGPUProgram->GetParameterByIndex(7) = m_pManager->IsLightOpen() ? 1 : 0;
    *pGPUProgram->GetParameterByIndex(8) = m_fMainScale;
    *pGPUProgram->GetParameterByIndex(9) = m_fDetailScale[0];
    *pGPUProgram->GetParameterByIndex(10) = m_fDetailScale[1];
    *pGPUProgram->GetParameterByIndex(11) = m_fDetailScale[2];
    *pGPUProgram->GetParameterByIndex(12) = m_fDetailScale[3];

    m_pUnit->m_pMaterial->AddNewTechnique( pEffect->GetTechniqueByIndex(1) );
    pGPUProgram = m_pUnit->m_pMaterial->GetRenderEffectInstance(1);
    *pGPUProgram->GetParameterByIndex(0) = m_pMainTexture;
    *pGPUProgram->GetParameterByIndex(1) = m_pLevel->GetAlphaMap();
    *pGPUProgram->GetParameterByIndex(2) = m_pDetail[0];
    *pGPUProgram->GetParameterByIndex(3) = m_pDetail[1];
    *pGPUProgram->GetParameterByIndex(4) = m_pDetail[2];
    *pGPUProgram->GetParameterByIndex(5) = m_pDetail[3];
    *pGPUProgram->GetParameterByIndex(6) = m_pLevel->GetLightMap();
    *pGPUProgram->GetParameterByIndex(7) = m_pManager->IsLightOpen() ? 1 : 0;
    *pGPUProgram->GetParameterByIndex(8) = m_fMainScale;
    *pGPUProgram->GetParameterByIndex(9) = m_fDetailScale[0];
    *pGPUProgram->GetParameterByIndex(10) = m_fDetailScale[1];
    *pGPUProgram->GetParameterByIndex(11) = m_fDetailScale[2];
    *pGPUProgram->GetParameterByIndex(12) = m_fDetailScale[3];

    // Enable the default technique.
    m_pUnit->m_pMaterial->SetActiveTechnique( 0 );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Notify the camera's state, Decide the lod of this tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyCameraBase*:     Pointer to the current camera.
//------------------------------------------------------------------------
void FlyTerrainTile::NotifyCamera( FlyCameraBase* pCamera )
{
    FlySceneObject::NotifyCamera( pCamera );

    FlyVector vDiff = pCamera->GetPosition()-m_WorldBBox.GetCenter();
    float fL = vDiff.GetSqrLength();

    m_nLodLevel = -1;
    for( UINT i=0;i<m_pManager->GetMaxLodLevel();i++ )
    {
        if( m_pManager->GetLodSqrDist(i) > fL )
        {
            m_nLodLevel = i - 1;
            break;
        }
    }

    if( m_nLodLevel < 0 )
        m_nLodLevel = m_pManager->GetMaxLodLevel() - 1;
}


//------------------------------------------------------------------------
// Update the terrain tile for the terrain level.
//------------------------------------------------------------------------
void FlyTerrainTile::UpdateObject(void)
{
    m_WorldBBox = m_Bounds;
    m_WorldBBox.TransformBox( GetWorldTransform() );
}


//------------------------------------------------------------------------
// Render the terrain tile.
//------------------------------------------------------------------------
void FlyTerrainTile::RenderObject(void)
{
    // Add the object to renderQueue.
    m_pUnit->AddToRenderQueue();
}


//------------------------------------------------------------------------
// Get the local boundingBox of the tile.
//------------------------------------------------------------------------
const FlyAabb& FlyTerrainTile::GetLocalBoundingBox(void) const
{
    return m_Bounds;
}


//------------------------------------------------------------------------
// Test the intersect with a segment.
//------------------------------------------------------------------------
bool FlyTerrainTile::IntersectSegment( const FlyVector& vStart,const FlyVector& vEnd,
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

    float h = m_pLevel->GetHeightValue( vRay.x,vRay.z );
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
UINT FlyTerrainTile::GetNumNeighbors(void) const
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
int FlyTerrainTile::GetNumDetailLayers(void) const
{
    int n = 0;

    for( int i=0;i<4;i++ )
    {
        if( m_pDetail[i] )
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
FlyTexture* FlyTerrainTile::GetDetailTexture( int nIndex )
{
    return (nIndex<0 || nIndex>=4) ? NULL : m_pDetail[nIndex];
}

//------------------------------------------------------------------------
// Set the detail texture for the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the terrain tile.
//------------------------------------------------------------------------
void FlyTerrainTile::SetDetailTexture( int nIndex,FlyTexture* pTexture )
{
    if( nIndex >= 0 && nIndex < 4 )
    {
        m_pDetail[nIndex] = pTexture;

        char cParamName[20];
        sprintf( cParamName,"g_Detail%d",nIndex );
        FlyRenderEffectInstance* pGPUProgram = m_pUnit->m_pMaterial->GetRenderEffectInstance(0);
        *pGPUProgram->GetParameterByName(cParamName) = pTexture;
    }
}

//------------------------------------------------------------------------
// Set the detail layer's scale for the tile.
//------------------------------------------------------------------------
void FlyTerrainTile::SetDetailScale( int nIndex,float fScale )
{
    if( nIndex >= 0 && nIndex < 4 )
    {
        m_fDetailScale[nIndex] = fScale;

        char cParamName[20];
        sprintf( cParamName,"g_DetailScale%d",nIndex );
        FlyRenderEffectInstance* pGPUProgram = m_pUnit->m_pMaterial->GetRenderEffectInstance(0);
        *pGPUProgram->GetParameterByName(cParamName) = fScale;
    }
}


//------------------------------------------------------------------------
// Set the main texture for the terrain tile.
//------------------------------------------------------------------------
void FlyTerrainTile::SetMainTexture( FlyTexture* pTexture )
{
    m_pMainTexture = pTexture;

    FlyRenderEffectInstance* pGPUProgram = m_pUnit->m_pMaterial->GetRenderEffectInstance(0);
    *pGPUProgram->GetParameterByName("g_MainTexture") = pTexture;
}


//------------------------------------------------------------------------
// Set the scale for the main texture.
//------------------------------------------------------------------------
void FlyTerrainTile::SetMainScale( float fScale )
{
    m_fMainScale = fScale;

    FlyRenderEffectInstance* pGPUProgram = m_pUnit->m_pMaterial->GetRenderEffectInstance(0);
    *pGPUProgram->GetParameterByName("g_MainScale") = fScale;
}


//------------------------------------------------------------------------
// Get the current index buffer of the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyGraphicBuffer**:     To store the index buffer.
//      UINT*:                  To store the count of indices.
//------------------------------------------------------------------------
void FlyTerrainTile::GetIndexBuffer( FlyGraphicBuffer** ppIB,UINT* pNumIndices )
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
    m_pManager->GetIndexBuffer( m_nLodLevel,nStitchFlags,&pIB,&nNumIndices );
    if( !pIB )
    {
        // Create the new indexBuffer.
        if( m_pManager->IsUseTriStripMode() )
            GenerateStripIndexData( nStitchFlags,&pIB,&nNumIndices );
        else
            GenerateListIndexData( nStitchFlags,&pIB,&nNumIndices );

        // Add the new indexBuffer to the list.
        m_pManager->AddIndexBuffer( pIB,nNumIndices,m_nLodLevel,nStitchFlags );
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
void FlyTerrainTile::GenerateListIndexData( UINT nStitchFlags,FlyGraphicBuffer** ppIB,
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

    for( UINT j=south;j<nTileSize-north;j+=step )
    {
        for( UINT i=west;i<nTileSize-east;i+=step )
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
// Generate the triangle strip indices for the tile.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:               Flags when generating the buffer.
//      FlyGraphicBuffer**: To store the index buffer.
//      UINT*:              To store the count of indices.
//------------------------------------------------------------------------
void FlyTerrainTile::GenerateStripIndexData( UINT nStitchFlags,FlyGraphicBuffer** ppIB,
                                             UINT* pNumIndices )
{
    int step = 1 << m_nLodLevel;
    int lowstep = 1 << (m_nLodLevel + 1);

    int numIndices = 0;

    UINT nTileSize = m_pLevel->GetTileSize();

    // Create the index buffer.
    UINT nLengthBuf = ((nTileSize/step)*2+3)*(nTileSize/step+2);

    // Get the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    FlyGraphicBuffer* pIB = pResMgr->MakeIndexBuffer( nLengthBuf*sizeof(WORD),BU_WRITEONLY,
        MM_MANAGED,IF_INDEX16 );
    WORD* pIndices = (WORD*)pIB->Lock( 0,nLengthBuf*sizeof(WORD),LOCK_NORMAL );

#define _Index(a,b) ((b)*(nTileSize+1)+(a))

    for( UINT j=0;j<nTileSize;j+=step )
    {
        for( UINT i=0;i<nTileSize;i+=step )
        {
            int x[4],y[4];
            x[0] = x[1] = i;
            x[2] = x[3] = i + step;
            y[0] = y[2] = j;
            y[1] = y[3] = j + step;

            if( j == 0  && (nStitchFlags & STITCH_NORTH) )
            {
                if( x[0] % lowstep != 0 )
                    x[0] -= step;

                if( x[2] % lowstep != 0 )
                    x[2] -= step;
            }

            if( i == 0  && (nStitchFlags & STITCH_WEST) )
            {
                if( y[0] % lowstep != 0 )
                    y[0] -= step;

                if( y[1] % lowstep != 0 )
                    y[1] -= step;
            }

            if( i == (nTileSize-1-step) && (nStitchFlags & STITCH_EAST) )
            {
                if( y[2] % lowstep != 0 )
                    y[2] -= step;

                if( y[3] % lowstep != 0 )
                    y[3] -= step;
            }

            if( i == 0 )
            {
                *pIndices++ = _Index( x[0],y[0] );
                numIndices++;
            }

            *pIndices++ = _Index( x[1],y[1] );
            *pIndices++ = _Index( x[2],y[2] );
            numIndices += 2;

            if( i == nTileSize-step )
            {
                *pIndices++ = _Index( x[3],y[3] );
                numIndices++;

                if( j < nTileSize-step )
                {
                    // Emit this index twice more (this is to turn around without
                    // artefacts)
                    // ** Hmm, looks like we can drop this and it's unnoticeable
                    *pIndices++ = _Index( x[3],y[3] ); numIndices++;
                    *pIndices++ = _Index( x[3],y[3] ); numIndices++;
                }
            }
        }

        j += step;
        for( UINT i=nTileSize;i>0;i-=step )
        {
            int x[4],y[4];
            x[0] = x[1] = i;
            x[2] = x[3] = i - step;
            y[0] = y[2] = j;
            y[1] = y[3] = j + step;

            if( j == (nTileSize-step) && (nStitchFlags & STITCH_SOUTH) )
            {
                if( x[1] % lowstep != 0 )
                    x[1] -= step;

                if( x[3] % lowstep != 0 )
                    x[3] -= step;
            }

            if( i == step  && (nStitchFlags & STITCH_WEST) )
            {
                if( y[2] % lowstep != 0 )
                    y[2] -= step;

                if( y[3] % lowstep != 0 )
                    y[3] -= step;
            }

            if( i == nTileSize && (nStitchFlags & STITCH_EAST) )
            {
                if( y[0] % lowstep != 0 )
                    y[0] -= step;

                if( y[1] % lowstep != 0 )
                    y[1] -= step;
            }

            if( i == nTileSize+1 )
            {
                *pIndices++ = _Index( x[0],y[0] );
                numIndices++;
            }

            *pIndices++ = _Index( x[1],y[1] );
            *pIndices++ = _Index( x[2],y[2] );
            numIndices += 2;

            if( i == step )
            {
                *pIndices++ = _Index( x[3],y[3] );
                numIndices++;

                if( j < nTileSize-step )
                {
                    *pIndices++ = _Index( x[3],y[3] );
                    numIndices++;
                }
            }
        }
    }

#undef _Index

    pIB->Unlock();

    *ppIB = pIB;
    *pNumIndices = numIndices;
}


//------------------------------------------------------------------------
// Stitch the edge which two sides have the different lod level.
// This code is from the source of OGRE.
//------------------------------------------------------------------------
UINT FlyTerrainTile::StitchEdge( FlyTerrainTile::Neighbor nb,int hiLod,int loLod,bool omitFirst,
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
    case FlyTerrainTile::NB_SOUTH:
        startx = starty = 0;
        endx = nTileSize;
        rowstep = step;
        horizontal = true;
        break;
    case FlyTerrainTile::NB_NORTH:
        // invert x AND y direction, helps to keep same winding
        startx = starty = nTileSize;
        endx = 0;
        rowstep = -step;
        step = -step;
        superstep = -superstep;
        halfsuperstep = -halfsuperstep;
        horizontal = true;
        break;
    case FlyTerrainTile::NB_EAST:
        startx = 0;
        endx = nTileSize;
        starty = nTileSize;
        rowstep = -step;
        horizontal = false;
        break;
    case FlyTerrainTile::NB_WEST:
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
        int k = 0;
        for( ;k!=halfsuperstep;k+=step )
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


//////////////////////////////////////////////////////////////////////////
// IMPLEMENTING THE CLASS FLYTERRAINRENDERUNIT.

//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyTerrainRenderUnit::FlyTerrainRenderUnit( FlyTerrainTile* pParent )
{
    m_pParent = pParent;

    m_pVB = NULL;
    m_pIB = NULL;

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;

    memset( &m_BBox,0,sizeof(FlyAabb) );

    // Create a new material.
    char cMatName[80];
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    sprintf( cMatName,"%s_material",m_pParent->GetName() );
    m_pMaterial = pResMgr->CreateMaterial( cMatName );
}

//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyTerrainRenderUnit::~FlyTerrainRenderUnit(void)
{
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    pResMgr->DeleteMaterial( m_pMaterial );
}

//------------------------------------------------------------------------
// Render the terrain tile.
//------------------------------------------------------------------------
void FlyTerrainRenderUnit::Render(void)
{
    // Get the pointer to the renderer.
    LPFLYRENDERER pRenderer = FlyKernel::Instance().GetRenderer();

    // Update the index buffer for unit.
    m_pParent->GetIndexBuffer( &m_pIB,&m_nNumIndis );
    if( m_pParent->GetSceneManager()->IsUseTriStripMode() )
    {
        m_nNumPrims = m_nNumIndis - 2;
        m_RenderType = RT_TRIANGLESTRIP;
    }
    else
    {
        m_nNumPrims = m_nNumIndis / 3;
        m_RenderType = RT_TRIANGLELIST;
    }

    pRenderer->SetRenderMaterial( m_pMaterial );
    pRenderer->SetRenderType( m_RenderType );
    pRenderer->RenderBuffer( VF_TWOTEXTURE,m_nNumVerts,m_nNumIndis,m_pVB,m_pIB );
}

//------------------------------------------------------------------------
// Get the count of vertices.
//------------------------------------------------------------------------
UINT FlyTerrainRenderUnit::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the count of the indices.
//------------------------------------------------------------------------
UINT FlyTerrainRenderUnit::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the count of the primitives.
//------------------------------------------------------------------------
UINT FlyTerrainRenderUnit::GetNumPrims(void) const
{
    return m_nNumPrims;
}

//------------------------------------------------------------------------
// Get the local boundingBox of the object.
//------------------------------------------------------------------------
const FlyAabb& FlyTerrainRenderUnit::GetBoundingBox(void) const
{
    return m_BBox;
}

//------------------------------------------------------------------------
// Get the square distance to the camera.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
float FlyTerrainRenderUnit::GetSqrCameraDist( const FlyVector& vCamPos ) const
{
    FlyVector vTemp = m_BBox.GetCenter() * m_pParent->GetWorldTransform();

    return (vTemp - vCamPos).GetSqrLength();
}

//------------------------------------------------------------------------
// Get the world matrices of the object.
//------------------------------------------------------------------------
void FlyTerrainRenderUnit::GetWorldMatrices( FlyMatrix* pMatrices ) const
{
    *pMatrices = m_pParent->GetWorldTransform();
}

//------------------------------------------------------------------------
// Get the count of the world matrices.
//------------------------------------------------------------------------
UINT FlyTerrainRenderUnit::GetNumWorldMatrices(void) const
{
    return 1;
}

//------------------------------------------------------------------------
// Get the count of the light object.
//------------------------------------------------------------------------
UINT FlyTerrainRenderUnit::GetLightCount(void) const
{
    return 0;
}

//------------------------------------------------------------------------
// Get the specified light object.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        Index of the light object.
//------------------------------------------------------------------------
FlyLightObject* FlyTerrainRenderUnit::GetLightObject( UINT nIndex )
{
    return NULL;
}

//------------------------------------------------------------------------
// Get the pointer to the vertex buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyTerrainRenderUnit::GetVertexBuffer(void)
{
    return m_pVB;
}

//------------------------------------------------------------------------
// Get the pointer to the index buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlyTerrainRenderUnit::GetIndexBuffer(void)
{
    return m_pIB;
}

#endif