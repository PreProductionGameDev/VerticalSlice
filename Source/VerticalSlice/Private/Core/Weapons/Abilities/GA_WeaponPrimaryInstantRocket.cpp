// 2022 ChronoOwl Studios
// Stefan Petrie

#include "Core/Weapons/Abilities/GA_WeaponPrimaryInstantRocket.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FP_Character.h"
#include "Core/GASBlueprintFunctionLibrary.h"
#include "Core/Abilities/AbilityTargetActor/ATA_LineTrace.h"
#include "Core/Abilities/AbilityTasks/AT_ServerWaitForClientTargetData.h"
#include "Core/Abilities/AbilityTasks/AT_WaitTargetDataUsingActor.h"
#include "Core/Weapons/Abilities/GA_WeaponPrimary.h"
#include "Core/Weapons/Projectiles/BProjectile.h"
#include "Kismet/KismetMathLibrary.h"

/**
 * @name Stefan Petrie
 * @brief Set up the Defaults for the Ability
 */
UGA_WeaponPrimaryInstantRocket::UGA_WeaponPrimaryInstantRocket()
{
	// Ability Settings
	AbilityInputID = EPlayerAbilityInputID::None;
	AbilityID = EPlayerAbilityInputID::PrimaryAction;
	bSourceObjectMustBeCurrentWeaponToActivate = true;
	bSourceObjectMustBeCurrentElementToActivate = false;

	// Tag Setup
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary.Instant")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	
	// Replication Settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

/**
 * @name Stefan Petrie
 * @brief Activates the Ability. Tells Server to Wait for TargetData, Instantly fires a bullet locally
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Source Actor Info
 * @param ActivationInfo The info made upon activation
 * @param TriggerEventData EventData made upon the activation of the ability
 */
void UGA_WeaponPrimaryInstantRocket::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	SetupCacheables();

	LineTraceTargetActor->ResetSpread();

	// Tell the Server to wait for Replicated TargetData
	// Client Returns the Task's Activate()
	// If the player is the server (HOST), This wont do anything since the Data is never be RPC
	// Bullet will be handled locally on the server
	ServerWaitForClientData = UAT_ServerWaitForClientTargetData::ServerWaitForClientTargetData(this, FName("None"), false);
	ServerWaitForClientData->ValidData.AddDynamic(this, &UGA_WeaponPrimaryInstantRocket::HandleTargetData);
	ServerWaitForClientData->ReadyForActivation();
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
void UGA_WeaponPrimaryInstantRocket::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ServerWaitForClientData)
	{
		ServerWaitForClientData->EndTask();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * @name Stefan Petrie
 * @brief Checks if the ability can activate. Checks the Time Between Shots
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Source Actor Info
 * @param SourceTags Tags from the Source Actor
 * @param TargetTags Tags from the Target
 * @param OptionalRelevantTags Optional Tags. Not Used
 * @return True if can Activate / False if can not Activate 
 */
bool UGA_WeaponPrimaryInstantRocket::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (GA_Primary)
	{
		if (abs(GetWorld()->GetTimeSeconds() - TimeOfLastShot) >= SourceWeapon->GetTimeBetweenShots())
		{
			return true;
		}
		return false;
	}
	return true;
}

/**
 * @name  Stefan Petrie
 * @brief Check the Cost for the Ability. Takes the weapon cost into account.
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Owning Actor Info
 * @param OptionalRelevantTags Any Relevant Tags. Not Required
 * @return True if the Ability Can Activate / False if no ammo to fire
 */
bool UGA_WeaponPrimaryInstantRocket::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	// Store the Current Weapon
	const ABWeapon* CurrentWeapon = Cast<ABWeapon>(GetSourceObject(Handle, ActorInfo));
	
	// Returns true if enough ammo to be allowed to shoot
	if ((CurrentWeapon->GetPrimaryClipAmmo() >= CurrentWeapon->GetAmmoCost() || CurrentWeapon->HasInfiniteAmmo()) && Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return true;
	}
	return false;
}

/**
 * @name Stefan Petrie
 * @brief Removes Ammo from the Player for firing. Takes infinite ammo into account
 * @param Handle The Ability Spec Handle
 * @param ActorInfo The Owning Actor Info
 * @param ActivationInfo Ability Activation Information
 */
void UGA_WeaponPrimaryInstantRocket::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// Get the Current Weapon and Apply the Cost if not Infinite Ammo
	if (ABWeapon* CurrentWeapon = Cast<ABWeapon>(GetSourceObject(Handle, ActorInfo)))
	{
		if (CurrentWeapon->HasInfiniteAmmo())
		{
			return;
		}
		// Apply the Cost
		CurrentWeapon->SetPrimaryClipAmmo(CurrentWeapon->GetPrimaryClipAmmo() - CurrentWeapon->GetAmmoCost());
	}
}

/**
 * @name Stefan Petrie
 * @brief Calculates trajectory for projectile but lets server handle shooting
 */
void UGA_WeaponPrimaryInstantRocket::FireBullet()
{
	// Ensure the Player is locally controlled
	if (!GetActorInfo().PlayerController->IsLocalController())
	{
		return;
	}

	// Check that the Player can Shoot
	if (!(GetWorld()->GetTimeSeconds() - TimeOfLastShot >= SourceWeapon->GetTimeBetweenShots() - 0.01))
	{
		return;
	}

	// Check the Cost and handle the Weapon Firing
	if (CheckCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), new FGameplayTagContainer()))
	{
		if (SourceWeapon)
		{
			// Setup Line Trace Target Actor
			TraceStartLocation.LocationType = EGameplayAbilityTargetingLocationType::SocketTransform;
			TraceStartLocation.SourceComponent = Weapon1PMesh;
			TraceStartLocation.SourceSocketName = FName("MuzzlePos");
			const FCollisionProfileName ProfileName = FName("ProjectileCollision");
			LineTraceTargetActor->Configure(TraceStartLocation, AimingTag, AimingRemovalTag, ProfileName, FGameplayTargetDataFilterHandle(), nullptr, FWorldReticleParameters(), false, false, false, true, true, true, true, 9999999.0, 0, 0, 0, 0, 1, SourceWeapon->GetNumberOfShots());

			// Wait for Target Data
			UAT_WaitTargetDataUsingActor* TargetDataWait = UAT_WaitTargetDataUsingActor::WaitTargetDataWithReusableActor(this, FName("DataWait"), EGameplayTargetingConfirmation::Instant, LineTraceTargetActor, true);
			TargetDataWait->ValidData.AddDynamic(this, &UGA_WeaponPrimaryInstantRocket::HandleTargetData);
			TargetDataWait->ReadyForActivation();

			// Set Last Shot Time
			TimeOfLastShot = GetWorld()->GetTimeSeconds();
			// Process Weapon Recoil
			HandleRecoil();	
		}
	}
	else
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

/**
 * @name Stefan Petrie
 * @brief Handles a fired bullet. Occurs when Server receives Target Data.
 *		  Server Spawns projectile
 * @param TargetData The Hit Result Target Data
 */
void UGA_WeaponPrimaryInstantRocket::HandleTargetData(const FGameplayAbilityTargetDataHandle& TargetData)
{
	// If We cannot apply the cost, Cancel the ability
	if (!CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}

	PlayAnimations();

	// TODO: SETUP GAMEPLAY EFFECT CREATION
	// PASS TO THE PROJECTILE CREATED

	// Spawn the Projectile on Server Only
	if (OwningPlayer->HasAuthority())
	{
		// Spawn
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = OwningPlayer;
		SpawnParameters.Instigator = OwningPlayer;
		const FVector SpawnLocation = HitResult.TraceStart;
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(HitResult.TraceStart, HitResult.Location);

		const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

		//auto* Projectile = GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation, SpawnParameters);
		
		ABProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABProjectile>(ProjectileClass, SpawnTransform);
		Projectile->SetOwner(OwningPlayer);

		Projectile->FinishSpawning(SpawnTransform);
	}
	
}

/**
 * @name Stefan Petrie
 * @brief Process the Recoil and apply it to the player
 */
void UGA_WeaponPrimaryInstantRocket::HandleRecoil()
{
	// TODO: PROCESS RECOIL
	Super::HandleRecoil();
}

/**
 * @name Stefan Petrie
 * @brief Setup and store all of the Cacheable information
 */
void UGA_WeaponPrimaryInstantRocket::SetupCacheables()
{
	// Store the Source Weapon
	if (!SourceWeapon)
	{
		SourceWeapon = Cast<ABWeapon>(GetCurrentSourceObject());
		if (!SourceWeapon) return;
	}
	// Store the Weapon First Person Mesh
	if (!Weapon1PMesh)
	{
		Weapon1PMesh = SourceWeapon->GetWeaponMesh1P();
	}
	// Store the Weapon Third Person Mesh
	if (!Weapon3PMesh)
	{
		Weapon3PMesh = SourceWeapon->GetWeaponMesh3P();
	}
	// Get the Owning Player from Avatar Info
	if (!OwningPlayer)
	{
		OwningPlayer = Cast<AFP_Character>(GetAvatarActorFromActorInfo());
		if (!OwningPlayer) return;
	}
	// Store the Primary Ability
	if (!GA_Primary)
	{
		GA_Primary = Cast<UGA_WeaponPrimary>(UGASBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GetAbilitySystemComponentFromActorInfo(), GA_PrimaryClass));
		if (!GA_Primary) return;
	}
	// Store the Line Trace Target Actor
	if (!LineTraceTargetActor)
	{
		LineTraceTargetActor = SourceWeapon->GetLineTraceTargetActor();
	}
}

/**
 * @name Stefan Petrie
 * @brief Play the Weapon Animations
 */
void UGA_WeaponPrimaryInstantRocket::PlayAnimations()
{
	// TODO: PLAY ANIMATIONS
	Super::PlayAnimations();
}
