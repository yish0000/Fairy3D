/*
 * ------------------------------------------------------------------------
 *  Name:   F3DShaderManager.h
 *  Desc:   ���ļ�Ϊ���涨����һ����ɫ�������������
 *  Author: Yish
 *  Date:   2011/5/21
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_SHADERMANAGER_H__
#define __F3D_SHADERMANAGER_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** ��ɫ�����������
*/
class FAIRY_API F3DShaderManager : public FGeneralAlloc
{
public:
    ~F3DShaderManager();

    // Get the singleton.
    static F3DShaderManager& GetInstance();

private:
	F3DShaderManager();
	F_DISABLE_COPY_ASSIGNMENT(F3DShaderManager);
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_SHADERMANAGER_H__