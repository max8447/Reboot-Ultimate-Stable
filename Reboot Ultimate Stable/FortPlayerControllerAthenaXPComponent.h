#pragma once

#include "ActorComponent.h"
#include "FortAccoladeItemDefinition.h"

struct FAthenaAccolades
{
public:
	UFortAccoladeItemDefinition* AccoladeDef;
	FString TemplateId;
	int Count;
};

struct FXPEventInfo
{
public:
	FName EventName;
	FText SimulatedText;
	UFortItemDefinition* QuestDef;
	EXPEventPriorityType Priority;
	int32 EventXpValue;
	int32 TotalXpEarnedInMatch;
	FPrimaryAssetId Accolade;
	int32 RestedValuePortion;
	int32 SeasonBoostValuePortion;
	int32 RestedXPRemaining;
};

class UFortPlayerControllerAthenaXPComponent : public UActorComponent // UFortControllerComponent
{
public:
	int& GetTotalXpEarned()
	{
		static auto TotalXpEarnedOffset = GetOffset("TotalXpEarned");
		return Get<int>(TotalXpEarnedOffset);
	}

	int& GetMedalBonusXP()
	{
		static auto MedalBonusXPOffset = GetOffset("MedalBonusXP");
		return Get<int>(MedalBonusXPOffset);
	}

	int& GetMatchXp()
	{
		static auto MatchXpOffset = GetOffset("MatchXp");
		return Get<int>(MatchXpOffset);
	}

	TArray<FAthenaAccolades>& GetPlayerAccolades()
	{
		static auto PlayerAccoladesOffset = GetOffset("PlayerAccolades");
		return Get<TArray<FAthenaAccolades>>(PlayerAccoladesOffset);
	}

	TArray<UFortAccoladeItemDefinition*> GetMedalsEarned()
	{
		static auto MedalsEarnedOffset = GetOffset("MedalsEarned");
		return Get<TArray<UFortAccoladeItemDefinition*>>(MedalsEarnedOffset);
	}

	void ClientMedalsRecived(TArray<FAthenaAccolades>& Medals)
	{
		static auto ClientMedalsRecivedFn = FindObject<UFunction>(L"/Script/FortniteGame.FortPlayerControllerAthenaXPComponent.ClientMedalsRecived");
		this->ProcessEvent(ClientMedalsRecivedFn, &Medals);
	}

	void OnXPEvent(FXPEventInfo& XPEvent)
	{
		static auto OnXPEventFn = FindObject<UFunction>(L"/Script/FortniteGame.FortPlayerControllerAthenaXPComponent.OnXPEvent");
		this->ProcessEvent(OnXPEventFn, &XPEvent);
	}
};