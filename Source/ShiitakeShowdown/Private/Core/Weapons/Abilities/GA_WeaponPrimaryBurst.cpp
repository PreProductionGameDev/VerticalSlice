// 2022 ChronoOwl Studios


#include "Core/Weapons/Abilities/GA_WeaponPrimaryBurst.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Core/GASBlueprintFunctionLibrary.h"
#include "Core/Weapons/BWeapon.h"
#include "Core/Weapons/Abilities/GA_WeaponPrimaryInstant.h"

/**
 * @name Stefan Petrie
 * @brief Set ups the Defaults for the Ability
 */
UGA_WeaponPrimaryBurst::UGA_WeaponPrimaryBurst()
{
	// Default Input
	AbilityInputID = EPlayerAbilityInputID::PrimaryAction;
	AbilityID = EPlayerAbilityInputID::PrimaryAction;
	bSourceObjectMustBeCurrentWeaponToActivate = true;
	bSourceObjectMustBeCurrentElementToActivate = false;

	// Ability Tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));

	// Replication Settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

/**
 * @name Stefan Petrie
 * @brief Activates the Ability. Locally handles the time between each shot to then use the Locally Predicted Instant Ability.
 *		  Will Wait for next valid shot before firing.
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Source Actor Info
 * @param ActivationInfo The info made upon activation
 * @param TriggerEventData EventData made upon the activation of the ability
 */
void UGA_WeaponPrimaryBurst::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Sets up all Cached Variables
	SetupCacheables();

	// Tries to activate the ability
	if (BatchRPCTryActivateAbility(InstantAbilityHandle, false))
	{
		InputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		InputRelease->OnRelease.AddDynamic(this, &UGA_WeaponPrimaryBurst::WaitForValidCancel);
		InputRelease->ReadyForActivation();

		// Call the Firing Loop
		FireBurst();
	}
	else
	{
		const UWorld* World = GetWorld();
		if (!World) return;

		if (!(World->GetTimeSeconds() - InstantAbility->TimeOfLastShot > TimeBetweenShots))
		{
			// Wait for the next valid shot
			UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, TimeBetweenShots - (World->GetTimeSeconds() - InstantAbility->TimeOfLastShot));
			WaitDelay->OnFinish.AddDynamic(this, &UGA_WeaponPrimaryBurst::WaitForValidShot);
			WaitDelay->ReadyForActivation();

			// Input release for removing Abilities
			InputReleaseSecondary = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
			InputReleaseSecondary->OnRelease.AddDynamic(this, &UGA_WeaponPrimaryBurst::WaitForValidCancel);
			InputReleaseSecondary->ReadyForActivation();
		}
	}
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
void UGA_WeaponPrimaryBurst::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * @name  Stefan Petrie
 * @brief Check the Cost for the Ability. Takes the weapon cost into account.
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Owning Actor Info
 * @param OptionalRelevantTags Any Relevant Tags. Not Required
 * @return True if the Ability Can Activate / False if no ammo to fire
 */
bool UGA_WeaponPrimaryBurst::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	// Store the Current Weapon
	const ABWeapon* CurrentWeapon = Cast<ABWeapon>(GetSourceObject(Handle, ActorInfo));
	// Returns true if enough ammo to be allowed to shoot
	if ((CurrentWeapon->GetPrimaryClipAmmo() >= CurrentWeapon->GetAmmoCost() || CurrentWeapon->HasInfiniteAmmo()) && Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return true;
	}

	// Activate the Reload ability if possible
	FGameplayTagContainer AbilitiesToActivate;
	AbilitiesToActivate.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	ActorInfo->AbilitySystemComponent->TryActivateAbilitiesByTag(AbilitiesToActivate);
	return false;
}

/**
 * @name Stefan Petrie
 * @brief The Firing loop delay between bursts
 */
void UGA_WeaponPrimaryBurst::FireLoop()
{
	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, TimeBetweenBurstShots);
	WaitDelay->OnFinish.AddDynamic(this, &UGA_WeaponPrimaryBurst::FireBurst);
	WaitDelay->ReadyForActivation();
}

/**
 * @name Stefan Petrie
 * @brief Fires the Burst Shot
 */
void UGA_WeaponPrimaryBurst::FireBurst()
{
	// Checks if the burst should end. And Calls the FireLoop to delay each burst
	if (BurstShotCurrent == BurstShotCount)
	{
		FireLoop();
		BurstShotCurrent = 0;
		return;
	}

	// Increase the Current burst and fire the shot
	BurstShotCurrent += 1;
	FireShot();
}

/**
 * @name Stefan Petrie
 * @brief Fire the bullet from the Instant Ability
 */
void UGA_WeaponPrimaryBurst::FireShot()
{
	if (CheckCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), new FGameplayTagContainer()))
	{
		CheckRecoilStopped();
		InstantAbility->FireBullet();
		
		// Wait and fire the next burst shot
		UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, TimeBetweenShots);
		WaitDelay->OnFinish.AddDynamic(this, &UGA_WeaponPrimaryBurst::FireBurst);
		WaitDelay->ReadyForActivation();
		return;
	}
	
	// Cancel the Firing Ability
	FiringCancelled(0.0f);
}

/**
 * @name Stefan
 * @brief Waits for the burst to end the abilities
 * @param TimePressed Needed for the callback. Not Used
 */
void UGA_WeaponPrimaryBurst::WaitForValidCancel(float TimePressed)
{
	if (BurstShotCurrent == BurstShotCount || BurstShotCurrent == 0)
	{
		FiringCancelled(0.0f);
		return;
	}

	if (!CheckCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), new FGameplayTagContainer()))
	{
		FiringCancelled(0.0f);
		return;
	}
	
	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, TimeBetweenShots * (BurstShotCount - BurstShotCurrent));
	WaitDelay->OnFinish.AddDynamic(this, &UGA_WeaponPrimaryBurst::EndFiring);
	WaitDelay->ReadyForActivation();
}

/**
 * @name Stefan Petrie
 * @brief Ends the Firing
 */
void UGA_WeaponPrimaryBurst::EndFiring()
{
	FiringCancelled(0.0f);
}

/**
 * @name Stefan Petrie
 * @brief Try to Batch all the RPC calls into a single ability, then activate the fire loop.
 */
void UGA_WeaponPrimaryBurst::WaitForValidShot()
{
	// Wait for the Next Valid Shot for the Firing Loop
	if (BatchRPCTryActivateAbility(InstantAbilityHandle, false))
	{
		InputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		InputRelease->OnRelease.AddDynamic(this, &UGA_WeaponPrimaryBurst::FiringCancelled);
		InputRelease->ReadyForActivation();
		FireLoop();
	}
}
