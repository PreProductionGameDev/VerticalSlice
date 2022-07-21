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
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	ABaseWeapon* GetEquippedGun();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ClientCameraRotation();
	
	UFUNCTION(Server, Reliable)
	void ServerCameraRotation(FTransform Transform);
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* ThirdPersonMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Ability System functions
	//Class Overrides
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void AddStartupGameplayAbilities();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UPlayerGameplayAbility>> GameplayAbilities;

	UPROPERTY()
	uint8 bAbilitiesInitialized:1;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AFP_Character* InstigatorCharacter, AActor* DamageCauser );

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	virtual void DeathCheck();

	
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AFP_Character* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	friend UPlayerAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerAbilitySystemComponent> AbilitySystemComponent;


	UPROPERTY()
	TObjectPtr<UPlayerAttributeSet> Attributes;

	// NEW WEAPON SYSTEMS
	// Default Weapons for the player upon spawn. Can move into the game mode upon further development
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ShiitakeShowdown|Weapon")
	TArray<TSubclassOf<ABWeapon>> DefaultInventoryWeapons;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ShiitakeShowdown|Weapon")
	TMap<FGameplayTag, ABWeapon*> WeaponInventory;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	ABWeapon* CurrentWeapon;

	bool DoesWeaponExistInInventory(const ABWeapon* InWeapon) const;

	void SetCurrentWeapon(ABWeapon* NewWeapon, ABWeapon* LastWeapon);
	
	// UnEquip specified weapon.
	void UnEquipWeapon(ABWeapon* WeaponToUnEquip);
	
	// UnEquips the current Weapon. Used for dropping.
	void UnEquipCurrentWeapon();

	UFUNCTION()
	virtual void CurrentWeaponPrimaryClipAmmoChanged(int32 OldPrimaryClipAmmo, int32 NewPrimaryClipAmmo);
	
	UFUNCTION()
	void OnRep_CurrentWeapon(ABWeapon* LastWeapon);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName WeaponAttachPoint;

	// Cached Tags
	FGameplayTag CurrentWeaponTag;
	FGameplayTag NoWeaponTag;
	FGameplayTag WeaponAbilityTag;
public:

	UFUNCTION()
	USkeletalMeshComponent* GetFirstPersonMesh();

	UFUNCTION()
	USkeletalMeshComponent* GetThirdPersonMesh();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	UPlayerAbilitySystemComponent* GetAbilitySystemComponent();

	virtual bool IsAlive();

	// NEW WEAPON SYSTEM

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	FName GetWeaponAttachPoint();
	
	// Get the Current Weapon
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

	// Clear all Weapons from the Player
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	void RemoveAllWeaponsFromInventory();

	// Equip a Weapon
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	void EquipWeapon(ABWeapon* NewWeapon);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(ABWeapon* NewWeapon);
	void ServerEquipWeapon_Implementation(ABWeapon* NewWeapon);
	bool ServerEquipWeapon_Validate(ABWeapon* NewWeapon);

	/*
	 * Cycling Between Weapons requires additional logic
	 */

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	int32 GetPrimaryClipAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapons")
	int32 GetMaxPrimaryClipAmmo() const;


	
private:
	// Movement Values
	void MoveForward(float value);
	void MoveRight(float value);

public:
	UFUNCTION(BlueprintCallable)
	void EquipGun(ABaseWeapon* const NewGun);
};
