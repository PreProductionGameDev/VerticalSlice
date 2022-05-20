// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Weapons/BaseWeapon.h"
#include "Core/Weapons/BaseWeaponDataAsset.h"

#include "BaseGun.generated.h"

UCLASS()
class VERTICALSLICE_API ABaseGun : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGun();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBaseWeaponDataAsset* Data;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Weapon Firing Functions
	// Used for Constant Firing Weapons
	// Start Firing
	UFUNCTION(BlueprintCallable)
	virtual void BeginPrimary() override;
	// Single Fire
	UFUNCTION(BlueprintCallable)
	virtual void Primary() override;
	// End the Firing
	UFUNCTION(BlueprintCallable)
	virtual void EndPrimary() override;


	// Weapon Secondary Functions
	// Used for Scopes and Alternate Fires
	// Begin Secondary
	UFUNCTION(BlueprintCallable)
	virtual void BeginSecondary() override;
	UFUNCTION(BlueprintCallable)
	virtual void Secondary() override;
	// End Secondary
	UFUNCTION(BlueprintCallable)
	virtual void EndSecondary() override;


	// Functions for Correct Shot Functionality
	// Set the Boolean value of CanShoot
	UFUNCTION(BlueprintCallable)
	void SetCanShoot(bool bShoot);
	// Shot Functionality
	void FirstShot();
	void ResetShot();
protected:
	// Timers for Shot Functionality
	// Rapid Fire Shooting
	FTimerHandle TimerHandle_HandleRefire;
	// Delay for First Shot
	FTimerHandle TimerHandle_DelayedShot;
	// Delay for being able to reshoot
	FTimerHandle TimerHandle_CanShoot;

	bool bCanShoot = true;
};
