//------------------------------------------------------------------------
// Name: FlyRenderEffect.h
// Desc: 
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


/** 评注对象
@remarks
*/
class _FLY_EXPORT FlyRenderAnnotation
{
protected:
    FString m_sName;            // 评注的名字
    REDATATYPE m_Type;          // 评注内变量的类型
    FlyRenderVariable* m_pVar;  // 变量的指针

public:
    FlyRenderAnnotation(void);
    ~FlyRenderAnnotation(void);

    const FString& GetName(void) const;
    REDATATYPE GetType(void) const;
    FlyRenderVariable* GetVariable(void) const;
    void SetName( const FString& name );
    void SetVariable( REDATATYPE Type,FlyRenderVariable* pVar );

    FlyRenderAnnotation& operator = ( const bool& value );
    FlyRenderAnnotation& operator = ( const int& value );
    FlyRenderAnnotation& operator = ( const FString& value );
    FlyRenderAnnotation& operator = ( const float& value );
    FlyRenderAnnotation& operator = ( const float2& value );
    FlyRenderAnnotation& operator = ( const float3& value );
    FlyRenderAnnotation& operator = ( const FlyVector& value );
    FlyRenderAnnotation& operator = ( const FlyMatrix& value );

    void Value( bool& value ) const;
    void Value( int& value ) const;
    void Value( FString& value ) const;
    void Value( float& value ) const;
    void Value( float2& value ) const;
    void Value( float3& value ) const;
    void Value( FlyVector& value ) const;
    void Value( FlyMatrix& value ) const;

    void Pointer( bool*& ptr ) const;
    void Pointer( int*& ptr ) const;
    void Pointer( FString*& ptr ) const;
    void Pointer( float*& ptr ) const;
    void Pointer( float2*& ptr ) const;
    void Pointer( float3*& ptr ) const;
    void Pointer( FlyVector*& ptr ) const;
    void Pointer( FlyMatrix*& ptr ) const;
};


/** Shader的参数
@remarks
*/
class _FLY_EXPORT FlyRenderParameter
{
protected:
    FlyRenderEffect*    m_pEffect;      // 指向父渲染效果的指针
    FString             m_sName;        // 参数的名字
    FString             m_sSemantic;    // 参数的语义

    REDATATYPE          m_Type;         // 参数变量的类型
    FlyRenderVariable*  m_pVariable;    // 指向参数变量的指针
    UINT                m_nArraySize;   // 参数数组的大小

    bool                m_bAutoUpdate;  // 是否为一个自动更新的参数
    AUTOPARAMETERTYPE   m_AutoType;     // 自动更新参数的类型

    // 参数的评注列表
    std::vector<FlyRenderAnnotation*> m_Annotations;

public:
    FlyRenderParameter( FlyRenderEffect* pEffect );
    ~FlyRenderParameter(void);

    void CopyVariableData( FlyRenderParameter* parameter );
    FlyRenderParameter* Clone(void);
    FlyRenderAnnotation* AddAnnotation( const FString& name );

    const FString& GetName(void) const;
    const FString& GetSemantic(void) const;
    REDATATYPE GetDataType(void) const;
    UINT GetArraySize(void) const;

    // Set the Render parameter content.
    void SetName( const FString& name );
    void SetSemantic( const FString& semantic );
    void SetVariable( REDATATYPE type,UINT nSize,FlyRenderVariable* pVar );

    size_t GetNumAnnotations(void) const;
    FlyRenderAnnotation* GetAnnotationByIndex( size_t nIndex );
    FlyRenderAnnotation* GetAnnotationByName( const FString& name );

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
    void SetValue( UINT nIndex,const bool& value );
    void SetValue( UINT nIndex,const int& value );
    void SetValue( UINT nIndex,const float& value );
    void SetValue( UINT nIndex,const FlyVector& value );
    void SetValue( UINT nIndex,const FlyMatrix& value );

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
    void GetValue( UINT nIndex,bool* pValue ) const;
    void GetValue( UINT nIndex,int* pValue ) const;
    void GetValue( UINT nIndex,float* pValue ) const;
    void GetValue( UINT nIndex,FlyVector* pValue ) const;
    void GetValue( UINT nIndex,FlyMatrix* pValue ) const;

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
    typedef std::vector<FlyRenderParameter*> ParameterList;
    typedef std::vector<FlyRenderTechnique*> TechniqueList;

protected:
    FString m_sName;                // 渲染效果的名字
    ParameterList m_Parameters;     // 参数列表
    FString m_ShaderText;           // Shader代码
    TechniqueList m_Techniques;     // 技术列表

    HRESULT GenerateEffect( FlyRenderEffectCompiler* compiler );
    void ReaderVariable( FlyRenderParameter* param,FILE* pFile );
    void ReadAnnotationValue( FlyRenderAnnotation* annotation,FILE* pFile );
    const TCHAR* GetDataType( REDATATYPE Type );

public:
    FlyRenderEffect(void);
    ~FlyRenderEffect(void);

    void Reset(void);

    HRESULT LoadEffect( const char* cFilename );
    HRESULT LoadEffectFromXML( const char* cXmlDoc );
    HRESULT Compile( const void* pBuffer );

    FlyRenderParameter* GetParameterByName( const FString& name );
    FlyRenderParameter* GetParameterByIndex( size_t nIndex );
    FlyRenderParameter* GetParameterBySemantic( const FString& semantic );
    FlyRenderTechnique* GetTechniqueByName( const FString& name );
    FlyRenderTechnique* GetTechniqueByIndex( size_t nIndex );

    size_t GetNumParameters(void) const { return m_Parameters.size(); }
    size_t GetNumTechniques(void) const { return m_Techniques.size(); }

    void SetParameterData( size_t nIndex,FlyRenderParameter* pParam );

    size_t GetShaderLength(void) const { return m_ShaderText.length(); }
    const FString& GetShaderText(void) const { return m_ShaderText; }

    const FString& GetName(void) const { return m_sName; }
    void SetName( const FString& name ) { m_sName = name; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDER_EFFECT_H__