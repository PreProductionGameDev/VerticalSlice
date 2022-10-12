// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_WeaponEquip.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGA_WeaponEquip : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_WeaponEquip();

	// Activation for Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// End the Ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void MontageNotifyEvent(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> EquipMontage;

	FName SectionName = FName("Equip");
};
