// 2022 ChronoOwl Studios
// Stefan Petrie

#pragma once

#include "CoreMinimal.h"
#include "Core/Weapons/Abilities/GA_WeaponPrimaryInstant.h"
#include "GA_WeaponPrimaryInstant_HitScan.generated.h"

/**
 *	The Primary Instant Ability for a Hit Scan weapon.
 *	Overrides the Parent class.
 */
UCLASS()
class VERTICALSLICE_API UGA_WeaponPrimaryInstant_HitScan : public UGA_WeaponPrimaryInstant
{
	GENERATED_BODY()

public:
	UGA_WeaponPrimaryInstant_HitScan();

	// Activation for the Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// End the Ability. Just calls super
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Checks if Ability Can Activate
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	// Checks the Cost for the Ability
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	// Apply the Cost for the Ability
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	// Create the Bullet. HitScan for this Class 
	virtual void FireBullet() override;
	// Handles the Data and deals the damage
	virtual void HandleTargetData(const FGameplayAbilityTargetDataHandle& TargetData) override;

protected:
	// Weapon First Person Mesh
	UPROPERTY(EditAnywhere, Category = "ShiitakeShowdown|WeaponComponents")
	USkeletalMeshComponent* Weapon1PMesh;
	// Weapon Third Person Mesh
	UPROPERTY(EditAnywhere, Category = "ShiitakeShowdown|WeaponComponents")
	USkeletalMeshComponent* Weapon3PMesh;
	// Weapon Animation Montage
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ShiitakeShowdown|WeaponComponents")
	UAnimMontage* WeaponAnimationMontage;

	// Weapon for the Ability
	UPROPERTY()
	class ABWeapon* SourceWeapon;
	// Player who Owns this Ability
	UPROPERTY()
	class AFP_Character* OwningPlayer;
	// Weapon Primary Ability class. Set in Blueprint Instance
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayAbility> GA_PrimaryClass;
	// Weapon Primary Ability. Obtained at Runtime
	UPROPERTY()
	class UGA_WeaponPrimary* GA_Primary;
	// LineTraceTargetActor obtained from player
	UPROPERTY()
	class AATA_LineTrace* LineTraceTargetActor;

	// Damage Effect Class. Handles Damage
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	// Tag for when player is Aiming
	FGameplayTag AimingTag;
	// Tag for End of player Aiming
	FGameplayTag AimingRemovalTag;

	// Gameplay Cue Tag for Firing the Weapon
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag GameplayCueWeaponFire;

	// Weapon Stats
	// Weapon Spread
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeaponSpread = 0.0f;
	// Aiming Spread Modifier
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimingSpreadMod = 0.0f;
	// Increase in spread when firing
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FiringSpreadIncrement = 0.0f;
	// Max Firing Spread
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FiringSpreadMax = 0.0f;

	// Trace Start location for TraceTargetActor
	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo TraceStartLocation;

	// Handles the weapons recoil patter
	virtual void HandleRecoil() override;
	// Store all runtime Variables
	virtual void SetupCacheables() override;
	// Play Animations
	virtual void PlayAnimations() override;
};
