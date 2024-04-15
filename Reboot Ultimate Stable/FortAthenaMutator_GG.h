// Gun Game

#pragma once

#include "Actor.h"
#include "CurveTable.h"
#include "GameplayAbilityTypes.h"
#include "FortWeaponItemDefinition.h"
#include "Stack.h"
#include "FortPlayerStateAthena.h"
#include "FortAthenaMutator.h"

struct FGunGameGunEntry
{
	static UStruct* GetStruct()
	{
		static auto Struct = FindObject<UStruct>(L"/Script/FortniteGame.GunGameGunEntry");
		return Struct;
	}

	static int GetStructSize() { return GetStruct()->GetPropertiesSize(); }

	UFortWeaponItemDefinition*& GetWeapon()
	{
		static auto WeaponOffset = FindOffsetStruct("/Script/FortniteGame.GunGameGunEntry", "Weapon");
		return *(UFortWeaponItemDefinition**)(__int64(this) + WeaponOffset);
	}

	FScalableFloat& GetEnabled()
	{
		static auto EnabledOffset = FindOffsetStruct("/Script/FortniteGame.GunGameGunEntry", "Enabled");
		return *(FScalableFloat*)(__int64(this) + EnabledOffset);
	}

	FScalableFloat& GetAwardAtElim()
	{
		static auto AwardAtElimOffset = FindOffsetStruct("/Script/FortniteGame.GunGameGunEntry", "AwardAtElim");
		return *(FScalableFloat*)(__int64(this) + AwardAtElimOffset);
	}
};

struct FGunGameGunEntries
{
	TArray<FGunGameGunEntry>                    Entries;                                                  // 0x0000(0x0010) (ZeroConstructor, NativeAccessSpecifierPublic)
};

struct FGunGamePlayerData
{
	TArray<UFortWeaponItemDefinition*>           CurrentlyAssignedWeapons;                                 // 0x0000(0x0010) (ZeroConstructor, NativeAccessSpecifierPublic)
};

class AFortAthenaMutator_GG : public AFortAthenaMutator
{
public:
	TArray<FGunGameGunEntry>& GetWeaponEntries()
	{
		static auto WeaponEntriesOffset = GetOffset("WeaponEntries");
		return Get<TArray<FGunGameGunEntry>>(WeaponEntriesOffset);
	}

	TMap<int, FGunGameGunEntries>& GetAwardEntriesAtElimMap()
	{
		static auto AwardEntriesAtElimMapOffset = GetOffset("AwardEntriesAtElimMap");
		return Get<TMap<int, FGunGameGunEntries>>(AwardEntriesAtElimMapOffset);
	}

	TMap<AFortPlayerStateAthena*, FGunGamePlayerData>& GetPlayerData()
	{
		static auto PlayerDataOffset = GetOffset("PlayerData");
		return Get<TMap<AFortPlayerStateAthena*, FGunGamePlayerData>>(PlayerDataOffset);
	}

	UFortWeaponItemDefinition* GetWeaponFromElimScore(int KillScore)
	{
		for (int i = 0; i < GetWeaponEntries().Num(); ++i)
		{
			auto& WeaponEntry = GetWeaponEntries().at(i);
			auto CurrentWeapon = WeaponEntry.GetWeapon();
			auto& AwardAtElim = WeaponEntry.GetAwardAtElim();

			float KillToGiveAt = 0;

			if (!IsBadReadPtr(AwardAtElim.GetCurve().CurveTable, 8) && AwardAtElim.GetCurve().RowName.IsValid())
			{
				LOG_INFO(LogDev, "CurveTable: {}", AwardAtElim.GetCurve().CurveTable->GetFullName());
				LOG_INFO(LogDev, "RowName: {}", AwardAtElim.GetCurve().RowName.ToString());
				KillToGiveAt = UDataTableFunctionLibrary::EvaluateCurveTableRow(AwardAtElim.GetCurve().CurveTable, AwardAtElim.GetCurve().RowName, 0.f);
			}

			if (KillScore == KillToGiveAt)
				return CurrentWeapon;

			return nullptr;
		}
	}

	void SwapOutWeapons(AFortPlayerController* PlayerController, int KillScore)
	{
		auto NewWeapon = GetWeaponFromElimScore(KillScore);
		auto OldWeapon = GetWeaponFromElimScore(KillScore - 1);
		auto WorldInventory = PlayerController->GetWorldInventory();

		if (NewWeapon && WorldInventory)
		{
			LOG_INFO(LogDev, "NewWeapon: {}", NewWeapon->GetFullName());

			if (OldWeapon && WorldInventory->FindItemInstance(OldWeapon))
				WorldInventory->RemoveItem(WorldInventory->FindItemInstance(OldWeapon)->GetItemEntry()->GetItemGuid(), nullptr, OldWeapon->GetMaxStackSize());

			WorldInventory->AddItem(NewWeapon, nullptr, NewWeapon->GetMaxStackSize());
			WorldInventory->Update();
		}
		else
		{
			LOG_WARN(LogDev, "Couldn't find new weapon!");
		}
	}

	FGunGameGunEntries GetEntriesFromAward(const FScalableFloat& AwardAtElim)
	{
		auto& AwardEntriesAtElimMap = GetAwardEntriesAtElimMap();

		float Value = 0; // TODO Get from AwardAtElim

		for (auto& AwardEntry : AwardEntriesAtElimMap)
		{
			if (AwardEntry.First == Value)
			{
				return AwardEntry.Second;
			}
		}
	}

	static UClass* StaticClass()
	{
		static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortAthenaMutator_GG");
		return Class;
	}
};