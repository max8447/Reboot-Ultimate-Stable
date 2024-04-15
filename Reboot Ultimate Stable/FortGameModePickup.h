#pragma once

#include "FortPickup.h"

class AFortGameModePickup : public AFortPickup//Athena
{
public:
	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortGameModePickup");
		return Class;
	}
};