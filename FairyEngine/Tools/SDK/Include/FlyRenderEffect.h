//------------------------------------------------------------------------
// Name: FlyRenderEffect.h
// Desc: This file define an interface to act as the RenderEffect.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDER_EFFECT_H__
#define __FLY_RENDER_EFFECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyRenderVariable.h"
#include "FlyTechnique.h"

#include "FlyRenderAutoParameter.h"
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Render effect's data type.

// Pre-declare the classes.
class FlyRenderEffect;
class FlyRenderParameter;

//////////////////////////////////////////////////////////////////////////
// Class to act as the render effect annotation.

class _FLY_EXPORT FlyRenderAnnotation
{
protected:
    std::string m_sName;
    REDATATYPE m_Type;
    FlyRenderVariable* m_pVar;

public:
    FlyRenderAnnotation(void);
    ~FlyRenderAnnotation(void);

    const char* GetName(void);
    REDATATYPE GetType(void);
    FlyRenderVariable* GetVariable(void);
    void SetName( const char* cName );
    void SetVariable( REDATATYPE Type,FlyRenderVariable* pVar );

    FlyRenderAnnotation& operator = ( const bool& value );
    FlyRenderAnnotation& operator = ( const int& value );
    FlyRenderAnnotation& operator = ( const std::string& value );
    FlyRenderAnnotation& operator = ( const float& value );
    FlyRenderAnnotation& operator = ( const float2& value );
    FlyRenderAnnotation& operator = ( const float3& value );
    FlyRenderAnnotation& operator = ( const FlyVector& value );
    FlyRenderAnnotation& operator = ( const FlyMatrix& value );

    void Value( bool& value ) const;
    void Value( int& value ) const;
    void Value( std::string& value ) const;
    void Value( float& value ) const;
    void Value( float2& value ) const;
    void Value( float3& value ) const;
    void Value( FlyVector& value ) const;
    void Value( FlyMatrix& value ) const;

    void Pointer( bool*& ptr ) const;
    void Pointer( int*& ptr ) const;
    void Pointer( std::string*& ptr ) const;
    void Pointer( float*& ptr ) const;
    void Pointer( float2*& ptr ) const;
    void Pointer( float3*& ptr ) const;
    void Pointer( FlyVector*& ptr ) const;
    void Pointer( FlyMatrix*& ptr ) const;
};

//////////////////////////////////////////////////////////////////////////
// Class to act as the render effect parameter.

class _FLY_EXPORT FlyRenderParameter
{
protected:
    FlyRenderEffect*    m_pEffect;      // Pointer to the parent effect.
    std::string         m_sName;        // Name of the parameter.
    std::string         m_sSemantic;    // Semantic of the parameter.

    REDATATYPE          m_Type;         // Type of the variable.
    FlyRenderVariable*  m_pVariable;    // Pointer to the RenderVariable.
    UINT                m_nArraySize;   // Array size of the array parameter.

    bool                m_bAutoUpdate;  // Is an auto-update parameter?
    AUTOPARAMETERTYPE   m_AutoType;     // Type of the auto-update parameter.

    // Annotation list for parameter.
    std::vector<FlyRenderAnnotation*> m_Annotations;

public:
    // Constructor and destructor.
    FlyRenderParameter( FlyRenderEffect* pEffect );
    ~FlyRenderParameter(void);

    void CopyVariableData( FlyRenderParameter* parameter );
    FlyRenderParameter* Clone(void);
    FlyRenderAnnotation* AddAnnotation( const char* cName );

    const char* GetName(void);
    const char* GetSemantic(void);
    REDATATYPE GetDataType(void);
    UINT GetArraySize(void);

    // Set the Render parameter content.
    void SetName( const char* cName );
    void SetSemantic( const char* cSemantic );
    void SetVariable( REDATATYPE type,UINT nSize,FlyRenderVariable* pVar );

    UINT GetNumAnnotations(void);
    FlyRenderAnnotation* GetAnnotationByIndex( int nIndex );
    FlyRenderAnnotation* GetAnnotationByName( const char* cName );

    bool IsAutoUpdateParameter(void) const { return m_bAutoUpdate; }
    AUTOPARAMETERTYPE GetAutoUpdateType(void) const { return m_AutoType; }

    void SetAutoUpdate( bool bAutoUpdate ) { m_bAutoUpdate = bAutoUpdate; }
    void SetAutoUpdateType( AUTOPARAMETERTYPE Type ) { m_AutoType = Type; }

    // Get the effect pointer.
    FlyRenderEffect* GetRenderEffect(void);

    FlyRenderParameter& operator = ( const bool& value );
    FlyRenderParameter& operator = ( const int& value );
    FlyRenderParameter& operator = ( const float& value );
    FlyRenderParameter& operator = ( const float2& value );
    FlyRenderParameter& operator = ( const float3& value );
    FlyRenderParameter& operator = ( const FlyVector& value );
    FlyRenderParameter& operator = ( const FlyMatrix& value );
    FlyRenderParameter& operator = ( const FlySampler& value );
    FlyRenderParameter& operator = ( const BoolArray& value );
    FlyRenderParameter& operator = ( const IntArray& value );
    FlyRenderParameter& operator = ( const FloatArray& value );
    FlyRenderParameter& operator = ( const VectorArray& value );
    FlyRenderParameter& operator = ( const MatrixArray& value );

    // Function only for the array.
    void SetValue( int nIndex,const bool& value );
    void SetValue( int nIndex,const int& value );
    void SetValue( int nIndex,const float& value );
    void SetValue( int nIndex,const FlyVector& value );
    void SetValue( int nIndex,const FlyMatrix& value );

    void Value( bool& value ) const;
    void Value( int& value ) const;
    void Value( float& value ) const;
    void Value( float2& value ) const;
    void Value( float3& value ) const;
    void Value( FlyVector& value ) const;
    void Value( FlyMatrix& value ) const;
    void Value( FlySampler& value ) const;
    void Value( BoolArray& value ) const;
    void Value( IntArray& value ) const;
    void Value( FloatArray& value ) const;
    void Value( VectorArray& value ) const;
    void Value( MatrixArray& value ) const;

    // Function only for the array.
    void GetValue( int nIndex,bool* pValue ) const;
    void GetValue( int nIndex,int* pValue ) const;
    void GetValue( int nIndex,float* pValue ) const;
    void GetValue( int nIndex,FlyVector* pValue ) const;
    void GetValue( int nIndex,FlyMatrix* pValue ) const;

    void Pointer( bool*& ptr ) const;
    void Pointer( int*& ptr ) const;
    void Pointer( float*& ptr ) const;
    void Pointer( float2*& ptr ) const;
    void Pointer( float3*& ptr ) const;
    void Pointer( FlyVector*& ptr ) const;
    void Pointer( FlyMatrix*& ptr ) const;
    void Pointer( FlySampler*& ptr ) const;
    void Pointer( BoolArray*& ptr ) const;
    void Pointer( IntArray*& ptr ) const;
    void Pointer( FloatArray*& ptr ) const;
    void Pointer( VectorArray*& ptr ) const;
    void Pointer( MatrixArray*& ptr ) const;

    // Operator to set the texture pointer.
    FlyRenderParameter& operator = ( const FlyTexture* value );
};

//////////////////////////////////////////////////////////////////////////
// Class to store the whole render effect content.

class _FLY_EXPORT FlyRenderEffect
{
protected:
    std::string m_sName;
    std::vector<FlyRenderParameter*> m_Parameters;
    std::string m_ShaderText;
    std::vector<FlyRenderTechnique*> m_Techniques;

    HRESULT GenerateEffect( FlyRenderEffectCompiler* compiler );
    void ReaderVariable( FlyRenderParameter* param,FILE* pFile );
    void ReadAnnotationValue( FlyRenderAnnotation* annotation,FILE* pFile );
    const char* GetDataType( REDATATYPE Type );

public:
    // Constructor and destructor.
    FlyRenderEffect(void);
    ~FlyRenderEffect(void);

    void Reset(void);

    HRESULT LoadEffect( const char* cFilename );
    HRESULT LoadEffectFromXML( const char* cXmlDoc );
    HRESULT Compile( const void* pBuffer );

    FlyRenderParameter* GetParameterByName( const char* cName );
    FlyRenderParameter* GetParameterByIndex( UINT nIndex );
    FlyRenderParameter* GetParameterBySemantic( const char* cSemantic );
    FlyRenderTechnique* GetTechniqueByName( const char* cName );
    FlyRenderTechnique* GetTechniqueByIndex( UINT nIndex );

    UINT GetNumParameters(void) const;
    UINT GetNumTechniques(void) const;

    void SetParameterData( int nIndex,FlyRenderParameter* pParam );

    size_t GetShaderLength(void) const;
    const char* GetShaderText(void) const;

    const char* GetName(void) const;
    void SetName( const char* cName );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDER_EFFECT_H__