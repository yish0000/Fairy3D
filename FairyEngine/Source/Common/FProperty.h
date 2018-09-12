/*
 * ------------------------------------------------------------------------
 *  Name:   FProperty.h
 *  Desc:   本文件定义了一个通用的属性接口。
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

#if 0
/** 属性类型
*/
enum EPropertyType
{
    FPROP_UNKNOWN,

    FPROP_INT,
    FPROP_FLOAT,
    FPROP_STRINGA,
    FPROP_STRINGW,
    FPROP_BOOL,
    FPROP_USERDATA,
};

/** 关联属性类型到枚举值
*/
template <class T> struct FPropertyType
{
    static EPropertyType ms_Type;
};

template <class T> FPropertyType<T>::ms_Type = FPROP_UNKNOWN;
template <> FPropertyType<int>::ms_Type = FPROP_INT;
template <> FPropertyType<float>::ms_Type = FPROP_FLOAT;
template <> FPropertyType<AString>::ms_Type = FPROP_STRINGA;
template <> FPropertyType<WString>::ms_Type = FPROP_STRINGW;
template <> FPropertyType<bool>::ms_Type = FPROP_BOOL;

///////////////////////////////////////////////////////////////////////////

class FPropertySet;

/** 属性对象
*/
class FProperty
{
    typedef void (*FPropertySet::GetterFunc) ( AString& value );
    typedef void (*FPropertySet::SetterFunc) ( const AString& value );

//     union Data
//     {
//         int* m_int;
//         float* m_float;
//         AString* m_stringa;
//         WString* m_stringw;
//         bool* m_bool;
//     };

public:
    FProperty();

    // 传入变量指针作为属性
    template <class T>
    FProperty( T* value );

    // 传入存取函数作为属性
    FProperty( GetterFunc getter, SetterFunc setter );

    // 析构函数
    virtual ~FProperty();

    // 拷贝一个属性对象
    FProperty& Clone( const FProperty& rhs );
    FProperty& operator = ( const FProperty& rhs ) { return Clone(rhs); }

    // 获取指定对象的属性
    template <class T>
    void GetValue( FPropertySet* pObject, T& value )
    {
        FASSERT(FPropertyType<T>::ms_Type == m_type);
        value = *m_data;
    }

    // 为指定对象设置属性
    template <class T>
    void SetValue( FPropertySet* pObject, const T& value )
    {
        FASSERT(FPropertyType<T>::ms_Type == m_type);
        *m_data = value;
    }

    // 

protected:
    EPropertyType m_type;
    T* m_data;
    GetterFunc m_getter;
    SetterFunc m_setter;
};

/** 属性集合
*/
class FPropertySet
{
    typedef std::map<AString, FProperty*> PropertyMap;

public:
    FPropertySet();
    virtual ~FPropertySet();

    bool RegisterProperty( const char* name, FProperty* prop );

    template <class T>
    void GetProperty( const char* name, T& value );
    template <class T>
    void SetProperty( const char* name, const T& value );

    // 
    void GetUnknownTypeProperty(  );

private:
    static PropertyMap ms_Props;
};

/** 属性列表
*/
class FPropertyList
{
public:
    virtual ~FPropertyList();

    // 添加属性
    bool AddProperty( FProperty* pProperty );

protected:
    std::list<FProperty*> m_Properties;
};
#endif

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_PROPERTY_H__