//------------------------------------------------------------------------
// Name: FlyVFilePack.h
// Desc: This file define a base class which can load the resource pack.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_VFILEPACK_H__
#define __FLY_VFILEPACK_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "FlyTypes.h"

//////////////////////////////////////////////////////////////////////////


class _FLY_EXPORT FlyVFilePack
{
public:
    // Constructor and destructor.
    FlyVFilePack(void);
    ~FlyVFilePack(void);
};

//////////////////////////////////////////////////////////////////////////

class _FLY_EXPORT FlyVFilePackFactory
{
public:
    // Constructor and destructor.
    FlyVFilePackFactory(void);
    ~FlyVFilePackFactory(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_VFILEPACK_H__