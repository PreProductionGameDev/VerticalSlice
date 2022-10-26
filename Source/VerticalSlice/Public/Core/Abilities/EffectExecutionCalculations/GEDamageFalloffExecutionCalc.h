// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEDamageFalloffExecutionCalc.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGEDamageFalloffExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEDamageFalloffExecutionCalc();

	// Calculate Exactly how much damage the player should take, and apply the effect 
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
