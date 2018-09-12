/*
 * ------------------------------------------------------------------------
 *  Name:   TestSuite1.cpp
 *  Desc:   本文件实现了一个对Common库的测试套件。
 *  Author: Yish
 *  Date:   2013/12/3
 *  ----------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * ------------------------------------------------------------------------
 */


#include "FTestCase.h"
#include <FBase.h>
#include <FRandom.h>
#include <FStrUtil.h>
#include <FSysAPI.h>
#include <time.h>

int __cdecl TestCall1(int a, int b)
{
	return a + b;
}

int __stdcall TestCall2(int a, int b)
{
	return a + b;
}

// int __thiscall TestCall3(int a, int b)
// {
// 	return a + b;
// }

// int TestCall4(int a, int b)
// {
// 	return a + b;
// }

void Test()
{
	int a = 5;
	int b = 6;
	int c = 0;

	c = TestCall1(a, b);
	c = TestCall2(a, b);
//	c = TestCall3(a, b);
//	c = TestCall4(a, b);
}


F_TESTCASE(Bitset)
{
	Test();

	int* pp = new int[5];

	F_Randomize(time(0));
	ftl::vector<int> arr;
	ftl::list<AString> lst;
	ftl::deque<bool> bd;
	ftl::set<float, std::greater<float> > sss;
	ftl::multiset<float, std::greater<float> > mss;
	ftl::hash_set<float, stdext::hash_compare<float, std::greater<float> > > hss;
	ftl::map<int, AString, std::greater<int> > mp;
	ftl::multimap<int, AString, std::less<int> > mmp;
	ftl::hash_map<int, AString, stdext::hash_compare<int, std::less<int> > > hmp;
	for(int i=0; i<10; i++)
	{
		arr.push_back(F_Rand(1, 100));
		lst.push_back(AStringUtil::Format("TestStr%d", F_Rand(100, 200)));
		bd.push_back(F_Rand(0, 1) ? true : false);
		sss.insert(F_Rand(1.0f, 10.0f));
		mss.insert(F_Rand(1.0f, 10.0f));
		hss.insert(F_Rand(1.0f, 10.0f));
		mp[i] = AStringUtil::Format("Value%d", F_Rand(1, 10));
		mmp.insert(std::pair<const int, AString>(i, AStringUtil::Format("Value%d", F_Rand(100, 200))));
		hmp[i] = AStringUtil::Format("Value%d", F_Rand(40, 80));
	}
	std::cout << arr << std::endl;
	std::cout << lst << std::endl;
	std::cout << bd << std::endl;
	std::cout << sss << std::endl;
	std::cout << mss << std::endl;
	std::cout << hss << std::endl;
	std::cout << mp << std::endl;
	std::cout << mmp << std::endl;
	std::cout << hmp << std::endl;

	char* pBuf = new char[1024];

#ifdef FAIRY_DEBUG
	F_OuputMemoryUsage("mem_usage.txt");
#endif

	printf("Test Bitset!\n");
}