/*
 * ------------------------------------------------------------------------
 *  Name:   FBitset.cpp
 *  Desc:   本文件用于实现位集合（按位存储和访问）。
 *  Author: Yish
 *  Date:   2010/11/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. 2010 All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FBitset.h"

/** 初始化指定大小的位集合
@Param 分配内存的位数
*/
void FBitset::Resize( size_t nBitCount )
{
    // 清除旧的数据
    F_SAFE_FREE( m_pData );

    m_nSize = (nBitCount >> 3) + 1;

    // 分配足够的内存
    m_pData = (uchar*)F_MALLOC( m_nSize );

    // 将内存全部清零
    ClearAll();
}

/** 是否存在置为1的二进制位
*/
bool FBitset::Any(void) const
{
    for( size_t i=0;i<m_nSize;i++ )
    {
        if( m_pData[i] ) return true;
    }

    return false;
}

/** 不存在置为1的二进制位
*/
bool FBitset::None(void) const
{
    return !Any();
}

/** 获取置为1的二进制位的个数
*/
size_t FBitset::Count(void) const
{
    size_t num = 0;
    size_t bitCount = m_nSize << 3;

    for( size_t i=0;i<bitCount;i++ )
    {
        if( Test(i) )
            num++;
    }

    return num;
}

/** 访问指定的二进制位
*/
bool FBitset::operator [] ( size_t nBitIndex ) const
{
    return Test( nBitIndex );
}

/** 检测指定的位是否为1
@Param 指定位在集合中的索引
*/
bool FBitset::Test( size_t nBitIndex ) const
{
    FASSERT( (nBitIndex >> 3) < m_nSize );
    return (m_pData[nBitIndex >> 3] & (uchar)(1 << (nBitIndex & 7))) != 0;
}

/** 将位集合的所有位设为1
*/
void FBitset::SetAll(void)
{
    memset( m_pData, 0xff, m_nSize );
}

/** 将制定的位设为1
@Param 指定位在集合中的索引
*/
void FBitset::SetBit( size_t nBitIndex )
{
    FASSERT( (nBitIndex >> 3) < m_nSize );
    m_pData[nBitIndex >> 3] |= (1 << (nBitIndex & 7));
}

/** 将位集合的内存全部清零
*/
void FBitset::ClearAll(void)
{
    memset( m_pData, 0, m_nSize );
}

/** 将指定的位设为0
@Param 指定位在集合中的索引
*/
void FBitset::ClearBit( size_t nBitIndex )
{
    FASSERT( (nBitIndex >> 3) < m_nSize );
    m_pData[nBitIndex >> 3] &= ~(1 << (nBitIndex & 7));
}

/** 将所有二进制位取反
*/
void FBitset::FlipAll(void)
{
    for( size_t i=0; i<m_nSize; i++ )
    {
        m_pData[i] = ~m_pData[i];
    }
}

/** 将指定的二进制位取反
*/
void FBitset::FlipBit( size_t nBitIndex )
{
    FASSERT( (nBitIndex >> 3) < m_nSize );
    m_pData[nBitIndex >> 3] ^= (1 << (nBitIndex & 7));
}

/** 输出比特集的数据
*/
std::ostream& operator << ( std::ostream& o, const FBitset& obj )
{
    o << "bitset(";
    for( size_t i=0;i<obj.Size();i++ )
        o << (int)obj.Test(i);
    o << ")";
    return o;
}