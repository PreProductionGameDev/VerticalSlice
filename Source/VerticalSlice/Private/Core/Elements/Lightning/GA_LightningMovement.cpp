// 2022 ChronoOwl Studios\
// Jacob


#include "Core/Elements/Lightning/GA_LightningMovement.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "FP_Character.h"


/**
 * @name Jacob
 * @brief Sets the defaults for an elemental ability
 */
UGA_LightningMovement::UGA_LightningMovement()
{
	//Changing defaults from parent
	AbilityInputID = EPlayerAbilityInputID::MovementAbility;
	bSourceObjectMustBeCurrentWeaponToActivate = false;
	bSourceObjectMustBeCurrentElementToActivate = true;

	// Ability Tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Element.Movement")));
}

/**
 * @name Jacob
 * @brief Spawns the indicator on the server starts moving it and waits for the input key to be released
 * @param Handle 
 * @param ActorInfo 
 * @param ActivationInfo 
 * @param TriggerEventData 
 */

void UGA_LightningMovement::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	// Activates parent logic so ability works
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(Cast<AFP_Character>(GetOwningActorFromActorInfo())->GetStamina() < StaminaCost)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}
	
	// Spawn Parameters
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<AFP_Character>(GetCurrentActorInfo()->OwnerActor);
	const FVector SpawnLocation = GetCurrentActorInfo()->OwnerActor->GetActorLocation();
	const FRotator SpawnRotation = GetCurrentActorInfo()->OwnerActor->GetActorRotation();

	// Spawns the Indicator
	Indicator = GetWorld()->SpawnActor(IndicatorClass, &SpawnLocation, &SpawnRotation, SpawnParameters);
	
	// Start moving indicator
	FTimerDynamicDelegate DynamicDelegate;
	DynamicDelegate.BindUFunction(this, "SyncCamera");

	FTimerManager& TimerManager= GetWorld()->GetTimerManager();
	IndicatorUpdateTimer = TimerManager.K2_FindDynamicTimerHandle(DynamicDelegate);
	TimerManager.SetTimer(IndicatorUpdateTimer, DynamicDelegate, 0.02f, true,0);
	
	// Execute teleport on input release
	InputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRelease->OnRelease.AddDynamic(this, &UGA_LightningMovement::OnKeyReleased);
	InputRelease->ReadyForActivation();

}

/**
 * @name Jacob
 * @brief ends the ability
 * @param Handle 
 * @param ActorInfo
 * @param ActivationInfo 
 * @param bReplicateEndAbility 
 * @param bWasCancelled 
 */
void UGA_LightningMovement::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * @name Jacob
 * @brief Moves the indicator in front of the players camera
 */
void UGA_LightningMovement::MoveIndicator()
{
	// Makes sure stamina doesn't regen while casting
	Cast<AFP_Character>(GetOwningActorFromActorInfo())->UseStamina(0);

	// Line cast forward 
	FHitResult OutHit;
	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(ETraceTypeQuery::TraceTypeQuery3);
	FCollisionQueryParams Params("LineTraceSingle", SCENE_QUERY_STAT_ONLY(KismetTraceUtils), false);
	Params.AddIgnoredActor(GetOwningActorFromActorInfo());
	Params.AddIgnoredActor(Indicator);
	FVector Begin =  Cast<AFP_Character>(GetOwningActorFromActorInfo())->FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Begin +  Cast<AFP_Character>(GetOwningActorFromActorInfo())->FirstPersonCameraComponent->GetComponentRotation().Vector() * TeleportDistance ;

	// Teleports to where the hit was or where the max range is 
	if(GetWorld()->LineTraceSingleByChannel(OutHit, Begin, End, CollisionChannel, Params))
	{
		Indicator->SetActorLocation(OutHit.Location - Cast<AFP_Character>(GetOwningActorFromActorInfo())->FirstPersonCameraComponent->GetComponentRotation().Vector() * 44 );
		return;
	}
	Indicator->SetActorLocation(End);
}

/**
 * @name Jacob
 * @brief Syncs the camera before moving the indicator 
 */
void UGA_LightningMovement::SyncCamera()
{
	Cast<AFP_Character>(GetOwningActorFromActorInfo())->ClientCameraRotation();
	UAbilityTask_NetworkSyncPoint* NetworkSync = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::BothWait);
	NetworkSync->OnSync.AddDynamic(this, &UGA_LightningMovement::MoveIndicator);
	NetworkSync->ReadyForActivation();
}

/**
 * @name Jacob
 * @brief Teleports the player  to the indicator
 * @param TimePressed needed for the key released
 */
void UGA_LightningMovement::OnKeyReleased(float TimePressed)
{
	UE_LOG(LogTemp, Warning, TEXT("release"));
	Cast<AFP_Character>(GetOwningActorFromActorInfo())->UseStamina(StaminaCost);
	
	// Clean up timer
	GetWorld()->GetTimerManager().ClearTimer(IndicatorUpdateTimer);
	IndicatorUpdateTimer.Invalidate();

	// Activate indicator teleport and reset it
	TeleportPlayer();
	Indicator = nullptr;
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
