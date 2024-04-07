#include "FortAthenaAIBotCustomizationData.h"
#include "FortServerBotManagerAthena.h"
#include "FortGameModeAthena.h"

void UFortAthenaAIBotCustomizationData::ApplyOverrideCharacterCustomizationHook(UFortAthenaAIBotCustomizationData* InBotData, AFortPlayerPawn* NewBot, __int64 idk)
{
	LOG_INFO(LogGame, "ApplyOverrideCharacterCustomizationHook");

	auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->GetGameMode());
	auto BotManager = GameMode->GetServerBotManager();
	auto Location = NewBot->GetActorLocation();
	auto Rotation = NewBot->GetActorRotation();
	FFortAthenaAIBotRunTimeCustomizationData RuntimeBotData{};
	RuntimeBotData.GetCustomSquadId() = 0;

	UFortServerBotManagerAthena::SpawnBotHook(BotManager, Location, Rotation, InBotData, RuntimeBotData);
	NewBot->K2_DestroyActor();
}