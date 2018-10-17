/*
 * ------------------------------------------------------------------------
 *  Name:   main.cpp
 *  Desc:   本程序用于加载DLL模块，并对其进行单元测试。
 *  Author: Yish
 *  Date:   2010/12/9
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FTestCase.h"
#include <windows.h>
#include <time.h>
#include <iostream>

using namespace std;

typedef size_t (*UnitTestCountFunc) (void);
typedef FTestCase* (*GetTestCaseFunc) ( size_t i );
typedef void (*ClearUnitTestFunc) (void);

// 获取当前的日期
const char* getdate(void)
{
    static char s_szDate[80];

    time_t t;
    time( &t );
    tm now;
    localtime_s( &now,&t );

    sprintf_s( s_szDate,80,"%d/%d/%d",now.tm_year+1900,now.tm_mon+1,now.tm_mday );
    return s_szDate;
}

// 程序的入口函数
int main( int argc,char* argv[] )
{
    size_t caseCount = 0;
    WIN32_FIND_DATA data;

    cout << "DLL模块单元测试程序 (" << getdate() << ")" << endl << endl;

    // 遍历当前目录下的所有DLL
    HANDLE hFind = FindFirstFile( "*.dll",&data );

    while( hFind != INVALID_HANDLE_VALUE )
    {
        HMODULE hDll = LoadLibrary( data.cFileName );

		int err = GetLastError();

        UnitTestCountFunc pTestCountFunc = (UnitTestCountFunc)GetProcAddress( hDll,"UnitTestCount" );
        if( pTestCountFunc )
        {
            size_t testCount = pTestCountFunc();

            if( testCount > 0 )
            {
                GetTestCaseFunc pGetTestFunc = (GetTestCaseFunc)GetProcAddress( hDll,"GetTestCase" );

                if( pGetTestFunc )
                {
                    for( size_t i=0;i<testCount;i++ )
                    {
                        FTestCase* pCase = pGetTestFunc( i );

                        cout << "模块(" << data.cFileName << ") 开始测试功能(" << pCase->GetName() << ") ..." << endl;
                        cout << "------------------------------------------------------------" << endl;
                        pCase->RunTest();
                        cout << "------------------------------------------------------------" << endl << endl;

                        caseCount++;
                    }
                }

                // 清除该模块的测试用例
                ClearUnitTestFunc pClearUnitTest = (ClearUnitTestFunc)GetProcAddress( hDll,"ClearUnitTest" );
                pClearUnitTest();
            }
        }

        // 释放模块
        FreeLibrary( hDll );

        if( !FindNextFile(hFind,&data) )
            break;
    }

    // 结束查找
    FindClose( hFind );

    cout << "共测试了" << caseCount << "个单元！" << endl;
    return cin.get();
}