// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseWeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UBaseWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate;
};
