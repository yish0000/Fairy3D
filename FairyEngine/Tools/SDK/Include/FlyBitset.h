//------------------------------------------------------------------------
// Name: FlyBitset.h
// Desc: This file define a class to store the bit data.
// ----------------------------------------------------------------------
// CopyRight (C) Yish. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_BITSET_H__
#define __FLY_BITSET_H__


//// HEADERS OF THIS FILE ////////////////////////////////////////////////
#include "Fly.h"

//////////////////////////////////////////////////////////////////////////


/*
* This file define a bit array.
*/
class _FLY_EXPORT FlyBitset
{
protected:
    unsigned char* m_pData;
    int m_nSize;

public:
    // Constructor and destructor.
    FlyBitset(void);
    ~FlyBitset(void);

    bool InitBits( int nNumberOfBits );
    void ClearAll(void);
    void SetAll(void);

    void ClearBit( int nBitIndex );
    void SetBit( int nBitIndex );
    unsigned char IsBitSet( int nBitIndex );
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_BITSET_H__