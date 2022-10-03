// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Core/Weapons/Abilities/GA_WeaponPrimary.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_WeaponPrimaryBurst.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGA_WeaponPrimaryBurst : public UGA_WeaponPrimary
{
	GENERATED_BODY()

public:
	UGA_WeaponPrimaryBurst();

	// Starts the Gameplay Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// Ends the Gameplay Ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// Check the Cost before application
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	// The Firing Loop to Delay between each shot
	virtual void FireLoop() override;
	// Firing of the Burst shots
	UFUNCTION()
	virtual void FireBurst();
	// The instant Shot Fired function
	virtual void FireShot() override;
	// Wait until we can cancel
	UFUNCTION()
	virtual void WaitForValidCancel(float TimePressed);
	// End the abilities
	UFUNCTION()
	void EndFiring();
	// Wait until the next Valid Shot
	virtual void WaitForValidShot() override;

protected:
	// The Time between each burst shot
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TimeBetweenBurstShots;
	// The Number of shots in a burst
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int BurstShotCount = 1;
	// The current burst amount
	int BurstShotCurrent;
	
};
