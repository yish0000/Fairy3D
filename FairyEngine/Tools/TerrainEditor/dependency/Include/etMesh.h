//------------------------------------------------------------------------
// Name: etMesh.h
// Desc: This file implements a mesh entity for the editor.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_MESH_H__
#define __ET_MESH_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etEntity.h"

//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etMesh : public etEntity
{
public:
    // Constructor and destructor.
    etMesh( const char* cName,const char* cMeshName );
    ~etMesh(void);

    // Get the name of the template.
    const char* GetTemplateName(void) const;
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_MESH_H__