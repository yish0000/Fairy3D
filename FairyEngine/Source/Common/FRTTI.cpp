/*
 * ------------------------------------------------------------------------
 *  Name:   FRTTI.cpp
 *  Desc:   This file define the RTTI for the base object of engine.
 *  Author: Yish
 *  Date:   2018/10/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FRTTI.h"

static FRTTI* s_pFirstRTTI = NULL;

/** Constructor of FRTTI
*/
FRTTI::FRTTI(const char* szName, FRTTI* pBaseRTTI, ClassFactoryFunc pFactory,
	RegisterReflectionFunc pReflection)
	: m_pBaseRTTI(pBaseRTTI), m_pObjectFactory(pFactory)
{
	FASSERT(szName && strlen(szName) < CLASSNAME_LENGTH);
	strcpy(m_szName, szName);

	if (pReflection)
		pReflection();

	m_pNextRTTI = s_pFirstRTTI;
	s_pFirstRTTI = this;
}

/** Is it derived from the specified type ?
*/
bool FRTTI::IsDerivedFrom(const FRTTI* pRTTI) const
{
	if (this == pRTTI)
		return true;
	else if (m_pBaseRTTI)
		return m_pBaseRTTI->IsDerivedFrom(pRTTI);

	return false;
}

/** Fills a vector with all properties of the represented class type, including all ancestor types.
*/
void FRTTI::EnumProperties(std::vector<FBaseProperty*>& result)
{
	if (m_pBaseRTTI)
		m_pBaseRTTI->EnumProperties(result);

	PropertyList::iterator it = m_Properties.begin();
	for (; it != m_Properties.end(); ++it)
		result.push_back(*it);
}

/** Create an object of this class.
*/
FObject* FRTTI::CreateObject()
{
	return m_pObjectFactory ? m_pObjectFactory() : NULL;
}

/** Find the RTTI by class name.
*/
FRTTI* FRTTI::FromName(const char* name)
{
	FASSERT_(name, "class name is null!");

	FRTTI* pFirst = s_pFirstRTTI;
	while (pFirst)
	{
		if (!strcmp(pFirst->GetName(), name))
			return pFirst;
		pFirst = pFirst->m_pNextRTTI;
	}

	return NULL;
}

/** Create an object by class name.
*/
FObject* FRTTI::CreateObject(const char* name)
{
	FRTTI* pRTTI = FromName(name);
	return pRTTI ? pRTTI->CreateObject() : NULL;
}