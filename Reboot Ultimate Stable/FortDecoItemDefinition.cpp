#include "FortDecoItemDefinition.h"

UClass* UFortDecoItemDefinition::StaticClass()
{
	static auto Class = FindObject<UClass>("/Script/FortniteGame.FortDecoItemDefinition");
	return Class;
}