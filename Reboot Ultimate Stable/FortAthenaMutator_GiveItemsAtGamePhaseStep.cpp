#include "FortAthenaMutator_GiveItemsAtGamePhaseStep.h"

#include "FortGameStateAthena.h"
#include "FortPlayerController.h"

void AFortAthenaMutator_GiveItemsAtGamePhaseStep::OnGamePhaseStepChangedHook(UObject* Context, FFrame& Stack, void* Ret)
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GetGameState());

	if (!GameState)
		return OnGamePhaseStepChangedOriginal(Context, Stack, Ret);

	LOG_INFO(LogDev, __FUNCTION__);

	auto Mutator = (AFortAthenaMutator_GiveItemsAtGamePhaseStep*)Context;

	TScriptInterface<UObject> SafeZoneInterface;
	EAthenaGamePhaseStep GamePhaseStep;

	static auto SafeZoneInterfaceOffset = FindOffsetStruct("/Script/FortniteGame.FortAthenaMutator_GiveItemsAtGamePhaseStep.OnGamePhaseStepChanged", "SafeZoneInterface", false);

	if (SafeZoneInterfaceOffset != -1)
		Stack.StepCompiledIn(&SafeZoneInterface);

	Stack.StepCompiledIn(&GamePhaseStep);

	if (Mutator->GetPhaseToGiveItems() == GamePhaseStep)
	{
		auto PhaseToGive = Mutator->GetPhaseToGiveItems();
		auto& ItemsToGive = Mutator->GetItemsToGive();

		LOG_INFO(LogDev, "PhaseToGiveItems: {} ItemsToGive.Num(): {}", (int)PhaseToGive, ItemsToGive.Num());

		for (int j = 0; j < ItemsToGive.Num(); j++)
		{
			auto ItemToGive = ItemsToGive.AtPtr(j, FItemsToGive::GetStructSize());

			if (!ItemToGive->GetItemToDrop())
				continue;

			float Out2 = 0;

			if (!IsBadReadPtr(ItemToGive->GetNumberToGive().GetCurve().CurveTable, 8) && ItemToGive->GetNumberToGive().GetCurve().RowName.IsValid())
			{
				Out2 = UDataTableFunctionLibrary::EvaluateCurveTableRow(ItemToGive->GetNumberToGive().GetCurve().CurveTable, ItemToGive->GetNumberToGive().GetCurve().RowName, 0.f);
			}
			else
			{
				if (IsBadReadPtr(ItemToGive->GetNumberToGive().GetCurve().CurveTable, 8))
					Out2 = ItemToGive->GetNumberToGive().GetValue();
			}

			LOG_INFO(LogDev, "[{}] Out2: {} ItemToGive.ItemToDrop: {}", j, Out2, ItemToGive->GetItemToDrop()->IsValidLowLevel() ? ItemToGive->GetItemToDrop()->GetFullName() : "BadRead");

			if (!Out2) // ?
				Out2 = 0;

			for (int i = 0; i < GetWorld()->GetNetDriver()->GetClientConnections().Num(); ++i)
			{
				auto PlayerController = Cast<AFortPlayerController>(GetWorld()->GetNetDriver()->GetClientConnections().at(i)->GetPlayerController());

				if (!PlayerController)
					continue;

				auto WorldInventory = PlayerController->GetWorldInventory();

				if (!WorldInventory)
					continue;

				bool bShouldUpdate = false;

				WorldInventory->AddItem(ItemToGive->GetItemToDrop(), &bShouldUpdate, Out2);

				if (bShouldUpdate)
					WorldInventory->Update();
			}
		}
	}

	// return OnGamePhaseStepChangedOriginal(Context, Stack, Ret);
}