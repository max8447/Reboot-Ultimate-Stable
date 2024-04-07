#pragma once

#include "FortDecoItemDefinition.h"

class UFortTrapItemDefinition : public UFortDecoItemDefinition
{
public:
	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>("/Script/FortniteGame.FortTrapItemDefinition");
		return Class;
	}
};

class UFortContextTrapItemDefinition : public UFortTrapItemDefinition
{
public:
	UFortTrapItemDefinition* GetFloorTrap()
	{
		static auto FloorTrapOffset = GetOffset("FloorTrap");
		return Get<UFortTrapItemDefinition*>(FloorTrapOffset);
	}

	UFortTrapItemDefinition* GetCeilingTrap()
	{
		static auto CeilingTrapOffset = GetOffset("CeilingTrap");
		return Get<UFortTrapItemDefinition*>(CeilingTrapOffset);
	}

	UFortTrapItemDefinition* GetWallTrap()
	{
		static auto WallTrapOffset = GetOffset("WallTrap");
		return Get<UFortTrapItemDefinition*>(WallTrapOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>("/Script/FortniteGame.FortContextTrapItemDefinition");
		return Class;
	}
};