// 2022 ChronoOwl Studios


#include "Core/Weapons/Abilities/GA_WeaponNext.h"

#include "FP_Character.h"

UGA_WeaponNext::UGA_WeaponNext()
{
	// Default Input
	AbilityInputID = EPlayerAbilityInputID::NextWeapon;
	AbilityID = EPlayerAbilityInputID::NextWeapon;
	bSourceObjectMustBeCurrentWeaponToActivate = false;
	bSourceObjectMustBeCurrentElementToActivate = false;

	// Ability Tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Swap")));
	
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary.Instant")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Secondary")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.SMG")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.Shotgun")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.Sniper")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.BurstRifle")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.Rocket")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Secondary")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Swap")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dying")));

	// Replication Settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UGA_WeaponNext::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (IsLocallyControlled())
	{
		if (const AFP_Character* Player = Cast<AFP_Character>(GetOwningActorFromActorInfo()))
		{
			const ABWeapon* Weapon = Player->GetCurrentWeapon();
			if (Weapon)
			{
				if (UAnimMontage* Montage = Weapon->GetWeaponMesh1P()->GetAnimInstance()->GetCurrentActiveMontage())
				{
					const int32 SectionID = Montage->GetSectionIndex(FName("UnEquip"));
					if (Montage->IsValidSectionIndex(SectionID))
					{
						Weapon->GetWeaponMesh1P()->GetAnimInstance()->Montage_JumpToSection("UnEquip");
						Weapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UGA_WeaponNext::SwapWeapon);
						return;
					}
				}
			}
		}
		SwapWeapon(FName(), FBranchingPointNotifyPayload());
	}
}

void UGA_WeaponNext::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (bWasCancelled)
	{
		// An Error Occured forcing this ability to cancel
		// I opted to remove canceling from ending the ability after failing to fix
		return;
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_WeaponNext::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{

	if (AFP_Character* Character = Cast<AFP_Character>(ActorInfo->OwnerActor))
	{
		return Character->WeaponInventoryLength() > 0 && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	}
	return false;
}

void UGA_WeaponNext::SwapWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (IsLocallyControlled())
	{
		if (AFP_Character* Player = Cast<AFP_Character>(GetOwningActorFromActorInfo()))
		{
			ABWeapon* Weapon = Player->GetCurrentWeapon();
			if (Weapon)
			{
				if (UAnimMontage* Montage = Weapon->GetWeaponMesh1P()->GetAnimInstance()->GetCurrentActiveMontage())
				{
					const int32 SectionID = Montage->GetSectionIndex(FName("UnEquip"));
					if (Montage->IsValidSectionIndex(SectionID))
					{
						Weapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
					}
				}
			}

			Player->NextWeapon();
			Weapon = Player->GetCurrentWeapon();
			if (Weapon)
			{
				if (UAnimMontage* Montage = Weapon->GetWeaponMesh1P()->GetAnimInstance()->GetCurrentActiveMontage())
				{
					const int32 SectionID = Montage->GetSectionIndex(FName("Equip"));
					if (Montage->IsValidSectionIndex(SectionID))
					{
						Weapon->GetWeaponMesh1P()->GetAnimInstance()->Montage_JumpToSection("Equip");
						Weapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UGA_WeaponNext::WeaponEquipped);
						return;
					}
				}
			}
		}
	}
	WeaponEquipped(FName(), FBranchingPointNotifyPayload());
}

void UGA_WeaponNext::WeaponEquipped(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (IsLocallyControlled())
	{
		if (const AFP_Character* Player = Cast<AFP_Character>(GetOwningActorFromActorInfo()))
		{
			ABWeapon* Weapon = Player->GetCurrentWeapon();
			if (Weapon)
			{
				if (UAnimMontage* Montage = Weapon->GetWeaponMesh1P()->GetAnimInstance()->GetCurrentActiveMontage())
				{
					const int32 SectionID = Montage->GetSectionIndex("Equip");
					if (Montage->IsValidSectionIndex(SectionID))
					{
						Weapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
					}
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("UhohSpagehttiohs"));
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
