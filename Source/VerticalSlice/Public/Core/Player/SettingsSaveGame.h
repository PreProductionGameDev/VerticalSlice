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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custome Save|Controls")
	bool bHoldToZoom;

	//Graphics settings
	FIntPoint Resolution = FIntPoint(1920,1080);
	
	// Cosmetics Settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Cosmetics")
	int32 PlayerModel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Cosmetics")
	TArray<float> PrimaryHue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Cosmetics")
	TArray<float> SecondaryHue;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Cosmetics")
	TArray<FName> HeadItem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Cosmetics")
	TArray<FName> BodyItem;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Cosmetics")
	TArray<FName> ExtraItem;

	// Control Settings
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	//Key;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey Jump;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey PrimaryAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey SecondaryAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey Reload;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey SMG;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey Shotgun;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey Sniper;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey AssaultRifle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey RocketLauncher;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey NextWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey PrevWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey SwapAbility;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey UtilityAbility;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey MovementAbility;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Save|Controls")
	FKey OpenLeaderboard;
};
