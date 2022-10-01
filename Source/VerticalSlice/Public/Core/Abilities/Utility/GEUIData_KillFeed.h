// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameplayEffectUIData.h"
#include "GEUIData_KillFeed.generated.h"

/**
 * UIData class to be used in Gameplay Effects
 * Used to have a KillFeed Icon tied to the Gameplay Effect
 */
UCLASS()
class VERTICALSLICE_API UGEUIData_KillFeed : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:
	UGEUIData_KillFeed(const FObjectInitializer& ObjectInitializer);

	// The Kill Feed Icon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	UTexture2D* KillFeedIcon;
};
