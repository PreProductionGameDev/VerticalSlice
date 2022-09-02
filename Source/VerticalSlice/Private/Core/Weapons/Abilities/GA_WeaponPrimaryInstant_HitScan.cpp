// 2022 ChronoOwl Studios
// Stefan Petrie

#include "Core/Weapons/Abilities/GA_WeaponPrimaryInstant_HitScan.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "FP_Character.h"
#include "Core/GASBlueprintFunctionLibrary.h"
#include "Core/Abilities/AbilityTargetActor/ATA_LineTrace.h"
#include "Core/Abilities/AbilityTasks/AT_ServerWaitForClientTargetData.h"
#include "Core/Abilities/AbilityTasks/AT_WaitTargetDataUsingActor.h"
#include "Core/Weapons/BWeapon.h"
#include "Core/Weapons/WeaponUtilityFunctionLibrary.h"
#include "Core/Weapons/Abilities/GA_WeaponPrimary.h"
#include "Curves/CurveVector.h"

/**
 * @name Stefan Petrie
 * @brief Set up the Defaults for the Ability
 */
UGA_WeaponPrimaryInstant_HitScan::UGA_WeaponPrimaryInstant_HitScan()
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
void UGA_WeaponPrimaryInstant_HitScan::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Setup Variables
	SetupCacheables();
	// Reset the Weapon Spread
	LineTraceTargetActor->ResetSpread();

	// Tell the Server to wait for Replicated TargetData
	// Client Returns the Task's Activate()
	// If the player is the server (HOST), This wont do anything since the Data is never be RPC
	// Bullet will be handled locally on the server
	UAT_ServerWaitForClientTargetData* ServerWaitForClientData = UAT_ServerWaitForClientTargetData::ServerWaitForClientTargetData(this, FName("None"), false);
	ServerWaitForClientData->ValidData.AddDynamic(this, &UGA_WeaponPrimaryInstant_HitScan::HandleTargetData);
	ServerWaitForClientData->ReadyForActivation();

	// Client Fires bullet immediately
	FireBullet();
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
void UGA_WeaponPrimaryInstant_HitScan::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
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
bool UGA_WeaponPrimaryInstant_HitScan::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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
bool UGA_WeaponPrimaryInstant_HitScan::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
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
void UGA_WeaponPrimaryInstant_HitScan::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	//Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

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
 * @brief Fire a Bullet. Setups the TraceTargetActor, and waits for TargetData.
 */
void UGA_WeaponPrimaryInstant_HitScan::FireBullet()
{
	if (GetActorInfo().PlayerController->IsLocalController())
	{
		if (GetWorld()->GetTimeSeconds() - TimeOfLastShot >= SourceWeapon->GetTimeBetweenShots() - 0.01)
		{
			if (CheckCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), new FGameplayTagContainer()))
			{
				if (SourceWeapon)
				{
					// Setup Line Trace Target Actor
					TraceStartLocation.SourceComponent = Weapon1PMesh;
					TraceStartLocation.SourceSocketName = FName("MuzzlePos");
					const FCollisionProfileName ProfileName = FName("ProjectileCollision");
					LineTraceTargetActor->Configure(TraceStartLocation, AimingTag, AimingRemovalTag, ProfileName, FGameplayTargetDataFilterHandle(), nullptr, FWorldReticleParameters(), false, false, false, true, true, true, true, 9999999.0, WeaponSpread, AimingSpreadMod, FiringSpreadIncrement, FiringSpreadMax, 1, SourceWeapon->GetNumberOfShots());

					// Wait for Target Data
					UAT_WaitTargetDataUsingActor* TargetDataWait = UAT_WaitTargetDataUsingActor::WaitTargetDataWithReusableActor(this, FName("DataWait"), EGameplayTargetingConfirmation::Instant, LineTraceTargetActor, true);
					TargetDataWait->ValidData.AddDynamic(this, &UGA_WeaponPrimaryInstant_HitScan::HandleTargetData);
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
	}
}

/**
 * @name Stefan Petrie
 * @brief Handles a fired bullet. Occurs when Server receives Target Data or Client Firing a bullet.
 *		  Damage is handled only on the server.
 * @param TargetData The Hit Result Target Data
 */
void UGA_WeaponPrimaryInstant_HitScan::HandleTargetData(const FGameplayAbilityTargetDataHandle& TargetData)
{
	// If We cannot apply the cost, Cancel the ability
	if (!CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
		return;
	}
	
	// Play the Animations
	PlayAnimations();

	// Make the Gameplay Effect Spec and Assign its Tag
	FGameplayEffectSpecHandle DamageGameplayEffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffectClass);
	DamageGameplayEffectSpec = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageGameplayEffectSpec, FGameplayTag::RequestGameplayTag(FName("Data.Damage")), static_cast<float>(SourceWeapon->GetDamage()));

	// Apply the Gameplay Effect to the target and add Target Data to the Effect Context 
	TArray<FActiveGameplayEffectHandle> AppliedDamageEffect = ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), DamageGameplayEffectSpec, TargetData);
	const FGameplayEffectContextHandle DamageEffectContext = UAbilitySystemBlueprintLibrary::GetEffectContext(DamageGameplayEffectSpec);
	UGASBlueprintFunctionLibrary::EffectContextAddTargetData(DamageEffectContext, TargetData);

	// Execute Gameplay Cue. Handles Audio and VFX
	FGameplayCueParameters GameplayCueParams;
	GameplayCueParams.EffectContext = DamageEffectContext;
	K2_ExecuteGameplayCueWithParams(GameplayCueWeaponFire, GameplayCueParams);

	// Check Hit Marker, Solely on the server
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		UWeaponUtilityFunctionLibrary::CheckHitMarker(TargetData, OwningPlayer);
	}
	
}

void UGA_WeaponPrimaryInstant_HitScan::HandleRecoil()
{
	// Dont Process Recoil if not pattern
	if (!SourceWeapon->GetRecoilPattern())
	{
		return;
	}
	// Cache the recoil time frame
	GA_Primary->StepBeforeRecoilTime = GA_Primary->RecoilTime - SourceWeapon->GetTimeBetweenShots();
	// Get the Recoil Vectors
	const FVector CurrentRecoilTimeVector = SourceWeapon->GetRecoilPattern()->GetVectorValue(GA_Primary->RecoilTime);
	const FVector PrevRecoilTimeVector = SourceWeapon->GetRecoilPattern()->GetVectorValue(GA_Primary->StepBeforeRecoilTime);
	// Add the Offset to the players Camera
	OwningPlayer->AddControllerPitchInput(PrevRecoilTimeVector.Y - CurrentRecoilTimeVector.Y);
	OwningPlayer->AddControllerYawInput(PrevRecoilTimeVector.X - CurrentRecoilTimeVector.X);
	// Update the Recoil time
	GA_Primary->RecoilTime = GA_Primary->RecoilTime + SourceWeapon->GetTimeBetweenShots();
}

/**
 * @name Stefan Petrie
 * @brief Setup and store all of the Cacheable information
 */
void UGA_WeaponPrimaryInstant_HitScan::SetupCacheables()
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
void UGA_WeaponPrimaryInstant_HitScan::PlayAnimations()
{
	Weapon1PMesh->GetAnimInstance()->Montage_Play(WeaponAnimationMontage, 1.0, EMontagePlayReturnType::MontageLength, 0, true);
	Weapon1PMesh->GetAnimInstance()->Montage_JumpToSection(FName("Shoot"));

	Weapon3PMesh->GetAnimInstance()->Montage_Play(WeaponAnimationMontage, 1.0, EMontagePlayReturnType::MontageLength, 0, true);
	Weapon3PMesh->GetAnimInstance()->Montage_JumpToSection(FName("Shoot"));
}
