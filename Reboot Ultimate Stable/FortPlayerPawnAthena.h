#pragma once

#include "FortPlayerPawn.h"

class AFortPlayerPawnAthena : public AFortPlayerPawn
{
public:
	static inline void (*OnCapsuleBeginOverlapOriginal)(UObject* Context, FFrame* Stack, void* Ret);

	uint8& GetDBNORevivalStacking()
	{
		static auto DBNORevivalStackingOffset = GetOffset("DBNORevivalStacking");
		return Get<uint8>(DBNORevivalStackingOffset);
	}

	void TeleportToSkyDive(float HeightAboveGround)
	{
		static auto fn = FindObject<UFunction>(L"/Script/FortniteGame.FortPlayerPawnAthena.TeleportToSkyDive");
		this->ProcessEvent(fn, &HeightAboveGround);
	}

	static void OnCapsuleBeginOverlapHook(UObject* Context, FFrame* Stack, void* Ret);
};