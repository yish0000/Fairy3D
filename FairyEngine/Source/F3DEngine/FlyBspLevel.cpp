//------------------------------------------------------------------------
// Name: FlyBspLevel.cpp
// Desc: This file is to implement the class FlyBspLevel.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyBspLevel.h"

#include "FlyKernel.h"
#include "FlyMiscBasis.h"


//------------------------------------------------------------------------
// Constructor of the class.
//------------------------------------------------------------------------
FlyBspLevel::FlyBspLevel(void)
{
    m_pRoot = NULL;
    m_nTesselation = 4;
    m_nNumNodes = 0;
    m_nNumLeaves = 0;
    m_VisData.tableData = NULL;

    m_nNumFaces = 0;
    m_pFaces = NULL;
    m_nNumBrushes = 0;
    m_pBrushes = NULL;

    m_pLeafFaces = NULL;
    m_pLeafBrushes = NULL;

    memset( &m_VisData,0,sizeof(sBspVisData) );
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyBspLevel::~FlyBspLevel(void)
{
    for( UINT i=0;i<m_nNumFaces;i++ )
    {
        SAFE_DELETE_ARRAY( m_pFaces[i].pVerts );
        SAFE_DELETE_ARRAY( m_pFaces[i].pIndis );
    }

    SAFE_DELETE_ARRAY( m_pFaces );
    SAFE_DELETE_ARRAY( m_pLeafFaces );
    SAFE_DELETE_ARRAY( m_pBrushes );
    SAFE_DELETE_ARRAY( m_pLeafBrushes );

    SAFE_DELETE_ARRAY( m_VisData.tableData );

    TextureList::iterator itr;
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Destroy the textures.
    for( itr=m_Textures.begin();itr!=m_Textures.end();itr++ )
        pResMgr->DeleteTexture( *itr );

    // Destroy the lightmaps.
    for( itr=m_Lightmaps.begin();itr!=m_Lightmaps.end();itr++ )
        pResMgr->DeleteTexture( *itr );
}


//------------------------------------------------------------------------
// Load the bsp indoor scene data from the quake3 level.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the bsp file.
//------------------------------------------------------------------------
HRESULT FlyBspLevel::LoadQuake3Level( const char* cFilename )
{
    HRESULT hr;

    FlyQuake3Level level;
    hr = level.LoadQuake3Map( cFilename );
    if( FAILED(hr) ) return hr;

    // Generate the faces.
    hr = GenerateFaces( level );
    if( FAILED(hr) ) return hr;

    // Create textures.
    hr = CreateTextures( level );
    if( FAILED(hr) ) return hr;

    // Create the lightmaps.
    hr = CreateLightmaps( level );
    if( FAILED(hr) ) return hr;

    // Generate the bsp nodes for the scene.
    hr = GenerateBspNodes( level );
    if( FAILED(hr) ) return hr;

    // Generate the brush data to test collision.
    hr = GenerateBrushes( level );
    if( FAILED(hr) ) return hr;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Generate the face data, make the polygon face, mesh face and 
// patches.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyQuake3Level&:      Contain the quake3 bsp data.
//------------------------------------------------------------------------
HRESULT FlyBspLevel::GenerateFaces( const FlyQuake3Level& level )
{
    // Create the memory for face directory.
    m_nNumFaces = level.m_nNumFaces;
    m_pFaces = new sBspFace[m_nNumFaces];
    if( !m_pFaces ) return FLY_OUTOFMEMORY;
    memset( m_pFaces,0,sizeof(sBspFace)*m_nNumFaces );

    // Calculate the count of each polygons.
    for( int i=0;i<level.m_nNumFaces;i++ )
    {
        switch( level.m_pFaces[i].type )
        {
        case BSP_FACE_POLYGON:
            MakePolygonFace( i,level );
            break;
        case BSP_FACE_MESH:
            MakeMeshFace( i,level );
            break;
        case BSP_FACE_PATCH:
            MakePatchFace( i,level );
            break;
        }

        // Update the boundingBox for face.
        sBspFace* pFace = &m_pFaces[i];
        for( int v=0;v<pFace->nNumVerts;v++ )
        {
            if( pFace->pVerts[v].x < pFace->BBox.vcMin.x )
                pFace->BBox.vcMin.x = pFace->pVerts[v].x;
            if( pFace->pVerts[v].y < pFace->BBox.vcMin.y )
                pFace->BBox.vcMin.y = pFace->pVerts[v].y;
            if( pFace->pVerts[v].z < pFace->BBox.vcMin.z )
                pFace->BBox.vcMin.z = pFace->pVerts[v].z;

            if( pFace->pVerts[v].x > pFace->BBox.vcMax.x )
                pFace->BBox.vcMax.x = pFace->pVerts[v].x;
            if( pFace->pVerts[v].y > pFace->BBox.vcMax.y )
                pFace->BBox.vcMax.y = pFace->pVerts[v].y;
            if( pFace->pVerts[v].z > pFace->BBox.vcMax.z )
                pFace->BBox.vcMax.z = pFace->pVerts[v].z;
        }
    }

    // Copy the leaf faces index.
    m_pLeafFaces = new int[level.m_nNumLeafFaces];
    if( !m_pLeafFaces ) return FLY_OUTOFMEMORY;
    memcpy( m_pLeafFaces,level.m_pLeafFaces,sizeof(int)*level.m_nNumLeafFaces );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Make the polygon faces for the bsp level.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        ID of the face to make.
//      const FlyQuake3Level&: Reference of the quake3 level.
//------------------------------------------------------------------------
void FlyBspLevel::MakePolygonFace( int nFaceID,const FlyQuake3Level& level )
{
    int temp = 0;
    int nIndexStart;

    // Get the face.
    sBspFace* pFace = &m_pFaces[nFaceID];

    pFace->nTextureID = level.m_pFaces[nFaceID].shader;
    pFace->nLightID = level.m_pFaces[nFaceID].lm_texture;
    pFace->nNumVerts = level.m_pFaces[nFaceID].vert_count;
    pFace->nNumIndis = (pFace->nNumVerts-2) * 3;

    // Create the geometries.
    pFace->pVerts = new sBspVertex[pFace->nNumVerts];
    if( !pFace->pVerts ) return;
    memset( pFace->pVerts,0,sizeof(VERTEX2T)*pFace->nNumVerts );
    pFace->pIndis = new WORD[pFace->nNumIndis];
    if( !pFace->pIndis ) return;
    memset( pFace->pIndis,0,sizeof(WORD)*pFace->nNumIndis );

    nIndexStart = level.m_pFaces[nFaceID].vert_start;
    for( int i=0;i<pFace->nNumVerts;i++ )
    {
        pFace->pVerts[i].x = level.m_pVerts[nIndexStart+i].point[0] / 8;
        pFace->pVerts[i].y = level.m_pVerts[nIndexStart+i].point[2] / 8;
        pFace->pVerts[i].z = level.m_pVerts[nIndexStart+i].point[1] / 8;

        pFace->pVerts[i].vN[0] = level.m_pVerts[nIndexStart+i].normal[0];
        pFace->pVerts[i].vN[1] = level.m_pVerts[nIndexStart+i].normal[2];
        pFace->pVerts[i].vN[2] = level.m_pVerts[nIndexStart+i].normal[1];

        pFace->pVerts[i].tu0 = level.m_pVerts[nIndexStart+i].texture[0];
        pFace->pVerts[i].tv0 = level.m_pVerts[nIndexStart+i].texture[1];
        pFace->pVerts[i].tu1 = level.m_pVerts[nIndexStart+i].lightmap[0];
        pFace->pVerts[i].tv1 = level.m_pVerts[nIndexStart+i].lightmap[1];
    }

    for( int i=0;i<pFace->nNumVerts-2;i++ )
    {
        pFace->pIndis[3*i+0] = 0;
        pFace->pIndis[3*i+1] = temp + 1;
        pFace->pIndis[3*i+2] = temp + 2;
        temp++;
    }
}


//------------------------------------------------------------------------
// Make the mesh faces for the bsp level.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        ID of the face to make.
//      const FlyQuake3Level&: Reference of the quake3 level.
//------------------------------------------------------------------------
void FlyBspLevel::MakeMeshFace( int nFaceID,const FlyQuake3Level& level )
{
    int nIndexStart;

    // Get the face.
    sBspFace* pFace = &m_pFaces[nFaceID];

    pFace->nTextureID = level.m_pFaces[nFaceID].shader;
    pFace->nLightID = level.m_pFaces[nFaceID].lm_texture;
    pFace->nNumVerts = level.m_pFaces[nFaceID].vert_count;
    pFace->nNumIndis = level.m_pFaces[nFaceID].elem_count;

    // Create the geometries
    pFace->pVerts = new sBspVertex[pFace->nNumVerts];
    if( !pFace->pVerts ) return;
    memset( pFace->pVerts,0,sizeof(VERTEX2T)*pFace->nNumVerts );
    pFace->pIndis = new WORD[pFace->nNumIndis];
    if( !pFace->pIndis ) return;
    memset( pFace->pIndis,0,sizeof(WORD)*pFace->nNumIndis );

    nIndexStart = level.m_pFaces[nFaceID].vert_start;
    for( int i=0;i<pFace->nNumVerts;i++ )
    {
        pFace->pVerts[i].x = level.m_pVerts[nIndexStart+i].point[0] / 8;
        pFace->pVerts[i].y = level.m_pVerts[nIndexStart+i].point[2] / 8;
        pFace->pVerts[i].z = level.m_pVerts[nIndexStart+i].point[1] / 8;

        pFace->pVerts[i].vN[0] = level.m_pVerts[nIndexStart+i].normal[0];
        pFace->pVerts[i].vN[1] = level.m_pVerts[nIndexStart+i].normal[2];
        pFace->pVerts[i].vN[2] = level.m_pVerts[nIndexStart+i].normal[1];

        pFace->pVerts[i].tu0 = level.m_pVerts[nIndexStart+i].texture[0];
        pFace->pVerts[i].tv0 = level.m_pVerts[nIndexStart+i].texture[1];
        pFace->pVerts[i].tu1 = level.m_pVerts[nIndexStart+i].lightmap[0];
        pFace->pVerts[i].tv1 = level.m_pVerts[nIndexStart+i].lightmap[1];
    }

    nIndexStart = level.m_pFaces[nFaceID].elem_start;
    for( int i=0;i<pFace->nNumIndis;i++ )
    {
        pFace->pIndis[i] = level.m_pMeshIndices[nIndexStart+i];
    }
}


//------------------------------------------------------------------------
// Make the patch faces for the bsp level.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:        ID of the patch face.
//      const FlyQuake3Level&: Reference of the quake3 level.
//------------------------------------------------------------------------
void FlyBspLevel::MakePatchFace( int nFaceID,const FlyQuake3Level& level )
{
    sBspVertex ctrlPoint[9];
    int nWidth,nHeight;
    int w,h;
    int nCurrentIndex = 0;
    int nCurrentVert = 0;

    // Get the face
    sBspFace* pFace = &m_pFaces[nFaceID];
    bsp_face_t* f = &level.m_pFaces[nFaceID];

    nWidth = f->mesh_cp[0];
    nHeight = f->mesh_cp[1];
    w = (nWidth - 1) / 2;
    h = (nHeight - 1) / 2;
    pFace->nNumVerts = w*h*(m_nTesselation+1)*(m_nTesselation+1);
    pFace->nNumIndis = w*h*m_nTesselation*m_nTesselation*2*3;
    pFace->nTextureID = f->shader;
    pFace->nLightID = f->lm_texture;

    pFace->pVerts = new sBspVertex[pFace->nNumVerts];
    if( !pFace->pVerts ) return;
    pFace->pIndis = new WORD[pFace->nNumIndis];
    if( !pFace->pIndis ) return;

    for( int y=0;y<h;y++ )
    {
        for( int x=0;x<w;x++ )
        {
            for( int r=0;r<3;r++ )
            {
                for( int p=0;p<3;p++ )
                {
                    int ni = f->vert_start + (y*2*nWidth+x*2) + r*nWidth + p;

                    ctrlPoint[r*3+p].x = level.m_pVerts[ni].point[0] / 8;
                    ctrlPoint[r*3+p].y = level.m_pVerts[ni].point[2] / 8;
                    ctrlPoint[r*3+p].z = level.m_pVerts[ni].point[1] / 8;
                    ctrlPoint[r*3+p].vN[0] = level.m_pVerts[ni].normal[0];
                    ctrlPoint[r*3+p].vN[1] = level.m_pVerts[ni].normal[2];
                    ctrlPoint[r*3+p].vN[2] = level.m_pVerts[ni].normal[1];
                    ctrlPoint[r*3+p].tu0 = level.m_pVerts[ni].texture[0];
                    ctrlPoint[r*3+p].tv0 = level.m_pVerts[ni].texture[1];
                    ctrlPoint[r*3+p].tu1 = level.m_pVerts[ni].lightmap[0];
                    ctrlPoint[r*3+p].tv1 = level.m_pVerts[ni].lightmap[1];
                }
            }

            Tesselate( &pFace->pVerts[nCurrentVert],&pFace->pIndis[nCurrentIndex],
                       nCurrentVert,ctrlPoint );

            nCurrentVert += (m_nTesselation+1)*(m_nTesselation+1);
            nCurrentIndex += m_nTesselation*m_nTesselation*2*3;
        }
    }
}


//------------------------------------------------------------------------
// Tesselate the patch's vertices and indices.
// ----------------------------------------------------------------------
// Param -> IN:
//      sBspVertex*:    Pointer to the vertices.
//      WORD*:          Pointer to the indices.
//      int:            Current index start.
//      sBspVertex*:    Pointer to the control vertices.
//------------------------------------------------------------------------
void FlyBspLevel::Tesselate( sBspVertex* pVerts,WORD* pIndis,int nIndexStart,
                             sBspVertex* pCtrlPts )
{
    float px,py;
    sBspVertex temp[3];

    for( UINT i=0;i<=m_nTesselation;i++ )
    {
        px = (float)i / m_nTesselation;

        pVerts[i] = pCtrlPts[0] * ((1.0f-px)*(1.0f-px)) +
                    pCtrlPts[3] * ((1.0f-px)*px*2) +
                    pCtrlPts[6] * (px*px);
    }

    for( UINT u=1;u<=m_nTesselation;u++ )
    {
        py = (float)u / m_nTesselation;

        temp[0] = pCtrlPts[0] * ((1.0f-py)*(1.0f-py)) +
                  pCtrlPts[1] * ((1.0f-py)*py*2) +
                  pCtrlPts[2] * (py*py);

        temp[1] = pCtrlPts[3] * ((1.0f-py)*(1.0f-py)) +
                  pCtrlPts[4] * ((1.0f-py)*py*2) +
                  pCtrlPts[5] * (py*py);

        temp[2] = pCtrlPts[6] * ((1.0f-py)*(1.0f-py)) +
                  pCtrlPts[7] * ((1.0f-py)*py*2) +
                  pCtrlPts[8] * (py*py);

        for( UINT v=0;v<=m_nTesselation;v++ )
        {
            px = (float)v / m_nTesselation;

            pVerts[u*(m_nTesselation+1)+v] = temp[0] * ((1.0f-px)*(1.0f-px)) +
                                             temp[1] * ((1.0f-px)*px*2) +
                                             temp[2] * (px*px);
        }
    }

    for( UINT r=0;r<m_nTesselation;r++ )
    {
        for( UINT p=0;p<m_nTesselation;p++ )
        {
            pIndis[(r*m_nTesselation+p)*6+2] = nIndexStart + r*(m_nTesselation+1)+p;
            pIndis[(r*m_nTesselation+p)*6+1] = nIndexStart + (r+1)*(m_nTesselation+1)+p;
            pIndis[(r*m_nTesselation+p)*6] = nIndexStart + r*(m_nTesselation+1)+p+1;
            pIndis[(r*m_nTesselation+p)*6+5] = nIndexStart + (r+1)*(m_nTesselation+1)+p;
            pIndis[(r*m_nTesselation+p)*6+4] = nIndexStart + (r+1)*(m_nTesselation+1)+p+1;
            pIndis[(r*m_nTesselation+p)*6+3] = nIndexStart + r*(m_nTesselation+1)+p+1;
        }
    }
}


//------------------------------------------------------------------------
// Create the textures for the indoor scene.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyQuake3Level&:      Contain the quake3 bsp data.
//------------------------------------------------------------------------
HRESULT FlyBspLevel::CreateTextures( const FlyQuake3Level& level )
{
    FILE* pTemp;
    TCHAR szTexture[MAX_PATH];

    // Get the Resource manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    for( int i=0;i<level.m_nNumTextures;i++ )
    {
        FlyTexture* pTexture;

        FRStrcpy( szTexture,level.m_sFilename.c_str() );
        FRStrcat( szTexture,level.m_pTextures[i].name );
        FRStrcat( szTexture,_T(".jpg") );

        _tfopen_s( &pTemp,szTexture,_T("rb") );
        if( !pTemp )
        {
            // 如果不存在，则尝试TGA纹理
            FRStrcpy( szTexture,level.m_sFilename.c_str() );
            FRStrcat( szTexture,level.m_pTextures[i].name );
            FRStrcat( szTexture,_T(".tga") );

            _tfopen_s( &pTemp,szTexture,_T("rb") );
            if( !pTemp )
            {
                // 添加一个空纹理指针
                m_Textures.push_back( NULL );
                continue;
            }

            fclose( pTemp );
            pTexture = pResMgr->Load2DTexture( szTexture,1 );
        }

        // 尝试加载JPG文件
        fclose( pTemp );
        pTexture = pResMgr->Load2DTexture( szTexture,1 );

        // 添加新纹理到列表
        m_Textures.push_back( pTexture );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Create the lightmaps for the bsp scene.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyQuake3Level&:      Contain the Quake3 bsp data.
//------------------------------------------------------------------------
HRESULT FlyBspLevel::CreateLightmaps( const FlyQuake3Level& level )
{
    TCHAR cTemp[20];
    float gamma = 2.5f;

    // Get the resource manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    for( int i=0;i<level.m_nNumLightmaps;i++ )
    {
        HRESULT hr;
        BYTE* pBits;
        int nPitch;

        _stprintf_s( cTemp,20,"Lightmap%d",i );
        FlyTexture* pTexture = pResMgr->AddNullTexture( cTemp );
        if( pTexture )
        {
            // Create a 2D texture.
            hr = pTexture->Create2DTexture( 128,128,TU_DEFAULT,PFT_X8R8G8B8,1 );
            if( FAILED(hr) )
            {
                pResMgr->DeleteTexture( pTexture );
                m_Lightmaps.push_back( NULL );
                continue;
            }

            hr = pTexture->Access2DMap( 0,TMA_WRITEONLY,(void**)&pBits,&nPitch );
            if( FAILED(hr) )
            {
                pResMgr->DeleteTexture( pTexture );
                m_Lightmaps.push_back( NULL );
                continue;
            }

            // Brighter the light map.
            for( int j=0;j<128*128;j++ )
            {
                float r,g,b;
                r = level.m_pLightmaps[i].data[j*3+0];
                g = level.m_pLightmaps[i].data[j*3+1];
                b = level.m_pLightmaps[i].data[j*3+2];

                r *= gamma / 255.0f;
                g *= gamma / 255.0f;
                b *= gamma / 255.0f;

                float scale = 1.0f;
                float temp;
                if( r > 1.0f && (temp = 1.0f/r) < scale ) scale = temp;
                if( g > 1.0f && (temp = 1.0f/g) < scale ) scale = temp;
                if( b > 1.0f && (temp = 1.0f/b) < scale ) scale = temp;

                scale *= 255.0f;
                r *= scale;
                g *= scale;
                b *= scale;

                pBits[j*4+0] = (UCHAR)b;
                pBits[j*4+1] = (UCHAR)g;
                pBits[j*4+2] = (UCHAR)r;
                pBits[j*4+3] = 255;
            }

            pTexture->Unaccess2DMap( 0 );
        }
        else
            return FLY_OUTOFMEMORY;

        m_Lightmaps.push_back( pTexture );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Generate the bsp nodes and leaves for the bsp scene.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyQuake3Level&:      Contain the Quake3 bsp data.
//------------------------------------------------------------------------
HRESULT FlyBspLevel::GenerateBspNodes( const FlyQuake3Level& level )
{
    FlyPlane splitter;
    FlyAabb aabb;

    int nLeafStart = level.m_nNumNodes;
    m_nNumNodes = level.m_nNumNodes+level.m_nNumLeaves;
    m_nNumLeaves = level.m_nNumLeaves;

    FlyBspNode* pNodeArray = new FlyBspNode[m_nNumNodes];
    if( !pNodeArray ) return FLY_OUTOFMEMORY;

    for( int i=0;i<level.m_nNumNodes;i++ )
    {
        bsp_node_t* p = &level.m_pNodes[i];
        FlyBspNode* q = &pNodeArray[i];

        q->SetOwner( this );

        aabb.vcMin.x = (float)p->mins[0] / 8;
        aabb.vcMin.y = (float)p->mins[2] / 8;
        aabb.vcMin.z = (float)p->mins[1] / 8;
        aabb.vcMax.x = (float)p->maxs[0] / 8;
        aabb.vcMax.y = (float)p->maxs[2] / 8;
        aabb.vcMax.z = (float)p->maxs[1] / 8;
        q->m_WorldBBox = aabb;

        bsp_plane_t plane = level.m_pPlanes[p->plane];
        splitter.Set( FlyVector(plane.normal[0],plane.normal[2],plane.normal[1]),
                      -plane.dist/8 );
        q->m_Splitter = splitter;

        // Set the child node for this node.
        // if child id is negative, the child is a leaf.
        if( p->front > 0 )
        {
            q->m_pFront = &pNodeArray[p->front];
        }
        else
        {
            q->m_pFront = &pNodeArray[nLeafStart+(~(p->front))];
        }

        if( p->back > 0 )
        {
            q->m_pBack = &pNodeArray[p->back];
        }
        else
        {
            q->m_pBack = &pNodeArray[nLeafStart+(~(p->back))];
        }
    }

    // Copy the leaves.
    for( int i=0;i<level.m_nNumLeaves;i++ )
    {
        bsp_leaf_t* l = &level.m_pLeaves[i];
        FlyBspNode* f = &pNodeArray[nLeafStart+i];

        f->SetOwner( this );

        aabb.vcMin.x = (float)l->mins[0] / 8;
        aabb.vcMin.y = (float)l->mins[2] / 8;
        aabb.vcMin.z = (float)l->mins[1] / 8;
        aabb.vcMax.x = (float)l->maxs[0] / 8;
        aabb.vcMax.y = (float)l->maxs[2] / 8;
        aabb.vcMax.z = (float)l->maxs[1] / 8;
        f->m_WorldBBox = aabb;
        f->m_nFaceStart = l->face_start;
        f->m_nNumFaces = l->face_count;
        f->m_nVisCluster = l->cluster;
    }

    // Get the root node.
    m_pRoot = &pNodeArray[0];

    // Copy the visibility set data.
    m_VisData.nNumClusters = level.m_VisData.cluster_count;
    m_VisData.nRowLength = level.m_VisData.cluster_size;
    int nBitSize = m_VisData.nNumClusters*m_VisData.nRowLength;
    m_VisData.tableData = new unsigned char[nBitSize];
    if( !m_VisData.tableData ) return FLY_OUTOFMEMORY;
    memcpy( m_VisData.tableData,level.m_VisData.data,nBitSize );

    return FLY_OK;
}


//------------------------------------------------------------------------
// Generate the brushes data for the bsp level.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyQuake3Level&:  Reference to the quake3 level.
//------------------------------------------------------------------------
HRESULT FlyBspLevel::GenerateBrushes( const FlyQuake3Level& level )
{
    UINT i;

    int nLeafStart = level.m_nNumNodes;
    m_nNumBrushes = level.m_nNumBrushes;
    m_pBrushes = new sBspBrush[m_nNumBrushes];
    if( !m_pBrushes ) return FLY_OUTOFMEMORY;

    for( i=0;i<m_nNumBrushes;i++ )
    {
        bsp_brush_t* pb = &level.m_pBrushes[i];
        sBspBrush* pBrush = &m_pBrushes[i];

        int nBrushSideIdx,nNumBrushSides;
        nNumBrushSides = pb->numsides;
        nBrushSideIdx = pb->firstside;

        while( nNumBrushSides-- )
        {
            bsp_brushside_t* pSide = &level.m_pBrushSides[nBrushSideIdx];
            bsp_plane_t* pPlane = &level.m_pPlanes[pSide->planenum];

            // Add the plane to out brush.
            FlyPlane plane;
            plane.m_vcN.x = pPlane->normal[0];
            plane.m_vcN.y = pPlane->normal[2];
            plane.m_vcN.z = pPlane->normal[1];
            plane.m_fD = -pPlane->dist / 8.0f;
            pBrush->planes.push_back( plane );
            nBrushSideIdx++;
        }
    }

    // Copy the leaf brush array.
    m_pLeafBrushes = new int[level.m_nNumLeafBrushes];
    if( !m_pLeafBrushes ) return FLY_OUTOFMEMORY;
    memcpy( m_pLeafBrushes,level.m_pLeafBrushes,sizeof(int)*level.m_nNumLeafBrushes );

    // Setup the brushes for the leaves.
    for( i=0;i<m_nNumLeaves;i++ )
    {
        int nBrushIdx,nBrushCount,nRealBrushIdx;
        FlyBspNode* node = &m_pRoot[nLeafStart+i];
        nBrushCount = level.m_pLeaves[i].brush_count;
        nBrushIdx = level.m_pLeaves[i].brush_start;

        while( nBrushCount-- )
        {
            nRealBrushIdx = m_pLeafBrushes[nBrushIdx];
            bsp_brush_t* pb = &level.m_pBrushes[nRealBrushIdx];
            if( pb->numsides > 0 &&
                (level.m_pTextures[pb->shaderIndex].contents & 1) )
            {
                node->m_SolidBrushes.push_back( &m_pBrushes[nRealBrushIdx] );
            }
            nBrushIdx++;
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Check whether it's visible from a leaf to another leaf.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyBspNode*:  From leaf to test.
//      const FlyBspNode*:  To leaf to test.
//------------------------------------------------------------------------
bool FlyBspLevel::IsLeafVisible( const FlyBspNode* pFrom,const FlyBspNode* pTo )
{
    // If the camera is outside the world,
    // Leaf can be seen.
    if( pFrom->m_nVisCluster == -1 )
        return true;

    if( pTo->m_nVisCluster == -1 )
        return false;

    // Use PVS to determine visibility
    return (*(m_VisData.tableData + pFrom->m_nVisCluster*m_VisData.nRowLength +
           ((pTo->m_nVisCluster)>>3)) & (1 << ((pTo->m_nVisCluster) & 7))) != 0;
}


//------------------------------------------------------------------------
// Find the leaf where the camera is in.
// ----------------------------------------------------------------------
// Param -> IN:
//      const FlyVector&:   Position of the camera.
//------------------------------------------------------------------------
FlyBspNode* FlyBspLevel::FindLeaf( const FlyVector& vPos )
{
    FlyBspNode* pNode = m_pRoot;

    while( !pNode->IsLeaf() )
    {
        pNode = pNode->GetNextNode( vPos );
    }

    return pNode;
}


//------------------------------------------------------------------------
// Get the root node for the bsp level.
//------------------------------------------------------------------------
FlyBspNode* FlyBspLevel::GetRootNode(void)
{
    return m_pRoot;
}


//------------------------------------------------------------------------
// Test the Intersection for the ray with the brush.
// ----------------------------------------------------------------------
// Param -> IN:
//      sBspBrush*:     Pointer to the bsp brush.
//      const FlyRay&:  Reference to the test ray.
//      float:          Length of the ray.
//------------------------------------------------------------------------
std::pair<bool,float> FlyBspLevel::IntersectBrush( sBspBrush* pBrush,const FlyRay& ray,
                                                   float fL )
{
    float t;
    bool bAllInside = true;
    std::pair<bool,float> ret;
    std::pair<bool,float> end;
    ret.first = false;
    ret.second = 0.0f;
    end.first = false;
    end.second = 0.0f;

    std::vector<FlyPlane>::iterator itr;
    for( itr=pBrush->planes.begin();itr!=pBrush->planes.end();itr++ )
    {
        FlyPlane plane = *itr;

        if( plane.Classify(ray.m_vcOrig) == FLY_FRONT )
        {
            bAllInside = false;

            if( ((FlyRay)ray).Intersects(plane,false,fL,&t,NULL) )
            {
                ret.first = true;
                ret.second = max( ret.second,t );
            }
            else
            {
                ret.first = false;
                ret.second = 0.0f;
                return ret;
            }
        }
        else
        {
            if( ((FlyRay)ray).Intersects(plane,false,fL,&t,NULL) )
            {
                if( !end.first )
                {
                    end.first = true;
                    end.second = t;
                }
                else
                {
                    end.second = min( t,end.second );
                }
            }
        }
    }

    if( bAllInside )
    {
        ret.first = true;
        ret.second = 0.0f;
        return ret;
    }

    if( end.first )
    {
        if( end.second < ret.second )
        {
            ret.first = false;
            return ret;
        }
    }

    return ret;
}


//------------------------------------------------------------------------
// Get the count of bsp faces.
//------------------------------------------------------------------------
UINT FlyBspLevel::GetNumFaces(void)
{
    return m_nNumFaces;
}

//------------------------------------------------------------------------
// Get the count of bsp leaves.
//------------------------------------------------------------------------
UINT FlyBspLevel::GetNumLeaves(void)
{
    return m_nNumLeaves;
}

//------------------------------------------------------------------------
// Get the count of bsp nodes.
//------------------------------------------------------------------------
UINT FlyBspLevel::GetNumNodes(void)
{
    return m_nNumNodes;
}


//------------------------------------------------------------------------
// Get the tesselation value of the level.
//------------------------------------------------------------------------
UINT FlyBspLevel::GetTesselation(void)
{
    return m_nTesselation;
}

//------------------------------------------------------------------------
// Set the tesselation value for the level.
//------------------------------------------------------------------------
void FlyBspLevel::SetTesselation( UINT nTesselation )
{
    m_nTesselation = nTesselation;
}


//------------------------------------------------------------------------
// Get the specified texture of the level.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the texture.
//------------------------------------------------------------------------
FlyTexture* FlyBspLevel::GetTexture( UINT nIndex )
{
    if( nIndex >= m_Textures.size() )
        return NULL;

    return m_Textures[nIndex];
}


//------------------------------------------------------------------------
// Get the specified lightmap of the level.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the lightmap.
//------------------------------------------------------------------------
FlyTexture* FlyBspLevel::GetLightmap( UINT nIndex )
{
    if( nIndex >= m_Lightmaps.size() )
        return NULL;

    return m_Lightmaps[nIndex];
}


//------------------------------------------------------------------------
// Get the specified bsp face.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the bsp face.
//------------------------------------------------------------------------
sBspFace* FlyBspLevel::GetBspFace( UINT nIndex )
{
    if( nIndex < 0 || nIndex >= m_nNumFaces )
        return NULL;

    return &m_pFaces[nIndex];
}