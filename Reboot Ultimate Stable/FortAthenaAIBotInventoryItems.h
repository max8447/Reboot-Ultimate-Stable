#pragma once

#include "FortItemDefinition.h"

class UFortAthenaAIBotInventoryItems : public UObject
{
public:
    TArray<FItemAndCount> GetItemsS13()
    {
        static auto ItemsOffset = GetOffset("Items");
        return Get<TArray<FItemAndCount>>(ItemsOffset);
    }

    TArray<UFortItemDefinition*> GetItemsS12()
    {
        static auto ItemsOffset = GetOffset("Items");
        return Get<TArray<UFortItemDefinition*>>(ItemsOffset);
    }
};