#pragma once

#include "Object.h"
#include "FortPlayerController.h"

class UFortAthenaAIBotCharacterCustomization : public UObject
{
public:
	FFortAthenaLoadout* GetCustomizationLoadout()
	{
		static auto CustomizationLoadoutOffset = GetOffset("CustomizationLoadout");
		return GetPtr<FFortAthenaLoadout>(CustomizationLoadoutOffset);
	}
};