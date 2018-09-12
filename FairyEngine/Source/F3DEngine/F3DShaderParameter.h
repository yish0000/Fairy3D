/*
 * ------------------------------------------------------------------------
 *  Name:   F3DShaderParameter.h
 *  Desc:   ���ļ�������һ����ɫ������ĳ��������������
 *  Author: Yish
 *  Date:   2011/4/1
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_SHADERPARAMETER_H__
#define __F3D_SHADERPARAMETER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"
#include "F3DAutoParameter.h"

///////////////////////////////////////////////////////////////////////////


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
typedef F3DVector3 float4;
typedef std::vector<bool> BoolArray;
typedef std::vector<int> IntArray;
typedef std::vector<float> FloatArray;
typedef std::vector<F3DVector3> VectorArray;
typedef std::vector<F3DMatrix4> MatrixArray;

/** ��ɫ������
*/
class F3DShaderVariable
{
public:
    F3DShaderVariable(void) {};
    virtual ~F3DShaderVariable(void) {};

    // ��¡����
    virtual F3DShaderVariable* Clone(void) { return NULL; }

    // ���صȺ������
    virtual void operator = ( const bool& val ) {};
    virtual void operator = ( const int& val ) {};
    virtual void operator = ( const float& val ) {};
    virtual void operator = ( const float2& val ) {};
    virtual void operator = ( const float3& val ) {};
    virtual void operator = ( const F3DVector3& val ) {};
    virtual void operator = ( const F3DMatrix4& val ) {};
    virtual void operator = ( const AString& val ) {};
//    virtual void operator = ( const SSamplerStageDesc& val ) {};
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
    virtual void Value( F3DVector3& val ) const {};
    virtual void Value( F3DMatrix4& val ) const {};
    virtual void Value( AString& val ) const {};
//    virtual void Value( SSamplerStageDesc& val ) const {};
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
    virtual void Pointer( F3DVector3*& ptr ) {};
    virtual void Pointer( F3DMatrix4*& ptr ) {};
    virtual void Pointer( AString*& ptr ) {};
//    virtual void Pointer( SSamplerStageDesc*& ptr ) {};
    virtual void Pointer( SShaderDesc*& ptr ) {};
    virtual void Pointer( BoolArray*& ptr ) {};
    virtual void Pointer( IntArray*& ptr ) {};
    virtual void Pointer( FloatArray*& ptr ) {};
    virtual void Pointer( VectorArray*& ptr ) {};
    virtual void Pointer( MatrixArray*& ptr ) {};
};

template <typename T>
class F3DShaderVariableT : public F3DShaderVariable
{
protected:
    T m_Val;

public:
    // ��¡����
    F3DShaderVariable* Clone(void)
    {
        T val;
        F3DShaderVariable* ret;
        ret = new F3DShaderVariableT<T>();
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
typedef F3DShaderVariableT<bool> ShaderVariableBool;
typedef F3DShaderVariableT<int> ShaderVariableInt;
typedef F3DShaderVariableT<float> ShaderVariableFloat;
typedef F3DShaderVariableT<float2> ShaderVariableFloat2;
typedef F3DShaderVariableT<float3> ShaderVariableFloat3;
typedef F3DShaderVariableT<F3DVector3> ShaderVariableVector;
typedef F3DShaderVariableT<F3DMatrix4> ShaderVariableMatrix;
typedef F3DShaderVariableT<AString> ShaderVariableString;
//typedef F3DShaderVariableT<SSamplerStageDesc> ShaderVariableSampler;
typedef F3DShaderVariableT<SShaderDesc> ShaderVariableShader;
typedef F3DShaderVariableT<BoolArray> ShaderVariableBoolArray;
typedef F3DShaderVariableT<IntArray> ShaderVariableIntArray;
typedef F3DShaderVariableT<FloatArray> ShaderVariableFloatArray;
typedef F3DShaderVariableT<VectorArray> ShaderVariableVectorArray;
typedef F3DShaderVariableT<MatrixArray> ShaderVariableMatrixArray;

///////////////////////////////////////////////////////////////////////////

/** ��ɫ����������������
*/
class FAIRY_API F3DShaderParameter : public FGeneralAlloc
{
protected:
    AString m_sName;                    // ��������������

    EShaderVariableType m_VarType;      // ��ɫ����������
    F3DShaderVariable* m_pVar;          // ��ɫ������
    size_t m_nArraySize;                // ��������ĸ���

    EAutoParameterType m_AutoType;      // �Զ����²���������

public:
    F3DShaderParameter( const AString& name );
	virtual ~F3DShaderParameter(void);

    // ���ò����ı���
    void SetVariable( EShaderVariableType Type,size_t nArrayCount,F3DShaderVariable* pVar=NULL );

    // ��ȡ����������
    const AString& GetName(void) const { return m_sName; }

    // ��ȡ�����Զ���������
    EAutoParameterType GetAutoParameterType(void) const { return m_AutoType; }

    // ���ò������Զ���������
    void SetAutoParameterType( EAutoParameterType Type ) { m_AutoType = Type; }

    // ���صȺ������
    F3DShaderParameter& operator = ( const bool& val ) { *m_pVar = val; return *this; }
    F3DShaderParameter& operator = ( const int& val ) { *m_pVar = val; return *this; }
    F3DShaderParameter& operator = ( const float& val ) { *m_pVar = val; return *this; }
    F3DShaderParameter& operator = ( const float2& val ) { *m_pVar = val; return *this; }
    F3DShaderParameter& operator = ( const float3& val ) { *m_pVar = val; return *this; }
    F3DShaderParameter& operator = ( const F3DVector3& val ) { *m_pVar = val; return *this; }
    F3DShaderParameter& operator = ( const F3DMatrix4& val ) { *m_pVar = val; return *this; }
//    F3DShaderParameter& operator = ( const SSamplerStageDesc& val ) { *m_pVar = val; return *this; }
    F3DShaderParameter& operator = ( const BoolArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }
    F3DShaderParameter& operator = ( const IntArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }
    F3DShaderParameter& operator = ( const FloatArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }
    F3DShaderParameter& operator = ( const VectorArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }
    F3DShaderParameter& operator = ( const MatrixArray& val ) { *m_pVar = val; m_nArraySize = val.size(); return *this; }

    // ����ֵ����
    F3DShaderParameter& operator = ( const F3DTextureBase* val );

    // ��ȡ������ֵ
    void Value( bool& val ) const { m_pVar->Value(val); }
    void Value( int& val ) const { m_pVar->Value(val); }
    void Value( float& val ) const { m_pVar->Value(val); }
    void Value( float2& val ) const { m_pVar->Value(val); }
    void Value( float3& val ) const { m_pVar->Value(val); }
    void Value( F3DVector3& val ) const { m_pVar->Value(val); }
    void Value( F3DMatrix4& val ) const { m_pVar->Value(val); }
//    void Value( SSamplerStageDesc& val ) const { m_pVar->Value(val); }
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
    void Pointer( F3DVector3*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( F3DMatrix4*& ptr ) { m_pVar->Pointer(ptr); }
//    void Pointer( SSamplerStageDesc*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( BoolArray*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( IntArray*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( FloatArray*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( VectorArray*& ptr ) { m_pVar->Pointer(ptr); }
    void Pointer( MatrixArray*& ptr ) { m_pVar->Pointer(ptr); }

    // ȡ����ָ����ֵ
    void GetValue( size_t index,bool& val ) const;
    void GetValue( size_t index,int& val ) const;
    void GetValue( size_t index,float& val ) const;
    void GetValue( size_t index,F3DVector3& val ) const;
    void GetValue( size_t index,F3DMatrix4& val ) const;

    // ���������ֵ
    void SetValue( size_t index,const bool& val );
    void SetValue( size_t index,const int& val );
    void SetValue( size_t index,const float& val );
    void SetValue( size_t index,const F3DVector3& val );
    void SetValue( size_t index,const F3DMatrix4& val );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_SHADERPARAMETER_H__