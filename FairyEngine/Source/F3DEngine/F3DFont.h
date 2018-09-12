/*
 * ------------------------------------------------------------------------
 *  Name:   F3DFont.h
 *  Desc:   ���ļ�������һ���������
 *  Author: Yish
 *  Date:   2012/1/14
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#ifndef __F3D_FONT_H__
#define __F3D_FONT_H__


//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "F3DTypes.h"

///////////////////////////////////////////////////////////////////////////


/** �������
@remarks
	ͨ��FreeType�������ʵ��TTF����ļ��ء�
*/
class FAIRY_API F3DFont : public FGeneralAlloc
{
public:		// Types

	struct TEXTPROP
	{
		wchar_t ch;
	};

public:
    F3DFont();
	virtual ~F3DFont() {}
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __F3D_FONT_H__