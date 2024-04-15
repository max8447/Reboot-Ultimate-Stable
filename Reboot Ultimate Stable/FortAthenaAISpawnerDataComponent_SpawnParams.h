#pragma once

#include "reboot.h"
#include "SubclassOf.h"
#include "Pawn.h"

class UFortAthenaAISpawnerDataComponent_SpawnParams : public UObject
{
public:
	TSubclassOf<APawn>& GetPawnClass()
	{
		static auto PawnClassOffset = GetOffset("PawnClass");
		return Get<TSubclassOf<APawn>>(PawnClassOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaAISpawnerDataComponent_SpawnParams");
		return Class;
	}
};