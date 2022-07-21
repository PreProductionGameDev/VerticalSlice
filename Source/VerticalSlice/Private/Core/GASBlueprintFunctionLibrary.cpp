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

bool UGASBlueprintFunctionLibrary::IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle)
{
	return Handle.IsValid();
}
