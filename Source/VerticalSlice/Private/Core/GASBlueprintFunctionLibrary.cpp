// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GASBlueprintFunctionLibrary.h"

UPlayerGameplayAbility* UGASBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is valid"));
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			UE_LOG(LogTemp, Error, TEXT("AbilitySpec Valid"));
			return Cast<UPlayerGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Error Ability Instance not found"));
	return nullptr;
}

bool UGASBlueprintFunctionLibrary::IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle)
{
	return Handle.IsValid();
}
