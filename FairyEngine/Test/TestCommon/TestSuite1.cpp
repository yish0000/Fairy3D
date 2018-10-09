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

F_TESTCASE_NOACTIVE(Bitset)
{
	int* pp = new int[5];

	F_Randomize(time(0));
	std::vector<int> arr;
	std::list<AString> lst;
	std::deque<bool> bd;
	std::set<float, std::greater<float> > sss;
	std::multiset<float, std::greater<float> > mss;
	std::unordered_set<float> hss;
	std::map<int, AString, std::greater<int> > mp;
	std::multimap<int, AString, std::less<int> > mmp;
	std::unordered_map<int, AString> hmp;
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