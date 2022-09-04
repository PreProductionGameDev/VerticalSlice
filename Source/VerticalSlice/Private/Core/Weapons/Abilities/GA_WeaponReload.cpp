// 2022 ChronoOwl Studios


#include "Core/Weapons/Abilities/GA_WeaponReload.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FP_Character.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

/**
 * @name Stefan Petrie
 * @brief Set up the Defaults for the Ability
 */
UGA_WeaponReload::UGA_WeaponReload()
{
	// Default Input
	AbilityInputID = EPlayerAbilityInputID::Reload;
	AbilityID = EPlayerAbilityInputID::Reload;
	bSourceObjectMustBeCurrentWeaponToActivate = true;
	bSourceObjectMustBeCurrentElementToActivate = false;

	// Ability Tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Secondary")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));

	// Replication Settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

/**
 * @name Stefan Petrie
 * @brief Activates the Ability. Plays the animations and applies the reload effect after the reload time.
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Source Actor Info
 * @param ActivationInfo The info made upon activation
 * @param TriggerEventData EventData made upon the activation of the ability
 */
void UGA_WeaponReload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Cache the Variables
	SetupCacheables();

	// End Ability if there is no Weapon
	if (!SourceWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Reload Invalid"));
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
	}
	
	// Play the Animations
	PlayAnimations();
	// Wait for the Reload Time
	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, ReloadTime);
	WaitDelay->OnFinish.AddDynamic(this, &UGA_WeaponReload::ReloadAmmo);
	WaitDelay->ReadyForActivation();
}

/**
 * @name  Stefan Petrie
 * @brief End the Ability by calling Parent EndAbility
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Source Actor Info
 * @param ActivationInfo The Information made upon activation
 * @param bReplicateEndAbility Whether to replicate the ability
 * @param bWasCancelled If the Ability was cancelled or completed naturally
 */
void UGA_WeaponReload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * @name Stefan Petrie
 * @brief Checks if the ability can activate. Checks if the player has not max clip ammo, and has reserve ammo
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Source Actor Info
 * @param SourceTags Tags from the Source Actor
 * @param TargetTags Tags from the Target
 * @param OptionalRelevantTags Optional Tags. Not Used
 * @return True if can Activate / False if can not Activate 
 */
bool UGA_WeaponReload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const AFP_Character* PlayerCharacter = Cast<AFP_Character>(ActorInfo->AvatarActor); 
	return PlayerCharacter->GetPrimaryClipAmmo() < PlayerCharacter->GetMaxPrimaryClipAmmo() && PlayerCharacter->GetPrimaryReserveAmmo() > 0;
}

/**
 * @name Stefan Petrie
 * @brief Setup and store all of the Cacheable information
 */
void UGA_WeaponReload::SetupCacheables()
{
	// Store Weapon
	if (!SourceWeapon)
	{
		SourceWeapon = Cast<ABWeapon>(GetCurrentSourceObject());
	}
	// Store Player
	if (!OwningPlayer)
	{
		OwningPlayer = Cast<AFP_Character>(GetAvatarActorFromActorInfo());
	}
}

/**
 * @name Stefan Petrie
 * @brief Create the Reload Gameplay Effect, Apply to the Player
 */
void UGA_WeaponReload::ReloadAmmo()
{
	if (!ReloadEffect || !SourceWeapon)
	{
		return;
	}
	
	// Find out how much Ammo to Reload
	const int AmmoToReload = SourceWeapon->GetMaxPrimaryClipAmmo() - SourceWeapon->GetPrimaryClipAmmo();

	// Set the ammo that can be Reloaded
	int AmmoCanReload = AmmoToReload;
	if (AmmoToReload <= OwningPlayer->GetPrimaryReserveAmmo())
	{
		AmmoCanReload = AmmoToReload;
	}

	// Create the Gameplay Effect and set the reload amount
	FGameplayEffectSpecHandle DamageGameplayEffectSpec = MakeOutgoingGameplayEffectSpec(ReloadEffect);
	DamageGameplayEffectSpec = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageGameplayEffectSpec, FGameplayTag::RequestGameplayTag(FName("Data.ReloadAmount.Reserve")), static_cast<float>(-AmmoCanReload));
	// Apply the Gameplay Effect to self
	TArray<FActiveGameplayEffectHandle> AppliedReloadEffect = ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), DamageGameplayEffectSpec, UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OwningPlayer));

	// Set the Clip Ammo
	SourceWeapon->SetPrimaryClipAmmo(SourceWeapon->GetPrimaryClipAmmo() + AmmoCanReload);

	// End the Ability
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

/**
 * @name Stefan Petrie
 * @brief Play the Weapon Animations
 */
void UGA_WeaponReload::PlayAnimations()
{
	if (SourceWeapon && WeaponAnimationMontage)
	{	
		SourceWeapon->GetWeaponMesh1P()->GetAnimInstance()->Montage_Play(WeaponAnimationMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0, true);
		SourceWeapon->GetWeaponMesh1P()->GetAnimInstance()->Montage_JumpToSection(FName("Reload"));
	}
}
