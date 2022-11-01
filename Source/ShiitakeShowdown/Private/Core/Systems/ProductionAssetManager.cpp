// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/ProductionAssetManager.h"
#include "AbilitySystemGlobals.h"

void UProductionAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	UAbilitySystemGlobals::Get().InitGlobalData();
}

UProductionAssetManager& UProductionAssetManager::Get()
{
	UProductionAssetManager* AssetManager = Cast<UProductionAssetManager>(GEngine->AssetManager);

	if (AssetManager)
	{
		return *AssetManager;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, Must set this to ProductionAssetManager"))
		return *NewObject<UProductionAssetManager>();
	}
}
