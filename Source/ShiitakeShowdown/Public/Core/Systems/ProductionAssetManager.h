// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ProductionAssetManager.generated.h"

/**
 *	Custom Asset Manager Class
 *	Required for initializing the AbilitySystem Globals
 */
UCLASS()
class SHIITAKESHOWDOWN_API UProductionAssetManager : public UAssetManager
{
	GENERATED_BODY()

	UProductionAssetManager() {};
	// Initial Loading of the Asset Manager
	virtual void StartInitialLoading() override;
	// Return Asset Manager Singleton
	static UProductionAssetManager& Get();
};
