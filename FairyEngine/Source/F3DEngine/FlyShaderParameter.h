/*
 * -----------------------------------------------------------------------
 *  Name:   FlyShaderParameter.h
 *  Desc:   本文件定义了一个着色器程序的常量参数管理对象。
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


/** 变量数据类型
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

/** 着色器变量
*/
class FlyShaderVariable
{
public:
    FlyShaderVariable(void) {};
    virtual ~FlyShaderVariable(void) {};

    // 克隆对象
    virtual FlyShaderVariable* Clone(void) { return NULL; }

    // 重载等号运算符
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

    // 获取变量的值
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

    // 获取变量的指针
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
    // 克隆对象
    FlyShaderVariable* Clone(void)
    {
        T val;
        FlyShaderVariable* ret;
        ret = new FlyShaderVariableT<T>();
        this->Value( val );
        *ret = val;
        return ret;
    }

    // 重载等号运算符
    void operator = ( const T& val )
    {
        m_Val = val;
    }

    // 获取变量的值
    void Value( T& val ) const
    {
        val = m_Val;
    }

    // 获取变量的指针
    void Pointer( T*& ptr )
    {
        ptr = &m_Val;
    }
};

// Shader程序里的变量类型
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

/** 着色器程序常量参数对象
*/
class _F3D_EXPORT FlyShaderParameter : public GfxResourceAlloc
{
protected:
    FString m_sName;                    // 常量参数的名字

    EShaderVariableType m_VarType;      // 着色器变量类型
    FlyShaderVariable* m_pVar;          // 着色器变量
    size_t m_nArraySize;                // 数组变量的个数

    EAutoParameterType m_AutoType;      // 自动更新参数的类型

public:
    FlyShaderParameter( const FString& name );
    virtual ~FlyShaderParameter(void);

    // 设置参数的变量
    void SetVariable( EShaderVariableType Type,size_t nArrayCount,FlyShaderVariable* pVar=NULL );

    // 获取参数的名字
    const FString& GetName(void) const { return m_sName; }

    // 获取参数自动更新类型
    EAutoParameterType GetAutoParameterType(void) const { return m_AutoType; }

    // 设置参数的自动更新类型
    void SetAutoParameterType( EAutoParameterType Type ) { m_AutoType = Type; }

    // 重载等号运算符
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

    // 纹理赋值运算
    FlyShaderParameter& operator = ( const FlyTexture* val );

    // 获取参数的值
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

    // 获取参数的指针
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

    // 取数组指定的值
    void GetValue( size_t index,bool& val ) const;
    void GetValue( size_t index,int& val ) const;
    void GetValue( size_t index,float& val ) const;
    void GetValue( size_t index,FlyVector& val ) const;
    void GetValue( size_t index,FlyMatrix& val ) const;

    // 设置数组的值
    void SetValue( size_t index,const bool& val );
    void SetValue( size_t index,const int& val );
    void SetValue( size_t index,const float& val );
    void SetValue( size_t index,const FlyVector& val );
    void SetValue( size_t index,const FlyMatrix& val );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SHADERPARAMETER_H__