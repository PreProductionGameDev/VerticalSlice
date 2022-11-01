// 2022 ChronoOwl Studios
// Stefan Petrie

#include "Core/Weapons/Abilities/GA_WeaponPrimary.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Core/GASBlueprintFunctionLibrary.h"
#include "Core/Weapons/BWeapon.h"
#include "Core/Weapons/Abilities/GA_WeaponPrimaryInstant.h"

/**
 * @name Stefan Petrie
 * @brief Set ups the Defaults for the Ability
 */
UGA_WeaponPrimary::UGA_WeaponPrimary()
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
void UGA_WeaponPrimary::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Activates parent logic to allow Ability to Work
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// Sets up all cached variables
	SetupCacheables();

	// Tries to activate the instant ability
	if (BatchRPCTryActivateAbility(InstantAbilityHandle, false))
	{
		// Setup input release for disabling the ability
		InputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		InputRelease->OnRelease.AddDynamic(this, &UGA_WeaponPrimary::FiringCancelled);
		InputRelease->ReadyForActivation();

		InstantAbility->FireBullet();
		// Call the Firing loop
		FireLoop();
	}
	else
	{
		const UWorld* World = GetWorld();
		if (!World) return;

		// Check if the time between shots is less. This should always be true, but just in case
		if (!(World->GetTimeSeconds() - InstantAbility->TimeOfLastShot > TimeBetweenShots))
		{
			// Wait for the next Valid Shot
			UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, TimeBetweenShots - (World->GetTimeSeconds() - InstantAbility->TimeOfLastShot));
			WaitDelay->OnFinish.AddDynamic(this, &UGA_WeaponPrimary::WaitForValidShot);
			WaitDelay->ReadyForActivation();

			// Input Release for removing the abilities.
			InputReleaseSecondary = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
			InputReleaseSecondary->OnRelease.AddDynamic(this, &UGA_WeaponPrimary::FiringCancelled);
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
void UGA_WeaponPrimary::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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
bool UGA_WeaponPrimary::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
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
 * @name  Stefan Petrie
 * @brief The Firing loop, Waits between each shot and then calls FireShot()
 */
void UGA_WeaponPrimary::FireLoop()
{
	// Wait the time between shots and then activate the shot
	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, TimeBetweenShots);
	WaitDelay->OnFinish.AddDynamic(this, &UGA_WeaponPrimary::FireShot);
	WaitDelay->ReadyForActivation();
}

/**
 * @name Stefan Petrie
 * @brief Fire the bullet from the Instant Ability
 */
void UGA_WeaponPrimary::FireShot()
{
	// Check If we can shoot and fire the Bullet and repeat loop
	if (CheckCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), new FGameplayTagContainer()))
	{
		CheckRecoilStopped();
		InstantAbility->FireBullet();
		FireLoop();
		return;
	}
	
	// Cancel the Firing Ability
	FiringCancelled(0.0f);
}

/**
 * @name Stefan Petrie
 * @brief Occurs when Firing is halted. Either from lack of ammo, or Mouse Release
 * @param TimePressed The Amount of Time Pressed. Required to use WaitInputRelease
 */
void UGA_WeaponPrimary::FiringCancelled(float TimePressed)
{
	// Cancel the Instant Ability
	InstantAbility->ExternalEndAbility();
	// Cancel this Ability
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);

	GetWorld()->GetTimerManager().SetTimer(RecoilResetHandle, this, &UGA_WeaponPrimary::ResetRecoilTime, SourceWeapon->GetRecoilCooldown());
}

/**
 * @name Stefan Petrie
 * @brief Try to Batch all the RPC calls into a single ability, then activate the fire loop.
 */
void UGA_WeaponPrimary::WaitForValidShot()
{
	// Wait for the Next Valid Shot for the Firing Loop
	if (BatchRPCTryActivateAbility(InstantAbilityHandle, false))
	{
		InputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		InputRelease->OnRelease.AddDynamic(this, &UGA_WeaponPrimary::FiringCancelled);
		InputRelease->ReadyForActivation();
		FireLoop();
	}
}

/**
 * @name Stefan Petrie
 * @brief Setup and store all of the Cacheable information
 */
void UGA_WeaponPrimary::SetupCacheables()
{
	// Get the Source Weapon
	if (!SourceWeapon)
	{
		SourceWeapon = Cast<ABWeapon>(GetCurrentSourceObject());
		if(!SourceWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon WarningL: No Source Weapon"));
			return;
		}
	}
	
	UPlayerAbilitySystemComponent* PlayerASC = Cast<UPlayerAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	// Get the Instant Ability Spec Handle
	if (!UGASBlueprintFunctionLibrary::IsAbilitySpecHandleValid(InstantAbilityHandle))
	{
		InstantAbilityHandle = PlayerASC->FindAbilitySpecHandleForClass(InstantAbilityClass);
	}

	// Get The Instant Ability
	if (!InstantAbility)
	{
		InstantAbility = Cast<UGA_WeaponPrimaryInstant>(UGASBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromHandle(PlayerASC, InstantAbilityHandle));
		if (!InstantAbility)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon Warning: No Instant Ability"));
			return;
		}
	}

	// Set the Time Between shots
	TimeBetweenShots = SourceWeapon->GetTimeBetweenShots();
}

/**
 * @name Stefan Petrie
 * @brief Set the Recoil time to 0
 */
void UGA_WeaponPrimary::ResetRecoilTime()
{
	RecoilTime = 0.0f;
}

/**
 * @name Stefan Petrie
 * @brief Lower Recoil time based on the time elapsed
 */
void UGA_WeaponPrimary::CheckRecoilStopped()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	if (World->GetTimerManager().TimerExists(RecoilResetHandle))
	{
		RecoilTime = World->GetTimerManager().GetTimerElapsed(RecoilResetHandle) / RecoilTime;
		World->GetTimerManager().ClearTimer(RecoilResetHandle);
	}
}
