//------------------------------------------------------------------------
// Name: etSprite.h
// Desc: This file define a sprite entity for the editor.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_SPRITE_H__
#define __ET_SPRITE_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etEntity.h"

//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etSprite : public etEntity
{
protected:
    std::string m_sTemName;

public:
    // Constructor and destructor.
    etSprite( const char* cName,const char* cTemName );
    ~etSprite(void);

    // Get the name of the template.
    const char* GetTemplateName(void) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_SPIRIT_H__