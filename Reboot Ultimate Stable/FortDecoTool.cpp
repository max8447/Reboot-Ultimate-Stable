#include "FortDecoTool.h"
#include "FortPlayerControllerAthena.h"
#include "FortTrapItemDefinition.h"

void AFortDecoTool::ServerCreateBuildingAndSpawnDecoHook(AFortDecoTool* DecoTool, FVector_NetQuantize10 BuildingLocation, FRotator BuildingRotation, FVector_NetQuantize10 Location, FRotator Rotation, EBuildingAttachmentType InBuildingAttachmentType)
{
	LOG_INFO(LogGame, __FUNCTION__);

	LOG_INFO(LogDev, "BuildingLocation: {}", __int64(&BuildingLocation));
	LOG_INFO(LogDev, "BuildingRotation: {}", __int64(&BuildingRotation));
	LOG_INFO(LogDev, "Location: {}", __int64(&Location));
	LOG_INFO(LogDev, "Rotation: {}", __int64(&Rotation));

	auto Pawn = Cast<AFortPawn>(DecoTool->GetOwner());

	if (!Pawn)
		return;

	auto PlayerController = Cast<AFortPlayerControllerAthena>(Pawn->GetController());

	if (!PlayerController)
		return;

	FVector LocationToSpawn = BuildingLocation.ToVector();

	LOG_INFO(LogDev, "LocationToSpawn: {}", LocationToSpawn.ToString().ToString());
	LOG_INFO(LogDev, "ItemDefinition: {}", DecoTool->GetItemDefinition()->GetFullName());

	if (auto TrapItemDefinition = Cast<UFortContextTrapItemDefinition>(DecoTool->GetItemDefinition()))
	{
		UClass* ClassToSpawn = nullptr;

		if (TrapItemDefinition->GetFloorTrap())
			ClassToSpawn = TrapItemDefinition->GetFloorTrap()->GetBlueprintClass().Get();
		if (!ClassToSpawn && TrapItemDefinition->GetCeilingTrap())
			ClassToSpawn = TrapItemDefinition->GetCeilingTrap()->GetBlueprintClass().Get();
		if (!ClassToSpawn && TrapItemDefinition->GetWallTrap())
			ClassToSpawn = TrapItemDefinition->GetWallTrap()->GetBlueprintClass().Get();

		if (ClassToSpawn)
		{
			LOG_INFO(LogDev, "ClassToSpawn: {}", ClassToSpawn->GetFullName());

			auto TrapActor = GetWorld()->SpawnActor<ABuildingSMActor>(ClassToSpawn, LocationToSpawn/* , Rotation.Quaternion() */);

			if (TrapActor)
			{
				LOG_ERROR(LogGame, "Failed to spawn trap!");
				return;
			}
		}
	}
	else if (auto DecoItemDefinition = Cast<UFortDecoItemDefinition>(DecoTool->GetItemDefinition()))
	{
		UClass* ClassToSpawn = DecoItemDefinition->GetBlueprintClass().Get();

		if (ClassToSpawn)
		{
			LOG_INFO(LogDev, "ClassToSpawn: {}", ClassToSpawn->GetFullName());

			auto TrapActor = GetWorld()->SpawnActor<ABuildingSMActor>(ClassToSpawn, LocationToSpawn/* , Rotation.Quaternion() */);

			if (TrapActor)
			{
				LOG_ERROR(LogGame, "Failed to spawn trap!");
				return;
			}
		}
	}
}