// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FP_Character.h"
#include "FPPlayerController.generated.h"

/**
 *		The Player Controller class
 *		Currently just used for UI
 *		Player Controllers are persistent meaning we dont need to try to recreate the HUD
 */
UCLASS()
class VERTICALSLICE_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Create the HUD
	void CreateHUD();
	// Return the Created HUD
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|UI")
	class UPlayerHUD* GetPlayerHUD() const;

	/*
	 *	AMMO AND WEAPON FUNCTIONS
	 */
	// Set the Primary Clip Ammo Values for the HUD
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|UI")
	void SetPrimaryClipAmmo(int32 ClipAmmo);
	// Set the Primary Reserve Ammo Values for the HUD
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShodown|UI")
	void SetPrimaryReserveAmmo(int32 ReserveAmmo);

protected:
	// The Widget Class set in the Player Controller to be created
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShiitakeShowdown|UI")
	TSubclassOf<class UPlayerHUD> UIHUDWidgetClass;
	// The Pointer for the Widget Object created
	UPROPERTY(BlueprintReadWrite, Category = "ShiitakeShowdown|UI")
	class UPlayerHUD* UIHUDWidget;

	// Server Only
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
	
};
