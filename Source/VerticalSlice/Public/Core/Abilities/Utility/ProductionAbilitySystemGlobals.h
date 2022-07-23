// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ProductionAbilitySystemGlobals.generated.h"

/**
 *	Child Class of UAbilitySystemGlobals
 *	Needed to stop a crash
 */
UCLASS()
class VERTICALSLICE_API UProductionAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	UProductionAbilitySystemGlobals();
	
	static UProductionAbilitySystemGlobals& GSGet()
	{
		return dynamic_cast<UProductionAbilitySystemGlobals&>(Get());
	}
};
