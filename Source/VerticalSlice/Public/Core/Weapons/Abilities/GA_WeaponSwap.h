// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_WeaponSwap.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGA_WeaponSwap : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_WeaponSwap();

	// Activation for the Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// End the Ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// Check if we can Activate the Ability
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	UFUNCTION()
	void SwapWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> EquipMontage;
};
