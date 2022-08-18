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
 EPlayerAbilityInputID Primary;
 // Primary ability 
 UPROPERTY(BlueprintReadWrite, EditAnywhere)
 EPlayerAbilityInputID Movement;
 // Ability HUD component
 UPROPERTY(BlueprintReadWrite, EditAnywhere)
 UTexture2D* Texture;
};