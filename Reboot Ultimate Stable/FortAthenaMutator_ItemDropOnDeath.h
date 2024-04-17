#pragma once

#include "FortAthenaMutator.h"
#include "GameplayAbilityTypes.h"
#include "CurveTable.h"
#include "FortWorldItemDefinition.h"
#include "FortInventory.h"

struct FItemsToDropOnDeath
{
	static UStruct* GetStruct()
	{
		static auto Struct = FindObject<UStruct>(L"/Script/FortniteGame.ItemsToDropOnDeath");
		return Struct;
	}

	static int GetStructSize() { return GetStruct()->GetPropertiesSize(); }

	UFortWorldItemDefinition*& GetItemToDrop()
	{
		static auto ItemToDropOffset = FindOffsetStruct("/Script/FortniteGame.ItemsToDropOnDeath", "ItemToDrop");
		return *(UFortWorldItemDefinition**)(__int64(this) + ItemToDropOffset);
	}

	FScalableFloat& GetNumberToDrop()
	{
		static auto NumberToDropOffset = FindOffsetStruct("/Script/FortniteGame.ItemsToDropOnDeath", "NumberToDrop");
		return *(FScalableFloat*)(__int64(this) + NumberToDropOffset);
	}
};

class AFortAthenaMutator_ItemDropOnDeath : public AFortAthenaMutator
{
public:
	TArray<FItemsToDropOnDeath>& GetItemsToDrop()
	{
		static auto ItemsToDropOffset = GetOffset("ItemsToDrop");
		return Get<TArray<FItemsToDropOnDeath>>(ItemsToDropOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaMutator_ItemDropOnDeath");
		return Class;
	}
};