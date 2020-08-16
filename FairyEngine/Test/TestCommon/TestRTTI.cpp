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

F_REFLECTION_CREATE_BEGIN(CWeapon, FObject)
 	F_PROPERTY_FIELD(AString, Name)
	F_PROPERTY_FIELD(int, MinDmg)
	F_PROPERTY_FIELD(int, MaxDmg)
	F_PROPERTY_FIELD(float, AreaOfEffect)
F_REFLECTION_END()

CWeapon::CWeapon()
{
	Name = "Big Ass Whooping Weapon";
	MinDmg = 10;
	MaxDmg = 140;
	AreaOfEffect = 4.5f;
}

///////////////////////////////////////////////////////////////////////////

F_REFLECTION_CREATE_BEGIN(CBaseMonsterStats, FObject)
	F_PROPERTY_FIELD(AString, Name)
F_REFLECTION_END()

CBaseMonsterStats::CBaseMonsterStats()
{
	Name = "Four-legged furry zombie giant";
}

///////////////////////////////////////////////////////////////////////////

F_REFLECTION_CREATE_BEGIN(CMonsterStats, CBaseMonsterStats)
	F_PROPERTY_FIELD(int, Level)
	F_PROPERTY_FIELD(int, HitPoints)
	F_PROPERTY_FIELD(bool, Flatulent)
	F_PROPERTY_FIELD(float, Speed)
	F_REFLECTION_READONLY(CWeapon*, Weapon)
F_REFLECTION_END()

CMonsterStats::CMonsterStats()
{
	Level = 15;
	HitPoints = 4000;
	Speed = 2.75;
	Flatulent = true;
}

///////////////////////////////////////////////////////////////////////////

static void PrintProperties(FObject* pObj)
{
	printf("obj(%s, 0x%p) {\n", pObj->TypeName(), pObj);

	std::vector<FBaseProperty*> properties;
	pObj->EnumProperties(properties);
	for (size_t i = 0; i < properties.size(); i++)
	{
		switch (properties[i]->GetType())
		{
		case FPROP_INT:
		{
			FTypedProperty<int>* prop = (FTypedProperty<int>*)(properties[i]);
			printf("%s:%d\n", prop->GetName(), prop->GetValue(pObj));
			break;
		}
		case FPROP_DWORD:
		{
			FTypedProperty<uint32>* prop = (FTypedProperty<uint32>*)(properties[i]);
			printf("%s:%d\n", prop->GetName(), prop->GetValue(pObj));
			break;
		}
		case FPROP_FLOAT:
		{
			FTypedProperty<float>* prop = (FTypedProperty<float>*)(properties[i]);
			printf("%s:%f\n", prop->GetName(), prop->GetValue(pObj));
			break;
		}
		case FPROP_STRING:
		{
			FTypedProperty<AString>* prop = (FTypedProperty<AString>*)(properties[i]);
			printf("%s:%s\n", prop->GetName(), prop->GetValue(pObj).c_str());
			break;
		}
		case FPROP_BOOL:
		{
			FTypedProperty<bool>* prop = (FTypedProperty<bool>*)(properties[i]);
			printf("%s:%s\n", prop->GetName(), prop->GetValue(pObj) ? "true" : "false");
			break;
		}
		case FPROP_PTR:
		{
			FTypedProperty<FObject*>* prop = (FTypedProperty<FObject*>*)(properties[i]);
			printf("%s:", prop->GetName());
			PrintProperties(prop->GetValue(pObj));
			break;
		}
		default: break;
		}
	}

	printf("}\n");
}

F_TESTCASE(TestRTTI)
{
	FObject* pObj = FRTTI::CreateObject("CMonsterStats");
	printf("pObj is CMonsterStats -> %s\n", pObj->Is<CMonsterStats>() ? "true" : "false");
	printf("pObj is CBaseMonsterStats -> %s\n", pObj->Is<CBaseMonsterStats>() ? "true" : "false");
	printf("pObj is CWeapon -> %s\n", pObj->Is<CWeapon>() ? "true" : "false");
	printf("pObj's type is %s\n", pObj->TypeName());
	PrintProperties(pObj);
	delete pObj;
}