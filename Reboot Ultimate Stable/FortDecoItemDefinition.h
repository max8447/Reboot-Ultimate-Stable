#pragma once

#include "FortWeaponItemDefinition.h"
#include "SoftClassPtr.h"

class UFortDecoItemDefinition : public UFortWeaponItemDefinition
{
public:

	TSoftClassPtr<UClass> GetBlueprintClass()
	{
		static auto BlueprintClassOffset = GetOffset("BlueprintClass");
		return Get<TSoftClassPtr<UClass>>(BlueprintClassOffset);
	}

	static UClass* StaticClass();
};