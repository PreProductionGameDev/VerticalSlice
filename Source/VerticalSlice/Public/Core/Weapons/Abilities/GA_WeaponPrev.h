// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_WeaponPrev.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGA_WeaponPrev : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_WeaponPrev();
	
	// Activation for the Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// End the Ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// Check if we can Activate the Ability
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	UFUNCTION()
void SwapWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void WeaponEquipped(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
};
