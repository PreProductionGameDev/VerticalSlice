// 2022 ChronoOwl Studios


#include "Core/Elements/Fire/GA_FireMovement.h"

#include "FP_Character.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Core/Elements/Fire/TrailHitbox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UGA_FireMovement::UGA_FireMovement()
{
	//Changing defaults from parent
	AbilityInputID = EPlayerAbilityInputID::MovementAbility;
	bSourceObjectMustBeCurrentWeaponToActivate = false;
	bSourceObjectMustBeCurrentElementToActivate = true;
}

void UGA_FireMovement::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	//activate parent so ability works
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

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

void UGA_FireMovement::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

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



