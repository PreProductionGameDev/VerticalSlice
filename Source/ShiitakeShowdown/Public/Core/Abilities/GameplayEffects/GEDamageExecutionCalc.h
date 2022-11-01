// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEDamageExecutionCalc.generated.h"

/**
 *	A Gameplay Effect Calculation class used for Calculating the amount of Damage to apply to the player
 */
UCLASS()
class SHIITAKESHOWDOWN_API UGEDamageExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEDamageExecutionCalc();

	// Calculates exactly how much damage the player should take, and applies that affect
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;	
};
