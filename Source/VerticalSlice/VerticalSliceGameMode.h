// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VerticalSliceGameMode.generated.h"

UCLASS(minimalapi)
class AVerticalSliceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVerticalSliceGameMode();

	// Function for the server to move to a new map
	UFUNCTION(BlueprintCallable)
	void ServerTravel(FString Map);

};



