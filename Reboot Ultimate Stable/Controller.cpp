#include "Controller.h"

#include "reboot.h"

AActor* AController::GetViewTarget()
{
	static auto GetViewTargetFn = FindObject<UFunction>(L"/Script/Engine.Controller.GetViewTarget");
	AActor* ViewTarget = nullptr;
	this->ProcessEvent(GetViewTargetFn, &ViewTarget);
	return ViewTarget;
}

void AController::Possess(class APawn* Pawn)
{
	auto PossessFn = FindFunction("Possess");
	this->ProcessEvent(PossessFn, &Pawn);
}

void AController::UnPossess()
{
	auto UnPossessFn = FindFunction("UnPossess");
	this->ProcessEvent(UnPossessFn);
}

void AController::SetControlRotation(FRotator NewRotation)
{
	static auto SetControlRotationFn = FindObject<UFunction>("/Script/Engine.Controller.SetControlRotation");
	this->ProcessEvent(SetControlRotationFn, &NewRotation);
}