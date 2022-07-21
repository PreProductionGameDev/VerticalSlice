// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ProductionAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UProductionAssetManager : public UAssetManager
{
	GENERATED_BODY()

	UProductionAssetManager() {};

	virtual void StartInitialLoading() override;

	// Return Asset Manager Signleton
	static UProductionAssetManager& Get();
};
