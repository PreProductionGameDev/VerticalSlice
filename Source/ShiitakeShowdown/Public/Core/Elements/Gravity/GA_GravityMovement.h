// 2022 ChronoOwl Studios
// Jacob

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_GravityMovement.generated.h"

/**
 * 
 */
UCLASS()
class SHIITAKESHOWDOWN_API UGA_GravityMovement : public UPlayerGameplayAbility
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
	void ThrowImpulse();

	//makes sure the Server has the same camera values as the player
	UFUNCTION()
	void SyncCamera();


	UFUNCTION()
	void OnGravPulse();

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundCue;

	// shows the casting player where the impulse will come from
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* ImpulseIndicator;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ImpulseClass;
	
	UPROPERTY()
	FTimerHandle ImpulseIndicatorTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraSystem* NiagaraSystem;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnPulse();

	
};
