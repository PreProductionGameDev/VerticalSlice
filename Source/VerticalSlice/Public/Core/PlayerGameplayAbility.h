// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data.h"
#include "Abilities/GameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UPlayerGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPlayerGameplayAbility();

	//Abilities will activate when input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EPlayerAbilityInputID AbilityInputID{EPlayerAbilityInputID::None};
	
};
