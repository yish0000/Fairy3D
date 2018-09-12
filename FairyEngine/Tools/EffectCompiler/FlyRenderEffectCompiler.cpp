//------------------------------------------------------------------------
// Name: FlyRenderEffectCompiler.cpp
// Desc: This file is to implement the class FlyRenderEffectCompiler.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyRenderEffectCompiler.h"

#include "FlyStringUtil.h"
#include "FlyRenderAutoParameter.h"


//////////////////////////////////////////////////////////////////////////
// Constructor and destructor for structures.

//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyRenderEffectCompiler::FlyRenderEffectCompiler(void)
{
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyRenderEffectCompiler::~FlyRenderEffectCompiler(void)
{
    for( int i=0;i<m_Parameters.size();i++ )
    {
        FX_Parameter* param = &m_Parameters[i];
        for( int j=0;j<param->annotations.size();j++ )
        {
            SAFE_DELETE( param->annotations[j].pVar );
        }

        SAFE_DELETE( m_Parameters[i].pVar );
    }

    for( i=0;i<m_Techniques.size();i++ )
    {
        FX_Technique* tech = &m_Techniques[i];

        // Delete the annotation variables.
        for( int j=0;j<tech->annotations.size();j++ )
        {
            SAFE_DELETE( tech->annotations[j].pVar );
        }

        // Delete the render states.
        for( j=0;j<tech->passes.size();j++ )
        {
            FX_Pass* pass = &tech->passes[j];

            for( int r=0;r<pass->annotations.size();r++ )
            {
                SAFE_DELETE( pass->annotations[j].pVar );
            }

            for( r=0;r<pass->render_states.size();r++ )
            {
                FX_RenderState* rs = &pass->render_states[r];

                switch( rs->Type )
                {
                case RS_CULLMODE:
                    {
                        CULLMODE* p = (CULLMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_POLYGONMODE:
                    {
                        POLYGONMODE* p = (POLYGONMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_SHADEMODE:
                    {
                        SHADEMODE* p = (SHADEMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_DEPTHMODE:
                    {
                        DEPTHMODE* p = (DEPTHMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_DEPTHFUNC:
                case RS_ALPHATESTFUNC:
                case RS_STENCILFUNC:
                case RS_BACKSTENCILFUNC:
                    {
                        FLYCMPFUNC* p = (FLYCMPFUNC*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_SLOPESCALE:
                case RS_DEPTHBIAS:
                    {
                        float* p = (float*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_ALPHABLENDENABLE:
                case RS_ALPHATESTENABLE:
                case RS_LIGHTING:
                case RS_SPECULARLIGHT:
                case RS_FRONTSTENCILENABLE:
                case RS_BACKSTENCILENABLE:
                    {
                        bool* p = (bool*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_STENCILFAIL:
                case RS_STENCILDEPTHFAIL:
                case RS_STENCILPASS:
                case RS_BACKSTENCILFAIL:
                case RS_BACKSTENCILDEPTHFAIL:
                case RS_BACKSTENCILPASS:
                    {
                        STENCILOPERATION* p = (STENCILOPERATION*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_ALPHATESTREF:
                case RS_STENCILMASK:
                case RS_STENCILWRITEMASK:
                    {
                        DWORD* p = (DWORD*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_SOURCEBLEND:
                case RS_DESTBLEND:
                    {
                        SCENEBLENDMODE* p = (SCENEBLENDMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_COORDSETINDEX:
                case RS_STENCILREF:
                    {
                        int* p = (int*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_TEXCOLORBLEND:
                case RS_TEXALPHABLEND:
                    {
                        TEXTUREBLENDMODE* p = (TEXTUREBLENDMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_STDMATERIAL:
                    {
                        FLYMATERIAL* p = (FLYMATERIAL*)rs->pStateData;
                        delete p;
                    }
                    break;
                }
            }
        }
    }
}


//------------------------------------------------------------------------
// Compile the render effect file from the xml document.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path of the xml document.
//------------------------------------------------------------------------
HRESULT FlyRenderEffectCompiler::CompileFromFile( const char* cXmlDoc )
{
    HRESULT hr;

    // Reset the old datas.
    Reset();

    // Add the file to the include list.
    m_IncludeList.push_back( cXmlDoc );

    hr = LoadSourceFromXML( cXmlDoc );
    if( FAILED(hr) ) return hr;

    // Check the technique count.
    if( m_Techniques.size() == 0 )
    {
        m_strError = "Effect must have a technique!";
        return FLY_FAIL;
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Compile the render effect file from the string buffer.
// ----------------------------------------------------------------------
// Param -> IN:
//      const void*:        Pointer to the effect source buffer.
//------------------------------------------------------------------------
HRESULT FlyRenderEffectCompiler::CompileFromBuffer( const void* pCodeBuffer )
{
    HRESULT hr;
    FlyXmlReader reader;

    // Reset the old datas.
    Reset();

    // Load the xml document.
    hr = reader.ParseFromBuffer( pCodeBuffer );
    if( FAILED(hr) )
    {
        m_strError = "Parse from the xml buffer failed!";
        return hr;
    }

    // Start to analyse the code.
    return Go( reader.GetRootNode(),REST_MEMORY,NULL );
}


//------------------------------------------------------------------------
// Load the render effect file from the xml document.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:        Path to the xml document.
//------------------------------------------------------------------------
HRESULT FlyRenderEffectCompiler::LoadSourceFromXML( const char* cXmlDoc )
{
    HRESULT hr;
    FlyXmlReader reader;

    // Load the xml document.
    hr = reader.LoadDocument( cXmlDoc );
    if( FAILED(hr) )
    {
        m_strError = "Load the xml source file failed!";
        return hr;
    }

    // Start to analyse the the code.
    return Go( reader.GetRootNode(),REST_FILE,cXmlDoc );
}


//------------------------------------------------------------------------
// Go! Start to compile xml source code.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyXmlReader::XmlNodeAccess:    Root node of the xml document.
//      int:                            Type of the source. (Memory,File)
//      const char*:                    Path to the current file.
//------------------------------------------------------------------------
HRESULT FlyRenderEffectCompiler::Go( FlyXmlReader::XmlNodeAccess root,int srcType,
                                     const char* cFilename )
{
    FlyXmlReader::XmlNodeAccess child0;

    // Is an invalid effect file ?
    if( !root.GetName() || strcmp(root.GetName(),"effect") != 0 )
    {
        ReportError( "Can't find the effect node!",root );
        return FLY_FAIL;
    }

    if( !root.GetParamValueByName("version") ||
        strcmp(root.GetParamValueByName("version"),"1.0") != 0 )
    {
        ReportError( "Can't load the file of specified version!",root );
        return FLY_FAIL;
    }

    // Read the elements of effect.
    for( int i=0;i<root.GetChildCount();i++ )
    {
        child0 = root.GetChild( i );

        // 1. Handle the parameters.
        if( strcmp(child0.GetName(),"parameter") == 0 )
        {
            if( !HandleParameter(child0) )
                return FLY_FAIL;
        }

        // 2. Handle the shaders.
        else if( strcmp(child0.GetName(),"shader") == 0 )
        {
            if( !HandleShader(child0) )
                return FLY_FAIL;
        }

        // 3. Handle the techniques.
        else if( strcmp(child0.GetName(),"technique") == 0 )
        {
            if( !HandleTechnique(child0) )
                return FLY_FAIL;
        }

        // 4. Handle the include command.
        else if( strcmp(child0.GetName(),"include") == 0 )
        {
            if( !HandleInclude(child0.GetParamValueByName("name"),srcType,cFilename) )
                return FLY_FAIL;
        }

        // Other unknown mark.
        else
        {
            ReportError( "Unknown component for the effect!",child0 );
            return FLY_FAIL;
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Handle the include command for the effect file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    File path to the include file.
//      int:            The type of the source. (From memory or file?)
//      const char*:    File path to the current file.
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::HandleInclude( const char* cIncludeFile,int srcType,
                                             const char* cCurrentFile )
{
    if( srcType == REST_MEMORY )
    {
        m_strError = "XML code buffer in memory can't include file!";
        return false;
    }

    char* cFilePath = (char*)FlyStringUtil::GetDirFromPath( cCurrentFile );
    strcat( cFilePath,cIncludeFile );

    for( int i=0;i<m_IncludeList.size();i++ )
    {
        if( m_IncludeList[i] == cFilePath )
        {
            m_strError = "The file has been included! ";
            m_strError += "(";
            m_strError += cFilePath;
            m_strError += ")";
            return false;
        }
    }

    // Add the include file to list.
    m_IncludeList.push_back( cFilePath );

    if( srcType == REST_FILE )
    {
        if( FAILED(LoadSourceFromXML(cFilePath)) )
        {
            m_strError += "\n(Open include file ";
            m_strError += cIncludeFile;
            m_strError += " error!)";
            return false;
        }
    }

    return true;
}


//------------------------------------------------------------------------
// Handle the parameter for the render effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyXmlReader::XmlNodeAccess:    Access to the xml node.
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::HandleParameter( FlyXmlReader::XmlNodeAccess param )
{
    FlySampler samp;
    REDATATYPE dataType;
    FlyXmlReader::XmlNodeAccess annotation;
    FX_Parameter* pParam;

    if( !param.GetParamValueByName("name") )
    {
        ReportError( "Parameter must have a name!",param );
        return false;
    }

    if( !param.GetParamValueByName("type") )
    {
        ReportError( "Parameter must have a type!",param );
        return false;
    }

    dataType = GetDataType( param.GetParamValueByName("type") );
    if( dataType == REDT_UNKNOWN )
    {
        ReportError( "Unknown type for the parameter!",param );
        return false;
    }

    // Add the new parameter.
    pParam = AddParameter( param.GetParamValueByName("name") );
    if( !pParam )
    {
        ReportError( "The parameter has been defined!",param );
        return false;
    }

    // Set the type and semantic.
    pParam->Type = dataType;
    if( param.GetParamValueByName("semantic") )
        pParam->semantic = param.GetParamValueByName( "semantic" );
    else
        pParam->semantic = "";

    // Read the array size.
    if( !param.GetParamValueByName("size") )
        pParam->nArraySize = 1;
    else
    {
        pParam->nArraySize = atoi( param.GetParamValueByName("size") );

        if( pParam->nArraySize <= 0 || pParam->nArraySize > 65536 )
        {
            ReportError( "Array's size must be 1 to 65536!",param );
            return false;
        }

        if( param.GetParamValueByName("value") && pParam->nArraySize > 1 )
        {
            ReportError( "Can't give the initial value to array!",param );
            return false;
        }
    }

    if( !param.GetParamValueByName("auto") )
        pParam->bAuto = false;
    else
    {
        if( strcmp(param.GetParamValueByName("auto"),"true") == 0 )
        {
            pParam->bAuto = true;

            const char* cTemp = param.GetParamValueByName( "autotype" );
            if( strcmp(cTemp,"world") == 0 )
                pParam->autoType = APT_WORLDMATRIX;
            else if( strcmp(cTemp,"view") == 0 )
                pParam->autoType = APT_VIEWMATRIX;
            else if( strcmp(cTemp,"projection") == 0 )
                pParam->autoType = APT_PROJECTIONMATRIX;
            else if( strcmp(cTemp,"worldviewproj") == 0 )
                pParam->autoType = APT_WORLDVIEWPROJMATRIX;
            else if( strcmp(cTemp,"ambient") == 0 )
                pParam->autoType = APT_AMBIENTCOLOR;
        }
    }

    switch( dataType )
    {
    case REDT_BOOL:
        if( pParam->nArraySize == 1 )
        {
            pParam->pVar = new RenderVariableBool;

            if( !param.GetParamValueByName("value") )
                *pParam->pVar = false;
            else
            {
                if( strcmp(param.GetParamValueByName("value"),"false") == 0 )
                    *pParam->pVar = false;
                else
                    *pParam->pVar = true;
            }
        }
        break;
    case REDT_DWORD:
    case REDT_INT:
        if( pParam->nArraySize == 1 )
        {
            pParam->pVar = new RenderVariableInt;

            if( !param.GetParamValueByName("value") )
                *pParam->pVar = 0;
            else
                *pParam->pVar = atoi( param.GetParamValueByName("value") );
        }
        break;
    case REDT_SAMPLER2D:
    case REDT_SAMPLER3D:
    case REDT_SAMPLERCUBE:
        if( pParam->nArraySize > 1 )
        {
            ReportError( "Sampler parameter can't be array!",param );
            return false;
        }
        else
        {
            pParam->pVar = new RenderVariableSampler;
            pParam->pVar->Value( samp );
            samp.InitSamplerStates();
            *pParam->pVar = samp;
        }
        break;
    case REDT_FLOAT:
        if( pParam->nArraySize == 1 )
        {
            pParam->pVar = new RenderVariableFloat;

            if( !param.GetParamValueByName("value") )
                *pParam->pVar = 0.0f;
            else
                *pParam->pVar = (float)atof( param.GetParamValueByName("value") );
        }
        break;
    case REDT_FLOAT2:
        if( pParam->nArraySize > 1 )
        {
            ReportError( "Float2 parameter can't be array!",param );
            return false;
        }
        else
        {
            pParam->pVar = new RenderVariableFloat2;
            if( !param.GetParamValueByName("value") )
            {
                float2 arr;
                arr.x = arr.y = 0.0f;
                *pParam->pVar = arr;
            }
            else
            {
                float2 arr;
                sscanf( param.GetParamValueByName("value"),"{%f,%f}",&arr.x,&arr.y );
                *pParam->pVar = arr;
            }
        }
        break;
    case REDT_FLOAT3:
        if( pParam->nArraySize > 1 )
        {
            ReportError( "Float3 parameter can't be array!",param );
            return false;
        }
        else
        {
            pParam->pVar = new RenderVariableFloat3;
            if( !param.GetParamValueByName("value") )
            {
                float3 arr;
                arr.x = arr.y = arr.z = 0.0f;
                *pParam->pVar = arr;
            }
            else
            {
                float3 arr;
                sscanf( param.GetParamValueByName("value"),"{%f,%f,%f}",&arr.x,&arr.y,&arr.z );
                *pParam->pVar = arr;
            }
        }
        break;
    case REDT_FLOAT4:
        if( pParam->nArraySize == 1 )
        {
            pParam->pVar = new RenderVariableVector;

            if( !param.GetParamValueByName("value") )
            {
                FlyVector arr;
                arr.x = arr.y = arr.z = arr.w = 0.0f;
                *pParam->pVar = arr;
            }
            else
            {
                FlyVector arr;
                sscanf( param.GetParamValueByName("value"),"{%f,%f,%f,%f}",&arr.x,&arr.y,&arr.z,&arr.w );
                *pParam->pVar = arr;
            }
        }
        break;
    case REDT_FLOAT4X4:
        if( pParam->nArraySize == 1 )
        {
            pParam->pVar = new RenderVariableMatrix;

            if( !param.GetParamValueByName("value") )
            {
                FlyMatrix arr;
                memset( &arr,0,sizeof(FlyMatrix) );
                *pParam->pVar = arr;
            }
            else
            {
                FlyMatrix arr;
                sscanf( param.GetParamValueByName("value"),"{{%f,%f,%f,%f},{%f,%f,%f,%f},{%f,%f,%f,%f},{%f,%f,%f,%f}}",
                        &arr._11,&arr._12,&arr._13,&arr._14,&arr._21,&arr._22,&arr._23,&arr._24,
                        &arr._31,&arr._32,&arr._33,&arr._34,&arr._41,&arr._42,&arr._43,&arr._44 );
                *pParam->pVar = arr;
            }
        }
        break;
    default:
        ReportError( "Invalid type for the parameter!",param );
        return false;
    }

    // Read the annotation for the parameter.
    for( int s=0;s<param.GetChildCount();s++ )
    {
        annotation = param.GetChild( s );

        if( strcmp(annotation.GetName(),"annotation") == 0 )
        {
            if( !HandleAnnotation(annotation,ANNO_PARAMETER,pParam) )
                return false;
        }
        else if( strcmp(annotation.GetName(),"state") == 0 )
        {
            REDATATYPE dataType = GetDataType( param.GetParamValueByName("type") );

            if( dataType != REDT_SAMPLER2D && dataType != REDT_SAMPLER3D &&
                dataType != REDT_SAMPLERCUBE )
            {
                ReportError( "Only sampler parameter can have state attributes!",annotation );
                return false;
            }

            if( !HandleSampler(annotation,pParam) )
                return false;
        }
        else
        {
            ReportError( "There is an invalid child node in the parameter!",annotation );
            return false;
        }
    }

    return true;
}


//------------------------------------------------------------------------
// Handle the shader component for the render effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyXmlReader::XmlNodeAccess:    Access to the shader node.
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::HandleShader( FlyXmlReader::XmlNodeAccess shader )
{
    bool bCode = false;
    std::string shaderCode;
    FlyXmlReader::XmlNodeAccess func;

    for( int f=0;f<shader.GetChildCount();f++ )
    {
        func = shader.GetChild( f );

        if( strcmp(func.GetName(),"function") == 0 )
        {
            if( !func.GetParamValueByName("name") )
            {
                ReportError( "Shader Function need a name!",func );
                return false;
            }

            if( !AddShaderFunc(func.GetParamValueByName("name")) )
            {
                ReportError( "The function has been defined!",func );
                return false;
            }
        }
        else if( strcmp(func.GetName(),"code") == 0 )
        {
            if( bCode )
            {
                ReportError( "A shader only can have one 'code' section!",func );
                return false;
            }

            shaderCode = func.GetText();
            m_Shader.shader_code.push_back( shaderCode );
            bCode = true;
        }
        else
        {
            ReportError( "Unknown component for the shader!",func );
            return false;
        }
    }

    if( !bCode )
    {
        ReportError( "A shader need a 'code' section!",shader );
        return false;
    }

    return true;
}


//------------------------------------------------------------------------
// Handle the technique component for the render effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyXmlReader::XmlNodeAccess:    Access to the technique node.
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::HandleTechnique( FlyXmlReader::XmlNodeAccess technique )
{
    FX_Technique* pTechnique;
    FlyXmlReader::XmlNodeAccess pass;
    FlyXmlReader::XmlNodeAccess state;

    if( !technique.GetParamValueByName("name") )
    {
        ReportError( "Technique must have a name!",technique );
        return false;
    }

    // Add the technique.
    pTechnique = AddTechnique( technique.GetParamValueByName("name") );
    if( !pTechnique )
    {
        ReportError( "The technique has been defined!",technique );
        return false;
    }

    for( int p=0;p<technique.GetChildCount();p++ )
    {
        pass = technique.GetChild( p );

        if( strcmp(pass.GetName(),"annotation") == 0 )
        {
            if( !HandleAnnotation(pass,ANNO_TECHNIQUE,pTechnique) )
                return false;
        }
        else if( strcmp(pass.GetName(),"pass") == 0 )
        {
            if( !pass.GetName() )
            {
                ReportError( "Render pass must have a name!",pass );
                return false;
            }

            // Add the new pass.
            FX_Pass* pPass = AddPassToTechnique( pTechnique,pass.GetParamValueByName("name") );
            if( !pPass )
            {
                ReportError( "The render pass have been defined in the technique!",pass );
                return false;
            }

            for( int s=0;s<pass.GetChildCount();s++ )
            {
                state = pass.GetChild( s );

                if( strcmp(state.GetName(),"annotation") == 0 )
                {
                    if( !HandleAnnotation(state,ANNO_PASS,pPass) )
                        return false;
                }
                else if( strcmp(state.GetName(),"state") == 0 )
                {
                    if( !HandleRenderState(state,pPass) )
                        return false;
                }
                else
                {
                    ReportError( "Unknown component for the render pass!",state );
                    return false;
                }
            }
        }
        else
        {
            ReportError( "Unknown component for the technique!",pass );
            return false;
        }
    }

    return true;
}


//------------------------------------------------------------------------
// Handle the sampler state for the parameter.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyXmlReader::XmlNodeAccess:    Access to the sampler state.
//      FX_Parameter*:                  Pointer to the parameter.
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::HandleSampler( FlyXmlReader::XmlNodeAccess sampler,FX_Parameter* pParam )
{
    FlySampler samp;
    pParam->pVar->Value( samp );

    // Read the states for the sampler.
    if( !sampler.GetParamValueByName("name") || !sampler.GetParamValueByName("value") )
    {
        ReportError( "Sampler state must have the name and value!",sampler );
        return false;
    }

    if( strcmp(sampler.GetParamValueByName("name"),"mag_filter") == 0 )
    {
        samp.MagFilter = GetFilterOption( sampler.GetParamValueByName("value") );

        if( samp.MagFilter == -1 )
        {
            ReportError( "Unknown value for mag_filter state!",sampler );
            return false;
        }
    }
    else if( strcmp(sampler.GetParamValueByName("name"),"min_filter") == 0 )
    {
        samp.MinFilter = GetFilterOption( sampler.GetParamValueByName("value") );

        if( samp.MinFilter == -1 )
        {
            ReportError( "Unknown value for min_filter state!",sampler );
            return false;
        }
    }
    else if( strcmp(sampler.GetParamValueByName("name"),"mip_filter") == 0 )
    {
        samp.MipFilter = GetFilterOption( sampler.GetParamValueByName("value") );

        if( samp.MipFilter == -1 )
        {
            ReportError( "Unknown value for mip_filter state!",sampler );
            return false;
        }
    }
    else if( strcmp(sampler.GetParamValueByName("name"),"address_u") == 0 )
    {
        samp.AddressU = GetAddressMode( sampler.GetParamValueByName("value") );

        if( samp.AddressU == -1 )
        {
            ReportError( "Unknown value for address_u state!",sampler );
            return false;
        }
    }
    else if( strcmp(sampler.GetParamValueByName("name"),"address_v") == 0 )
    {
        samp.AddressV = GetAddressMode( sampler.GetParamValueByName("value") );

        if( samp.AddressV == -1 )
        {
            ReportError( "Unknown value for address_v state!",sampler );
            return false;
        }
    }
    else if( strcmp(sampler.GetParamValueByName("name"),"address_w") == 0 )
    {
        samp.AddressW = GetAddressMode( sampler.GetParamValueByName("value") );

        if( samp.AddressW == -1 )
        {
            ReportError( "Unknown value for address_w state!",sampler );
            return false;
        }
    }
    else if( strcmp(sampler.GetParamValueByName("name"),"border_color") == 0 )
        sscanf( sampler.GetParamValueByName("value"),"0x%x",&samp.dwBorderColor );
    else if( strcmp(sampler.GetParamValueByName("name"),"mipmap_lodbias") == 0 )
        samp.fMipMapLodBias = (float)atof( sampler.GetParamValueByName("value") );
    else if( strcmp(sampler.GetParamValueByName("name"),"anisotropy") == 0 )
        samp.nMaxAnisotropy = (UINT)atoi( sampler.GetParamValueByName("value") );
    else if( strcmp(sampler.GetParamValueByName("name"),"max_miplevel") == 0 )
        samp.nMaxMipLevel = (UINT)atoi( sampler.GetParamValueByName("value") );
    else
    {
        ReportError( "Unknown state for the sampler!",sampler );
        return false;
    }

    *pParam->pVar = samp;
    return true;
}


//------------------------------------------------------------------------
// Handle the annotations for the render effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyXmlReader::XmlNodeAccess: Access to the xml node.
//      int: Type of the annotation's parent.
//      void*: Pointer to the annotation's parent.
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::HandleAnnotation( FlyXmlReader::XmlNodeAccess anno,int type,void* ptr )
{
    REDATATYPE annoType;
    FX_Annotation* pAnnotation;

    if( !anno.GetParamValueByName("name") ||
        !anno.GetParamValueByName("type") ||
        !anno.GetParamValueByName("value") )
    {
        ReportError( "Parameter's Annotation must have 'name','type' and 'value'!",anno );
        return false;
    }

    // Check the type of the annotation.
    annoType = GetDataType( anno.GetParamValueByName("type") );
    if( annoType == REDT_UNKNOWN )
    {
        ReportError( "Unknown type for the parameter's annotation!",anno );
        return false;
    }

    // Add the annotation to parameter.
    switch( type )
    {
    case ANNO_PARAMETER:
        pAnnotation = AddAnnotationToParam( (FX_Parameter*)ptr,anno.GetParamValueByName("name") );
        if( !pAnnotation )
        {
            ReportError( "The parameter annotation has been defined!",anno );
            return false;
        }
        break;
    case ANNO_TECHNIQUE:
        pAnnotation = AddAnnotationToTechnique( (FX_Technique*)ptr,anno.GetParamValueByName("name") );
        if( !pAnnotation )
        {
            ReportError( "The technique annotation has been defined!",anno );
            return false;
        }
        break;
    case ANNO_PASS:
        pAnnotation = AddAnnotationToPass( (FX_Pass*)ptr,anno.GetParamValueByName("name") );
        if( !pAnnotation )
        {
            ReportError( "The pass annotation has been defined!",anno );
            return false;
        }
        break;
    }

    pAnnotation->Type = annoType;

    switch( annoType )
    {
    case REDT_BOOL:
        pAnnotation->pVar = new RenderVariableBool;
        if( strcmp(anno.GetParamValueByName("value"),"false") == 0 )
            *pAnnotation->pVar = false;
        else
            *pAnnotation->pVar = true;
        break;
    case REDT_DWORD:
    case REDT_INT:
        pAnnotation->pVar = new RenderVariableInt;
        *pAnnotation->pVar = atoi( anno.GetParamValueByName("value") );
        break;
    case REDT_STRING:
        pAnnotation->pVar = new RenderVariableString;
        *pAnnotation->pVar = std::string( anno.GetParamValueByName("value") );
        break;
    case REDT_FLOAT:
        pAnnotation->pVar = new RenderVariableFloat;
        *pAnnotation->pVar = (float)atof( anno.GetParamValueByName("value") );
        break;
    case REDT_FLOAT2:
        pAnnotation->pVar = new RenderVariableFloat2;
        {
            float2 arr;
            sscanf( anno.GetParamValueByName("value"),"{%f,%f}",&arr.x,&arr.y );
            *pAnnotation->pVar = arr;
        }
        break;
    case REDT_FLOAT3:
        pAnnotation->pVar = new RenderVariableFloat3;
        {
            float3 arr;
            sscanf( anno.GetParamValueByName("value"),"{%f,%f,%f}",&arr.x,&arr.y,&arr.z );
            *pAnnotation->pVar = arr;
        }
        break;
    case REDT_FLOAT4:
        pAnnotation->pVar = new RenderVariableVector;
        {
            FlyVector arr;
            sscanf( anno.GetParamValueByName("value"),"{%f,%f,%f,%f}",&arr.x,&arr.y,&arr.z,&arr.w );
            *pAnnotation->pVar = arr;
        }
        break;
    case REDT_FLOAT4X4:
        pAnnotation->pVar = new RenderVariableMatrix;
        {
            FlyMatrix arr;
            sscanf( anno.GetParamValueByName("value"),"{{%f,%f,%f,%f},{%f,%f,%f,%f},{%f,%f,%f,%f},{%f,%f,%f,%f}}",
                &arr._11,&arr._12,&arr._13,&arr._14,&arr._21,&arr._22,&arr._23,&arr._24,
                &arr._31,&arr._32,&arr._33,&arr._34,&arr._41,&arr._42,&arr._43,&arr._44 );
            *pAnnotation->pVar = arr;
        }
        break;
    default:
        ReportError( "Invalid type for the parameter annotation!",anno );
        return false;
    }

    return true;
}


//------------------------------------------------------------------------
// Handle the render states for the pass.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyXmlReader::XmlNodeAccess:    Access to the render state.
//      FX_Pass*:                       Pointer to the render pass.
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::HandleRenderState( FlyXmlReader::XmlNodeAccess state,FX_Pass* pPass )
{
    FX_RenderState rs;

    if( !state.GetParamValueByName("name") )
    {
        ReportError( "Render state must have a name!",state );
        return false;
    }

    if( strcmp(state.GetParamValueByName("name"),"material") != 0 &&
        strcmp(state.GetParamValueByName("name"),"light") != 0 &&
        strcmp(state.GetParamValueByName("name"),"tex_colorblend") != 0 &&
        strcmp(state.GetParamValueByName("name"),"tex_alphablend") != 0 &&
        !state.GetParamValueByName("value") )
    {
        ReportError( "Render state need a value!",state );
        return false;
    }

    if( strcmp(state.GetParamValueByName("name"),"cullmode") == 0 )
    {
        CULLMODE* p = new CULLMODE;
        rs.Type = RS_CULLMODE;

        if( strcmp(state.GetParamValueByName("value"),"cw") == 0 )
            *p = CM_CW;
        else if( strcmp(state.GetParamValueByName("value"),"ccw") == 0 )
            *p = CM_CCW;
        else if( strcmp(state.GetParamValueByName("value"),"none") == 0 )
            *p = CM_NONE;
        else
        {
            ReportError( "Unknown value for cull mode!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"polygonmode") == 0 )
    {
        POLYGONMODE* p = new POLYGONMODE;
        rs.Type = RS_POLYGONMODE;

        if( strcmp(state.GetParamValueByName("value"),"point") == 0 )
            *p = PM_POINT;
        else if( strcmp(state.GetParamValueByName("value"),"wireframe") == 0 )
            *p = PM_WIREFRAME;
        else if( strcmp(state.GetParamValueByName("value"),"solid") == 0 )
            *p = PM_SOLID;
        else
        {
            ReportError( "Unknown value for polygon mode!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"shademode") == 0 )
    {
        SHADEMODE* p = new SHADEMODE;
        rs.Type = RS_SHADEMODE;

        if( strcmp(state.GetParamValueByName("value"),"flat") == 0 )
            *p = SM_FLAT;
        else if( strcmp(state.GetParamValueByName("value"),"gouraud") == 0 )
            *p = SM_GOURAUD;
        else
        {
            ReportError( "Unknown value for shade mode!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"depthmode") == 0 )
    {
        DEPTHMODE* p = new DEPTHMODE;
        rs.Type = RS_DEPTHMODE;

        if( strcmp(state.GetParamValueByName("value"),"none") == 0 )
            *p = DEPTH_NONE;
        else if( strcmp(state.GetParamValueByName("value"),"readonly") == 0 )
            *p = DEPTH_READONLY;
        else if( strcmp(state.GetParamValueByName("value"),"readwrite") == 0 )
            *p = DEPTH_READWRITE;
        else
        {
            ReportError( "Unknown value for depth buffer mode!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"depthfunc") == 0 )
    {
        FLYCMPFUNC* p = new FLYCMPFUNC;
        rs.Type = RS_DEPTHFUNC;

        *p = GetCmpFunc( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown value for depth compare func!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"slopescale") == 0 )
    {
        float* p = new float;
        rs.Type = RS_SLOPESCALE;

        *p = (float)atof( state.GetParamValueByName("value") );
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"depthbias") == 0 )
    {
        float* p = new float;
        rs.Type = RS_DEPTHBIAS;

        *p = (float)atof( state.GetParamValueByName("value") );
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"stencil_enable") == 0 )
    {
        bool* p = new bool;
        rs.Type = RS_FRONTSTENCILENABLE;

        if( strcmp(state.GetParamValueByName("value"),"false") == 0 )
            *p = false;
        else
            *p = true;
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"ccw_stencil_enable") == 0 )
    {
        bool* p = new bool;
        rs.Type = RS_BACKSTENCILENABLE;

        if( strcmp(state.GetParamValueByName("value"),"false") == 0 )
            *p = false;
        else
            *p = true;
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"stencil_func") == 0 )
    {
        FLYCMPFUNC* p = new FLYCMPFUNC;
        rs.Type = RS_STENCILFUNC;

        *p = GetCmpFunc( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown value for stencil compare func!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"stencil_ref") == 0 )
    {
        int* p = new int;
        rs.Type = RS_STENCILREF;

        *p = atoi( state.GetParamValueByName("value") );
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"stencil_mask") == 0 )
    {
        DWORD* p = new DWORD;
        rs.Type = RS_STENCILMASK;

        *p = (DWORD)atol( state.GetParamValueByName("value") );
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"stencil_write_mask") == 0 )
    {
        DWORD* p = new DWORD;
        rs.Type = RS_STENCILWRITEMASK;

        *p = (DWORD)atol( state.GetParamValueByName("value") );
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"stencil_fail") == 0 )
    {
        STENCILOPERATION* p = new STENCILOPERATION;
        rs.Type = RS_STENCILFAIL;

        *p = GetStencilOp( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown operation for the stencil_fail!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"stencil_zfail") == 0 )
    {
        STENCILOPERATION* p = new STENCILOPERATION;
        rs.Type = RS_STENCILDEPTHFAIL;

        *p = GetStencilOp( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown operation for the stencil_zfail!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"stencil_pass") == 0 )
    {
        STENCILOPERATION* p = new STENCILOPERATION;
        rs.Type = RS_STENCILPASS;

        *p = GetStencilOp( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown operation for the stencil_pass!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"ccw_stencil_func") == 0 )
    {
        FLYCMPFUNC* p = new FLYCMPFUNC;
        rs.Type = RS_BACKSTENCILFUNC;

        *p = GetCmpFunc( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown compare function for the ccw_stencil!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"ccw_stencil_fail") == 0 )
    {
        STENCILOPERATION* p = new STENCILOPERATION;
        rs.Type = RS_BACKSTENCILFAIL;

        *p = GetStencilOp( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown operation for the ccw_stencil_fail!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"ccw_stencil_zfail") == 0 )
    {
        STENCILOPERATION* p = new STENCILOPERATION;
        rs.Type = RS_BACKSTENCILDEPTHFAIL;

        *p = GetStencilOp( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown operation for the ccw_stencil_zfail!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"ccw_stencil_pass") == 0 )
    {
        STENCILOPERATION* p = new STENCILOPERATION;
        rs.Type = RS_BACKSTENCILPASS;

        *p = GetStencilOp( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown operation for the ccw_stencil_pass!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"alphablend_enable") == 0 )
    {
        bool* p = new bool;
        rs.Type = RS_ALPHABLENDENABLE;

        if( strcmp(state.GetParamValueByName("value"),"false") == 0 )
            *p = false;
        else
            *p = true;
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"alphatest_enable") == 0 )
    {
        bool* p = new bool;
        rs.Type = RS_ALPHATESTENABLE;

        if( strcmp(state.GetParamValueByName("value"),"false") == 0 )
            *p = false;
        else
            *p = true;
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"alphafunc") == 0 )
    {
        FLYCMPFUNC* p = new FLYCMPFUNC;
        rs.Type = RS_ALPHATESTFUNC;

        *p = GetCmpFunc( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown value for the alpha test func!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"alpharef") == 0 )
    {
        DWORD* p = new DWORD;
        rs.Type = RS_ALPHATESTREF;

        *p = (DWORD)atol( state.GetParamValueByName("value") );
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"srcblend") == 0 )
    {
        SCENEBLENDMODE* p = new SCENEBLENDMODE;
        rs.Type = RS_SOURCEBLEND;

        *p = GetSceneBlendMode( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown value for the srcblend!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"destblend") == 0 )
    {
        SCENEBLENDMODE* p = new SCENEBLENDMODE;
        rs.Type = RS_DESTBLEND;

        *p = GetSceneBlendMode( state.GetParamValueByName("value") );
        if( *p == -1 )
        {
            ReportError( "Unknown value for the destblend!",state );
            delete p;
            return false;
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"coordindex") == 0 )
    {
        int* p = new int;
        rs.Type = RS_COORDSETINDEX;

        if( !state.GetParamValueByName("index") )
        {
            ReportError( "Coord index need an index value!",state );
            delete p;
            return false;
        }

        rs.nIndex = atoi( state.GetParamValueByName("index") );
        if( rs.nIndex < 0 || rs.nIndex >= MAX_TEXTURES )
        {
            ReportError( "Invalid index for the coordset!",state );
            delete p;
            return false;
        }

        *p = atoi( state.GetParamValueByName("value") );
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"tex_colorblend") == 0 ||
             strcmp(state.GetParamValueByName("name"),"tex_alphablend") == 0 )
    {
        FlyXmlReader::XmlNodeAccess child;
        TEXTUREBLENDMODE* p = new TEXTUREBLENDMODE;
        rs.Type = RS_TEXCOLORBLEND;

        if( strcmp(state.GetParamValueByName("name"),"tex_colorblend") == 0 )
            p->Type = BT_COLOR;
        else
            p->Type = BT_ALPHA;

        if( !state.GetParamValueByName("index") )
        {
            ReportError( "Color or alpha blend need an index value!",state );
            delete p;
            return false;
        }

        rs.nIndex = atoi( state.GetParamValueByName("index") );
        if( rs.nIndex < 0 || rs.nIndex >= MAX_TEXTURES )
        {
            ReportError( "Invalid index for the blend mode!",state );
            delete p;
            return false;
        }

        for( int i=0;i<state.GetChildCount();i++ )
        {
            child = state.GetChild( i );
            if( strcmp(child.GetName(),"member") != 0 )
            {
                ReportError( "Unknown mark for the render state!",child );
                delete p;
                return false;
            }

            if( !child.GetParamValueByName("name") ||
                !child.GetParamValueByName("value") )
            {
                ReportError( "Blend state need the name and value!",child );
                delete p;
                return false;
            }

            if( strcmp(child.GetParamValueByName("name"),"arg1") == 0 )
            {
                p->Arg1 = GetBlendSource( child.GetParamValueByName("value") );
                if( p->Arg1 == -1 )
                {
                    ReportError( "Unknown value for blend source arg1!",child );
                    delete p;
                    return false;
                }
            }
            else if( strcmp(child.GetParamValueByName("name"),"arg2") == 0 )
            {
                p->Arg2 = GetBlendSource( child.GetParamValueByName("value") );
                if( p->Arg2 == -1 )
                {
                    ReportError( "Unknown value for blend source arg2!",child );
                    delete p;
                    return false;
                }
            }
            else if( strcmp(child.GetParamValueByName("name"),"operation") == 0 )
            {
                p->Operation = GetTextureOP( child.GetParamValueByName("value") );
                if( p->Operation == -1 )
                {
                    ReportError( "Unknown value for texture operation!",child );
                    delete p;
                    return false;
                }
            }
            else if( strcmp(child.GetParamValueByName("name"),"manual_factor") == 0 )
                p->fFactor = (float)atof( child.GetParamValueByName("value") );
            else
            {
                ReportError( "Unknown member for the render state!",child );
                delete p;
                return false;
            }
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"lighting") == 0 )
    {
        bool* p = new bool;
        rs.Type = RS_LIGHTING;
        
        if( strcmp(state.GetParamValueByName("value"),"false") == 0 )
            *p = false;
        else
            *p = true;
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"specular_enable") == 0 )
    {
        bool* p = new bool;
        rs.Type = RS_SPECULARLIGHT;

        if( strcmp(state.GetParamValueByName("value"),"false") == 0 )
            *p = false;
        else
            *p = true;
        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"material") == 0 )
    {
        FlyXmlReader::XmlNodeAccess child;
        FLYMATERIAL* p = new FLYMATERIAL;
        rs.Type = RS_STDMATERIAL;

        for( int i=0;i<state.GetChildCount();i++ )
        {
            child = state.GetChild( i );
            if( strcmp(child.GetName(),"member") != 0 )
            {
                ReportError( "Unknown mark for the render state!",child );
                delete p;
                return false;
            }

            if( !child.GetParamValueByName("name") ||
                !child.GetParamValueByName("value") )
            {
                ReportError( "Material member need the name and value!",child );
                delete p;
                return false;
            }

            if( strcmp(child.GetParamValueByName("name"),"ambient") == 0 )
            {
                sscanf( child.GetParamValueByName("value"),"{%f,%f,%f,%f}",
                        &p->cAmbient.fR,&p->cAmbient.fG,&p->cAmbient.fB,&p->cAmbient.fA );
            }
            else if( strcmp(child.GetParamValueByName("name"),"diffuse") == 0 )
            {
                sscanf( child.GetParamValueByName("value"),"{%f,%f,%f,%f}",
                        &p->cDiffuse.fR,&p->cDiffuse.fG,&p->cDiffuse.fB,&p->cDiffuse.fA );
            }
            else if( strcmp(child.GetParamValueByName("name"),"specular") == 0 )
            {
                sscanf( child.GetParamValueByName("value"),"{%f,%f,%f,%f}",
                        &p->cSpecular.fR,&p->cSpecular.fG,&p->cSpecular.fB,&p->cSpecular.fA );
            }
            else if( strcmp(child.GetParamValueByName("name"),"emissive") == 0 )
            {
                sscanf( child.GetParamValueByName("value"),"{%f,%f,%f,%f}",
                        &p->cEmissive.fR,&p->cEmissive.fG,&p->cEmissive.fB,&p->cEmissive.fA );
            }
            else if( strcmp(child.GetParamValueByName("name"),"power") == 0 )
                p->fPower = (float)atof( child.GetParamValueByName("value") );
            else
            {
                ReportError( "Unknown member for the material state!",child );
                delete p;
                return false;
            }
        }

        rs.pStateData = p;
    }
    else if( strcmp(state.GetParamValueByName("name"),"vertex_shader") == 0 )
    {
        std::string profile;

        if( strcmp(state.GetParamValueByName("value"),"null") == 0 ||
            strcmp(state.GetParamValueByName("value"),"NULL") == 0 || 
            strcmp(state.GetParamValueByName("value"),"0") == 0 )
        {
            pPass->shaderDescs[0].sFuncName = "";
            pPass->shaderDescs[0].sProfile = "";
        }
        else
        {
            if( !state.GetParamValueByName("profile") )
                profile = "auto";
            else
                profile = state.GetParamValueByName( "profile" );

            if( !IsShaderFuncExisted(state.GetParamValueByName("value")) )
            {
                ReportError( "Shader function undefined! ",state );
                return false;
            }

            pPass->shaderDescs[0].sFuncName = state.GetParamValueByName( "value" );
            pPass->shaderDescs[0].sProfile = profile;
        }

        return true;
    }
    else if( strcmp(state.GetParamValueByName("name"),"pixel_shader") == 0 )
    {
        std::string profile;

        if( strcmp(state.GetParamValueByName("value"),"null") == 0 ||
            strcmp(state.GetParamValueByName("value"),"NULL") == 0 || 
            strcmp(state.GetParamValueByName("value"),"0") == 0 )
        {
            pPass->shaderDescs[1].sFuncName = "";
            pPass->shaderDescs[1].sProfile = "";
        }
        else
        {
            if( !state.GetParamValueByName("profile") )
                profile = "auto";
            else
                profile = state.GetParamValueByName( "profile" );

            if( !IsShaderFuncExisted(state.GetParamValueByName("value")) )
            {
                ReportError( "Shader function undefined! ",state );
                return false;
            }

            pPass->shaderDescs[1].sFuncName = state.GetParamValueByName( "value" );
            pPass->shaderDescs[1].sProfile = profile;
        }

        return true;
    }
    else
    {
        ReportError( "Unknown render state for the pass!",state );
        return false;
    }

    // Add the render states.
    pPass->render_states.push_back( rs );

    return true;
}


//------------------------------------------------------------------------
// Add a new parameter to the list.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the parameter.
//------------------------------------------------------------------------
FX_Parameter* FlyRenderEffectCompiler::AddParameter( const char* cName )
{
    FX_Parameter param;

    // Check whether the parameter has been defined.
    for( int i=0;i<m_Parameters.size();i++ )
    {
        if( m_Parameters[i].name == cName )
            return NULL;
    }

    // Add a new parameter.
    param.name = cName;
    param.pVar = NULL;
    param.nArraySize = 0;
    m_Parameters.push_back( param );

    return &m_Parameters[m_Parameters.size()-1];
}


//------------------------------------------------------------------------
// Add a shader function to the render effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the shader function.
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::AddShaderFunc( const char* cFuncName )
{
    std::string str = cFuncName;

    // Check whether the function has been defined.
    for( int i=0;i<m_Shader.funcs.size();i++ )
    {
        if( str == m_Shader.funcs[i] )
            return false;
    }

    // Add the new shader function.
    m_Shader.funcs.push_back( str );

    return true;
}


//------------------------------------------------------------------------
// Add a new technique to render effect.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the technique.
//------------------------------------------------------------------------
FX_Technique* FlyRenderEffectCompiler::AddTechnique( const char* cName )
{
    FX_Technique technique;

    // Check whether the technique has been defined.
    for( int i=0;i<m_Techniques.size();i++ )
    {
        if( m_Techniques[i].name == cName )
            return NULL;
    }

    // Add a new technique.
    technique.name = cName;
    m_Techniques.push_back( technique );

    return &m_Techniques[m_Techniques.size()-1];
}


//------------------------------------------------------------------------
// Add a new render pass to the technique.
// ----------------------------------------------------------------------
// Param -> IN:
//      FX_Technique*:  Pointer to the specified technique.
//      const char*:    Name of the technique.
//------------------------------------------------------------------------
FX_Pass* FlyRenderEffectCompiler::AddPassToTechnique( FX_Technique* pTechnique,const char* cName )
{
    FX_Pass pass;

    // Check whether the pass has been defined.
    for( int i=0;i<pTechnique->passes.size();i++ )
    {
        if( pTechnique->passes[i].name == cName )
            return NULL;
    }

    // Add a new render pass.
    pass.name = cName;
    pTechnique->passes.push_back( pass );

    return &pTechnique->passes[pTechnique->passes.size()-1];
}


//------------------------------------------------------------------------
// Add an annotation to the parameter.
// ----------------------------------------------------------------------
// Param -> IN:
//      FX_Parameter*:  Pointer to the parameter.
//      const char*:    Name of the annotation.
//------------------------------------------------------------------------
FX_Annotation* FlyRenderEffectCompiler::AddAnnotationToParam( FX_Parameter* param,const char* cName )
{
    FX_Annotation annotation;

    // Check whether the annotation has been defined.
    for( int i=0;i<param->annotations.size();i++ )
    {
        if( param->annotations[i].name == cName )
            return NULL;
    }

    // Add the new annotation.
    annotation.name = cName;
    param->annotations.push_back( annotation );

    return &param->annotations[param->annotations.size()-1];
}


//------------------------------------------------------------------------
// Add an annotation to the technique.
// ----------------------------------------------------------------------
// Param -> IN:
//      FX_Technique*:  Pointer to the technique.
//      const char*:    Name of the annotation.
//------------------------------------------------------------------------
FX_Annotation* FlyRenderEffectCompiler::AddAnnotationToTechnique( FX_Technique* technique,const char* cName )
{
    FX_Annotation annotation;

    // Check whether the annotation has been defined.
    for( int i=0;i<technique->annotations.size();i++ )
    {
        if( technique->annotations[i].name == cName )
            return NULL;
    }

    // Add the new annotation.
    annotation.name = cName;
    technique->annotations.push_back( annotation );

    return &technique->annotations[technique->annotations.size()-1];
}


//------------------------------------------------------------------------
// Add an annotation to the render pass.
// ----------------------------------------------------------------------
// Param -> IN:
//      FX_Pass*:       Pointer to the render pass.
//      const char*:    Name of the annotation.
//------------------------------------------------------------------------
FX_Annotation* FlyRenderEffectCompiler::AddAnnotationToPass( FX_Pass* pass,const char* cName )
{
    FX_Annotation annotation;

    // Check whether the annotation has been defined.
    for( int i=0;i<pass->annotations.size();i++ )
    {
        if( pass->annotations[i].name == cName )
            return NULL;
    }

    // Add the new annotation.
    annotation.name = cName;
    pass->annotations.push_back( annotation );

    return &pass->annotations[pass->annotations.size()-1];
}


//------------------------------------------------------------------------
// Report an error for the compiler.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:                    Information of the error!
//      FlyXmlReader::XmlNodeAccess:    Access to the current xml node.
//------------------------------------------------------------------------
void FlyRenderEffectCompiler::ReportError( const char* cErrorInfo,FlyXmlReader::XmlNodeAccess curNode )
{
    FlyXmlReader::XmlNodeAccess parent;

    // Clear the old data.
    m_strError = cErrorInfo;

    m_strError += "(Node:\"";
    m_strError += curNode.GetName();
    m_strError += "\" Parent:\"";

    if( !curNode.IsRoot() )
    {
        parent = curNode.GetParent();
        m_strError += parent.GetName();
        m_strError += "\")";
    }
    else
        m_strError += "RootNode\")";
}


//------------------------------------------------------------------------
// Reset the compiler's data.
//------------------------------------------------------------------------
HRESULT FlyRenderEffectCompiler::Reset(void)
{
    for( int i=0;i<m_Parameters.size();i++ )
    {
        FX_Parameter* param = &m_Parameters[i];
        for( int j=0;j<param->annotations.size();j++ )
        {
            SAFE_DELETE( param->annotations[j].pVar );
        }

        SAFE_DELETE( m_Parameters[i].pVar );
    }

    for( i=0;i<m_Techniques.size();i++ )
    {
        FX_Technique* tech = &m_Techniques[i];

        // Delete the annotation variables.
        for( int j=0;j<tech->annotations.size();j++ )
        {
            SAFE_DELETE( tech->annotations[j].pVar );
        }

        // Delete the render states.
        for( j=0;j<tech->passes.size();j++ )
        {
            FX_Pass* pass = &tech->passes[j];

            for( int r=0;r<pass->annotations.size();r++ )
            {
                SAFE_DELETE( pass->annotations[r].pVar );
            }

            for( r=0;r<pass->render_states.size();r++ )
            {
                FX_RenderState* rs = &pass->render_states[r];

                switch( rs->Type )
                {
                case RS_CULLMODE:
                    {
                        CULLMODE* p = (CULLMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_POLYGONMODE:
                    {
                        POLYGONMODE* p = (POLYGONMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_SHADEMODE:
                    {
                        SHADEMODE* p = (SHADEMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_DEPTHMODE:
                    {
                        DEPTHMODE* p = (DEPTHMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_DEPTHFUNC:
                case RS_ALPHATESTFUNC:
                case RS_STENCILFUNC:
                case RS_BACKSTENCILFUNC:
                    {
                        FLYCMPFUNC* p = (FLYCMPFUNC*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_SLOPESCALE:
                case RS_DEPTHBIAS:
                    {
                        float* p = (float*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_ALPHABLENDENABLE:
                case RS_ALPHATESTENABLE:
                case RS_LIGHTING:
                case RS_SPECULARLIGHT:
                case RS_FRONTSTENCILENABLE:
                case RS_BACKSTENCILENABLE:
                    {
                        bool* p = (bool*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_STENCILFAIL:
                case RS_STENCILDEPTHFAIL:
                case RS_STENCILPASS:
                case RS_BACKSTENCILFAIL:
                case RS_BACKSTENCILDEPTHFAIL:
                case RS_BACKSTENCILPASS:
                    {
                        STENCILOPERATION* p = (STENCILOPERATION*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_ALPHATESTREF:
                case RS_STENCILMASK:
                case RS_STENCILWRITEMASK:
                    {
                        DWORD* p = (DWORD*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_SOURCEBLEND:
                case RS_DESTBLEND:
                    {
                        SCENEBLENDMODE* p = (SCENEBLENDMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_COORDSETINDEX:
                case RS_STENCILREF:
                    {
                        int* p = (int*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_TEXCOLORBLEND:
                case RS_TEXALPHABLEND:
                    {
                        TEXTUREBLENDMODE* p = (TEXTUREBLENDMODE*)rs->pStateData;
                        delete p;
                    }
                    break;
                case RS_STDMATERIAL:
                    {
                        FLYMATERIAL* p = (FLYMATERIAL*)rs->pStateData;
                        delete p;
                    }
                    break;
                }
            }
        }
    }

    m_Parameters.clear();
    m_Shader.funcs.clear();
    m_Shader.shader_code.clear();
    m_Techniques.clear();
    m_IncludeList.clear();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Save the render effect data to the FFX file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the binary effect file.
//------------------------------------------------------------------------
HRESULT FlyRenderEffectCompiler::SaveEffectToFFX( const char* cFfxFile )
{
    size_t nLen;
    FX_Header header;

    // Create the binary file.
    FILE* pFile = fopen( cFfxFile,"wb" );

    // Write the file header.
    header.fourcc[0] = 'F';
    header.fourcc[1] = 'F';
    header.fourcc[2] = 'X';
    header.fourcc[3] = '\0';
    header.version = FLY_VERSION( 1,0 );
    header.num_parameters = m_Parameters.size();
    header.num_shaders = m_Shader.funcs.size();
    header.num_techniques = m_Techniques.size();
    fwrite( &header,sizeof(FX_Header),1,pFile );

    // Write the parameters.
    for( int i=0;i<m_Parameters.size();i++ )
    {
        FX_Parameter* pParam = &m_Parameters[i];

        // Write the name of the param.
        nLen = pParam->name.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        fwrite( pParam->name.c_str(),nLen,1,pFile );

        // Write the semantic of the param.
        nLen = pParam->semantic.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        fwrite( pParam->semantic.c_str(),nLen,1,pFile );

        // Write the array size.
        fwrite( &pParam->Type,sizeof(REDATATYPE),1,pFile );
        fwrite( &pParam->nArraySize,sizeof(UINT),1,pFile );

        fwrite( &pParam->bAuto,sizeof(bool),1,pFile );
        fwrite( &pParam->autoType,sizeof(int),1,pFile );

        // Write the initial value.
        if( pParam->nArraySize == 1 )
            WriteRenderVariable( pParam->Type,pParam->pVar,pFile );

        // Write the annotation.
        nLen = pParam->annotations.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( int j=0;j<nLen;j++ )
            WriteAnnotation( &pParam->annotations[j],pFile );
    }

    // Write the shader text.
    nLen = 0;
    for( i=0;i<m_Shader.shader_code.size();i++ )
        nLen += m_Shader.shader_code[i].size();
    fwrite( &nLen,sizeof(size_t),1,pFile );

    for( i=0;i<m_Shader.shader_code.size();i++ )
    {
        nLen = m_Shader.shader_code[i].size();
        fwrite( m_Shader.shader_code[i].c_str(),nLen,1,pFile );
    }

    // Write the techniques.
    for( i=0;i<m_Techniques.size();i++ )
    {
        FX_Technique* pTechnique = &m_Techniques[i];

        nLen = pTechnique->name.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        fwrite( pTechnique->name.c_str(),nLen,1,pFile );

        // Write the annotations.
        nLen = pTechnique->annotations.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( int j=0;j<nLen;j++ )
            WriteAnnotation( &pTechnique->annotations[j],pFile );

        // Write the passes.
        nLen = pTechnique->passes.size();
        fwrite( &nLen,sizeof(size_t),1,pFile );
        for( j=0;j<nLen;j++ )
            WriteRenderPass( &pTechnique->passes[j],pFile );
    }

    fclose( pFile );
    return FLY_OK;
}


//------------------------------------------------------------------------
// Write the Render variable for the effect file.
// ----------------------------------------------------------------------
// Param -> IN:
//      REDATATYPE:         Type of the variable.
//      FlyRenderVariable*: Pointer to the render variable.
//      FILE*:              Pointer to the file.
//------------------------------------------------------------------------
void FlyRenderEffectCompiler::WriteRenderVariable( REDATATYPE Type,FlyRenderVariable* pVar,FILE* pFile )
{
    size_t nLen;

    // Write the data value.
    switch( Type )
    {
    case REDT_BOOL:
        {
            bool value;
            pVar->Value( value );
            fwrite( &value,sizeof(bool),1,pFile );
        }
        break;
    case REDT_DWORD:
    case REDT_INT:
        {
            int value;
            pVar->Value( value );
            fwrite( &value,sizeof(int),1,pFile );
        }
        break;
    case REDT_STRING:
        {
            std::string value;
            pVar->Value( value );
            nLen = value.size();
            fwrite( &nLen,sizeof(size_t),1,pFile );
            fwrite( value.c_str(),nLen,1,pFile );
        }
        break;
    case REDT_SAMPLER2D:
    case REDT_SAMPLER3D:
    case REDT_SAMPLERCUBE:
        {
            FlySampler value;
            pVar->Value( value );
            fwrite( &value,sizeof(FlySampler),1,pFile );
        }
        break;
    case REDT_FLOAT:
        {
            float value;
            pVar->Value( value );
            fwrite( &value,sizeof(float),1,pFile );
        }
        break;
    case REDT_FLOAT2:
        {
            float2 value;
            pVar->Value( value );
            fwrite( &value,sizeof(float),2,pFile );
        }
        break;
    case REDT_FLOAT3:
        {
            float3 value;
            pVar->Value( value );
            fwrite( &value,sizeof(float),3,pFile );
        }
        break;
    case REDT_FLOAT4:
        {
            FlyVector value;
            pVar->Value( value );
            fwrite( &value,sizeof(FlyVector),1,pFile );
        }
        break;
    case REDT_FLOAT4X4:
        {
            FlyMatrix value;
            pVar->Value( value );
            fwrite( &value,sizeof(FlyMatrix),1,pFile );
        }
        break;
    }
}


//------------------------------------------------------------------------
// Write the annotation for the render effect file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FX_Annotation*:     Pointer to the annotation.
//      FILE*:              Pointer to the file.
//------------------------------------------------------------------------
void FlyRenderEffectCompiler::WriteAnnotation( FX_Annotation* pAnnotation,FILE* pFile )
{
    size_t nLen;

    nLen = pAnnotation->name.size();
    fwrite( &nLen,sizeof(size_t),1,pFile );
    fwrite( pAnnotation->name.c_str(),nLen,1,pFile );
    fwrite( &pAnnotation->Type,sizeof(REDATATYPE),1,pFile );

    // Write the render variable.
    WriteRenderVariable( pAnnotation->Type,pAnnotation->pVar,pFile );
}


//------------------------------------------------------------------------
// Write the render pass data for the effect file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FX_Pass*:           Pointer to the render pass.
//      FILE*:              Pointer to the file.
//------------------------------------------------------------------------
void FlyRenderEffectCompiler::WriteRenderPass( FX_Pass* pPass,FILE* pFile )
{
    size_t nLen;

    nLen = pPass->name.size();
    fwrite( &nLen,sizeof(size_t),1,pFile );
    fwrite( pPass->name.c_str(),nLen,1,pFile );

    // Write the annotations.
    nLen = pPass->annotations.size();
    fwrite( &nLen,sizeof(size_t),1,pFile );
    for( int i=0;i<nLen;i++ )
        WriteAnnotation( &pPass->annotations[i],pFile );

    // Write the vertex shader desc.
    nLen = pPass->shaderDescs[0].sFuncName.size();
    fwrite( &nLen,sizeof(size_t),1,pFile );
    fwrite( pPass->shaderDescs[0].sFuncName.c_str(),nLen,1,pFile );
    nLen = pPass->shaderDescs[0].sProfile.size();
    fwrite( &nLen,sizeof(size_t),1,pFile );
    fwrite( pPass->shaderDescs[0].sProfile.c_str(),nLen,1,pFile );

    nLen = pPass->shaderDescs[1].sFuncName.size();
    fwrite( &nLen,sizeof(size_t),1,pFile );
    fwrite( pPass->shaderDescs[1].sFuncName.c_str(),nLen,1,pFile );
    nLen = pPass->shaderDescs[1].sProfile.size();
    fwrite( &nLen,sizeof(size_t),1,pFile );
    fwrite( pPass->shaderDescs[1].sProfile.c_str(),nLen,1,pFile );

    // Write the render states.
    nLen = pPass->render_states.size();
    fwrite( &nLen,sizeof(size_t),1,pFile );
    for( i=0;i<nLen;i++ )
    {
        FX_RenderState* rs = &pPass->render_states[i];
        fwrite( &rs->Type,sizeof(REDATATYPE),1,pFile );

        switch( rs->Type )
        {
        case RS_CULLMODE:
        case RS_POLYGONMODE:
        case RS_SHADEMODE:
        case RS_DEPTHMODE:
        case RS_DEPTHFUNC:
        case RS_STENCILFUNC:
        case RS_STENCILREF:
        case RS_STENCILFAIL:
        case RS_STENCILDEPTHFAIL:
        case RS_STENCILPASS:
        case RS_BACKSTENCILFUNC:
        case RS_BACKSTENCILFAIL:
        case RS_BACKSTENCILDEPTHFAIL:
        case RS_BACKSTENCILPASS:
        case RS_ALPHATESTFUNC:
        case RS_SOURCEBLEND:
        case RS_DESTBLEND:
            fwrite( rs->pStateData,sizeof(int),1,pFile );
            break;
        case RS_SLOPESCALE:
        case RS_DEPTHBIAS:
            fwrite( rs->pStateData,sizeof(float),1,pFile );
            break;
        case RS_FRONTSTENCILENABLE:
        case RS_BACKSTENCILENABLE:
        case RS_ALPHABLENDENABLE:
        case RS_ALPHATESTENABLE:
            fwrite( rs->pStateData,sizeof(bool),1,pFile );
            break;
        case RS_ALPHATESTREF:
        case RS_STENCILMASK:
        case RS_STENCILWRITEMASK:
            fwrite( rs->pStateData,sizeof(DWORD),1,pFile );
            break;
        case RS_COORDSETINDEX:
            fwrite( &rs->nIndex,sizeof(int),1,pFile );
            fwrite( rs->pStateData,sizeof(int),1,pFile );
            break;
        case RS_TEXCOLORBLEND:
        case RS_TEXALPHABLEND:
            fwrite( &rs->nIndex,sizeof(int),1,pFile );
            fwrite( rs->pStateData,sizeof(TEXTUREBLENDMODE),1,pFile );
            break;
        case RS_STDMATERIAL:
            fwrite( rs->pStateData,sizeof(FLYMATERIAL),1,pFile );
            break;
        case RS_LIGHTING:
        case RS_SPECULARLIGHT:
            fwrite( rs->pStateData,sizeof(bool),1,pFile );
            break;
        }
    }
}


//------------------------------------------------------------------------
// Is the shader function defined ?
//------------------------------------------------------------------------
bool FlyRenderEffectCompiler::IsShaderFuncExisted( const char* cFuncName )
{
    for( int i=0;i<m_Shader.funcs.size();i++ )
    {
        if( m_Shader.funcs[i] == cFuncName )
            return true;
    }

    return false;
}


//------------------------------------------------------------------------
// Get the render effect data type from its string.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    String of the data type.
//------------------------------------------------------------------------
REDATATYPE FlyRenderEffectCompiler::GetDataType( const char* cType )
{
    if( strcmp(cType,"bool") == 0 )
        return REDT_BOOL;
    else if( strcmp(cType,"dword") == 0 )
        return REDT_DWORD;
    else if( strcmp(cType,"string") == 0 )
        return REDT_STRING;
    else if( strcmp(cType,"sampler2D") == 0 )
        return REDT_SAMPLER2D;
    else if( strcmp(cType,"sampler3D") == 0 )
        return REDT_SAMPLER3D;
    else if( strcmp(cType,"samplerCube") == 0 )
        return REDT_SAMPLERCUBE;
    else if( strcmp(cType,"int") == 0 )
        return REDT_INT;
    else if( strcmp(cType,"int2") == 0 )
        return REDT_INT2;
    else if( strcmp(cType,"int3") == 0 )
        return REDT_INT3;
    else if( strcmp(cType,"int4") == 0 )
        return REDT_INT4;
    else if( strcmp(cType,"float") == 0 )
        return REDT_FLOAT;
    else if( strcmp(cType,"float2") == 0 )
        return REDT_FLOAT2;
    else if( strcmp(cType,"float2x2") == 0 )
        return REDT_FLOAT2X2;
    else if( strcmp(cType,"float2x3") == 0 )
        return REDT_FLOAT2X3;
    else if( strcmp(cType,"float2x4") == 0 )
        return REDT_FLOAT2X4;
    else if( strcmp(cType,"float3") == 0 )
        return REDT_FLOAT3;
    else if( strcmp(cType,"float3x2") == 0 )
        return REDT_FLOAT3X2;
    else if( strcmp(cType,"float3x3") == 0 )
        return REDT_FLOAT3X3;
    else if( strcmp(cType,"float3x4") == 0 )
        return REDT_FLOAT3X4;
    else if( strcmp(cType,"float4") == 0 )
        return REDT_FLOAT4;
    else if( strcmp(cType,"float4x2") == 0 )
        return REDT_FLOAT4X2;
    else if( strcmp(cType,"float4x3") == 0 )
        return REDT_FLOAT4X3;
    else if( strcmp(cType,"float4x4") == 0 )
        return REDT_FLOAT4X4;
    else
        return REDT_UNKNOWN;
}


//------------------------------------------------------------------------
// Get the string format render effect data type.
// ----------------------------------------------------------------------
// Param -> IN:
//      REDATATYPE:     Render effect data type.
//------------------------------------------------------------------------
const char* FlyRenderEffectCompiler::GetDataType( REDATATYPE Type )
{
    switch( Type )
    {
    case REDT_BOOL:         return TEXT("bool");
    case REDT_DWORD:        return TEXT("int");
    case REDT_STRING:       return TEXT("string");
    case REDT_SAMPLER2D:    return TEXT("sampler2D");
    case REDT_SAMPLER3D:    return TEXT("sampler3D");
    case REDT_SAMPLERCUBE:  return TEXT("samplerCube");
    case REDT_INT:          return TEXT("int");
    case REDT_INT2:         return TEXT("int2");
    case REDT_INT3:         return TEXT("int3");
    case REDT_INT4:         return TEXT("int4");
    case REDT_FLOAT:        return TEXT("float");
    case REDT_FLOAT2:       return TEXT("float2");
    case REDT_FLOAT2X2:     return TEXT("float2x2");
    case REDT_FLOAT2X3:     return TEXT("float2x3");
    case REDT_FLOAT3:       return TEXT("float3");
    case REDT_FLOAT3X2:     return TEXT("float3x2");
    case REDT_FLOAT3X3:     return TEXT("float3x3");
    case REDT_FLOAT4:       return TEXT("float4");
    case REDT_FLOAT4X2:     return TEXT("float4x2");
    case REDT_FLOAT4X3:     return TEXT("float4x3");
    case REDT_FLOAT4X4:     return TEXT("float4x4");
    default:                return NULL;
    }
}


//------------------------------------------------------------------------
// Translate the string of the filter mode to the value.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    String of the filter mode.
//------------------------------------------------------------------------
FILTEROPTION FlyRenderEffectCompiler::GetFilterOption( const char* filter )
{
    if( strcmp(filter,"none") == 0 )
        return FILTER_NONE;
    else if( strcmp(filter,"point") == 0 )
        return FILTER_POINT;
    else if( strcmp(filter,"linear") == 0 )
        return FILTER_LINEAR;
    else if( strcmp(filter,"anisotropic") == 0 )
        return FILTER_ANISOTROPIC;
    else if( strcmp(filter,"pyramidalquad") == 0 )
        return FILTER_PYRAMIDALQUAD;
    else if( strcmp(filter,"gaussianquad") == 0 )
        return FILTER_PYRAMIDALQUAD;
    else
        return (FILTEROPTION)-1;
}


//------------------------------------------------------------------------
// Translate the string of the address mode to the value.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    String of the address mode.
//------------------------------------------------------------------------
TEXTUREADDRESS FlyRenderEffectCompiler::GetAddressMode( const char* address )
{
    if( strcmp(address,"wrap") == 0 )
        return TADDR_WRAP;
    else if( strcmp(address,"mirror") == 0 )
        return TADDR_MIRROR;
    else if( strcmp(address,"clamp") == 0 )
        return TADDR_CLAMP;
    else if( strcmp(address,"border") == 0 )
        return TADDR_BORDER;
    else if( strcmp(address,"mirroronce") == 0 )
        return TADDR_MIRRORONCE;
    else
        return (TEXTUREADDRESS)-1;
}


//------------------------------------------------------------------------
// Get the compare func value from the string.
//------------------------------------------------------------------------
FLYCMPFUNC FlyRenderEffectCompiler::GetCmpFunc( const char* cFunc )
{
    if( strcmp(cFunc,"never") == 0 )
        return CMP_NEVER;
    else if( strcmp(cFunc,"less") == 0 )
        return CMP_LESS;
    else if( strcmp(cFunc,"equal") == 0 )
        return CMP_EQUAL;
    else if( strcmp(cFunc,"lessequal") == 0 )
        return CMP_LESSEQUAL;
    else if( strcmp(cFunc,"greater") == 0 )
        return CMP_GREATER;
    else if( strcmp(cFunc,"greaterequal") == 0 )
        return CMP_GREATEREQUAL;
    else if( strcmp(cFunc,"notequal") == 0 )
        return CMP_NOTEQUAL;
    else if( strcmp(cFunc,"always") == 0 )
        return CMP_ALWAYS;
    else
        return (FLYCMPFUNC)-1;
}


//------------------------------------------------------------------------
// Convert the string to scene blend mode.
//------------------------------------------------------------------------
SCENEBLENDMODE FlyRenderEffectCompiler::GetSceneBlendMode( const char* cBlend )
{
    if( strcmp(cBlend,"zero") == 0 )
        return SBM_ZERO;
    else if( strcmp(cBlend,"one") == 0 )
        return SBM_ONE;
    else if( strcmp(cBlend,"srccolor") == 0 )
        return SBM_SRCCOLOR;
    else if( strcmp(cBlend,"invsrccolor") == 0 )
        return SBM_INVSRCCOLOR;
    else if( strcmp(cBlend,"srcalpha") == 0 )
        return SBM_SRCALPHA;
    else if( strcmp(cBlend,"invsrcalpha") == 0 )
        return SBM_INVSRCALPHA;
    else if( strcmp(cBlend,"destcolor") == 0 )
        return SBM_DESTCOLOR;
    else if( strcmp(cBlend,"invdestcolor") == 0 )
        return SBM_INVDESTCOLOR;
    else if( strcmp(cBlend,"destalpha") == 0 )
        return SBM_DESTALPHA;
    else if( strcmp(cBlend,"invdestalpha") == 0 )
        return SBM_INVDESTALPHA;
    else
        return (SCENEBLENDMODE)-1;
}


//------------------------------------------------------------------------
// Convert the string to the blend source.
//------------------------------------------------------------------------
BLENDSOURCE FlyRenderEffectCompiler::GetBlendSource( const char* cSource )
{
    if( strcmp(cSource,"current") == 0 )
        return BSTA_CURRENT;
    else if( strcmp(cSource,"diffuse") == 0 )
        return BSTA_DIFFUSE;
    else if( strcmp(cSource,"texture") == 0 )
        return BSTA_TEXTURE;
    else if( strcmp(cSource,"specular") == 0 )
        return BSTA_SPECULAR;
    else
        return (BLENDSOURCE)-1;
}


//------------------------------------------------------------------------
// Convert the string to the texture operation.
//------------------------------------------------------------------------
TEXTUREOPERATION FlyRenderEffectCompiler::GetTextureOP( const char* cTOP )
{
    if( strcmp(cTOP,"disable") == 0 )
        return TOP_DISABLE;
    else if( strcmp(cTOP,"selectarg1") == 0 )
        return TOP_SELECTARG1;
    else if( strcmp(cTOP,"selectarg2") == 0 )
        return TOP_SELECTARG2;
    else if( strcmp(cTOP,"modulate") == 0 )
        return TOP_MODULATE;
    else if( strcmp(cTOP,"modulatex2") == 0 )
        return TOP_MODULATEX2;
    else if( strcmp(cTOP,"modulatex4") == 0 )
        return TOP_MODULATEX4;
    else if( strcmp(cTOP,"add") == 0 )
        return TOP_ADD;
    else if( strcmp(cTOP,"addsigned") == 0 )
        return TOP_ADDSIGNED;
    else if( strcmp(cTOP,"addsmooth") == 0 )
        return TOP_ADDSMOOTH;
    else if( strcmp(cTOP,"subtract") == 0 )
        return TOP_SUBTRACT;
    else if( strcmp(cTOP,"blenddiffusealpha") == 0 )
        return TOP_BLENDDIFFUSEALPHA;
    else if( strcmp(cTOP,"blendtexturealpha") == 0 )
        return TOP_BLENDTEXTUREALPHA;
    else if( strcmp(cTOP,"blendcurrentalpha") == 0 )
        return TOP_BLENDCURRENTALPHA;
    else if( strcmp(cTOP,"dotproduct3") == 0 )
        return TOP_DOTPRODUCT3;
    else if( strcmp(cTOP,"blendmanual") == 0 )
        return TOP_BLENDMANUAL;
    else
        return (TEXTUREOPERATION)-1;
}


//------------------------------------------------------------------------
// Get stencil operation enumeration value.
//------------------------------------------------------------------------
STENCILOPERATION FlyRenderEffectCompiler::GetStencilOp( const char* cSTOP )
{
    if( strcmp(cSTOP,"keep") == 0 )
        return STOP_KEEP;
    else if( strcmp(cSTOP,"zero") == 0 )
        return STOP_ZERO;
    else if( strcmp(cSTOP,"replace") == 0 )
        return STOP_REPLACE;
    else if( strcmp(cSTOP,"increment") == 0 )
        return STOP_INCREMENT;
    else if( strcmp(cSTOP,"decrement") == 0 )
        return STOP_DECREMENT;
    else if( strcmp(cSTOP,"invert") == 0 )
        return STOP_INVERT;
    else
        return (STENCILOPERATION)-1;
}


//------------------------------------------------------------------------
// Get the light type from the string.
//------------------------------------------------------------------------
LIGHTTYPE FlyRenderEffectCompiler::GetLightType( const char* cType )
{
    if( strcmp(cType,"point") == 0 )
        return LGT_POINT;
    else if( strcmp(cType,"directional") == 0 )
        return LGT_DIRECTIONAL;
    else if( strcmp(cType,"spot") == 0 )
        return LGT_SPOT;
    else
        return (LIGHTTYPE)-1;
}


//------------------------------------------------------------------------
// Get the fog mode from the string.
//------------------------------------------------------------------------
FOGMODE FlyRenderEffectCompiler::GetFogMode( const char* cMode )
{
    if( strcmp(cMode,"none") == 0 )
        return FOG_NONE;
    else if( strcmp(cMode,"exp") == 0 )
        return FOG_EXP;
    else if( strcmp(cMode,"exp2") == 0 )
        return FOG_EXP2;
    else if( strcmp(cMode,"linear") == 0 )
        return FOG_LINEAR;
    else
        return (FOGMODE)-1;
}


//------------------------------------------------------------------------
// Get the number of parameters.
//------------------------------------------------------------------------
UINT FlyRenderEffectCompiler::GetNumParameters(void)
{
    return m_Parameters.size();
}


//------------------------------------------------------------------------
// Get the number of shader functions.
//------------------------------------------------------------------------
UINT FlyRenderEffectCompiler::GetNumShaderFuncs(void)
{
    return m_Shader.funcs.size();
}


//------------------------------------------------------------------------
// Get the number of techniques.
//------------------------------------------------------------------------
UINT FlyRenderEffectCompiler::GetNumTechniques(void)
{
    return m_Techniques.size();
}


//------------------------------------------------------------------------
// Get the compile error info for the user!
//------------------------------------------------------------------------
const char* FlyRenderEffectCompiler::GetErrorInfo(void)
{
    return m_strError.c_str();
}


//------------------------------------------------------------------------
// Generate the shader text for the engine to create shader.
// ----------------------------------------------------------------------
// Param -> IN:
//      std::string&:   String object to store the shader text.
//------------------------------------------------------------------------
void FlyRenderEffectCompiler::GenShaderText( std::string& shader_text )
{
    std::stringstream ss;

    for( int i=0;i<m_Parameters.size();i++ )
    {
        FX_Parameter* param = &m_Parameters[i];

        ss << GetDataType(param->Type) << " " << param->name;
        if( param->nArraySize > 1 )
        {
            ss << "[" << param->nArraySize << "]";
        }

        ss << ";" << std::endl;
    }

    // read the Shader code.
    for( i=0;i<m_Shader.shader_code.size();i++ )
    {
        ss << m_Shader.shader_code[i].c_str() << std::endl;
    }

    shader_text = ss.str();
}


//------------------------------------------------------------------------
// Get the parameter pointer from the compile result.
//------------------------------------------------------------------------
FX_Parameter* FlyRenderEffectCompiler::GetParameter( int nIndex )
{
    if( nIndex < 0 && nIndex >= m_Parameters.size() )
        return NULL;

    return &m_Parameters[nIndex];
}


//------------------------------------------------------------------------
// Get the technique pointer from the compile result.
//------------------------------------------------------------------------
FX_Technique* FlyRenderEffectCompiler::GetTechnique( int nIndex )
{
    if( nIndex < 0 && nIndex >= m_Parameters.size() )
        return NULL;

    return &m_Techniques[nIndex];
}