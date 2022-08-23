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

#include "Core/Networking/NetworkInterface.h"
#include "GI_Multiplayer.generated.h"

/**
 *  Multiplayer Game Instance
 *  Handles Saving, Networking and More
 */
UCLASS()
class UGI_Multiplayer : public UGameInstance, public INetworkInterface
{
    GENERATED_BODY()

public:
    UGI_Multiplayer();
    
    // Settings
    UFUNCTION(BlueprintCallable)
    void SaveSettings();
    UFUNCTION(BlueprintCallable)
    void LoadSettings();

    // Colors
    UFUNCTION(BlueprintCallable)
    void StoreColors();
    UFUNCTION(BlueprintCallable)
    void StoreModels();

    // Menu Creation
    UFUNCTION(BlueprintCallable)
    void CreateMainMenuUI();
    
    UFUNCTION(BlueprintCallable)
    virtual void Init() override;
    UFUNCTION(BlueprintCallable)
    virtual void Shutdown() override;

    // Networking
    virtual void Host() override;
    UFUNCTION(BlueprintCallable)
    void Search();
    virtual void Join() override{};
    UFUNCTION(BlueprintCallable)
    void Join(const FString& Address);

    // Scoreboard Sorting
    UFUNCTION(BlueprintCallable)
    static TMap<FString, int32> SortScoreBoard(TMap<FString, int32> UnsortedMap);

    // Map to store all of the colors and mapped to the player names
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FVector> SavedColors;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> SavedModels;
    // Settings Saved Game Asset
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USettingsSaveGame* Settings;
    
private:
    IOnlineSessionPtr SessionInterface;
    TSharedPtr<class FOnlineSessionSearch> SessionSearch;
    
    FString MapLocation;
    // Creation of a Session
    void CreateSession();

    // Delegate for the Async Server creation 
    void OnCreateSessionComplete(FName SessionName, bool bSuccess);
    // Delegate for the Async Server Destruction
    void OnDestroySessionComplete(FName SessionName, bool bSuccess);
    // Delegate for the Async Server Search
    void OnFindSessionsComplete(bool bSuccess);


    // Menu System
    TSubclassOf<UUserWidget> MenuClass;
};
