/*
 * ------------------------------------------------------------------------
 *  Name:   FDModule.h
 *  Desc:   Dynamic link library for each platform.
 *  Author: Yish
 *  Date:   2011/6/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_DMODULE_H__
#define __FAIRY_DMODULE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Dynamic link library
*/
class FDModule
{
public:
    // Load the library
    virtual bool Load(void) = 0;

    // Unload the library
    virtual void Unload(void) = 0;

    // Get the specified function
    virtual void* GetFunction( const AString& funcName ) const = 0;

    // Get the library's name
    const AString& GetName(void) const { return m_sName; }

	// Create an instance of this library
	static FDModule* Create(const AString& moduleName);

protected:
	AString m_sName;	// Name of the dynamic library

protected:
	FDModule( const AString& moduleName ) : m_sName(moduleName) {}
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_DMODULE_H__