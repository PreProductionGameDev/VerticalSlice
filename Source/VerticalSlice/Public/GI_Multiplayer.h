// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Core/Player/SettingsSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Templates/SharedPointer.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "GI_Multiplayer.generated.h"

/**
 *  Multiplayer Game Instance
 *  Handles Saving, Networking and More
 */
UCLASS()
class UGI_Multiplayer : public UGameInstance
{
    GENERATED_BODY()

public:
    UGI_Multiplayer();
    
    // Settings
    UFUNCTION(BlueprintCallable)
    void SaveSettings();
    UFUNCTION(BlueprintCallable)
    void LoadSettings();

    // Gets and saves the colors to the map 
    UFUNCTION(BlueprintCallable)
    void StoreColors();
    // Gets and saves the models to the map
    UFUNCTION(BlueprintCallable)
    void StoreModels();

    
    UFUNCTION(BlueprintCallable)
    virtual void Init() override;
    UFUNCTION(BlueprintCallable)
    virtual void Shutdown() override;

    // Networking
    ///Hosts a listen server
    ///Uses Location as the server command for game mode and map
    UFUNCTION(BlueprintCallable)
    void Host(const FString& Location);
    UFUNCTION(BlueprintCallable)
    void Search();
    /// connects a player to a server
    /// Uses Address as the server location
    UFUNCTION(BlueprintCallable)
    void Join(const FString& Address);

    // Scoreboard Sorting
    UFUNCTION(BlueprintCallable)
    static TMap<FString, int32> SortScoreBoard(TMap<FString, int32> UnsortedMap);

    // Map to store all of the colors and mapped to the player names
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FVector> SavedColors;
    // Map to store all of the players selected models
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> SavedModels;
    // Settings Saved Game Asset
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USettingsSaveGame* Settings;
    
private:
    IOnlineSessionPtr SessionInterface;
};
