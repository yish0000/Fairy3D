/*
 * ------------------------------------------------------------------------
 *  Name:   FLinuxDModule.cpp
 *  Desc:   ���ļ�Ϊ����ʵ����һ��Linuxƽ̨�µĶ�̬�����
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
	// ���캯��
	FLinuxDModule(const AString& moduleName)
		: FDModule(moduleName), m_hModule(NULL)
	{
	}

	// ��������
	~FLinuxDModule()
	{
		Unload();
	}

	// ���ض�̬��
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

	// ж�ض�̬��
	void Unload()
	{
		if( m_hModule )
		{
			dlclose( m_hModule );
			m_hModule = NULL;
		}
	}

	// ȡָ���ĺ���ָ��
	void* GetFunction( const AString& funcName ) const
	{
		return (void*)dlsym( m_hModule,funcName.c_str() );
	}
};

// ������̬�����
FDModule* FDModule::Create(const AString& moduleName)
{
	return new FLinuxDModule(moduleName);
}