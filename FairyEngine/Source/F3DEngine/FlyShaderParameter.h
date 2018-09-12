/*
 * -----------------------------------------------------------------------
 *  Name:   FlyShaderParameter.h
 *  Desc:   ���ļ�������һ����ɫ������ĳ��������������
 *  Author: Yish
 *  Date:   2011/4/1
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FLY_SHADERPARAMETER_H__
#define __FLY_SHADERPARAMETER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyGfxBasis.h"
#include "FlyAutoParameter.h"

//////////////////////////////////////////////////////////////////////////


/** ������������
*/
enum EShaderVariableType
{
    SVT_UNKNOWN,

    SVT_BOOL,
    SVT_DWORD,
    SVT_STRING,
    SVT_SAMPLER2D,
    SVT_SAMPLER3D,
    SVT_SAMPLERCUBE,
    SVT_SHADER,
    SVT_INT,
    SVT_INT2,
    SVT_INT3,
    SVT_INT4,
    SVT_FLOAT,
    SVT_FLOAT2,
    SVT_FLOAT2X2,
    SVT_FLOAT2X3,
    SVT_FLOAT2X4,
    SVT_FLOAT3,
    SVT_FLOAT3X2,
    SVT_FLOAT3X3,
    SVT_FLOAT3X4,
    SVT_FLOAT4,
    SVT_FLOAT4X2,
    SVT_FLOAT4X3,
    SVT_FLOAT4X4,
};

struct IntArray4 { int x,y,z,w; };
struct FloatArray2 { float x,y; };
struct FloatArray3 { float x,y,z; };

typedef IntArray4 BOOL4;
typedef IntArray4 int4;
typedef FloatArray2 float2;
typedef FloatArray3 float3;
typedef FlyVector float4;
typedef std::vector<bool> BoolArray;
typedef std::vector<int> IntArray;
typedef std::vector<float> FloatArray;
typedef std::vector<FlyVector> VectorArray;
typedef std::vector<FlyMatrix> MatrixArray;

/** ��ɫ������
*/
class FlyShaderVariable
{
public:
    FlyShaderVariable(void) {};
    virtual ~FlyShaderVariable(void) {};

    // ��¡����
    virtual FlyShaderVariable* Clone(void) { return NULL; }

    // ���صȺ������
    virtual void operator = ( const bool& val ) {};
    virtual void operator = ( const int& val ) {};
    virtual void operator = ( const float& val ) {};
    virtual void operator = ( const float2& val ) {};
    virtual void operator = ( const float3& val ) {};
    virtual void operator = ( const FlyVector& val ) {};
    virtual void operator = ( const FlyMatrix& val ) {};
    virtual void operator = ( const FString& val ) {};
    virtual void operator = ( const SSamplerStage& val ) {};
    virtual void operator = ( const SShaderDesc& val ) {};
    virtual void operator = ( const BoolArray& val ) {};
    virtual void operator = ( const IntArray& val ) {};
    virtual void operator = ( const FloatArray& val ) {};
    virtual void operator = ( const VectorArray& val ) {};
    virtual void operator = ( const MatrixArray& val ) {};

    // ��ȡ������ֵ
    virtual void Value( bool& val ) const {};
    virtual void Value( int& val ) const {};
    virtual void Value( float& val ) const {};
    virtual void Value( float2& val ) const {};
    virtual void Value( float3& val ) const {};
    virtual void Value( FlyVector& val ) const {};
    virtual void Value( FlyMatrix& val ) const {};
    virtual void Value( FString& val ) const {};
    virtual void Value( SSamplerStage& val ) const {};
    virtual void Value( SShaderDesc& val ) const {};
    virtual void Value( BoolArray& val ) const {};
    virtual void Value( IntArray& val ) const {};
    virtual void Value( FloatArray& val ) const {};
    virtual void Value( VectorArray& val ) const {};
    virtual void Value( MatrixArray& val ) const {};

    // ��ȡ������ָ��
    virtual void Pointer( bool*& ptr ) {};
    virtual void Pointer( int*& ptr ) {};
    virtual void Pointer( float*& ptr ) {};
    virtual void Pointer( float2*& ptr ) {};
    virtual void Pointer( float3*& ptr ) {};
    virtual void Pointer( FlyVector*& ptr ) {};
    virtual void Pointer( FlyMatrix*& ptr ) {};
    virtual void Pointer( FString*& ptr ) {};
    virtual void Pointer( SSamplerStage*& ptr ) {};
    virtual void Pointer( SShaderDesc*& ptr ) {};
    virtual void Pointer( BoolArray*& ptr ) {};
    virtual void Pointer( IntArray*& ptr ) {};
    virtual void Pointer( FloatArray*& ptr ) {};
    virtual void Pointer( VectorArray*& ptr ) {};
    virtual void Pointer( MatrixArray*& ptr ) {};
};

template <typename T>
class FlyShaderVariableT : public FlyShaderVariable
{
protected:
    T m_Val;

public:
    // ��¡����
    FlyShaderVariable* Clone(void)
    {
        T val;
        FlyShaderVariable* ret;
        ret = new FlyShaderVariableT<T>();
        this->Value( val );
        *ret = val;
        return ret;
    }

    // ���صȺ������
    void operator = ( const T& val )
    {
        m_Val = val;
    }

    // ��ȡ������ֵ
    void Value( T& val ) const
    {
        val = m_Val;
    }

    // ��ȡ������ָ��
    void Pointer( T*& ptr )
    {
        ptr = &m_Val;
    }
};

// Shader������ı�������
typedef FlyShaderVariableT<bool> ShaderVariableBool;
typedef FlyShaderVariableT<int> ShaderVariableInt;
typedef FlyShaderVariableT<float> ShaderVariableFloat;
typedef FlyShaderVariableT<float2> ShaderVariableFloat2;
typedef FlyShaderVariableT<float3> ShaderVariableFloat3;
typedef FlyShaderVariableT<FlyVector> ShaderVariableVector;
typedef FlyShaderVariableT<FlyMatrix> ShaderVariableMatrix;
typedef FlyShaderVariableT<FString> ShaderVariableString;
typedef FlyShaderVariableT<SSamplerStage> ShaderVariableSampler;
typedef FlyShaderVariableT<SShaderDesc> ShaderVariableShader;
typedef FlyShaderVariableT<BoolArray> ShaderVariableBoolArray;
typedef FlyShaderVariableT<IntArray> ShaderVariableIntArray;
typedef FlyShaderVariableT<FloatArray> ShaderVariableFloatArray;
typedef FlyShaderVariableT<VectorArray> ShaderVariableVectorArray;
typedef FlyShaderVariableT<MatrixArray> ShaderVariableMatrixArray;

//////////////////////////////////////////////////////////////////////////

/** ��ɫ����������������
*/
class _F3D_EXPORT FlyShaderParameter : public GfxResourceAlloc
{
protected:
    FString m_sName;                    // ��������������

    EShaderVariableType m_VarType;      // ��ɫ����������
    FlyShaderVariable* m_pVar;          // ��ɫ������
    size_t m_nArraySize;                // ��������ĸ���

    EAutoParameterType m_AutoType;      // �Զ����²���������

public:
    FlyShaderParameter( const FString& name );
    virtual ~FlyShaderParameter(void);

    // ���ò����ı���
    void SetVariable( EShaderVariableType Type,size_t nArrayCount,FlyShaderVariable* pVar=NULL );

    // ��ȡ����������
    const FString& GetName(void) const { return m_sName; }

    // ��ȡ�����Զ���������
    EAutoParameterType GetAutoParameterType(void) const { return m_AutoType; }

    // ���ò������Զ���������
    void SetAutoParameterType( EAutoParameterType Type ) { m_AutoType = Type; }

    // ���صȺ������
    FlyShaderParameter& operator = ( const bool& val ) { *m_pVar = val; return *this; }
    FlyShaderParameter& operator = ( const int& val ) { *m_pVar = val; return *this; }
    FlyShaderParameter& operator = ( const float& val ) { *m_pVar = val; return *this; }
    FlyShaderParameter& operator = ( const float2& val ) { *m_pVar = val; return *this; }
    FlyShaderParameter& operator = ( const float3& val ) { *m_pVar = val; return *this; }
    FlyShaderParameter& operator = ( const FlyVector& val ) { *m_pVar = val; return *this; }
    FlyShaderParameter& operator = ( const FlyMatrix& val ) { *m_pVar = val; return *this; }
    FlyShaderParameter& operator = ( const SSamplerStage& val ) { *m_pVar = val; return *this; }
    FlyShaderParameter& operator = ( const BoolArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }
    FlyShaderParameter& operator = ( const IntArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }
    FlyShaderParameter& operator = ( const FloatArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }
    FlyShaderParameter& operator = ( const VectorArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }
    FlyShaderParameter& operator = ( const MatrixArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }

    // ����ֵ����
    FlyShaderParameter& operator = ( const FlyTexture* val );

    // ��ȡ������ֵ
    void Value( bool& val ) const { m_pVar->Value(val); }
    void Value( int& val ) const { m_pVar->Value(val); }
    void Value( float& val ) const { m_pVar->Value(val); }
    void Value( float2& val ) const { m_pVar->Value(val); }
    void Value( float3& val ) const { m_pVar->Value(val); }
    void Value( FlyVector& val ) const { m_pVar->Value(val); }
    void Value( FlyMatrix& val ) const { m_pVar->Value(val); }
    void Value( SSamplerStage& val ) const { m_pVar->Value(val); }
    void Value( BoolArray& val ) const { m_pVar->Value(val); }
    void Value( IntArray& val ) const { m_pVar->Value(val); }
    void Value( FloatArray& val ) const { m_pVar->Value(val); }
    void Value( VectorArray& val ) const { m_pVar->Value(val); }
    void Value( MatrixArray& val ) const { m_pVar->Value(val); }

    // ��ȡ������ָ��
    void Pointer( bool*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( int*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( float*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( float2*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( float3*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( FlyVector*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( FlyMatrix*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( SSamplerStage*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( BoolArray*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( IntArray*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( FloatArray*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( VectorArray*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( MatrixArray*& ptr ) { m_pVar->Pointer(ptr); }

    // ȡ����ָ����ֵ
    void GetValue( size_t index,bool& val ) const;
    void GetValue( size_t index,int& val ) const;
    void GetValue( size_t index,float& val ) const;
    void GetValue( size_t index,FlyVector& val ) const;
    void GetValue( size_t index,FlyMatrix& val ) const;

    // ���������ֵ
    void SetValue( size_t index,const bool& val );
    void SetValue( size_t index,const int& val );
    void SetValue( size_t index,const float& val );
    void SetValue( size_t index,const FlyVector& val );
    void SetValue( size_t index,const FlyMatrix& val );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SHADERPARAMETER_H__