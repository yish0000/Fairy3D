//------------------------------------------------------------------------
// Name: main.cpp
// Desc: This file define a console application to compile the xml
//       render effect data!
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifdef _DEBUG
#pragma comment(lib,"tiny_xml/TinyXML_d.lib")
#else
#pragma comment(lib,"tiny_xml/TinyXML.lib")
#endif

#include "FlyRenderEffectCompiler.h"


// Global function list.
void PrintLogo(void);
void PrintUsage(void);
bool ValidPath( const char* cPath );
void PrintResult(void);


FlyRenderEffectCompiler g_Compiler;

// Main function of the application.
int main( int argc,char* argv[] )
{
    char cXmlPath[256];
    char cBinPath[256];

    PrintLogo();

    if( argc < 2 || argc > 3 )
    {
        PrintUsage();
        return 0;
    }

    strcpy( cXmlPath,argv[1] );

    if( argc == 3 )
        strcpy( cBinPath,argv[2] );
    else
    {
        char* pPoint;
        strcpy( cBinPath,cXmlPath );
        pPoint = strchr( cBinPath,'.' );
        pPoint[1] = 'f';
        pPoint[2] = 'x';
        pPoint[3] = '\0';
    }

    // Start to compile the file.
    if( FAILED(g_Compiler.CompileFromFile(cXmlPath)) )
    {
        printf( "Error: Compile failed!\n%s\n",g_Compiler.GetErrorInfo() );
        return 0;
    }

    // Save the binary effect file.
    printf( "Compiled successfully!\n" );
    printf( "/out: %s\n",cBinPath );
    g_Compiler.SaveEffectToFFX( cBinPath );

    std::string shader;
    g_Compiler.GenShaderText( shader );
    FILE* pFile = fopen( "test.txt","w" );
    fwrite( shader.c_str(),shader.size(),1,pFile );
    fclose( pFile );

    // Print the compile result.
    PrintResult();

    return 0;
}

//////////////////////////////////////////////////////////////////////////

void PrintLogo(void)
{
    printf( "************************************************************\n" );
    printf( "*                  Effect compiler v1.0                    *\n" );
    printf( "*                        BY Yish.                          *\n" );
    printf( "************************************************************\n\n" );
}

//////////////////////////////////////////////////////////////////////////

void PrintUsage(void)
{
    printf( "Usage: Filename1, Filename2\n" );
    printf( "    - Filename1: Path to the xml source file!\n" );
    printf( "    - Filename2: Path to the binary effect file!\n\n" );
}

//////////////////////////////////////////////////////////////////////////

void PrintResult(void)
{
    printf( "Compile result:\n" );
    printf( "    - Parameter count: %d\n",g_Compiler.GetNumParameters() );
    printf( "    - Shader function count: %d\n",g_Compiler.GetNumShaderFuncs() );
    printf( "    - Technique count: %d\n",g_Compiler.GetNumTechniques() );
}