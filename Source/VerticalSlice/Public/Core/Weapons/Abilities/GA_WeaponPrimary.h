// 2022 ChronoOwl Studios
// Stefan Petrie

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_WeaponPrimary.generated.h"

/**
 *	The Primary Firing Ability. 
 *	All Locally ran to find time between each shot.
 *	Calls the Instant Ability to fire a bullet replicated.
 */
UCLASS()
class VERTICALSLICE_API UGA_WeaponPrimary : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_WeaponPrimary();

	// Starts the Gameplay Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// Ends the Gameplay Ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// Check the Cost before application
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	// The Firing Loop to Delay between each shot
	UFUNCTION()
	void FireLoop();
	// The instant Shot Fired function
	UFUNCTION()
	void FireShot();
	// When Firing gets cancelled
	UFUNCTION()
	void FiringCancelled(float TimePressed);
	// Wait until the next Valid Shot
	UFUNCTION()
	void WaitForValidShot();
	
	// Recoil Time
	float RecoilTime = 0.0f;
	// The Frame before the recoil time
	float StepBeforeRecoilTime = 0.0f;
protected:
	// The Source Weapon Class
	UPROPERTY()
	class ABWeapon* SourceWeapon;
	// The time between each shot. Obtained from Source Weapon
	float TimeBetweenShots;
	// The Instant Ability class. Set in a blueprint instance
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayAbility> InstantAbilityClass;
	// The Instant Ability. Obtained at Runtime
	UPROPERTY()
	class UGA_WeaponPrimaryInstant* InstantAbility;
	// The Instant Ability Handle
	FGameplayAbilitySpecHandle InstantAbilityHandle;

	// Async timer that waits for activation key to be released
	UPROPERTY()
	class UAbilityTask_WaitInputRelease* InputRelease;

	// Async time that waits for Activation key to be released. Required for additional functionality
	UPROPERTY()
	UAbilityTask_WaitInputRelease* InputReleaseSecondary;

	// Setup and Store all the variables
	void SetupCacheables();
	// Reset Recoil time to 0
	UFUNCTION()
	void ResetRecoilTime();
	// Check the recoil state
	UFUNCTION()
	void CheckRecoilStopped();
	FTimerHandle RecoilResetHandle;
	
};
