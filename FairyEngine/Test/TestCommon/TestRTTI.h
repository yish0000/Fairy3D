/*
 * ------------------------------------------------------------------------
 *  Name:   TestRTTI.h
 *  Desc:   本文件用来测试RTTI.
 *  Author: Yish
 *  Date:   2018/10/16
 *  ----------------------------------------------------------------------
 *  CopyRight (C) Yish Shi. All rights Observed.
 * ------------------------------------------------------------------------
 */

#pragma once

#include <FObject.h>

class CWeapon : public FObject
{
	F_DECLARE_REFLECTION_CREATE(CWeapon)

public:
	CWeapon();

private:
	AString	Name;
	int		MinDmg;
	int		MaxDmg;
	float	AreaOfEffect;
};

class CBaseMonsterStats : public FObject
{
	F_DECLARE_REFLECTION_CREATE(CBaseMonsterStats)

public:

	//----------------------------------------------------------------------------------------------
	CBaseMonsterStats();

private:

	AString		Name;
};

class CMonsterStats : public CBaseMonsterStats
{
	F_DECLARE_REFLECTION_CREATE(CMonsterStats)

public:

	//----------------------------------------------------------------------------------------------
	CMonsterStats();

	//----------------------------------------------------------------------------------------------
	// Sample accessors.

	CWeapon*	GetWeapon()				{ return &m_Weapon; };

private:

	int			Level;
	int			HitPoints;
	float		Speed;
	bool		Flatulent;
	CWeapon		m_Weapon;
};