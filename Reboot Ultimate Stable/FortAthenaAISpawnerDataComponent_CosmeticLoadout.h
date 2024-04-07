#pragma once

#include "reboot.h"
#include "FortPlayerController.h"

class UFortAthenaAISpawnerDataComponent_CosmeticLoadout : public UObject
{
public:
	FFortAthenaLoadout* GetCosmeticLoadout()
	{
		static auto CosmeticLoadoutOffset = GetOffset("CosmeticLoadout");
		return GetPtr<FFortAthenaLoadout>(CosmeticLoadoutOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>("/Script/FortniteGame.FortAthenaAISpawnerDataComponent_CosmeticLoadout");
		return Class;
	}
};