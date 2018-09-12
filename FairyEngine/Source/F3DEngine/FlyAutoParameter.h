/*
 * -----------------------------------------------------------------------
 *  Name:   FlyAutoParameter.h
 *  Desc:   ���ļ�Ϊ���涨����һ����ɫ���Զ����²�������
 *  Author: Yish
 *  Date:   2011/5/20
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __FLY_AUTOPARAMETER_H__
#define __FLY_AUTOPARAMETER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyGfxBasis.h"

//////////////////////////////////////////////////////////////////////////


/** �Զ����²�������
*/
enum EAutoParameterType
{
    APT_NONE,
    APT_WORLDMATRIX,
    APT_VIEWMATRIX,
    APT_PROJECTIONMATRIX,
    APT_WORLDVIEWPROJMATRIX,
    APT_WORLDMATRIXARRAY,
    APT_AMBIENTCOLOR,
};

/** �Զ����²���
*/
class _FLY_EXPORT FlyAutoParameter : public GfxResourceAlloc
{
public:
    FlyAutoParameter(void);
    virtual ~FlyAutoParameter(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_AUTOPARAMETER_H__