//------------------------------------------------------------------------
// Name: FlyRenderEffectInstance.h
// Desc: This file define an instance of the renderEffect.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_RENDEREFFECT_INSTANCE_H__
#define __FLY_RENDEREFFECT_INSTANCE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyRenderEffect.h"

//////////////////////////////////////////////////////////////////////////


/*
* This class define an instance for the renderEffect.
* A RenderEffect is a template of GPU program, and the RenderEffectInstance
* is an instance of the GPU program.
*/
class _FLY_EXPORT FlyRenderEffectInstance
{
    typedef std::vector<FlyRenderParameter*> ParameterList;

protected:
    FlyRenderEffect* m_pEffect;

    // Parameter list of this instance.
    ParameterList m_Parameters;

public:
    // Constructor and destructor.
    FlyRenderEffectInstance( FlyRenderEffect* pRenderEffect );
    ~FlyRenderEffectInstance(void);

    // Update the parameters for the GPU program.
    void UpdateParameters( FlyRenderAutoParameter* pAutoParameter );

    FlyRenderParameter* GetParameterByName( const char* cName );
    FlyRenderParameter* GetParameterByIndex( UINT nIndex );
    FlyRenderParameter* GetParameterBySemantic( const char* cSemantic );

    FlyRenderEffect* GetRenderEffect(void) const;
    void SetRenderEffect( FlyRenderEffect* pEffect );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_RENDEREFFECT_INSTANCE_H__