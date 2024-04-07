#pragma once

#include "Actor.h"
#include "reboot.h"
#include "GameplayTagContainer.h"

class AFortPoiVolume final : public AActor // AVolume
{
public:
	FGameplayTagContainer GetLocationTags()
	{
		static auto LocationTagsOffset = GetOffset("LocationTags");
		return Get<FGameplayTagContainer>(LocationTagsOffset);
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>("/Script/FortniteGame.FortPoiVolume");
		return Class;
	}
};