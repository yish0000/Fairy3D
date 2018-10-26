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

	AString	GetName()						{ return m_szName; };
	void	SetName(const AString& szValue)		{ m_szName = szValue; };

	int		GetMinDmg()						{ return m_iMinDmg; };
	void	SetMinDmg(const int& iValue)			{ m_iMinDmg = iValue; };

	int		GetMaxDmg()						{ return m_iMaxDmg; };
	void	SetMaxDmg(const int& iValue)			{ m_iMaxDmg = iValue; };

	float	GetAreaOfEffect()				{ return m_fAreaOfEffect; };
	void	SetAreaOfEffect(const float& fValue)	{ m_fAreaOfEffect = fValue; };

private:
	AString	m_szName;
	int		m_iMinDmg;
	int		m_iMaxDmg;
	float	m_fAreaOfEffect;
};

class CBaseMonsterStats : public FObject
{
	F_DECLARE_REFLECTION_CREATE(CBaseMonsterStats)

public:

	//----------------------------------------------------------------------------------------------
	CBaseMonsterStats();

	//----------------------------------------------------------------------------------------------
	// Sample accessors.
	AString	GetName()					{ return m_szName; };
	void	SetName(const AString& szValue)	{ m_szName = szValue; };

private:

	AString		m_szName;
};

class CMonsterStats : public CBaseMonsterStats
{
	F_DECLARE_REFLECTION_CREATE(CMonsterStats)

public:

	//----------------------------------------------------------------------------------------------
	CMonsterStats();

	//----------------------------------------------------------------------------------------------
	// Sample accessors.
	int		GetHitPoints()				{ return m_iHitPoints; };
	void	SetHitPoints(const int& iValue)	{ m_iHitPoints = iValue; };

	int		GetLevel()					{ return m_iLevel; };
	void	SetLevel(const int& iValue)		{ m_iLevel = iValue; };

	float	GetSpeed()					{ return m_fSpeed; };
	void	SetSpeed(const float& fValue)	{ m_fSpeed = fValue; };

	bool	GetFlatulent()				{ return m_bFlatulent; };
	void	SetFlatulent(const bool& bValue)	{ m_bFlatulent = bValue; };

	CWeapon*	GetWeapon()				{ return &m_Weapon; };

private:

	int			m_iLevel;
	int			m_iHitPoints;
	float		m_fSpeed;
	bool		m_bFlatulent;
	CWeapon		m_Weapon;
};