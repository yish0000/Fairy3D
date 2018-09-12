//------------------------------------------------------------------------
// Name: SMDExporter.h
// Desc: SMD file exporter class definition
//		 Write By Yish, October 9, 2008
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#ifndef __SMD_EXPORTER_H__
#define __SMD_EXPORTER_H__


//// HEADERS OF THIS PLUGIN //////////////////////////////////////////////
#include "Res\resource.h"

#include "max.h"
#include "istdplug.h"
#include "stdmat.h"
#include "decomp.h"
#include "shape.h"
#include "interpik.h"

#include "SMDStructs.h"
//////////////////////////////////////////////////////////////////////////


//// MACROS AND FUNCTIONS ////////////////////////////////////////////////
#define SMD_VERSION	    0x0100

extern ClassDesc* GetSMDExpDesc(void);
extern TCHAR* GetString( int id );
extern HINSTANCE hInstance;
//////////////////////////////////////////////////////////////////////////


/*************************************************************************
*                                                                        *
*    This is a class to store the material list                          *
*                                                                        *
*************************************************************************/
class MaterialKeeper
{
public:
	BOOL	AddMtl( Mtl* mtl );
	int		GetMtlID( Mtl* mtl );
	int		Count(void);
	Mtl*	GetMtl( int id );
    bool    ContainMtl( Mtl* mtl );

	Tab<Mtl*>	mtlTab;
};


/*************************************************************************
*                                                                        *
*  This is the main class for the SMDExporter                            *
*                                                                        *
*************************************************************************/
class SMDExporter : public SceneExport
{
protected:
	Interface*		m_ip;               // Interface of MAX
	FILE*			m_pFile;            // File pointer
	int				m_nTotalNodeCount;  // Number of total nodes
	int				m_nCurNode;         // Current nodes
    TSTR            m_sFilename;        // To store the model file path.
    TSTR            m_sTexFolder;       // Folder to store the textures.

	MaterialKeeper	mtlList;            // Material list

	// Exported file data
	SMDHEADER       m_Header;           // SMD file's header
	SMDMATERIAL*    m_pMaterials;       // Material arrays
	LONG            m_nNumMeshes;       // Number of meshes
	LPSMDMESH       m_pMeshes;          // Meshes arrays

    bool            m_bOptimized;       // Optimize the mesh data ?

public:
	/* Constructor and Destructor */
	SMDExporter(void);
	~SMDExporter(void);

	//------------------------------------------------
	// Scene export methods

	int ExtCount(void);                     // Number of extensions supported
	const TCHAR* Ext( int n );              // Extensions in #n position ("*.smd")
	const TCHAR* LongDesc(void);            // Show long exporter's description
	const TCHAR* ShortDesc(void);           // Show short exporter's description
	const TCHAR* AuthorName(void);          // Show ASCII author's name
	const TCHAR* AuthorEmail(void);         // Get our email
	const TCHAR* CopyrightMessage(void);    // Show the CopyRight message
	const TCHAR* OtherMessage1(void);       // Other message #1
	const TCHAR* OtherMessage2(void);       // Other message #2
	unsigned int Version(void);             // Show Exporter version
	void ShowAbout( HWND hWnd );            // Show about message

	BOOL SupportsOptions( int ext,DWORD options );
	int DoExport( const TCHAR* name,ExpInterface* ei,Interface* i,
                  BOOL suppressPrompts=FALSE,DWORD options=0 );

	//------------------------------------------------
	// Other export methods

	// Nodes enumeration
	BOOL nodeEnum( INode* node );
	void PreProcess( INode* node,int& nodeCount );

	// High level info export
	void ExportGeomObject( INode* node );
	void ExportBinaryData(void);
	void ExportBinaryHeader(void);

	// Middle level data generate
	HRESULT GenerateMeshData( INode* node );
	HRESULT OptimizeMeshData( LPSMDMESH pMesh );

    bool IsOptimized(void) { return m_bOptimized; }
    void SetOptimized( bool bOptimized ) { m_bOptimized = bOptimized; }
    void SetTexFolder( char* cFolder ) { m_sTexFolder = cFolder; }

	//------------------------------------------------
	// Misc methods

	TriObject* GetTriObjectFromNode( INode* node,TimeValue t,int& deleteIt );
	Point3 GetVertexNormal( Mesh* mesh,int faceNo,RVertex* rv );
	TCHAR* GetCurrentDir( TCHAR* szPath );
	TCHAR* GetCurrentName( TCHAR* szPath );
	void CleanUpMem(void);
};

//////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __SMD_EXPORTER_H__