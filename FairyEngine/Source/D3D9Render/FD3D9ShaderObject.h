/*
 * ------------------------------------------------------------------------
 *  Name:   FD3D9ShaderObject.h
 *  Desc:   本文件定义了一个D3D9下的着色器对象。
 *  Author: Yish
 *  Date:   2012/2/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __FD3D9_SHADEROBJECT_H__
#define __FD3D9_SHADEROBJECT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FD3D9Common.h"
#include "F3DShaderObject.h"

///////////////////////////////////////////////////////////////////////////


class FlyRenderParameter;

///////////////////////////////////////////////////////////////////////////

class FD3D9ShaderObject : public F3DShaderObject
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
    F3DRenderDevice* m_pDevice;
    IDirect3DVertexShader9* m_pVertexShader;
    IDirect3DPixelShader9* m_pPixelShader;

    ParameterBindList m_ParamBinds[2];

    UINT m_BoolStart[2];
    UINT m_IntStart[2];
    UINT m_FloatStart[2];
    std::vector<BOOL4> m_BoolRegisters[2];
    std::vector<int4> m_IntRegisters[2];
    std::vector<float4> m_FloatRegisters[2];
    std::vector<SSamplerStage> m_Samplers[2];

    ParameterBind MakeBindFunc( const D3DShaderParamHandle& p_handle,FlyRenderParameter* param );

public:
    // Constructor and destructor.
    FD3D9ShaderObject( LPFLYRENDERDEVICE pDevice );
    ~FD3D9ShaderObject(void);

    // Create the shader object.
    HRESULT CreateShader( FRenderEffect* pEffect,int type,ShaderDesc* desc,
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