// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GameplayTagContainer.h"
#include "AT_WaitTargetDataUsingActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitTargetDataUsingActorDelegate, const FGameplayAbilityTargetDataHandle&, Data);


/**
 *	Wait for TargetData from an existing TargetActor and will not destroy upon recieving data
 */
UCLASS()
class VERTICALSLICE_API UAT_WaitTargetDataUsingActor : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataUsingActorDelegate ValidData;

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataUsingActorDelegate Cancelled;

	/**
	* Uses specified spawned TargetActor and waits for it to return valid data or to be canceled. The TargetActor is *NOT* destroyed.
	*
	* @param OwningAbility The Ability which is using this function
	* @param TaskInstanceName The Name set for the task to make it easy to aqcuire again
	* @param ConfirmationType The Enum for Confirmation
	* @param InTargetActor The GAS AbilityTargetActor being passed in to wait for Valid data from 
	* @param bCreateKeyIfNotValidForMorePredicting Will create a new scoped prediction key if the current scoped prediction key is not valid for more predicting.
	* If false, it will always create a new scoped prediction key. We would want to set this to true if we want to use a potentially existing valid scoped prediction
	* key like the ability's activation key in a batched ability.
	*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static UAT_WaitTargetDataUsingActor* WaitTargetDataWithReusableActor(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName, 
		TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType,
		AGameplayAbilityTargetActor* InTargetActor,
		bool bCreateKeyIfNotValidForMorePredicting = false);

	// The Activation of this AbilityTask
	virtual void Activate() override;

	// Replication Callbacks
	UFUNCTION()
	virtual void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);
	UFUNCTION()
	virtual void OnTargetDataReplicatedCancelledCallback();
	UFUNCTION()
	virtual void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& Data);
	UFUNCTION()
	virtual void OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& Data);

	// Called when the ability is asked to confirm from an outside node. What his means depends on the individual tas. By Default, this does nothing other than ending if bEndTask is true
	virtual void ExternalConfirm(bool bEndTask) override;
	// Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By Default this does nothing other than ending the task
	virtual void ExternalCancel() override;

protected:
	// AbilityTargetActor Cached
	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor;
	// Whether to Create a new Key for More predicting
	bool bCreateKeyIfNotValidForMorePredicting;
	// Cached Confirmation Type 
	TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType;
	// The Delegate Handle for Data Replication
	FDelegateHandle OnTargetDataReplicatedCallbackDelegateHandle;

	// Initialize the Target actor on load
	virtual void InitializeTargetActor() const;
	virtual void FinalizeTargetActor() const;
	// Register the Callbacks
	virtual void RegisterTargetDataCallbacks();
	// On Destroy of this ability
	virtual void OnDestroy(bool AbilityEnded) override;
	// Returns if this Data is replicated to the server
	virtual bool ShouldReplicateDataToServer() const;
	
};
