#pragma once

#include "FortWeapon.h"
#include "Rotator.h"
#include "FortItemDefinition.h"
#include "NetSerialization.h"

enum class EBuildingAttachmentType : uint8
{
	ATTACH_Floor = 0,
	ATTACH_Wall = 1,
	ATTACH_Ceiling = 2,
	ATTACH_Corner = 3,
	ATTACH_All = 4,
	ATTACH_WallThenFloor = 5,
	ATTACH_FloorAndStairs = 6,
	ATTACH_CeilingAndStairs = 7,
	ATTACH_None = 8,
	ATTACH_MAX = 9,
};

class AFortDecoTool : public AFortWeapon
{
public:
	UFortItemDefinition* GetItemDefinition()
	{
		static auto ItemDefinitionOffset = GetOffset("ItemDefinition");
		return Get<UFortItemDefinition*>(ItemDefinitionOffset);
	}

	static void ServerCreateBuildingAndSpawnDecoHook(AFortDecoTool* DecoTool, FVector_NetQuantize10 BuildingLocation, FRotator BuildingRotation, FVector_NetQuantize10 Location, FRotator Rotation, EBuildingAttachmentType InBuildingAttachmentType);
};