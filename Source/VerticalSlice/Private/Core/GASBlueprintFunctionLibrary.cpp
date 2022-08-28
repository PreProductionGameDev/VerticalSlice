// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GASBlueprintFunctionLibrary.h"

UPlayerGameplayAbility* UGASBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			return Cast<UPlayerGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}
	return nullptr;
}

UPlayerGameplayAbility* UGASBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass)
{
	if (AbilitySystemComponent)
	{
		if (FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(InAbilityClass))
		{
			return Cast<UPlayerGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}
	return nullptr;
}

bool UGASBlueprintFunctionLibrary::IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle)
{
	return Handle.IsValid();
}

FGameplayAbilityTargetDataHandle UGASBlueprintFunctionLibrary::EffectContextGetTargetData(FGameplayEffectContextHandle EffectContextHandle)
{
	if (FGEContext* EffectContext = static_cast<FGEContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetTargetData();
	}

	return FGameplayAbilityTargetDataHandle();
}

void UGASBlueprintFunctionLibrary::EffectContextAddTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (FGEContext* EffectContext = static_cast<FGEContext*>(EffectContextHandle.Get()))
	{
		EffectContext->AddTargetData(TargetData);
	}
}

void UGASBlueprintFunctionLibrary::ClearTargetData(FGameplayAbilityTargetDataHandle& TargetData)
{
	TargetData.Clear();
}
