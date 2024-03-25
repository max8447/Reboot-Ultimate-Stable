#pragma once

#include "Object.h"
#include "TSubclassOf.h"

class UCheatManager : public UObject
{
public:
	void Teleport();
	void DestroyTarget();
	void DestroyAll(TSubclassOf<AActor> AClass);

	static UClass* StaticClass();
};