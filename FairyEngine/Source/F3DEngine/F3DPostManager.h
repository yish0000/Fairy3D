/*
 * ------------------------------------------------------------------------
 *  Name:   F3DPostManager.h
 *  Desc:   ���ļ�������F3D�ĺ�����Ч��������
 *  Author: Yish
 *  Date:   2012/4/7
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_POSTMANAGER_H__
#define __F3D_POSTMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"
#include "F3DPostEffect.h"

///////////////////////////////////////////////////////////////////////////


/** ������Ч������
@remarks
@note
    ����һ������
*/
class FAIRY_API F3DPostManager
{
public:
    ~F3DPostManager();

    // Get the singleton.
    static F3DPostManager& GetInstance();

private:
	F3DPostManager();
	F_DISABLE_COPY_ASSIGNMENT(F3DPostManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_POSTMANAGER_H__