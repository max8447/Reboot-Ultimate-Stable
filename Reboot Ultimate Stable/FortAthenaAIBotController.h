#pragma once

#include "reboot.h"
#include "AIController.h"
#include "FortInventory.h"

class AFortAthenaAIBotController : public AAIController
{
public:
	static inline void (*OnPossesedPawnDiedOriginal)(AFortAthenaAIBotController* PlayerController, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UObject* FHitComponent, FName BoneName, FVector Momentum);

	TArray<FItemAndCount> StartupInventory; // LOL SKUNKED

	AFortInventory* GetInventory()
	{
		static auto InventoryOffset = GetOffset("Inventory");
		return Get<AFortInventory*>(InventoryOffset);
	}

	class UFortAthenaAIBotCustomizationData*& GetBotData()
	{
		static auto BotDataOffset = GetOffset("BotData");
		return Get<class UFortAthenaAIBotCustomizationData*>(BotDataOffset);
	}

	class UFortAthenaAIBotInventoryItems*& GetStartupInventory()
	{
		static auto StartupInventoryOffset = GetOffset("StartupInventory");
		return Get<class UFortAthenaAIBotInventoryItems*>(StartupInventoryOffset);
	}

	TArray<UObject*> GetDigestedBotSkillSets()
	{
		static auto DigestedBotSkillSetsOffset = GetOffset("DigestedBotSkillSets");
		return Get<TArray<UObject*>>(DigestedBotSkillSetsOffset);
	}

	void AddDigestedSkillSets();

	static void OnPossesedPawnDiedHook(AFortAthenaAIBotController* PlayerController, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UObject* FHitComponent, FName BoneName, FVector Momentum);

	static UClass* StaticClass();
};