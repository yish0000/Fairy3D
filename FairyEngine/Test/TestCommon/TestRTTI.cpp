/*
 * ------------------------------------------------------------------------
 *  Name:   TestRTTI.cpp
 *  Desc:   本文件用于测试引擎的RTTI功能
 *  Author: Yish
 *  Date:   2018/10/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Shi. All rights Observed.
 * ------------------------------------------------------------------------
 */

#include "FTestCase.h"
#include "TestRTTI.h"

F_REFLECTION_BEGIN(CWeapon, FObject)
 	F_REFLECTION(AString, Name)
	F_REFLECTION(int, MinDmg)
	F_REFLECTION(int, MaxDmg)
	F_REFLECTION(float, AreaOfEffect)
F_REFLECTION_END()

CWeapon::CWeapon()
{
	m_szName = "Big Ass Whooping Weapon";
	m_iMinDmg = 10;
	m_iMaxDmg = 140;
	m_fAreaOfEffect = 4.5f;
}

///////////////////////////////////////////////////////////////////////////

F_REFLECTION_BEGIN(CBaseMonsterStats, FObject)
	F_REFLECTION(AString, Name)
F_REFLECTION_END()

CBaseMonsterStats::CBaseMonsterStats()
{
	m_szName = "Four-legged furry zombie giant";
}

///////////////////////////////////////////////////////////////////////////

F_REFLECTION_BEGIN(CMonsterStats, CBaseMonsterStats)
	F_REFLECTION(int, Level)
	F_REFLECTION(int, HitPoints)
	F_REFLECTION(bool, Flatulent)
	F_REFLECTION(float, Speed)
	F_REFLECTION_READONLY(CWeapon*, Weapon)
F_REFLECTION_END()

CMonsterStats::CMonsterStats()
{
	m_iLevel = 15;
	m_iHitPoints = 4000;
	m_fSpeed = 2.75;
	m_bFlatulent = true;
}

///////////////////////////////////////////////////////////////////////////

F_TESTCASE(TestRTTI)
{
	FObject* pObj = FRTTI::CreateObject("CMonsterStats");
	printf("pObj is CMonsterStats -> %s\n", pObj->Is<CMonsterStats>() ? "true" : "false");
	printf("pObj is CBaseMonsterStats -> %s\n", pObj->Is<CBaseMonsterStats>() ? "true" : "false");
	printf("pObj is CWeapon -> %s\n", pObj->Is<CWeapon>() ? "true" : "false");
	delete pObj;
}