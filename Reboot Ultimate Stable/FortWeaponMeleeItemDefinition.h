#pragma once

#include "FortWeaponItemDefinition.h"
#include "reboot.h"

class UFortWeaponMeleeItemDefinition : public UFortWeaponItemDefinition
{
public:
	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortWeaponMeleeItemDefinition");
		return Class;
	}
};