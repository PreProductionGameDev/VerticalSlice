// 2022 ChronoOwl Studios
// Jacob

#include "Core/Elements/Gravity/GA_GravityMovement.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "FP_Character.h"
#include "Kismet/KismetSystemLibrary.h"

/**
 * @name Jacob
 * @brief sets the defaults for the abilities 
 */
UGA_GravityMovement::UGA_GravityMovement()
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
 * @brief Upon activation will trigger the logic of the ability
 * @param Handle 
 * @param ActorInfo 
 * @param ActivationInfo 
 * @param TriggerEventData 
 */
void UGA_GravityMovement::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if(Cast<AFP_Character>(GetOwningActorFromActorInfo())->GetStamina() < 0.5f)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
	}
	// Spawn Parameters
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<AFP_Character>(GetCurrentActorInfo()->OwnerActor);
	const FVector SpawnLocation = GetCurrentActorInfo()->OwnerActor->GetActorLocation();
	const FRotator SpawnRotation = GetCurrentActorInfo()->OwnerActor->GetActorRotation();

	// Spawns the Indicator
	ImpulseIndicator = Cast<AImpulseIndicator>(GetWorld()->SpawnActor(AImpulseIndicator::StaticClass(), &SpawnLocation, &SpawnRotation, SpawnParameters));
	
	// Start moving indicator
	FTimerDynamicDelegate DynamicDelegate;
	DynamicDelegate.BindUFunction(this, "SyncCamera");

	FTimerManager& TimerManager= GetWorld()->GetTimerManager();
	ImpulseIndicatorTimerHandle = TimerManager.K2_FindDynamicTimerHandle(DynamicDelegate);
	TimerManager.SetTimer(ImpulseIndicatorTimerHandle, DynamicDelegate, 0.02f, true,0);

	// Execute teleport on input release
	InputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRelease->OnRelease.AddDynamic(this, &UGA_GravityMovement::OnKeyReleased);
	InputRelease->ReadyForActivation();
}

/**
 * @name Jacob
 * @brief ends and cleans up the ability
 * @param Handle 
 * @param ActorInfo 
 * @param ActivationInfo 
 * @param bReplicateEndAbility 
 * @param bWasCancelled 
 */
void UGA_GravityMovement::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * @name Jacob
 * @brief Moves the indicator for the impulse in front of the players camera
 */
void UGA_GravityMovement::MoveImpulsePoint()
{	
	// Makes sure stamina doesn't regen while casting
	Cast<AFP_Character>(GetOwningActorFromActorInfo())->UseStamina(0);
	
	// Line cast forward 
	FHitResult OutHit;
	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(ETraceTypeQuery::TraceTypeQuery3);
	FCollisionQueryParams Params("LineTraceSingle", SCENE_QUERY_STAT_ONLY(KismetTraceUtils), false);
	Params.AddIgnoredActor(GetOwningActorFromActorInfo());
	Params.AddIgnoredActor(ImpulseIndicator);
	FVector Begin =  Cast<AFP_Character>(GetOwningActorFromActorInfo())->FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Begin +  Cast<AFP_Character>(GetOwningActorFromActorInfo())->FirstPersonCameraComponent->GetComponentRotation().Vector() * 1000 ;

	// Teleports to where the hit was or where the max range is 
	if(GetWorld()->LineTraceSingleByChannel(OutHit, Begin, End, CollisionChannel, Params))
	{
		ImpulseIndicator->SetActorLocation(OutHit.Location - Cast<AFP_Character>(GetOwningActorFromActorInfo())->FirstPersonCameraComponent->GetComponentRotation().Vector() * 44 );
		return;
	}
	ImpulseIndicator->SetActorLocation(End);
}

/**
 * @name Jacob
 * @brief Ensures that the server has the correct camera rotation before moving the indicator
 */
void UGA_GravityMovement::SyncCamera()
{
	Cast<AFP_Character>(GetOwningActorFromActorInfo())->ClientCameraRotation();
	UAbilityTask_NetworkSyncPoint* NetworkSync = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::BothWait);
	NetworkSync->OnSync.AddDynamic(this, &UGA_GravityMovement::MoveImpulsePoint);
	NetworkSync->ReadyForActivation();
}

/**
 * @name Jacob
 * @brief applies an impulse around the indicator and takes the stamina cost
 * @param TimePressed 
 */
void UGA_GravityMovement::OnKeyReleased(float TimePressed)
{
	if(Cast<AFP_Character>(GetOwningActorFromActorInfo())->UseStamina(0.5f))
	{
		TArray<AActor*> OverlapActors;
		TArray<AActor*> IgnoredActors;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		if(UKismetSystemLibrary::SphereOverlapActors(this, ImpulseIndicator->GetActorLocation(),800.0f,ObjectTypes,AFP_Character::StaticClass(),IgnoredActors,OverlapActors ))
		{
			UE_LOG(LogTemp, Warning, TEXT("OverlappedActors"));

			for(AActor* OverlapedActor : OverlapActors)
			{
				Cast<AFP_Character>(OverlapedActor)->LaunchCharacter((OverlapedActor->GetActorLocation()- ImpulseIndicator->GetActorLocation()).GetSafeNormal()*2500.0f, true, true);
				Cast<AFP_Character>(OverlapedActor)->ServerPlaySoundAtLocation(this, SoundCue, ImpulseIndicator->GetActorLocation(), ImpulseIndicator->GetActorRotation());
			}
		}
	}
	GetWorld()->DestroyActor(ImpulseIndicator);
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
