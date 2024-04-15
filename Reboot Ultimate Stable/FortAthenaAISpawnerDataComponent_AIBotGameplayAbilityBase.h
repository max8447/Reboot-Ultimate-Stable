#pragma once

#include "reboot.h"
#include "FortAbilitySet.h"

struct FInitialGameplayEffectInfo
{
public:
	static UStruct* GetStruct()
	{
		static auto Struct = FindObject<UStruct>(L"/Script/FortniteGame.InitialGameplayEffectInfo");
		return Struct;
	}

	static int GetStructSize() { return GetStruct()->GetPropertiesSize(); }

	TSubclassOf<UObject> GetGameplayEffect()
	{
		static auto GameplayEffectOffset = FindOffsetStruct("/Script/FortniteGame.InitialGameplayEffectInfo", "GameplayEffect");
		return *(TSubclassOf<UObject>*)(__int64(this) + GameplayEffectOffset);
	}

	float GetLevel()
	{
		static auto LevelOffset = FindOffsetStruct("/Script/FortniteGame.InitialGameplayEffectInfo", "Level");
		return *(float*)(__int64(this) + LevelOffset);
	}
};

class UFortAthenaAISpawnerDataComponent_AIBotGameplayAbilityBase : public UObject
{
public:
	TArray<FInitialGameplayEffectInfo> GetInitialGameplayEffect()
	{
		static auto InitialGameplayEffectOffset = GetOffset("InitialGameplayEffect");
		return Get<TArray<FInitialGameplayEffectInfo>>(InitialGameplayEffectOffset);
	}

	TArray<UFortAbilitySet*> GetInitialGameplayAbilitiesSet()
	{
		static auto InitialGameplayAbilitiesSetOffset = GetOffset("InitialGameplayAbilitiesSet");
		return Get<TArray<UFortAbilitySet*>>(InitialGameplayAbilitiesSetOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAISpawnerDataComponent_AIBotGameplayAbilityBase");
		return Class;
	}
};