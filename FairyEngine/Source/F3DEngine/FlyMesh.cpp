//------------------------------------------------------------------------
// Name: FlyMesh.cpp
// Desc: A simple format of file to store a model data.
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#include "FlyMesh.h"

#include "FlyKernel.h"
#include "FlyStringUtil.h"


/*************************************************************************
*                                                                        *
*    IMPLEMENTING THE CLASS FLYMESH                                      *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyMesh::FlyMesh(void)
{
    m_sName = "";
    m_nDirtyState = 0;
    m_BBox.SetEmpty();
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyMesh::~FlyMesh(void)
{
    for( size_t i=0;i<m_SubMeshes.size();i++ )
        SAFE_DELETE( m_SubMeshes[i] );

    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    for( size_t i=0;i<m_Materials.size();i++ )
        pResMgr->DeleteTexture( m_Materials[i].pTexture );
}


//------------------------------------------------------------------------
// Destroy the mesh resource, clean all the memory.
//------------------------------------------------------------------------
void FlyMesh::Destroy(void)
{
    size_t i = 0;
    for( ;i<m_SubMeshes.size();i++ )
        SAFE_DELETE( m_SubMeshes[i] );

    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    for( i=0;i<m_Materials.size();i++ )
        pResMgr->DeleteTexture( m_Materials[i].pTexture );

    m_Materials.clear();
    m_SubMeshes.clear();
    m_BBox.SetEmpty();

    // Update the dirty state.
    m_nDirtyState++;
}


//------------------------------------------------------------------------
// Create a sub mesh object for the mesh.
//------------------------------------------------------------------------
FlySubMesh* FlyMesh::CreateSubMesh(void)
{
    FlySubMesh* pSubMesh = new FlySubMesh( this );
    if( !pSubMesh ) return NULL;

    m_SubMeshes.push_back( pSubMesh );

    // Update the dirty state.
    m_nDirtyState++;

    return pSubMesh;
}


//------------------------------------------------------------------------
// Destroy a specified sub mesh.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the sub mesh.
//------------------------------------------------------------------------
void FlyMesh::DestroySubMesh( UINT nIndex )
{
    if( nIndex >= m_SubMeshes.size() )
        return;

    SAFE_DELETE( m_SubMeshes[nIndex] );
    m_SubMeshes.erase( m_SubMeshes.begin() + nIndex );

    // Rebuild the boundingBox.
    RebuildBBox();

    // Update the dirty state.
    m_nDirtyState++;
}


//------------------------------------------------------------------------
// Get the count of sub meshes.
//------------------------------------------------------------------------
UINT FlyMesh::GetNumSubMeshes(void) const
{
    return (UINT)m_SubMeshes.size();
}


//------------------------------------------------------------------------
// Get the specified sub mesh.
//------------------------------------------------------------------------
FlySubMesh* FlyMesh::GetSubMesh( UINT nIndex )
{
    if( nIndex >= m_SubMeshes.size() )
        return NULL;

    return m_SubMeshes[nIndex];
}


//------------------------------------------------------------------------
// Create a material for the mesh.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the render material.
//------------------------------------------------------------------------
void FlyMesh::CreateMaterial( const FLYCOLOR& cAmbient,const FLYCOLOR& cDiffuse,
                              const FLYCOLOR& cSpecular,const FLYCOLOR& cEmissive,
                              float fPower,const char* cTexture )
{
    sMeshMaterial material;

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    material.cAmbient = cAmbient;
    material.cDiffuse = cDiffuse;
    material.cSpecular = cSpecular;
    material.cEmissive = cEmissive;
    material.fPower = fPower;
    material.pTexture = pResMgr->Load2DTexture( cTexture,0 );
    m_Materials.push_back( material );
}


//------------------------------------------------------------------------
// Destroy a specified render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the render material.
//------------------------------------------------------------------------
void FlyMesh::DestroyMaterial( size_t nIndex )
{
    if( nIndex >= m_Materials.size() )
        return;

    for( size_t i=0;i<m_SubMeshes.size();i++ )
    {
        if( m_SubMeshes[i]->GetRenderMaterial() == nIndex )
        {
            m_SubMeshes[i]->SetRenderMaterial( -1 );

            // Update the dirty state.
            m_nDirtyState++;
        }
    }

    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();
    pResMgr->DeleteTexture( m_Materials[nIndex].pTexture );
    m_Materials.erase( m_Materials.begin() + nIndex );
}


//------------------------------------------------------------------------
// Get the count of the materials.
//------------------------------------------------------------------------
size_t FlyMesh::GetNumMaterials(void) const
{
    return m_Materials.size();
}


//------------------------------------------------------------------------
// Get a specified render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the render material.
//------------------------------------------------------------------------
const FlyMesh::sMeshMaterial* FlyMesh::GetMaterial( size_t nIndex ) const
{
    if( nIndex >= m_Materials.size() )
        return NULL;

    return &m_Materials[nIndex];
}


//------------------------------------------------------------------------
// Rebuild the boundingBox for the mesh.
//------------------------------------------------------------------------
void FlyMesh::RebuildBBox(void)
{
    m_BBox.SetEmpty();

    for( size_t i=0;i<m_SubMeshes.size();i++ )
    {
        m_BBox.Merge( m_SubMeshes[i]->GetBoundingBox() );
    }
}


//------------------------------------------------------------------------
// Load a number of mesh from a SMD file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the SMD file.
//------------------------------------------------------------------------
HRESULT FlyMesh::LoadMeshFromFile( const char* cModelFile )
{
    HRESULT hr;
    DWORD dwVersion;
    UINT nNumMaterials,nNumBlocks;
    char cIdentifier[4];

    // Open the Mesh file
    FILE* pFile = fopen( cModelFile,"rb" );
    if( !pFile ) return FLY_INVALIDPARAM;

    // Check the file format.
    cIdentifier[0] = fgetc( pFile );
    cIdentifier[1] = fgetc( pFile );
    cIdentifier[2] = fgetc( pFile );
    cIdentifier[3] = fgetc( pFile );
    fread( &dwVersion,sizeof(DWORD),1,pFile );
    if( strcmp(cIdentifier,"SMD") != 0 )
    {
        fclose( pFile );
        return FLY_INVALIDFILE;
    }

    // Clean the old data.
    Destroy();

    // Get the pointer to the resourceManager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Save the model's whole path.
    m_sName = cModelFile;

    // Read the file header.
    fread( &nNumMaterials,sizeof(UINT),1,pFile );
    fread( &nNumBlocks,sizeof(UINT),1,pFile );

    UINT i = 0;
    for( ;i<nNumMaterials;i++ )
    {
        size_t nLen;
        sMeshMaterial material;
        std::string path;
        std::string texture;

        // Read the standard material.
        fread( &material.cAmbient,sizeof(FLYCOLOR),1,pFile );
        fread( &material.cDiffuse,sizeof(FLYCOLOR),1,pFile );
        fread( &material.cEmissive,sizeof(FLYCOLOR),1,pFile );
        fread( &material.cSpecular,sizeof(FLYCOLOR),1,pFile );
        fread( &material.fPower,sizeof(float),1,pFile );

        // Read the texture file name.
        fread( &nLen,sizeof(size_t),1,pFile );
        path.resize( nLen,'\0' );
        fread( &path[0],nLen,1,pFile );

        texture = FlyStringUtil::GetDirFromPath( cModelFile );
        texture += path;
        material.pTexture = pResMgr->Load2DTexture( texture.c_str(),0 );
        m_Materials.push_back( material );
    }

    // Build mesh arrays
    sMeshBlock* pBlocks = new sMeshBlock[nNumBlocks];
    if( !pBlocks )
    {
        fclose( pFile );
        return FLY_OUTOFMEMORY;
    }

    for( i=0;i<nNumBlocks;i++ )
    {
        fread( &pBlocks[i].nMatID,sizeof(UINT),1,pFile );
        fread( &pBlocks[i].nNumVertices,sizeof(UINT),1,pFile );
        fread( &pBlocks[i].nNumFaces,sizeof(UINT),1,pFile );

        pBlocks[i].pVertices = new VERTEX[pBlocks[i].nNumVertices];
        pBlocks[i].pIndices = new WORD[pBlocks[i].nNumFaces*3];
        if( !pBlocks[i].pVertices || !pBlocks[i].pIndices )
        {
            fclose( pFile );
            delete[] pBlocks;
            return FLY_OUTOFMEMORY;
        }

        fread( pBlocks[i].pVertices,sizeof(VERTEX),pBlocks[i].nNumVertices,pFile );
        fread( pBlocks[i].pIndices,sizeof(WORD),pBlocks[i].nNumFaces*3,pFile );
    }

    fclose( pFile );
    hr = BuildMeshInfo( nNumBlocks,pBlocks );
    if( FAILED(hr) )
    {
        delete[] pBlocks;
        return hr;
    }

    delete[] pBlocks;
    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the mesh data to the specified file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the mesh file.
//      bool:           Save the texture file ?
//------------------------------------------------------------------------
void FlyMesh::SaveMeshToFile( const char* cModelFile,bool bSaveTexture/* =false */ )
{
    size_t nNum;
    DWORD dwVersion;

    FILE* pFile = fopen( cModelFile,"wb" );
    if( !pFile ) return;

    // Write the identifer.
    fputc( 'S',pFile );
    fputc( 'M',pFile );
    fputc( 'D',pFile );
    fputc( '\0',pFile );

    // Write the file header.
    dwVersion = FLY_VERSION( 2,0 );
    fwrite( &dwVersion,sizeof(DWORD),1,pFile );
    nNum = m_Materials.size();
    fwrite( &nNum,sizeof(UINT),1,pFile );
    nNum = m_SubMeshes.size();
    fwrite( &nNum,sizeof(UINT),1,pFile );

    // Write the materials.
    size_t i = 0;
    for( ;i<m_Materials.size();i++ )
    {
        fwrite( &m_Materials[i],sizeof(FLYMATERIAL),1,pFile );

        std::string path;
        size_t nIndex;
        std::string texture = m_Materials[i].pTexture->GetName();
        strrev( &texture[0] );

        if( texture != "" )
        {
            path = FlyStringUtil::GetDirFromPath( m_sName.c_str() );
            nIndex = texture.size() - path.size();
            texture[nIndex] = '\0';
            strrev( &texture[0] );
            nIndex = texture.size();
        }
        else
        {
            nIndex = 0;
        }

        fwrite( &nIndex,sizeof(size_t),1,pFile );
        fwrite( texture.c_str(),nIndex,1,pFile );

        // Save the texture of the sub mesh.
        if( bSaveTexture )
        {
            path = FlyStringUtil::GetDirFromPath( cModelFile );
            path += texture;
            m_Materials[i].pTexture->Save2DTexture( path.c_str(),FIMAGE_BMP );
        }
    }

    // Write the sub mesh data.
    for( i=0;i<m_SubMeshes.size();i++ )
    {
        LPVOID pTemp;
        UINT nNumVerts,nNumFaces;
        UINT nBufferSize;

        // Get the sub mesh's material.
        UINT nMatID = m_SubMeshes[i]->GetRenderMaterial();

        nNumVerts = m_SubMeshes[i]->GetNumVerts();
        nNumFaces = m_SubMeshes[i]->GetNumIndis() / 3;

        fwrite( &nMatID,sizeof(UINT),1,pFile );
        fwrite( &nNumVerts,sizeof(UINT),1,pFile );
        fwrite( &nNumFaces,sizeof(UINT),1,pFile );

        nBufferSize = sizeof(VERTEX)*nNumVerts;
        pTemp = m_SubMeshes[i]->GetVertexBuffer()->Lock( 0,nBufferSize,LOCK_NORMAL );
        fwrite( pTemp,nBufferSize,1,pFile );
        m_SubMeshes[i]->GetVertexBuffer()->Unlock();

        nBufferSize = sizeof(WORD)*nNumFaces*3;
        pTemp = m_SubMeshes[i]->GetIndexBuffer()->Lock( 0,nBufferSize,LOCK_NORMAL );
        fwrite( pTemp,nBufferSize,1,pFile );
        m_SubMeshes[i]->GetIndexBuffer()->Unlock();
    }

    fclose( pFile );
}


//------------------------------------------------------------------------
// Build the sub mesh data for the mesh.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT: Count of the blocks.
//      sMeshBlock*: Mesh block array.
//------------------------------------------------------------------------
HRESULT FlyMesh::BuildMeshInfo( UINT nNumBlocks,sMeshBlock* pBlocks )
{
    for( UINT i=0;i<nNumBlocks;i++ )
    {
        HRESULT hr;
        UINT nSize;

        // Create the sub mesh.
        FlySubMesh* pSubMesh = new FlySubMesh( this );
        if( !pSubMesh ) return FLY_OUTOFMEMORY;

        // Create the graphic buffer.
        hr = pSubMesh->CreateGraphicBuffer( pBlocks[i].nNumVertices,pBlocks[i].nNumFaces*3,
            pBlocks[i].nNumFaces );
        if( FAILED(hr) )
        {
            delete pSubMesh;
            return hr;
        }

        nSize = pBlocks[i].nNumVertices*sizeof(VERTEX);
        pSubMesh->GetVertexBuffer()->WriteData( 0,nSize,pBlocks[i].pVertices,false );
        nSize = pBlocks[i].nNumFaces*3*sizeof(WORD);
        pSubMesh->GetIndexBuffer()->WriteData( 0,nSize,pBlocks[i].pIndices,false );

        // Set the material for sub mesh.
        pSubMesh->SetRenderMaterial( pBlocks[i].nMatID );

        // Calculate the boundingbox.
        FlyVector vMin = FlyVector(  999999.9f, 999999.9f, 999999.9f );
        FlyVector vMax = FlyVector( -999999.9f,-999999.9f,-999999.9f );

        for( UINT v=0;v<pBlocks[i].nNumVertices;v++ )
        {
            VERTEX* pVert = &pBlocks[i].pVertices[v];

            if( pVert->x < vMin.x ) vMin.x = pVert->x;
            if( pVert->y < vMin.y ) vMin.y = pVert->y;
            if( pVert->z < vMin.z ) vMin.z = pVert->z;
            if( pVert->x > vMax.x ) vMax.x = pVert->x;
            if( pVert->y > vMax.y ) vMax.y = pVert->y;
            if( pVert->z > vMax.z ) vMax.z = pVert->z;
        }

        pSubMesh->m_BBox.vcMin = vMin;
        pSubMesh->m_BBox.vcMax = vMax;

        m_SubMeshes.push_back( pSubMesh );
    }

    // Calculate the bounding box for mesh.
    FlyVector vMin;
    FlyVector vMax;
    vMin.Set(  999999.9f, 999999.9f, 999999.9f );
    vMax.Set( -999999.9f,-999999.9f,-999999.9f );

    for( size_t s=0;s<m_SubMeshes.size();s++ )
    {
        FlyAabb* box = &m_SubMeshes[s]->m_BBox;

        if( box->vcMin.x < vMin.x ) vMin.x = box->vcMin.x;
        if( box->vcMin.y < vMin.y ) vMin.y = box->vcMin.y;
        if( box->vcMin.z < vMin.z ) vMin.z = box->vcMin.z;
        if( box->vcMax.x > vMax.x ) vMax.x = box->vcMax.x;
        if( box->vcMax.y > vMax.y ) vMax.y = box->vcMax.y;
        if( box->vcMax.z > vMax.z ) vMax.z = box->vcMax.z;
    }

    m_BBox.vcMin = vMin;
    m_BBox.vcMax = vMax;

    // Update the dirty state.
    m_nDirtyState++;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the bounding box of the mesh.
//------------------------------------------------------------------------
const FlyAabb& FlyMesh::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the bounding radius of the mesh.
//------------------------------------------------------------------------
float FlyMesh::GetBoundingRadius(void) const
{
    return m_BBox.GetBoundingRadius();
}


//------------------------------------------------------------------------
// Get the count of vertices.
//------------------------------------------------------------------------
UINT FlyMesh::GetNumVerts(void) const
{
    UINT nNum = 0;

    for( size_t i=0;i<m_SubMeshes.size();i++ )
        nNum += m_SubMeshes[i]->GetNumVerts();

    return nNum;
}


//------------------------------------------------------------------------
// Get the count of indices.
//------------------------------------------------------------------------
UINT FlyMesh::GetNumIndis(void) const
{
    UINT nNum = 0;

    for( size_t i=0;i<m_SubMeshes.size();i++ )
        nNum += m_SubMeshes[i]->GetNumIndis();

    return nNum;
}


//------------------------------------------------------------------------
// Get the count of primitives.
//------------------------------------------------------------------------
UINT FlyMesh::GetNumPrims(void) const
{
    UINT nNum = 0;

    for( size_t i=0;i<m_SubMeshes.size();i++ )
        nNum += m_SubMeshes[i]->GetNumPrims();

    return nNum;
}


//------------------------------------------------------------------------
// Get the name of the mesh.
//------------------------------------------------------------------------
const char* FlyMesh::GetName(void) const
{
    return m_sName.c_str();
}


//------------------------------------------------------------------------
// Set the name for the mesh.
//------------------------------------------------------------------------
void FlyMesh::SetName( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Get the dirty state of the mesh.
//------------------------------------------------------------------------
int FlyMesh::GetDirtyState(void) const
{
    return m_nDirtyState;
}


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYSUBMESH                                  *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor and destructor of the class
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyMesh*:   Pointer to the parent mesh.
//------------------------------------------------------------------------
FlySubMesh::FlySubMesh( FlyMesh* pParent )
{
    m_pParent = pParent;

    m_pVB = NULL;
    m_pIB = NULL;
    memset( &m_BBox,0,sizeof(FlyAabb) );

    m_nNumVerts = 0;
    m_nNumIndis = 0;
    m_nNumPrims = 0;
}

FlySubMesh::~FlySubMesh(void)
{
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );
}


//------------------------------------------------------------------------
// Create the graphic buffers for the sub mesh.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Count of the vertices.
//      UINT:       Count of the indices.
//      UINT:       Count of the primitives.
//------------------------------------------------------------------------
HRESULT FlySubMesh::CreateGraphicBuffer( UINT nNumVerts,UINT nNumIndis,UINT nNumPrims )
{
    // Clean the old data.
    SAFE_DELETE( m_pVB );
    SAFE_DELETE( m_pIB );

    // Get the resource manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    m_nNumVerts = nNumVerts;
    m_nNumIndis = nNumIndis;
    m_nNumPrims = nNumPrims;

    // Create the vertex buffer.
    m_pVB = pResMgr->MakeVertexBuffer( nNumVerts*sizeof(VERTEX),BU_WRITEONLY,MM_MANAGED );
    if( !m_pVB ) return FLY_CREATEBUFFER;

    // Create the index buffer.
    m_pIB = pResMgr->MakeIndexBuffer( nNumIndis*sizeof(WORD),BU_WRITEONLY,MM_MANAGED,IF_INDEX16 );
    if( !m_pIB ) return FLY_CREATEBUFFER;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the bounding box of the sub mesh.
//------------------------------------------------------------------------
const FlyAabb& FlySubMesh::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the render material of the subMesh.
//------------------------------------------------------------------------
UINT FlySubMesh::GetRenderMaterial(void) const
{
    return m_nMaterial;
}

//------------------------------------------------------------------------
// Set the render material for the subMesh.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       Index of the material.
//------------------------------------------------------------------------
void FlySubMesh::SetRenderMaterial( UINT nMaterial )
{
    m_nMaterial = nMaterial;
}


//------------------------------------------------------------------------
// Get the number of the vertices.
//------------------------------------------------------------------------
UINT FlySubMesh::GetNumVerts(void) const
{
    return m_nNumVerts;
}

//------------------------------------------------------------------------
// Get the number of the indices.
//------------------------------------------------------------------------
UINT FlySubMesh::GetNumIndis(void) const
{
    return m_nNumIndis;
}

//------------------------------------------------------------------------
// Get the number of the primitives.
//------------------------------------------------------------------------
UINT FlySubMesh::GetNumPrims(void) const
{
    return m_nNumPrims;
}


//------------------------------------------------------------------------
// Get the pointer to the vertex buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlySubMesh::GetVertexBuffer(void)
{
    return m_pVB;
}

//------------------------------------------------------------------------
// Get the pointer to the index buffer.
//------------------------------------------------------------------------
FlyGraphicBuffer* FlySubMesh::GetIndexBuffer(void)
{
    return m_pIB;
}