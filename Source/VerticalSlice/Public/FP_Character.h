// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Core/Weapons/BaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Core/Weapons/BWeapon.h"


#include "FP_Character.generated.h"

class UPlayerGameplayAbility;
class UPlayerAbilitySystemComponent;
class UPlayerAttributeSet;
class UAmmoAttributeSet;

UCLASS()
class VERTICALSLICE_API AFP_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true", DisplayName = "FPCamera"));
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, Replicated)
	TObjectPtr<ABaseWeapon> EquipedGun;
	
public:
	// Sets default values for this character's properties
	AFP_Character();

	// Setup Values for Network Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Return CurrentHealth from AttributeSet
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();
	// Return MaxHealth from AttributeSet
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth();

	// OLD WEAPON GETTER
	UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction = true))
	ABaseWeapon* GetEquippedGun();

	// Set the Camera Rotation for the networked Players
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientCameraRotation();
	// Sets the Camera Rotation for the networked player 
	UFUNCTION(Server, Reliable)
	void ServerCameraRotation(FTransform Transform);

	// Return the FirstPerson Mesh
	UFUNCTION()
	USkeletalMeshComponent* GetFirstPersonMesh();
	// Return the ThirdPerson Mesh
	UFUNCTION()
	USkeletalMeshComponent* GetThirdPersonMesh();
	
	// Returns the AbilitySystemComponent
	UFUNCTION(BlueprintCallable)
	UPlayerAbilitySystemComponent* GetPlayerAbilitySystemComponent();

	// Returns the Alive State of the Player
	virtual bool IsAlive();

	//clears the viewport if the client is owned by local controller
	UFUNCTION(BlueprintCallable)
	bool ClientClearWidgets();
	
	/*
	 *	NEW WEAPON SYSTEM
	 *	Improved the level of GAS Orientated Design
	 */
	
	// Returns the Position to Attach the Weapon. Not Setup Currently
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	FName GetWeaponAttachPoint();
	// Get the Current Equipped Weapon
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	ABWeapon* GetCurrentWeapon() const;

	// Adds a new weapon to the inventory.
	// Will return false if weapon already exists in inventory
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	bool AddWeaponToInventory(ABWeapon* NewWeapon, bool bEquipWeapon = false);
	// Remove Weapon from Inventory
	// Returns False if the weapon already exists in the inventory, True if its a New Weapon
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	bool RemoveWeaponFromInventory(ABWeapon* WeaponToRemove);
	// Clear all Weapons from the Player's Inventory
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	void RemoveAllWeaponsFromInventory();
	// Equip a Weapon
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	void EquipWeapon(ABWeapon* NewWeapon);

	// Server functionality for Equipping the Weapon
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(ABWeapon* NewWeapon);
	void ServerEquipWeapon_Implementation(ABWeapon* NewWeapon);
	bool ServerEquipWeapon_Validate(ABWeapon* NewWeapon);

	/*
	 * TODO: Cycling Between Weapons requires additional logic
	 * Have not been setup yet.
	 */

	// Return the Equipped Weapons PrimaryClip Ammo
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	int32 GetPrimaryClipAmmo() const;
	// Return the Equipped Weapons Max PrimaryClip Ammo
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	int32 GetMaxPrimaryClipAmmo() const;
	// Returns the Equipped Weapons Reserve Ammo from the Attribute Set
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	int32 GetPrimaryReserveAmmo() const;

	// OLD WEAPON PLEASE REMOVE
	UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction = true))
	void EquipGun(ABaseWeapon* const NewGun);
	
	
protected:
	// The Visible FirstPerson Mesh
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Occurs after the Initialization of Components
	virtual void PostInitializeComponents() override;

	// Ability System functions
	// Class Overrides
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Returns the Ability System Component
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Adds all the Gameplay Abilities Pre-Set on the player
	void AddStartupGameplayAbilities();

	// Passive Gameplay Affects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;
	// All Active Gameplay Abilities
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UPlayerGameplayAbility>> GameplayAbilities;
	UPROPERTY()
	uint8 bAbilitiesInitialized:1;

	// On Damage of Player
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AFP_Character* InstigatorCharacter, AActor* DamageCauser );
	// On the Change of the players Health Value
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	// On Player Death
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();
	// Checks if Player has Died and Call OnDeath()
	virtual void DeathCheck();

	// Handles the Damage
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AFP_Character* InstigatorCharacter, AActor* DamageCauser);
	// Handles the Health Changed
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	friend UPlayerAttributeSet;
	// The GAS Component for the Player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerAbilitySystemComponent> AbilitySystemComponent;
	// Players Health Attribute Set
	UPROPERTY()
	TObjectPtr<UPlayerAttributeSet> Attributes;
	// Players Ammo Attribute Set
	TObjectPtr<UAmmoAttributeSet> AmmoAttributes;
	
	/*
	 *	NEW WEAPON SYSTEM
	 *	Improved the level of GAS Orientated Design
	 */
	// Default Weapons for the player upon spawn. Can move into the game mode upon further development
	// These are the Weapons Equipped upon loading a player.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ShiitakeShowdown|Weapon")
	TArray<TSubclassOf<ABWeapon>> DefaultInventoryWeapons;
	// The Players Weapons Inventory.
	// Access each weapon by its Gameplay Tag
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ShiitakeShowdown|Weapon")
	TMap<FGameplayTag, ABWeapon*> WeaponInventory;
	// The Currently Equipped Weapon
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	ABWeapon* CurrentWeapon = nullptr;
	// The Location to Attach the weapon to
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName WeaponAttachPoint;

	// Returns true if the weapon exists in the inventory.
	bool DoesWeaponExistInInventory(const ABWeapon* InWeapon) const;
	// Swap Between weapons
	void SetCurrentWeapon(ABWeapon* NewWeapon, ABWeapon* LastWeapon);
	// UnEquip specified weapon.
	void UnEquipWeapon(ABWeapon* WeaponToUnEquip);
	// UnEquips the current Weapon. Used for dropping.
	void UnEquipCurrentWeapon();
	// TODO: ADD IMPLEMENTATION UPON ADDING THE AMMO SYSTEM 
	UFUNCTION()
	virtual void CurrentWeaponPrimaryClipAmmoChanged(int32 OldPrimaryClipAmmo, int32 NewPrimaryClipAmmo);

	// Ensures the Replication of the CurrentWeapon
	UFUNCTION()
	void OnRep_CurrentWeapon(ABWeapon* LastWeapon);
	// Spawns the Default Inventory set in the Blueprint.
	// Need to abstract the default inventory to be part of the GameMode
	void SpawnDefaultInventory();
	
	// The CurrentWeapon is only automatically replicated to simulated clients.
	// The autonomous client can use this to request the proper CurrentWeapon from the server when it knows it may be
	// out of sync with it from predictive client-side changes.
	UFUNCTION(Server, Reliable)
	void ServerSyncCurrentWeapon();
	void ServerSyncCurrentWeapon_Implementation();
	bool ServerSyncCurrentWeapon_Validate();

	// The CurrentWeapon is only automatically replicated to simulated clients.
	// Use this function to manually sync the autonomous client's CurrentWeapon when we're ready to.
	// This allows us to predict weapon changes (changing weapons fast multiple times in a row so that the server doesn't
	// replicate and clobber our CurrentWeapon).
	UFUNCTION(Client, Reliable)
	void ClientSyncCurrentWeapon(ABWeapon* InWeapon);
	void ClientSyncCurrentWeapon_Implementation(ABWeapon* InWeapon);
	bool ClientSyncCurrentWeapon_Validate(ABWeapon* InWeapon);

	// Attribute changed callbacks
	virtual void CurrentWeaponPrimaryReserveAmmoChanged(const FOnAttributeChangeData& Data);
	
	// Set to true when we change the weapon predicatively and flip it to false when the Server replicates to confirm.
	// We use this if the Server refused a weapon change ability's activation to ask the Server to sync the client back up
	// with the correct CurrentWeapon.
	bool bChangedWeaponLocally;
	// Attribute changed delegate handles
	FDelegateHandle PrimaryReserveAmmoChangedDelegateHandle;
	// Cached Weapon Tags
	FGameplayTag CurrentWeaponTag;
	FGameplayTag NoWeaponTag;
	FGameplayTag WeaponAbilityTag;

private:
	// Movement Functions
	void MoveForward(float value);
	void MoveRight(float value);
};
