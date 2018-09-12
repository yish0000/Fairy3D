//------------------------------------------------------------------------
// Name: FlyD3DShaderObject.cpp
// Desc: This file is to implement the class FlyD3DShaderObject.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#include "FlyD3DShaderObject.h"
#include "FlyD3DRenderDevice.h"


//////////////////////////////////////////////////////////////////////////

class SetShaderParameter
{
public:
    SetShaderParameter(void) { /* Nothing to do! */ };
    virtual ~SetShaderParameter(void) { /* Nothing to do! */ };

    virtual void ExchangeData(void) = 0;
};

//////////////////////////////////////////////////////////////////////////

template <typename SrcType,typename DestType>
class SetD3DShaderParameter
{
};

template <>
class SetD3DShaderParameter<bool,bool> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( BOOL* pBoolReg,FlyRenderParameter* param )
    {
        m_pBoolReg = pBoolReg;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        bool v;
        m_pParam->Value( v );
        *m_pBoolReg = static_cast<BOOL>(v);
    }

protected:
    BOOL* m_pBoolReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<int,bool> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( BOOL* pBoolReg,FlyRenderParameter* param )
    {
        m_pBoolReg = pBoolReg;
        m_pParam = param;
    }

    // Override the virtual function
    void ExchangeData(void)
    {
        int v;
        m_pParam->Value( v );
        *m_pBoolReg = static_cast<BOOL>(v);
    }

protected:
    BOOL* m_pBoolReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float,bool> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( BOOL* pBoolReg,FlyRenderParameter* param )
    {
        m_pBoolReg = pBoolReg;
        m_pParam = param;
    }

    // Override the virtual function
    void ExchangeData(void)
    {
        float v;
        m_pParam->Value( v );
        *m_pBoolReg = static_cast<BOOL>(v);
    }

protected:
    BOOL* m_pBoolReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<bool,int> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( int* pIntReg,FlyRenderParameter* param )
    {
        m_pIntReg = pIntReg;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        bool v;
        m_pParam->Value( v );
        *m_pIntReg = static_cast<int>(v);
    }

protected:
    int* m_pIntReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<int,int> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( int* pIntReg,FlyRenderParameter* param )
    {
        m_pIntReg = pIntReg;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        int v;
        m_pParam->Value( v );
        *m_pIntReg = static_cast<int>(v);
    }

protected:
    int* m_pIntReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float,int> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( int* pIntReg,FlyRenderParameter* param )
    {
        m_pIntReg = pIntReg;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        float v;
        m_pParam->Value( v );
        *m_pIntReg = static_cast<int>(v);
    }

protected:
    int* m_pIntReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<bool,float> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float* pFloatReg,FlyRenderParameter* param )
    {
        m_pFloatReg = pFloatReg;
        m_pParam = param;
    }

    // Override the function.
    void ExchangeData(void)
    {
        bool v;
        m_pParam->Value( v );
        *m_pFloatReg = static_cast<float>(v);
    }

protected:
    float* m_pFloatReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<int,float> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float* pFloatReg,FlyRenderParameter* param )
    {
        m_pFloatReg = pFloatReg;
        m_pParam = param;
    }

    // Override the function.
    void ExchangeData(void)
    {
        int v;
        m_pParam->Value( v );
        *m_pFloatReg = static_cast<float>(v);
    }

protected:
    float* m_pFloatReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float,float> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float* pFloatReg,FlyRenderParameter* param )
    {
        m_pFloatReg = pFloatReg;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        float v;
        m_pParam->Value( v );
        *m_pFloatReg = static_cast<float>(v);
    }

protected:
    float* m_pFloatReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float2,float2> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatReg,FlyRenderParameter* param )
    {
        m_pFloatReg = pFloatReg;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        float2* v;
        m_pParam->Pointer( v );
        m_pFloatReg->x = v->x;
        m_pFloatReg->y = v->y;
    }

protected:
    float4* m_pFloatReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float3,float3> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatReg,FlyRenderParameter* param )
    {
        m_pFloatReg = pFloatReg;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        float3* v;
        m_pParam->Pointer( v );
        m_pFloatReg->x = v->x;
        m_pFloatReg->y = v->y;
        m_pFloatReg->z = v->z;
    }

protected:
    float4* m_pFloatReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float4,float4> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatReg,FlyRenderParameter* param )
    {
        m_pFloatReg = pFloatReg;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        FlyVector* v;
        m_pParam->Pointer( v );
        memcpy( m_pFloatReg,v,sizeof(FlyVector) );
    }

protected:
    float4* m_pFloatReg;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<FlyMatrix,FlyMatrix> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatReg,size_t nRegCount,FlyRenderParameter* param )
    {
        m_pFloatReg = pFloatReg;
        m_nRegSize = nRegCount*sizeof(float4);
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        FlyMatrix* v;
        m_pParam->Pointer( v );
        memcpy( m_pFloatReg,v,m_nRegSize );
    }

protected:
    float4* m_pFloatReg;
    size_t m_nRegSize;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<bool*,bool*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( BOOL4* pBoolRegs,FlyRenderParameter* param )
    {
        m_pBoolRegs = pBoolRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<bool>* v;
        m_pParam->Pointer( v );

        for( size_t i=0;i<v->size();i++ )
        {
            m_pBoolRegs[i].x = static_cast<BOOL>((*v)[i]);
        }
    }

protected:
    BOOL4* m_pBoolRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<int*,bool*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( BOOL4* pBoolRegs,FlyRenderParameter* param )
    {
        m_pBoolRegs = pBoolRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<int>* v;
        m_pParam->Pointer( v );

        for( size_t i=0;i<v->size();i++ )
        {
            m_pBoolRegs[i].x = static_cast<BOOL>((*v)[i]);
        }
    }

protected:
    BOOL4* m_pBoolRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float*,bool*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( BOOL4* pBoolRegs,FlyRenderParameter* param )
    {
        m_pBoolRegs = pBoolRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<float>* v;
        m_pParam->Pointer( v );

        for( size_t i=0;i<v->size();i++ )
        {
            m_pBoolRegs[i].x = static_cast<BOOL>((*v)[i]);
        }
    }

protected:
    BOOL4* m_pBoolRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<bool*,int*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( int4* pIntRegs,FlyRenderParameter* param )
    {
        m_pIntRegs = pIntRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<bool>* v;
        m_pParam->Pointer( v );
        
        for( size_t i=0;i<v->size();i++ )
        {
            m_pIntRegs[i].x = static_cast<int>((*v)[i]);
        }
    }
    
protected:
    int4* m_pIntRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<int*,int*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( int4* pIntRegs,FlyRenderParameter* param )
    {
        m_pIntRegs = pIntRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<int>* v;
        m_pParam->Pointer( v );

        for( size_t i=0;i<v->size();i++ )
        {
            m_pIntRegs[i].x = static_cast<int>((*v)[i]);
        }
    }

protected:
    int4* m_pIntRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float*,int*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( int4* pIntRegs,FlyRenderParameter* param )
    {
        m_pIntRegs = pIntRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<float>* v;
        m_pParam->Pointer( v );

        for( size_t i=0;i<v->size();i++ )
        {
            m_pIntRegs[i].x = static_cast<int>((*v)[i]);
        }
    }

protected:
    int4* m_pIntRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<bool*,float*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatRegs,FlyRenderParameter* param )
    {
        m_pFloatRegs = pFloatRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<bool>* v;
        m_pParam->Pointer( v );
        
        for( size_t i=0;i<v->size();i++ )
        {
            m_pFloatRegs[i].x = static_cast<float>((*v)[i]);
        }
    }

protected:
    float4* m_pFloatRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<int*,float*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatRegs,FlyRenderParameter* param )
    {
        m_pFloatRegs = pFloatRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<int>* v;
        m_pParam->Pointer( v );

        for( size_t i=0;i<v->size();i++ )
        {
            m_pFloatRegs[i].x = static_cast<float>((*v)[i]);
        }
    }

protected:
    float4* m_pFloatRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float*,float*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatRegs,FlyRenderParameter* param )
    {
        m_pFloatRegs = pFloatRegs;
        m_pParam = param;
    }

    // Override the virtual function.
    void ExchangeData(void)
    {
        std::vector<float>* v;
        m_pParam->Pointer( v );

        for( size_t i=0;i<v->size();i++ )
        {
            m_pFloatRegs[i].x = static_cast<float>((*v)[i]);
        }
    }

protected:
    float4* m_pFloatRegs;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<float4*,float4*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatRegs,size_t nRegCount,FlyRenderParameter* param )
    {
        m_nRegSize = nRegCount*sizeof(float4);
        m_pParam = param;
        m_pFloatRegs = pFloatRegs;
    }

    // Override the function.
    void ExchangeData(void)
    {
        std::vector<FlyVector>* v;
        m_pParam->Pointer( v );
        memcpy( &m_pFloatRegs[0],&(*v)[0],min(m_nRegSize,v->size()*sizeof(FlyVector)) );
    }

protected:
    float4* m_pFloatRegs;
    size_t m_nRegSize;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<FlyMatrix*,FlyMatrix*> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( float4* pFloatRegs,size_t nRows,FlyRenderParameter* param )
    {
        m_pFloatRegs = pFloatRegs;
        m_nRows = nRows;
        m_pParam = param;
    }

    // Override the function.
    void ExchangeData(void)
    {
        std::vector<FlyMatrix>* v;
        m_pParam->Pointer( v );

        size_t start = 0;
        size_t length = v->size();
        for( size_t i=0;i<length;i++ )
        {
            memcpy( &m_pFloatRegs[start],&(*v)[i],m_nRows*sizeof(FlyVector) );
            start += m_nRows;
        }
    }

protected:
    float4* m_pFloatRegs;
    size_t m_nRows;
    FlyRenderParameter* m_pParam;
};

template <>
class SetD3DShaderParameter<FlySampler,FlySampler> : public SetShaderParameter
{
public:
    SetD3DShaderParameter( FlySampler* pSampler,FlyRenderParameter* param )
    {
        m_pSampler = pSampler;
        m_pParam = param;
    }

    // Override the function.
    void ExchangeData(void)
    {
        m_pParam->Value( *m_pSampler );
    }

protected:
    FlySampler* m_pSampler;
    FlyRenderParameter* m_pParam;
};


/*************************************************************************
*                                                                        *
*     IMPLEMENTING THE CLASS FLYD3DSHADEROBJECT                          *
*                                                                        *
*************************************************************************/


//------------------------------------------------------------------------
// Constructor of the class
//------------------------------------------------------------------------
FlyD3DShaderObject::FlyD3DShaderObject( LPFLYRENDERDEVICE pDevice )
{
    m_pDevice = pDevice;
    m_pVertexShader = NULL;
    m_pPixelShader = NULL;

    m_bValidate[0] = false;
    m_bValidate[1] = false;
}


//------------------------------------------------------------------------
// Destructor of the class
//------------------------------------------------------------------------
FlyD3DShaderObject::~FlyD3DShaderObject(void)
{
    SAFE_RELEASE( m_pVertexShader );
    SAFE_RELEASE( m_pPixelShader );

    for( size_t i=0;i<m_ParamBinds[0].size();i++ )
    {
        SetShaderParameter* pSetShader = (SetShaderParameter*)m_ParamBinds[0][i].pFuncClass;
        SAFE_DELETE( pSetShader );
    }

    for( size_t i=0;i<m_ParamBinds[1].size();i++ )
    {
        SetShaderParameter* pSetShader = (SetShaderParameter*)m_ParamBinds[1][i].pFuncClass;
        SAFE_DELETE( pSetShader );
    }
}


//------------------------------------------------------------------------
// Create the Shader object for the engine.
// ----------------------------------------------------------------------
// Param -> IN:
//      FlyRenderEffect*:           Pointer to the render effect.
//      int:                        Type of the shader.
//      ShaderDesc*:                Shader description data.
//      const char*:                String of the shader text.
//      UINT:                       Length of the shader code.
//      std::string&:               To store the compile error info.
//------------------------------------------------------------------------
HRESULT FlyD3DShaderObject::CreateShader( FlyRenderEffect* pEffect,int type,ShaderDesc* pDesc,
                                          const char* shader_text,UINT nCodeLen,
                                          std::string& errInfo )
{
    size_t i;

    // Check the shader type.
    if( type != SHADER_VERTEX && type != SHADER_PIXEL )
        return FLY_INVALIDPARAM;

    // Get the Direct3D device pointer.
    LPDIRECT3DDEVICE9 pd3dDevice = ((FlyD3DRenderDevice*)m_pDevice)->GetD3DDevicePtr();

    std::string shader_profile = pDesc->sProfile;
    switch( type )
    {
    case SHADER_VERTEX:
        if( shader_profile == "auto" )
        {
            shader_profile = D3DXGetVertexShaderProfile( pd3dDevice );
        }
        break;
    case SHADER_PIXEL:
        if( shader_profile == "auto" )
        {
            shader_profile = D3DXGetPixelShaderProfile( pd3dDevice );
        }
        break;
    }

    HRESULT hr;
    ID3DXConstantTable* const_table = NULL;
    ID3DXBuffer* code;
    ID3DXBuffer* error_msg;

    // Compile the shader text.
    hr = D3DXCompileShader( shader_text,nCodeLen,NULL,NULL,pDesc->sFuncName.c_str(),
                            shader_profile.c_str(),0,&code,&error_msg,&const_table );
    if( FAILED(hr) || error_msg != NULL )
    {
        errInfo = (char*)error_msg->GetBufferPointer();
        SAFE_RELEASE( const_table );
        SAFE_RELEASE( code );
        SAFE_RELEASE( error_msg );
        return FLY_COMPILEHLSL;
    }

    if( code == NULL )
    {
        SAFE_RELEASE( const_table );
        return FLY_NULLSHADER;
    }
    else
    {
        switch( type )
        {
        case SHADER_VERTEX:
            hr = pd3dDevice->CreateVertexShader( (DWORD*)code->GetBufferPointer(),
                                                 &m_pVertexShader );
            if( FAILED(hr) )
            {
                SAFE_RELEASE( code );
                SAFE_RELEASE( const_table );
                return hr;
            }
            break;
        case SHADER_PIXEL:
            hr = pd3dDevice->CreatePixelShader( (DWORD*)code->GetBufferPointer(),
                                                &m_pPixelShader );
            if( FAILED(hr) )
            {
                SAFE_RELEASE( code );
                SAFE_RELEASE( const_table );
                return hr;
            }
            break;
        }

        code->Release();
    }

    switch( type )
    {
    case SHADER_VERTEX:
        m_Samplers[0].resize( m_pDevice->GetDeviceCaps()->nMaxVertexTextures );
        for( i=0;i<m_Samplers[0].size();i++ )
            m_Samplers[0][i].InitSamplerStates();
        break;
    case SHADER_PIXEL:
        m_Samplers[1].resize( m_pDevice->GetDeviceCaps()->nMaxTextures );
        for( i=0;i<m_Samplers[1].size();i++ )
            m_Samplers[1][i].InitSamplerStates();
        break;
    }

    // Fill in the parameter constant table.
    if( const_table )
    {
        UINT bool_begin = 0xffffffff,bool_end = 0;
        UINT int_begin = 0xffffffff,int_end = 0;
        UINT float_begin = 0xffffffff,float_end = 0;

        D3DXCONSTANTTABLE_DESC ct_desc;
        const_table->GetDesc( &ct_desc );
        for( UINT c=0;c<ct_desc.Constants;c++ )
        {
            D3DXHANDLE handle = const_table->GetConstant( NULL,c );

            UINT count;
            D3DXCONSTANT_DESC const_desc;
            const_table->GetConstantDesc( handle,&const_desc,&count );

            switch( const_desc.RegisterSet )
            {
            case D3DXRS_BOOL:
                bool_begin = min( bool_begin,const_desc.RegisterIndex );
                bool_end = max( bool_end,const_desc.RegisterIndex+const_desc.RegisterCount );
                break;
            case D3DXRS_INT4:
                int_begin = min( int_begin,const_desc.RegisterIndex );
                int_end = max( int_end,const_desc.RegisterIndex+const_desc.RegisterCount );
                break;
            case D3DXRS_FLOAT4:
                float_begin = min( float_begin,const_desc.RegisterIndex );
                float_end = max( float_end,const_desc.RegisterIndex+const_desc.RegisterCount );
                break;
            }
        }

        if( bool_end > bool_begin )
        {
            m_BoolRegisters[type].resize( bool_end-bool_begin );
            m_BoolStart[type] = bool_begin;
        }

        if( int_end > int_begin )
        {
            m_IntRegisters[type].resize( int_end-int_begin );
            m_IntStart[type] = int_begin;
        }

        if( float_end > float_begin )
        {
            m_FloatRegisters[type].resize( float_end-float_begin );
            m_FloatStart[type] = float_begin;
        }

        for( UINT c=0;c<ct_desc.Constants;c++ )
        {
            D3DXHANDLE handle = const_table->GetConstant( NULL,c );

            UINT count;
            D3DXCONSTANT_DESC const_desc;
            const_table->GetConstantDesc( handle,&const_desc,&count );

            D3DShaderParamHandle d3d_handle;
            d3d_handle.ucShaderType = (UCHAR)type;
            d3d_handle.ucRegisterSet = (UCHAR)const_desc.RegisterSet;

            switch( const_desc.RegisterSet )
            {
            case D3DXRS_BOOL:
                d3d_handle.wRegisterIndex = (WORD)(const_desc.RegisterIndex-m_BoolStart[type]);
                break;
            case D3DXRS_INT4:
                d3d_handle.wRegisterIndex = (WORD)(const_desc.RegisterIndex-m_IntStart[type]);
                break;
            case D3DXRS_FLOAT4:
                d3d_handle.wRegisterIndex = (WORD)(const_desc.RegisterIndex-m_FloatStart[type]);
                break;
            default:
                d3d_handle.wRegisterIndex = (WORD)const_desc.RegisterIndex;
                break;
            }

            d3d_handle.wRegisterCount = (WORD)const_desc.RegisterCount;
            d3d_handle.ucRows = (UCHAR)const_desc.Rows;
            d3d_handle.ucColumns = (UCHAR)const_desc.Columns;

            ParameterBind bind;
            bind = MakeBindFunc( d3d_handle,pEffect->GetParameterByName(const_desc.Name) );
            m_ParamBinds[type].push_back( bind );
        }

        const_table->Release();
    }

    m_bValidate[type] = true;

    return FLY_OK;
}


//------------------------------------------------------------------------
// Reset the shader object's content.
//------------------------------------------------------------------------
HRESULT FlyD3DShaderObject::Reset(void)
{
    SAFE_RELEASE( m_pVertexShader );
    SAFE_RELEASE( m_pPixelShader );

    size_t i = 0;
    for( ;i<m_ParamBinds[0].size();i++ )
    {
        SetShaderParameter* pSetShader = (SetShaderParameter*)m_ParamBinds[0][i].pFuncClass;
        SAFE_DELETE( pSetShader );
    }

    for( i=0;i<m_ParamBinds[1].size();i++ )
    {
        SetShaderParameter* pSetShader = (SetShaderParameter*)m_ParamBinds[1][i].pFuncClass;
        SAFE_DELETE( pSetShader );
    }

    m_ParamBinds[0].clear();
    m_ParamBinds[1].clear();

    m_BoolRegisters[0].clear();
    m_BoolRegisters[1].clear();
    m_IntRegisters[0].clear();
    m_IntRegisters[1].clear();
    m_FloatRegisters[0].clear();
    m_FloatRegisters[1].clear();

    return FLY_OK;
}


//------------------------------------------------------------------------
// Bind the parameter of the renderEffect with the shader handle.
// ----------------------------------------------------------------------
// Param -> IN:
//      D3DShaderParamHandle:   Handle to the shader constant table.
//      FlyRenderParameter*:    Pointer to the parameter of the effect.
//------------------------------------------------------------------------
FlyD3DShaderObject::ParameterBind FlyD3DShaderObject::MakeBindFunc( const D3DShaderParamHandle& p_handle,
                                                                    FlyRenderParameter* param )
{
    ParameterBind bind;
    bind.handle = p_handle;
    bind.param = param;

    SetShaderParameter* pFunc;

    switch( param->GetDataType() )
    {
    case REDT_BOOL:
        if( param->GetArraySize() != 1 )
        {
            switch( p_handle.ucRegisterSet )
            {
            case D3DXRS_BOOL:
                pFunc = new SetD3DShaderParameter<bool*,bool*>(
                    &m_BoolRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                    param );
                break;
            case D3DXRS_INT4:
                pFunc = new SetD3DShaderParameter<bool*,int*>(
                    &m_IntRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                    param );
                break;
            case D3DXRS_FLOAT4:
                pFunc = new SetD3DShaderParameter<bool*,float*>(
                    &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                    param );
                break;
            default: assert( false ); break;
            }
        }
        else
        {
            switch( p_handle.ucRegisterSet )
            {
            case D3DXRS_BOOL:
                pFunc = new SetD3DShaderParameter<bool,bool>(
                    &m_BoolRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex].x,
                    param );
                break;
            case D3DXRS_INT4:
                pFunc = new SetD3DShaderParameter<bool,int>(
                    &m_IntRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex].x,
                    param );
                break;
            case D3DXRS_FLOAT4:
                pFunc = new SetD3DShaderParameter<bool,float>(
                    &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex].x,
                    param );
                break;
            default: assert( false ); break;
            }
        }
        break;
    case REDT_DWORD:
    case REDT_INT:
        if( param->GetArraySize() != 1 )
        {
            switch( p_handle.ucRegisterSet )
            {
            case D3DXRS_BOOL:
                pFunc = new SetD3DShaderParameter<int*,bool*>(
                    &m_BoolRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                    param );
                break;
            case D3DXRS_INT4:
                pFunc = new SetD3DShaderParameter<int*,int*>(
                    &m_IntRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                    param );
                break;
            case D3DXRS_FLOAT4:
                pFunc = new SetD3DShaderParameter<int*,float*>(
                    &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                    param );
                break;
            default: assert( false ); break;
            }
        }
        else
        {
            switch( p_handle.ucRegisterSet )
            {
            case D3DXRS_BOOL:
                pFunc = new SetD3DShaderParameter<int,bool>(
                    &m_BoolRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex].x,
                    param );
                break;
            case D3DXRS_INT4:
                pFunc = new SetD3DShaderParameter<int,int>(
                    &m_IntRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex].x,
                    param );
                break;
            case D3DXRS_FLOAT4:
                pFunc = new SetD3DShaderParameter<int,float>(
                    &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex].x,
                    param );
                break;
            default: assert( false ); break;
            }
        }
        break;
    case REDT_FLOAT:
        if( param->GetArraySize() != 1 )
        {
            pFunc = new SetD3DShaderParameter<float*,float*>(
                &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                param );
        }
        else
        {
            pFunc = new SetD3DShaderParameter<float,float>(
                &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex].x,
                param );
        }
        break;
    case REDT_FLOAT2:
        pFunc = new SetD3DShaderParameter<float2,float2>(
                        &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                        param );
        break;
    case REDT_FLOAT3:
        pFunc = new SetD3DShaderParameter<float3,float3>(
                        &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                        param );
        break;
    case REDT_FLOAT4:
        if( param->GetArraySize() != 1 )
            pFunc = new SetD3DShaderParameter<float4*,float4*>(
                            &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                            p_handle.wRegisterCount,param );
        else
            pFunc = new SetD3DShaderParameter<float4,float4>(
                            &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                            param );
        break;
    case REDT_FLOAT4X4:
        if( param->GetArraySize() != 1 )
        {
            pFunc = new SetD3DShaderParameter<FlyMatrix*,FlyMatrix*>(
                &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                p_handle.ucRows,param );
        }
        else
        {
            assert( D3DXRS_FLOAT4 == p_handle.ucRegisterSet );
            pFunc = new SetD3DShaderParameter<FlyMatrix,FlyMatrix>(
                            &m_FloatRegisters[p_handle.ucShaderType][p_handle.wRegisterIndex],
                            p_handle.wRegisterCount,param );
        }
        break;
    case REDT_SAMPLER2D:
    case REDT_SAMPLER3D:
    case REDT_SAMPLERCUBE:
        pFunc = new SetD3DShaderParameter<FlySampler,FlySampler>(
                        &m_Samplers[p_handle.ucShaderType][p_handle.wRegisterIndex],
                        param );
        break;
    default: assert( false ); break;
    }

    bind.pFuncClass = pFunc;
    return bind;
}


//------------------------------------------------------------------------
// Apply the shader object for the render device.
//------------------------------------------------------------------------
HRESULT FlyD3DShaderObject::Apply(void)
{
    // Get the Direct3D device pointer.
    LPDIRECT3DDEVICE9 pd3dDevice = ((FlyD3DRenderDevice*)m_pDevice)->GetD3DDevicePtr();

    // Set the shader pointer.
    m_pDevice->SetShaderObject( this );

    for( int type=SHADER_VERTEX;type<2;type++ )
    {
        if( !m_bValidate[type] ) continue;

        // 1. Update the parameters to shader.
        for( size_t i=0;i<m_ParamBinds[type].size();i++ )
        {
            SetShaderParameter* pSetShader = (SetShaderParameter*)m_ParamBinds[type][i].pFuncClass;
            pSetShader->ExchangeData();
        }

        // 2. Update the bool parameters.
        if( !m_BoolRegisters[type].empty() )
        {
            if( type == SHADER_VERTEX )
            {
                pd3dDevice->SetVertexShaderConstantB( m_BoolStart[type],
                    &m_BoolRegisters[type][0].x,m_BoolRegisters[type].size() );
            }
            else
            {
                pd3dDevice->SetPixelShaderConstantB( m_BoolStart[type],
                    &m_BoolRegisters[type][0].x,m_BoolRegisters[type].size() );
            }
        }

        // 3. Update the int parameters.
        if( !m_IntRegisters[type].empty() )
        {
            if( type == SHADER_VERTEX )
            {
                pd3dDevice->SetVertexShaderConstantI( m_IntStart[type],
                    &m_IntRegisters[type][0].x,m_IntRegisters[type].size() );
            }
            else
            {
                pd3dDevice->SetPixelShaderConstantI( m_IntStart[type],
                    &m_IntRegisters[type][0].x,m_IntRegisters[type].size() );
            }
        }

        // 4. Update the float parameters.
        if( !m_FloatRegisters[type].empty() )
        {
            if( type == SHADER_VERTEX )
            {
                pd3dDevice->SetVertexShaderConstantF( m_FloatStart[type],
                    &m_FloatRegisters[type][0].x,m_FloatRegisters[type].size() );
            }
            else
            {
                pd3dDevice->SetPixelShaderConstantF( m_FloatStart[type],
                    &m_FloatRegisters[type][0].x,m_FloatRegisters[type].size() );
            }
        }

        // 5. Update the sampler parameters.
        for( UINT s=0;s<m_Samplers[type].size();s++ )
        {
            UINT nStage;
            FlySampler* sampler;

            if( type == SHADER_VERTEX )
                nStage = s + D3DVERTEXTEXTURESAMPLER0;
            else
                nStage = s;

            sampler = &m_Samplers[type][s];
            if( !sampler->pTexture )
                m_pDevice->SetTexture( nStage,NULL );
            else
            {
                m_pDevice->SetTexture( nStage,sampler->pTexture );
                m_pDevice->SetTextureAddressingMode( nStage,sampler->AddressU,sampler->AddressV,sampler->AddressW );
                m_pDevice->SetTextureFilterMode( nStage,sampler->MagFilter,sampler->MinFilter,sampler->MipFilter );
            }
        }
    }

    return FLY_OK;
}


//------------------------------------------------------------------------
// Is the vertex shader object validate ?
//------------------------------------------------------------------------
bool FlyD3DShaderObject::IsVSValidate(void)
{
    return m_bValidate[SHADER_VERTEX];
}

//------------------------------------------------------------------------
// Is the pixel shader object validate ?
//------------------------------------------------------------------------
bool FlyD3DShaderObject::IsPSValidate(void)
{
    return m_bValidate[SHADER_PIXEL];
}


//------------------------------------------------------------------------
// Get the Direct3D vertex shader pointer.
//------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 FlyD3DShaderObject::GetD3DVertexShaderPtr(void)
{
    return m_pVertexShader;
}

//------------------------------------------------------------------------
// Get the Direct3D pixel shader pointer.
//------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 FlyD3DShaderObject::GetD3DPixelShaderPtr(void)
{
    return m_pPixelShader;
}