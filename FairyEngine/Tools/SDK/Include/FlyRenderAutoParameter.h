//------------------------------------------------------------------------
// Name: FlyRenderAutoParameter.h
// Desc: This file define a class to update the parameters of shader.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDER_AUTOPARAMETER_H__
#define __FLY_RENDER_AUTOPARAMETER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

#include "FlyLightObject.h"
#include "FlyTechnique.h"
//////////////////////////////////////////////////////////////////////////


enum AUTOPARAMETERTYPE
{
    APT_WORLDMATRIX,
    APT_VIEWMATRIX,
    APT_PROJECTIONMATRIX,
    APT_WORLDVIEWPROJMATRIX,
    APT_WORLDMATRIXARRAY,
    APT_AMBIENTCOLOR,
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyRenderAutoParameter
{
protected:
    FlyMatrix m_mWorld;             // Current world matrix.
    FlyMatrix m_mView;              // Current view matrix.
    FlyMatrix m_mProjection;        // Current projection matrix.

    FlyMatrix m_mWorldViewProj;     // Product of the three matrix, (World,View,projection)
    FlyMatrix m_mWorldViewProjT;    // Transpose the above matrix.
    bool m_bMatrixDirty;            // Need to update the worldViewProj ?

    FLYCOLOR m_cAmbient;            // Current ambient color.
    LightObjectList m_LightList;    // Current light object list.

    FlyRenderPass* m_pPass;         // Pointer to the current render pass.

public:
    // Constructor and destructor.
    FlyRenderAutoParameter(void);
    ~FlyRenderAutoParameter(void);

    const FlyMatrix& GetWorldMatrix(void) const { return m_mWorld; }
    const FlyMatrix& GetViewMatrix(void) const { return m_mView; }
    const FlyMatrix& GetProjectionMatrix(void) const { return m_mProjection; }
    const FlyMatrix& GetWorldViewProjMatrix(void);
    const FlyMatrix& GetWorldViewProjMatrixT(void);

    void SetWorldMatrix( const FlyMatrix& mWorld );
    void SetViewMatrix( const FlyMatrix& mView );
    void SetProjectionMatrix( const FlyMatrix& mProjection );

    const FLYCOLOR& GetAmbientColor(void) const { return m_cAmbient; }
    void SetAmbientColor( const FLYCOLOR& color ) { m_cAmbient = color; }

    void SetLightObject( int nIndex,FlyLightObject* pObject );
    void SetCurrentPass( FlyRenderPass* pPass );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDER_AUTOPARAMETER_H__