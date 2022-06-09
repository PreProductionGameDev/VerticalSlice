// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseWeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VERTICALSLICE_API UBaseWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxAmmo = 999;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartingAmmo = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* WeaponAnim;
};
