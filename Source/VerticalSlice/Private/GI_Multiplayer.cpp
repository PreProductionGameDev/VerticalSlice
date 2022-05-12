// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Multiplayer.h"



UGI_Multiplayer::UGI_Multiplayer()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
    }
}

void UGI_Multiplayer::Init()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
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