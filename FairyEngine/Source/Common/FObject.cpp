/*
 * ------------------------------------------------------------------------
 *  Name:   FObject.cpp
 *  Desc:   Base class of all the objects for engine.
 *  Author: Yish
 *  Date:   2018/10/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Shi. All rights Observed.
 * ------------------------------------------------------------------------
 */

#include "FObject.h"

//static FRTTI classFObject("FObject", NULL, NULL, NULL);

/** Constructor of the class.
*/
FObject::FObject()
{
}

/** Get the RTTI of this object.
*/
FRTTI* FObject::GetRTTI() const
{
	return NULL;//&classFObject;
}

/** Get the RTTI of this class.
*/
FRTTI* FObject::GetClassRTTI()
{
	return NULL;// &classFObject;
}

/** Is this object kind of specified type ?
*/
bool FObject::IsKindOf(const FRTTI* pRTTI) const
{
	FRTTI* pRTTIThis = GetRTTI();
	return pRTTIThis ? pRTTIThis->IsDerivedFrom(pRTTI) : false;
}