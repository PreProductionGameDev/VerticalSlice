// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Core/Data.h"
#include "Core/PlayerAbilitySystemComponent.h"
#include "Core/PlayerGameplayAbility.h"
#include "BWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponAmmoChangedDelegate, int32, OldValue, int32, NewValue);

class AFP_Character;
class APlayerAbilitySystemComponent;

UCLASS(Blueprintable, BlueprintType)
class VERTICALSLICE_API ABWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABWeapon();

	// Whether or not to spawn this weapon with collision enabled (Pickup Mode)
	// Set to false when spawning directly into a player's inventory or true when spawning into the world in pickup mode.
	UPROPERTY(BlueprintReadWrite)
	bool bSpawnWithCollision;

	// Tag used for the FP Anim blueprint
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FGameplayTag WeaponTag;

	// Tags used to limit pickups. Would be Death or Spectator
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FGameplayTagContainer RestrictedPickupTags;

	// Tag used for Firing Mode
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "ShiitakeShowdown|GASWeapon")
	FGameplayTag FireMode;

	// Primary Ammo Type
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FGameplayTag PrimaryAmmoType;

	UPROPERTY(BlueprintAssignable, Category = "ShiitakeShowdown|GASWeapon")
	FWeaponAmmoChangedDelegate OnPrimaryClipAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "ShiitakeShowdown|GASWeapon")
	FWeaponAmmoChangedDelegate OnMaxPrimaryClipAmmoChanged;

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Return First Person Mesh
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ShiitakeShowdown|GASWeapon")
	virtual USkeletalMeshComponent* GetWeaponMesh1P() const;

	// Return Third Person Mesh
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ShiitakeShowdown|GASWeapon")
	virtual USkeletalMeshComponent* GetWeaponMesh3P() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

	void SetOwningCharacter(AFP_Character* InOwningCharacter);

	// Pickup on Touch
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called when Player equips this weapon
	virtual void Equip();

	// Called when Player UnEquips this weapon
	virtual void UnEquip();

	// Adding the GAS Abilities
	virtual void AddAbilities();
	// Removing the GAS Abilities
	virtual void RemoveAbilities();

	virtual int32 GetAbilityLevel(EPlayerAbilityInputID AbilityID);

	// Resets things like firing mode
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual void ResetWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void OnDropped(FVector NewLocation);
	virtual void OnDropped_Implementation(FVector NewLocation);
	virtual bool OnDropped_Validate(FVector NewLocation);

	// Primary Ammo Functions
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual int32 GetPrimaryClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual int32 GetMaxPrimaryClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual void SetPrimaryClipAmmo(int32 NewPrimaryClipAmmo);

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual void SetMaxPrimaryClipAmmo(int32 NewMaxPrimaryClipAmmo);

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual bool HasInfiniteAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Animation")
	UAnimMontage* GetEquip1PMontage() const;

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Animation")
	UAnimMontage* GetEquip3PMontage() const;

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Audio")
	class USoundCue* GetPickupSound() const;

protected:
	UPROPERTY()
	UPlayerAbilitySystemComponent* AbilitySystemComponent;

	// Default Ammo in the Clip
	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_PrimaryClipAmmo, Category = "ShiitakeShowdown|GASWeapon|Ammo")
	int32 PrimaryClipAmmo;

	// Max Clip Size
	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_MaxPrimaryClipAmmo, Category = "ShiitakeShowdown|GASWeapon|Ammo")
	int32 MaxPrimaryClipAmmo;

	// Infinite Ammo for Specific GameModes or Testing
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon|Ammo")
	bool bInfiniteAmmo;

	// Collision capsule for when weapon is in pickup mode
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CollisionComp;
	
	UPROPERTY(VisibleAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	USkeletalMeshComponent* WeaponMesh1P;

	UPROPERTY(VisibleAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	USkeletalMeshComponent* WeaponMesh3P;

	// Relative Location of weapon 3P Mesh when in pickup mode
	// 1P weapon mesh is invisible so it doesn't need one
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FVector WeaponMesh3PickupRelativeLocation;

	// Relative Location of weapon 1P Mesh when equipped
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FVector WeaponMesh1PEquippedRelativeLocation;

	// Relative Location of weapon 3P Mesh when equipped
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FVector WeaponMesh3PEquippedRelativeLocation;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ShiitakeShowdown|GASWeapon")
	AFP_Character* OwningCharacter;

	UPROPERTY(EditAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	TArray<TSubclassOf<UPlayerGameplayAbility>> Abilities;

	UPROPERTY(BlueprintReadOnly, Category = "ShiitakeShowdown|GASWeapon")
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	FGameplayTag DefaultFireMode;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	UAnimMontage* Equip1PMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	UAnimMontage* Equip3PMontage;

	// Sound Played when Picked up
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|Audio")
	class USoundCue* PickupSound;

	// Cache Tags
	FGameplayTag WeaponPrimaryInstantAbilityTag;
	FGameplayTag WeaponSecondaryInstantAbilityTag;
	FGameplayTag WeaponIsFiringTag;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;


	// Called when Player Picks up the weapon
	virtual void PickUpOnTouch(AFP_Character* InCharacter);

	UFUNCTION()
	virtual void OnRep_PrimaryClipAmmo(int32 OldPrimaryClipAmmo);

	UFUNCTION()
	virtual void OnRep_MaxPrimaryClipAmmo(int32 OldMaxPrimaryClipAmmo);
};
