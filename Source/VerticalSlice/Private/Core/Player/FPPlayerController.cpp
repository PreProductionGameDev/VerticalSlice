// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Player/FPPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Core/UI/PlayerHUD.h"
#include "Core/Abilities/AttributeSets/AmmoAttributeSet.h"

void AFPPlayerController::CreateHUD()
{
	// Only need to create the HUD once
	if (UIHUDWidget)
	{
		return;
	}
	// If there is no HUD to create, dont try to create one
	if (!UIHUDWidgetClass)
	{
		return;
	}
	// Only create HUD for Local Player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Create the HUD and apply to Player Controller
	UIHUDWidget = CreateWidget<UPlayerHUD>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();

	// TODO: Set Default Attributes for Health once setup

	// Get the Ammo Values from the Player
	AFP_Character* CurrentPlayer = GetPawn<AFP_Character>();
	if (CurrentPlayer)
	{
		ABWeapon* CurrentWeapon = CurrentPlayer->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			// Set the Primary and Clip ammo onto the HUD
			UIHUDWidget->SetPrimaryClipAmmo(CurrentPlayer->GetPrimaryClipAmmo());
			UIHUDWidget->SetPrimaryReserveAmmo(CurrentPlayer->GetPrimaryReserveAmmo());
		}
	}
	
}

UPlayerHUD* AFPPlayerController::GetPlayerHUD()
{
	return UIHUDWidget;
}

void AFPPlayerController::SetPrimaryClipAmmo(int32 ClipAmmo)
{
	if (UIHUDWidget)
	{
		UIHUDWidget->SetPrimaryClipAmmo(ClipAmmo);
	}
}

void AFPPlayerController::SetPrimaryReserveAmmo(int32 ReserveAmmo)
{
	if (UIHUDWidget)
	{
		UIHUDWidget->SetPrimaryReserveAmmo(ReserveAmmo);
	}
}

void AFPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AFPPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For Minor Edge Cases on PlayerState is Rep before possession
	CreateHUD();
}
