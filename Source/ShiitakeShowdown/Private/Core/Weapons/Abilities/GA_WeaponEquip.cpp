// 2022 ChronoOwl Studios


#include "Core/Weapons/Abilities/GA_WeaponEquip.h"

#include "Core/Weapons/BWeapon.h"


UGA_WeaponEquip::UGA_WeaponEquip()
{
	bSourceObjectMustBeCurrentWeaponToActivate = true;
	bSourceObjectMustBeCurrentElementToActivate = false;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Equip")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Secondary")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Swap")));

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_WeaponEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const int32 SectionID = EquipMontage->GetSectionIndex(SectionName);
	if (EquipMontage->IsValidSectionIndex(SectionID))
	{
		if (const ABWeapon* SourceWeapon = Cast<ABWeapon>(GetCurrentSourceObject()))
		{
			SourceWeapon->GetWeaponMesh1P()->GetAnimInstance()->Montage_JumpToSection(SectionName);
			SourceWeapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UGA_WeaponEquip::MontageNotifyEvent);
		}
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UGA_WeaponEquip::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const int32 SectionID = EquipMontage->GetSectionIndex(SectionName);
	if (EquipMontage->IsValidSectionIndex(SectionID))
	{
		if (const ABWeapon* SourceWeapon = Cast<ABWeapon>(GetCurrentSourceObject()))
		{
			SourceWeapon->GetWeaponMesh1P()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UGA_WeaponEquip::MontageNotifyEvent);
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_WeaponEquip::MontageNotifyEvent(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	UE_LOG(LogTemp, Warning, TEXT("Notify Occured"));
	if (NotifyName == FName("Equip"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}
