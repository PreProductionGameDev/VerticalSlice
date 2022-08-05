// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 *		THE NEW PLAYER HUD CLASS (Now in C++!)
 *		TODO: Add Health, ability functionality 
 */
UCLASS()
class VERTICALSLICE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// WEAPON FUNCTIONS
	// OVERRIDE IN BLUEPRINTS
	// Set the Primary Clip Ammo
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPrimaryClipAmmo(int32 ClipAmmo);
	// Set the Primary Reserve Ammo
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPrimaryReserveAmmo(int32 ReserveAmmo);
	
};
