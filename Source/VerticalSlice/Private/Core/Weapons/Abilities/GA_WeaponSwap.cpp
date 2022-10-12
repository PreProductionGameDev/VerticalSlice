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
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Reload")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Primary.Instant")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Secondary")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.Equip")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dying")));

	// Replication Settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_WeaponSwap::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (AFP_Character* Character = Cast<AFP_Character>(GetOwningActorFromActorInfo()))
	{
		Character->EquipWeaponFromTag(WeaponTag);
		FGameplayTagContainer EquipContainer;
		EquipContainer.AddTag(FGameplayTag::RequestGameplayTag("Ability.Weapon.Equip"));
		GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(EquipContainer);
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_WeaponSwap::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_WeaponSwap::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AFP_Character* Character = Cast<AFP_Character>(GetOwningActorFromActorInfo()))
	{
		return Character->DoesWeaponTagExistInInventory(WeaponTag) && !Character->IsWeaponTagCurrentlyEquipped(WeaponTag);
	}
	return false;
}
