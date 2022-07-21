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
	EPlayerAbilityInputID AbilityInputID = EPlayerAbilityInputID::None;

	// Value to associate an Ability with a slot without tying it to an automatically activated input.
	// EG Passive Abilities as they are not tied to an input.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EPlayerAbilityInputID AbilityID = EPlayerAbilityInputID::None;


	// Attempts to activate the given ability handle and batch all RPCs into one. This will only batch all RPCs that happen
	// in one frame. Best case scenario we batch ActivateAbility, SendTargetData, and EndAbility into one RPC instead of three.
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);

	// Same as calling K2_EndAbility. Meant for use with batching system to end the ability externally.
	virtual void ExternalEndAbility();

};
