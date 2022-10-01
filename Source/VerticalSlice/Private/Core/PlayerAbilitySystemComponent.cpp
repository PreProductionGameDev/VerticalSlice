// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerAbilitySystemComponent.h"

#include "Core/PlayerGameplayAbility.h"

UPlayerAbilitySystemComponent::UPlayerAbilitySystemComponent()
{

}

FGameplayAbilitySpecHandle UPlayerAbilitySystemComponent::FindAbilitySpecHandleForClass(TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject)
{
	ABILITYLIST_SCOPE_LOCK()
	// Cycle through all ActivatableAbilities and locate specified SpecHandle. Return a new handle otherwise.
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		TSubclassOf<UGameplayAbility> SpecAbilityClass = Spec.Ability->GetClass();
		if (SpecAbilityClass == AbilityClass)
		{
			if (!OptionalSourceObject || (OptionalSourceObject && Spec.SourceObject == OptionalSourceObject))
			{
				return Spec.Handle;
			}
		}
	}

	return FGameplayAbilitySpecHandle();
}

bool UPlayerAbilitySystemComponent::BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately)
{
	bool AbilityActivated = false;
	if (InAbilityHandle.IsValid())
	{
		FScopedServerAbilityRPCBatcher GSAbilityRPCBatcher(this, InAbilityHandle);
		AbilityActivated = TryActivateAbility(InAbilityHandle, true);

		if (EndAbilityImmediately)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(InAbilityHandle);
			if (AbilitySpec)
			{
				UPlayerGameplayAbility* GASAbility = Cast<UPlayerGameplayAbility>(AbilitySpec->GetPrimaryInstance());
				GASAbility->ExternalEndAbility();
			}
		}
		return AbilityActivated;
	}
	
	return AbilityActivated;
}

const UTexture2D* UPlayerAbilitySystemComponent::GetLastDamagedIcon()
{
	if (LastDamagedBy)
	{
		return LastDamagedBy;
	}

	return nullptr;
}
