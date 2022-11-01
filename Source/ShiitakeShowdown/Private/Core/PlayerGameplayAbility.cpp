// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerGameplayAbility.h"

#include "FP_Character.h"
#include "Core/PlayerAbilitySystemComponent.h"

UPlayerGameplayAbility::UPlayerGameplayAbility()
{
}

UObject* UPlayerGameplayAbility::K2_GetSourceObject(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const
{
	return GetSourceObject(Handle, &ActorInfo);
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

bool UPlayerGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (bSourceObjectMustBeCurrentWeaponToActivate)
	{
		AFP_Character* Player = Cast<AFP_Character>(ActorInfo->AvatarActor);
		if (Player && Player->GetCurrentWeapon() && Cast<UObject>(Player->GetCurrentWeapon()) == GetSourceObject(Handle, ActorInfo))
		{
			return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
		}
		return false;
	}

	if(bSourceObjectMustBeCurrentElementToActivate && !bSourceObjectMustBeCurrentWeaponToActivate)
	{
		AFP_Character* Player = Cast<AFP_Character>(ActorInfo->AvatarActor);
		if (!(Player && Player->ActiveElement() && Cast<UObject>(Player->ActiveElement()) == GetSourceObject(Handle, ActorInfo)))
		{
			return false;
		}
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UPlayerGameplayAbility::ExternalEndAbility()
{
	check(CurrentActorInfo);
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UPlayerGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) && GSCheckCost(Handle, *ActorInfo);
}

bool UPlayerGameplayAbility::GSCheckCost_Implementation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const
{
	return true;
}

void UPlayerGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	GSApplyCost(Handle, *ActorInfo, ActivationInfo);
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}
