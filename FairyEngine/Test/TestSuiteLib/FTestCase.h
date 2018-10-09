/*
 * ------------------------------------------------------------------------
 *  Name:   FTestCase.h
 *  Desc:   本文件定义了一个用于对DLL进行单元测试的框架。
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

/** 模块测试用例基类
*/
class FTestCase
{
protected:
    std::string m_sName;    // 测试用例的名字

public:
    FTestCase( const std::string& name );
    virtual ~FTestCase(void) {};

    // 运行这个测试用例
    virtual void RunTest(void) = 0;

    // 获取测试用例的名字
    const std::string& GetName(void) const { return m_sName; }
};

// 定义一个声明单元测试用例的宏
#define F_TESTCASE( _case ) \
    class FTC ## _case : public FTestCase \
    { \
    public: \
        FTC ## _case() : FTestCase(#_case) {} \
        void RunTest(void); \
    }; \
    FTC ## _case s_TC ## _case; \
    void FTC ## _case::RunTest(void)

// 定义一个单元测试用例，但不生效
#define F_TESTCASE_NOACTIVE( _case ) \
    class FTC ## _case : public FTestCase \
    { \
    public: \
        FTC ## _case() : FTestCase(#_case) {} \
        void RunTest(void); \
    }; \
    void FTC ## _case::RunTest(void)

// 导出的函数，供测试器使用
UNITTEST_API size_t UnitTestCount(void);
UNITTEST_API FTestCase* GetTestCase( size_t i );
UNITTEST_API void ClearUnitTest(void);

///////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FAIRY_TESTCASE_H__