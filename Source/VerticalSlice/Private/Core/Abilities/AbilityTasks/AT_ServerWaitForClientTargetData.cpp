// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/AbilityTasks/AT_ServerWaitForClientTargetData.h"
#include "AbilitySystemComponent.h"

UAT_ServerWaitForClientTargetData::UAT_ServerWaitForClientTargetData(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UAT_ServerWaitForClientTargetData* UAT_ServerWaitForClientTargetData::ServerWaitForClientTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, bool TriggerOnce)
{
	// Creates a new AbilityTask
	UAT_ServerWaitForClientTargetData* MyObj = NewAbilityTask<UAT_ServerWaitForClientTargetData>(OwningAbility, TaskInstanceName);
	MyObj->bTriggerOnce = TriggerOnce;
	return MyObj;
}

void UAT_ServerWaitForClientTargetData::Activate()
{
	if (!Ability || !Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		return;
	}
	// Creates the SpecHandle and PredictionKey and Applies Delegates.
	FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
	FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
	AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UAT_ServerWaitForClientTargetData::OnTargetDataReplicatedCallback);
}

void UAT_ServerWaitForClientTargetData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag)
{
	// Stores Data in a Mutable form
	FGameplayAbilityTargetDataHandle MutableData = Data;
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	// Broadcast the Valid Data
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(MutableData);
	}
	// End Task if only to be activated once
	if (bTriggerOnce)
	{
		EndTask();
	}
}

void UAT_ServerWaitForClientTargetData::OnDestroy(bool AbilityEnded)
{
	// Remove the AbilityTargetData Delegate from the ASC
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).RemoveAll(this);
	}
	Super::OnDestroy(AbilityEnded);
}


