// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerGameplayAbility.h"

#include "Core/PlayerAbilitySystemComponent.h"

UPlayerGameplayAbility::UPlayerGameplayAbility()
{
}

bool UPlayerGameplayAbility::BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately)
{
	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (ASC)
	{
		return ASC->BatchRPCTryActivateAbility(InAbilityHandle, EndAbilityImmediately);
	}

	return false;
}

void UPlayerGameplayAbility::ExternalEndAbility()
{
	check(CurrentActorInfo);
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
