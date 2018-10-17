/*
 * ------------------------------------------------------------------------
 *  Name:   FProperty.cpp
 *  Desc:   This file implements the property system for engine.
 *  Author: Yish
 *  Date:   2013/1/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FProperty.h"

/** Destructor of the class.
*/
FPropertyList::~FPropertyList()
{
	std::list<FBaseProperty*>::iterator it = m_Properties.begin();
	for (; it != m_Properties.end(); ++it)
		delete *it;
	m_Properties.clear();
}

/** Get the instance of property list.
*/
FPropertyList& FPropertyList::GetInstance()
{
	static FPropertyList obj;
	return obj;
}

/** Add a property.
*/
void FPropertyList::AddProperty(FBaseProperty* pProperty)
{
	if (pProperty != NULL)
		m_Properties.push_back(pProperty);
}