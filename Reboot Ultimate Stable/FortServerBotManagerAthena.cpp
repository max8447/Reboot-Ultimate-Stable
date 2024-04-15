#include "FortServerBotManagerAthena.h"
#include "bots.h"
#include "KismetTextLibrary.h"

void UFortServerBotManagerAthena::SetupBot(UFortServerBotManagerAthena* BotManager, AFortPlayerPawnAthena* Pawn, UFortAthenaAIBotCustomizationData* InBotData,
	FFortAthenaAIBotRunTimeCustomizationData RuntimeBotData)
{
	__int64 BotManagerAddr = __int64(BotManager);
	__int64 PawnAddr = __int64(Pawn);
	__int64 BehaviorTreeAddr = __int64(InBotData->GetBehaviorTree());
	__int64 StartupInventoryAddr = __int64(InBotData->GetStartupInventory());
	__int64 BotNameSettingsAddr = __int64(InBotData->GetBotNameSettings());

	LOG_INFO(LogBots, "BotManager: {}", BotManagerAddr);
	LOG_INFO(LogBots, "NewBotPawn: {}", PawnAddr);
	LOG_INFO(LogBots, "BehaviorTree: {}", BehaviorTreeAddr);
	LOG_INFO(LogBots, "StartupInventory: {}", StartupInventoryAddr);
	LOG_INFO(LogBots, "BotNameSettings: {}", BotNameSettingsAddr);

	DWORD CustomSquadId;
	if (Fortnite_Version >= 12)
		CustomSquadId = RuntimeBotData.GetCustomSquadId(); // FFortAthenaAIBotRunTimeCustomizationData doesn't exist on S11
	BYTE TrueByte = TRUE;
	BYTE FalseByte = FALSE;

	static void (*SetupBotOriginal)(__int64 BotManager, __int64 Pawn, __int64 BehaviorTree, __int64 a4, DWORD * SkillLevel, __int64 idk, __int64 StartupInventory, __int64 BotNameSettings, 
		__int64 idk_1, BYTE * CanRespawnOnDeath, unsigned __int8 BitFieldDataThing, BYTE * CustomSquadId, FFortAthenaAIBotRunTimeCustomizationData InRuntimeBotData);

	// if (Fortnite_Version == 12.41)
		// BotManagerSetupOriginal = decltype(BotManagerSetupOriginal)(__int64(GetModuleHandleA(0)) + 0x19D93F0); 0x19D93F5
	// else if (Fortnite_Version == 13.40)
		// BotManagerSetupOriginal = decltype(BotManagerSetupOriginal)(__int64(GetModuleHandleA(0)) + 0x1FB6070); // might be 0x1FB5670 0x1FB6070

	if (!SetupBotOriginal)
	{
		LOG_INFO(LogBots, "Manually setting up bot.");

		auto BotNameSettings = InBotData->GetBotNameSettings();
		FString OverrideName = UKismetTextLibrary::Conv_TextToString(BotNameSettings->GetOverrideName());
		Pawn->GetPlayerState()->SetPlayerName(OverrideName);

		return;
	}

	SetupBotOriginal(BotManagerAddr, PawnAddr, BehaviorTreeAddr, 0, &CustomSquadId, 0, StartupInventoryAddr, BotNameSettingsAddr, 0, &FalseByte, 0, &TrueByte, RuntimeBotData);

	LOG_INFO(LogBots, "Finished setting up bot.");
}

AFortPlayerPawnAthena* UFortServerBotManagerAthena::SpawnBotHook(UFortServerBotManagerAthena* BotManager, FVector& InSpawnLocation, FRotator& InSpawnRotation,
	UFortAthenaAIBotCustomizationData* InBotData, FFortAthenaAIBotRunTimeCustomizationData& InRuntimeBotData)
{
	LOG_INFO(LogBots, "SpawnBotHook!");

	if (__int64(_ReturnAddress()) - __int64(GetModuleHandleA(0)) == 0x1A4153F)
	{
		LOG_WARN(LogBots, "Invalid!");
		return SpawnBotOriginal(BotManager, InSpawnLocation, InSpawnRotation, InBotData, InRuntimeBotData);
	}

	FTransform SpawnTransform;
	SpawnTransform.Translation = InSpawnLocation;
	SpawnTransform.Rotation = InSpawnRotation.Quaternion();
	SpawnTransform.Scale3D = FVector(1, 1, 1);

	AActor* SpawnLocator = UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), FindObject<UClass>(L"/Script/Engine.DefaultPawn"), SpawnTransform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn), SpawnTransform);
	AFortPlayerPawnAthena* NewBotPawn = BotManager->GetCachedBotMutator()->SpawnBot(InBotData->GetPawnClass(), SpawnLocator, InSpawnLocation, InSpawnRotation, true);
	AFortAthenaAIBotController* NewBotController = Cast<AFortAthenaAIBotController>(NewBotPawn->GetController());

	if (!NewBotPawn)
	{
		LOG_WARN(LogBots, "Failed to spawn Pawn!");
		return SpawnBotOriginal(BotManager, InSpawnLocation, InSpawnRotation, InBotData, InRuntimeBotData);
	}

	UObject* CharacterToApply = nullptr; // TODO do this like proper

	if (Fortnite_Version < 12)
		CharacterToApply = FindObject("CID_613_Athena_Commando_M_Columbus_7Y4QE", nullptr, ANY_PACKAGE);

	if (!CharacterToApply)
	{
		auto DefaultCharacterName = InBotData->GetCharacterCustomization()->GetCustomizationLoadout()->GetCharacter()->GetName();

		if (DefaultCharacterName != "CID_556_Athena_Commando_F_RebirthDefaultA")
		{
			CharacterToApply = InBotData->GetCharacterCustomization()->GetCustomizationLoadout()->GetCharacter();
		}
		else
		{
			if (InBotData->GetFullName().contains("Alter"))
			{
				CharacterToApply = FindObject("CID_NPC_Athena_Commando_M_HenchmanBad", nullptr, ANY_PACKAGE);
			}
			else if (InBotData->GetFullName().contains("Ego"))
			{
				CharacterToApply = FindObject("CID_NPC_Athena_Commando_M_HenchmanGood", nullptr, ANY_PACKAGE);
			}
			else
			{
				CharacterToApply = FindObject("CID_NPC_Athena_Commando_M_HenchmanGood", nullptr, ANY_PACKAGE);
			}
		}
	}

	static auto HeroDefinitionOffset = CharacterToApply->GetOffset("HeroDefinition");

	if (HeroDefinitionOffset != 1)
	{
		auto HeroDefinition = CharacterToApply->Get(HeroDefinitionOffset);

		if (HeroDefinition)
			ApplyHID(NewBotPawn, HeroDefinition, true);
	}

	TArray<FItemAndCount> StartupInventory;

	if (Fortnite_Version < 13)
	{
		auto StartupInventoryItemsS12 = InBotData->GetStartupInventory()->GetItemsS12();

		for (int i = 0; i < StartupInventoryItemsS12.Num(); ++i)
		{
			FItemAndCount ItemAndCount;
			ItemAndCount.GetCount() = 1;
			ItemAndCount.GetItem() = StartupInventoryItemsS12.at(i);
			StartupInventory.Add(ItemAndCount);
		}
	}
	else
	{
		auto StartupInventoryItemsS13 = InBotData->GetStartupInventory()->GetItemsS13();
		StartupInventory = StartupInventoryItemsS13;
	}

	NewBotController->GetStartupInventory() = InBotData->GetStartupInventory();
	NewBotController->StartupInventory = StartupInventory;

	SpawnLocator->K2_DestroyActor();

	UFortServerBotManagerAthena::SetupBot(BotManager, NewBotPawn, InBotData, InRuntimeBotData);

	LOG_INFO(LogBots, "Finish SpawnBot!");

	return NewBotPawn;
}