// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEReloadExecutionCalc.generated.h"

/**
 *	Reload Calculation by taking in a Gameplay Effect data
 */
UCLASS()
class VERTICALSLICE_API UGEReloadExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEReloadExecutionCalc();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
