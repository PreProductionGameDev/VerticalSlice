// 2022 ChronoOwl Studios


#include "Core/Weapons/WeaponUtilityFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

/**
 * @name Stefan
 * @brief Creates a Hit marker on the player, ensures they arent same team and damage is actually dealt
 * @param TargetData The Data Handle containing the hit result
 * @param OwningPlayer The Player tried to deal damage
 */
void UWeaponUtilityFunctionLibrary::CheckHitMarker(const FGameplayAbilityTargetDataHandle& TargetData, AFP_Character* OwningPlayer)
{
	const FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);

	// Get And Validate the Hit Player and their ASC
	AFP_Character* HitPlayer = Cast<AFP_Character>(HitResult.GetActor());
	if (!HitPlayer) return;
	const UAbilitySystemComponent* HitPlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitPlayer);
	if (!HitPlayerASC) return;

	// Get the Source Player's ASC
	const UAbilitySystemComponent* SourcePlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPlayer);
	if (!SourcePlayerASC) return;

	// Return if the Player is dead
	if (HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) return;

	// Return if both players are Blue team
	if (HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Blue"))) &&
		SourcePlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Blue"))))
			return;

	// Return if both players are Red Team
	if (HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Red"))) &&
		SourcePlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Red"))))
			return;

	// Check if player has entered the dying state
	const bool HitPlayerIsDying = HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dying")));
	// Create Hit Marker on the owning player
	OwningPlayer->CreateHitMarker(HitPlayerIsDying);
}
