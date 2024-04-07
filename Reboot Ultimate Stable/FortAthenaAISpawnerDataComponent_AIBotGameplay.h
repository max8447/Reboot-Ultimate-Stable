#pragma once

#include "reboot.h"
#include "FortBotNameSettings.h"

class UFortAthenaAISpawnerDataComponent_AIBotGameplay : public UObject
{
public:
	UFortBotNameSettings* GetNameSettings()
	{
		static auto NameSettingsOffset = GetOffset("NameSettings");
		return Get<UFortBotNameSettings*>(NameSettingsOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>("/Script/FortniteGame.FortAthenaAISpawnerDataComponent_AIBotGameplay");
		return Class;
	}
};