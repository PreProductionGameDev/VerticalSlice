// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Core/Data.h"
#include "FElementInfo.generated.h"

/** 
 * Data Struct for holding element information
 */

USTRUCT(BlueprintType)
struct FElementInfo
{
 GENERATED_BODY()
 
 // Primary ability 
 UPROPERTY(BlueprintReadWrite, EditAnywhere)
 EPlayerAbilityInputID Primary = EPlayerAbilityInputID::UtilityAbility;
 // Primary ability 
 UPROPERTY(BlueprintReadWrite, EditAnywhere)
 EPlayerAbilityInputID Movement = EPlayerAbilityInputID::MovementAbility;
 // Ability HUD component
 UPROPERTY(BlueprintReadWrite, EditAnywhere)
 UTexture2D* Texture = nullptr;
};