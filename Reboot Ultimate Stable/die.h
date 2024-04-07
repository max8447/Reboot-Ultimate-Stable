#pragma once

#include "reboot.h"
#include "FortGameModeAthena.h"

extern inline void (*SetZoneToIndexOriginal)(AFortGameModeAthena* GameModeAthena, int OverridePhaseMaybeIDFK) = nullptr;

void SetZoneToIndexHook(AFortGameModeAthena* GameModeAthena, int OverridePhaseMaybeIDFK);

static inline void ProcessEventHook(UObject* Object, UFunction* Function, void* Parameters)
{
	if (!Object || !Function)
		return;

	if (Globals::bLogProcessEvent)
	{
		auto FunctionName = Function->GetName(); // UKismetSystemLibrary::GetPathName(Function).ToString();
		auto FunctionFullName = Function->GetFullName();
		auto ObjectName = Object->GetName();

		if (!strstr(FunctionName.c_str(), ("EvaluateGraphExposedInputs")) &&
			!strstr(FunctionName.c_str(), ("Tick")) &&
			!strstr(FunctionName.c_str(), ("OnSubmixEnvelope")) &&
			!strstr(FunctionName.c_str(), ("OnSubmixSpectralAnalysis")) &&
			!strstr(FunctionName.c_str(), ("OnMouse")) &&
			!strstr(FunctionName.c_str(), ("Pulse")) &&
			!strstr(FunctionName.c_str(), ("BlueprintUpdateAnimation")) &&
			!strstr(FunctionName.c_str(), ("BlueprintPostEvaluateAnimation")) &&
			!strstr(FunctionName.c_str(), ("BlueprintModifyCamera")) &&
			!strstr(FunctionName.c_str(), ("BlueprintModifyPostProcess")) &&
			!strstr(FunctionName.c_str(), ("Loop Animation Curve")) &&
			!strstr(FunctionName.c_str(), ("UpdateTime")) &&
			!strstr(FunctionName.c_str(), ("GetMutatorByClass")) &&
			!strstr(FunctionName.c_str(), ("UpdatePreviousPositionAndVelocity")) &&
			!strstr(FunctionName.c_str(), ("IsCachedIsProjectileWeapon")) &&
			!strstr(FunctionName.c_str(), ("LockOn")) &&
			!strstr(FunctionName.c_str(), ("GetAbilityTargetingLevel")) &&
			!strstr(FunctionName.c_str(), ("ReadyToEndMatch")) &&
			!strstr(FunctionName.c_str(), ("ReceiveDrawHUD")) &&
			!strstr(FunctionName.c_str(), ("OnUpdateDirectionalLightForTimeOfDay")) &&
			!strstr(FunctionName.c_str(), ("GetSubtitleVisibility")) &&
			!strstr(FunctionName.c_str(), ("GetValue")) &&
			!strstr(FunctionName.c_str(), ("InputAxisKeyEvent")) &&
			!strstr(FunctionName.c_str(), ("ServerTouchActiveTime")) &&
			!strstr(FunctionName.c_str(), ("SM_IceCube_Blueprint_C")) &&
			!strstr(FunctionName.c_str(), ("OnHovered")) &&
			!strstr(FunctionName.c_str(), ("OnCurrentTextStyleChanged")) &&
			!strstr(FunctionName.c_str(), ("OnButtonHovered")) &&
			!strstr(FunctionName.c_str(), ("ExecuteUbergraph_ThreatPostProcessManagerAndParticleBlueprint")) &&
			!strstr(FunctionName.c_str(), "PinkOatmeal") &&
			!strstr(FunctionName.c_str(), "CheckForDancingAtFish") &&
			!strstr(FunctionName.c_str(), ("UpdateCamera")) &&
			!strstr(FunctionName.c_str(), ("GetMutatorContext")) &&
			!strstr(FunctionName.c_str(), ("CanJumpInternal")) &&
			!strstr(FunctionName.c_str(), ("OnDayPhaseChanged")) &&
			!strstr(FunctionName.c_str(), ("Chime")) &&
			!strstr(FunctionName.c_str(), ("ServerMove")) &&
			!strstr(FunctionName.c_str(), ("OnVisibilitySetEvent")) &&
			!strstr(FunctionName.c_str(), "ReceiveHit") &&
			!strstr(FunctionName.c_str(), "ReadyToStartMatch") &&
			!strstr(FunctionName.c_str(), "K2_GetComponentToWorld") &&
			!strstr(FunctionName.c_str(), "ClientAckGoodMove") &&
			!strstr(FunctionName.c_str(), "Prop_WildWest_WoodenWindmill_01") &&
			!strstr(FunctionName.c_str(), "ContrailCheck") &&
			!strstr(FunctionName.c_str(), "B_StockBattleBus_C") &&
			!strstr(FunctionName.c_str(), "Subtitles.Subtitles_C.") &&
			!strstr(FunctionName.c_str(), "/PinkOatmeal/PinkOatmeal_") &&
			!strstr(FunctionName.c_str(), "BP_SpectatorPawn_C") &&
			!strstr(FunctionName.c_str(), "FastSharedReplication") &&
			!strstr(FunctionName.c_str(), "OnCollisionHitEffects") &&
			!strstr(FunctionName.c_str(), "BndEvt__SkeletalMesh") &&
			!strstr(FunctionName.c_str(), ".FortAnimInstance.AnimNotify_") &&
			!strstr(FunctionName.c_str(), "OnBounceAnimationUpdate") &&
			!strstr(FunctionName.c_str(), "ShouldShowSoundIndicator") &&
			!strstr(FunctionName.c_str(), "Primitive_Structure_AmbAudioComponent_C") &&
			!strstr(FunctionName.c_str(), "PlayStoppedIdleRotationAudio") &&
			!strstr(FunctionName.c_str(), "UpdateOverheatCosmetics") &&
			!strstr(FunctionName.c_str(), "StormFadeTimeline__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "BindVolumeEvents") &&
			!strstr(FunctionName.c_str(), "UpdateStateEvent") &&
			!strstr(FunctionName.c_str(), "VISUALS__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "Flash__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "SetCollisionEnabled") &&
			!strstr(FunctionName.c_str(), "SetIntensity") &&
			!strstr(FunctionName.c_str(), "Storm__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "CloudsTimeline__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "SetRenderCustomDepth") &&
			!strstr(FunctionName.c_str(), "K2_UpdateCustomMovement") &&
			!strstr(FunctionName.c_str(), "AthenaHitPointBar_C.Update") &&
			!strstr(FunctionName.c_str(), "ExecuteUbergraph_Farm_WeatherVane_01") &&
			!strstr(FunctionName.c_str(), "HandleOnHUDElementVisibilityChanged") &&
			!strstr(FunctionName.c_str(), "ExecuteUbergraph_Fog_Machine") &&
			!strstr(FunctionName.c_str(), "ReceiveBeginPlay") &&
			!strstr(FunctionName.c_str(), "OnMatchStarted") &&
			!strstr(FunctionName.c_str(), "CustomStateChanged") &&
			!strstr(FunctionName.c_str(), "OnBuildingActorInitialized") &&
			!strstr(FunctionName.c_str(), "OnWorldReady") &&
			!strstr(FunctionName.c_str(), "OnAttachToBuilding") &&
			!strstr(FunctionName.c_str(), "Clown Spinner") &&
			!strstr(FunctionName.c_str(), "K2_GetActorLocation") &&
			!strstr(FunctionName.c_str(), "GetViewTarget") &&
			!strstr(FunctionName.c_str(), "GetAllActorsOfClass") &&
			!strstr(FunctionName.c_str(), "OnUpdateMusic") &&
			!strstr(FunctionName.c_str(), "Check Closest Point") &&
			!strstr(FunctionName.c_str(), "OnSubtitleChanged__DelegateSignature") &&
			!strstr(FunctionName.c_str(), "OnServerBounceCallback") &&
			!strstr(FunctionName.c_str(), "BlueprintGetInteractionTime") &&
			!strstr(FunctionName.c_str(), "OnServerStopCallback") &&
			!strstr(FunctionName.c_str(), "Light Flash Timeline__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "MainFlightPath__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "PlayStartedIdleRotationAudio") &&
			!strstr(FunctionName.c_str(), "BGA_Athena_FlopperSpawn_") &&
			!strstr(FunctionName.c_str(), "CheckShouldDisplayUI") &&
			!strstr(FunctionName.c_str(), "Timeline_0__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "ClientMoveResponsePacked") &&
			!strstr(FunctionName.c_str(), "ExecuteUbergraph_B_Athena_FlopperSpawnWorld_Placement") &&
			!strstr(FunctionName.c_str(), "Countdown__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "OnParachuteTrailUpdated") &&
			!strstr(FunctionName.c_str(), "Moto FadeOut__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "ExecuteUbergraph_Apollo_GasPump_Valet") &&
			!strstr(FunctionName.c_str(), "GetOverrideMeshMaterial") &&
			!strstr(FunctionName.c_str(), "VendWobble__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "WaitForPawn") &&
			!strstr(FunctionName.c_str(), "FragmentMovement__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "TrySetup") &&
			!strstr(FunctionName.c_str(), "Fade Doused Smoke__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "SetPlayerToSkydive") &&
			!strstr(FunctionName.c_str(), "BounceCar__UpdateFunc") &&
			!strstr(FunctionName.c_str(), "BP_CalendarDynamicPOISelect") &&
			!strstr(FunctionName.c_str(), "OnComponentHit_Event_0") &&
			!strstr(FunctionName.c_str(), "HandleSimulatingComponentHit") &&
			!strstr(FunctionName.c_str(), "CBGA_GreenGlop_WithGrav_C") &&
			!strstr(FunctionName.c_str(), "WarmupCountdownEndTimeUpdated") &&
			!strstr(FunctionName.c_str(), "BP_CanInteract") &&
			!strstr(FunctionName.c_str(), "AthenaHitPointBar_C") &&
			!strstr(FunctionName.c_str(), "ServerFireAIDirectorEvent") &&
			!strstr(FunctionName.c_str(), "BlueprintThreadSafeUpdateAnimation") &&
			!strstr(FunctionName.c_str(), "On Amb Zap Spawn") &&
			!strstr(FunctionName.c_str(), "ServerSetPlayerCanDBNORevive") &&
			!strstr(FunctionName.c_str(), "BGA_Petrol_Pickup_C") &&
			!strstr(FunctionName.c_str(), "GetMutatorsForContextActor") &&
			!strstr(FunctionName.c_str(), "GetControlRotation") &&
			!strstr(FunctionName.c_str(), "K2_GetComponentLocation") &&
			!strstr(FunctionName.c_str(), "MoveFromOffset__UpdateFunc") &&
			!strstr(FunctionFullName.c_str(), "PinkOatmeal_GreenGlop_C") &&
			!strstr(ObjectName.c_str(), "CBGA_GreenGlop_WithGrav_C") &&
			!strstr(ObjectName.c_str(), "FlopperSpawn") &&
			!strstr(FunctionFullName.c_str(), "GCNL_EnvCampFire_Fire_C") &&
			!strstr(FunctionName.c_str(), "BlueprintGetAllHighlightableComponents") &&
			!strstr(FunctionFullName.c_str(), "Primitive_Structure_AmbAudioComponent") &&
			!strstr(FunctionName.c_str(), "ServerTriggerCombatEvent") &&
			!strstr(FunctionName.c_str(), "SpinCubeTimeline__UpdateFunc") &&
			!strstr(ObjectName.c_str(), "FortPhysicsObjectComponent") &&
			!strstr(FunctionName.c_str(), "GetTextValue") &&
			!strstr(FunctionName.c_str(), "ExecuteUbergraph_BGA_Petrol_Pickup"))
		{
			LOG_INFO(LogDev, "Function called: {} with {}", FunctionFullName, ObjectName);
		}

		if (FunctionFullName.contains("K2_ActivateAbilityFromEvent"))
		{
			struct FGameplayAbilityTargetDataHandle
			{
				uint8                                         Pad_3965[0x28];                                    // 0x0000(0x0028)(Fixing Struct Size After Last Property [ Dumper-7 ])
			};

			struct FGameplayEventDataTest
			{
				struct FGameplayTag                           EventTag;                                          // 0x0000(0x0008)(Edit, BlueprintVisible, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
				class AActor* Instigator;                                        // 0x0008(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
				class AActor* Target;                                            // 0x0010(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
				class UObject* OptionalObject;                                    // 0x0018(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
				class UObject* OptionalObject2;                                   // 0x0020(0x0008)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
				struct FGameplayEffectContextHandle           ContextHandle;                                     // 0x0028(0x0018)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
				struct FGameplayTagContainer                  InstigatorTags;                                    // 0x0040(0x0020)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
				struct FGameplayTagContainer                  TargetTags;                                        // 0x0060(0x0020)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
				float                                         EventMagnitude;                                    // 0x0080(0x0004)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
				uint8                                         Pad_3966[0x4];                                     // 0x0084(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
				struct FGameplayAbilityTargetDataHandle       TargetData;                                        // 0x0088(0x0028)(Edit, BlueprintVisible, NativeAccessSpecifierPublic)
			};

			FGameplayEventDataTest EventData = *(FGameplayEventDataTest*)Parameters;

			LOG_ERROR(LogDev, "EventTag: {}", EventData.EventTag.TagName.ToString());
			if (EventData.Instigator)
				LOG_ERROR(LogDev, "Instigator: {}", EventData.Instigator->GetFullName());
			if (EventData.Target)
				LOG_ERROR(LogDev, "Target: {}", EventData.Target->GetFullName());
			if (EventData.OptionalObject)
				LOG_ERROR(LogDev, "OptionalObject: {}", EventData.OptionalObject->GetFullName());
			if (EventData.OptionalObject2)
				LOG_ERROR(LogDev, "OptionalObject2: {}", EventData.OptionalObject2->GetFullName());
			for (int i = 0; i < sizeof(EventData.ContextHandle.UnknownData00); i++)
			{
				LOG_ERROR(LogDev, "ContextHandle: {}", EventData.ContextHandle.UnknownData00[i]);
			}
			for (int i = 0; i < EventData.InstigatorTags.GameplayTags.Num(); i++)
			{
				LOG_ERROR(LogDev, "InstigatorTag: {}", EventData.InstigatorTags.GameplayTags.at(i).TagName.ToString());
			}
			for (int i = 0; i < EventData.InstigatorTags.ParentTags.Num(); i++)
			{
				LOG_ERROR(LogDev, "InstigatorTag: {}", EventData.InstigatorTags.ParentTags.at(i).TagName.ToString());
			}
			for (int i = 0; i < EventData.TargetTags.GameplayTags.Num(); i++)
			{
				LOG_ERROR(LogDev, "TargetTag: {}", EventData.TargetTags.GameplayTags.at(i).TagName.ToString());
			}
			for (int i = 0; i < EventData.TargetTags.ParentTags.Num(); i++)
			{
				LOG_ERROR(LogDev, "TargetTag: {}", EventData.TargetTags.ParentTags.at(i).TagName.ToString());
			}
			LOG_ERROR(LogDev, "EventMagnitude: {}", EventData.EventMagnitude);
			for (int i = 0; i < sizeof(EventData.Pad_3966); i++)
			{
				LOG_ERROR(LogDev, "a: {}", EventData.Pad_3966[i]);
			}
			for (int i = 0; i < sizeof(EventData.TargetData.Pad_3965); i++)
			{
				LOG_ERROR(LogDev, "TargetData: {}", EventData.TargetData.Pad_3965[i]);
			}
		}
	}

	return Object->ProcessEvent(Function, Parameters);
}