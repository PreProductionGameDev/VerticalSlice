// 2022 ChronoOwl Studios
// Jacob


#include "Core/Elements/Fire/GA_FireMovement.h"

#include "FP_Character.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Core/Elements/Fire/TrailHitbox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

/**
 * @name Jacob
 * @brief overites the parents defaults
 */
UGA_FireMovement::UGA_FireMovement()
{
	//Changing defaults from parent
	AbilityInputID = EPlayerAbilityInputID::MovementAbility;
	bSourceObjectMustBeCurrentWeaponToActivate = false;
	bSourceObjectMustBeCurrentElementToActivate = true;
}

/**
 * @name Jacob
 * @brief Sets the player speed and acceleration and starts the timers?
 * @param Handle 
 * @param ActorInfo 
 * @param ActivationInfo 
 * @param TriggerEventData 
 */
void UGA_FireMovement::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	//activate parent so ability works
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(Cast<AFP_Character>(GetOwningActorFromActorInfo())->GetStamina() < 0.0125)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}
	
	//start spawning fire
	FTimerDynamicDelegate DynamicDelegate;
	DynamicDelegate.BindUFunction(this, "SpawnFire");

	FTimerManager& TimerManager= GetWorld()->GetTimerManager();
	SpawnTimer = TimerManager.K2_FindDynamicTimerHandle(DynamicDelegate);
	TimerManager.SetTimer(SpawnTimer, DynamicDelegate, 0.04f, true,0);

	//changes the player movement
	UCharacterMovementComponent* CharacterMovementComponent= Cast<AFP_Character>(GetActorInfo().OwnerActor)->GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 2000;
	CharacterMovementComponent->MaxAcceleration = 3040;

	//reset on input release
	InputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRelease->OnRelease.AddDynamic(this, &UGA_FireMovement::StopSpawning);
	InputRelease->ReadyForActivation();
	
}


/**
 * @name Jacob
 * @brief handles cleanup of the ability
 * @param Handle 
 * @param ActorInfo 
 * @param ActivationInfo 
 * @param bReplicateEndAbility 
 * @param bWasCancelled 
 */
void UGA_FireMovement::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * @name Jacob
 * @brief Spawns Fire actor at player location
 */
void UGA_FireMovement::SpawnFire()
{
	if(!Cast<AFP_Character>(GetCurrentActorInfo()->OwnerActor)->UseStamina(0.0125))
	{
		StopSpawning(0.0f);
	}
	UE_LOG(LogTemp, Warning, TEXT("FireSpawned"));
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<AFP_Character>(GetCurrentActorInfo()->OwnerActor);
	const FVector SpawnLocation = GetCurrentActorInfo()->OwnerActor->GetActorLocation();
	const FRotator SpawnRotation = GetCurrentActorInfo()->OwnerActor->GetActorRotation();
	GetWorld()->SpawnActor(ATrailHitbox::StaticClass(), &SpawnLocation, &SpawnRotation, SpawnParameters);
}

/**
 * @name Jacob
 * @brief Stops spawning fire and resets the character movement speed
 * @param TimeHeld not used variable required for input release
 */
void UGA_FireMovement::StopSpawning(float TimeHeld)
{
	UE_LOG(LogTemp, Warning, TEXT("keyReleased"));
	
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	SpawnTimer.Invalidate();

	//changes the player movement
	UCharacterMovementComponent* CharacterMovementComponent= Cast<AFP_Character>(GetActorInfo().OwnerActor)->GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 1200;
	CharacterMovementComponent->MaxAcceleration = 2048;
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}



