// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Networking/GI_Multiplayer.h"

#include "DelayAction.h"
#include "Chaos/PBDCollisionConstraintsContact.h"
#include "Core/Gamemodes/Lobby/LobbyActor.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerState.h"
#include "Core/Networking/FServerData.h"
#include "Core/Player/FPPlayerController.h"
#include "Core/UI/Menus/ServerFinder.h"
#include "Core/UI/Menus/MainMenu.h"   
#include "GameFramework/InputSettings.h"

const static FName SESSION_NAME = TEXT("GameSession");
const static FName SERVER_GAME_MODE_SETTINGS_KEY = TEXT("ServerGameMode");
const static FName SERVER_MAP_SETTINGS_KEY = TEXT("ServerMap");
const static FName SERVER_PLAYER_COUNT = TEXT("ServerPlayer");

UGI_Multiplayer::UGI_Multiplayer(const FObjectInitializer& ObjectInitializer)
{
    const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
    }
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
    if(!IsValid(Settings) && Settings->PrimaryAction != FKey())
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
    SetBinding("Jump", Settings->Jump);
    SetBinding("PrimaryAction", Settings->PrimaryAction);
    SetBinding("SecondaryAction", Settings->SecondaryAction);
    SetBinding("Reload", Settings->Reload);
    SetBinding("UtilityAbility", Settings->UtilityAbility);
    SetBinding("SwapAbility", Settings->SwapAbility);
    SetBinding("SMG", Settings->SMG);
    SetBinding("Shotgun", Settings->Shotgun);
    SetBinding("Sniper", Settings->Sniper);
    SetBinding("AssaultRifle", Settings->AssaultRifle);
    SetBinding("RocketLauncher", Settings->RocketLauncher);
    SetBinding("NextWeapon", Settings->NextWeapon);
    SetBinding("OpenLeaderboard", Settings->OpenLeaderboard);
    SetBinding("PrevWeapon", Settings->PrevWeapon);
}

void UGI_Multiplayer::SetBinding(const FName Mapping, const FKey Binding) const
{
    UInputSettings* InputSettings = UInputSettings::GetInputSettings();
    TArray<FInputActionKeyMapping> KeyBinds;
    InputSettings->GetActionMappingByName(Mapping,KeyBinds);
    for(FInputActionKeyMapping KeyBind: KeyBinds)
    {
        InputSettings->RemoveActionMapping(KeyBind);
    }
    InputSettings->AddActionMapping(FInputActionKeyMapping(Mapping,Binding));
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
            
            SessionInterface->OnSessionSettingsUpdatedDelegates.AddUObject(this, &UGI_Multiplayer::OnSessionSettingsUpdate);
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
        auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
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

        if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
        {
            SessionSettings.bIsLANMatch = true;
        }
        else
        {
            SessionSettings.bIsLANMatch = false;
        }
        SessionSettings.NumPublicConnections = 8;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;
        SessionSettings.bUseLobbiesIfAvailable = true;
        SessionSettings.bAllowJoinInProgress = true;
        SessionSettings.Set(SERVER_GAME_MODE_SETTINGS_KEY, FString("CTF"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
        SessionSettings.Set(SERVER_MAP_SETTINGS_KEY, FString("Temple"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
        SessionSettings.Set(SERVER_PLAYER_COUNT, 1, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
        
        SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
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
        SessionInterface->DestroySession(SESSION_NAME);
    }
}

void UGI_Multiplayer::RefreshServerList(UServerFinder* InServerFinder)
{
    if (!InServerFinder)
    {
        return;
    }
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (SessionSearch.IsValid())
    {
        this->ServerFinder = InServerFinder;
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
        UE_LOG(LogTemp, Warning, TEXT("%d"), SessionSearch->SearchResults.Num());
        UE_LOG(LogTemp, Warning, TEXT("SessionSearch Completed"));

        TArray<FServerData> ServerNames;
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            FServerData Data;
            Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
            SearchResult.Session.SessionSettings.Get(SERVER_PLAYER_COUNT, Data.CurrentPlayers);
            Data.Name = SearchResult.Session.OwningUserName;
            SearchResult.Session.SessionSettings.Get(SERVER_GAME_MODE_SETTINGS_KEY,Data.CurrentGameModeInfo);
            SearchResult.Session.SessionSettings.Get(SERVER_MAP_SETTINGS_KEY,Data.CurrentMapInfo);
           
            ServerNames.Add(Data);
        }

        ServerFinder->SetServerList(ServerNames);
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

void UGI_Multiplayer::OnSessionSettingsUpdate(FName SessionName, const FOnlineSessionSettings& UpdatedSettings)
{
    
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

void UGI_Multiplayer::SetGameMode(FString GameMode)
{
    FOnlineSessionSettings* LocalSessionSettings = SessionInterface->GetSessionSettings(SESSION_NAME);
    
    LocalSessionSettings->Set(SERVER_GAME_MODE_SETTINGS_KEY, GameMode, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    
    SessionInterface->UpdateSession(SESSION_NAME,*LocalSessionSettings , true);
}

void UGI_Multiplayer::SetMap(FString Map)
{
    FOnlineSessionSettings* LocalSessionSettings = SessionInterface->GetSessionSettings(SESSION_NAME);
    
    LocalSessionSettings->Set(SERVER_MAP_SETTINGS_KEY, Map, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    
    SessionInterface->UpdateSession(SESSION_NAME,*LocalSessionSettings , true);

}

void UGI_Multiplayer::SetPlayers(int32 Players)
{
    FOnlineSessionSettings* LocalSessionSettings = SessionInterface->GetSessionSettings(SESSION_NAME);
    
    LocalSessionSettings->Set(SERVER_PLAYER_COUNT, Players, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    
    SessionInterface->UpdateSession(SESSION_NAME,*LocalSessionSettings , true);
}

FOnlineSessionSearchResult UGI_Multiplayer::GetSessionSearchResult(int32 Index)
{
    return SessionSearch->SearchResults[Index];
}

