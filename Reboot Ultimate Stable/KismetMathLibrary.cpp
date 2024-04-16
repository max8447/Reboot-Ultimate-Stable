#include "KismetMathLibrary.h"

#include "reboot.h"

float UKismetMathLibrary::RandomFloatInRange(float min, float max)
{
	static auto fn = FindObject<UFunction>(L"/Script/Engine.KismetMathLibrary.RandomFloatInRange");

	struct { float min; float max; float ret; } params{min, max};

	static auto DefaultObject = StaticClass();
	DefaultObject->ProcessEvent(fn, &params);

	return params.ret;
}

int UKismetMathLibrary::RandomIntegerInRange(int min, int max)
{
	static auto fn = FindObject<UFunction>("/Script/Engine.KismetMathLibrary.RandomIntegerInRange");

	struct { int min; int max; int ret; } params{ min, max };

	static auto DefaultObject = StaticClass();
	DefaultObject->ProcessEvent(fn, &params);

	return params.ret;
}

bool UKismetMathLibrary::RandomBoolWithWeight(float weight)
{
	static auto fn = FindObject<UFunction>(L"/Script/Engine.KismetMathLibrary.RandomBoolWithWeight");

	struct { float weight; bool ret; } params{ weight };

	static auto DefaultObject = StaticClass();
	DefaultObject->ProcessEvent(fn, &params);

	return params.ret;
}

FRotator UKismetMathLibrary::FindLookAtRotation(FVector Start, FVector Target)
{
	static auto fn = FindObject<UFunction>("/Script/Engine.KismetMathLibrary.FindLookAtRotation");

	struct { FVector Start; FVector Target; FRotator ret; } params{ Start , Target };

	static auto DefaultObject = StaticClass();
	DefaultObject->ProcessEvent(fn, &params);

	return params.ret;
}

UClass* UKismetMathLibrary::StaticClass()
{
	static auto Class = FindObject<UClass>(L"/Script/Engine.KismetMathLibrary");
	return Class;
}