/*
 * ------------------------------------------------------------------------
 *  Name:   FWinDModule.cpp
 *  Desc:   Dynamic link library for each platform.
 *  Author: Yish
 *  Date:   2011/6/11
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FDModule.h"
#include "FLogManager.h"

class FWinDModule : public FDModule
{
protected:
	HMODULE m_hModule;

public:
	// Constructor.
	FWinDModule(const AString& moduleName)
		: FDModule(moduleName), m_hModule(NULL)
	{
	}

	// Destructor.
	~FWinDModule()
	{
		Unload();
	}

	// Load the dynamic link library.
	bool Load()
	{
		m_hModule = (HMODULE)LoadLibraryA(m_sName.c_str());

		if( !m_hModule )
		{
			// Fail to load the library.
			AString strError;
			LPVOID lpMsgBuf;
			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID( LANG_NEUTRAL,SUBLANG_DEFAULT ),
				(LPSTR)&lpMsgBuf,
				0,
				NULL
				);

			strError = (char*)lpMsgBuf;
			LocalFree( lpMsgBuf );
			FLOG_ERROR( "FDModule::Load(), " + strError );
			return false;
		}

		return true;
	}

	// Unload the dynamic link library.
	void Unload()
	{
		if( m_hModule )
		{
			FreeLibrary( m_hModule );
			m_hModule = NULL;
		}
	}

	// Get the specified function.
	void* GetFunction( const AString& funcName ) const
	{
		return (void*)GetProcAddress( m_hModule,funcName.c_str() );
	}
};

// Create the dynamic library object.
FDModule* FDModule::Create(const AString& moduleName)
{
	return new FWinDModule(moduleName);
}