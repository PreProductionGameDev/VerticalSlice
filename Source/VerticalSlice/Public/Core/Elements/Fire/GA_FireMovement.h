// 2022 ChronoOwl Studios
// Jacob

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GA_FireMovement.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGA_FireMovement : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	
	// constructor
	UGA_FireMovement();

	// Logic to do on activation
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Logic to perform on ability end
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	// Timer to control the fire spawning
	FTimerHandle SpawnTimer;

	// Async timer that waits for activation key to be released
	UPROPERTY()
	UAbilityTask_WaitInputRelease* InputRelease;

	// Spawns fire actor
	UFUNCTION()
	void SpawnFire();

	// stops the spawn timer
	UFUNCTION()
	void StopSpawning(float TimeHeld);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* Sound;
};
