// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AT_ServerWaitForClientTargetData.generated.h"

/**
 *  The server waits for TargetData from the CLient
 */
UCLASS()
class VERTICALSLICE_API UAT_ServerWaitForClientTargetData : public UAbilityTask
{
	GENERATED_BODY()

	// Object Initializer
	UAT_ServerWaitForClientTargetData(const FObjectInitializer& ObjectInitializer);
	// The ValidData Delegate
	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataDelegate ValidData;
	// The Static Callable function to wait for ClientTargetData
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static UAT_ServerWaitForClientTargetData* ServerWaitForClientTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, bool TriggerOnce);
	// The Activation of the AbilityTask
	virtual void Activate() override;

	// Callback for DataReplication
	UFUNCTION()
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);

protected:
	// The TaskDestroy function
	virtual void OnDestroy(bool AbilityEnded) override;
	bool bTriggerOnce = false;
};

