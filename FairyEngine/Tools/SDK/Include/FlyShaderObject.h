//------------------------------------------------------------------------
// Name: FlyShaderObject.h
// Desc: This file define an interface for the shader object.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_SHADEROBJECT_H__
#define __FLY_SHADEROBJECT_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


class FlyShaderObject
{
public:
    // A structure to descripe the shader object.
    struct ShaderDesc
    {
        std::string sProfile;
        std::string sFuncName;
    };

protected:
    bool m_bValidate[2];

public:
    // Constructor and destructor.
    FlyShaderObject(void) { /* Nothing to do! */ };
    virtual ~FlyShaderObject(void) { /* Nothing to do! */ };

    virtual HRESULT CreateShader( FlyRenderEffect* pEffect,int type,ShaderDesc* desc,
                                  const char* shader_text,UINT nCodeLen,
                                  std::string& errInfo ) = 0;

    // Apply the shader object.
    virtual HRESULT Apply(void) = 0;

    // Reset the shader object.
    virtual HRESULT Reset(void) = 0;

    // Check whether the shader object is validate.
    virtual bool IsVSValidate(void) = 0;
    virtual bool IsPSValidate(void) = 0;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_SHADEROBJECT_H__