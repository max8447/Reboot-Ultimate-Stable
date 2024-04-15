#pragma once

#include "reboot.h"

class UFortAthenaAISpawnerDataComponent_CosmeticLoadout : public UObject
{
public:
	struct FFortAthenaLoadout* GetCosmeticLoadout()
	{
		static auto CosmeticLoadoutOffset = GetOffset("CosmeticLoadout");
		return GetPtr<struct FFortAthenaLoadout>(CosmeticLoadoutOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAISpawnerDataComponent_CosmeticLoadout");
		return Class;
	}
};