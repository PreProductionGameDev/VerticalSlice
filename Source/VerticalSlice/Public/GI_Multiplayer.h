// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "../../../Plugins/Developer/RiderLink/Source/RD/src/rd_core_cpp/src/main/types/Void.h"
#include "Core/Player/SettingsSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Templates/SharedPointer.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

#include "GI_Multiplayer.generated.h"

/**
 *
 */
UCLASS()
class UGI_Multiplayer : public UGameInstance
{
    GENERATED_BODY()

public:
    UGI_Multiplayer();
    
    
    UFUNCTION(BlueprintCallable)
    void SaveSettings();

    UFUNCTION(BlueprintCallable)
    void LoadSettings();
    
    UFUNCTION(BlueprintCallable)
    void StoreColors();

    UFUNCTION(BlueprintCallable)
    void StoreModels();

    UFUNCTION(BlueprintCallable)
    virtual void Init() override;

    UFUNCTION(BlueprintCallable)
    virtual void Shutdown() override;

    UFUNCTION(BlueprintCallable)
    void Host(const FString& Location);

    UFUNCTION(BlueprintCallable)
    void Search();

    UFUNCTION(BlueprintCallable)
    void Join(const FString& Address);

    UFUNCTION(BlueprintCallable)
    static TMap<FString, int32> SortScoreBoard(TMap<FString, int32> UnsortedMap);

    //map to store all of the colors and maped to the player names
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FVector> SavedColors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> SavedModels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USettingsSaveGame* Settings;

    
private:

    IOnlineSessionPtr SessionInterface;
};
