/*
 * ------------------------------------------------------------------------
 *  Name:   FLinuxDModule.cpp
 *  Desc:   本文件为引擎实现了一个Linux平台下的动态库对象。
 *  Author: Yish
 *  Date:   2011/6/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FDModule.h"
#include "FException.h"
#include "FLogManager.h"
#include <dlfcn.h>


class FLinuxDModule : public FDModule
{
protected:
	void* m_hModule;

public:
	// 构造函数
	FLinuxDModule(const AString& moduleName)
		: FDModule(moduleName), m_hModule(NULL)
	{
	}

	// 析构函数
	~FLinuxDModule()
	{
		Unload();
	}

	// 加载动态库
	bool Load()
	{
		m_hModule = dlopen(m_sName.c_str(), RTLD_LAZY | RTLD_GLOBAL);

		if( !m_hModule )
		{
			strError = AString(dlerror());
			FLOG_ERROR( "FDModule::Load(), " + strError );
			return false;
		}

		return true;
	}

	// 卸载动态库
	void Unload()
	{
		if( m_hModule )
		{
			dlclose( m_hModule );
			m_hModule = NULL;
		}
	}

	// 取指定的函数指针
	void* GetFunction( const AString& funcName ) const
	{
		return (void*)dlsym( m_hModule,funcName.c_str() );
	}
};

// 创建动态库对象
FDModule* FDModule::Create(const AString& moduleName)
{
	return new FLinuxDModule(moduleName);
}