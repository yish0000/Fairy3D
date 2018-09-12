//------------------------------------------------------------------------
// Name: FlyRenderVariable.h
// Desc: This file define the class act as the variable in the render
//       effect system.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDERVARIABLE_H__
#define __FLY_RENDERVARIABLE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyRenderState.h"
#include "FlyUtil.h"

#include "FlyShaderObject.h"
//////////////////////////////////////////////////////////////////////////

enum REDATATYPE
{
    REDT_UNKNOWN,

    REDT_BOOL,
    REDT_DWORD,
    REDT_STRING,
    REDT_SAMPLER2D,
    REDT_SAMPLER3D,
    REDT_SAMPLERCUBE,
    REDT_SHADER,
    REDT_INT,
    REDT_INT2,
    REDT_INT3,
    REDT_INT4,
    REDT_FLOAT,
    REDT_FLOAT2,
    REDT_FLOAT2X2,
    REDT_FLOAT2X3,
    REDT_FLOAT2X4,
    REDT_FLOAT3,
    REDT_FLOAT3X2,
    REDT_FLOAT3X3,
    REDT_FLOAT3X4,
    REDT_FLOAT4,
    REDT_FLOAT4X2,
    REDT_FLOAT4X3,
    REDT_FLOAT4X4,
};

struct BoolArray4
{
    BOOL x,y,z,w;
};

struct IntArray4
{
    INT x,y,z,w;
};

struct FloatArray2
{
    float x,y;
};

struct FloatArray3
{
    float x,y,z;
};

typedef BoolArray4 BOOL4;
typedef IntArray4 int4;
typedef FloatArray2 float2;
typedef FloatArray3 float3;
typedef FlyVector float4;
typedef std::vector<bool> BoolArray;
typedef std::vector<int> IntArray;
typedef std::vector<float> FloatArray;
typedef std::vector<FlyVector> VectorArray;
typedef std::vector<FlyMatrix> MatrixArray;

//////////////////////////////////////////////////////////////////////////
// Class act as the render effect variable.

class _FLY_EXPORT FlyRenderVariable
{
public:
    // Constructor and destructor.
    FlyRenderVariable(void);
    virtual ~FlyRenderVariable(void);

    virtual FlyRenderVariable* Clone(void);
    virtual void CopyVariableData( FlyRenderVariable* pVar );

    virtual void operator = ( const bool& value );
    virtual void operator = ( const int& value );
    virtual void operator = ( const float& value );
    virtual void operator = ( const float2& value );
    virtual void operator = ( const float3& value );
    virtual void operator = ( const FlyVector& value );
    virtual void operator = ( const FlyMatrix& value );
    virtual void operator = ( const FlySampler& value );
    virtual void operator = ( const std::string& value );
    virtual void operator = ( const FlyShaderObject::ShaderDesc& value );
    virtual void operator = ( const BoolArray& value );
    virtual void operator = ( const IntArray& value );
    virtual void operator = ( const FloatArray& value );
    virtual void operator = ( const VectorArray& value );
    virtual void operator = ( const MatrixArray& value );

    virtual void Value( bool& value ) const;
    virtual void Value( int& value ) const;
    virtual void Value( float& value ) const;
    virtual void Value( float2& value ) const;
    virtual void Value( float3& value ) const;
    virtual void Value( FlyVector& value ) const;
    virtual void Value( FlyMatrix& value ) const;
    virtual void Value( FlySampler& value ) const;
    virtual void Value( std::string& value ) const;
    virtual void Value( FlyShaderObject::ShaderDesc& value ) const;
    virtual void Value( BoolArray& value ) const;
    virtual void Value( IntArray& value ) const;
    virtual void Value( FloatArray& value ) const;
    virtual void Value( VectorArray& value ) const;
    virtual void Value( MatrixArray& value ) const;

    virtual void Pointer( bool*& ptr );
    virtual void Pointer( int*& ptr );
    virtual void Pointer( float*& ptr );
    virtual void Pointer( float2*& ptr );
    virtual void Pointer( float3*& ptr );
    virtual void Pointer( FlyVector*& ptr );
    virtual void Pointer( FlyMatrix*& ptr );
    virtual void Pointer( FlySampler*& ptr );
    virtual void Pointer( std::string*& ptr );
    virtual void Pointer( FlyShaderObject::ShaderDesc*& ptr );
    virtual void Pointer( BoolArray*& ptr );
    virtual void Pointer( IntArray*& ptr );
    virtual void Pointer( FloatArray*& ptr );
    virtual void Pointer( VectorArray*& ptr );
    virtual void Pointer( MatrixArray*& ptr );
};

template <typename T>
class FlyRenderVariableT : public FlyRenderVariable
{
protected:
    T m_Val;

public:

    // Clone the object.
    FlyRenderVariable* Clone(void)
    {
        T value;
        FlyRenderVariable* ret;

        ret = new FlyRenderVariableT<T>;
        this->Value( value );
        *ret = value;
        return ret;
    }

    void CopyVariableData( FlyRenderVariable* pVar )
    {
        FlyRenderVariableT<T>* p = static_cast<FlyRenderVariableT<T>*>(pVar);
        if( p ) m_Val = p->m_Val;
    }

    // Override = operator.
    void operator = ( const T& value )
    {
        m_Val = value;
    }

    void Value( T& value ) const
    {
        value = m_Val;
    }

    // Return the pointer.
    void Pointer( T*& ptr )
    {
        ptr = &m_Val;
    }
};

// Define the render effect variables.
typedef FlyRenderVariableT<bool> RenderVariableBool;
typedef FlyRenderVariableT<int> RenderVariableInt;
typedef FlyRenderVariableT<float> RenderVariableFloat;
typedef FlyRenderVariableT<float2> RenderVariableFloat2;
typedef FlyRenderVariableT<float3> RenderVariableFloat3;
typedef FlyRenderVariableT<FlyVector> RenderVariableVector;
typedef FlyRenderVariableT<FlyMatrix> RenderVariableMatrix;
typedef FlyRenderVariableT<FlySampler> RenderVariableSampler;
typedef FlyRenderVariableT<std::string> RenderVariableString;
typedef FlyRenderVariableT<FlyShaderObject::ShaderDesc> RenderVariableShader;
typedef FlyRenderVariableT<BoolArray> RenderVariableBoolArray;
typedef FlyRenderVariableT<IntArray> RenderVariableIntArray;
typedef FlyRenderVariableT<FloatArray> RenderVariableFloatArray;
typedef FlyRenderVariableT<VectorArray> RenderVariableVectorArray;
typedef FlyRenderVariableT<MatrixArray> RenderVariableMatrixArray;

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDERVARIABLE_H__