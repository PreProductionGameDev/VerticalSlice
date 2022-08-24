// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Networking/GI_Multiplayer.h"

#include "Core/Gamemodes/Lobby/LobbyActor.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerState.h"
#include "Core/Networking/FServerData.h"
#include "Core/Player/FPPlayerController.h"
#include "Core/UI/Menus/MainMenu.h"   

const static FName SESSION_NAME = TEXT("HUHWHAT");

UGI_Multiplayer::UGI_Multiplayer(const FObjectInitializer& ObjectInitializer)
{
    const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
    }

    const ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/ShiitakeSorcerers/UI/Networking/WB_MainMenu"));
    if (!ensure(MenuBPClass.Class != nullptr))
    {
        return;
    }

    MenuClass = MenuBPClass.Class;
}

void UGI_Multiplayer::SaveSettings()
{
    if(!UGameplayStatics::SaveGameToSlot(Settings, "Settings", 0 ))
    {
        //warns the client that saving failed
        UE_LOG(LogTemp, Warning, TEXT("Settings have failed to save"));
    }
}

void UGI_Multiplayer::LoadSettings()
{
    //Attempts to load settings
    Settings = Cast<USettingsSaveGame>(UGameplayStatics::LoadGameFromSlot("Settings",0));
    if(!IsValid(Settings))
    {
        //Creates new settings if it failed to load
        UE_LOG(LogTemp, Warning, TEXT("Settings have failed to load"));
        Settings = NewObject<USettingsSaveGame>();
        if(!UGameplayStatics::SaveGameToSlot(Settings, "Settings", 0 ))
        {
            //crashes if game failed to create default settings
            UE_LOG(LogTemp, Fatal, TEXT("Settings have failed to initalize"));
        }
    }
}

void UGI_Multiplayer::StoreColors()
{
    //gets all of the lobby actors
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(this, ALobbyActor::StaticClass(), Actors);

    //go through the array and match each user name to the player color and add it to the map
    for(AActor* Actor: Actors)
    {
        APlayerController* Controller = Cast<APlayerController>(Actor->GetOwner());
        if(Controller!= nullptr)
        {
            ALobbyActor* LobbyActor = Cast<ALobbyActor>(Actor);
            SavedColors.Add(Controller->PlayerState->GetPlayerName(), FVector( LobbyActor->DoRep_Hue1, LobbyActor->DoRep_Hue2, LobbyActor->DoRep_Hue3));
        }
    }
}

void UGI_Multiplayer::StoreModels()
{
    //gets all of the lobby actors
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(this, ALobbyActor::StaticClass(), Actors);
    
    //go through the array and match each user name to the player color and add it to the map
    for(AActor* Actor: Actors)
    {
        APlayerController* Controller = Cast<APlayerController>(Actor->GetOwner());
        if(Controller!= nullptr)
        {
            ALobbyActor* LobbyActor = Cast<ALobbyActor>(Actor);
            SavedModels.Add(Controller->PlayerState->GetPlayerName(), LobbyActor->DoRep_CurrentMesh);
        }
    }
}

void UGI_Multiplayer::CreateMainMenuUI()
{
    if (!ensure(MenuClass != nullptr))
    {
        return;
    }
    // Create and Add the Widget to the Viewport
    Menu = CreateWidget<UMainMenu>(this, MenuClass);
    if (!ensure(Menu != nullptr))
    {
        return;
    }
    Menu->AddToViewport();

    // Setup Input for the Player Controller
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr))
    {
        return;
    }

    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(Menu->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;

    Menu->SetNetworkInterface(this);
}

void UGI_Multiplayer::Init()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
        SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGI_Multiplayer::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGI_Multiplayer::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGI_Multiplayer::OnFindSessionsComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGI_Multiplayer::OnJoinSessionComplete);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Subsystem Found"));
    }
    LoadSettings();
    Super::Init();
}

void UGI_Multiplayer::Shutdown()
{
    Super::Shutdown();
}

void UGI_Multiplayer::Host()
{
    UE_LOG(LogTemp, Warning, TEXT("Started Host"));
    // Validate the Engine
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr))
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Got Engine"));

    // Validate the World
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr))
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Got World"));

    // Set the Lobby Travel Location
    MapLocation = TEXT("/Game/ShiitakeSorcerers/Maps/LobbyScreenMockUp_P?listen");
    if (SessionInterface.IsValid())
    {
        auto ExistingSession = SessionInterface->GetNamedSession(TEXT("HUHWHAT"));
        if (ExistingSession != nullptr)
        {
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            CreateSession();
        }
    }
}

void UGI_Multiplayer::CreateSession()
{
    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = false;
        SessionSettings.NumPublicConnections = 8;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;
        SessionSettings.bUseLobbiesIfAvailable = true;
        SessionSettings.bAllowJoinInProgress = true;
        SessionSettings.bAllowInvites = true;
        
        SessionInterface->CreateSession(0, TEXT("HUHWHAT"), SessionSettings);
    }    
}

void UGI_Multiplayer::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
    // Failure to Create the Session
    if (!bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failure to Create Session"));
        return;
    }

    if (Menu != nullptr)
    {
        Menu->RemoveUI();
    }

    // Engine Check
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr))
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Got Engine"));

    // World Check
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr))
    {
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Got World"));

    // Server Travel
    UE_LOG(LogTemp, Warning, TEXT("%s"), *MapLocation);
    World->ServerTravel(MapLocation);
}

void UGI_Multiplayer::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
    if (bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("PREVIOUS SESSION DESTROYED. NEW ONE MADE"));
        CreateSession();
    }
}

void UGI_Multiplayer::RefreshServerList()
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (SessionSearch.IsValid())
    {
        //SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
        UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}

void UGI_Multiplayer::OnFindSessionsComplete(bool bSuccess)
{
    if (bSuccess && SessionSearch.IsValid() && Menu != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionSearch Completed"));

        TArray<FServerData> ServerNames;
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            FServerData Data;
            Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
            Data.CurrentPlayers = SearchResult.Session.NumOpenPublicConnections;
            Data.Name = SearchResult.Session.OwningUserName;
            ServerNames.Add(Data);
        }

        Menu->SetServerList(ServerNames);
    }
}

void UGI_Multiplayer::Join(uint32 Index)
{
    if (!SessionInterface.IsValid())
    {
        return;
    }

    if (!SessionSearch.IsValid())
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("JOINING"));
    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UGI_Multiplayer::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!SessionInterface.IsValid())
    {
        return;
    }

    if (Menu != nullptr)
    {
        Menu->RemoveUI();
    }
    
    FString Address;
    if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
    {
        UE_LOG(LogTemp, Warning, TEXT("COULD NOT CONNECT STRING"));
        return;
    }
    
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr))
    {
        return;
    }

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr))
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("CLIENT TRAVEL?"));
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

TMap<FString, int32> UGI_Multiplayer::SortScoreBoard(TMap<FString, int32> UnsortedMap)
{
    TMap<FString, int32> SortedMap;
    int Loops = UnsortedMap.Num();
    for(int i = 0; i < Loops ; i++)
    {
        int32 Max = -1;
        TTuple<FString, int> TopScore;
        for (auto Map : UnsortedMap)
        {
            if(Max < Map.Value)
            {
                TopScore = Map;
                Max = Map.Value;
            }
        }
        UnsortedMap.Remove(TopScore.Key);
        SortedMap.Add(TopScore);
    }
    return SortedMap;
    
}

