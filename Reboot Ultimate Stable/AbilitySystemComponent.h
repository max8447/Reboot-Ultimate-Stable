#pragma once

#include "Object.h"
#include "GameplayAbilitySpec.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"

// using FPredictionKey = PadHex18;
// using FGameplayEventData = PadHexA8;

// using FPredictionKey = PadHex10; 
using FGameplayEventData = PadHexB0;

// using FGameplayEventData = __int64;

struct FGameplayEffectContextHandle
{
	unsigned char                                      UnknownData00[0x18];                                      // 0x0000(0x0018) MISSED OFFSET
};

struct FGameplayAbilityTargetDataHandle
{
	uint8                                         Pad_3965[0x28];                                    // 0x0000(0x0028)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

struct FGameplayEventDataTest
{
	struct FGameplayTag                           EventTag;                                          // 0x0000(0x0008)(Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class AActor* Instigator;                                        // 0x0008(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class AActor* Target;                                            // 0x0010(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class UObject* OptionalObject;                                    // 0x0018(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class UObject* OptionalObject2;                                   // 0x0020(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FGameplayEffectContextHandle           ContextHandle;                                     // 0x0028(0x0018)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
	struct FGameplayTagContainer                  InstigatorTags;                                    // 0x0040(0x0020)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
	struct FGameplayTagContainer                  TargetTags;                                        // 0x0060(0x0020)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
	float                                         EventMagnitude;                                    // 0x0080(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_3966[0x4];                                     // 0x0084(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	struct FGameplayAbilityTargetDataHandle       TargetData;                                        // 0x0088(0x0028)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
};

struct FActiveGameplayEffectHandle
{
	int                                                Handle;                                                   // 0x0000(0x0004) (ZeroConstructor, IsPlainOldData)
	bool                                               bPassedFiltersAndWasExecuted;                             // 0x0004(0x0001) (ZeroConstructor, IsPlainOldData)
	unsigned char                                      UnknownData00[0x3];                                       // 0x0005(0x0003) MISSED OFFSET
};

struct FGameplayAbilitySpecContainer : public FFastArraySerializer
{
	TArray<FGameplayAbilitySpec>& GetItems()
	{
		static auto ItemsOffset = FindOffsetStruct("/Script/GameplayAbilities.GameplayAbilitySpecContainer", "Items");
		return *(TArray<FGameplayAbilitySpec>*)(__int64(this) + ItemsOffset);
	}
};

class UAbilitySystemComponent : public UObject
{
public:
	static inline FGameplayAbilitySpecHandle* (*GiveAbilityOriginal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, __int64 inSpec);
	static inline bool (*InternalTryActivateAbilityOriginal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle Handle, PadHex10 InPredictionKey, UObject** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData);
	static inline bool (*InternalTryActivateAbilityOriginal2)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle Handle, PadHex18 InPredictionKey, UObject** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData);

	void ClientActivateAbilityFailed(FGameplayAbilitySpecHandle AbilityToActivate, int16_t PredictionKey)
	{
		struct { FGameplayAbilitySpecHandle AbilityToActivate; int16_t PredictionKey; } UAbilitySystemComponent_ClientActivateAbilityFailed_Params{ AbilityToActivate, PredictionKey };
		static auto fn = FindObject<UFunction>(L"/Script/GameplayAbilities.AbilitySystemComponent.ClientActivateAbilityFailed");

		this->ProcessEvent(fn, &UAbilitySystemComponent_ClientActivateAbilityFailed_Params);
	}

	void ClientActivateAbilitySucceed(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey PredictionKey)
	{
		struct { FGameplayAbilitySpecHandle AbilityToActivate; FPredictionKey PredictionKey; } UAbilitySystemComponent_ClientActivateAbilitySucceed{ AbilityToActivate, PredictionKey };
		static auto fn = FindObject<UFunction>(L"/Script/GameplayAbilities.AbilitySystemComponent.ClientActivateAbilitySucceed");

		this->ProcessEvent(fn, &UAbilitySystemComponent_ClientActivateAbilitySucceed);
	}

	void ServerTryActivateAbilityWithEventData(FGameplayAbilitySpecHandle AbilityToActivate, bool InputPressed, FPredictionKey PredictionKey, FGameplayEventDataTest TriggerEventData)
	{
		struct { FGameplayAbilitySpecHandle AbilityToActivate; bool InputPressed; FPredictionKey PredictionKey; FGameplayEventDataTest TriggerEventData; } AbilitySystemComponent_ServerTryActivateAbilityWithEventData{ AbilityToActivate , InputPressed, PredictionKey , TriggerEventData };
		static auto fn = FindObject<UFunction>(L"/Script/GameplayAbilities.AbilitySystemComponent.ServerTryActivateAbilityWithEventData");
		
		this->ProcessEvent(fn, &AbilitySystemComponent_ServerTryActivateAbilityWithEventData);
	}

	void ServerTryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, bool InputPressed, FPredictionKey PredictionKey)
	{
		struct { FGameplayAbilitySpecHandle AbilityToActivate; bool InputPressed; FPredictionKey PredictionKey; } AbilitySystemComponent_ServerTryActivateAbility{ AbilityToActivate , InputPressed, PredictionKey };
		static auto fn = FindObject<UFunction>(L"/Script/GameplayAbilities.AbilitySystemComponent.ServerTryActivateAbility");

		this->ProcessEvent(fn, &AbilitySystemComponent_ServerTryActivateAbility);
	}

	TArray<UAttributeSet*>& GetSpawnedAttributes()
	{
		static auto SpawnedAttributesOffset = GetOffset("SpawnedAttributes");
		return Get<TArray<UAttributeSet*>>(SpawnedAttributesOffset);
	}

	FGameplayAbilitySpecContainer* GetActivatableAbilities()
	{
		static auto ActivatableAbilitiesOffset = this->GetOffset("ActivatableAbilities");
		return GetPtr<FGameplayAbilitySpecContainer>(ActivatableAbilitiesOffset);
	}

	UAttributeSet* AddDefaultSubobjectSet(UAttributeSet* Subobject)
	{
		GetSpawnedAttributes().Add(Subobject);
		return Subobject;
	}

	void ServerEndAbility(FGameplayAbilitySpecHandle AbilityToEnd, FGameplayAbilityActivationInfo* ActivationInfo, FPredictionKey* PredictionKey);
	void ClientEndAbility(FGameplayAbilitySpecHandle AbilityToEnd, FGameplayAbilityActivationInfo* ActivationInfo);
	void ClientCancelAbility(FGameplayAbilitySpecHandle AbilityToCancel, FGameplayAbilityActivationInfo* ActivationInfo);
	bool HasAbility(UObject* DefaultAbility);
	FActiveGameplayEffectHandle ApplyGameplayEffectToSelf(UClass* GameplayEffectClass, float Level, const FGameplayEffectContextHandle& EffectContext = FGameplayEffectContextHandle());
	// FGameplayEffectContextHandle MakeEffectContext();
	void RemoveActiveGameplayEffectBySourceEffect(UClass* GEClass, int StacksToRemove, UAbilitySystemComponent* Instigator);
	void ConsumeAllReplicatedData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey* AbilityOriginalPredictionKey);
	FGameplayAbilitySpecHandle GiveAbilityEasy(UClass* AbilityClass, UObject* SourceObject = nullptr, bool bDoNotRegive = true);
	FGameplayAbilitySpec* FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle);
	void RemoveActiveGameplayEffectBySourceEffect(UClass* GameplayEffect, UAbilitySystemComponent* InstigatorAbilitySystemComponent, int StacksToRemove);
	void ClearAbility(const FGameplayAbilitySpecHandle& Handle);

	static void InternalServerTryActivateAbilityHook(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey* PredictionKey, const FGameplayEventData* TriggerEventData);
};

void LoopSpecs(UAbilitySystemComponent* AbilitySystemComponent, std::function<void(FGameplayAbilitySpec*)> func);