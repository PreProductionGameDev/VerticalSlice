// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSaveGame.generated.h"

/**
 *	Saved Game Data
 *	Used for Settings
 */
UCLASS()
class VERTICALSLICE_API USettingsSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	USettingsSaveGame();

	// Volume Settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Audio")
	float MainVolume;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Audio")
	float SFXVolume;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Audio")
	float MusicVolume;

	// Gameplay Settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	float MouseSensitivity;
};
