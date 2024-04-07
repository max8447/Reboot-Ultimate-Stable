#pragma once

#include "reboot.h"
#include "FortItemDefinition.h"
#include "Array.h"

class UFortAthenaAISpawnerDataComponent_AIBotInventory : public UObject
{
public:
	TArray<FItemAndCount> GetItems()
	{
		static auto ItemsOffset = GetOffset("Items");
		return Get<TArray<FItemAndCount>>(ItemsOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>("/Script/FortniteGame.FortAthenaAISpawnerDataComponent_AIBotInventory");
		return Class;
	}
};