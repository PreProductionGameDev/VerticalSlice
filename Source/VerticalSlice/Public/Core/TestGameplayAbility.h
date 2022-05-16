// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data.h"
#include "Abilities/GameplayAbility.h"
#include "TestGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UTestGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTestGameplayAbility();

	//Abilities will activate when input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	ETestAbilityInputID AbilityInputID{ETestAbilityInputID::None};
	
};
