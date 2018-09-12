//------------------------------------------------------------------------
// Name: FlyVFileSystem.h
// Desc: This file define a virtual file system for the engine.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_VFILESYSTEM_H__
#define __FLY_VFILESYSTEM_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyVFileSystem
{
public:
    // Constructor and destructor.
    FlyVFileSystem(void);
    ~FlyVFileSystem(void);

    static FlyVFileSystem& Instance(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_VFILESYSTEM_H__