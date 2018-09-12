//------------------------------------------------------------------------
// Name: SMDExporter.h
// Desc: SMD file exporter class Implementing
//       This module contains the main export functions
//       Write By Yish, October 9, 2008
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#include "SMDExporter.h"


/*************************************************************************
*                                                                        *
*          Global output [ Scene info ]                                  *
*                                                                        *
*************************************************************************/


// Check whether two Vertex equal
bool VertexEqual( SMDVERTEX* pVert1,SMDVERTEX* pVert2 )
{
    if( pVert1->x != pVert2->x ) return false;
    if( pVert1->y != pVert2->y ) return false;
    if( pVert1->z != pVert2->z ) return false;
    if( pVert1->vN[0] != pVert2->vN[0] ) return false;
    if( pVert1->vN[1] != pVert2->vN[1] ) return false;
    if( pVert1->vN[2] != pVert2->vN[2] ) return false;
    if( pVert1->tu != pVert2->tu ) return false;
    if( pVert1->tv != pVert2->tv ) return false;

    return true;
}


//------------------------------------------------------------------------
// Export the geometry datas
//------------------------------------------------------------------------
void SMDExporter::ExportGeomObject( INode* node )
{
	ObjectState os = node->EvalWorldState( 0 );
	if( !os.obj ) return;

	// Targets are actually geomobjects, but we will export them
	// from the camera and lights, so we skip them here.
	if( os.obj->ClassID() == Class_ID(TARGET_CLASS_ID,0) )
		return;

	GenerateMeshData( node );
    if( m_bOptimized ) OptimizeMeshData( &m_pMeshes[m_nCurNode] );

    ExportBinaryData();
}


//------------------------------------------------------------------------
// Generate the mesh data
//------------------------------------------------------------------------
HRESULT SMDExporter::GenerateMeshData( INode* node )
{
	int				c;				// Counter
	BOOL			needDel;
	UINT			nNumTex;
	VERTEXDATA*		pVertices;		// To store the vertex data
	FACEDATA*		pFaces;			// To store the face data
	TEXTUREDATA*	pTexCoords;		// To store the texcoords data
	TEXFACEDATA*	pTexFaces;		// To store the texface data

	Matrix3			tm = node->GetNodeTM( 0 );

	// If not safety return the function
	ObjectState os = node->EvalWorldState( 0 );
	if( !os.obj || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID )
		return E_FAIL;

	TriObject* tri = GetTriObjectFromNode( node,0,needDel );
	if( !tri ) return E_FAIL;

	// Get the mesh pointer
	Mesh* mesh = &tri->GetMesh();

	// Build the mesh's normal vectors
	mesh->buildNormals();

	// Write the mesh data
	m_pMeshes[m_nCurNode].nMaterialID = mtlList.GetMtlID( node->GetMtl() );
	m_pMeshes[m_nCurNode].nNumVertices = mesh->getNumFaces()*3;
	m_pMeshes[m_nCurNode].nNumFaces = mesh->getNumFaces();

	// To store the MAX Vertices data
	pVertices = new VERTEXDATA[mesh->getNumVerts()];
	if( !pVertices ) return E_FAIL;

	for( c=0;c<mesh->getNumVerts();c++ )
	{
		Point3 v = tm * mesh->verts[c];
		pVertices[c].x = v.x;
		pVertices[c].y = v.z;
		pVertices[c].z = v.y;
	}

	// To store the MAX face data
	pFaces = new FACEDATA[mesh->getNumFaces()];
	if( !pFaces ) return E_FAIL;

	for( c=0;c<mesh->getNumFaces();c++ )
	{
		pFaces[c].v1 = mesh->faces[c].v[0];
		pFaces[c].v2 = mesh->faces[c].v[2];
		pFaces[c].v3 = mesh->faces[c].v[1];
	}

	nNumTex = mesh->getNumTVerts();

	// To store the MAX TexCoords data
	pTexCoords = new TEXTUREDATA[mesh->getNumTVerts()];
	if( !pTexCoords ) return E_FAIL;
	memset( pTexCoords,0,sizeof(TEXTUREDATA)*mesh->getNumTVerts() );

	for( c=0;c<mesh->getNumTVerts();c++ )
	{
		if( nNumTex )
		{
			pTexCoords[c].tu = mesh->tVerts[c].x;
			pTexCoords[c].tv = mesh->tVerts[c].y;
		}
	}

	// To store the MAX TexFace data
	pTexFaces = new TEXFACEDATA[mesh->getNumFaces()];
	if( !pTexFaces ) return E_FAIL;
	memset( pTexFaces,0,sizeof(TEXFACEDATA)*mesh->getNumFaces() );

	for( c=0;c<mesh->getNumFaces();c++ )
	{
		if( nNumTex )
		{
			pTexFaces[c].t1 = mesh->tvFace[c].t[0];
			pTexFaces[c].t2 = mesh->tvFace[c].t[2];
			pTexFaces[c].t3 = mesh->tvFace[c].t[1];
		}
	}

	// rewrite the data
	m_pMeshes[m_nCurNode].pVertices = new SMDVERTEX[mesh->getNumFaces()*3];
	m_pMeshes[m_nCurNode].pIndices  = new WORD[mesh->getNumFaces()*3];	
	if( !m_pMeshes[m_nCurNode].pVertices ) return E_FAIL;
	if( !m_pMeshes[m_nCurNode].pIndices )  return E_FAIL;

	for( c=0;c<mesh->getNumFaces();c++ )
	{
		// Write the positions
		memcpy( &m_pMeshes[m_nCurNode].pVertices[c*3+0],&pVertices[pFaces[c].v1],sizeof(float)*3 );
		memcpy( &m_pMeshes[m_nCurNode].pVertices[c*3+1],&pVertices[pFaces[c].v2],sizeof(float)*3 );
		memcpy( &m_pMeshes[m_nCurNode].pVertices[c*3+2],&pVertices[pFaces[c].v3],sizeof(float)*3 );

		int vert;
		Face* f = &mesh->faces[c];
		Point3 fn = mesh->getFaceNormal(c);
		Point3 vn;

		vert = f->getVert(0);
		vn = GetVertexNormal( mesh,c,mesh->getRVertPtr(vert) );
		m_pMeshes[m_nCurNode].pVertices[c*3+0].vN[0] = vn.x;
		m_pMeshes[m_nCurNode].pVertices[c*3+0].vN[1] = vn.z;
		m_pMeshes[m_nCurNode].pVertices[c*3+0].vN[2] = vn.y;

		vert = f->getVert(2);
		vn = GetVertexNormal( mesh,c,mesh->getRVertPtr(vert) );
		m_pMeshes[m_nCurNode].pVertices[c*3+1].vN[0] = vn.x;
		m_pMeshes[m_nCurNode].pVertices[c*3+1].vN[1] = vn.z;
		m_pMeshes[m_nCurNode].pVertices[c*3+1].vN[2] = vn.y;

		vert = f->getVert(1);
		vn = GetVertexNormal( mesh,c,mesh->getRVertPtr(vert) );
		m_pMeshes[m_nCurNode].pVertices[c*3+2].vN[0] = vn.x;
		m_pMeshes[m_nCurNode].pVertices[c*3+2].vN[1] = vn.z;
		m_pMeshes[m_nCurNode].pVertices[c*3+2].vN[2] = vn.y;

		if( nNumTex )
		{
			// Write the TexCoords
			m_pMeshes[m_nCurNode].pVertices[c*3+0].tu = pTexCoords[pTexFaces[c].t1].tu;
			m_pMeshes[m_nCurNode].pVertices[c*3+0].tv = -pTexCoords[pTexFaces[c].t1].tv;
			m_pMeshes[m_nCurNode].pVertices[c*3+1].tu = pTexCoords[pTexFaces[c].t2].tu;
			m_pMeshes[m_nCurNode].pVertices[c*3+1].tv = -pTexCoords[pTexFaces[c].t2].tv;
			m_pMeshes[m_nCurNode].pVertices[c*3+2].tu = pTexCoords[pTexFaces[c].t3].tu;
			m_pMeshes[m_nCurNode].pVertices[c*3+2].tv = -pTexCoords[pTexFaces[c].t3].tv;
		}
		else
		{
			// Write the TexCoords
			m_pMeshes[m_nCurNode].pVertices[c*3+0].tu = 0.0f;
			m_pMeshes[m_nCurNode].pVertices[c*3+0].tv = 0.0f;
			m_pMeshes[m_nCurNode].pVertices[c*3+1].tu = 0.0f;
			m_pMeshes[m_nCurNode].pVertices[c*3+1].tv = 0.0f;
			m_pMeshes[m_nCurNode].pVertices[c*3+2].tu = 0.0f;
			m_pMeshes[m_nCurNode].pVertices[c*3+2].tv = 0.0f;
		}

		// Write the index
		m_pMeshes[m_nCurNode].pIndices[c*3+0] = c*3+0;
		m_pMeshes[m_nCurNode].pIndices[c*3+1] = c*3+1;
		m_pMeshes[m_nCurNode].pIndices[c*3+2] = c*3+2;
	}

	delete[] pVertices;
	delete[] pTexCoords;
	delete[] pFaces;
	delete[] pTexFaces;

	return S_OK;
}


//------------------------------------------------------------------------
// Optimize the mesh datas
//------------------------------------------------------------------------
HRESULT SMDExporter::OptimizeMeshData( LPSMDMESH pMesh )
{
    UINT    i,j;
    bool    bExisted = false;
    UINT    nNumVerts = 0;
    SMDVERTEX* pVerts = new SMDVERTEX[pMesh->nNumVertices];
    SMDVERTEX* vert;

    // Optimize the vertices datas
    for( i=0;i<pMesh->nNumVertices;i++ )
    {
        for( j=0;j<nNumVerts;j++ )
        {
            if( VertexEqual(&pMesh->pVertices[i],&pVerts[j]) )
            {
                bExisted = true;
                break;
            }
        }

        if( bExisted )
        {
            bExisted = false;
            continue;
        }

        // Add the unExisted vertex.
        memcpy( &pVerts[nNumVerts],&pMesh->pVertices[i],sizeof(SMDVERTEX) );
        nNumVerts++;
        bExisted = false;
    }

    // Re-sort the indices data
    for( i=0;i<pMesh->nNumFaces;i++ )
    {
        vert = &pMesh->pVertices[pMesh->pIndices[i*3+0]];
        for( j=0;j<nNumVerts;j++ )
        {
            if( VertexEqual(vert,&pVerts[j]) ) break;
        }
        pMesh->pIndices[i*3+0] = j;

        vert = &pMesh->pVertices[pMesh->pIndices[i*3+1]];
        for( j=0;j<nNumVerts;j++ )
        {
            if( VertexEqual(vert,&pVerts[j]) ) break;
        }
        pMesh->pIndices[i*3+1] = j;

        vert = &pMesh->pVertices[pMesh->pIndices[i*3+2]];
        for( j=0;j<nNumVerts;j++ )
        {
            if( VertexEqual(vert,&pVerts[j]) ) break;
        }
        pMesh->pIndices[i*3+2] = j;
    }

    // Clean the old vertices data
    delete[] pMesh->pVertices;

    // Reset the vertices data
    pMesh->nNumVertices = nNumVerts;
    pMesh->pVertices = pVerts;

    return S_OK;
}


//------------------------------------------------------------------------
// Export the binary header
//------------------------------------------------------------------------
void SMDExporter::ExportBinaryHeader(void)
{
	m_Header.dwVersion = SMD_VERSION;
	m_Header.nNumMeshes = m_nTotalNodeCount;
	m_Header.nNumMaterials = mtlList.Count();

	fwrite( &m_Header,sizeof(SMDHEADER),1,m_pFile );

	// Export the skin list
	m_pMaterials = new SMDMATERIAL[m_Header.nNumMaterials];
    memset( m_pMaterials,0,sizeof(SMDMATERIAL)*m_Header.nNumMaterials );

	for( int c=0;c<m_Header.nNumMaterials;c++ )
	{
		Mtl* mtl = mtlList.GetMtl(c);
		StdMat*	std = (StdMat*)mtl;

		memcpy( &m_pMaterials[c].cAmbient,&std->GetAmbient(0),sizeof(float)*3 );
		m_pMaterials[c].cAmbient.fA = 1.0f;

		memcpy( &m_pMaterials[c].cDiffuse,&std->GetDiffuse(0),sizeof(float)*3 );
		m_pMaterials[c].cDiffuse.fA = 1.0f;

		memcpy( &m_pMaterials[c].cSpecular,&std->GetSpecular(0),sizeof(float)*3 );
		m_pMaterials[c].cSpecular.fA = 1.0f;

		memset( &m_pMaterials[c].cEmissive,0,sizeof(float)*4 );
		m_pMaterials[c].cEmissive.fA = 1.0f;
		m_pMaterials[c].fPower = 1.0f;

		// Export the texture of material
        // Which contain only diffuse map
		Texmap* subtex = mtl->GetSubTexmap(1);

		if( subtex )
		{
			if( subtex->ClassID() == Class_ID(BMTEX_CLASS_ID,0x00) )
			{
				TSTR mapName = ((BitmapTex*)subtex)->GetMapName();

				// Copy texture file to model folder
				TCHAR newTexture[256];
                strcpy( newTexture,GetCurrentDir(m_sFilename.data()) );
                strcat( newTexture,m_sTexFolder.data() );
                CreateDirectory( newTexture,NULL );
                strcat( newTexture,"\\" );
                strcat( newTexture,GetCurrentName(mapName) );
				CopyFile( mapName,newTexture,FALSE );

                if( strcmp(m_sTexFolder.data(),"") == 0 ||
                    strcmp(m_sTexFolder.data(),".") == 0 )
                {
                    m_pMaterials[c].sTexture = GetCurrentName( mapName.data() );
                }
                else
                {
				    m_pMaterials[c].sTexture = m_sTexFolder;
                    m_pMaterials[c].sTexture += "\\";
                    m_pMaterials[c].sTexture += GetCurrentName( mapName.data() );
                }
			}
		}

        fwrite( &m_pMaterials[c].cAmbient,sizeof(SMDCOLOR),1,m_pFile );
        fwrite( &m_pMaterials[c].cDiffuse,sizeof(SMDCOLOR),1,m_pFile );
        fwrite( &m_pMaterials[c].cEmissive,sizeof(SMDCOLOR),1,m_pFile );
        fwrite( &m_pMaterials[c].cSpecular,sizeof(SMDCOLOR),1,m_pFile );
        fwrite( &m_pMaterials[c].fPower,sizeof(float),1,m_pFile );

        int nLen = m_pMaterials[c].sTexture.Length();
        fwrite( &nLen,sizeof(int),1,m_pFile );
        fwrite( m_pMaterials[c].sTexture.data(),nLen,1,m_pFile );
	}
}


//------------------------------------------------------------------------
// Export the binary format SMD File
//------------------------------------------------------------------------
void SMDExporter::ExportBinaryData(void)
{
    int c = m_nCurNode;

	// Export the MESH datas
    fwrite( &m_pMeshes[c].nMaterialID,sizeof(UINT),1,m_pFile );
    fwrite( &m_pMeshes[c].nNumVertices,sizeof(UINT),1,m_pFile );
    fwrite( &m_pMeshes[c].nNumFaces,sizeof(UINT),1,m_pFile );
	fwrite( m_pMeshes[c].pVertices,sizeof(SMDVERTEX),m_pMeshes[c].nNumVertices,m_pFile );
	fwrite( m_pMeshes[c].pIndices,sizeof(WORD),m_pMeshes[c].nNumFaces*3,m_pFile );
}


#define SINGLE_QUOTE	39
#define CTL_CHARS		31

//------------------------------------------------------------------------
// Get the folder from the path
//------------------------------------------------------------------------
TCHAR* SMDExporter::GetCurrentDir( TCHAR* szPath )
{
	static char buf[256];
	
	int len = strlen(szPath);
	for( int i=len-1;i>=0;i-- )
	{
		if( szPath[i]=='\\' )
		{
			memcpy( &buf[0],szPath,sizeof(char)*(i+1) );
			buf[i+1] = '\0';
			break;
		}
	}

	return buf;
}


//------------------------------------------------------------------------
// Get the name from the path
//------------------------------------------------------------------------
TCHAR* SMDExporter::GetCurrentName( TCHAR* szPath )
{
	static char buf[256];

	int len = strlen(szPath);
	for( int i=len-1;i>=0;i-- )
	{
		if( szPath[i]=='\\' )
		{
			memcpy( &buf[0],&szPath[i+1],sizeof(char)*(len-i-1) );
			buf[len-i-1] = '\0';
			break;
		}
	}

	return buf;
}


/*************************************************************************
*                                                                        *
*    MISC FUNCTIONS OF THIS CLASS                                        *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Get triangle objects' pointer
// Return a pointer to a triobject given an INode or NULL if the node
// cann't be converted to triobject
//------------------------------------------------------------------------
TriObject* SMDExporter::GetTriObjectFromNode( INode* node,TimeValue t,int& deleteIt )
{
	deleteIt = FALSE;
	Object* obj = node->EvalWorldState(0).obj;

	if( obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID,0)) )
	{
		TriObject* tri = (TriObject*)obj->ConvertToType( 0,Class_ID(TRIOBJ_CLASS_ID,0) );

		// Note that triobject should only be deleted if the pointer is
		// not equal to the object pointer that called ConvertToType()
		if( tri != obj ) deleteIt = TRUE;

		return tri;
	}
	else
	{
		return NULL;
	}
}


// ------------------------------------------------------------------------
// Get the normal vector from a vertex
// ------------------------------------------------------------------------
Point3 SMDExporter::GetVertexNormal( Mesh* mesh,int faceNo,RVertex* rv )
{
	Point3	vertexNormal;
	int	numNormals;
	Face*	f = &mesh->faces[faceNo];
	DWORD	smGroup = f->smGroup;

	// Is normal specified ?
	if( rv->rFlags & SPECIFIED_NORMAL )
		vertexNormal = rv->rn.getNormal();

	// If normal is not specified it's only available if the
	// face belong to a smoothing group
	else if( (numNormals = rv->rFlags & NORCT_MASK) && smGroup )
	{
		// If there is only one vertex is found in rn member
		if( numNormals == 1 )
			vertexNormal = rv->rn.getNormal();
		else
		{
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for( int c=0;c<numNormals;c++ )
			{
				if( rv->ern[c].getSmGroup() & smGroup )
					vertexNormal = rv->ern[c].getNormal();
			}
		}
	}
	else
	{
		vertexNormal = mesh->getFaceNormal( faceNo );
	}

	return vertexNormal;
}


//------------------------------------------------------------------------
// Clean up the memory
//------------------------------------------------------------------------
void SMDExporter::CleanUpMem(void)
{
	if( m_pMaterials )
	{
		delete[] m_pMaterials;
		m_pMaterials = NULL;
	}

	if( m_pMeshes )
	{
		for( int i=0;i<m_nTotalNodeCount;i++ )
		{
			if( m_pMeshes[i].pVertices )
			{
				delete[] m_pMeshes[i].pVertices;
				m_pMeshes[i].pVertices = NULL;
			}

			if( m_pMeshes[i].pIndices )
			{
				delete[] m_pMeshes[i].pIndices;
				m_pMeshes[i].pIndices = NULL;
			}
		}

		delete[] m_pMeshes;
		m_pMeshes = NULL;
	}
}