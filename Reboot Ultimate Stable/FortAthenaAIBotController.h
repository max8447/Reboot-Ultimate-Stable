#pragma once

#include "reboot.h"
#include "AIController.h"
#include "FortInventory.h"
#include "FortAthenaAIBotCustomizationData.h"

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

	UFortAthenaAIBotCustomizationData*& GetBotData()
	{
		static auto BotDataOffset = GetOffset("BotData");
		return Get<UFortAthenaAIBotCustomizationData*>(BotDataOffset);
	}

	UFortAthenaAIBotInventoryItems*& GetStartupInventory()
	{
		static auto StartupInventoryOffset = GetOffset("StartupInventory");
		return Get<UFortAthenaAIBotInventoryItems*>(StartupInventoryOffset);
	}

	static void OnPossesedPawnDiedHook(AFortAthenaAIBotController* PlayerController, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UObject* FHitComponent, FName BoneName, FVector Momentum);

	static UClass* StaticClass();
};