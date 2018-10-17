/*
 * ------------------------------------------------------------------------
 *  Name:   main.cpp
 *  Desc:   ���������ڼ���DLLģ�飬��������е�Ԫ���ԡ�
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

// ��ȡ��ǰ������
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

// �������ں���
int main( int argc,char* argv[] )
{
    size_t caseCount = 0;
    WIN32_FIND_DATA data;

    cout << "DLLģ�鵥Ԫ���Գ��� (" << getdate() << ")" << endl << endl;

    // ������ǰĿ¼�µ�����DLL
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

                        cout << "ģ��(" << data.cFileName << ") ��ʼ���Թ���(" << pCase->GetName() << ") ..." << endl;
                        cout << "------------------------------------------------------------" << endl;
                        pCase->RunTest();
                        cout << "------------------------------------------------------------" << endl << endl;

                        caseCount++;
                    }
                }

                // �����ģ��Ĳ�������
                ClearUnitTestFunc pClearUnitTest = (ClearUnitTestFunc)GetProcAddress( hDll,"ClearUnitTest" );
                pClearUnitTest();
            }
        }

        // �ͷ�ģ��
        FreeLibrary( hDll );

        if( !FindNextFile(hFind,&data) )
            break;
    }

    // ��������
    FindClose( hFind );

    cout << "��������" << caseCount << "����Ԫ��" << endl;
    return cin.get();
}