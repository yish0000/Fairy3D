/*
 * ------------------------------------------------------------------------
 *  Name:   FMacDModule.mm
 *  Desc:   Implement the FDModule for mac.
 *  Author: Yish
 *  Date:   2011/6/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FDModule.h"
#include "FLogManager.h"
#include <dlfcn.h>

class FMacDModule : public FDModule
{
protected:
	void* m_hModule;

public:
	FMacDModule(const AString& moduleName)
		: FDModule(moduleName), m_hModule(NULL)
	{
	}

	~FMacDModule()
	{
		Unload();
	}

	bool Load()
	{
        m_hModule = dlopen(m_sName.c_str(), RTLD_LAZY);
        if( !m_hModule )
        {
            FLOG_ERRORF("FMacDModule::Load, Load the dynamic library(%s) failed!", m_sName.c_str());
            return false;
        }
        
		return true;
	}

	void Unload()
	{
        if( m_hModule )
        {
            dlclose(m_hModule);
            m_hModule = NULL;
        }
	}

	void* GetFunction( const AString& funcName ) const
	{
		return dlsym(m_hModule, funcName.c_str());
	}
};

// Create the dynamic library
FDModule* FDModule::Create(const AString& moduleName)
{
	return new FMacDModule(moduleName);
}