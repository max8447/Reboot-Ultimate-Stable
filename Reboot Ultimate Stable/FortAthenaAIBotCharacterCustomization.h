#pragma once

#include "Object.h"

class UFortAthenaAIBotCharacterCustomization : public UObject
{
public:
	struct FFortAthenaLoadout* GetCustomizationLoadout()
	{
		static auto CustomizationLoadoutOffset = GetOffset("CustomizationLoadout");
		return GetPtr<struct FFortAthenaLoadout>(CustomizationLoadoutOffset);
	}
};