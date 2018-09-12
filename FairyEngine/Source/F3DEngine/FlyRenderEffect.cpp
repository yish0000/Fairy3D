//------------------------------------------------------------------------
// Name: FlyRenderEffect.cpp
// Desc: This file is to implement the FlyRenderEffect module.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyRenderEffect.h"
#include "FlyRenderEffectCompiler.h"

#include "FlyLogManager.h"


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYRENDERVARIABLE                           *
*                                                                        *
*************************************************************************/

//------------------------------------------------------------------------
// Constructor and destructor of the class.
//------------------------------------------------------------------------
FlyRenderAnnotation::FlyRenderAnnotation(void)
{
    m_pVar = NULL;
}

FlyRenderAnnotation::~FlyRenderAnnotation(void)
{
    SAFE_DELETE( m_pVar );
}


//------------------------------------------------------------------------
// Get the annotation's name.
//------------------------------------------------------------------------
const char* FlyRenderAnnotation::GetName(void)
{
    return m_sName.c_str();
}

//------------------------------------------------------------------------
// Get the annotation's type.
//------------------------------------------------------------------------
REDATATYPE FlyRenderAnnotation::GetType(void)
{
    return m_Type;
}

//------------------------------------------------------------------------
// Get the annotation's variable pointer.
//------------------------------------------------------------------------
FlyRenderVariable* FlyRenderAnnotation::GetVariable(void)
{
    return m_pVar;
}

//------------------------------------------------------------------------
// Set the annotation's name.
//------------------------------------------------------------------------
void FlyRenderAnnotation::SetName( const char* cName )
{
    m_sName = cName;
}

//------------------------------------------------------------------------
// Set the annotation's variable.
//------------------------------------------------------------------------
void FlyRenderAnnotation::SetVariable( REDATATYPE Type,FlyRenderVariable* pVar )
{
    SAFE_DELETE( m_pVar );

    switch( Type )
    {
    case REDT_BOOL:
        if( pVar )
            m_pVar = pVar->Clone();
        else
            m_pVar = new RenderVariableBool;
        break;
    case REDT_INT:
    case REDT_DWORD:
        if( pVar )
            m_pVar = pVar->Clone();
        else
            m_pVar = new RenderVariableInt;
        break;
    case REDT_STRING:
        if( pVar )
            m_pVar = pVar->Clone();
        else
            m_pVar = new RenderVariableString;
        break;
    case REDT_FLOAT:
        if( pVar )
            m_pVar = pVar->Clone();
        else
            m_pVar = new RenderVariableFloat;
        break;
    case REDT_FLOAT2:
        if( pVar )
            m_pVar = pVar->Clone();
        else
            m_pVar = new RenderVariableFloat2;
        break;
    case REDT_FLOAT3:
        if( pVar )
            m_pVar = pVar->Clone();
        else
            m_pVar = new RenderVariableFloat3;
        break;
    case REDT_FLOAT4:
        if( pVar )
            m_pVar = pVar->Clone();
        else
            m_pVar = new RenderVariableVector;
        break;
    case REDT_FLOAT4X4:
        if( pVar )
            m_pVar = pVar->Clone();
        else
            m_pVar = new RenderVariableMatrix;
        break;
    }

    m_Type = Type;
}


//------------------------------------------------------------------------
// Override the operator = to give the value for annotation.
//------------------------------------------------------------------------
FlyRenderAnnotation& FlyRenderAnnotation::operator = ( const bool& value )
{
    *m_pVar = value;
    return *this;
}

FlyRenderAnnotation& FlyRenderAnnotation::operator = ( const int& value )
{
    *m_pVar = value;
    return *this;
}

FlyRenderAnnotation& FlyRenderAnnotation::operator = ( const std::string& value )
{
    *m_pVar = value;
    return *this;
}

FlyRenderAnnotation& FlyRenderAnnotation::operator = ( const float& value )
{
    *m_pVar = value;
    return *this;
}

FlyRenderAnnotation& FlyRenderAnnotation::operator = ( const float2& value )
{
    *m_pVar = value;
    return *this;
}

FlyRenderAnnotation& FlyRenderAnnotation::operator = ( const float3& value )
{
    *m_pVar = value;
    return *this;
}

FlyRenderAnnotation& FlyRenderAnnotation::operator = ( const FlyVector& value )
{
    *m_pVar = value;
    return *this;
}

FlyRenderAnnotation& FlyRenderAnnotation::operator = ( const FlyMatrix& value )
{
    *m_pVar = value;
    return *this;
}


//------------------------------------------------------------------------
// Get the value of the annotation.
//------------------------------------------------------------------------
void FlyRenderAnnotation::Value( bool& value ) const
{
    m_pVar->Value( value );
}

void FlyRenderAnnotation::Value( int& value ) const
{
    m_pVar->Value( value );
}

void FlyRenderAnnotation::Value( std::string& value ) const
{
    m_pVar->Value( value );
}

void FlyRenderAnnotation::Value( float& value ) const
{
    m_pVar->Value( value );
}

void FlyRenderAnnotation::Value( float2& value ) const
{
    m_pVar->Value( value );
}

void FlyRenderAnnotation::Value( float3& value ) const
{
    m_pVar->Value( value );
}

void FlyRenderAnnotation::Value( FlyVector& value ) const
{
    m_pVar->Value( value );
}

void FlyRenderAnnotation::Value( FlyMatrix& value ) const
{
    m_pVar->Value( value );
}


//------------------------------------------------------------------------
// Get the pointer to the variable data.
//------------------------------------------------------------------------
void FlyRenderAnnotation::Pointer( bool*& ptr ) const
{
    m_pVar->Pointer( ptr );
}

void FlyRenderAnnotation::Pointer( int*& ptr ) const
{
    m_pVar->Pointer( ptr );
}

void FlyRenderAnnotation::Pointer( std::string*& ptr ) const
{
    m_pVar->Pointer( ptr );
}

void FlyRenderAnnotation::Pointer( float*& ptr ) const
{
    m_pVar->Pointer( ptr );
}

void FlyRenderAnnotation::Pointer( float2*& ptr ) const
{
    m_pVar->Pointer( ptr );
}

void FlyRenderAnnotation::Pointer( float3*& ptr ) const
{
    m_pVar->Pointer( ptr );
}

void FlyRenderAnnotation::Pointer( FlyVector*& ptr ) const
{
    m_pVar->Pointer( ptr );
}

void FlyRenderAnnotation::Pointer( FlyMatrix*& ptr ) const
{
    m_pVar->Pointer( ptr );
}


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYRENDERPARAMETER                          *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor and destructor of the class
//------------------------------------------------------------------------
FlyRenderParameter::FlyRenderParameter( FlyRenderEffect* pEffect )
{
    m_pEffect = pEffect;
    m_pVariable = NULL;
}

FlyRenderParameter::~FlyRenderParameter(void)
{
    SAFE_DELETE( m_pVariable );

    for( size_t i=0;i<m_Annotations.size();i++ )
        SAFE_DELETE( m_Annotations[i] );
}


//------------------------------------------------------------------------
// Clone a new parameter instance for us.
//------------------------------------------------------------------------
FlyRenderParameter* FlyRenderParameter::Clone(void)
{
    FlyRenderParameter* param = new FlyRenderParameter( m_pEffect );

    param->m_sName = m_sName;
    param->m_sSemantic = m_sSemantic;
    param->m_Type = m_Type;
    param->m_pVariable = m_pVariable->Clone();
    param->m_nArraySize = m_nArraySize;
    param->m_bAutoUpdate = m_bAutoUpdate;
    param->m_AutoType = m_AutoType;

    for( size_t i=0;i<m_Annotations.size();i++ )
    {
        FlyRenderAnnotation* p = m_Annotations[i];

        FlyRenderAnnotation* pAnno = new FlyRenderAnnotation();
        pAnno->SetName( p->GetName() );
        pAnno->SetVariable( p->GetType(),p->GetVariable() );
        param->m_Annotations.push_back( pAnno );
    }

    return param;
}


//------------------------------------------------------------------------
// Copy another parameter's data to me.
//------------------------------------------------------------------------
void FlyRenderParameter::CopyVariableData( FlyRenderParameter* parameter )
{
    m_pVariable->CopyVariableData( parameter->m_pVariable );
}


//------------------------------------------------------------------------
// Add an annotation for the render parameter.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the annotation.
//------------------------------------------------------------------------
FlyRenderAnnotation* FlyRenderParameter::AddAnnotation( const char* cName )
{
    // Check whether the annotation existed ?
    for( size_t i=0;i<m_Annotations.size();i++ )
    {
        if( strcmp(m_Annotations[i]->GetName(),cName) == 0 )
            return m_Annotations[i];
    }

    // Create a new one.
    FlyRenderAnnotation* pAnnotation = new FlyRenderAnnotation();
    pAnnotation->SetName( cName );
    m_Annotations.push_back( pAnnotation );
    return pAnnotation;
}


//------------------------------------------------------------------------
// Set the name for the parameter.
//------------------------------------------------------------------------
void FlyRenderParameter::SetName( const char* cName )
{
    m_sName = cName;
}

//------------------------------------------------------------------------
// Set the semantic for the parameter.
//------------------------------------------------------------------------
void FlyRenderParameter::SetSemantic( const char* cSemantic )
{
    m_sSemantic = cSemantic;
}


//------------------------------------------------------------------------
// Set the data type for the parameter.
// ----------------------------------------------------------------------
// Param -> IN:
//      REDATATYPE:         Type of the render variable.
//      UINT:               Array size of the render variable.
//      FlyRenderVariable*: Pointer to the render variable.
//------------------------------------------------------------------------
void FlyRenderParameter::SetVariable( REDATATYPE type,UINT nSize,FlyRenderVariable* pVar )
{
    // Delete the old var.
    SAFE_DELETE( m_pVariable );

    switch( type )
    {
    case REDT_BOOL:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableBool;
        }
        else
        {
            m_pVariable = new RenderVariableBoolArray;

            std::vector<bool> arr;
            arr.resize( nSize );
            *m_pVariable = arr;
        }
        break;
    case REDT_INT:
    case REDT_DWORD:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableInt;
        }
        else
        {
            m_pVariable = new RenderVariableIntArray;

            std::vector<int> arr;
            arr.resize( nSize );
            *m_pVariable = arr;
        }
        break;
    case REDT_STRING:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableString;
        }
        break;
    case REDT_SAMPLER2D:
    case REDT_SAMPLER3D:
    case REDT_SAMPLERCUBE:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableSampler;
        }
        break;
    case REDT_FLOAT:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableFloat;
        }
        else
        {
            m_pVariable = new RenderVariableFloatArray;

            std::vector<float> arr;
            arr.resize( nSize );
            *m_pVariable = arr;
        }
        break;
    case REDT_FLOAT2:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableFloat2;
        }
        break;
    case REDT_FLOAT3:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableFloat3;
        }
        break;
    case REDT_FLOAT4:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableVector;
        }
        else
        {
            m_pVariable = new RenderVariableVectorArray;

            std::vector<FlyVector> arr;
            arr.resize( nSize );
            *m_pVariable = arr;
        }
        break;
    case REDT_FLOAT4X4:
        if( nSize == 1 )
        {
            if( pVar )
                m_pVariable = pVar->Clone();
            else
                m_pVariable = new RenderVariableMatrix;
        }
        else
        {
            m_pVariable = new RenderVariableMatrixArray;

            std::vector<FlyMatrix> arr;
            arr.resize( nSize );
            *m_pVariable = arr;
        }
        break;
    }

    m_Type = type;
    m_nArraySize = nSize;
}


//------------------------------------------------------------------------
// Get the number of the annotations.
//------------------------------------------------------------------------
size_t FlyRenderParameter::GetNumAnnotations(void) const
{
    return m_Annotations.size();
}

//------------------------------------------------------------------------
// Get the the annotation pointer by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the annotation.
//------------------------------------------------------------------------
FlyRenderAnnotation* FlyRenderParameter::GetAnnotationByIndex( size_t nIndex )
{
    if( nIndex >= m_Annotations.size() )
        return NULL;

    return m_Annotations[nIndex];
}

//------------------------------------------------------------------------
// Get the annotation pointer by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the annotation.
//------------------------------------------------------------------------
FlyRenderAnnotation* FlyRenderParameter::GetAnnotationByName( const char* cName )
{
    for( size_t i=0;i<m_Annotations.size();i++ )
    {
        if( strcmp(m_Annotations[i]->GetName(),cName) == 0 )
            return m_Annotations[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the name of the render parameter.
//------------------------------------------------------------------------
const char* FlyRenderParameter::GetName(void)
{
    return m_sName.c_str();
}

//------------------------------------------------------------------------
// Get the semantic of the render parameter.
//------------------------------------------------------------------------
const char* FlyRenderParameter::GetSemantic(void)
{
    return m_sSemantic.c_str();
}

//------------------------------------------------------------------------
// Get the data type of the render parameter.
//------------------------------------------------------------------------
REDATATYPE FlyRenderParameter::GetDataType(void)
{
    return m_Type;
}

//------------------------------------------------------------------------
// Get the array size of the render parameter.
//------------------------------------------------------------------------
UINT FlyRenderParameter::GetArraySize(void)
{
    return m_nArraySize;
}

//------------------------------------------------------------------------
// Get the RenderEffect does the parameter own to.
//------------------------------------------------------------------------
FlyRenderEffect* FlyRenderParameter::GetRenderEffect(void)
{
    return m_pEffect;
}


//------------------------------------------------------------------------
// Give the value for the render parameter.
//------------------------------------------------------------------------
FlyRenderParameter& FlyRenderParameter::operator = ( const bool& value )
{
    *m_pVariable = value;
    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const int& value )
{
    *m_pVariable = value;
    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const float& value )
{
    *m_pVariable = value;
    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const float2& value )
{
    *m_pVariable = value;
    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const float3& value )
{
    *m_pVariable = value;
    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const FlyVector& value )
{
    *m_pVariable = value;
    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const FlyMatrix& value )
{
    *m_pVariable = value;
    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const FlySampler& value )
{
    *m_pVariable = value;
    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const BoolArray& value )
{
    if( m_nArraySize == value.size() )
        *m_pVariable = value;

    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const IntArray& value )
{
    if( m_nArraySize == value.size() )
        *m_pVariable = value;

    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const FloatArray& value )
{
    if( m_nArraySize == value.size() )
        *m_pVariable = value;

    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const VectorArray& value )
{
    if( m_nArraySize == value.size() )
        *m_pVariable = value;

    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const MatrixArray& value )
{
    if( m_nArraySize == value.size() )
        *m_pVariable = value;

    return *this;
}

FlyRenderParameter& FlyRenderParameter::operator = ( const FlyTexture* value )
{
    FlySampler* samp;
    m_pVariable->Pointer( samp );
    samp->pTexture = (FlyTexture*)value;
    return *this;
}


//------------------------------------------------------------------------
// Get the value from the render parameter.
//------------------------------------------------------------------------
void FlyRenderParameter::Value( bool& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( int& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( float& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( float2& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( float3& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( FlyVector& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( FlyMatrix& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( FlySampler& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( BoolArray& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( IntArray& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( FloatArray& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( VectorArray& value ) const
{
    m_pVariable->Value( value );
}

void FlyRenderParameter::Value( MatrixArray& value ) const
{
    m_pVariable->Value( value );
}


//------------------------------------------------------------------------
// Functions to get the value from the array parameter.
//------------------------------------------------------------------------
void FlyRenderParameter::GetValue( UINT nIndex,bool* pValue ) const
{
    BoolArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        *pValue = (*ptr)[nIndex];
}

void FlyRenderParameter::GetValue( UINT nIndex,int* pValue ) const
{
    IntArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        *pValue = (*ptr)[nIndex];
}

void FlyRenderParameter::GetValue( UINT nIndex,float* pValue ) const
{
    FloatArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        *pValue = (*ptr)[nIndex];
}

void FlyRenderParameter::GetValue( UINT nIndex,FlyVector* pValue ) const
{
    VectorArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        *pValue = (*ptr)[nIndex];
}

void FlyRenderParameter::GetValue( UINT nIndex,FlyMatrix* pValue ) const
{
    MatrixArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        *pValue = (*ptr)[nIndex];
}


//------------------------------------------------------------------------
// Functions set the value for the array parameters.
//------------------------------------------------------------------------
void FlyRenderParameter::SetValue( UINT nIndex,const bool& value )
{
    BoolArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        (*ptr)[nIndex] = value;
}

void FlyRenderParameter::SetValue( UINT nIndex,const int& value )
{
    IntArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        (*ptr)[nIndex] = value;
}

void FlyRenderParameter::SetValue( UINT nIndex,const float& value )
{
    FloatArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        (*ptr)[nIndex] = value;
}

void FlyRenderParameter::SetValue( UINT nIndex,const FlyVector& value )
{
    VectorArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        (*ptr)[nIndex] = value;
}

void FlyRenderParameter::SetValue( UINT nIndex,const FlyMatrix& value )
{
    MatrixArray* ptr = NULL;
    m_pVariable->Pointer( ptr );

    if( ptr && nIndex < m_nArraySize )
        (*ptr)[nIndex] = value;
}

//------------------------------------------------------------------------
// Get the pointer to the parameter data.
//------------------------------------------------------------------------
void FlyRenderParameter::Pointer( bool*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( int*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( float*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( float2*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( float3*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( FlyVector*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( FlyMatrix*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( FlySampler*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( BoolArray*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( IntArray*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( FloatArray*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( VectorArray*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}

void FlyRenderParameter::Pointer( MatrixArray*& ptr ) const
{
    m_pVariable->Pointer( ptr );
}


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYRENDEREFFECT                             *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor and of the class.
//------------------------------------------------------------------------
FlyRenderEffect::FlyRenderEffect(void)
{
}


//------------------------------------------------------------------------
// Destructor of the class.
//------------------------------------------------------------------------
FlyRenderEffect::~FlyRenderEffect(void)
{
    for( size_t i=0;i<m_Parameters.size();i++ )
        SAFE_DELETE( m_Parameters[i] );

    for( size_t i=0;i<m_Techniques.size();i++ )
        SAFE_DELETE( m_Techniques[i] );
}


//------------------------------------------------------------------------
// Reset the render effect data.
//------------------------------------------------------------------------
void FlyRenderEffect::Reset(void)
{
    size_t i = 0;
    for( ;i<m_Parameters.size();i++ )
        SAFE_DELETE( m_Parameters[i] );

    for( i=0;i<m_Techniques.size();i++ )
        SAFE_DELETE( m_Techniques[i] );

    m_Parameters.clear();
    m_ShaderText.resize( 0 );
    m_Techniques.clear();
}


//------------------------------------------------------------------------
// Load the Render effect data from the FFX file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the FFX file.
//------------------------------------------------------------------------
HRESULT FlyRenderEffect::LoadEffect( const char* cFilename )
{
    size_t i;
    std::string text;
    FX_Header header;
    size_t len;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    FILE* pFile = fopen( cFilename,"rb" );
    if( !pFile ) return FLY_INVALIDPARAM;

    // Reader the file header.
    fread( &header,sizeof(FX_Header),1,pFile );
    if( strcmp(header.fourcc,"FFX") != 0 && header.version != FLY_VERSION(1,0) )
    {
        fclose( pFile );
        return FLY_INVALIDFILE;
    }

    // Clean the old data.
    Reset();

    // 1. Read the parameters.
    for( i=0;i<header.num_parameters;i++ )
    {
        REDATATYPE type;
        UINT nArraySize;

        fread( &len,sizeof(size_t),1,pFile );
        text.resize( len,'\0' );
        fread( &text[0],len,1,pFile );

        // Add the new parameter.
        FlyRenderParameter* param = new FlyRenderParameter( this );
        param->SetName( text.c_str() );

        fread( &len,sizeof(size_t),1,pFile );
        text.resize( len,'\0' );
        fread( &text[0],len,1,pFile );
        param->SetSemantic( text.c_str() );

        fread( &type,sizeof(REDATATYPE),1,pFile );
        fread( &nArraySize,sizeof(UINT),1,pFile );
        param->SetVariable( type,nArraySize,NULL );

        bool bAuto;
        AUTOPARAMETERTYPE autoType;
        fread( &bAuto,sizeof(bool),1,pFile );
        fread( &autoType,sizeof(AUTOPARAMETERTYPE),1,pFile );
        param->SetAutoUpdate( bAuto );
        param->SetAutoUpdateType( autoType );

        if( nArraySize == 1 )
            ReaderVariable( param,pFile );

        // Add the annotations.
        size_t num_annotations;
        fread( &num_annotations,sizeof(size_t),1,pFile );
        for( size_t j=0;j<num_annotations;j++ )
        {
            FlyRenderAnnotation* pAnnotation;

            fread( &len,sizeof(size_t),1,pFile );
            text.resize( len,'\0' );
            fread( &text[0],len,1,pFile );
            pAnnotation = param->AddAnnotation( text.c_str() );
            ReadAnnotationValue( pAnnotation,pFile );
        }

        m_Parameters.push_back( param );
    }

    // 2. Read the shader text.
    std::stringstream ss;

    for( i=0;i<m_Parameters.size();i++ )
    {
        FlyRenderParameter* param = m_Parameters[i];

        ss << GetDataType(param->GetDataType()) << " " << param->GetName();
        if( param->GetArraySize() > 1 )
        {
            ss << "[" << param->GetArraySize() << "]";
        }

        ss << ";" << std::endl;
    }

    fread( &len,sizeof(size_t),1,pFile );
    text.resize( len,'\0' );
    fread( &text[0],len,1,pFile );

    ss << text;
    m_ShaderText = ss.str();

    // 3. Read the techniques.
    for( i=0;i<header.num_techniques;i++ )
    {
        fread( &len,sizeof(size_t),1,pFile );
        text.resize( len,'\0' );
        fread( &text[0],len,1,pFile );

        // Add the new technique.
        FlyRenderTechnique* technique = new FlyRenderTechnique( this );
        technique->SetName( text.c_str() );

        // Add the annotations for the technique.
        size_t num_annotations;
        fread( &num_annotations,sizeof(size_t),1,pFile );
        for( size_t j=0;j<num_annotations;j++ )
        {
            FlyRenderAnnotation* pAnnotation;

            fread( &len,sizeof(size_t),1,pFile );
            text.resize( len,'\0' );
            fread( &text[0],len,1,pFile );
            pAnnotation = technique->AddAnnotation( text.c_str() );
            ReadAnnotationValue( pAnnotation,pFile );
        }

        // Read the render pass datas.
        size_t num_passes;
        fread( &num_passes,sizeof(size_t),1,pFile );
        for( size_t j=0;j<num_passes;j++ )
        {
            FlyRenderPass* pass;
            std::string func,profile;

            fread( &len,sizeof(size_t),1,pFile );
            text.resize( len,'\0' );
            fread( &text[0],len,1,pFile );
            pass = technique->AddPass( text.c_str() );

            // Read the annotations for pass.
            fread( &num_annotations,sizeof(size_t),1,pFile );
            for( size_t a=0;a<num_annotations;a++ )
            {
                FlyRenderAnnotation* pAnnotation;

                fread( &len,sizeof(size_t),1,pFile );
                text.resize( len,'\0' );
                fread( &text[0],len,1,pFile );
                pAnnotation = pass->AddAnnotation( text.c_str() );
                ReadAnnotationValue( pAnnotation,pFile );
            }

            // Read the vertex shader description.
            fread( &len,sizeof(size_t),1,pFile );
            func.resize( len,'\0' );
            fread( &func[0],len,1,pFile );
            fread( &len,sizeof(size_t),1,pFile );
            profile.resize( len,'\0' );
            fread( &profile[0],len,1,pFile );
            pass->SetVShaderDesc( func.c_str(),profile.c_str() );

            // Read the pixel shader description.
            fread( &len,sizeof(size_t),1,pFile );
            func.resize( len,'\0' );
            fread( &func[0],len,1,pFile );
            fread( &len,sizeof(size_t),1,pFile );
            profile.resize( len,'\0' );
            fread( &profile[0],len,1,pFile );
            pass->SetPShaderDesc( func.c_str(),profile.c_str() );

            // Read the render states.
            size_t num_renderstates;
            fread( &num_renderstates,sizeof(size_t),1,pFile );
            for( size_t a=0;a<num_renderstates;a++ )
            {
                int index;
                RSTYPE rsType;
                fread( &rsType,sizeof(RSTYPE),1,pFile );

                // Get the render state buffer pointer.
                FlyRenderStateBuffer* rs = &pass->m_RenderStates;

                switch( rsType )
                {
                case RS_CULLMODE:
                    fread( &rs->CullMode,sizeof(CULLMODE),1,pFile );
                    break;
                case RS_POLYGONMODE:
                    fread( &rs->PolygonMode,sizeof(POLYGONMODE),1,pFile );
                    break;
                case RS_SHADEMODE:
                    fread( &rs->ShadeMode,sizeof(SHADEMODE),1,pFile );
                    break;
                case RS_DEPTHMODE:
                    fread( &rs->DepthMode,sizeof(DEPTHMODE),1,pFile );
                    break;
                case RS_DEPTHFUNC:
                    fread( &rs->DepthFunc,sizeof(FLYCMPFUNC),1,pFile );
                    break;
                case RS_SLOPESCALE:
                    fread( &rs->fSlopeScale,sizeof(float),1,pFile );
                    break;
                case RS_DEPTHBIAS:
                    fread( &rs->fDepthBias,sizeof(float),1,pFile );
                    break;
                case RS_FRONTSTENCILENABLE:
                    fread( &rs->bFrontStencilEnable,sizeof(bool),1,pFile );
                    break;
                case RS_BACKSTENCILENABLE:
                    fread( &rs->bBackStencilEnable,sizeof(bool),1,pFile );
                    break;
                case RS_STENCILFUNC:
                    fread( &rs->StencilFunc,sizeof(FLYCMPFUNC),1,pFile );
                    break;
                case RS_STENCILREF:
                    fread( &rs->nStencilRef,sizeof(int),1,pFile );
                    break;
                case RS_STENCILMASK:
                    fread( &rs->dwStencilMask,sizeof(DWORD),1,pFile );
                    break;
                case RS_STENCILWRITEMASK:
                    fread( &rs->dwStencilWriteMask,sizeof(DWORD),1,pFile );
                    break;
                case RS_STENCILFAIL:
                    fread( &rs->StencilFail,sizeof(STENCILOPERATION),1,pFile );
                    break;
                case RS_STENCILDEPTHFAIL:
                    fread( &rs->StencilDepthFail,sizeof(STENCILOPERATION),1,pFile );
                    break;
                case RS_STENCILPASS:
                    fread( &rs->StencilPass,sizeof(STENCILOPERATION),1,pFile );
                    break;
                case RS_BACKSTENCILFUNC:
                    fread( &rs->BackStencilFunc,sizeof(FLYCMPFUNC),1,pFile );
                    break;
                case RS_BACKSTENCILFAIL:
                    fread( &rs->BackStencilFail,sizeof(STENCILOPERATION),1,pFile );
                    break;
                case RS_BACKSTENCILDEPTHFAIL:
                    fread( &rs->BackStencilDepthFail,sizeof(STENCILOPERATION),1,pFile );
                    break;
                case RS_BACKSTENCILPASS:
                    fread( &rs->BackStencilPass,sizeof(STENCILOPERATION),1,pFile );
                    break;
                case RS_ALPHABLENDENABLE:
                    fread( &rs->bAlphaBlend,sizeof(bool),1,pFile );
                    break;
                case RS_ALPHATESTENABLE:
                    fread( &rs->bAlphaTest,sizeof(bool),1,pFile );
                    break;
                case RS_ALPHATESTFUNC:
                    fread( &rs->AlphaTestFunc,sizeof(FLYCMPFUNC),1,pFile );
                    break;
                case RS_ALPHATESTREF:
                    fread( &rs->dwAlphaTestRef,sizeof(DWORD),1,pFile );
                    break;
                case RS_SOURCEBLEND:
                    fread( &rs->SourceBlend,sizeof(SCENEBLENDMODE),1,pFile );
                    break;
                case RS_DESTBLEND:
                    fread( &rs->DestBlend,sizeof(SCENEBLENDMODE),1,pFile );
                    break;
                case RS_COORDSETINDEX:
                    fread( &index,sizeof(int),1,pFile );
                    fread( &rs->CoordSetIndex[index],sizeof(int),1,pFile );
                    break;
                case RS_TEXCOLORBLEND:
                    fread( &index,sizeof(int),1,pFile );
                    fread( &rs->ColorBlend[index],sizeof(TEXTUREBLENDMODE),1,pFile );
                    break;
                case RS_TEXALPHABLEND:
                    fread( &index,sizeof(int),1,pFile );
                    fread( &rs->AlphaBlend[index],sizeof(TEXTUREBLENDMODE),1,pFile );
                    break;
                case RS_LIGHTING:
                    fread( &rs->bLighting,sizeof(bool),1,pFile );
                    break;
                case RS_SPECULARLIGHT:
                    fread( &rs->bSpecularLight,sizeof(bool),1,pFile );
                    break;
                case RS_STDMATERIAL:
                    fread( &rs->StdMaterial,sizeof(FLYMATERIAL),1,pFile );
                    break;
                }
            }

            // Create the shader for render pass if needed.
            std::string sHlslError;
            if( FAILED(pass->CreateShader(m_ShaderText.c_str(),sHlslError)) )
            {
                logMgr.WriteError( 0,"Create the vertex shader failed! FlyRenderPass::CreateShader() failed!" );
                logMgr.WriteText( 0,sHlslError.c_str() );
                return FLY_COMPILEHLSL;
            }
        }

        m_Techniques.push_back( technique );
    }

    fclose( pFile );
    m_sName = cFilename;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Reader the render variable from the effect file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderParameter*: Pointer to the render parameter.
//      FILE*:      Pointer to the effect file.
//------------------------------------------------------------------------
void FlyRenderEffect::ReaderVariable( FlyRenderParameter* param,FILE* pFile )
{
    switch( param->GetDataType() )
    {
    case REDT_BOOL:
        {
            bool value;
            fread( &value,sizeof(bool),1,pFile );
            *param = value;
        }
        break;
    case REDT_DWORD:
    case REDT_INT:
        {
            int value;
            fread( &value,sizeof(int),1,pFile );
            *param = value;
        }
        break;
    case REDT_SAMPLER2D:
    case REDT_SAMPLER3D:
    case REDT_SAMPLERCUBE:
        {
            FlySampler value;
            fread( &value,sizeof(FlySampler),1,pFile );
            *param = value;
        }
        break;
    case REDT_FLOAT:
        {
            float value;
            fread( &value,sizeof(float),1,pFile );
            *param = value;
        }
        break;
    case REDT_FLOAT2:
        {
            float2 value;
            fread( &value,sizeof(float2),1,pFile );
            *param = value;
        }
        break;
    case REDT_FLOAT3:
        {
            float3 value;
            fread( &value,sizeof(float3),1,pFile );
            *param = value;
        }
        break;
    case REDT_FLOAT4:
        {
            FlyVector value;
            fread( &value,sizeof(FlyVector),1,pFile );
            *param = value;
        }
        break;
    case REDT_FLOAT4X4:
        {
            FlyMatrix value;
            fread( &value,sizeof(FlyMatrix),1,pFile );
            *param = value;
        }
        break;
    }
}


//------------------------------------------------------------------------
// Read the annotation value from the effect file.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderAnnotation*:   Pointer to the annotation.
//      FILE*:                  Pointer to the file.
//------------------------------------------------------------------------
void FlyRenderEffect::ReadAnnotationValue( FlyRenderAnnotation* annotation,FILE* pFile )
{
    REDATATYPE type;
    fread( &type,sizeof(REDATATYPE),1,pFile );
    annotation->SetVariable( type,NULL );

    switch( annotation->GetType() )
    {
    case REDT_BOOL:
        {
            bool value;
            fread( &value,sizeof(bool),1,pFile );
            *annotation = value;
        }
        break;
    case REDT_DWORD:
    case REDT_INT:
        {
            int value;
            fread( &value,sizeof(int),1,pFile );
            *annotation = value;
        }
        break;
    case REDT_STRING:
        {
            size_t len;
            std::string value;
            fread( &len,sizeof(size_t),1,pFile );
            value.resize( len,'\0' );
            fread( &value[0],len,1,pFile );
            *annotation = value;
        }
        break;
    case REDT_FLOAT:
        {
            float value;
            fread( &value,sizeof(float),1,pFile );
            *annotation = value;
        }
        break;
    case REDT_FLOAT2:
        {
            float2 value;
            fread( &value,sizeof(float2),1,pFile );
            *annotation = value;
        }
        break;
    case REDT_FLOAT3:
        {
            float3 value;
            fread( &value,sizeof(float3),1,pFile );
            *annotation = value;
        }
        break;
    case REDT_FLOAT4:
        {
            FlyVector value;
            fread( &value,sizeof(FlyVector),1,pFile );
            *annotation = value;
        }
        break;
    case REDT_FLOAT4X4:
        {
            FlyMatrix value;
            fread( &value,sizeof(FlyMatrix),1,pFile );
            *annotation = value;
        }
        break;
    }
}


//------------------------------------------------------------------------
// Load the Render effect from the XML source file.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Path to the xml source file.
//------------------------------------------------------------------------
HRESULT FlyRenderEffect::LoadEffectFromXML( const char* cXmlDoc )
{
    FlyRenderEffectCompiler compiler;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // Load the xml source file.
    if( FAILED(compiler.CompileFromFile(cXmlDoc)) )
    {
        logMgr.WriteError( 0,"Load the xml source file error!" );
        logMgr.WriteText( 0,compiler.GetErrorInfo() );
        return FLY_COMPILEEFFECT;
    }

    m_sName = cXmlDoc;

    // Clear the old datas.
    Reset();

    return GenerateEffect( &compiler );
}


//------------------------------------------------------------------------
// Compile the RenderEffect's source file to the binary format.
// ----------------------------------------------------------------------
// Param -> IN:
//      const void*:    Pointer to the source content.
//      char*:          To store the error info.
//------------------------------------------------------------------------
HRESULT FlyRenderEffect::Compile( const void* pBuffer )
{
    FlyRenderEffectCompiler compiler;

    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // Compile the xml string from the memory.
    if( FAILED(compiler.CompileFromBuffer((const char*)pBuffer)) )
    {
        logMgr.WriteError( 0,"Compile the xml source file error!" );
        logMgr.WriteText( 0,compiler.GetErrorInfo() );
        return FLY_COMPILEEFFECT;
    }

    // Clear the old datas.
    Reset();

    return GenerateEffect( &compiler );
}


//------------------------------------------------------------------------
// Generate the effect data from the compile result.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderEffectCompiler*:   Pointer to the effect compiler.
//------------------------------------------------------------------------
HRESULT FlyRenderEffect::GenerateEffect( FlyRenderEffectCompiler* compiler )
{
    // Get the logManager.
    FlyLogManager& logMgr = FlyLogManager::GetSingleton();

    // 1. Setup the parameters.
    UINT i = 0;
    for( ;i<compiler->GetNumParameters();i++ )
    {
        FX_Parameter* pParam = compiler->GetParameter( i );

        FlyRenderParameter* param = new FlyRenderParameter( this );
        param->SetName( pParam->name.c_str() );
        param->SetSemantic( pParam->semantic.c_str() );
        param->SetVariable( pParam->Type,pParam->nArraySize,pParam->pVar );
        param->SetAutoUpdate( pParam->bAuto );
        param->SetAutoUpdateType( (AUTOPARAMETERTYPE)pParam->autoType );

        // Add the annotations.
        for( size_t j=0;j<pParam->annotations.size();j++ )
        {
            FX_Annotation* pAnno = &pParam->annotations[j];
            FlyRenderAnnotation* anno = param->AddAnnotation( pAnno->name.c_str() );
            anno->SetVariable( pAnno->Type,pAnno->pVar );
        }

        m_Parameters.push_back( param );
    }

    // 2. Setup the shader code.
    compiler->GenShaderText( m_ShaderText );

    // 3. Setup the techniques.
    for( i=0;i<compiler->GetNumTechniques();i++ )
    {
        FX_Technique* pTech = compiler->GetTechnique( i );

        FlyRenderTechnique* tech = new FlyRenderTechnique( this );
        tech->SetName( pTech->name.c_str() );

        // Add the annotations.
        size_t j = 0;
        for( ;j<pTech->annotations.size();j++ )
        {
            FX_Annotation* pAnno = &pTech->annotations[j];
            FlyRenderAnnotation* anno = tech->AddAnnotation( pAnno->name.c_str() );
            anno->SetVariable( pAnno->Type,pAnno->pVar );
        }

        // Add the render passes.
        for( j=0;j<pTech->passes.size();j++ )
        {
            FX_Pass* pPass = &pTech->passes[j];
            FlyRenderPass* pass = tech->AddPass( pPass->name.c_str() );

            // Add the annotations.
            for( size_t a=0;a<pPass->annotations.size();a++ )
            {
                FX_Annotation* pAnno = &pTech->passes[j].annotations[a];
                FlyRenderAnnotation* anno = pass->AddAnnotation( pAnno->name.c_str() );
                anno->SetVariable( pAnno->Type,pAnno->pVar );
            }

            // Set the shader description.
            pass->SetVShaderDesc( pPass->shaderDescs[0].sFuncName.c_str(),pPass->shaderDescs[0].sProfile.c_str() );
            pass->SetPShaderDesc( pPass->shaderDescs[1].sFuncName.c_str(),pPass->shaderDescs[1].sProfile.c_str() );

            // Set the render state values.
            for( size_t r=0;r<pPass->render_states.size();r++ )
            {
                FlyRenderStateBuffer* rsBuf = &pass->m_RenderStates;
                FX_RenderState* rs = &pTech->passes[j].render_states[r];

                switch( rs->Type )
                {
                case RS_CULLMODE:
                    {
                        CULLMODE value = *(CULLMODE*)rs->pStateData;
                        rsBuf->CullMode = value;
                    }
                    break;
                case RS_POLYGONMODE:
                    {
                        POLYGONMODE value = *(POLYGONMODE*)rs->pStateData;
                        rsBuf->PolygonMode = value;
                    }
                    break;
                case RS_SHADEMODE:
                    {
                        SHADEMODE value = *(SHADEMODE*)rs->pStateData;
                        rsBuf->ShadeMode = value;
                    }
                    break;
                case RS_DEPTHMODE:
                    {
                        DEPTHMODE value = *(DEPTHMODE*)rs->pStateData;
                        rsBuf->DepthMode = value;
                    }
                    break;
                case RS_DEPTHFUNC:
                    {
                        FLYCMPFUNC value = *(FLYCMPFUNC*)rs->pStateData;
                        rsBuf->DepthFunc = value;
                    }
                    break;
                case RS_SLOPESCALE:
                    {
                        float value = *(float*)rs->pStateData;
                        rsBuf->fSlopeScale = value;
                    }
                    break;
                case RS_DEPTHBIAS:
                    {
                        float value = *(float*)rs->pStateData;
                        rsBuf->fDepthBias = value;
                    }
                    break;
                case RS_FRONTSTENCILENABLE:
                    {
                        bool value = *(bool*)rs->pStateData;
                        rsBuf->bFrontStencilEnable = value;
                    }
                    break;
                case RS_BACKSTENCILENABLE:
                    {
                        bool value = *(bool*)rs->pStateData;
                        rsBuf->bBackStencilEnable = value;
                    }
                    break;
                case RS_STENCILFUNC:
                    {
                        FLYCMPFUNC value = *(FLYCMPFUNC*)rs->pStateData;
                        rsBuf->StencilFunc = value;
                    }
                    break;
                case RS_STENCILREF:
                    {
                        int value = *(int*)rs->pStateData;
                        rsBuf->nStencilRef = value;
                    }
                    break;
                case RS_STENCILMASK:
                    {
                        DWORD value = *(DWORD*)rs->pStateData;
                        rsBuf->dwStencilMask = value;
                    }
                    break;
                case RS_STENCILWRITEMASK:
                    {
                        DWORD value = *(DWORD*)rs->pStateData;
                        rsBuf->dwStencilWriteMask = value;
                    }
                    break;
                case RS_STENCILFAIL:
                    {
                        STENCILOPERATION value = *(STENCILOPERATION*)rs->pStateData;
                        rsBuf->StencilFail = value;
                    }
                    break;
                case RS_STENCILDEPTHFAIL:
                    {
                        STENCILOPERATION value = *(STENCILOPERATION*)rs->pStateData;
                        rsBuf->StencilDepthFail = value;
                    }
                    break;
                case RS_STENCILPASS:
                    {
                        STENCILOPERATION value = *(STENCILOPERATION*)rs->pStateData;
                        rsBuf->StencilPass = value;
                    }
                    break;
                case RS_BACKSTENCILFUNC:
                    {
                        FLYCMPFUNC value = *(FLYCMPFUNC*)rs->pStateData;
                        rsBuf->BackStencilFunc = value;
                    }
                    break;
                case RS_BACKSTENCILFAIL:
                    {
                        STENCILOPERATION value = *(STENCILOPERATION*)rs->pStateData;
                        rsBuf->BackStencilFail = value;
                    }
                    break;
                case RS_BACKSTENCILDEPTHFAIL:
                    {
                        STENCILOPERATION value = *(STENCILOPERATION*)rs->pStateData;
                        rsBuf->BackStencilDepthFail = value;
                    }
                    break;
                case RS_BACKSTENCILPASS:
                    {
                        STENCILOPERATION value = *(STENCILOPERATION*)rs->pStateData;
                        rsBuf->BackStencilPass = value;
                    }
                    break;
                case RS_ALPHABLENDENABLE:
                    {
                        bool value = *(bool*)rs->pStateData;
                        rsBuf->bAlphaBlend = value;
                    }
                    break;
                case RS_ALPHATESTENABLE:
                    {
                        bool value = *(bool*)rs->pStateData;
                        rsBuf->bAlphaTest = value;
                    }
                    break;
                case RS_ALPHATESTFUNC:
                    {
                        FLYCMPFUNC value = *(FLYCMPFUNC*)rs->pStateData;
                        rsBuf->AlphaTestFunc = value;
                    }
                    break;
                case RS_ALPHATESTREF:
                    {
                        DWORD value = *(DWORD*)rs->pStateData;
                        rsBuf->dwAlphaTestRef = value;
                    }
                    break;
                case RS_SOURCEBLEND:
                    {
                        SCENEBLENDMODE value = *(SCENEBLENDMODE*)rs->pStateData;
                        rsBuf->SourceBlend = value;
                    }
                    break;
                case RS_DESTBLEND:
                    {
                        SCENEBLENDMODE value = *(SCENEBLENDMODE*)rs->pStateData;
                        rsBuf->DestBlend = value;
                    }
                    break;
                case RS_COORDSETINDEX:
                    {
                        int value = *(int*)rs->pStateData;
                        rsBuf->CoordSetIndex[rs->nIndex] = value;
                    }
                    break;
                case RS_TEXCOLORBLEND:
                    {
                        TEXTUREBLENDMODE value = *(TEXTUREBLENDMODE*)rs->pStateData;
                        rsBuf->ColorBlend[rs->nIndex] = value;
                    }
                    break;
                case RS_TEXALPHABLEND:
                    {
                        TEXTUREBLENDMODE value = *(TEXTUREBLENDMODE*)rs->pStateData;
                        rsBuf->AlphaBlend[rs->nIndex] = value;
                    }
                    break;
                case RS_LIGHTING:
                    {
                        bool value = *(bool*)rs->pStateData;
                        rsBuf->bLighting = value;
                    }
                    break;
                case RS_SPECULARLIGHT:
                    {
                        bool value = *(bool*)rs->pStateData;
                        rsBuf->bSpecularLight = value;
                    }
                    break;
                case RS_STDMATERIAL:
                    {
                        FLYMATERIAL value = *(FLYMATERIAL*)rs->pStateData;
                        rsBuf->StdMaterial = value;
                    }
                    break;
                }
            }

            // Create the shader for the pass.
            std::string sHlslError;
            if( FAILED(pass->CreateShader(m_ShaderText.c_str(),sHlslError)) )
            {
                logMgr.WriteError( 0,"Create the vertex shader failed! FlyRenderPass::CreateShader() failed!" );
                logMgr.WriteText( 0,sHlslError.c_str() );
                return FLY_COMPILEHLSL;
            }
        }

        // Add the new technique to list.
        m_Techniques.push_back( tech );
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Set the parameter data for the renderEffect.
// ----------------------------------------------------------------------
// Param -> IN:
//      int:                    Index of the parameter.
//      FlyRenderParameter*:    Pointer to the parameter data.
//------------------------------------------------------------------------
void FlyRenderEffect::SetParameterData( size_t nIndex,FlyRenderParameter* pParam )
{
    m_Parameters[nIndex]->CopyVariableData( pParam );
}


//------------------------------------------------------------------------
// Get the render parameter by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the render parameter.
//------------------------------------------------------------------------
FlyRenderParameter* FlyRenderEffect::GetParameterByIndex( size_t nIndex )
{
    if( nIndex >= m_Parameters.size() )
        return NULL;

    return m_Parameters[nIndex];
}


//------------------------------------------------------------------------
// Get the render parameter by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the render parameter.
//------------------------------------------------------------------------
FlyRenderParameter* FlyRenderEffect::GetParameterByName( const char* cName )
{
    for( size_t i=0;i<m_Parameters.size();i++ )
    {
        if( strcmp(m_Parameters[i]->GetName(),cName) == 0 )
            return m_Parameters[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the render parameter by its semantic.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    String to the parameter's semantic.
//------------------------------------------------------------------------
FlyRenderParameter* FlyRenderEffect::GetParameterBySemantic( const char* cSemantic )
{
    for( size_t i=0;i<m_Parameters.size();i++ )
    {
        if( strcmp(m_Parameters[i]->GetName(),cSemantic) == 0 )
            return m_Parameters[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the render parameter by its index.
// ----------------------------------------------------------------------
// Param -> IN:
//      size_t:     Index of the render parameter.
//------------------------------------------------------------------------
FlyRenderTechnique* FlyRenderEffect::GetTechniqueByIndex( size_t nIndex )
{
    if( nIndex >= m_Techniques.size() )
        return NULL;

    return m_Techniques[nIndex];
}


//------------------------------------------------------------------------
// Get the render parameter by its name.
// ----------------------------------------------------------------------
// Param -> IN:
//      const char*:    Name of the render parameter.
//------------------------------------------------------------------------
FlyRenderTechnique* FlyRenderEffect::GetTechniqueByName( const char* cName )
{
    for( size_t i=0;i<m_Techniques.size();i++ )
    {
        if( strcmp(m_Techniques[i]->GetName(),cName) == 0 )
            return m_Techniques[i];
    }

    return NULL;
}


//------------------------------------------------------------------------
// Get the count of parameters.
//------------------------------------------------------------------------
size_t FlyRenderEffect::GetNumParameters(void) const
{
    return m_Parameters.size();
}

//------------------------------------------------------------------------
// Get the count of techniques.
//------------------------------------------------------------------------
size_t FlyRenderEffect::GetNumTechniques(void) const
{
    return m_Techniques.size();
}


//------------------------------------------------------------------------
// Get the length of the shader code.
//------------------------------------------------------------------------
size_t FlyRenderEffect::GetShaderLength(void) const
{
    return m_ShaderText.size();
}

//------------------------------------------------------------------------
// Get the shader text of the render effect.
//------------------------------------------------------------------------
const char* FlyRenderEffect::GetShaderText(void) const
{
    return m_ShaderText.c_str();
}


//------------------------------------------------------------------------
// Get the name of the renderEffect.
//------------------------------------------------------------------------
const char* FlyRenderEffect::GetName(void) const
{
    return m_sName.c_str();
}

//------------------------------------------------------------------------
// Set the name for the render effect.
//------------------------------------------------------------------------
void FlyRenderEffect::SetName( const char* cName )
{
    m_sName = cName;
}


//------------------------------------------------------------------------
// Get the string format render effect data type.
// ----------------------------------------------------------------------
// Param -> IN:
//      REDATATYPE:     Render effect data type.
//------------------------------------------------------------------------
const char* FlyRenderEffect::GetDataType( REDATATYPE Type )
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