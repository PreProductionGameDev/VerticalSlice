// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Weapons/BaseWeaponDataAsset.h"
#include "HitscanWeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VERTICALSLICE_API UHitscanWeaponDataAsset : public UBaseWeaponDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxRange;

	
};
