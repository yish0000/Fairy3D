//------------------------------------------------------------------------
// Name: etParticle.h
// Desc: This file define a particle effect entity for the editor.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_PARTICLE_H__
#define __ET_PARTICLE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etEntity.h"

//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etParticle : public etEntity
{
public:
    // Constructor and destructor.
    etParticle( const char* cName,const char* cParTemplate );
    ~etParticle(void);

    const char* GetTemplateName(void) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_PARTICLE_H__