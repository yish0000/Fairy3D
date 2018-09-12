//------------------------------------------------------------------------
// Name: etCollision.h
// Desc: This file define a class to store the collision data.
// ----------------------------------------------------------------------
// CopyRight (C) YishSoft. All right Observed.
//------------------------------------------------------------------------


#ifndef __ET_COLLISION_H__
#define __ET_COLLISION_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "etCommon.h"
#include "etTerrain.h"

#include "FlyBitset.h"
//////////////////////////////////////////////////////////////////////////


class _ET_EXPORT etCollision
{
protected:
    FlyBitset m_Data;

    FlyTexture* m_pTexture;

public:
    // Constructor and destructor.
    etCollision(void);
    ~etCollision(void);

    HRESULT Initialize( UINT nMapSize );
    void DestroyLayer(void);

    FlyTexture* GetTexture(void) { return m_pTexture; }
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __ET_COLLISION_H__