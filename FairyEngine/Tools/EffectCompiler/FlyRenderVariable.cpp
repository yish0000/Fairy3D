//------------------------------------------------------------------------
// Name: FlyRenderVariable.cpp
// Desc: This file is to implement the class FlyRenderVariable.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyRenderVariable.h"


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyRenderVariable::FlyRenderVariable(void)
{
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyRenderVariable::~FlyRenderVariable(void)
{
}


//------------------------------------------------------------------------
// Clone the content to another object.
//------------------------------------------------------------------------
FlyRenderVariable* FlyRenderVariable::Clone(void)
{
    return NULL;
}

//------------------------------------------------------------------------
// Copy another variable data to me.
//------------------------------------------------------------------------
void FlyRenderVariable::CopyVariableData( FlyRenderVariable* pVar )
{
}


//------------------------------------------------------------------------
// Override the operators for the class.
//------------------------------------------------------------------------
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

void FlyRenderVariable::operator = ( const std::string& value )
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


//------------------------------------------------------------------------
// Get the value of the render variable.
//------------------------------------------------------------------------
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

void FlyRenderVariable::Value( std::string& value ) const
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


//------------------------------------------------------------------------
// Get the actual data pointer for the variable.
//------------------------------------------------------------------------
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

void FlyRenderVariable::Pointer( std::string*& ptr )
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