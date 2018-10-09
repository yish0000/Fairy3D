/*
 * ------------------------------------------------------------------------
 *  Name:   FTestCase.h
 *  Desc:   ���ļ�������һ�����ڶ�DLL���е�Ԫ���ԵĿ�ܡ�
 *  Author: Yish
 *  Date:   2010/12/8
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */

#ifndef __FAIRY_TESTCASE_H__
#define __FAIRY_TESTCASE_H__

//// HEADERS OF THIS FILE /////////////////////////////////////////////////
#include <string>
#include <assert.h>

///////////////////////////////////////////////////////////////////////////

#if defined(_USRDLL)
	#define UNITTEST_API extern "C" __declspec(dllexport)
#else
	#define UNITTEST_API extern "C" __declspec(dllimport)
#endif

/** ģ�������������
*/
class FTestCase
{
protected:
    std::string m_sName;    // ��������������

public:
    FTestCase( const std::string& name );
    virtual ~FTestCase(void) {};

    // ���������������
    virtual void RunTest(void) = 0;

    // ��ȡ��������������
    const std::string& GetName(void) const { return m_sName; }
};

// ����һ��������Ԫ���������ĺ�
#define F_TESTCASE( _case ) \
    class FTC ## _case : public FTestCase \
    { \
    public: \
        FTC ## _case() : FTestCase(#_case) {} \
        void RunTest(void); \
    }; \
    FTC ## _case s_TC ## _case; \
    void FTC ## _case::RunTest(void)

// ����һ����Ԫ����������������Ч
#define F_TESTCASE_NOACTIVE( _case ) \
    class FTC ## _case : public FTestCase \
    { \
    public: \
        FTC ## _case() : FTestCase(#_case) {} \
        void RunTest(void); \
    }; \
    void FTC ## _case::RunTest(void)

// �����ĺ�������������ʹ��
UNITTEST_API size_t UnitTestCount(void);
UNITTEST_API FTestCase* GetTestCase( size_t i );
UNITTEST_API void ClearUnitTest(void);

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_TESTCASE_H__