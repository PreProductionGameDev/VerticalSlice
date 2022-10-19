// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Core/Data.h"
#include "Core/PlayerAbilitySystemComponent.h"
#include "Core/PlayerGameplayAbility.h"
#include "BWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponAmmoChangedDelegate, int32, OldValue, int32, NewValue);

class AATA_LineTrace;
class AFP_Character;
class APlayerAbilitySystemComponent;
class UCurveVector;

/*
 *		The Default Weapon Class using the GameplayAbility System
 *		This Class handles the functionality of using and adding weapons to the player
 */
UCLASS(Blueprintable, BlueprintType)
class VERTICALSLICE_API ABWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABWeapon();

	/*
	 *	Default Public Variables
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FText WeaponName;
	
	// Whether or not to spawn this weapon with collision enabled (Pickup Mode)
	// Set to false when spawning directly into a player's inventory or true when spawning into the world in pickup mode.
	UPROPERTY(BlueprintReadWrite, Replicated)
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

	// Delegate for the Change of Ammo Clip. Updates UI
	UPROPERTY(BlueprintAssignable, Category = "ShiitakeShowdown|GASWeapon")
	FWeaponAmmoChangedDelegate OnPrimaryClipAmmoChanged;
	// Delegate for the change of Max Ammo Clip. Updates UI
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

	// Network Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Occurs before Replicating
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	
	// Sets the Owning Character for the weapon
	void SetOwningCharacter(AFP_Character* InOwningCharacter);
	// Overlap for Picking up the weapon
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called when Player equips this weapon
	virtual void Equip();
	// Called when Player UnEquips this weapon
	virtual void UnEquip();

	// Adding the GAS Abilities
	virtual void AddAbilities();
	// Removing the GAS Abilities
	virtual void RemoveAbilities();
	// Returns the GAS Ability Level. Default is 1
	virtual int32 GetAbilityLevel(EPlayerAbilityInputID AbilityID);

	// Resets the weapon stats
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual void ResetWeapon();

	// Network Multicasted function for Weapon Dropping
	UFUNCTION(NetMulticast, Reliable)
	void OnDropped(FVector NewLocation);
	virtual void OnDropped_Implementation(FVector NewLocation);
	virtual bool OnDropped_Validate(FVector NewLocation);

	// Removes the Weapon from the Inventory
	void DeleteWeapon();
	
	// Primary AmmoClip Functions
	// Getters and Setters
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual int32 GetPrimaryClipAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual int32 GetMaxPrimaryClipAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual void SetPrimaryClipAmmo(int32 NewPrimaryClipAmmo);
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual void SetMaxPrimaryClipAmmo(int32 NewMaxPrimaryClipAmmo);
	// Variable to Enable Infinite Ammo
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|GASWeapon")
	virtual bool HasInfiniteAmmo() const;

	// Get Animation Montages for Equipping in both First and Third Person
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Animation")
	UAnimMontage* GetEquip1PMontage() const;
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Animation")
	UAnimMontage* GetEquip3PMontage() const;

	// Audio Cue for picking up the weapon
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Audio")
	class USoundCue* GetPickupSound() const;
	// Audio Cue for shooting
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Audio")
	class USoundCue* GetGunShotSound() const;
	
	// Variable Getters
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Targeting")
	AATA_LineTrace* GetLineTraceTargetActor();
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Recoil")
	UCurveVector* GetRecoilPattern() const;
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Stats")
	int32 GetAmmoCost() const;
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Stats")
	int32 GetNumberOfShots() const;
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Stats")
	float GetTimeBetweenShots() const;
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Stats")
	float GetRecoilCooldown() const;
	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Stats")
	int32 GetDamage() const;

	UPROPERTY(BlueprintReadWrite)
	bool Pickupable = false;
	
protected:
	// The AbilitySystemComponent of the owning player
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
	// The Vector Curve for Recoil Pattern
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon|Recoil")
	UCurveVector* RecoilPattern;
	// The rate at which the recoil subsides
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon|Recoil")
	float RecoilCooldownRate;
	// The Ammo Cost per Shot
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon|Stats")
	int32 AmmoCost;
	// The Time for each shot
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon|Stats")
	float TimeBetweenShots;
	// The Damage per shot
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon|Stats")
	int32 Damage;
	// Number of Shots fired
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon|Stats")
	int32 ShotCount = 1;
	// Collision capsule for when weapon is in pickup mode
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CollisionComp;
	
	// Mesh For First Person
	UPROPERTY(VisibleAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	USkeletalMeshComponent* WeaponMesh1P;
	// Mesh for Third Person
	UPROPERTY(VisibleAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	USkeletalMeshComponent* WeaponMesh3P;
	// Relative Location of weapon 3P Mesh when in pickup mode
	// 1P weapon mesh is invisible so it doesn't need one
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FVector WeaponMesh3PickupRelativeLocation;
	// Relative Location of weapon 1P Mesh when equipped
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FVector WeaponMesh1PEquippedRelativeLocation;
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FRotator WeaponMesh1PEquippedRelativeRotation;
	// Relative Location of weapon 3P Mesh when equipped
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FVector WeaponMesh3PEquippedRelativeLocation;
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|GASWeapon")
	FRotator WeaponMesh3PEquippedRelativeRotation;

	// The Character who Owns this Weapon
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ShiitakeShowdown|GASWeapon")
	AFP_Character* OwningCharacter;

	// The Array of Abilities for the weapon
	UPROPERTY(EditAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	TArray<TSubclassOf<UPlayerGameplayAbility>> Abilities;
	// AbilitySpecHandles for the applied abilities
	UPROPERTY(BlueprintReadOnly, Category = "ShiitakeShowdown|GASWeapon")
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// The Firing Mode. Used if we add alternate ways to fire
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	FGameplayTag DefaultFireMode;

	// Animation Montage for Equipping in First Person
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	UAnimMontage* Equip1PMontage;
	// Animation Montage for Equipping in Third Person
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|GASWeapon")
	UAnimMontage* Equip3PMontage;
	// Sound Played when Picked up
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|Audio")
	class USoundCue* PickupSound;
	// Sound when Shooting
	UPROPERTY(EditDefaultsOnly, Category = "ShiitakeShowdown|Audio")
	class USoundCue* GunshotSound;

	// Cached Gameplay Tags
	FGameplayTag WeaponPrimaryInstantAbilityTag;
	FGameplayTag WeaponSecondaryInstantAbilityTag;
	FGameplayTag WeaponIsFiringTag;

	// LineTraceTargetActor used for the raycast for GAS Network data
	AATA_LineTrace* LineTraceTargetActor;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called at teh End of the Game
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	// Called when Player Picks up the weapon
	virtual void PickUpOnTouch(AFP_Character* InCharacter);

	// Replication functions for Primary Clip and Max Clip Ammo
	UFUNCTION()
	virtual void OnRep_PrimaryClipAmmo(int32 OldPrimaryClipAmmo);
	UFUNCTION()
	virtual void OnRep_MaxPrimaryClipAmmo(int32 OldMaxPrimaryClipAmmo);
};
