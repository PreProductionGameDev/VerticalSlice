// 2022 ChronoOwl Studios
// Stefan Petrie

#pragma once

#include "CoreMinimal.h"
#include "Core/Weapons/Abilities/GA_WeaponPrimaryInstant.h"
#include "GA_WeaponPrimaryInstantRocket.generated.h"

/**
 *	The Primary Instant Ability for a Rocket Weapon.
 *	Overrides the Parent class.
 */
UCLASS()
class SHIITAKESHOWDOWN_API UGA_WeaponPrimaryInstantRocket : public UGA_WeaponPrimaryInstant
{
	GENERATED_BODY()

public:
	UGA_WeaponPrimaryInstantRocket();

	// Activation for the Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// End the Ability. Ends Server Wait Task and Calss super
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Checks if the Ability can Activate
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	// Checks the cost for the Ability
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	// Applies the Cost for the Ability
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	// Create the Bullet. Projectile for this Class
	virtual void FireBullet() override;
	// Handles the Data and spawns the bullet
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag AimingTag;
	// Tag for End of player Aiming
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag AimingRemovalTag;

	// Class for the Projectile 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ABProjectile> ProjectileClass;

	// Trace Start location for TraceTargetActor
	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo TraceStartLocation;
	// Server Wait Task
	UPROPERTY()
	class UAT_ServerWaitForClientTargetData* ServerWaitForClientData;

	// Handles the weapons recoil patter
	virtual void HandleRecoil() override;
	// Store all runtime Variables
	virtual void SetupCacheables() override;
	// Play Animations
	virtual void PlayAnimations() override;
};
