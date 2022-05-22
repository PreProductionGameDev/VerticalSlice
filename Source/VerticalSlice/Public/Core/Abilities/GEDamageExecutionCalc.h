// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEDamageExecutionCalc.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGEDamageExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEDamageExecutionCalc();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;	
};
