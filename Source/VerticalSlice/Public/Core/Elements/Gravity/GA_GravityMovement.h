// 2022 ChronoOwl Studios
// Jacob

#pragma once

#include "CoreMinimal.h"
#include "ImpulseIndicator.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_GravityMovement.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGA_GravityMovement : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	// constructor
	UGA_GravityMovement();

	// Activation logic
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Ends the ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//finds a line in front of the player to move impulse point to
	UFUNCTION()
	void MoveImpulsePoint();

	//makes sure the Server has the same camera values as the player
	UFUNCTION()
	void SyncCamera();

	UFUNCTION()
	void OnKeyReleased(float TimePressed);

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundCue;

	// shows the casting player where the impulse will come from
	UPROPERTY()
	AImpulseIndicator* ImpulseIndicator;

	
	UPROPERTY()
	FTimerHandle ImpulseIndicatorTimerHandle;

	// Async timer that waits for activation key to be released
	UPROPERTY()
	UAbilityTask_WaitInputRelease* InputRelease;
	
};
