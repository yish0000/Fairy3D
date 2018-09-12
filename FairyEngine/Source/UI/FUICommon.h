/*
 * ------------------------------------------------------------------------
 *  Name:   FUICommon.h
 *  Desc:   本文件定义了一些UI库公用的一些代码。
 *  Author: Yish
 *  Date:   2012/8/25
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FUI_COMMON_H__
#define __FUI_COMMON_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FUIBase.h"

///////////////////////////////////////////////////////////////////////////

/** Type of all the controls.
*/
enum FUI_OBJECT_TYPE
{
	FUI_OBJECT_UNKNOWN,

	FUI_OBJECT_BUTTON,
	FUI_OBJECT_CHECKBOX,
	FUI_OBJECT_COMBOBOX,
	FUI_OBJECT_EDITBOX,
	FUI_OBJECT_LABEL,
	FUI_OBJECT_LISTBOX,
	FUI_OBJECT_LISTVIEW,
	FUI_OBJECT_PICTUREBOX,
	FUI_OBJECT_PROGRESS,
	FUI_OBJECT_RADIOBUTTON,
	FUI_OBJECT_SLIDER,
	FUI_OBJECT_TEXTAREA,
	FUI_OBJECT_TREEVIEW,
	FUI_OBJECT_CHILDWINDOW,
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FUI_COMMON_H__