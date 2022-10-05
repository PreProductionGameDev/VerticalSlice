// 2022 ChronoOwl Studios

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

	// ELEMENT FUNCTIONS
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SwapElementIcons(bool IsPrimaryAbility);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAbilityCooldown(FTimerHandle AbilityTimerHandle);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetElementIcons(UTexture2D* ElementOne, UTexture2D* ElementTwo);
	
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
	// Hit Markers
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MakeHitMarker(bool PlayerKilled);
	// CrossHair Toggle
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleCrosshair(bool isVisible);
	

	// Kill Feed
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddKillFeedMessage(const FString& KillerName, const FString& VictimName, UTexture2D* KillIcon);
};
