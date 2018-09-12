/*
 * -----------------------------------------------------------------------
 *  Name:   F3DShaderParameter.cpp
 *  Desc:   本文件用于实现着色器常量参数对象。
 *  Author: Yish
 *  Date:   2011/5/20
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#include "F3DPCH.h"

#include "F3DShaderParameter.h"


/** 构造函数
*/
F3DShaderParameter::F3DShaderParameter( const AString& name ) : m_sName(name),
    m_VarType(SVT_UNKNOWN),m_pVar(NULL),m_nArraySize(0),m_AutoType(APT_NONE)
{
}

/** 析构函数
*/
F3DShaderParameter::~F3DShaderParameter(void)
{
    F_SAFE_DELETE( m_pVar );
}

/** 设置参数的变量
@Param 变量类型
@Param 变量数组的大小
@Param 要拷贝的变量值
*/
void F3DShaderParameter::SetVariable( EShaderVariableType Type,size_t nArrayCount,
                                      F3DShaderVariable* pVar/* =NULL */ )
{
    F_SAFE_DELETE( m_pVar );

    switch( Type )
    {
    case SVT_BOOL:
        if( pVar )
            m_pVar = pVar->Clone();
        else
        {
            if( nArrayCount > 1 )
            {
                m_pVar = new ShaderVariableBoolArray();

                std::vector<bool> val;
                val.resize( nArrayCount );
                *m_pVar = val;
            }
            else
            {
                m_pVar = new ShaderVariableBool();
            }
        }
        break;
    case SVT_INT:
    case SVT_DWORD:
        if( pVar )
            m_pVar = pVar->Clone();
        else
        {
            if( nArrayCount > 1 )
            {
                m_pVar = new ShaderVariableIntArray();

                std::vector<int> val;
                val.resize( nArrayCount );
                *m_pVar = val;
            }
            else
            {
                m_pVar = new ShaderVariableInt();
            }
        }
        break;
    case SVT_SAMPLER2D:
    case SVT_SAMPLER3D:
    case SVT_SAMPLERCUBE:
        if( pVar )
            m_pVar = pVar->Clone();
        else
        {
            //if( nArrayCount > 1 )
                FASSERT_( false,"纹理参数没有数组类型！" );
//             else
//                 m_pVar = new ShaderVariableSampler();
        }
        break;
    case SVT_FLOAT:
        if( pVar )
            m_pVar = pVar->Clone();
        else
        {
            if( nArrayCount > 1 )
            {
                m_pVar = new ShaderVariableFloatArray();

                std::vector<float> val;
                val.resize( nArrayCount );
                *m_pVar = val;
            }
            else
            {
                m_pVar = new ShaderVariableFloat();
            }
        }
        break;
    case SVT_FLOAT2:
        if( pVar )
            m_pVar = pVar->Clone();
        else
        {
            if( nArrayCount > 1 )
                FASSERT_( false,"float2没有数组类型！" );
            else
                m_pVar = new ShaderVariableFloat2();
        }
        break;
    case SVT_FLOAT3:
        if( pVar )
            m_pVar = pVar->Clone();
        else
        {
            if( nArrayCount > 1 )
                FASSERT_( false,"float3没有数组类型！" );
            else
                m_pVar = new ShaderVariableFloat3();
        }
        break;
    case SVT_FLOAT4:
        if( pVar )
            m_pVar = pVar->Clone();
        else
        {
            if( nArrayCount > 1 )
            {
                m_pVar = new ShaderVariableVectorArray();

                VectorArray val;
                val.resize( nArrayCount );
                *m_pVar = val;
            }
            else
            {
                m_pVar = new ShaderVariableVector();
            }
        }
        break;
    case SVT_FLOAT4X4:
        if( pVar )
            m_pVar = pVar->Clone();
        else
        {
            if( nArrayCount > 1 )
            {
                m_pVar = new ShaderVariableMatrixArray();

                MatrixArray val;
                val.resize( nArrayCount );
                *m_pVar = val;
            }
            else
            {
                m_pVar = new ShaderVariableMatrix();
            }
        }
        break;
    default: FASSERT_( false,"绑定错误的变量类型！" );
    }

    m_VarType = Type;
    m_nArraySize = nArrayCount;
}

/** 纹理赋值运算
*/
F3DShaderParameter& F3DShaderParameter::operator = ( const F3DTextureBase* val )
{
//     SSamplerStageDesc* sampler;
//     m_pVar->Pointer( sampler );
//     sampler->pTexture = const_cast<F3DTextureBase*>(val);
    return *this;
}

/** 取布尔数组指定的值
*/
void F3DShaderParameter::GetValue( size_t index,bool& val ) const
{
    BoolArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    val = (*arr)[index];
}

/** 取整形数组指定的值
*/
void F3DShaderParameter::GetValue( size_t index,int& val ) const
{
    IntArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    val = (*arr)[index];
}

/** 取浮点数组指定的值
*/
void F3DShaderParameter::GetValue( size_t index,float& val ) const
{
    FloatArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    val = (*arr)[index];
}

/** 取向量数组指定的值
*/
void F3DShaderParameter::GetValue( size_t index,F3DVector3& val ) const
{
    VectorArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    val = (*arr)[index];
}

/** 取矩阵数组指定的值
*/
void F3DShaderParameter::GetValue( size_t index,F3DMatrix4& val ) const
{
    MatrixArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    val = (*arr)[index];
}

/** 设置布尔数组指定的值
*/
void F3DShaderParameter::SetValue( size_t index,const bool& val )
{
    BoolArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    (*arr)[index] = val;
}

/** 设置整形数组指定的值
*/
void F3DShaderParameter::SetValue( size_t index,const int& val )
{
    IntArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    (*arr)[index] = val;
}

/** 设置浮点数组指定的值
*/
void F3DShaderParameter::SetValue( size_t index,const float& val )
{
    FloatArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    (*arr)[index] = val;
}

/** 设置向量数组指定的值
*/
void F3DShaderParameter::SetValue( size_t index,const F3DVector3& val )
{
    VectorArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    (*arr)[index] = val;
}

/** 设置矩阵数组指定的值
*/
void F3DShaderParameter::SetValue( size_t index,const F3DMatrix4& val )
{
    MatrixArray* arr = NULL;
    m_pVar->Pointer( arr );

    FASSERT( arr && index < m_nArraySize );
    (*arr)[index] = val;
}