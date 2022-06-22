// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/PlayerGameplayAbility.h"
#include "Core/Weapons/BaseWeaponDataAsset.h"

#include "BaseWeapon.generated.h"

class AFP_Character;

class USkeletalMeshComponent;

UCLASS()
class VERTICALSLICE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UPlayerGameplayAbility>> GameplayAbilities;

	UPROPERTY(VisibleAnywhere, Category = "Abilites")
	TArray<FGameplayAbilitySpecHandle> GameplayAbilityHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBaseWeaponDataAsset* Data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* FP_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* TP_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pos")
	UBillboardComponent* TPHelper;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Ammo Pool for the Weapon
	UPROPERTY(EditAnywhere)
	int CurrentAmmoPool;


public:	
	UFUNCTION(BlueprintCallable)
	virtual void OnEquip();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void OnUnequip();

	virtual void OnUnequip_Implementation();

	// Checks if the User can pickup ammo (I.E. Not at max ammo)
	UFUNCTION(BlueprintCallable)
	bool CanPickupAmmo();
	
	// Adds the specified Ammo ammount
	// Clamps to Max Ammo Specified in Data asset.
	UFUNCTION(BlueprintCallable)
	void AddAmmo(int ammoPickup);

	// Decrease Ammo per for a shot
	UFUNCTION(BlueprintCallable)
	void RemoveAmmo(int AmmoToRemove);

	// Gets the Ammo count
	UFUNCTION(BlueprintCallable)
	int GetAmmo();
	
	// Checks if the Weapon can shoot
	UFUNCTION(BlueprintCallable)
	bool CanShoot();
};
