#include "GameMode.h"

#include "reboot.h"

void AGameMode::RestartGame()
{
	static auto RestartGameFn = FindObject<UFunction>(L"/Script/Engine.GameMode.RestartGame");
	this->ProcessEvent(RestartGameFn);
}

void AGameMode::EndMatch()
{
	static auto EndMatchFn = FindObject<UFunction>(L"/Script/Engine.GameMode.EndMatch");
	this->ProcessEvent(EndMatchFn);
}