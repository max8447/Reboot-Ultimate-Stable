#pragma once

#include "Object.h"
#include "FortPlayerPawnAthena.h"
#include "FortAthenaAIBotCustomizationData.h"
#include "FortAthenaMutator_Bots.h"

struct FFortAthenaAIBotRunTimeCustomizationData
{
	FGameplayTag& GetPredefinedCosmeticSetTag()
	{
		static auto PredefinedCosmeticSetTagOffset = FindOffsetStruct("/Script/FortniteGame.FortAthenaAIBotRunTimeCustomizationData", "PredefinedCosmeticSetTag");
		return *(FGameplayTag*)(__int64(this) + PredefinedCosmeticSetTagOffset);
	}

	float& GetCullDistanceSquared()
	{
		static auto CullDistanceSquaredOffset = FindOffsetStruct("/Script/FortniteGame.FortAthenaAIBotRunTimeCustomizationData", "CullDistanceSquared");
		return *(float*)(__int64(this) + CullDistanceSquaredOffset);
	}

	bool& ShouldCheckForOverlaps()
	{
		static auto bCheckForOverlapsOffset = FindOffsetStruct("/Script/FortniteGame.FortAthenaAIBotRunTimeCustomizationData", "bCheckForOverlaps");
		return *(bool*)(__int64(this) + bCheckForOverlapsOffset);
	}

	uint8& GetCustomSquadId()
	{
		static auto CustomSquadIdOffset = FindOffsetStruct("/Script/FortniteGame.FortAthenaAIBotRunTimeCustomizationData", "CustomSquadId");
		return *(uint8*)(__int64(this) + CustomSquadIdOffset);
	}

	bool& HasCustomSquadId()
	{
		static auto bHasCustomSquadIdOffset = FindOffsetStruct("/Script/FortniteGame.FortAthenaAIBotRunTimeCustomizationData", "bHasCustomSquadId");
		return *(bool*)(__int64(this) + bHasCustomSquadIdOffset);
	}
};

class UFortServerBotManagerAthena : public UObject
{
public:
	static inline AFortPlayerPawnAthena* (*SpawnBotOriginal)(UFortServerBotManagerAthena* BotManager, FVector InSpawnLocation, FRotator InSpawnRotation, UFortAthenaAIBotCustomizationData* InBotData, FFortAthenaAIBotRunTimeCustomizationData InRuntimeBotData);

	AFortAthenaMutator_Bots*& GetCachedBotMutator()
	{
		static auto CachedBotMutatorOffset = GetOffset("CachedBotMutator");
		return Get<AFortAthenaMutator_Bots*>(CachedBotMutatorOffset);
	}

	static void SetupBot(UFortServerBotManagerAthena* BotManager, AFortPlayerPawnAthena* Pawn, UFortAthenaAIBotCustomizationData* InBotData, FFortAthenaAIBotRunTimeCustomizationData RuntimeBotData);
	static AFortPlayerPawnAthena* SpawnBotHook(UFortServerBotManagerAthena* BotManager, FVector& InSpawnLocation, FRotator& InSpawnRotation, UFortAthenaAIBotCustomizationData* InBotData, FFortAthenaAIBotRunTimeCustomizationData& InRuntimeBotData);
};