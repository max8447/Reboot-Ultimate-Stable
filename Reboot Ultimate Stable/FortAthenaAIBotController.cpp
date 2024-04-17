#include "FortAthenaAIBotController.h"

#include "bots.h"

void AFortAthenaAIBotController::AddDigestedSkillSets()
{
	for (int i = 0; i < GetDigestedBotSkillSets().Num(); i++)
	{
		auto CurrentDigestedBotSkillSet = GetDigestedBotSkillSets().at(i);

		UClass* AimingDigestedSkillSetClass = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotAimingDigestedSkillSet");
		UClass* HarvestDigestedSkillSetClass = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotHarvestDigestedSkillSet");
		UClass* InventoryDigestedSkillSetClass = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotInventoryDigestedSkillSet");
		UClass* LootingDigestedSkillSetClass = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotLootingDigestedSkillSet");
		UClass* MovementDigestedSkillSetClass = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotMovementDigestedSkillSet");
		UClass* PerceptionDigestedSkillSetClass = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotPerceptionDigestedSkillSet");
		UClass* PlayStyleDigestedSkillSetClass = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotPlayStyleDigestedSkillSet");

		if (CurrentDigestedBotSkillSet->IsA(AimingDigestedSkillSetClass))
			Get("CacheAimingDigestedSkillSet") = CurrentDigestedBotSkillSet;
		if (CurrentDigestedBotSkillSet->IsA(HarvestDigestedSkillSetClass))
			Get("CacheHarvestDigestedSkillSet") = CurrentDigestedBotSkillSet;
		if (CurrentDigestedBotSkillSet->IsA(InventoryDigestedSkillSetClass))
			Get("CacheInventoryDigestedSkillSet") = CurrentDigestedBotSkillSet;
		if (CurrentDigestedBotSkillSet->IsA(LootingDigestedSkillSetClass))
			Get("CacheLootingSkillSet") = CurrentDigestedBotSkillSet;
		if (CurrentDigestedBotSkillSet->IsA(MovementDigestedSkillSetClass))
			Get("CacheMovementSkillSet") = CurrentDigestedBotSkillSet;
		if (CurrentDigestedBotSkillSet->IsA(PerceptionDigestedSkillSetClass))
			Get("CachePerceptionDigestedSkillSet") = CurrentDigestedBotSkillSet;
		if (CurrentDigestedBotSkillSet->IsA(PlayStyleDigestedSkillSetClass))
			Get("CachePlayStyleSkillSet") = CurrentDigestedBotSkillSet;
	}
}

void AFortAthenaAIBotController::OnPossesedPawnDiedHook(AFortAthenaAIBotController* PlayerController, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UObject* FHitComponent, FName BoneName, FVector Momentum)
{
	LOG_INFO(LogDev, "OnPossesedPawnDiedHook!");

	for (auto& PlayerBot : AllPlayerBotsToTick)
	{
		if (!PlayerBot.bShouldTick)
			continue;

		auto CurrentController = PlayerBot.Controller;

		if (!CurrentController)
			continue;

		if (CurrentController == PlayerController)
		{
			AFortPlayerStateAthena* KillerPlayerState = nullptr;

			if (InstigatedBy)
				KillerPlayerState = Cast<AFortPlayerStateAthena>(InstigatedBy->GetPlayerState());

			PlayerBot.OnDied(KillerPlayerState, DamageCauser);
			return;
		}
	}

	auto StartupInventory = PlayerController->StartupInventory;

	std::vector<std::pair<UFortItemDefinition*, int>> ItemsToDrop;

	for (int i = 0; i < StartupInventory.Num(); ++i)
		ItemsToDrop.push_back({ StartupInventory.at(i).GetItem(), StartupInventory.at(i).GetCount() });

	for (int i = 0; i < ItemsToDrop.size(); ++i)
	{
		auto StartupInventoryItem = ItemsToDrop[i];

		if (!StartupInventoryItem.first)
			continue;

		if (auto WorldItemDefinition = Cast<UFortWorldItemDefinition>(StartupInventoryItem.first))
		{
			if (WorldItemDefinition->CanBeDropped())
			{
				PickupCreateData PickupData;
				PickupData.bToss = true;
				PickupData.ItemEntry = FFortItemEntry::MakeItemEntry(WorldItemDefinition, StartupInventoryItem.second, -1, MAX_DURABILITY, WorldItemDefinition->GetFinalLevel(Cast<AFortGameStateAthena>(GetWorld()->GetGameState())->GetWorldLevel()));
				PickupData.SpawnLocation = HitLocation;
				PickupData.SourceType = EFortPickupSourceTypeFlag::GetAIValue();
				PickupData.bRandomRotation = true;
				PickupData.bShouldFreeItemEntryWhenDeconstructed = true;

				AFortPickup::SpawnPickup(PickupData);

				LOG_INFO(LogBots, "ItemToDrop: {}", WorldItemDefinition->GetFullName());

				if (auto WeaponItemDefinition = Cast<UFortWeaponItemDefinition>(WorldItemDefinition))
				{
					if (WeaponItemDefinition->GetAmmoData() && WeaponItemDefinition->GetAmmoData() != WeaponItemDefinition && WeaponItemDefinition->GetAmmoData()->CanBeDropped())
					{
						PickupCreateData PickupData;
						PickupData.bToss = true;
						PickupData.ItemEntry = FFortItemEntry::MakeItemEntry(WeaponItemDefinition->GetAmmoData(), WeaponItemDefinition->GetAmmoData()->GetDropCount(), -1, MAX_DURABILITY, WeaponItemDefinition->GetAmmoData()->GetFinalLevel(Cast<AFortGameStateAthena>(GetWorld()->GetGameState())->GetWorldLevel()));
						PickupData.SpawnLocation = HitLocation;
						PickupData.SourceType = EFortPickupSourceTypeFlag::GetAIValue();
						PickupData.bRandomRotation = true;
						PickupData.bShouldFreeItemEntryWhenDeconstructed = true;

						AFortPickup::SpawnPickup(PickupData);

						LOG_INFO(LogBots, "AmmoToDrop: {}", WeaponItemDefinition->GetAmmoData()->GetFullName());
					}
				}
			}
		}
	}
}

UClass* AFortAthenaAIBotController::StaticClass()
{
	static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotController");
	return Class;
}