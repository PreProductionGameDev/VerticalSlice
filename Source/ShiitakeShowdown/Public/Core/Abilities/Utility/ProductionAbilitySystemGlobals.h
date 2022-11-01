// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ProductionAbilitySystemGlobals.generated.h"

/**
 *	Child Class of UAbilitySystemGlobals
 *	Required to stop a crash when using the Ability System Tags
 */
UCLASS()
class SHIITAKESHOWDOWN_API UProductionAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	UProductionAbilitySystemGlobals();

	// Returns the Existing GAS Global class
	static UProductionAbilitySystemGlobals& GSGet()
	{
		return dynamic_cast<UProductionAbilitySystemGlobals&>(Get());
	}

	/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
