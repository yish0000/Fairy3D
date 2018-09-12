//------------------------------------------------------------------------
// Name: FlyRenderEffectCompiler.h
// Desc: This file define a class to Compile the render effect file.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDEREFFECT_COMPILER_H__
#define __FLY_RENDEREFFECT_COMPILER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyRenderVariable.h"
#include "FlyXmlReader.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Class to compile the xml format render effect file.

#define ANNO_PARAMETER  1
#define ANNO_TECHNIQUE  2
#define ANNO_PASS       3

#define REST_FILE       1
#define REST_MEMORY     2

struct FX_Header
{
    char fourcc[4];
    UINT version;
    UINT num_parameters;
    UINT num_shaders;
    UINT num_techniques;
};

struct FX_Annotation
{
    REDATATYPE Type; // Type of the variable.
    std::string name; // Name of the annotation.
    FlyRenderVariable* pVar; // Pointer to the var value.
};

struct FX_Parameter
{
    REDATATYPE Type; // Type of the parameter.
    std::string name; // Name of the parameter.
    FlyRenderVariable* pVar; // Pointer to the var value.
    UINT nArraySize; // Size of the array.
    std::vector<FX_Annotation> annotations; // Annotation array.
    std::string semantic; // Semantic of the parameter.
    bool bAuto; // Auto-update ?
    int autoType; // Auto-update type.
};

struct FX_Shader
{
    std::vector<std::string> funcs;
    std::vector<std::string> shader_code;
};

struct FX_RenderState
{
    RSTYPE Type; // Type of the state value.
    void* pStateData; // pointer of the State data
    int nIndex; // Index of the state value.
};

struct FX_Pass
{
    std::string name; // Name of the render pass.
    std::vector<FX_Annotation> annotations; // Annotation array.
    FlyShaderObject::ShaderDesc shaderDescs[2]; // Shader desc array.
    std::vector<FX_RenderState> render_states; // Render state array.
};

struct FX_Technique
{
    std::string name; // Name of the technique.
    std::vector<FX_Annotation> annotations; // Annotation array.
    std::vector<FX_Pass> passes; // Render Pass array.
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyRenderEffectCompiler
{
protected:
    std::vector<FX_Parameter> m_Parameters;
    std::vector<FX_Technique> m_Techniques;
    FX_Shader m_Shader;

    // To store the error code!
    std::string m_strError;
    std::vector<std::string> m_IncludeList;

    // Add a parameter to the list.
    FX_Parameter* AddParameter( const char* cName );

    // Add a shader function to the list.
    bool AddShaderFunc( const char* cFuncName );

    // Add a technique to the list.
    FX_Technique* AddTechnique( const char* cName );

    // Add a pass to the list.
    FX_Pass* AddPassToTechnique( FX_Technique* pTechnique,const char* cName );

    // Add a annotation to the list.
    FX_Annotation* AddAnnotationToParam( FX_Parameter* param,const char* cName );
    FX_Annotation* AddAnnotationToTechnique( FX_Technique* technique,const char* cName );
    FX_Annotation* AddAnnotationToPass( FX_Pass* pass,const char* cName );

    bool HandleInclude( const char* cIncludeFile,int srcType,const char* cCurrentFile );
    bool HandleParameter( FlyXmlReader::XmlNodeAccess param );
    bool HandleShader( FlyXmlReader::XmlNodeAccess shader );
    bool HandleTechnique( FlyXmlReader::XmlNodeAccess technique );
    bool HandleSampler( FlyXmlReader::XmlNodeAccess sampler,FX_Parameter* pParam );
    bool HandleAnnotation( FlyXmlReader::XmlNodeAccess anno,int type,void* ptr );
    bool HandleRenderState( FlyXmlReader::XmlNodeAccess state,FX_Pass* pPass );

    void WriteRenderVariable( REDATATYPE Type,FlyRenderVariable* pVar,FILE* pFile );
    void WriteAnnotation( FX_Annotation* pAnnotation,FILE* pFile );
    void WriteRenderPass( FX_Pass* pPass,FILE* pFile );

    // Check whether the shader function is defined
    bool IsShaderFuncExisted( const char* cFuncName );

    // Until functions used when compiling.
    const char* GetDataType( REDATATYPE Type );
    REDATATYPE GetDataType( const char* cType );
    FILTEROPTION GetFilterOption( const char* filter );
    TEXTUREADDRESS GetAddressMode( const char* address );
    FLYCMPFUNC GetCmpFunc( const char* cFunc );
    SCENEBLENDMODE GetSceneBlendMode( const char* cBlend );
    BLENDSOURCE GetBlendSource( const char* cSource );
    TEXTUREOPERATION GetTextureOP( const char* cTOP );
    LIGHTTYPE GetLightType( const char* cType );
    FOGMODE GetFogMode( const char* cMode );
    STENCILOPERATION GetStencilOp( const char* cSTOP );

    // Report the error code.
    void ReportError( const char* cErrorInfo,FlyXmlReader::XmlNodeAccess curNode );

    // Compile the single file.
    HRESULT LoadSourceFromXML( const char* cXmlDoc );

    // Go! Start to compile the source file.
    HRESULT Go( FlyXmlReader::XmlNodeAccess root,int srcType,const char* cFilename );

public:
    // Constructor and destructor.
    FlyRenderEffectCompiler(void);
    ~FlyRenderEffectCompiler(void);

    // Reset the compiler.
    HRESULT Reset(void);

    HRESULT CompileFromFile( const char* cXmlDoc );
    HRESULT CompileFromBuffer( const void* pCodeBuffer );
    HRESULT SaveEffectToFFX( const char* cFfxFile );

    // Get the info of the effect file.
    size_t GetNumParameters(void) const;
    size_t GetNumShaderFuncs(void) const;
    size_t GetNumTechniques(void) const;
    FX_Parameter* GetParameter( size_t nIndex );
    FX_Technique* GetTechnique( size_t nIndex );

    void GenShaderText( std::string& shader_text );

    // Get the error info.
    const char* GetErrorInfo(void) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDEREFFECT_COMPILER_H__