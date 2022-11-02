// 2022 ChronoOwl Studios
// Jacob

#include "Core/Elements/Gravity/GA_GravityMovement.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "FP_Character.h"
#include "NiagaraFunctionLibrary.h"
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
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	Cast<AFP_Character>(GetCurrentActorInfo()->OwnerActor)->UseStamina(0.5f);
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.Element.Movement")));
	
	SyncCamera();
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
	if(bWasCancelled)
	{
		if(ImpulseIndicator)
		{
			GetWorld()->DestroyActor(ImpulseIndicator);
		}
	}
	else
	{
		//tag for dropping the flag
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.Element.Movement")));
	}
}

/**
 * @name Jacob
 * @brief Moves the indicator for the impulse in front of the players camera
 */
void UGA_GravityMovement::ThrowImpulse()
{	
	// Spawn Parameters
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<AFP_Character>(GetCurrentActorInfo()->OwnerActor);
	const FVector SpawnLocation = Cast<AFP_Character>(GetCurrentActorInfo()->OwnerActor)->FirstPersonCameraComponent.Get()->GetComponentLocation();
	const FRotator SpawnRotation = Cast<AFP_Character>(GetCurrentActorInfo()->OwnerActor)->FirstPersonCameraComponent.Get()->GetComponentRotation();

	// Spawns the Indicator
	ImpulseIndicator = Cast<AActor>(GetWorld()->SpawnActor(ImpulseClass, &SpawnLocation, &SpawnRotation, SpawnParameters));

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UGA_GravityMovement::OnGravPulse, .8, false);

}

/**
 * @name Jacob
 * @brief Ensures that the server has the correct camera rotation before moving the indicator
 */
void UGA_GravityMovement::SyncCamera()
{
	Cast<AFP_Character>(GetOwningActorFromActorInfo())->ClientCameraRotation();
	UAbilityTask_NetworkSyncPoint* NetworkSync = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::BothWait);
	NetworkSync->OnSync.AddDynamic(this, &UGA_GravityMovement::ThrowImpulse);
	NetworkSync->ReadyForActivation();
}


void UGA_GravityMovement::OnGravPulse()
{
	TArray<AActor*> OverlapActors;
	TArray<AActor*> IgnoredActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	if(UKismetSystemLibrary::SphereOverlapActors(this, ImpulseIndicator->GetActorLocation(),800.0f,ObjectTypes,AFP_Character::StaticClass(),IgnoredActors,OverlapActors ))
	{
		for(AActor* OverlapedActor : OverlapActors)
		{
			Cast<AFP_Character>(OverlapedActor)->LaunchCharacter((OverlapedActor->GetActorLocation()- ImpulseIndicator->GetActorLocation()).GetSafeNormal()*2500.0f, true, true);
		}
	}
	
	PlaySound(ImpulseIndicator->GetActorLocation());

	SpawnPulse();
	GetWorld()->DestroyActor(ImpulseIndicator);
	ImpulseIndicator= nullptr;
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
