#pragma once

#include "Object.h"
#include "Rotator.h"

class UKismetMathLibrary : public UObject
{
public:
	static float RandomFloatInRange(float min, float max);
	static int RandomIntegerInRange(int min, int max);
	static bool RandomBoolWithWeight(float weight);
	static FRotator FindLookAtRotation(FVector Start, FVector Target);

	static UClass* StaticClass();
};