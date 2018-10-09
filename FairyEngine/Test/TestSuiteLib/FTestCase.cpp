/*
 * ------------------------------------------------------------------------
 *  Name:   FTestCase.cpp
 *  Desc:   ���ļ�������һ�����ڽ��е�Ԫ���ԵĿ�ܡ�
 *  Author: Yish
 *  Date:   2010/12/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#include "FTestCase.h"

#pragma warning(disable:4273)

// ȫ�ֱ���
size_t g_nTestCount = 0;            // ���������ĸ���
FTestCase** g_ppTestCases = NULL;   // ��Ԫ�����б�
size_t g_nAllocated = 0;            // �ѷ���Ŀռ�

/** ���ָ���ĵ�Ԫ���Ե��б�
@Param ��Ԫ���Ե�ָ��
*/
static void AddTestCase( FTestCase* pTest )
{
    assert( pTest );

    // ����Ƿ���ڸò�������
    for( size_t i=0;i<g_nTestCount;i++ )
    {
        if( g_ppTestCases[i]->GetName() == pTest->GetName() )
            assert( false && "A same test unit has existed!" );
    }

    // �����µĿռ�
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

/** ��Ԫ���ԵĹ��캯��
@Param ��Ԫ���Ե�����
*/
FTestCase::FTestCase( const std::string& name )
    : m_sName(name)
{
    // ��ӵ������б���
    AddTestCase( this );
}

//////////////////////////////////////////////////////////////////////////

/** ��ȡģ���ڵ�Ԫ���Եĸ���
*/
size_t UnitTestCount(void)
{
    return g_nTestCount;
}

/** ��ȡģ����ָ���ĵ�Ԫ����
*/
FTestCase* GetTestCase( size_t i )
{
    assert( i < g_nTestCount );
    return g_ppTestCases[i];
}

/** ���ģ��Ĳ�������
*/
void ClearUnitTest(void)
{
    free( g_ppTestCases );
    g_ppTestCases = NULL;
    g_nTestCount = 0;
    g_nAllocated = 0;
}