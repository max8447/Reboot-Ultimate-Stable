#include "CheatManager.h"

#include "reboot.h"

void UCheatManager::Teleport()
{
	static auto TeleportFn = FindObject<UFunction>(L"/Script/Engine.CheatManager.Teleport");
	this->ProcessEvent(TeleportFn);
}

void UCheatManager::DestroyTarget()
{
	static auto DestroyTargetFn = FindObject<UFunction>(L"/Script/Engine.CheatManager.DestroyTarget");
	this->ProcessEvent(DestroyTargetFn);
}

void UCheatManager::DestroyAll(TSubclassOf<AActor> AClass)
{
	static auto DestroyAllFn = FindObject<UFunction>(L"/Script/Engine.CheatManager.DestroyAll");
	this->ProcessEvent(DestroyAllFn, &AClass);
}

UClass* UCheatManager::StaticClass()
{
	static auto Class = FindObject<UClass>(L"/Script/Engine.CheatManager");
	return Class;
}