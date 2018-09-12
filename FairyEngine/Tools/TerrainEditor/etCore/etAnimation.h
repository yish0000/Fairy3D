//------------------------------------------------------------------------
// Name: etAnimation.h
// Desc: This file implement an animation entity for the editor.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_ANIMATION_H__
#define __ET_ANIMATION_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etEntity.h"

//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etAnimation : public etEntity
{
public:
    // Constructor and destructor.
    etAnimation( const char* cName,const char* cAnimName );
    ~etAnimation(void);

    // Get the name of the template.
    const char* GetTemplateName(void) const;

    // Set the FPS for the animation.
    void SetAnimFPS( float fFPS );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_ANIMATION_H__