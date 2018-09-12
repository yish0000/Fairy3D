//------------------------------------------------------------------------
// Name: FlyAnimation.cpp
// Desc: This file is to implement the class FlyAnimation.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyAnimation.h"

#include "FlyKernel.h"
#include "FlyStringUtil.h"


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyAnimation::FlyAnimation(void)
{
    m_sName = "";

    m_pVertices = NULL;
    m_pVertices_Orig = NULL;
    m_pFaces = NULL;
    m_pMeshes = NULL;
    m_pMaterials = NULL;

    memset( &m_sHeader,0,sizeof(sModelHeader) );
    memset( &m_BBox,0,sizeof(FlyAabb) );

    m_nDirtyState = 0;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyAnimation::~FlyAnimation(void)
{
    Destroy();
}


//------------------------------------------------------------------------
// Destroy the animation, clean all the memory.
//------------------------------------------------------------------------
void FlyAnimation::Destroy(void)
{
    FlyTexture* pTexture;

    // Get the resource manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Clear all the anim node.
    for( size_t i=0;i<m_AnimGroup.size();i++ )
        SAFE_DELETE_ARRAY( m_AnimGroup[i].pJoints );
    m_AnimGroup.clear();

    // Clear the materials.
    for( size_t i=0;i<m_RMaterials.size();i++ )
    {
        pTexture = m_RMaterials[i]->GetTexture( 0,0 );
        if( pTexture ) pResMgr->DeleteTexture( pTexture );
        pTexture = m_RMaterials[i]->GetTexture( 0,1 );
        if( pTexture ) pResMgr->DeleteTexture( pTexture );
        pResMgr->DeleteMaterial( m_RMaterials[i] );
    }
    m_RMaterials.clear();

    // Clear the geometry datas.
    SAFE_DELETE_ARRAY( m_pVertices );
    SAFE_DELETE_ARRAY( m_pVertices_Orig );
    SAFE_DELETE_ARRAY( m_pFaces );
    SAFE_DELETE_ARRAY( m_pMeshes );
    SAFE_DELETE_ARRAY( m_pMaterials );

    memset( &m_sHeader,0,sizeof(sModelHeader) );
    memset( &m_BBox,0,sizeof(sModelHeader) );

    // Update the dirty state.
    m_nDirtyState++;
}


//------------------------------------------------------------------------
// Load the data from the *.amd animation file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the model file.
//------------------------------------------------------------------------
HRESULT FlyAnimation::LoadAnimation( const char* cModelFile )
{
    DWORD dw;
    char cIdentifier[4];
    sChunk chunk;
    bool bLoaded[6];

    FILE* pFile = fopen( cModelFile,"rb" );
    if( !pFile ) return FLY_FILENOTFOUND;

    // Check the file's format.
    cIdentifier[0] = fgetc( pFile );
    cIdentifier[1] = fgetc( pFile );
    cIdentifier[2] = fgetc( pFile );
    cIdentifier[3] = fgetc( pFile );
    fread( &dw,sizeof(DWORD),1,pFile );
    if( strcmp(cIdentifier,"AMD") != 0 &&
        dw != AMD_VERSION(2,0) )
    {
        fclose( pFile );
        return FLY_INVALIDFILE;
    }

    m_sName = cModelFile;

    // Clean the old data.
    Destroy();

    // Start to read the model file.
    memset( bLoaded,0,6 );

    while( true )
    {
        UINT i;
        size_t nLen;
        bool bEnd = false;

        // Read the chunk head.
        fread( &chunk,sizeof(sChunk),1,pFile );

        switch( chunk.wIdentifier )
        {
        case CHK_HEADER:
            if( bLoaded[0] )
            {
                fclose( pFile );
                return FLY_INVALIDFILE;
            }

            fread( &nLen,sizeof(size_t),1,pFile );
            m_sHeader.sAuthor.resize( nLen,'\0' );
            fread( &m_sHeader.sAuthor[0],nLen,1,pFile );
            fread( &nLen,sizeof(size_t),1,pFile );
            m_sHeader.sModelName.resize( nLen,'\0' );
            fread( &m_sHeader.sModelName[0],nLen,1,pFile );
            fread( &m_sHeader.nBonesPerVert,sizeof(UINT),5,pFile );
            bLoaded[0] = true;
            break;
        case CHK_VERTEX:
            {
                if( !bLoaded[0] || bLoaded[1] )
                {
                    fclose( pFile );
                    return FLY_INVALIDFILE;
                }

                m_pVertices = new AVERTEX[m_sHeader.nNumVerts];
                if( !m_pVertices )
                {
                    fclose( pFile );
                    return FLY_OUTOFMEMORY;
                }

                for( i=0;i<m_sHeader.nNumVerts;i++ )
                {
                    UINT nBoneID[4];
                    float fWeight[4];
                    FlyVector vNormal;

                    fread( &m_pVertices[i].x,sizeof(float),3,pFile );
                    fread( &m_pVertices[i].tu,sizeof(float),2,pFile );
                    fread( &vNormal,sizeof(FlyVector),1,pFile );
                    m_pVertices[i].vN[0] = vNormal.x;
                    m_pVertices[i].vN[1] = vNormal.y;
                    m_pVertices[i].vN[2] = vNormal.z;
                    fread( nBoneID,sizeof(UINT),m_sHeader.nBonesPerVert,pFile );
                    fread( fWeight,sizeof(float),m_sHeader.nBonesPerVert,pFile );
                    m_pVertices[i].fBone1 = (float)nBoneID[0];
                    m_pVertices[i].fBone2 = (float)nBoneID[1];
                    m_pVertices[i].fWeight1 = fWeight[0];
                    m_pVertices[i].fWeight2 = fWeight[1];
                }

                bLoaded[1] = true;
            }
            break;
        case CHK_VERTEX_ORIG:
            if( !bLoaded[0] || bLoaded[2] )
            {
                fclose( pFile );
                return FLY_INVALIDFILE;
            }

            m_pVertices_Orig = new sVertexOrig[m_sHeader.nNumVerts];
            if( !m_pVertices_Orig )
            {
                fclose( pFile );
                return FLY_OUTOFMEMORY;
            }

            fread( m_pVertices_Orig,sizeof(sVertexOrig),m_sHeader.nNumVerts,pFile );
            bLoaded[2] = true;
            break;
        case CHK_FACE:
            if( !bLoaded[0] || bLoaded[3] )
            {
                fclose( pFile );
                return FLY_INVALIDFILE;
            }

            m_pFaces = new sFace[m_sHeader.nNumFaces];
            if( !m_pFaces )
            {
                fclose( pFile );
                return FLY_OUTOFMEMORY;
            }

            fread( m_pFaces,sizeof(sFace),m_sHeader.nNumFaces,pFile );
            bLoaded[3] = true;
            break;
        case CHK_MESH:
            if( !bLoaded[0] || bLoaded[4] )
            {
                fclose( pFile );
                return FLY_INVALIDFILE;
            }

            m_pMeshes = new sMesh[m_sHeader.nNumMeshes];
            if( !m_pMeshes )
            {
                fclose( pFile );
                return FLY_OUTOFMEMORY;
            }

            for( i=0;i<m_sHeader.nNumMeshes;i++ )
            {
                fread( &nLen,sizeof(size_t),1,pFile );
                m_pMeshes[i].sName.resize( nLen,'\0' );
                fread( &m_pMeshes[i].sName[0],nLen,1,pFile );
                fread( &m_pMeshes[i].nOffsetVerts,sizeof(UINT),5,pFile );
            }

            bLoaded[4] = true;
            break;
        case CHK_MATERIAL:
            if( !bLoaded[0] || bLoaded[5] )
            {
                fclose( pFile );
                return FLY_INVALIDFILE;
            }

            m_pMaterials = new sMaterial[m_sHeader.nNumMaterials];
            if( !m_pMaterials )
            {
                fclose( pFile );
                return FLY_OUTOFMEMORY;
            }

            for( i=0;i<m_sHeader.nNumMaterials;i++ )
            {
                size_t nLen;
                std::string path;

                fread( &m_pMaterials[i],68,1,pFile );

                fread( &nLen,sizeof(size_t),1,pFile );
                path.resize( nLen,'\0' );
                fread( &path[0],nLen,1,pFile );

                if( path != "" )
                {
                    m_pMaterials[i].sTexture_A = FlyStringUtil::GetDirFromPath( cModelFile );
                    m_pMaterials[i].sTexture_A += path;
                }

                fread( &nLen,sizeof(size_t),1,pFile );
                path.resize( nLen,'\0' );
                fread( &path[0],nLen,1,pFile );

                if( path != "" )
                {
                    m_pMaterials[i].sTexture_B = FlyStringUtil::GetDirFromPath( cModelFile );
                    m_pMaterials[i].sTexture_B += path;
                }
            }

            bLoaded[5] = true;
            break;
        case CHK_END:
            bEnd = true;
            break;

        // Unknown file chunk.
        default: fseek( pFile,chunk.nChunkSize,SEEK_CUR );
        }

        if( bEnd ) break;
    }

    // Close the model file.
    fclose( pFile );

    // Build the boundingBox.
    m_BBox.vcMin.Set( -999999.0f,-999999.0f,-999999.0f );
    m_BBox.vcMax.Set(  999999.0f, 999999.0f, 999999.0f );

    for( UINT n=0;n<m_sHeader.nNumVerts;n++ )
    {
        AVERTEX* pVert = &m_pVertices[n];

        if( pVert->x < m_BBox.vcMin.x ) m_BBox.vcMin.x = pVert->x;
        if( pVert->y < m_BBox.vcMin.y ) m_BBox.vcMin.y = pVert->y;
        if( pVert->z < m_BBox.vcMin.z ) m_BBox.vcMin.z = pVert->z;
        if( pVert->x > m_BBox.vcMax.x ) m_BBox.vcMax.x = pVert->x;
        if( pVert->y > m_BBox.vcMax.y ) m_BBox.vcMax.y = pVert->y;
        if( pVert->z > m_BBox.vcMax.z ) m_BBox.vcMax.z = pVert->z;
    }

    // 输出顶点数据
    FILE* pFile1 = fopen( "vertex.txt","w" );
    FILE* pFile2 = fopen( "vertex_orig.txt","w" );
    fprintf( pFile1,"%d\n",m_sHeader.nNumVerts );
    fprintf( pFile1,"%d\n",m_sHeader.nNumFaces );
    for( UINT i=0;i<m_sHeader.nNumVerts;i++ )
    {
        fprintf( pFile1,"%.8f,%.8f,%.8f  %4d,%4d,%.8f,%.8f\n",m_pVertices[i].x,m_pVertices[i].y,
            m_pVertices[i].z,(UINT)m_pVertices[i].fBone1,(UINT)m_pVertices[i].fBone2,
            m_pVertices[i].fWeight1,m_pVertices[i].fWeight2 );
        fprintf( pFile2,"%.8f,%.8f,%.8f\n",m_pVertices_Orig[i].fXYZ[0],m_pVertices_Orig[i].fXYZ[1],
            m_pVertices_Orig[i].fXYZ[2] );
    }
    fclose( pFile1 );
    fclose( pFile2 );

    // Build the render materials
    HRESULT hr = BuildMaterials();
    if( FAILED(hr) ) return hr;

    // Update the dirty state.
    m_nDirtyState++;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Add an animation node from the *.anm file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the animation file.
//------------------------------------------------------------------------
HRESULT FlyAnimation::AddAnimNode( const char* cAnimFile )
{
    DWORD dw;
    char cIdentifier[4];
    sChunk chunk;
    bool bLoaded[2];
    sAnimNode animNode;

    FILE* pFile = fopen( cAnimFile,"rb" );
    if( !pFile ) return FLY_FILENOTFOUND;

    // Is the file valid format ?
    cIdentifier[0] = fgetc( pFile );
    cIdentifier[1] = fgetc( pFile );
    cIdentifier[2] = fgetc( pFile );
    cIdentifier[3] = fgetc( pFile );
    fread( &dw,sizeof(DWORD),1,pFile );
    if( strcmp(cIdentifier,"ANM") != 0 || 
        dw != AMD_VERSION(2,0) )
    {
        fclose( pFile );
        return FLY_INVALIDFILE;
    }

    // Start to read the animation file.
    bLoaded[0] = bLoaded[1] = false;

    while( true )
    {
        UINT n;
        bool bEnd = false;
        size_t nLen;

        // Read the chunk head.
        fread( &chunk,sizeof(sChunk),1,pFile );

        switch( chunk.wIdentifier )
        {
        case CHK_ANIMHEADER:
            if( bLoaded[0] )
            {
                fclose( pFile );
                return FLY_INVALIDFILE;
            }

            fread( &nLen,sizeof(size_t),1,pFile );
            animNode.header.sAuthor.resize( nLen,'\0' );
            fread( &animNode.header.sAuthor[0],nLen,1,pFile );
            fread( &nLen,sizeof(size_t),1,pFile );
            animNode.header.sAnimName.resize( nLen,'\0' );
            fread( &animNode.header.sAnimName[0],nLen,1,pFile );
            fread( &animNode.header.nNumJoints,sizeof(UINT),1,pFile );
            fread( &animNode.header.nNumFrames,sizeof(UINT),1,pFile );

            bLoaded[0] = true;
            break;
        case CHK_JOINT:
            if( !bLoaded[0] || bLoaded[1] )
            {
                fclose( pFile );
                return FLY_INVALIDFILE;
            }

            animNode.pJoints = new sJoint[animNode.header.nNumJoints];
            if( !animNode.pJoints )
            {
                fclose( pFile );
                return FLY_OUTOFMEMORY;
            }
            memset( animNode.pJoints,0,sizeof(sJoint)*animNode.header.nNumJoints );

            for( n=0;n<animNode.header.nNumJoints;n++ )
            {
                LPJOINT pJoint = &animNode.pJoints[n];

                // Read the joint_main.
                fread( &nLen,sizeof(size_t),1,pFile );
                pJoint->sName.resize( nLen,'\0' );
                fread( &pJoint->sName[0],nLen,1,pFile );
                fread( &nLen,sizeof(size_t),1,pFile );
                pJoint->sParentName.resize( nLen,'\0' );
                fread( &pJoint->sParentName[0],nLen,1,pFile );
                fread( &pJoint->wNumKF_Position,sizeof(WORD),1,pFile );
                fread( &pJoint->wNumKF_Rotation,sizeof(WORD),1,pFile );

                // Read the keyframe position data.
                pJoint->pKF_Position = new sKeyframe_Pos[pJoint->wNumKF_Position];
                if( !pJoint->pKF_Position )
                {
                    fclose( pFile );
                    return FLY_OUTOFMEMORY;
                }
                fread( pJoint->pKF_Position,sizeof(sKeyframe_Pos),pJoint->wNumKF_Position,pFile );

                // Read the keyframe rotation data.
                pJoint->pKF_Rotation = new sKeyframe_Rot[pJoint->wNumKF_Rotation];
                if( !pJoint->pKF_Rotation )
                {
                    fclose( pFile );
                    return FLY_OUTOFMEMORY;
                }
                fread( pJoint->pKF_Rotation,sizeof(sKeyframe_Rot),pJoint->wNumKF_Rotation,pFile );
            }

            bLoaded[1] = true;
            break;
        case CHK_END:
            bEnd = true;
            break;

        // Unknown file chunk.
        default: fseek( pFile,chunk.nChunkSize,SEEK_CUR );
        }

        if( bEnd ) break;
    }

    // Close the animation file.
    fclose( pFile );

    // 输出骨骼数据
    pFile = fopen( "skeleton.txt","w" );
    for( UINT i=0;i<animNode.header.nNumJoints;i++ )
    {
        fprintf( pFile,"%s            %s\n",animNode.pJoints[i].sName.c_str(),
            animNode.pJoints[i].sParentName.c_str() );
    }
    fclose( pFile );

    // Setup the skeleton system.
    SetupAnimation( &animNode );

    // Add the animation node to the list.
    m_AnimGroup.push_back( animNode );

    // Update the dirty state.
    m_nDirtyState++;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Prepare the joints data for the animation.
//------------------------------------------------------------------------
HRESULT FlyAnimation::SetupAnimation( sAnimNode* pNode )
{
    UINT nParentID;
    FlyMatrix mX,mY,mZ;

    for( UINT i=0;i<pNode->header.nNumJoints;i++ )
    {
        LPJOINT pJoint = &pNode->pJoints[i];

        pJoint->qRotation.GetMatrix( &pJoint->mMatrix_Relative );
        pJoint->mMatrix_Relative._41 = pJoint->vPosition.x;
        pJoint->mMatrix_Relative._42 = pJoint->vPosition.y;
        pJoint->mMatrix_Relative._43 = pJoint->vPosition.z;

        // Find the parent bone.
        for( UINT j=0;j<pNode->header.nNumJoints;j++ )
        {
            nParentID = NULL_BONEID;

            if( pJoint->sParentName == pNode->pJoints[j].sName )
            {
                nParentID = j;
                break;
            }
        }

        pJoint->wParentID = nParentID;

        if( nParentID != NULL_BONEID )
        {
            pJoint->mMatrix_Absolute = pNode->pJoints[nParentID].mMatrix_Absolute *
                pJoint->mMatrix_Relative;
        }
        else
        {
            pJoint->mMatrix_Absolute = pJoint->mMatrix_Relative;
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Build the animation renderable units to render.
//------------------------------------------------------------------------
HRESULT FlyAnimation::BuildMaterials(void)
{
    std::string sMatName;

    // Get the resource Manager.
    FlyResourceManager* pResMgr = FlyKernel::Instance().GetResourceManager();

    // Build the material datas.
    for( UINT i=0;i<m_sHeader.nNumMaterials;i++ )
    {
        FLYMATERIAL mat;
        FlyTexture* pTexture;

        // Create the render material.
        char cTemp[20];
        sprintf( cTemp,"_%d",i );
        sMatName = "anim_" + m_sName + cTemp;
        FlyRenderMaterial* pMaterial = pResMgr->CreateMaterial( sMatName.c_str() );
        if( !pMaterial ) return FLY_OUTOFMEMORY;

        memcpy( &mat,&m_pMaterials[i],sizeof(FLYMATERIAL) );
        pMaterial->SetStdMaterial( 0,&mat );

        // Load the texture A.
        if( m_pMaterials[i].sTexture_A != "" )
        {
            pTexture = pResMgr->Load2DTexture( m_pMaterials[i].sTexture_A.c_str(),0 );
            if( pTexture )
            {
                pMaterial->SetTexture( 0,0,pTexture );

                if( pTexture->HasAlphaChannel() )
                {
                    pMaterial->SetSceneAlphaMode( 0,true,false );
                    pMaterial->SetAlphaBlendMode( 0,SBM_SRCALPHA,SBM_INVSRCALPHA );
                }
            }
        }

        // Load the texture B.
        if( m_pMaterials[i].sTexture_B != "" )
        {
            pTexture = pResMgr->Load2DTexture( m_pMaterials[i].sTexture_B.c_str(),0 );
            if( pTexture ) pMaterial->SetTexture( 0,1,pTexture );
        }

        // Add the material to list.
        m_RMaterials.push_back( pMaterial );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the number of the indices.
//------------------------------------------------------------------------
UINT FlyAnimation::GetNumIndis(void) const
{
    return m_sHeader.nNumFaces*3;
}


//------------------------------------------------------------------------
// Get the number of the vertices.
//------------------------------------------------------------------------
UINT FlyAnimation::GetNumVerts(void) const
{
    return m_sHeader.nNumVerts;
}


//------------------------------------------------------------------------
// Get the count of the faces.
//------------------------------------------------------------------------
UINT FlyAnimation::GetNumFaces(void) const
{
    return m_sHeader.nNumFaces;
}


//------------------------------------------------------------------------
// Get the count of the materials.
//------------------------------------------------------------------------
UINT FlyAnimation::GetNumMaterials(void) const
{
    return m_sHeader.nNumMaterials;
}


//------------------------------------------------------------------------
// Get the count of the meshes.
//------------------------------------------------------------------------
UINT FlyAnimation::GetNumMeshes(void) const
{
    return m_sHeader.nNumMeshes;
}


//------------------------------------------------------------------------
// Get the number of frames of the animNode.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:       Index of the animation.
//------------------------------------------------------------------------
UINT FlyAnimation::GetNumAnimFrames( int nAnimID ) const
{
    if( nAnimID < 0 || nAnimID >= (int)m_AnimGroup.size() )
        return 0;

    return m_AnimGroup[nAnimID].header.nNumFrames;
}


//------------------------------------------------------------------------
// Get the number of joints of the animNode.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:       Index of the animation.
//------------------------------------------------------------------------
UINT FlyAnimation::GetNumAnimJoints( int nAnimID ) const
{
    if( nAnimID < 0 || nAnimID >= (int)m_AnimGroup.size() )
        return 0;

    return m_AnimGroup[nAnimID].header.nNumJoints;
}


//------------------------------------------------------------------------
// Get the count of animation nodes.
//------------------------------------------------------------------------
UINT FlyAnimation::GetNumAnimations(void) const
{
    return (UINT)m_AnimGroup.size();
}


//------------------------------------------------------------------------
// Get the current boundingBox.
//------------------------------------------------------------------------
const FlyAabb& FlyAnimation::GetBoundingBox(void) const
{
    return m_BBox;
}


//------------------------------------------------------------------------
// Get the name of the animation.
//------------------------------------------------------------------------
const char* FlyAnimation::GetName(void) const
{
    return m_sName.c_str();
}


//------------------------------------------------------------------------
// Set the name for the animation.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the animation.
//------------------------------------------------------------------------
void FlyAnimation::SetName( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Get the current animated vertex data.
//------------------------------------------------------------------------
const AVERTEX* FlyAnimation::GetVertexData(void) const
{
    return m_pVertices;
}


//------------------------------------------------------------------------
// Get the specified mesh data.
//------------------------------------------------------------------------
const sMesh* FlyAnimation::GetMeshData( UINT nIndex ) const
{
    if( nIndex >= m_sHeader.nNumMeshes )
        return NULL;

    return &m_pMeshes[nIndex];
}


//------------------------------------------------------------------------
// Get the face data of the animation.
//------------------------------------------------------------------------
const sFace* FlyAnimation::GetFaceData(void) const
{
    return m_pFaces;
}


//------------------------------------------------------------------------
// Get the specified render material.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:        Index of the render material.
//------------------------------------------------------------------------
FlyRenderMaterial* FlyAnimation::GetRenderMaterial( UINT nIndex )
{
    if( nIndex >= m_sHeader.nNumMaterials )
        return NULL;

    return m_RMaterials[nIndex];
}


//------------------------------------------------------------------------
// Start to animate the vertices.
// ----------------------------------------------------------------------
// Param -> IN:
//      UINT:       ID of the animation.
//      float:      Current frame value.
//------------------------------------------------------------------------
HRESULT FlyAnimation::GoAnimation( int nAnimID,float fFrame )
{
    // Set the current animation and frame value.
    m_nAnimID = nAnimID;
    m_fAnimFrame = fFrame;

    // Start to animate.
    AnimationPrepare();
    AnimationVertices();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Prepare the transform matrix for each bones.
//------------------------------------------------------------------------
HRESULT FlyAnimation::AnimationPrepare(void)
{
    sAnimNode*      pCurNode;
    LPJOINT         pJoint;
    UINT            nNumKeyPos;
    UINT            nNumKeyRot;
    LPKEYFRAME_ROT  pLastRot;
    LPKEYFRAME_ROT  pThisRot;
    LPKEYFRAME_POS  pLastPos;
    LPKEYFRAME_POS  pThisPos;
    float           fScale;
    FlyVector       vPosition;
    FlyQuat         qRotation;

    // Get the current FlyAnimation node.
    pCurNode = &m_AnimGroup[m_nAnimID];

    for( UINT i=0;i<pCurNode->header.nNumJoints;i++ )
    {
        pJoint = &pCurNode->pJoints[i];
        nNumKeyPos = pJoint->wNumKF_Position;
        nNumKeyRot = pJoint->wNumKF_Rotation;

        // Read the keyframe position
        pLastPos = NULL;
        pThisPos = NULL;

        if( m_fAnimFrame <= 0.0f )
            pThisPos = &pJoint->pKF_Position[0];
        else if( m_fAnimFrame >= (float)pCurNode->header.nNumFrames-1 )
            pThisPos = &pJoint->pKF_Position[(int)m_fAnimFrame];
        else
        {
            pLastPos = &pJoint->pKF_Position[(int)m_fAnimFrame];
            pThisPos = &pJoint->pKF_Position[(int)m_fAnimFrame+1];
        }

        // Interpolate the two positions
        if( pThisPos && pLastPos )
        {
            fScale = ( m_fAnimFrame - pLastPos->fTime ) / ( pThisPos->fTime - pLastPos->fTime );

            // Interpolation
            vPosition = pLastPos->vPosition + (pThisPos->vPosition - pLastPos->vPosition) * fScale;
        }
        else
        {
            vPosition = pThisPos->vPosition;
        }

        // Read the keyframe rotation
        pLastRot = NULL;
        pThisRot = NULL;

        if( m_fAnimFrame < 0.0f )
            pThisRot = &pJoint->pKF_Rotation[0];
        else if( m_fAnimFrame >= (float)pCurNode->header.nNumFrames-1 )
            pThisRot = &pJoint->pKF_Rotation[(int)m_fAnimFrame];
        else
        {
            pLastRot = &pJoint->pKF_Rotation[(int)m_fAnimFrame];
            pThisRot = &pJoint->pKF_Rotation[(int)m_fAnimFrame+1];
        }

        // Interpolate the two rotations
        if( pThisRot && pLastRot )
        {
            fScale = ( m_fAnimFrame - pLastRot->fTime ) / ( pThisRot->fTime - pLastRot->fTime );

            // Interpolation
            qRotation = qRotation.Slerp( fScale,pLastRot->qRotation,pThisRot->qRotation );
        }
        else
        {
            qRotation = pThisRot->qRotation;
        }

        // Calculate the matrix for joints.
        qRotation.GetMatrix( &pJoint->mMatrix_Relative );
        pJoint->mMatrix_Relative._41 = vPosition.x;
        pJoint->mMatrix_Relative._42 = vPosition.y;
        pJoint->mMatrix_Relative._43 = vPosition.z;

        if( pJoint->wParentID != NULL_BONEID )
        {
            pJoint->mMatrix_Absolute = pJoint->mMatrix_Relative *
                pCurNode->pJoints[pJoint->wParentID].mMatrix_Absolute;
        }
        else
        {
            pJoint->mMatrix_Absolute = pJoint->mMatrix_Relative;
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Transform the vertices with the bone's matrix.
//------------------------------------------------------------------------
HRESULT FlyAnimation::AnimationVertices(void)
{
    bool bChanged = false;
    AVERTEX* pVertex;
    sVertexOrig* pVertex_Orig;
    FlyMatrix matA,matB;
    FlyVector sVectorA,sVectorB;
    FlyVector vFinal;

    // Get the current animation node.
    sAnimNode* pCurNode = &m_AnimGroup[m_nAnimID];

    FlyVector vBoxMin = FlyVector(  999999.0f, 999999.0f, 999999.0f );
    FlyVector vBoxMax = FlyVector( -999999.0f,-999999.0f,-999999.0f );

    for( UINT i=0;i<m_sHeader.nNumVerts;i++ )
    {
        pVertex = &m_pVertices[i];
        pVertex_Orig = &m_pVertices_Orig[i];

        vFinal.Set( 0.0f,0.0f,0.0f );

        // The first bone's effect.
        if( pVertex->fBone1 != (float)NULL_BONEID )
        {
            matA = pCurNode->pJoints[(UINT)pVertex->fBone1].mMatrix_Absolute;
            sVectorA.x = pVertex_Orig->fXYZ[0];
            sVectorA.y = pVertex_Orig->fXYZ[1];
            sVectorA.z = pVertex_Orig->fXYZ[2];
            sVectorA.RotateWith( matA );
            sVectorA += matA.GetTranslation();
            bChanged = true;
        }

        // The second bone's effect.
        if( pVertex->fBone2 != (float)NULL_BONEID )
        {
            matB = pCurNode->pJoints[(UINT)pVertex->fBone2].mMatrix_Absolute;
            sVectorB.x = pVertex_Orig->fXYZ[0];
            sVectorB.y = pVertex_Orig->fXYZ[1];
            sVectorB.z = pVertex_Orig->fXYZ[2];
            sVectorB.RotateWith( matB );
            sVectorB += matB.GetTranslation();
            bChanged = true;
        }

        // Blend the final position.
        vFinal = sVectorA*pVertex->fWeight1 + sVectorB*pVertex->fWeight2;

        if( bChanged )
        {
            pVertex->x = vFinal.x;
            pVertex->y = vFinal.y;
            pVertex->z = vFinal.z;
        }

//         pVertex->x = pVertex_Orig->fXYZ[0];
//         pVertex->y = pVertex_Orig->fXYZ[1];
//         pVertex->z = pVertex_Orig->fXYZ[2];

        // Calculate the boundingBox.
        if( pVertex->x < vBoxMin.x ) vBoxMin.x = pVertex->x;
        if( pVertex->y < vBoxMin.y ) vBoxMin.y = pVertex->y;
        if( pVertex->z < vBoxMin.z ) vBoxMin.z = pVertex->z;
        if( pVertex->x > vBoxMax.x ) vBoxMax.x = pVertex->x;
        if( pVertex->y > vBoxMax.y ) vBoxMax.y = pVertex->y;
        if( pVertex->z > vBoxMax.z ) vBoxMax.z = pVertex->z;

        bChanged = false;
    }

    m_BBox.vcMin = vBoxMin;
    m_BBox.vcMax = vBoxMax;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Get the current dirty state.
//------------------------------------------------------------------------
int FlyAnimation::GetDirtyState(void) const
{
    return m_nDirtyState;
}