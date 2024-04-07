#pragma once

#include "Object.h"
#include "SubclassOf.h"
#include "Actor.h"

class UCheatManager : public UObject
{
public:
	void Teleport();
	void DestroyTarget();
	void DestroyAll(TSubclassOf<AActor> AClass);

	static UClass* StaticClass();
};