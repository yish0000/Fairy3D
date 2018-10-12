/*
 * ------------------------------------------------------------------------
 *  Name:   FBitset.h
 *  Desc:   本文件定义了一个类用于实现位的存储和访问。
 *  Author: Yish
 *  Date:   2010/11/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_BITSET_H__
#define __FAIRY_BITSET_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include "FBase.h"

///////////////////////////////////////////////////////////////////////////

/** Bit set.
*/
class FBitset
{
protected:
    uchar* m_pData;     // Data buffer.
    size_t m_nSize;     // Size of the buffer.

public:
    // Default constructor.
    FBitset(void) : m_pData(NULL), m_nSize(0) {}

    // Construct a specified size of the bitset.
    FBitset( size_t nBitCount ) { Resize(nBitCount); }

    // Destructor.
    ~FBitset(void) { F_SAFE_FREE(m_pData); }

	// Resize the bitset.
    void Resize( size_t nBitCount );

    // Whether there is a bit which is 1.
    bool Any(void) const;

	// Whether all the bits are 0.
    bool None(void) const;

    // Get the count of bits which is 1.
    size_t Count(void) const;

    // Bit count.
    size_t Size(void) const { return m_nSize << 3; }

	// Is the specified bit 1 ?
    bool operator [] ( size_t nBitIndex ) const;

	// Is the specified bit 1 ?
    bool Test( size_t nBitIndex ) const;

	// Set all the bits to 1.
    void SetAll(void);

	// Set the specified bit to 1.
    void SetBit( size_t nBitIndex );

	// Set all the bits to 0.
    void ClearAll(void);

	// Set the specified bit to 0.
    void ClearBit( size_t nBitIndex );

	// Flip all the bits.
    void FlipAll(void);

	// Flip the specified bit.
    void FlipBit( size_t nBitIndex );

    // Output the data.
    friend std::ostream& operator << ( std::ostream& o, const FBitset& obj );
};

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_BITSET_H__