/*
 * ------------------------------------------------------------------------
 *  Name:   FRTTI.h
 *  Desc:   This file define the RTTI for the base object of engine.
 *  Author: Yish
 *  Date:   2018/10/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_RTTI_H__
#define __FAIRY_RTTI_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FProperty.h"

///////////////////////////////////////////////////////////////////////////

class FObject;

/** RTTI
*/
class FRTTI
{
public:

	enum
	{
		CLASSNAME_LENGTH = 32,
	};

	typedef FObject* (*ClassFactoryFunc) ();
	typedef void(*RegisterReflectionFunc) ();
	typedef std::list<FBaseProperty*> PropertyList;

public:
	FRTTI(const char* szClassName, FRTTI* pBaseRTTI, ClassFactoryFunc pFactory, RegisterReflectionFunc pReflection);

	// Get all the properties.
	PropertyList& GetProperties() { return m_Properties; }
	PropertyList::iterator GetFirstProperty() { return m_Properties.begin(); }
	PropertyList::iterator GetLastProperty() { return m_Properties.end(); }

	// Fills a vector with all properties of the represented class type, including all ancestor types.
	void EnumProperties(std::vector<FBaseProperty*>& result, bool bIncludeBase = true);

	// Is it derived from the specified type ?
	bool IsDerivedFrom(const FRTTI* pRTTI) const;
	// Get the RTTI of base class.
	FRTTI* GetBaseRTTI() const { return m_pBaseRTTI; }
	// Get the name of class.
	const char* GetName() const { return m_szName; }
	// Get the factory function of this class.
	ClassFactoryFunc GetClassFactory() const { return m_pObjectFactory; }

	// Create an object.
	FObject* CreateObject();

	// Find the RTTI by class name.
	static FRTTI* FromName(const char* name);
	// Create an object by class name.
	static FObject* CreateObject(const char* name);

	// Registers a property. Takes in the property name, its getter and setter functions, and the property
	// type as a template parameter. Should be called from within a user-defined RegisterReflection function.
	template <class T, class PropertyType>
	static void RegisterProperty(const char* szName,
		typename FProperty<T, PropertyType>::GetterFunc Getter,
		typename FProperty<T, PropertyType>::SetterFunc Setter)
	{
		FProperty<T, PropertyType>* pProperty = new FProperty<T, PropertyType>(szName, Getter, Setter);
		T::GetClassRTTI()->GetProperties().push_back(pProperty);
		FPropertyList::GetInstance().AddProperty(pProperty);
	}

	template <class T, class PropertyType>
	static void RegisterProperty(const char* szName, uint32 offset)
	{
		FFieldProperty<T, PropertyType>* pProperty = new FFieldProperty<T, PropertyType>(szName, offset);
		T::GetClassRTTI()->GetProperties().push_back(pProperty);
		FPropertyList::GetInstance().AddProperty(pProperty);
	}

protected:
	char m_szName[CLASSNAME_LENGTH];		// Class name.
	FRTTI* m_pBaseRTTI;                     // RTTI of the base class.
	ClassFactoryFunc m_pObjectFactory;      // Function to create the object.
	PropertyList m_Properties;              // Property list.
	FRTTI* m_pNextRTTI;						// Linked list of registered RTTI.
};

///////////////////////////////////////////////////////////////////////////
//  
//  Some utility macros
//  
///////////////////////////////////////////////////////////////////////////

#define F_DECLARE_RTTI(class_name) \
	protected: \
		static const FRTTI class##class_name; \
	public: \
		static FRTTI* GetClassRTTI(); \
		virtual FRTTI* GetRTTI() const;

#define F_IMPLEMENT_RTTI(class_name, base_class_name) \
	const FRTTI class_name::class##class_name(#class_name, base_class_name::GetClassRTTI(), NULL, NULL); \
	FRTTI* class_name::GetClassRTTI() { return (FRTTI*)&class_name::class##class_name; } \
	FRTTI* class_name::GetRTTI() const { return (FRTTI*)&class_name::class##class_name; } \

#define F_DECLARE_RTTI_CREATE(class_name) \
	F_DECLARE_RTTI(class_name) \
	static FObject* CreateObject();

#define F_IMPLEMENT_RTTI_CREATE(class_name, base_class_name) \
	const FRTTI class_name::class##class_name(#class_name, base_class_name::GetClassRTTI(), class_name::CreateObject, NULL); \
	FRTTI* class_name::GetClassRTTI() { return (FRTTI*)&class_name::class##class_name; } \
	FRTTI* class_name::GetRTTI() const { return (FRTTI*)&class_name::class##class_name; } \
	FObject* class_name::CreateObject() { return new class_name(); }

#define F_DECLARE_REFLECTION(class_name) \
	F_DECLARE_RTTI(class_name) \
	protected: \
		static void RegisterReflection();

#define F_DECLARE_REFLECTION_CREATE(class_name) \
	F_DECLARE_REFLECTION(class_name) \
	public: \
		static FObject* CreateObject();

#define F_REFLECTION_BEGIN(class_name, base_class_name) \
	const FRTTI class_name::class##class_name(#class_name, base_class_name::GetClassRTTI(), NULL, class_name::RegisterReflection); \
	FRTTI* class_name::GetClassRTTI() { return (FRTTI*)&class_name::class##class_name; } \
	FRTTI* class_name::GetRTTI() const { return (FRTTI*)&class_name::class##class_name; } \
	void class_name::RegisterReflection() { \
		typedef class_name T;

#define F_REFLECTION_CREATE_BEGIN(class_name, base_class_name) \
	const FRTTI class_name::class##class_name(#class_name, base_class_name::GetClassRTTI(), class_name::CreateObject, class_name::RegisterReflection); \
	FRTTI* class_name::GetClassRTTI() { return (FRTTI*)&class_name::class##class_name; } \
	FRTTI* class_name::GetRTTI() const { return (FRTTI*)&class_name::class##class_name; } \
	FObject* class_name::CreateObject() { return new class_name(); } \
	void class_name::RegisterReflection() { \
		typedef class_name T;

#define F_REFLECTION(type, name) \
	FRTTI::RegisterProperty<T, type>(#name, &T::Get##name, &T::Set##name);
#define F_REFLECTION_READONLY(type, name) \
	FRTTI::RegisterProperty<T, type>(#name, &T::Get##name, NULL);
#define F_PROPERTY_FIELD(type, name) \
	FRTTI::RegisterProperty<T, type>(#name, F_STRUCT_OFFSET(T, name));

#define F_REFLECTION_END() }

#define F_RTTI_OF(class_name) (class_name::GetClassRTTI())

///////////////////////////////////////////////////////////////////////////

#endif	//#ifndef __FAIRY_RTTI_H__