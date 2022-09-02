// 2022 ChronoOwl Studios
// Stefan Petrie

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_WeaponReload.generated.h"

/**
 *	The Reload Ability for the Weapon
 */
UCLASS()
class VERTICALSLICE_API UGA_WeaponReload : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_WeaponReload();

	// Activation for the Ability
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// End the Ability
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// Check if we can actually activate the Ability
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
	// The Source Weapon Class
	UPROPERTY()
	class ABWeapon* SourceWeapon;
	// Player who Owns this Ability
	UPROPERTY()
	class AFP_Character* OwningPlayer;
	// The Time for the Reload. Saves setting up Anim Notifs
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ReloadTime;
	// Reloading Gameplay Effect
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UGameplayEffect> ReloadEffect;
	// Weapon Animation Montage
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* WeaponAnimationMontage;

	// Cache the Variables
	void SetupCacheables();
	// Reloading the Ammo server side
	UFUNCTION()
	void ReloadAmmo();
	// Play the Third and First Person Animations
	UFUNCTION()
	void PlayAnimations();
};
