// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**	
 *		THE NEW PLAYER HUD CLASS (Now in C++!)
 *		Override all functions in blueprints for the UI
 */
UCLASS()
class VERTICALSLICE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// HEALTH FUNCTION
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthBarValues(float oldHealthValue,float newHealthValue);

	// ELEMENT ICONS
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SwapElementIcons(bool IsPrimaryAbility);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAbilityCooldown(FTimerHandle AbilityTimerHandle);
	
	// WEAPON FUNCTIONS
	// Set the Weapon Icon
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWeaponIcon(UTexture2D* NewTexture);
	// Set the Primary Clip Ammo
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPrimaryClipAmmo(int32 ClipAmmo);
	// Set the Primary Reserve Ammo
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPrimaryReserveAmmo(int32 ReserveAmmo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MakeHitMarker(bool PlayerKilled);
};
