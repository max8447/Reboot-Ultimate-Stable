#include "FortPlayerStateAthena.h"
#include "Stack.h"
#include "FortPlayerControllerAthena.h"
#include "FortPawn.h"

uint8 AFortPlayerStateAthena::ToDeathCause(const FGameplayTagContainer& TagContainer, bool bWasDBNO, AFortPawn* Pawn)
{
	static auto ToDeathCauseFn = FindObject<UFunction>(L"/Script/FortniteGame.FortPlayerStateAthena.ToDeathCause");

	if (ToDeathCauseFn)
	{
		struct
		{
			FGameplayTagContainer                       InTags;                                                   // (ConstParm, Parm, OutParm, ReferenceParm, NativeAccessSpecifierPublic)
			bool                                               bWasDBNO;                                                 // (Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
			uint8_t                                        ReturnValue;                                              // (Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		} AFortPlayerStateAthena_ToDeathCause_Params{ TagContainer, bWasDBNO };

		AFortPlayerStateAthena::StaticClass()->ProcessEvent(ToDeathCauseFn, &AFortPlayerStateAthena_ToDeathCause_Params);

		return AFortPlayerStateAthena_ToDeathCause_Params.ReturnValue;
	}

	static bool bHaveFoundAddress = false;

	static uint64 Addr = 0;

	if (!bHaveFoundAddress)
	{
		bHaveFoundAddress = true;

		if (Engine_Version == 419)
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 41 0F B6 F8 48 8B DA 48 8B F1 E8 ? ? ? ? 33 ED").Get();
		if (Engine_Version == 420)
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 0F B6 FA 48 8B D9 E8 ? ? ? ? 33 F6 48 89 74 24").Get();
		if (Engine_Version == 421) // 5.1
			Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 0F B6 FA 48 8B D9 E8 ? ? ? ? 33").Get();

		if (!Addr)
		{
			LOG_WARN(LogPlayer, "Failed to find ToDeathCause address!");
			return 0;
		}
	}

	if (!Addr)
	{
		return 0;
	}

	if (Engine_Version == 419)
	{
		static uint8(*sub_7FF7AB499410)(AFortPawn * Pawn, FGameplayTagContainer TagContainer, char bWasDBNOIg) = decltype(sub_7FF7AB499410)(Addr);
		return sub_7FF7AB499410(Pawn, TagContainer, bWasDBNO);
	}

	static uint8(*sub_7FF7AB499410)(FGameplayTagContainer TagContainer, char bWasDBNOIg) = decltype(sub_7FF7AB499410)(Addr);
	return sub_7FF7AB499410(TagContainer, bWasDBNO);
}

void AFortPlayerStateAthena::ServerSetInAircraftHook(UObject* Context, FFrame& Stack, void* Ret)
{
	/* LOG_INFO(LogDev, "bLateGame: {}", Globals::bLateGame)

	if (Globals::bLateGame)
		return ServerSetInAircraftOriginal(Context, Stack, Ret); */

	auto PlayerState = (AFortPlayerStateAthena*)Context;
	auto PlayerController = Cast<AFortPlayerControllerAthena>(PlayerState->GetOwner());

	if (!PlayerController)
		return ServerSetInAircraftOriginal(Context, Stack, Ret);

	// std::cout << "PlayerController->IsInAircraft(): " << PlayerController->IsInAircraft() << '\n';

	struct aaa { bool wtf; };

	auto bNewInAircraft = ((aaa*)Stack.Locals)->wtf;// *(bool*)Stack.Locals;
	LOG_INFO(LogDev, "bNewInAircraft: {}", bNewInAircraft);
	auto WorldInventory = PlayerController->GetWorldInventory();
	auto& InventoryList = WorldInventory->GetItemList();

	auto& ItemInstances = InventoryList.GetItemInstances();

	bool bOverrideDontClearInventory = false;

	if (/* (bNewInAircraft && !PlayerController->IsInAircraft()) || */ /* (Globals::bLateGame ? bNewInAircraft : true)) && */
		!Globals::bLateGame.load() && ItemInstances.Num() && !bOverrideDontClearInventory)
	{
		static auto CurrentShieldOffset = PlayerState->GetOffset("CurrentShield");

		if (CurrentShieldOffset != -1)
			PlayerState->Get<float>(CurrentShieldOffset) = 0; // real
	}

	return ServerSetInAircraftOriginal(Context, Stack, Ret);
}