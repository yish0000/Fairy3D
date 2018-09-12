//------------------------------------------------------------------------
// Name: etVegetation.h
// Desc: This file define an entity to draw vegetations in the scene.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_VEGETATION_H__
#define __ET_VEGETATION_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etEntity.h"

//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etVegetation : public etEntity
{
protected:
    std::string m_sTemName;

public:
    // Constructor and destructor.
    etVegetation( const char* cName,const char* cVegetationName );
    ~etVegetation(void);

    // Get the name of the template.
    const char* GetTemplateName(void) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_VEGETATION_H__