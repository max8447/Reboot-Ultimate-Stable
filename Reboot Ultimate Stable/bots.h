#pragma once

#include "FortGameModeAthena.h"
#include "OnlineReplStructs.h"
#include "FortAthenaAIBotController.h"
#include "BuildingContainer.h"
#include "botnames.h"
#include "FortInventory.h"
#include "FortWeaponRangedItemDefinition.h"
#include "GameplayTagContainer.h"
#include "KismetMathLibrary.h"
#include "BuildingFoundation.h"
#include "unordered_set"
#include "gui.h"

class BotPOI
{
	FVector CenterLocation;
	FVector Range; // this just has to be FVector2D
};

class BotPOIEncounter
{
public:
	int NumChestsSearched;
	int NumAmmoBoxesSearched;
	int NumPlayersEncountered;
};

enum class EAIWarmupBehavior : uint8_t
{
	None = 0,
	PlayEmote = 1,
	LootAndShoot = 2,
	Idle = 3,
	EAIWarmupBehavior_MAX = 4
};

class PlayerBot
{
public:
	static inline UClass* PawnClass = nullptr;
	static inline UClass* ControllerClass = nullptr;
	static inline std::map<AActor*, AController*> ClaimedChests;

	AController* Controller = nullptr; // This can be 1. AFortAthenaAIBotController OR AFortPlayerControllerAthena
	AFortPlayerPawnAthena* Pawn = nullptr;
	AFortPlayerStateAthena* PlayerState = nullptr;
	BotPOIEncounter currentBotEncounter;
	int TotalPlayersEncountered;
	std::vector<BotPOI> POIsTraveled;
	float NextJumpTime = 1.0f;
	bool bIsEmoting = false; // afortpawn::isplayingemote (bitfield)
	bool bForceJumpFromAircraft = false;
	bool bCanJumpFromBus = false;
	bool bHasJumpedFromBus = false;
	bool bShouldTick = false;
	EAIWarmupBehavior WarmupBehavior = EAIWarmupBehavior::None;
	AActor* CurrentTargetActor = nullptr;

	void OnPlayerEncountered()
	{
		currentBotEncounter.NumPlayersEncountered++;
		TotalPlayersEncountered++;
	}

	void MoveToNewPOI()
	{

	}

	static bool ShouldUseAIBotController()
	{
		return Fortnite_Version >= 11 && Fortnite_Version < 19; // s15+ use aibotspawnerdata
	}

	static void InitializeBotClasses()
	{
		static auto BlueprintGeneratedClassClass = FindObject<UClass>(L"/Script/Engine.BlueprintGeneratedClass");

		if (!ShouldUseAIBotController())
		{
			PawnClass = FindObject<UClass>(L"/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");
			ControllerClass = AFortPlayerControllerAthena::StaticClass();
		}
		else
		{
			PawnClass = LoadObject<UClass>(L"/Game/Athena/AI/Phoebe/BP_PlayerPawn_Athena_Phoebe.BP_PlayerPawn_Athena_Phoebe_C", BlueprintGeneratedClassClass);
			// ControllerClass = PawnClass->CreateDefaultObject()->GetAIControllerClass();
		}

		if (/* !ControllerClass
			|| */ !PawnClass
			)
		{
			LOG_ERROR(LogBots, "Failed to find a class for the bots!");
			return;
		}
	}

	static bool IsReadyToSpawnBot()
	{
		return PawnClass;
	}

	void SetupInventory()
	{
		auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GetGameState());
		auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->GetGameMode());

		if (!ShouldUseAIBotController()) // TODO REWRITE
		{
			AFortInventory** Inventory = nullptr;

			if (auto FortPlayerController = Cast<AFortPlayerController>(Controller))
			{
				Inventory = &FortPlayerController->GetWorldInventory();
			}
			else
			{
				if (auto FortAthenaAIBotController = Cast<AFortAthenaAIBotController>(Controller))
				{
					static auto InventoryOffset = Controller->GetOffset("Inventory");
					Inventory = Controller->GetPtr<AFortInventory*>(InventoryOffset);
				}
			}

			if (!Inventory)
			{
				LOG_ERROR(LogBots, "No inventory pointer!");

				Pawn->K2_DestroyActor();
				Controller->K2_DestroyActor();
				return;
			}

			static auto FortInventoryClass = FindObject<UClass>(L"/Script/FortniteGame.FortInventory"); // AFortInventory::StaticClass()
			*Inventory = GetWorld()->SpawnActor<AFortInventory>(FortInventoryClass, FTransform{}, CreateSpawnParameters(ESpawnActorCollisionHandlingMethod::AlwaysSpawn, false, Controller));

			if (!*Inventory)
			{
				LOG_ERROR(LogBots, "Failed to spawn Inventory!");

				Pawn->K2_DestroyActor();
				Controller->K2_DestroyActor();
				return;
			}

			(*Inventory)->GetInventoryType() = EFortInventoryType::World;

			if (auto FortPlayerController = Cast<AFortPlayerController>(Controller))
			{
				static auto bHasInitializedWorldInventoryOffset = FortPlayerController->GetOffset("bHasInitializedWorldInventory");
				FortPlayerController->Get<bool>(bHasInitializedWorldInventoryOffset) = true;
			}

			// if (false)
			{
				if (Inventory)
				{
					auto& StartingItems = GameMode->GetStartingItems();

					for (int i = 0; i < StartingItems.Num(); ++i)
					{
						auto& StartingItem = StartingItems.at(i);

						(*Inventory)->AddItem(StartingItem.GetItem(), nullptr, StartingItem.GetCount());
					}

					if (auto FortPlayerController = Cast<AFortPlayerController>(Controller))
					{
						UFortItem* PickaxeInstance = FortPlayerController->AddPickaxeToInventory();

						if (PickaxeInstance)
						{
							FortPlayerController->ServerExecuteInventoryItemHook(FortPlayerController, PickaxeInstance->GetItemEntry()->GetItemGuid());
						}
					}

					(*Inventory)->Update();
				}
			}
		}
	}

	void PickRandomLoadout()
	{
		auto AllHeroTypes = GetAllObjectsOfClass(FindObject<UClass>(L"/Script/FortniteGame.FortHeroType"));
		std::vector<UFortItemDefinition*> AthenaHeroTypes;

		UFortItemDefinition* HeroType = FindObject<UFortItemDefinition>(L"/Game/Athena/Heroes/HID_030_Athena_Commando_M_Halloween.HID_030_Athena_Commando_M_Halloween");

		for (int i = 0; i < AllHeroTypes.size(); ++i)
		{
			auto CurrentHeroType = (UFortItemDefinition*)AllHeroTypes.at(i);

			if (CurrentHeroType->GetPathName().starts_with("/Game/Athena/Heroes/"))
				AthenaHeroTypes.push_back(CurrentHeroType);
		}

		if (AthenaHeroTypes.size())
		{
			HeroType = AthenaHeroTypes.at(std::rand() % AthenaHeroTypes.size());
		}

		static auto HeroTypeOffset = PlayerState->GetOffset("HeroType");
		PlayerState->Get(HeroTypeOffset) = HeroType;
	}

	void ApplyCosmeticLoadout()
	{
		static auto HeroTypeOffset = PlayerState->GetOffset("HeroType");
		const auto CurrentHeroType = PlayerState->Get(HeroTypeOffset);

		if (!CurrentHeroType)
		{
			LOG_WARN(LogBots, "CurrentHeroType called with an invalid HeroType!");
			return;
		}

		ApplyHID(Pawn, CurrentHeroType, true);
	}

	void ChangeName()
	{
		bool bUseOverrideName = false;

		FString NewName;

		if (bUseOverrideName)
		{
			NewName = L"Override";
		}
		else
		{
			static int CurrentBotNum = 1;
			std::wstring BotNumWStr;

			if (Fortnite_Version < 9)
			{
				BotNumWStr = std::to_wstring(CurrentBotNum++);
				NewName = (L"RebootBot" + BotNumWStr).c_str();

				if (auto PlayerController = Cast<AFortPlayerController>(Controller))
					PlayerController->ServerChangeName(NewName);
			}
			else
			{
				if (Fortnite_Version < 11)
				{
					BotNumWStr = std::to_wstring(CurrentBotNum++ + 200);
					NewName = (std::format(L"Anonymous[{}]", BotNumWStr)).c_str();
				}
				else
				{
					if (!PlayerBotNames.empty())
					{
						int RandomIndex = std::rand() % PlayerBotNames.size();
						std::string RandomBotName = PlayerBotNames[RandomIndex];
						NewName = std::wstring(RandomBotName.begin(), RandomBotName.end()).c_str();
						PlayerBotNames.erase(PlayerBotNames.begin() + RandomIndex);
					}
				}

				auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->GetGameMode());
				GameMode->ChangeName(Controller, NewName, true);
			}
		}

		LOG_INFO(LogBots, "NewName: {}", NewName.ToString());
	}

	void ChooseWarmupBehavior()
	{
		if (!ShouldUseAIBotController())
			return;

		static auto FortAthenaAIBotWarmupDigestedSkillSetClass = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotWarmupDigestedSkillSet"); // changes on like ch5 (very bad)
		auto DigestedSkillSets = Cast<AFortAthenaAIBotController>(Controller)->GetDigestedBotSkillSets();

		for (int i = 0; i < DigestedSkillSets.Num(); i++)
		{
			auto CurrentDigestedSkillSet = DigestedSkillSets.at(i);

			if (CurrentDigestedSkillSet->IsA(FortAthenaAIBotWarmupDigestedSkillSetClass))
			{
				static UCurveTable* CurveTable = FindObject<UCurveTable>(L"/Game/Athena/AI/Phoebe/PhoebeSkillSets/Phoebe_SkillSets.Phoebe_SkillSets");

				auto WarmupPlayEmoteBehaviorWeightRowName = UKismetStringLibrary::Conv_StringToName(L"Phoebe.Skill.warmup.PlayEmoteBehaviorWeight");
				auto WarmupLootAndShootBehaviorWeightRowName = UKismetStringLibrary::Conv_StringToName(L"Phoebe.Skill.warmup.LootAndShootBehaviorWeight");
				auto WarmupIdleBehaviorWeightRowName = UKismetStringLibrary::Conv_StringToName(L"Phoebe.Skill.warmup.IdleBehaviorWeight");

				float WarmupPlayEmoteBehaviorWeight = UDataTableFunctionLibrary::EvaluateCurveTableRow(CurveTable, WarmupPlayEmoteBehaviorWeightRowName, 0.f);
				float WarmupLootAndShootBehaviorWeight = UDataTableFunctionLibrary::EvaluateCurveTableRow(CurveTable, WarmupLootAndShootBehaviorWeightRowName, 0.f);
				float WarmupIdleBehaviorWeight = UDataTableFunctionLibrary::EvaluateCurveTableRow(CurveTable, WarmupIdleBehaviorWeightRowName, 0.f);

				while (WarmupBehavior == EAIWarmupBehavior::None || WarmupBehavior == EAIWarmupBehavior::EAIWarmupBehavior_MAX)
				{
					float RandomFloat = UKismetMathLibrary::RandomFloatInRange(0.f, 100.f);

					LOG_INFO(LogDev, "RandomFloat: {}", RandomFloat);

					if (RandomFloat <= WarmupPlayEmoteBehaviorWeight)
						WarmupBehavior = EAIWarmupBehavior::PlayEmote;
					else if (RandomFloat <= WarmupPlayEmoteBehaviorWeight + WarmupLootAndShootBehaviorWeight)
						WarmupBehavior = EAIWarmupBehavior::LootAndShoot;
					else if (RandomFloat <= WarmupPlayEmoteBehaviorWeight + WarmupLootAndShootBehaviorWeight + WarmupIdleBehaviorWeight)
						WarmupBehavior = EAIWarmupBehavior::Idle;

					Sleep(1);
				}

				LOG_INFO(LogBots, "WarmupBehavior: {}", (int)WarmupBehavior);

				break;
			}
		}
	}

	void StartEmoting()
	{
		if (!Controller || !Pawn || bIsEmoting)
			return;

		static auto AthenaDanceItemDefinitionClass = FindObject<UClass>(L"/Script/FortniteGame.AthenaDanceItemDefinition");
		auto RandomDanceID = GetRandomObjectOfClass(AthenaDanceItemDefinitionClass);

		if (!RandomDanceID)
			return;

		auto AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();

		if (!AbilitySystemComponent)
			return;

		UObject* AbilityToUse = nullptr;

		if (!AbilityToUse)
		{
			static auto EmoteGameplayAbilityDefault = FindObject(L"/Game/Abilities/Emotes/GAB_Emote_Generic.Default__GAB_Emote_Generic_C");
			AbilityToUse = EmoteGameplayAbilityDefault;
		}

		if (!AbilityToUse)
			return;

		int outHandle = 0;

		FGameplayAbilitySpec* Spec = MakeNewSpec((UClass*)AbilityToUse, RandomDanceID, true);

		if (!Spec)
			return;

		static unsigned int* (*GiveAbilityAndActivateOnce)(UAbilitySystemComponent * ASC, int* outHandle, __int64 Spec, FGameplayEventData * TriggerEventData) = decltype(GiveAbilityAndActivateOnce)(Addresses::GiveAbilityAndActivateOnce); // EventData is only on ue500?

		if (GiveAbilityAndActivateOnce)
		{
			GiveAbilityAndActivateOnce(AbilitySystemComponent, &outHandle, __int64(Spec), nullptr);
		}

		bIsEmoting = true;
	}

	void LookAtLocation(FVector FocalPoint)
	{
		auto LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), FocalPoint);
		Controller->SetControlRotation(LookAtRotation);
	}

	void MoveInDirection(FVector Direction)
	{
		static auto AddMovementInputFn = FindObject<UFunction>(L"/Script/Engine.Pawn.AddMovementInput");

		struct
		{
			FVector WorldDirection;
			float ScaleValue;
			bool bForce;
		}Pawn_AddMovementInput_Params{ Direction , 1.f , true };

		Pawn->ProcessEvent(AddMovementInputFn, &Pawn_AddMovementInput_Params);
	}

	void MoveToLocation(FVector Destination, bool bShouldLookAtDestination = true)
	{
		MoveInDirection(Destination - Pawn->GetActorLocation());

		if (bShouldLookAtDestination)
			LookAtLocation(Destination);
	}

	AActor* FindNearestAvailableChest()
	{
		static auto TieredChestClass = FindObject<UClass>(L"/Game/Building/ActorBlueprints/Containers/Tiered_Chest_Athena.Tiered_Chest_Athena_C");
		TArray<AActor*> AllTieredChests = UGameplayStatics::GetAllActorsOfClass(GetWorld(), TieredChestClass);

		AActor* NearestChest = nullptr;

		for (int i = 0; i < AllTieredChests.Num(); i++)
		{
			AActor* CurrentChest = AllTieredChests.at(i);

			if (ClaimedChests.contains(CurrentChest) && ClaimedChests[CurrentChest] != Controller)
				continue;

			if (!NearestChest || CurrentChest->GetDistanceTo_Manual(Pawn) < NearestChest->GetDistanceTo_Manual(Pawn))
				NearestChest = CurrentChest;
		}

		AllTieredChests.Free();
		ClaimedChests[NearestChest] = Controller;

		return NearestChest;
	}

	void Initialize(const FTransform& SpawnTransform, AActor* InSpawnLocator)
	{
		auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GetGameState());
		auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->GetGameMode());

		if (!IsReadyToSpawnBot())
		{
			LOG_ERROR(LogBots, "We are not prepared to spawn a bot!");
			return;
		}

		if (!ShouldUseAIBotController())
		{
			Controller = GetWorld()->SpawnActor<AController>(ControllerClass);
			Pawn = GetWorld()->SpawnActor<AFortPlayerPawnAthena>(PawnClass, SpawnTransform, CreateSpawnParameters(ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
			PlayerState = Cast<AFortPlayerStateAthena>(Controller->GetPlayerState());
		}
		else
		{
			Pawn = GameMode->GetServerBotManager()->GetCachedBotMutator()->SpawnBot(PawnClass, InSpawnLocator, SpawnTransform.Translation, SpawnTransform.Rotation.Rotator(), false);
			Controller = Cast<AFortAthenaAIBotController>(Pawn->GetController());
			PlayerState = Cast<AFortPlayerStateAthena>(Controller->GetPlayerState());
		}

		if (!Controller || !Pawn || !PlayerState)
		{
			LOG_ERROR(LogBots, "Failed to spawn controller, pawn or playerstate ({} {})!", bool(__int64(Controller)), bool(__int64(Pawn)), bool(__int64(Controller->GetPlayerState())));
			return;
		}

		if (Fortnite_Version >= 20)
			LOG_INFO(LogBots, "Bot spawned at {}", Pawn->GetActorLocation().ToString().ToString());

		ChangeName();

		if (Addresses::PickTeam)
			PlayerState->GetTeamIndex() = GameMode->Athena_PickTeamHook(GameMode, 0, Controller);

		static auto SquadIdOffset = PlayerState->GetOffset("SquadId", false);

		if (SquadIdOffset != -1)
			PlayerState->GetSquadId() = PlayerState->GetTeamIndex() - NumToSubtractFromSquadId;

		GameState->AddPlayerStateToGameMemberInfo(PlayerState);

		PlayerState->SetIsBot(true);

		if (Controller->GetPawn() != Pawn)
		{
			Controller->Possess(Pawn);
		}

		Pawn->SetHealth(21);
		Pawn->SetMaxHealth(21);
		Pawn->SetShield(21);

		auto PlayerAbilitySet = GetPlayerAbilitySet();
		auto AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();

		if (PlayerAbilitySet)
		{
			PlayerAbilitySet->GiveToAbilitySystem(AbilitySystemComponent);
		}

		SetupInventory();

		if (Fortnite_Version >= 9)
		{
			PickRandomLoadout();
			ApplyCosmeticLoadout();
		}

		if (!ShouldUseAIBotController())
		{
			GameState->GetPlayersLeft()++;
			GameState->OnRep_PlayersLeft();

			if (auto FortPlayerControllerAthena = Cast<AFortPlayerControllerAthena>(Controller))
				GameMode->GetAlivePlayers().Add(FortPlayerControllerAthena);
		}
		else
		{
			if (auto AIBotController = Cast<AFortAthenaAIBotController>(Controller))
				AIBotController->AddDigestedSkillSets();
		}

		bShouldTick = true;

		LOG_INFO(LogDev, "Finished spawning bot!");
	}

	void OnDied(AFortPlayerStateAthena* KillerPlayerState, AActor* DamageCauser, FVector HitLocation = FVector())
	{
		LOG_INFO(LogBots, "Bot Died!");

		bShouldTick = false;

		auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->GetGameMode());
		auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GetGameState());

		if (!GameState->IsRespawningAllowed(PlayerState))
		{
			GameState->GetPlayerBotsLeft()--;
			GameState->OnRep_PlayerBotsLeft();
		}

		AFortPlayerPawn* KillerPawn = nullptr;
		AFortPlayerControllerAthena* KillerPlayerController = nullptr;

		if (KillerPlayerState)
		{
			KillerPlayerController = Cast<AFortPlayerControllerAthena>(KillerPlayerState->GetOwner());

			if (KillerPlayerController)
			{
				KillerPawn = Cast<AFortPlayerPawn>(KillerPlayerController->GetPawn());
			}
		}

		UFortWeaponItemDefinition* KillerWeaponDef = nullptr;

		static auto FortProjectileBaseClass = FindObject<UClass>(L"/Script/FortniteGame.FortProjectileBase");

		if (DamageCauser)
		{
			if (DamageCauser->IsA(FortProjectileBaseClass))
			{
				auto Owner = Cast<AFortWeapon>(DamageCauser->GetOwner());
				KillerWeaponDef = Owner->IsValidLowLevel() ? Owner->GetWeaponData() : nullptr; // I just added the IsValidLowLevel check because what if the weapon destroys (idk)?
			}
			if (auto Weapon = Cast<AFortWeapon>(DamageCauser))
			{
				KillerWeaponDef = Weapon->GetWeaponData();
			}
		}

		uint8_t DeathCause = 0;

		static auto FallDamageEnumValue = 1;

		auto DeathInfo = PlayerState->GetDeathInfo(); // Alloc<void>(DeathInfoStructSize);
		PlayerState->ClearDeathInfo();

		auto Tags = MemberOffsets::FortPlayerPawn::CorrectTags == 0 ? FGameplayTagContainer()
			: Pawn->Get<FGameplayTagContainer>(MemberOffsets::FortPlayerPawn::CorrectTags);

		DeathCause = AFortPlayerStateAthena::ToDeathCause(Tags, false, Pawn); // DeadPawn->IsDBNO() ??

		FGameplayTagContainer CopyTags;

		for (int i = 0; i < Tags.GameplayTags.Num(); ++i)
		{
			CopyTags.GameplayTags.Add(Tags.GameplayTags.at(i));
		}

		for (int i = 0; i < Tags.ParentTags.Num(); ++i)
		{
			CopyTags.ParentTags.Add(Tags.ParentTags.at(i));
		}

		*(bool*)(__int64(DeathInfo) + MemberOffsets::DeathInfo::bDBNO) = Pawn->IsDBNO();
		*(uint8*)(__int64(DeathInfo) + MemberOffsets::DeathInfo::DeathCause) = DeathCause;
		*(AActor**)(__int64(DeathInfo) + MemberOffsets::DeathInfo::FinisherOrDowner) = KillerPlayerState ? KillerPlayerState : PlayerState;

		if (MemberOffsets::DeathInfo::DeathLocation != -1)
			*(FVector*)(__int64(DeathInfo) + MemberOffsets::DeathInfo::DeathLocation) = HitLocation;

		if (MemberOffsets::DeathInfo::DeathTags != -1)
			*(FGameplayTagContainer*)(__int64(DeathInfo) + MemberOffsets::DeathInfo::DeathTags) = CopyTags;

		if (MemberOffsets::DeathInfo::bInitialized != -1)
			*(bool*)(__int64(DeathInfo) + MemberOffsets::DeathInfo::bInitialized) = true;

		if (DeathCause == FallDamageEnumValue)
		{
			if (MemberOffsets::FortPlayerPawnAthena::LastFallDistance != -1)
				*(float*)(__int64(DeathInfo) + MemberOffsets::DeathInfo::Distance) = Pawn->Get<float>(MemberOffsets::FortPlayerPawnAthena::LastFallDistance);
		}
		else
		{
			if (MemberOffsets::DeathInfo::Distance != -1)
				*(float*)(__int64(DeathInfo) + MemberOffsets::DeathInfo::Distance) = KillerPawn ? KillerPawn->GetDistanceTo_Manual(Pawn) : 0;
		}

		if (MemberOffsets::FortPlayerState::PawnDeathLocation != -1)
			PlayerState->Get<FVector>(MemberOffsets::FortPlayerState::PawnDeathLocation) = HitLocation;

		static auto OnRep_DeathInfoFn = FindObject<UFunction>(L"/Script/FortniteGame.FortPlayerStateAthena.OnRep_DeathInfo");

		if (OnRep_DeathInfoFn)
		{
			PlayerState->ProcessEvent(OnRep_DeathInfoFn);
		}

		if (KillerPlayerState && KillerPlayerState != PlayerState)
		{
			if (MemberOffsets::FortPlayerStateAthena::KillScore != -1)
				KillerPlayerState->Get<int>(MemberOffsets::FortPlayerStateAthena::KillScore)++;

			if (MemberOffsets::FortPlayerStateAthena::TeamKillScore != -1)
				KillerPlayerState->Get<int>(MemberOffsets::FortPlayerStateAthena::TeamKillScore)++;

			KillerPlayerState->ClientReportKill(PlayerState);
		}

		if (KillerPawn && KillerPawn != Pawn)
		{
			KillerPawn->AttemptSiphonHealAndMats();
			KillerPawn->ApplySiphonEffect();
		}

		for (int i = 0; i < GameMode->GetAliveBots().Num(); i++)
		{
			if (GameMode->GetAliveBots().at(i) == Controller)
				GameMode->GetAliveBots().Remove(i);
		}

		LOG_INFO(LogDev, "Removed!");

		std::unordered_set<int> AliveTeamIndexes;

		for (int i = 0; i < GameMode->GetAlivePlayers().Num(); i++)
		{
			auto AlivePlayer = GameMode->GetAlivePlayers().at(i);

			if (!AlivePlayer)
				continue;

			if (AlivePlayer != Controller)
			{
				auto TeamIndex = AlivePlayer->GetPlayerStateAthena()->GetTeamIndex();

				AliveTeamIndexes.emplace(TeamIndex);
			}
		}

		for (int i = 0; i < GameMode->GetAliveBots().Num(); i++)
		{
			auto AliveBot = GameMode->GetAliveBots().at(i);

			if (!AliveBot)
				continue;

			if (AliveBot != Controller)
			{
				auto TeamIndex = Cast<AFortPlayerStateAthena>(AliveBot->GetPlayerState())->GetTeamIndex();
				AliveTeamIndexes.emplace(TeamIndex);
			}
			else
			{
				LOG_INFO(LogBots, "AliveBots: {}", GameMode->GetAliveBots().Num());
				GameMode->GetAliveBots().Remove(i);
			}
		}

		if (GameMode->GetAliveBots().Num() > 0 && false) // dk what i was cooking here tbh
		{
			if (!AliveTeamIndexes.empty())
			{
				auto Last = AliveTeamIndexes.begin();
				std::advance(Last, std::distance(AliveTeamIndexes.begin(), AliveTeamIndexes.end()) - 1);
				AliveTeamIndexes.erase(Last);
			}
		}

		if (AliveTeamIndexes.size() <= 1 && bStartedBus /* && GameMode->IsMatchInProgress() // will readd later */)
		{
			auto ClientConnections = GetWorld()->GetNetDriver()->GetClientConnections();

			for (int i = 0; i < ClientConnections.Num(); i++)
			{
				auto CurrentPlayerController = Cast<AFortPlayerControllerAthena>(ClientConnections.at(i)->GetPlayerController());

				if (!CurrentPlayerController || CurrentPlayerController == KillerPlayerController)
					continue;

				auto CurrentPawn = CurrentPlayerController->GetMyFortPawn();

				if (!CurrentPawn || CurrentPawn == KillerPawn)
					continue;

				CurrentPlayerController->PlayWinEffects(CurrentPawn, CurrentPawn->GetCurrentWeapon()->GetWeaponData(), DeathCause, false);
				CurrentPlayerController->ClientNotifyWon(CurrentPawn, CurrentPawn->GetCurrentWeapon()->GetWeaponData(), DeathCause);
				CurrentPlayerController->ClientNotifyTeamWon(CurrentPawn, CurrentPawn->GetCurrentWeapon()->GetWeaponData(), DeathCause);
			}

			if (KillerPlayerController)
			{
				KillerPlayerController->PlayWinEffects(KillerPawn, KillerWeaponDef, DeathCause, false);
				KillerPlayerController->ClientNotifyWon(KillerPawn, KillerWeaponDef, DeathCause);
				KillerPlayerController->ClientNotifyTeamWon(KillerPawn, KillerWeaponDef, DeathCause);
			}

			if (GameState->GetOffset("WinningPlayerState") != -1)
				GameState->Get<APlayerState*>("WinningPlayerState") = KillerPlayerState;

			if (GameState->GetOffset("WinningTeam") != -1)
				GameState->Get<uint8>("WinningTeam") = KillerPlayerState ? KillerPlayerState->GetTeamIndex() : 0;

			if (GameState->FindFunction("OnRep_WinningPlayerState"))
				GameState->ProcessEvent(GameState->FindFunction("OnRep_WinningPlayerState"));

			if (GameState->FindFunction("OnRep_WinningTeam"))
				GameState->ProcessEvent(GameState->FindFunction("OnRep_WinningTeam"));

			GameMode->EndMatch();
		}
	}
};

inline std::vector<PlayerBot> AllPlayerBotsToTick;

namespace Bots
{
	static AController* SpawnBot(FTransform SpawnTransform, AActor* InSpawnLocator)
	{
		auto playerBot = PlayerBot();
		playerBot.Initialize(SpawnTransform, InSpawnLocator);
		AllPlayerBotsToTick.push_back(playerBot);
		return playerBot.Controller;
	}

	static void SpawnBotsAtPlayerStarts(int AmountOfBots)
	{
		return;

		auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GetGameState());
		auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->GetGameMode());

		static auto FortPlayerStartCreativeClass = FindObject<UClass>(L"/Script/FortniteGame.FortPlayerStartCreative");
		static auto FortPlayerStartWarmupClass = FindObject<UClass>(L"/Script/FortniteGame.FortPlayerStartWarmup");
		TArray<AActor*> PlayerStarts = UGameplayStatics::GetAllActorsOfClass(GetWorld(), Globals::bCreative ? FortPlayerStartCreativeClass : FortPlayerStartWarmupClass);

		int ActorsNum = PlayerStarts.Num();

		// Actors.Free();

		if (ActorsNum == 0)
		{
			// LOG_INFO(LogDev, "No Actors!");
			return;
		}

		// Find playerstart (scuffed)

		for (int i = 0; i < AmountOfBots; ++i)
		{
			AActor* PlayerStart = PlayerStarts.at(std::rand() % (PlayerStarts.size() - 1));

			if (!PlayerStart)
			{
				return;
			}

			auto NewBot = SpawnBot(PlayerStart->GetTransform(), PlayerStart);
			NewBot->SetCanBeDamaged(Fortnite_Version < 7); // idk lol for spawn island
		}

		return;
	}

	static void Tick()
	{
		if (AllPlayerBotsToTick.size() == 0)
			return;

		auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GetGameState());
		auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->GetGameMode());

		for (auto& PlayerBot : AllPlayerBotsToTick)
		{
			if (!PlayerBot.bShouldTick)
				continue;

			auto CurrentPlayer = PlayerBot.Controller;

			if (CurrentPlayer->IsActorBeingDestroyed())
				continue;

			auto CurrentPawn = PlayerBot.Pawn;

			if (CurrentPawn->IsActorBeingDestroyed())
				continue;

			auto CurrentPlayerState = Cast<AFortPlayerStateAthena>(CurrentPlayer->GetPlayerState());

			if (!CurrentPlayerState 
				// || !CurrentPlayerState->IsBot()
				)
				continue;

			if (GameState->GetGamePhase() == EAthenaGamePhase::Warmup)
			{
				switch (PlayerBot.WarmupBehavior)
				{
				case EAIWarmupBehavior::None:
					PlayerBot.ChooseWarmupBehavior();
					break;
				case EAIWarmupBehavior::PlayEmote:
					PlayerBot.StartEmoting();
					break;
				case EAIWarmupBehavior::LootAndShoot:
					PlayerBot.MoveInDirection(CurrentPawn->GetActorForwardVector());
					break;
				case EAIWarmupBehavior::Idle:
					break;
				case EAIWarmupBehavior::EAIWarmupBehavior_MAX:
					PlayerBot.ChooseWarmupBehavior();
					break;
				default:
					PlayerBot.ChooseWarmupBehavior();
					break;
				}
			}
			else if (GameState->GetGamePhase() == EAthenaGamePhase::Aircraft)
			{
				CurrentPlayerState->SetIsInAircraft(true);
			}

			if (PlayerBot::ShouldUseAIBotController() && CurrentPlayerState->IsInAircraft() && !CurrentPlayerState->HasThankedBusDriver() && UKismetMathLibrary::RandomBoolWithWeight(0.05f))
			{
				static auto ThankBusDriverFn = FindObject<UFunction>(L"/Script/FortniteGame.FortAthenaAIBotController.ThankBusDriver");
				CurrentPlayer->ProcessEvent(ThankBusDriverFn);
				CurrentPlayerState->SetHasThankedBusDriver(true);
			}

			if (CurrentPawn && !CurrentPlayerState->IsInAircraft())
			{
				if (PlayerBot.NextJumpTime <= UGameplayStatics::GetTimeSeconds(GetWorld()))
				{
					static auto JumpFn = FindObject<UFunction>(L"/Script/Engine.Character.Jump");

					CurrentPawn->ProcessEvent(JumpFn);
					PlayerBot.NextJumpTime = UGameplayStatics::GetTimeSeconds(GetWorld()) + (rand() % 4 + 3);
				}
			}

			bool bShouldJumpFromBus = (CurrentPlayerState->IsInAircraft() && CurrentPlayerState->HasThankedBusDriver() && UKismetMathLibrary::RandomBoolWithWeight(0.07f) && PlayerBot.bCanJumpFromBus && !PlayerBot.bHasJumpedFromBus) || PlayerBot.bForceJumpFromAircraft;

			if (bShouldJumpFromBus)
			{
				int AircraftIndex = GameState->GetAircraftIndex(PlayerBot.PlayerState);
				auto Aircraft = GameState->GetAircraft(AircraftIndex);

				CurrentPawn->TeleportTo(Aircraft->GetActorLocation(), FRotator());
				CurrentPawn->BeginSkydiving(true);
				CurrentPlayerState->SetIsInAircraft(false);
				PlayerBot.bHasJumpedFromBus = true;
				PlayerBot.bForceJumpFromAircraft = false;
			}

			if (!CurrentPlayerState->IsInAircraft() && GameState->GetGamePhase() > EAthenaGamePhase::Warmup)
			{
				if (!PlayerBot.CurrentTargetActor)
					PlayerBot.CurrentTargetActor = PlayerBot.FindNearestAvailableChest();

				PlayerBot.MoveToLocation(PlayerBot.CurrentTargetActor->GetActorLocation());
			}
		}
	}
}

namespace Bosses
{

}
