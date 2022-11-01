// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "FWeaponInventory.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponInventory
{
 GENERATED_BODY();
 
 UPROPERTY(BlueprintReadWrite, EditAnywhere)
 FGameplayTag Key;
 UPROPERTY(BlueprintReadWrite, EditAnywhere)
 class ABWeapon* Weapon;

public:
 FWeaponInventory()
 {
  Weapon = nullptr;
 }
 
 FWeaponInventory(FGameplayTag inKey, ABWeapon* inWeapon)
 {
  Key = inKey;
  Weapon = inWeapon;
 }

 bool operator==(const FGameplayTag inKey) const
 {
  return Key == inKey;
 }
};
