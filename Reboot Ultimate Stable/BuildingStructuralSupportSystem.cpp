#include "BuildingStructuralSupportSystem.h"

#include "reboot.h"

bool UBuildingStructuralSupportSystem::IsWorldLocValid(const FVector& WorldLoc)
{
	static auto IsWorldLocValidFn = FindObject<UFunction>(L"/Script/FortniteGame.BuildingStructuralSupportSystem.IsWorldLocValid");

	if (!IsWorldLocValidFn)
		return true;

	struct { FVector WorldLoc; bool Ret; } Params{ WorldLoc };

	this->ProcessEvent(IsWorldLocValidFn, &Params);

	return Params.Ret;
}