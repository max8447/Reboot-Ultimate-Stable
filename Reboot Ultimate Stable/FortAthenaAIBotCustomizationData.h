#pragma once

#include "reboot.h"
#include "FortAthenaAIBotCharacterCustomization.h"
#include "FortPlayerState.h"
#include "BehaviorTree.h"
#include "FortAthenaAIBotInventoryItems.h"
#include "FortBotNameSettings.h"
#include "SubclassOf.h"
#include "FortPlayerPawnAthena.h"

class UFortAthenaAIBotCustomizationData : public UObject // UPrimaryDataAsset
{
public:
	UFortAthenaAIBotCharacterCustomization*& GetCharacterCustomization()
	{
		static auto CharacterCustomizationOffset = GetOffset("CharacterCustomization");
		return Get<UFortAthenaAIBotCharacterCustomization*>(CharacterCustomizationOffset);
	}

	TSubclassOf<AFortPlayerPawnAthena> GetPawnClass()
	{
		static auto PawnClassOffset = GetOffset("PawnClass");
		return Get<TSubclassOf<AFortPlayerPawnAthena>>(PawnClassOffset);
	}

	UBehaviorTree* GetBehaviorTree()
	{
		static auto BehaviorTreeOffset = GetOffset("BehaviorTree");
		return Get<UBehaviorTree*>(BehaviorTreeOffset);
	}

	UFortAthenaAIBotInventoryItems* GetStartupInventory()
	{
		static auto StartupInventoryOffset = GetOffset("StartupInventory");
		return Get<UFortAthenaAIBotInventoryItems*>(StartupInventoryOffset);
	}

	UFortBotNameSettings* GetBotNameSettings()
	{
		static auto BotNameSettingsOffset = GetOffset("BotNameSettings");
		return Get<UFortBotNameSettings*>(BotNameSettingsOffset);
	}

	float GetSkillLevel()
	{
		static auto SkillLevelOffset = GetOffset("SkillLevel");
		return Get<float>(SkillLevelOffset);
	}

	static void ApplyOverrideCharacterCustomizationHook(UFortAthenaAIBotCustomizationData* InBotData, AFortPlayerPawn* NewBot, __int64 idk);

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAIBotCustomizationData");
		return Class;
	}
};