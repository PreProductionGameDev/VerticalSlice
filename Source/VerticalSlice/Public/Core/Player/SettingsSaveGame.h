// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API USettingsSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	USettingsSaveGame();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Audio")
	float MainVolume;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Audio")
	float SFXVolume;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Audio")
	float MusicVolume;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	float MouseSensitivity;
	
};
