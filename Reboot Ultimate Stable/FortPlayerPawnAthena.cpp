#include "FortPlayerPawnAthena.h"
#include "FortInventory.h"
#include "FortPlayerControllerAthena.h"
#include "FortPoiVolume.h"
#include "GameplayStatics.h"
#include "BuildingFoundation.h"
#include "KismetTextLibrary.h"

void AFortPlayerPawnAthena::OnCapsuleBeginOverlapHook(UObject* Context, FFrame* Stack, void* Ret)
{
	using UPrimitiveComponent = UObject;

	auto Pawn = (AFortPlayerPawnAthena*)Context;
	UPrimitiveComponent* OverlappedComp;
	AActor* OtherActor = nullptr;
	UPrimitiveComponent* OtherComp;
	int OtherBodyIndex;
	bool bFromSweep;
	auto SweepResultPtr = (FHitResult*)std::realloc(0, FHitResult::GetStructSize());

	LOG_INFO(LogDev, "OnCapsuleBeginOverlapHook!");

	Stack->StepCompiledIn(&OverlappedComp);
	Stack->StepCompiledIn(&OtherActor);
	Stack->StepCompiledIn(&OtherComp);
	Stack->StepCompiledIn(&OtherBodyIndex);
	Stack->StepCompiledIn(&bFromSweep);
	Stack->StepCompiledIn(SweepResultPtr);

	std::free(SweepResultPtr);

	// LOG_INFO(LogDev, "OtherActor: {}", __int64(OtherActor));
	// LOG_INFO(LogDev, "OtherComp: {}", OtherComp ? OtherComp->GetFullName() : "BadRead");
	
	if (!Pawn->IsDBNO())
	{
		if (auto Pickup = Cast<AFortPickup>(OtherActor))
		{
			static auto PawnWhoDroppedPickupOffset = Pickup->GetOffset("PawnWhoDroppedPickup");

			if (Pickup->Get<AFortPawn*>(PawnWhoDroppedPickupOffset) != Pawn)
			{
				auto ItemDefinition = Pickup->GetPrimaryPickupItemEntry()->GetItemDefinition();

				if (!ItemDefinition)
				{
					return;
				}

				if (!IsPrimaryQuickbar(ItemDefinition))
				{
					ServerHandlePickupHook(Pawn, Pickup, 0.4f, FVector(), true);
				}
			}
		}
	}

	if (auto PoiVolume = Cast<AFortPoiVolume>(OtherActor->GetOuter()))
	{
		auto LocationTag = PoiVolume->GetLocationTags().GameplayTags.at(0); // i think
		auto AllBuildingFoundations = UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildingFoundation::StaticClass());

		for (int i = 0; i < AllBuildingFoundations.Num(); i++)
		{
			auto CurrentBuildingFoundation = Cast<ABuildingFoundation>(AllBuildingFoundations.at(i));

			if (LocationTag.TagName == CurrentBuildingFoundation->GetMapLocationTag().TagName)
			{
				std::string MapLocationTextStr = UKismetTextLibrary::Conv_TextToString(CurrentBuildingFoundation->GetMapLocationText()).ToString();

				LOG_INFO(LogGame, "Entered {}", MapLocationTextStr);
			}
		}
	}

	// return OnCapsuleBeginOverlapOriginal(Context, Stack, Ret); // we love explicit
}