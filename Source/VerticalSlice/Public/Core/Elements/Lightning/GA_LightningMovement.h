// 2022 ChronoOwl Studios
// Jacob

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GA_LightningMovement.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGA_LightningMovement : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_LightningMovement();

	// Starts the gameplay ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Rnds the gameplay ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Moves the indicator in front of the player
	UFUNCTION()
	void MoveIndicator();

	// Clears timers and telports player
	UFUNCTION()
	void KeyReleased(float TimePressed);

	// Blueprint function to move the player
	UFUNCTION(BlueprintImplementableEvent)
	void TeleportPlayer();

	// The cost of the ability
	UPROPERTY()
	float StaminaCost = 0.75f;

	// The distance of the teleport
	UPROPERTY()
	float TeleportDistance = 2500.0f;

	// The class to spawn the indicator
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> IndicatorClass;

	// Holds the indicator that the players see
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Indicator;

	// Timer to control the Indicator movement
	FTimerHandle IndicatorUpdateTimer;

	// Async timer that waits for activation key to be released
	UPROPERTY()
	UAbilityTask_WaitInputRelease* InputRelease;
};
