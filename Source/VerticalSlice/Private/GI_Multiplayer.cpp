// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Multiplayer.h"

#include "Core/Gamemodes/Lobby/LobbyActor.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"


UGI_Multiplayer::UGI_Multiplayer()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
    }
}

void UGI_Multiplayer::StoreColors()
{
    //gets all of the lobby actors
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(this, ALobbyActor::StaticClass(), Actors);

    //map to store all of the colors and maped to the player names
    TMap<FString, FVector> SavedColors;

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

void UGI_Multiplayer::Init()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
        SessionInterface = Subsystem->GetSessionInterface();
    }
}

void UGI_Multiplayer::Host(const FString& Location)
{
    UE_LOG(LogTemp, Warning, TEXT("Started Host"));
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;
    UE_LOG(LogTemp, Warning, TEXT("Got Engine"));

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;
    UE_LOG(LogTemp, Warning, TEXT("Got World"));

    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = false;
        SessionSettings.NumPublicConnections = 8;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;
        SessionSettings.bUseLobbiesIfAvailable = true;
        SessionSettings.bAllowJoinInProgress = true;

        SessionInterface->DestroySession(TEXT("RandomTempName42"));
        SessionInterface->CreateSession(0, TEXT("RandomTempName42"), SessionSettings);
    }

    World->ServerTravel(Location);
}

void UGI_Multiplayer::Search()
{

}

void UGI_Multiplayer::Join(const FString& Address)
{
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UGI_Multiplayer::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
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
