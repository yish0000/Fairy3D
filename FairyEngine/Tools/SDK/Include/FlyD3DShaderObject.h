//------------------------------------------------------------------------
// Name: FlyD3DShaderObject.h
// Desc: This file is to implement the interface FlyShaderObject.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_D3DSHADEROBJECT_H__
#define __FLY_D3DSHADEROBJECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyShaderObject.h"

#include "FlyUtil.h"
#include "FlyRenderVariable.h"
//////////////////////////////////////////////////////////////////////////


class FlyRenderParameter;

//////////////////////////////////////////////////////////////////////////

class FlyD3DShaderObject : public FlyShaderObject
{
    struct D3DShaderParamHandle
    {
        UCHAR ucShaderType;
        UCHAR ucRegisterSet;
        WORD wRegisterIndex;
        WORD wRegisterCount;
        UCHAR ucRows;
        UCHAR ucColumns;
    };

    struct ParameterBind
    {
        FlyRenderParameter* param;
        D3DShaderParamHandle handle;
        void* pFuncClass;
    };

    typedef std::vector<ParameterBind> ParameterBindList;

protected:
    FlyRenderDevice* m_pDevice;
    IDirect3DVertexShader9* m_pVertexShader;
    IDirect3DPixelShader9* m_pPixelShader;

    ParameterBindList m_ParamBinds[2];

    UINT m_BoolStart[2];
    UINT m_IntStart[2];
    UINT m_FloatStart[2];
    std::vector<BOOL4> m_BoolRegisters[2];
    std::vector<int4> m_IntRegisters[2];
    std::vector<float4> m_FloatRegisters[2];
    std::vector<FlySampler> m_Samplers[2];

    ParameterBind MakeBindFunc( const D3DShaderParamHandle& p_handle,FlyRenderParameter* param );

public:
    // Constructor and destructor.
    FlyD3DShaderObject( LPFLYRENDERDEVICE pDevice );
    ~FlyD3DShaderObject(void);

    // Create the shader object.
    HRESULT CreateShader( FlyRenderEffect* pEffect,int type,ShaderDesc* desc,
                          const char* shader_text,UINT nCodeLen,
                          std::string& errInfo );

    // Apply the shader for device.
    HRESULT Apply(void);

    // Reset the shader content.
    HRESULT Reset(void);

    bool IsVSValidate(void);
    bool IsPSValidate(void);

    LPDIRECT3DVERTEXSHADER9 GetD3DVertexShaderPtr(void);
    LPDIRECT3DPIXELSHADER9 GetD3DPixelShaderPtr(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_D3DSHADEROBJECT_H__