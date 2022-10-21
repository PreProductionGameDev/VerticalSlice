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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class AGameMode>> GameModes;

	UFUNCTION(BlueprintCallable)
	TArray<TSubclassOf<class ABWeapon>> GetDefaultInventory();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TSubclassOf<class ABWeapon>> DefaultInventoryWeapons;

};



