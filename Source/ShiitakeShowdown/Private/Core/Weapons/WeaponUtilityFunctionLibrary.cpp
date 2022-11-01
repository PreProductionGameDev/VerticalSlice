// 2022 ChronoOwl Studios
// Stefan Petrie

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
	for (int i = 0; i < UAbilitySystemBlueprintLibrary::GetDataCountFromTargetData(TargetData); i++)
	{
		const FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, i);

		// Get And Validate the Hit Player and their ASC
		AFP_Character* HitPlayer = Cast<AFP_Character>(HitResult.GetActor());
		if (!HitPlayer) continue;
		const UAbilitySystemComponent* HitPlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitPlayer);
		if (!HitPlayerASC) continue;

		// Get the Source Player's ASC
		const UAbilitySystemComponent* SourcePlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPlayer);
		if (!SourcePlayerASC) continue;

		// Return if the Player is dead
		if (HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead"))))
			continue;

		// Return if both players are Blue team
		if (HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Blue"))) &&
			SourcePlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Blue"))))
				continue;

		// Return if both players are Red Team
		if (HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Red"))) &&
			SourcePlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Red"))))
				continue;

		// Check if player has entered the dying state
		const bool HitPlayerIsDying = HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dying")));
		// Create Hit Marker on the owning player
		OwningPlayer->CreateHitMarker(HitPlayerIsDying);
	}
}

void UWeaponUtilityFunctionLibrary::CheckHitMarkerWithActors(AFP_Character* HitPlayer, AFP_Character* OwningPlayer)
{
	if (!HitPlayer)
		return;
	const UAbilitySystemComponent* HitPlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitPlayer);
	if (!HitPlayerASC)
		return;

	// Get the Source Player's ASC
	const UAbilitySystemComponent* SourcePlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPlayer);
	if (!SourcePlayerASC)
		return;

	// Return if the Player is dead
	if (HitPlayerASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead"))))
		return;

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
