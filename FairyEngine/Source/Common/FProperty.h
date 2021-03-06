﻿/*
 * ------------------------------------------------------------------------
 *  Name:   FProperty.h
 *  Desc:   This file define the property system for engine.
 *  Author: Yish
 *  Date:   2013/1/24
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_PROPERTY_H__
#define __FAIRY_PROPERTY_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

class FObject;

/** Data type of property.
*/
enum EPropertyType
{
    FPROP_INT,
	FPROP_DWORD,
    FPROP_FLOAT,
    FPROP_STRING,
    FPROP_BOOL,
    FPROP_PTR,
};

/** Type to type id.
*/
template <class T> struct FPropertyType
{
    static EPropertyType TypeID;
};

template <class T> EPropertyType FPropertyType<T>::TypeID = FPROP_PTR;
template <> EPropertyType FPropertyType<int>::TypeID = FPROP_INT;
template <> EPropertyType FPropertyType<uint32>::TypeID = FPROP_DWORD;
template <> EPropertyType FPropertyType<float>::TypeID = FPROP_FLOAT;
template <> EPropertyType FPropertyType<AString>::TypeID = FPROP_STRING;
template <> EPropertyType FPropertyType<bool>::TypeID = FPROP_BOOL;

///////////////////////////////////////////////////////////////////////////

/** Base property
*/
class FBaseProperty
{
public:
	FBaseProperty(const char* name)
		: m_szName(name)
	{
	}

	// Get the name of property.
	const char* GetName() const { return m_szName; }

	// Get the data type of the property.
	virtual EPropertyType GetType() const = 0;

protected:
	const char* m_szName;
};

/** Typed property.
*/
template <class T>
class FTypedProperty : public FBaseProperty
{
public:
	FTypedProperty(const char* name)
		: FBaseProperty(name)
	{
	}

	// Get the value of this property.
	virtual T GetValue(FObject* pObject) = 0;
	// Set the value of this property.
	virtual void SetValue(FObject* pObject, const T& val) = 0;
	// Is this property readonly ?
	virtual bool IsReadOnly() const = 0;

	// Get the data type of the property.
	virtual EPropertyType GetType() const { return FPropertyType<T>::TypeID; }
};

/** Property (Member of the class)
*/
template <class OwnerType, class T>
class FFieldProperty : public FTypedProperty<T>
{
public:
	FFieldProperty(const char* name, uint32 offset)
		: FTypedProperty<T>(name), m_offset(offset)
	{
	}

	// Get the value of this property
	virtual T GetValue(FObject* pObject)
	{
		return *(T*)((FBYTE*)pObject + m_offset);
	}

	// Set the value of this property.
	virtual void SetValue(FObject* pObject, const T& val)
	{
		*(T*)((FBYTE*)pObject + m_offset) = val;
	}

	// Is this property readonly ?
	virtual bool IsReadOnly() const
	{
		return true;
	}

protected:
	uint32 m_offset;
};

/** Property
*/
template <class OwnerType, class T>
class FProperty : public FTypedProperty<T>
{
public:
	typedef T(OwnerType::*GetterFunc) ();
	typedef void(OwnerType::*SetterFunc) (const T& value);

public:
	FProperty(const char* name, GetterFunc getter, SetterFunc setter)
		: FTypedProperty<T>(name), m_getter(getter), m_setter(setter)
	{
	}

	// Get the value of this property.
	virtual T GetValue(FObject* pObject)
	{
		return (((OwnerType*)pObject)->*m_getter)();
	}

	// Set the value of this property.
	virtual void SetValue(FObject* pObject, const T& val)
	{
		if (!m_setter)
		{
			FASSERT(false); // Cannot write to a read-only property
			return;
		}

		(((OwnerType*)pObject)->*m_setter)(val);
	}

	// Is this property readonly ?
	virtual bool IsReadOnly() const
	{
		return (m_setter == NULL);
	}

protected:
    GetterFunc m_getter;
    SetterFunc m_setter;
};

/** Global manager of all the properties.
*/
class FPropertyList
{
public:
    virtual ~FPropertyList();

	// Get the instance.
	static FPropertyList& GetInstance();

    // Add a property.
	void AddProperty(FBaseProperty* pProperty);

protected:
    std::list<FBaseProperty*> m_Properties;
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_PROPERTY_H__