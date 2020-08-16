/*
 * ------------------------------------------------------------------------
 *  Name:   FSingletonInit.cpp
 *  Desc:   Initialize all the singletons orderly.
 *  Author: Yish
 *  Date:   2014/2/19
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Studio. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FLogManager.h"
#include "FVFileSystem.h"

class FSingletonInit
{
public:
	FSingletonInit()
	{
		FLogManager::GetInstance();
		FVFileSystem::GetInstance();
	}
};

static FSingletonInit l_SingletonInit;