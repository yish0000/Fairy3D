/*
 * ------------------------------------------------------------------------
 *  Name:   FTestCase.cpp
 *  Desc:   本文件定义了一个用于进行单元测试的框架。
 *  Author: Yish
 *  Date:   2010/12/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FTestCase.h"

#pragma warning(disable:4273)

// 全局变量
size_t g_nTestCount = 0;            // 测试用例的个数
FTestCase** g_ppTestCases = NULL;   // 单元测试列表
size_t g_nAllocated = 0;            // 已分配的空间

/** 添加指定的单元测试到列表
@Param 单元测试的指针
*/
static void AddTestCase( FTestCase* pTest )
{
    assert( pTest );

    // 检查是否存在该测试用例
    for( size_t i=0;i<g_nTestCount;i++ )
    {
        if( g_ppTestCases[i]->GetName() == pTest->GetName() )
            assert( false && "A same test unit has existed!" );
    }

    // 分配新的空间
    if( !g_ppTestCases )
    {
        g_nAllocated = 1;
        g_ppTestCases = (FTestCase**)malloc( sizeof(FTestCase*)*g_nAllocated );
    }
    else
    {
        if( g_nTestCount == g_nAllocated )
            g_nAllocated *= 2;

        g_ppTestCases = (FTestCase**)realloc( g_ppTestCases,sizeof(FTestCase*)*g_nAllocated );
    }

    g_ppTestCases[g_nTestCount] = pTest;
    g_nTestCount++;
}

/** 单元测试的构造函数
@Param 单元测试的名字
*/
FTestCase::FTestCase( const std::string& name )
    : m_sName(name)
{
    // 添加到测试列表中
    AddTestCase( this );
}

//////////////////////////////////////////////////////////////////////////

/** 获取模块内单元测试的个数
*/
size_t UnitTestCount(void)
{
    return g_nTestCount;
}

/** 获取模块内指定的单元测试
*/
FTestCase* GetTestCase( size_t i )
{
    assert( i < g_nTestCount );
    return g_ppTestCases[i];
}

/** 清除模块的测试用例
*/
void ClearUnitTest(void)
{
    free( g_ppTestCases );
    g_ppTestCases = NULL;
    g_nTestCount = 0;
    g_nAllocated = 0;
}