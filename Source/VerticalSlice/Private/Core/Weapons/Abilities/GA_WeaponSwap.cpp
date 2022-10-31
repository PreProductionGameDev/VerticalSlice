// 2022 ChronoOwl Studios


#include "Core/Weapons/Abilities/GA_WeaponSwap.h"

#include "FP_Character.h"

UGA_WeaponSwap::UGA_WeaponSwap()
{
	// Default Input
	AbilityInputID = EPlayerAbilityInputID::SMG;
	AbilityID = EPlayerAbilityInputID::SMG;
	bSourceObjectMustBeCurrentWeaponToActivate = false;
	bSourceObjectMustBeCurrentElementToActivate = false;

	// Ability Tags
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.SMG")));
	
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.SMG")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.Shotgun")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.Sniper")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.BurstRifle")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.Rocket")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Secondary")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Swap")));
	
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary.Instant")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Secondary")));
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dying")));

	// Replication Settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UGA_WeaponSwap::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("OWO WHATS TIS?"));
	if (IsLocallyControlled())
	{
		if (const ABWeapon* Weapon = Cast<ABWeapon>(GetCurrentSourceObject()))
		{
			if (AFP_Character* Player =  Cast<AFP_Character>(Weapon->GetOwner()))
			{
				if (Player->GetCurrentWeapon())
				{
					if (UAnimMontage* Montage = Player->GetCurrentWeapon()->GetWeaponMesh1P()->GetAnimInstance()->GetCurrentActiveMontage())
					{
						const int32 SectionID = Montage->GetSectionIndex(FName("UnEquip"));
						if (Montage->IsValidSectionIndex(SectionID))
						{
							UE_LOG(LogTemp, Warning, TEXT("UN EQUIP THIS"));
							Player->GetCurrentWeapon()->GetWeaponMesh1P()->GetAnimInstance()->Montage_JumpToSection("UnEquip");
							Player->GetCurrentWeapon()->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UGA_WeaponSwap::SwapWeapon);
							return;
						}
					}
				}
			}
		}
	}
	SwapWeapon(FName(), FBranchingPointNotifyPayload());
}

void UGA_WeaponSwap::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_WeaponSwap::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AFP_Character* Character = Cast<AFP_Character>(ActorInfo->AvatarActor))
	{
		return (Character->DoesWeaponTagExistInInventory(WeaponTag) && !Character->IsWeaponTagCurrentlyEquipped(WeaponTag)) && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	}
	return false;
}

void UGA_WeaponSwap::SwapWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (IsLocallyControlled())
	{
		if (const ABWeapon* Weapon = Cast<ABWeapon>(GetCurrentSourceObject()))
		{
			if (UAnimMontage* Montage = Weapon->GetWeaponMesh1P()->GetAnimInstance()->GetCurrentActiveMontage())
			{
				const int32 SectionID = Montage->GetSectionIndex(FName("UnEquip"));
				if (Montage->IsValidSectionIndex(SectionID))
				{
					Weapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
				}
			}
			
			if (AFP_Character* Character = Cast<AFP_Character>(Weapon->GetOwner()))
			{
				Character->EquipWeaponFromTag(WeaponTag);
				Weapon = Character->GetCurrentWeapon();
				if (Weapon)
				{
					if (UAnimMontage* Montage = Weapon->GetWeaponMesh1P()->GetAnimInstance()->GetCurrentActiveMontage())
					{
						const int32 SectionID = Montage->GetSectionIndex(FName("Equip"));
						if (Montage->IsValidSectionIndex(SectionID))
						{
							//Weapon->GetWeaponMesh1P()->GetAnimInstance()->Montage_JumpToSection("Equip");
							Weapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UGA_WeaponSwap::WeaponEquipped);
							return;
						}
					}
				}
			}
		}
	}
	WeaponEquipped(FName(), FBranchingPointNotifyPayload());
}

void UGA_WeaponSwap::WeaponEquipped(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (const ABWeapon* Weapon = Cast<ABWeapon>(GetCurrentSourceObject()))
	{
		if (UAnimMontage* Montage = Weapon->GetWeaponMesh1P()->GetAnimInstance()->GetCurrentActiveMontage())
		{
			const int32 SectionID = Montage->GetSectionIndex(FName("Equip"));
			if (Montage->IsValidSectionIndex(SectionID))
			{
				Weapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();
			}
		}
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
