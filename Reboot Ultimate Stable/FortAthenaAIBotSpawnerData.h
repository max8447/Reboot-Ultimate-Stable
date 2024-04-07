#pragma once

#include "reboot.h"
#include "FortAthenaAISpawnerDataComponent_SpawnParams.h"
#include "FortAthenaAISpawnerDataComponent_CosmeticLoadout.h"
#include "FortAthenaAISpawnerDataComponent_AIBotInventory.h"
#include "FortAthenaAISpawnerDataComponent_AIBotGameplay.h"
#include "SubclassOf.h"

class UFortAthenaAIBotSpawnerData : public UObject
{
public:
	TSubclassOf<UFortAthenaAISpawnerDataComponent_SpawnParams>& GetSpawnParamsComponent()
	{
		static auto SpawnParamsComponentOffset = GetOffset("SpawnParamsComponent");
		return Get<TSubclassOf<UFortAthenaAISpawnerDataComponent_SpawnParams>>(SpawnParamsComponentOffset);
	}

	TSubclassOf<UFortAthenaAISpawnerDataComponent_CosmeticLoadout>& GetCosmeticComponent()
	{
		static auto CosmeticComponentOffset = GetOffset("CosmeticComponent");
		return Get<TSubclassOf<UFortAthenaAISpawnerDataComponent_CosmeticLoadout>>(CosmeticComponentOffset);
	}

	TSubclassOf<UFortAthenaAISpawnerDataComponent_AIBotInventory>& GetInventoryComponent()
	{
		static auto InventoryComponentOffset = GetOffset("InventoryComponent");
		return Get<TSubclassOf<UFortAthenaAISpawnerDataComponent_AIBotInventory>>(InventoryComponentOffset);
	}

	TSubclassOf<UFortAthenaAISpawnerDataComponent_AIBotGameplay>& GetGameplayComponent()
	{
		static auto GameplayComponentOffset = GetOffset("GameplayComponent");
		return Get<TSubclassOf<UFortAthenaAISpawnerDataComponent_AIBotGameplay>>(GameplayComponentOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotSpawnerData");
		return Class;
	}
};