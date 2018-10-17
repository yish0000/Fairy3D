/*
 * ------------------------------------------------------------------------
 *  Name:   FObject.h
 *  Desc:   Base class of all the objects for engine.
 *  Author: Yish
 *  Date:   2018/10/12
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Shi. All rights Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_OBJECT_H__
#define __FAIRY_OBJECT_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FRTTI.h"

///////////////////////////////////////////////////////////////////////////

class FObject
{
public:
	virtual ~FObject() {}

	// Enumerate all the properties.
	void EnumProperties(std::vector<FBaseProperty*>& result);

	// Get the RTTI.
	virtual FRTTI* GetRTTI() const;
	static FRTTI* GetClassRTTI();

	// Is this object kind of specified type ?
	bool IsKindOf(const FRTTI* pRTTI) const;

	// Get the name of this type.
	const char* TypeName() const;

	template <class T>
	bool Is() const { return IsKindOf(F_RTTI_OF(T)); }

protected:
	FObject();
	F_DISABLE_COPY_ASSIGNMENT(FObject);
};

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_OBJECT_H__