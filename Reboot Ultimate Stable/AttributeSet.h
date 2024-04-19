#pragma once

#include "reboot.h"

class UAttributeSet : public UObject
{
public:
};

struct FGameplayAttribute
{
	FString AttributeName; 
	void* Attribute; // Property
	UStruct* AttributeOwner;

	std::string GetAttributeName()
	{
		return AttributeName.ToString();
	}

	std::string GetAttributePropertyName()
	{
		if (!Attribute)
			return "INVALIDATTRIBUTE";

		return GetFNameOfProp(Attribute)->ToString();
	}
};

struct FGameplayAttributeData
{
	float& GetBaseValue()
	{
		static auto BaseValueOffset = FindOffsetStruct("/Script/GameplayAbilities.GameplayAttributeData", "BaseValue");
		return *(float*)(__int64(this) + BaseValueOffset);
	}

	float& GetCurrentValue()
	{
		static auto CurrentValueOffset = FindOffsetStruct("/Script/GameplayAbilities.GameplayAttributeData", "CurrentValue");
		return *(float*)(__int64(this) + CurrentValueOffset);
	}
};

struct FFortGameplayAttributeData : public FGameplayAttributeData
{
	float& GetMinimum()
	{
		static auto MinimumOffset = FindOffsetStruct("/Script/FortniteGame.FortGameplayAttributeData", "Minimum");
		return *(float*)(__int64(this) + MinimumOffset);
	}

	float& GetMaximum()
	{
		static auto MaximumOffset = FindOffsetStruct("/Script/FortniteGame.FortGameplayAttributeData", "Maximum");
		return *(float*)(__int64(this) + MaximumOffset);
	}
};