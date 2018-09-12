/*
 * -----------------------------------------------------------------------
 *  Name:   FlyRenderVariable.cpp
 *  Desc:   本文件用于实现材质内的变量。
 *  Author: Yish
 *  Date:   11/7/2010
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * -----------------------------------------------------------------------
 */


#include "FlyRenderVariable.h"


/** 构造函数
*/
FlyRenderVariable::FlyRenderVariable(void)
{
}


/** 析构函数
*/
FlyRenderVariable::~FlyRenderVariable(void)
{
}


/** 将变量拷贝一份
*/
FlyRenderVariable* FlyRenderVariable::Clone(void)
{
    return NULL;
}


/** 将另一份变量的数据拷贝过来
*/
void FlyRenderVariable::CopyVariableData( FlyRenderVariable* pVar )
{
}


/** 重载赋值运算符
*/
void FlyRenderVariable::operator = ( const bool& value )
{
}

void FlyRenderVariable::operator = ( const int& value )
{
}

void FlyRenderVariable::operator = ( const float& value )
{
}

void FlyRenderVariable::operator = ( const float2& value )
{
}

void FlyRenderVariable::operator = ( const float3& value )
{
}

void FlyRenderVariable::operator = ( const FlyVector& value )
{
}

void FlyRenderVariable::operator = ( const FlyMatrix& value )
{
}

void FlyRenderVariable::operator = ( const FlySampler& value )
{
}

void FlyRenderVariable::operator = ( const FString& value )
{
}

void FlyRenderVariable::operator = ( const FlyShaderObject::ShaderDesc& value )
{
}

void FlyRenderVariable::operator = ( const BoolArray& value )
{
}

void FlyRenderVariable::operator = ( const IntArray& value )
{
}

void FlyRenderVariable::operator = ( const FloatArray& value )
{
}

void FlyRenderVariable::operator = ( const VectorArray& value )
{
}

void FlyRenderVariable::operator = ( const MatrixArray& value )
{
}


/** 获取变量的实际值
*/
void FlyRenderVariable::Value( bool& value ) const
{
}

void FlyRenderVariable::Value( int& value ) const
{
}

void FlyRenderVariable::Value( float& value ) const
{
}

void FlyRenderVariable::Value( float2& value ) const
{
}

void FlyRenderVariable::Value( float3& value ) const
{
}

void FlyRenderVariable::Value( FlyVector& value ) const
{
}

void FlyRenderVariable::Value( FlyMatrix& value ) const
{
}

void FlyRenderVariable::Value( FlySampler& value ) const
{
}

void FlyRenderVariable::Value( FString& value ) const
{
}

void FlyRenderVariable::Value( FlyShaderObject::ShaderDesc& value ) const
{
}

void FlyRenderVariable::Value( BoolArray& value ) const
{
}

void FlyRenderVariable::Value( IntArray& value ) const
{
}

void FlyRenderVariable::Value( FloatArray& value ) const
{
}

void FlyRenderVariable::Value( VectorArray& value ) const
{
}

void FlyRenderVariable::Value( MatrixArray& value ) const
{
}


/** 获取实际数据的指针
*/
void FlyRenderVariable::Pointer( bool*& ptr )
{
}

void FlyRenderVariable::Pointer( int*& ptr )
{
}

void FlyRenderVariable::Pointer( float*& ptr )
{
}

void FlyRenderVariable::Pointer( float2*& ptr )
{
}

void FlyRenderVariable::Pointer( float3*& ptr )
{
}

void FlyRenderVariable::Pointer( FlyVector*& ptr )
{
}

void FlyRenderVariable::Pointer( FlyMatrix*& ptr )
{
}

void FlyRenderVariable::Pointer( FlySampler*& ptr )
{
}

void FlyRenderVariable::Pointer( FString*& ptr )
{
}

void FlyRenderVariable::Pointer( FlyShaderObject::ShaderDesc*& ptr )
{
}

void FlyRenderVariable::Pointer( BoolArray*& ptr )
{
}

void FlyRenderVariable::Pointer( IntArray*& ptr )
{
}

void FlyRenderVariable::Pointer( FloatArray*& ptr )
{
}

void FlyRenderVariable::Pointer( VectorArray*& ptr )
{
}

void FlyRenderVariable::Pointer( MatrixArray*& ptr )
{
}