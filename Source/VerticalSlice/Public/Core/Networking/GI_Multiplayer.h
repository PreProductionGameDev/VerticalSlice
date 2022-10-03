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
    UGI_Multiplayer(const FObjectInitializer& ObjectInitializer);
    
    // Settings
    UFUNCTION(BlueprintCallable)
    void SaveSettings();
    UFUNCTION(BlueprintCallable)
    void LoadSettings();

    // Menu Creation
    UFUNCTION(BlueprintCallable)
    void CreateMainMenuUI();
    
    UFUNCTION(BlueprintCallable)
    virtual void Init() override;
    UFUNCTION(BlueprintCallable)
    virtual void Shutdown() override;

    // Networking
    // Host a new Session
    virtual void Host() override;
    // Join the Server from the index
    virtual void Join(uint32 Index) override;
    // Refresh the Server List
    virtual void RefreshServerList(class UServerFinder* InServerFinder) override;

    // Scoreboard Sorting
    UFUNCTION(BlueprintCallable)
    static TMap<FString, int32> SortScoreBoard(TMap<FString, int32> UnsortedMap);
    
    // Settings Saved Game Asset
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USettingsSaveGame* Settings;
    
protected:
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
    // Delegate for the Async Server Join
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    UPROPERTY()
    class UServerFinder* ServerFinder;
    
    // Menu System
    // The WidgetClass
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    TSubclassOf<UUserWidget> MenuClass;
    // The Menu Instance
    class UMainMenu* Menu;
};
