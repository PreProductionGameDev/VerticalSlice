// 2022 ChronoOwl Studios
// Stefan Petrie

#pragma once

#include "CoreMinimal.h"
#include "FP_Character.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WeaponUtilityFunctionLibrary.generated.h"

/**
 *	Utility functions to be used within the Weapon Space
 */
UCLASS()
class SHIITAKESHOWDOWN_API UWeaponUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Check if the player should receive a Hit Marker for their attack 
	UFUNCTION(BlueprintCallable, Category="ShiitakeShowdown|Weapon")
	static void CheckHitMarker(const FGameplayAbilityTargetDataHandle& TargetData, AFP_Character* OwningPlayer);

	UFUNCTION(BlueprintCallable, Category = "ShiitakeShowdown|Weapon")
	static void CheckHitMarkerWithActors(AFP_Character* HitPlayer, AFP_Character* OwningPlayer);
};
