// 2022 ChronoOwl Studios

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
class VERTICALSLICE_API UWeaponUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Check if the player should receive a Hit Marker for their attack 
	UFUNCTION(BlueprintCallable, Category="ShiitakeShowdown|Weapon")
	static void CheckHitMarker(const FGameplayAbilityTargetDataHandle& TargetData, AFP_Character* OwningPlayer);
};
