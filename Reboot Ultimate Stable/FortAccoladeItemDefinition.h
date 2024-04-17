#pragma once

#include "FortItemDefinition.h"

enum class EXPEventPriorityType : uint8
{
	NearReticle = 0,
	XPBarOnly = 1,
	TopCenter = 2,
	Feed = 3,
	EXPEventPriorityType_MAX = 4,
};

class UFortAccoladeItemDefinition : public UFortItemDefinition // UFortPersistableItemDefinition
{
public:
	EXPEventPriorityType& GetPriority()
	{
		static auto PriorityOffset = GetOffset("Priority");
		return Get<EXPEventPriorityType>(PriorityOffset);
	}

	int GetAccoladeXpValue()
	{
		static auto GetAccoladeXpValueFn = FindObject<UFunction>(L"/Script/FortniteGame.FortAccoladeItemDefinition.GetAccoladeXpValue");
		int ReturnValue;
		this->ProcessEvent(GetAccoladeXpValueFn, &ReturnValue);
		return ReturnValue;
	}
};