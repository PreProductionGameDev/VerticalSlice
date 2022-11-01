// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/Utility/ProductionAbilitySystemGlobals.h"
#include "Core/Abilities/GameplayEffects/GameplayEffectContext.h"

UProductionAbilitySystemGlobals::UProductionAbilitySystemGlobals()
{
	
}

FGameplayEffectContext* UProductionAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGEContext();
}
