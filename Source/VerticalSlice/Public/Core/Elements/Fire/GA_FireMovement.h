// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA_FireMovement.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGA_FireMovement : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	
	//constructor
	UGA_FireMovement();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	//Timer to control the fire spawning
	FTimerHandle SpawnTimer;

	UPROPERTY()
	UAbilityTask_WaitInputRelease* InputRelease;

	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* GameplayEvent;

	UFUNCTION()
	void SpawnFire();

	UFUNCTION()
	void StopSpawning(float TimeHeld);
};
