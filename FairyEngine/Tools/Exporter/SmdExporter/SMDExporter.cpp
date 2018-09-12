//------------------------------------------------------------------------
// Name: SMDExporter.h
// Desc: SMD file exporter class Implementing
//		 Write By Yish, October 9, 2008
// ----------------------------------------------------------------------
// CopyRight (C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#include "SMDExporter.h"


//// LINK THE STATIC LIBRARY /////////////////////////////////////////////
#pragma comment(lib,"core.lib")
#pragma comment(lib,"geom.lib")
#pragma comment(lib,"mesh.lib")
#pragma comment(lib,"maxutil.lib")
#pragma comment(lib,"comctl32.lib")
//////////////////////////////////////////////////////////////////////////


//// GLOBAL VARIABLES ////////////////////////////////////////////////////
HINSTANCE	hInstance;
BOOL		controlsInit = FALSE;

static BOOL	showPrompts;
static BOOL	exportSelected;

// Our Class_ID of SMDExporter 1.0
// It is unique and randomly generated!!
#define	SMDEXPORTER_CLASS_ID	Class_ID( 0x6b1c0e06,0x37a82ad3 )
//////////////////////////////////////////////////////////////////////////


//// OUR DLL MODULE'S ENTRY FUNCTION /////////////////////////////////////
BOOL APIENTRY DllMain( HANDLE hModule,DWORD dwReason,LPVOID lpvResearved )
{
	hInstance = (HINSTANCE)hModule;

	// Initialize the custom controls 
	// and this must be done only once
	if( !controlsInit )
	{
		controlsInit = TRUE;

		InitCustomControls( hInstance );
		InitCommonControls();
	}

	return TRUE;
}


//------------------------------------------------------------------------
// Name: class SMDExpClassDesc
// Desc: 
//------------------------------------------------------------------------
class SMDExpClassDesc : public ClassDesc
{
public:
	int				IsPublic(void)					{ return 1; }
	void*			Create( BOOL loading=false )	{ return new SMDExporter(); }
	const TCHAR*	ClassName(void)					{ return GetString(IDS_SMDEXPORTER); }
	SClass_ID		SuperClassID(void)				{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID(void)					{ return SMDEXPORTER_CLASS_ID; }
	const TCHAR*	Category(void)					{ return GetString(IDS_CATEGORY); }
};

static SMDExpClassDesc SMDExpDesc;


// EXPORTER FUNCTIONS IMPLEMENTING
const TCHAR* LibDescription(void) { return GetString(IDS_LIBDESCRIPTION); }
int LibNumberClasses(void) { return 1; }
ClassDesc* LibClassDesc( int i ) { if( i == 0 ) return GetSMDExpDesc(); else return NULL; }
ULONG LibVersion(void) { return VERSION_3DSMAX; }
ULONG CanAutoDefer(void) { return 1; }
ClassDesc* GetSMDExpDesc(void) { return &SMDExpDesc; }


//------------------------------------------------------------------------
// Get the string from the string table
//------------------------------------------------------------------------
TCHAR* GetString( int n )
{
	static TCHAR buf[256];

	if( hInstance ) return LoadString( hInstance,n,buf,sizeof(buf) ) ? buf : NULL;

	return NULL;
}


/*************************************************************************
*                                                                        *
*    class SMDExporter's Implementing                                    *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
SMDExporter::SMDExporter(void)
{
	m_ip				= NULL;
	m_pFile				= NULL;
	m_nCurNode			= 0;
	m_nTotalNodeCount	= 0;
    m_nNumMeshes        = 0;

    m_bOptimized        = true;

	// Initialize the arrays
	m_pMaterials        = NULL;
	m_pMeshes           = NULL;

    m_sTexFolder        = "";
    m_sFilename         = "";
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
SMDExporter::~SMDExporter(void)
{
	/* Nothing to do! */
}


int SMDExporter::ExtCount(void) { return 1; }
const TCHAR* SMDExporter::Ext( int n ) { if( n == 0 ) return TEXT("SMD"); else return NULL; }
const TCHAR* SMDExporter::LongDesc(void) { return GetString( IDS_LONGDESC ); }
const TCHAR* SMDExporter::ShortDesc(void) { return GetString( IDS_SHORTDESC ); }
void SMDExporter::ShowAbout( HWND hWnd ) { /* Nothing to do! */ }
const TCHAR* SMDExporter::AuthorName(void) { return TEXT("Yish"); }
const TCHAR* SMDExporter::AuthorEmail(void) { return TEXT("dmfs0000@yahoo.com.cn"); }
const TCHAR* SMDExporter::CopyrightMessage(void) { return GetString( IDS_COPYRIGHT ); }
const TCHAR* SMDExporter::OtherMessage1(void) { return TEXT(""); }
const TCHAR* SMDExporter::OtherMessage2(void) { return TEXT(""); }
unsigned int SMDExporter::Version(void) { return SMD_VERSION; }


/*************************************************************************
*  Name: INT CALLBACK ExporterDlgProc()                                  *
*                                                                        *
*  Desc: Exporter Dialog's CALLBACK Function                             *
*        Control the exporter's function                                 *
*************************************************************************/
INT CALLBACK ExporterDlgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    char cTemp[256];

	// Get the pointer of the exporter
	SMDExporter* pExp = (SMDExporter*)GetWindowLongPtr( hWnd,GWLP_USERDATA );

    // Get the controls' handle
    HWND hAscFmt = GetDlgItem( hWnd,IDC_ASCFMT );
    HWND hBinFmt = GetDlgItem( hWnd,IDC_BINARYFMT );
    HWND hOptimize = GetDlgItem( hWnd,IDC_OPTIMIZE );
    HWND hTexPath = GetDlgItem( hWnd,IDC_TEXPATH );

	switch( uMsg )
	{
	case WM_INITDIALOG:

		// Initialize the dialog
		pExp = (SMDExporter*)lParam;
		SetWindowLongPtr( hWnd,GWLP_USERDATA,lParam );
		CenterWindow( hWnd,GetParent(hWnd) );
        SetWindowText( hTexPath,"." );

        // Init the controls
        if( pExp->IsOptimized() ) SendMessage( hOptimize,BM_SETCHECK,BST_CHECKED,0 );
		break;

	case WM_CLOSE:
		EndDialog( hWnd,0 );
		break;

	case WM_COMMAND:

		switch( LOWORD(wParam) )
		{
        case IDC_OPTIMIZE:
            if( SendMessage(hOptimize,BM_GETCHECK,0,0) == BST_CHECKED )
                pExp->SetOptimized( true );
            else pExp->SetOptimized( false );
            break;
		case IDOK:
            GetWindowText( hTexPath,cTemp,256 );
            pExp->SetTexFolder( cTemp );
			EndDialog( hWnd,1 );
			break;
		case IDCANCEL:
			EndDialog( hWnd,0 );
			break;
		}

		break;

	default: return FALSE;
	}

	return TRUE;
}


/*************************************************************************
*  Name: DWORD WINAPI fn()                                               *
*                                                                        *
*  Desc: Dummy function for progress bar                                 *
*************************************************************************/
DWORD WINAPI fn( LPVOID arg ) { return 0; }


//------------------------------------------------------------------------
// Name: DoExport()
// Desc: Start the exporter!
//------------------------------------------------------------------------
int SMDExporter::DoExport( const TCHAR* name,ExpInterface* ei,Interface* i,
						   BOOL suppressPrompts,DWORD options )
{
	// Set a global prompt display switch
	showPrompts = suppressPrompts ? FALSE : TRUE;
	exportSelected = (options & SCENE_EXPORT_SELECTED) ? TRUE : FALSE;

	// Store the Interface pointer
	m_ip = i;
    m_sFilename = name;

	if( showPrompts )
	{
		// If user click "Cancel" then exit the plug-in
		if( !DialogBoxParam(hInstance,"SMDExporterDlg",m_ip->GetMAXHWnd(),ExporterDlgProc,(LPARAM)this) )
			return TRUE;
	}

	// Open a file to write data
	m_pFile = fopen( name,"wb" );
	if( !m_pFile ) return FALSE;

	// Startup the progress bar.
	m_ip->ProgressStart( GetString(IDS_PROGRESS_MSG),TRUE,fn,NULL );

	// Get the total node count by traversing the scene
	// It's nice to have a accurate progress bar
	m_nTotalNodeCount   = 0;
	m_nCurNode          = 0;
	PreProcess( m_ip->GetRootNode(),m_nTotalNodeCount );

	// Call our node Enumerator.
	// The nodeEnum function will recourse itself and
	// Export each object found in the scene
	int numChildren = m_ip->GetRootNode()->NumberOfChildren();

    fputc( 'S',m_pFile );
    fputc( 'M',m_pFile );
    fputc( 'D',m_pFile );
    fputc( '\0',m_pFile );
    ExportBinaryHeader();

	// Check the count of mesh
	m_nNumMeshes = m_nTotalNodeCount;
	m_pMeshes = new SMDMESH[m_nNumMeshes];
    memset( m_pMeshes,0,sizeof(SMDMESH)*m_nNumMeshes );

	for( int c=0;c<numChildren;c++ )
	{
		if( m_ip->GetCancel() ) break;

		nodeEnum( m_ip->GetRootNode()->GetChildNode(c) );
	}

	// Exported all datas, Close the file
	m_ip->ProgressEnd();
	fclose( m_pFile );

	// Clean up the memory
	CleanUpMem();

	return TRUE;
}


//------------------------------------------------------------------------
// Supports options
//------------------------------------------------------------------------
BOOL SMDExporter::SupportsOptions( int ext,DWORD options )
{
	// We only support one extension
	assert( ext==0 );

	return options == SCENE_EXPORT_SELECTED ? TRUE : FALSE;
}


//------------------------------------------------------------------------
// Pre-process the progress bar
//------------------------------------------------------------------------
void SMDExporter::PreProcess( INode* node,int& nodeCount )
{
    Mtl* mtl = node->GetMtl();

	if( !exportSelected || node->Selected() )
	{
		// The ObjectState is a 'thing' that flows down the pipeline containing
		// all information about the object. By calling EvalWorldState() we tell
		// max to evaluate the object at the end of pipeline
		ObjectState os = node->EvalWorldState(0);

		// The obj member of ObjectState is the actual
		// object we will export
		if( os.obj )
		{
			// We look at the superclassID to determin the type of the object
			switch( os.obj->SuperClassID() )
			{
			case GEOMOBJECT_CLASS_ID:
                // Add the materials to our material list!
                // Only export the Material which have four colors
                // and the Diffuse texMap.
	            if( (StdMat*)mtl )
                {
                    mtlList.AddMtl( mtl );
                }

				nodeCount++;
				break;
			}
		}
	}

	// Traverse the child nodes
	for( int c=0;c<node->NumberOfChildren();c++ )
	{
		PreProcess( node->GetChildNode(c),nodeCount );
	}
}


//------------------------------------------------------------------------
// Name: BOOL nodeEnum()
//
// Desc: This method is the main object exporter.
//       It is called once of every node in the scene.
//       The objects are exported as they are encoutered.
//
//       Before recursing into the children of node,we will export it.
//       The benefit of this is that a nodes parent is always before
//       the children in the resulting file. This is desired since a
//       child's transformation matrix is optionally relative to the
//       parent.
//------------------------------------------------------------------------
BOOL SMDExporter::nodeEnum( INode* node )
{
    // If user press cancel then break recurse
    if( m_ip->GetCancel() )
        return FALSE;

    // Only export if exporting everything or it's selected
    if( !exportSelected || node->Selected() )
    {
        // The ObjectState is a 'thing' that flows down the pipeline containing
        // all information about the object. By calling EvalWorldState() we tell
        // max to evaluate the object at the end of pipeline
        ObjectState os = node->EvalWorldState(0);

        // The obj member of ObjectState is the actual
        // object we will export
        if( os.obj )
        {
            // We look at the superclassID to determin the type of the object
            switch( os.obj->SuperClassID() )
            {
            case GEOMOBJECT_CLASS_ID:
                ExportGeomObject( node );
                break;
            }
        }
    }

    // Update the processBar
    m_nCurNode++;
    m_ip->ProgressUpdate( int((float)m_nCurNode/m_nTotalNodeCount * 100.0f) );

    // For each child of this node, we recurse into ourselves
    // until no more children is found
    for( int c=0;c<node->NumberOfChildren();c++ )
    {
        if( !nodeEnum(node->GetChildNode(c)) )
            return FALSE;
    }

    return TRUE;
}


/*************************************************************************
*    Name: class MaterialKeeper                                          *
*                                                                        *
*    Desc: Class to store a list of material                             *
*************************************************************************/


//------------------------------------------------------------------------
// Add material to this list
//------------------------------------------------------------------------
BOOL MaterialKeeper::AddMtl( Mtl* mtl )
{
	if( !mtl ) return FALSE;

	int numMtls = mtlTab.Count();
	for( int i=0;i<numMtls;i++ )
	{
		if( mtl==mtlTab[i] ) return FALSE;
	}

	mtlTab.Append( 1,&mtl,25 );
	return TRUE;
}


//------------------------------------------------------------------------
// Get material's ID
//------------------------------------------------------------------------
int MaterialKeeper::GetMtlID( Mtl* mtl )
{
	int numMtls = mtlTab.Count();

	for( int i=0;i<numMtls;i++ )
	{
		if( mtl==mtlTab[i] ) return i;
	}

	return -1;
}


//------------------------------------------------------------------------
// Get the list's count
//------------------------------------------------------------------------
int MaterialKeeper::Count(void)
{
	return mtlTab.Count();
}


//------------------------------------------------------------------------
// Get a material
//------------------------------------------------------------------------
Mtl* MaterialKeeper::GetMtl( int id )
{
	return mtlTab[id];
}


// Check whether the material exists in the list
bool MaterialKeeper::ContainMtl( Mtl* mtl )
{
    for( int i=0;i<mtlTab.Count();i++ )
    {
        if( mtl == mtlTab[i] ) return true;
    }

    return false;
}