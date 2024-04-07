#pragma once

#include "FortWeaponItemDefinition.h"

class UFortWeaponRangedItemDefinition : public UFortWeaponItemDefinition
{
public:
	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>("/Script/FortniteGame.FortWeaponRangedItemDefinition");
		return Class;
	}
};