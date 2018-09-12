/*
 * -----------------------------------------------------------------------
 *  Name:   F3DAutoParameter.h
 *  Desc:   ���ļ�Ϊ���涨����һ����ɫ���Զ����²�������
 *  Author: Yish
 *  Date:   2011/5/20
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#ifndef __F3D_AUTOPARAMETER_H__
#define __F3D_AUTOPARAMETER_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "F3DTypes.h"

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
class FAIRY_API F3DAutoParameter : public FGeneralAlloc
{
public:
    F3DAutoParameter();
	virtual ~F3DAutoParameter();
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_AUTOPARAMETER_H__