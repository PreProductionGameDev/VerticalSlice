// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
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
	int Damage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxRange = 6000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HorizontalWeaponSpread = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VerticalWeaponSpread = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumberOfShots = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* shotSound;
};
