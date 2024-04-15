#include "FortDecoItemDefinition.h"

UClass* UFortDecoItemDefinition::StaticClass()
{
	static auto Class = FindObject<UClass>(L"/Script/FortniteGame.FortDecoItemDefinition");
	return Class;
}