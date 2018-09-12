//------------------------------------------------------------------------
// Name: etLiquid.h
// Desc: This file define an entity to draw the water for the editor.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_LIQUID_H__
#define __ET_LIQUID_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etEntity.h"

//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etLiquid : public etEntity
{
public:
    // Constructor and destructor.
    etLiquid( const char* cName );
    ~etLiquid(void);
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_LIQUID_H__