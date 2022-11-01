// 2022 ChronoOwl Studios
// Stefan Petrie

#include "Core/Weapons/Abilities/GA_WeaponPrimaryInstant.h"

/**
 * @name Stefan Petrie
 * @brief Set ups the Defaults for the Ability
 */
UGA_WeaponPrimaryInstant::UGA_WeaponPrimaryInstant()
{
	// Default Inputs
	AbilityInputID = EPlayerAbilityInputID::None;
	AbilityID = EPlayerAbilityInputID::PrimaryAction;
	bSourceObjectMustBeCurrentWeaponToActivate = true;
	bSourceObjectMustBeCurrentElementToActivate = false;

	// Ability Tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary.Instant")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	
	// Replication Settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGA_WeaponPrimaryInstant::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

/**
 * @name Stefan Petrie
 * @brief Fire the Bullet. OVERRIDE IN CHILDREN IS REQUIRED
 */
void UGA_WeaponPrimaryInstant::FireBullet()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Using Primary Instant Child  Class for Firing"));
}

/**
 * @name Stefan Petrie
 * @brief Handle the Data. Deals Damage Server Side. OVERRIDE IN CHILDREN IS REQUIRED
 * @param TargetData The Target Data produced from a shot
 */
void UGA_WeaponPrimaryInstant::HandleTargetData(const FGameplayAbilityTargetDataHandle& TargetData)
{
	UE_LOG(LogTemp, Warning, TEXT("Not Using Primary Instant Child Class For TargetData"));
}

/**
 * @name Stefan Petrie
 * @brief Handles Weapon Recoil. OVERRIDE IN CHILDREN IS REQUIRED
 */
void UGA_WeaponPrimaryInstant::HandleRecoil()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Using Primary Instant Child for Recoil"))
}

/**
 * @name Stefan Petrie
 * @brief Setup all Variables. OVERRIDE IN CHILDREN IS REQUIRED
 */
void UGA_WeaponPrimaryInstant::SetupCacheables()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Using Primary Instant Child for Cacheables"));
}

/**
 * @name Stefan Petrie
 * @brief Play the Weapon Animations
 */
void UGA_WeaponPrimaryInstant::PlayAnimations()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Using Primary Instant Child Class For Animations"));
}
